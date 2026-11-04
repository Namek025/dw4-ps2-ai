#!/usr/bin/env python3
"""Unattended decompilation: Claude Code matches functions one at a time.

For each pending function (an INCLUDE_ASM left in an MWCC unit under src/), runs
`claude -p` with a fixed task, then checks the result itself:
  - only the unit's source, include/ (not meta.h) or config/symbol_addrs.txt changed;
  - `ninja` passes (the rom SHA-1 still matches);
  - INCLUDE_ASM gone           -> commit "match: FUNC (unit)";
  - INCLUDE_ASM under #else of
    an #ifdef NON_MATCHING     -> commit "nonmatching: FUNC (unit)";
  - anything else              -> tracked changes are reverted.
Nothing is pushed. Each attempt is logged to .agent/log.jsonl; the agent's last
message is kept in .agent/runs/FUNC.txt and its whole transcript in .agent/runs/FUNC.jsonl.

The prompt carries the function's asm, m2c's draft and the INCLUDE_ASM line, so the
agent reads only that part of the unit (some units are 100+ KB).
--batch N sends up to N small functions (<= --batch-max bytes) of the same unit,
consecutive in the queue, in one call: one commit per batch ("match: A B; nonmatching: C").

Needs the full environment (WSL, asm/, target rom, MWCC) and a clean git tree
(untracked files are ignored). If .venv/ exists its bin/ goes first in PATH, so
the agent's `python3` has m2c.

Usage:
  python3 tools/agent_loop.py queue  [--unit U] [--max-size B] [--retry]
  python3 tools/agent_loop.py run    [-n N] [--unit U] [--max-size B] [--retry]
                                     [--turns T] [--model M] [--timeout MIN] [--dry-run]
                                     [--batch N] [--batch-max B]
  python3 tools/agent_loop.py report

--unit U      only src/U.* (e.g. beNDMWGameRam, lua/lvm)
--max-size B  only functions up to B bytes
--retry       also functions already NON_MATCHING or reverted in an earlier run
--full-context  load CLAUDE.md, memory, MCP servers and skills as an interactive
              session does. By default the agent gets only the parts of CLAUDE.md it
              needs (LOOP_SECTIONS) through --append-system-prompt: ~12K tokens of
              fixed context per turn instead of ~37K.
"""
import argparse
import json
import os
import re
import signal
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
LOG_DIR = ROOT / ".agent"
LOG = LOG_DIR / "log.jsonl"
LUA_DIR = Path("src/lua")

INCLUDE_RE = re.compile(r'^INCLUDE_ASM\("([^"]+)", (\w+)\);', re.M)

ALLOWED_TOOLS = [
    "Read", "Edit", "MultiEdit", "Grep", "Glob",
    "Bash(ninja:*)",
    "Bash(python3 tools/m2c_func.py:*)",
    "Bash(python3 tools/fdist.py:*)",
    "Bash(python3 tools/fcmp.py:*)",
    "Bash(python3 tools/fcov.py:*)",
    "Bash(python3 tools/permute_decls.py:*)",
    "Bash(python3 tools/xref.py:*)",
    "Bash(python3 tools/asm_range.py:*)",
    "Bash(git diff:*)",
    "Bash(grep:*)",
]

# Lean context: only these CLAUDE.md sections go to the agent (from the "## " heading to
# the next one), minus the bullets about tools it doesn't use.
LOOP_SECTIONS = ["## Conventions", "## MWCC matching tricks"]
LOOP_SKIP_BULLETS = ("- Mods in C++", "- Progress and cutting", "- Meta boilerplate",
                     "- Moving asm", "- Data:", "- **decomp-permuter**")
LEAN_FLAGS = ["--setting-sources", "", "--strict-mcp-config", "--disable-slash-commands",
              "--tools", "Read", "Edit", "Grep", "Glob", "Bash"]
LOOP_HEADER = """\
Project: matching decompilation of Digimon World 4 (PS2). Game and Alchemy engine code
is C++ built with Metrowerks MWCC 3.0b52 (flags per path in configure.py: game -O4,p,
Alchemy -O4,s); the build compiles each unit and checks the rom SHA-1. C89 style:
declarations at block start. A trick that fixed a function and isn't in the list below:
mention it in your final message. What follows is from the project's CLAUDE.md.
"""


def loop_context():
    out, keep, skip = [LOOP_HEADER], False, False
    for line in (ROOT / "CLAUDE.md").read_text().splitlines():
        if line.startswith("## "):
            keep = any(line.startswith(s) for s in LOOP_SECTIONS)
            skip = False
        elif line.startswith("- "):
            skip = line.startswith(LOOP_SKIP_BULLETS)
        elif not line.startswith("  "):
            skip = False
        if keep and not skip:
            out.append(line)
    return "\n".join(out)


PROMPT = """\
Decompile {what} in `{src}` so it compiles byte for byte to the original.
The system prompt has the compiler, flags and the MWCC matching tricks: follow them.

{src} is {kb} KB: never Read it whole. Read around the lines below (offset/limit) and
Grep it for the types and inlines you need. Don't search include/meta.h with Read: Grep it.

{funcs}
1. Start from the m2c draft above. Use the types in include/ (meta.h, alchemy.h, game.h)
   and the style of the functions already decompiled near that line in {src}.
2. Replace the line `INCLUDE_ASM("{asmdir}", FUNC);` with the function. You may add the
   declarations or local types it needs in the same file.
3. Build and compare: `ninja {obj}` then `python3 tools/fdist.py {obj} FUNC -v`
   (distance 0 = match; `python3 tools/fcmp.py {obj} FUNC` shows both listings).
4. s-registers permuted: mark the local declarations with /*P*/ and run
   `python3 tools/permute_decls.py {src} FUNC`; remove the markers after.
5. At distance 0 run `ninja` (whole build, must pass) and stop.

If you can't reach a match, keep your closest attempt like this, and check `ninja` passes:
#ifdef NON_MATCHING
/* Nonmatching: <one line on what still differs> */
<your function>
#else
INCLUDE_ASM("{asmdir}", FUNC);
#endif

Rules: don't change other functions, include/meta.h (generated), config/splat.yaml,
configure.py, tools/ or asm/. Edit other include/ headers only for a missing type, and
then `ninja` must still pass. Don't run git commands that change anything.
End with one line{end}: MATCH, NONMATCHING <distance> or GIVEUP <reason>.
"""

FUNC_BLOCK = """\
### `{func}` ({size} bytes), `INCLUDE_ASM` at {src}:{line}
asm:
```
{asm}
```
m2c draft:
```
{m2c}
```
"""

MAX_ASM_LINES = 300


def sh(cmd, **kw):
    return subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True, **kw)


def units():
    """MWCC units: C++ sources plus Lua (.c built as C++). GCC units are left out."""
    for p in sorted((ROOT / "src").rglob("*.c*")):
        rel = p.relative_to(ROOT)
        if rel.suffix == ".cpp" or LUA_DIR in rel.parents:
            yield rel


def func_size(asmdir, func):
    p = ROOT / asmdir / f"{func}.s"
    if not p.exists():
        return None
    m = re.search(r"nonmatching \S+, (0x[0-9A-Fa-f]+)", p.read_text())
    return int(m.group(1), 16) if m else None


def func_status(src, func):
    """'todo', 'nonmatching' or 'done' for one function of a unit."""
    text = (ROOT / src).read_text()
    for m in INCLUDE_RE.finditer(text):
        if m.group(2) != func:
            continue
        before = text[:m.start()].rstrip().splitlines()
        return "nonmatching" if before and before[-1].strip().startswith("#else") else "todo"
    return "done"


def last_results():
    out = {}
    if LOG.exists():
        for line in LOG.read_text().splitlines():
            e = json.loads(line)
            out[e["func"]] = e["result"]
    return out


def pending(args):
    tried = last_results()
    items = []
    for src in units():
        if args.unit and src.with_suffix("").as_posix() != f"src/{args.unit}":
            continue
        for m in INCLUDE_RE.finditer((ROOT / src).read_text()):
            asmdir, func = m.groups()
            status = func_status(src, func)
            if not args.retry and (status == "nonmatching" or tried.get(func) == "reverted"):
                continue
            size = func_size(asmdir, func)
            if args.max_size and (size is None or size > args.max_size):
                continue
            items.append({"src": str(src), "asmdir": asmdir, "func": func,
                          "size": size, "status": status})
    items.sort(key=lambda i: (i["size"] is None, i["size"] or 0, i["func"]))
    return items


def changed_files():
    out = sh(["git", "status", "--porcelain", "--untracked-files=no"]).stdout
    return [l[3:].strip() for l in out.splitlines() if l.strip()]


def allowed_change(path, src):
    p = Path(path)
    if p == Path(src) or p == Path("config/symbol_addrs.txt"):
        return True
    return p.parts[0] == "include" and p.name != "meta.h"


def asm_text(item):
    p = ROOT / item["asmdir"] / f"{item['func']}.s"
    lines = p.read_text().splitlines() if p.exists() else ["(asm not found)"]
    if len(lines) > MAX_ASM_LINES:
        lines = lines[:MAX_ASM_LINES] + [f"... ({len(lines) - MAX_ASM_LINES} more lines in {p.relative_to(ROOT)})"]
    return "\n".join(lines)


def m2c_text(item, env):
    try:
        r = subprocess.run(["python3", "tools/m2c_func.py", item["func"]], cwd=ROOT,
                           capture_output=True, text=True, timeout=120, env=env)
        out = r.stdout.strip() or r.stderr.strip()[-1000:]
    except subprocess.TimeoutExpired:
        out = "(m2c timed out)"
    lines = out.splitlines()
    if len(lines) > MAX_ASM_LINES:
        out = "\n".join(lines[:MAX_ASM_LINES] + ["... (truncated; run tools/m2c_func.py for all of it)"])
    return out


def include_line(item):
    text = (ROOT / item["src"]).read_text()
    for m in INCLUDE_RE.finditer(text):
        if m.group(2) == item["func"]:
            return text.count("\n", 0, m.start()) + 1
    return "?"


def build_prompt(batch, env):
    first = batch[0]
    obj = f"build/{Path(first['src']).with_suffix('.mw.o')}"
    funcs = "".join(FUNC_BLOCK.format(func=i["func"], size=i["size"] or "?", src=i["src"],
                                      line=include_line(i), asm=asm_text(i), m2c=m2c_text(i, env))
                    for i in batch)
    if len(batch) == 1:
        what = f"`{first['func']}` ({first['size'] or '?'} bytes)"
        end = ""
    else:
        what = (f"these {len(batch)} functions, one after another, "
                + ", ".join(f"`{i['func']}`" for i in batch)
                + " (FUNC below = each of them)")
        end = " per function (`FUNC: MATCH`, ...)"
    kb = (ROOT / first["src"]).stat().st_size // 1024
    return PROMPT.format(what=what, src=first["src"], kb=kb, funcs=funcs,
                         asmdir=first["asmdir"], obj=obj, end=end)


def agent_env():
    env = dict(os.environ)
    venv = ROOT / ".venv" / "bin"
    if venv.is_dir():
        env["PATH"] = f"{venv}{os.pathsep}{env.get('PATH', '')}"
    return env


def run_agent(batch, args):
    env = agent_env()
    prompt = build_prompt(batch, env)
    cmd = ["claude", "-p", prompt, "--output-format", "stream-json", "--verbose",
           "--max-turns", str(args.turns), "--permission-mode", "acceptEdits",
           "--allowedTools", *ALLOWED_TOOLS]
    if args.model:
        cmd += ["--model", args.model]
    if not args.full_context:
        cmd += LEAN_FLAGS + ["--append-system-prompt", loop_context()]
    if args.dry_run:
        if not args.full_context:
            print(loop_context(), "\n" + "=" * 70)
        print(prompt)
        return None
    (LOG_DIR / "runs").mkdir(parents=True, exist_ok=True)
    transcript = LOG_DIR / "runs" / f"{batch[0]['func']}.jsonl"
    # Own process group, killed whole afterwards: tools the agent starts (permute_decls.py,
    # ninja) would otherwise outlive a timeout and keep rewriting the unit.
    p = subprocess.Popen(cmd, cwd=ROOT, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True,
                         stdin=subprocess.DEVNULL, env=env, start_new_session=True)
    try:
        out, err = p.communicate(timeout=args.timeout * 60 * len(batch))
    except subprocess.TimeoutExpired:
        kill_group(p)
        out, err = p.communicate()
        transcript.write_text(out or "")
        return {"timeout": True}
    kill_group(p)
    transcript.write_text(out)
    for line in reversed(out.splitlines()):
        try:
            e = json.loads(line)
        except json.JSONDecodeError:
            continue
        if e.get("type") == "result":
            return e
    return {"is_error": True, "result": (out + err)[-2000:], "returncode": p.returncode}


def kill_group(p):
    try:
        os.killpg(p.pid, signal.SIGKILL)
    except (ProcessLookupError, PermissionError):
        pass


def hit_limit(res):
    text = str(res.get("result", "")).lower()
    return res.get("is_error") and ("limit" in text or "quota" in text or "credit" in text)


def settle(batch, res):
    """Check the agent's work; commit or revert. Returns {func: result name}."""
    src = batch[0]["src"]
    files = changed_files()
    bad = [f for f in files if not allowed_change(f, src)]
    results = {}
    for i in batch:
        status = func_status(i["src"], i["func"])
        if status == i["status"]:
            results[i["func"]] = "no_change"
        else:
            results[i["func"]] = "match" if status == "done" else "nonmatching"
    fail = None
    if res.get("timeout"):
        fail = "timeout"
    elif bad:
        fail = "bad_files"
    elif all(r == "no_change" for r in results.values()):
        fail = "no_change"
    elif sh(["ninja"], timeout=1800).returncode != 0:
        fail = "build_failed"
    if fail is None:
        parts = []
        for kind in ("match", "nonmatching"):
            names = [f for f, r in results.items() if r == kind]
            if names:
                parts.append(f"{kind}: {' '.join(names)}")
        sh(["git", "add", *files])
        sh(["git", "commit", "-q", "-m", f"{'; '.join(parts)} ({src})"])
        return {f: ("reverted" if r == "no_change" else r) for f, r in results.items()}
    if files:
        sh(["git", "checkout", "--", *files])
    name = "timeout" if fail == "timeout" else "reverted"
    return {f: name for f in results}


def log(entry):
    LOG_DIR.mkdir(exist_ok=True)
    with LOG.open("a") as f:
        f.write(json.dumps(entry) + "\n")


def cmd_queue(args):
    items = pending(args)
    for i in items:
        size = f"{i['size']:6}" if i["size"] is not None else "     ?"
        print(f"{size}  {i['status']:11}  {i['func']:40}  {i['src']}")
    print(f"{len(items)} functions")


def cmd_run(args):
    if not args.dry_run:
        if changed_files():
            sys.exit("Working tree has uncommitted changes; commit or stash them first.")
        if sh(["ninja"], timeout=1800).returncode != 0:
            sys.exit("`ninja` fails before starting; fix the build first.")
    batches = make_batches(pending(args)[:args.n], args)
    for k, batch in enumerate(batches, 1):
        names = ", ".join(f"{i['func']} ({i['size']} bytes)" for i in batch)
        print(f"[{k}/{len(batches)}] {names}, {batch[0]['src']}", flush=True)
        t0 = time.time()
        res = run_agent(batch, args)
        if res is None:
            continue
        if hit_limit(res):
            print("Usage limit reached; stopping. Run again later.")
            break
        results = settle(batch, res)
        (LOG_DIR / "runs").mkdir(parents=True, exist_ok=True)
        (LOG_DIR / "runs" / f"{batch[0]['func']}.txt").write_text(str(res.get("result", "")))
        secs = round(time.time() - t0)
        turns, cost = res.get("num_turns"), res.get("total_cost_usd")
        n = len(batch)
        for i in batch:
            log({"time": time.strftime("%Y-%m-%d %H:%M:%S"), "func": i["func"],
                 "unit": i["src"], "size": i["size"], "before": i["status"],
                 "result": results[i["func"]],
                 "turns": round(turns / n, 1) if turns else turns,
                 "cost_usd": cost / n if cost else cost, "seconds": round(secs / n),
                 "batch": n})
        summary = ", ".join(f"{f} {r}" for f, r in results.items()) if n > 1 else results[batch[0]["func"]]
        print(f"    -> {summary} ({turns} turns, {secs} s, ${cost or 0:.2f})", flush=True)


def make_batches(items, args):
    """Group consecutive small functions of the same unit, up to --batch per call."""
    batches = []
    for i in items:
        small = i["size"] is not None and i["size"] <= args.batch_max
        last = batches[-1] if batches else None
        if (last and small and len(last) < args.batch and last[0]["src"] == i["src"]
                and all(j["size"] is not None and j["size"] <= args.batch_max for j in last)):
            last.append(i)
        else:
            batches.append([i])
    return batches


def cmd_report(args):
    if not LOG.exists():
        sys.exit("No runs yet.")
    entries = [json.loads(l) for l in LOG.read_text().splitlines()]
    by = {}
    for e in entries:
        by.setdefault(e["result"], []).append(e)
    print(f"{len(entries)} attempts")
    for result, es in sorted(by.items()):
        size = sum(e["size"] or 0 for e in es)
        print(f"  {result:12} {len(es):5}  {size:8} bytes")
    turns = [e["turns"] for e in entries if e.get("turns")]
    cost = sum(e.get("cost_usd") or 0 for e in entries)
    secs = sum(e["seconds"] for e in entries)
    if turns:
        print(f"turns/attempt {sum(turns) / len(turns):.1f}")
    print(f"time {secs / 3600:.1f} h, reported cost ${cost:.2f}")
    print("\nmatch rate by size:")
    for lo, hi in ((0, 64), (64, 256), (256, 1024), (1024, 4096), (4096, 1 << 30)):
        es = [e for e in entries if e["size"] is not None and lo <= e["size"] < hi]
        if es:
            n = sum(e["result"] == "match" for e in es)
            print(f"  {lo:5}-{hi if hi < 1 << 30 else '':<5} {n:4}/{len(es):<4} {100 * n / len(es):5.1f}%")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("command", choices=["queue", "run", "report"])
    ap.add_argument("-n", type=int, default=10, help="functions to attempt (run)")
    ap.add_argument("--unit")
    ap.add_argument("--max-size", type=int)
    ap.add_argument("--retry", action="store_true")
    ap.add_argument("--turns", type=int, default=60, help="max agent turns per function")
    ap.add_argument("--model")
    ap.add_argument("--timeout", type=int, default=45, help="minutes per function")
    ap.add_argument("--dry-run", action="store_true", help="print the prompts, run nothing")
    ap.add_argument("--batch", type=int, default=1, help="small functions per call (run)")
    ap.add_argument("--batch-max", type=int, default=64, help="max bytes of a batched function")
    ap.add_argument("--full-context", action="store_true",
                    help="load CLAUDE.md, memory, MCP and skills (default: lean context)")
    args = ap.parse_args()
    {"queue": cmd_queue, "run": cmd_run, "report": cmd_report}[args.command](args)


if __name__ == "__main__":
    main()
