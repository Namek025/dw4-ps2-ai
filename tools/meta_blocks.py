#!/usr/bin/env python3
"""Find runs of meta registration boilerplate in the provisional asm chunks.

Splits every asm/text/*.s chunk into functions, runs tools/meta_src.py's recognizer on
each, and prints the runs where recognized functions follow one another (a few
unrecognized ones in between, e.g. retrieveVTable with a real constructor, are allowed).
Each run is a candidate unit for config/splat.yaml, then `tools/meta_src.py --check`.

Usage: python3 tools/meta_blocks.py [--min N] [--gap G]
"""
import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import meta_src  # noqa: E402

INSN = re.compile(r"/\* \w+ ([0-9A-F]{8}) [0-9A-F]{8} \*/\s+(\S+)\s*(.*)")


def chunk_funcs(path):
    """(addr, name, f) for each function of a multi-function splat .s file."""
    out = []
    name, ins, addr = None, [], None
    for line in path.read_text().splitlines():
        m = re.match(r"\s*glabel (\S+)", line)
        if m:
            name, ins, addr = m.group(1), [], None
            continue
        if line.startswith("endlabel") and name:
            out.append((addr, name, ins))
            name = None
            continue
        m = INSN.search(line)
        if name and m:
            if addr is None:
                addr = int(m.group(1), 16)
            args = [a.strip() for a in m.group(3).split(",")] if m.group(3) else []
            ins.append((m.group(2), args))
    res = []
    for addr, name, ins in out:
        f = {"ins": ins, "ops": [op for op, _ in ins], "frame": 0,
             "calls": [a[0] for op, a in ins if op == "jal"],
             "syms": [s for _, a in ins for x in a for s in meta_src.SYM.findall(x)]}
        if ins and ins[0][0] == "addiu" and meta_src.reg(ins[0][1][0]) == 29:
            f["frame"] = -int(ins[0][1][2], 0)
        res.append((addr, name, f))
    return res


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--min", type=int, default=20, help="recognized functions per run")
    ap.add_argument("--gap", type=int, default=4, help="unrecognized functions allowed in a row")
    args = ap.parse_args()
    sizes = meta_src.struct_sizes()
    for chunk in sorted((ROOT / "asm" / "text").glob("*.s")):
        funcs = chunk_funcs(chunk)
        marks = []
        for i, (addr, name, f) in enumerate(funcs):
            end = funcs[i + 1][0] if i + 1 < len(funcs) else addr + 4 * len(f["ins"])
            ok = meta_src.recognize(name, f, {}, sizes) is not None or \
                name.endswith(("_register", "_registerFields"))
            marks.append((addr, end, name, ok))
        run = None
        runs = []
        for addr, end, name, ok in marks:
            if ok:
                if run and run["gap"] <= args.gap:
                    run["end"], run["n"], run["gap"] = end, run["n"] + 1, 0
                    run["names"].append(name)
                else:
                    if run:
                        runs.append(run)
                    run = {"start": addr, "end": end, "n": 1, "gap": 0, "names": [name]}
            elif run:
                run["gap"] += 1
        if run:
            runs.append(run)
        for r in runs:
            if r["n"] < args.min:
                continue
            first = next((n[:-len("_register")] for n in r["names"] if n.endswith("_register")), "?")
            print(f"{chunk.stem}: 0x{r['start']:08X}-0x{r['end']:08X} {r['end'] - r['start']:7} bytes "
                  f"{r['n']:5} recognized  first class {first}")
            print(f"    - [0x{r['start'] - 0x100000:06X}, cpp, alchemy/meta_{first}]")


if __name__ == "__main__":
    main()
