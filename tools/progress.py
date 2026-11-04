#!/usr/bin/env python3
"""Show how much of .text is decompiled.

A function counts as done when it is defined in C/C++ (no INCLUDE_ASM left).
The unit's functions are splat's files in asm/matchings/<unit>/ (needs
`disassemble_all`) and asm/nonmatchings/<unit>/. Sizes come from the
`nonmatching NAME, SIZE` line of each file (original sizes, so functions the
linker dropped don't count).

The summary covers all of .text: every function splat wrote (glabel in any
asm/**/*.s), sized up to the next function, is done (in a unit, no
INCLUDE_ASM), pending (INCLUDE_ASM in a unit) or uncut (still in a provisional
text/* chunk). Regions are rough (CLAUDE.md "Code regions"); names starting
with be / ig count as game / Alchemy wherever they are.

Usage: python3 tools/progress.py [--chunks]   (or: ninja progress)
  --chunks  also list the provisional text/* chunks (functions, named be*/ig*)
"""
import argparse
import re
from collections import defaultdict
from pathlib import Path

TEXT_START, TEXT_END = 0x100000, 0x467120
TEXT_SIZE = TEXT_END - TEXT_START

# Start address -> default owner, up to the next entry.
REGIONS = [
    (0x100000, "sony+libc"),
    (0x130000, "alchemy"),
    (0x230000, "mwcc runtime"),
    (0x231000, "alchemy"),
    (0x246D30, "lua"),
    (0x2590C0, "cri"),
    (0x2B5B68, "alchemy"),
    (0x370780, "game"),
    (0x440000, "alchemy"),
]

GLABEL = re.compile(r"^\s*glabel\s+(\S+)")
INSN = re.compile(r"^\s*/\* [0-9A-F]+ ([0-9A-F]{8}) ")
INCLUDE_ASM = re.compile(r"INCLUDE_ASM\(\"[^\"]+\", (\w+)\)")


def sizes(d):
    out = {}
    for p in Path(d).glob("*.s"):
        m = re.search(r"nonmatching \S+, (0x[0-9A-Fa-f]+)", p.read_text())
        out[p.stem] = int(m.group(1), 16)
    return out


def scan_functions():
    """Every function splat wrote: {addr: (name, asm file)}."""
    funcs = {}
    for p in Path("asm").rglob("*.s"):
        name = None
        for line in p.read_text().splitlines():
            m = GLABEL.match(line)
            if m:
                name = m.group(1)
                continue
            m = INSN.match(line)
            if m and name:
                addr = int(m.group(1), 16)
                if TEXT_START <= addr < TEXT_END:
                    funcs.setdefault(addr, (name, p))
                name = None
    return funcs


def owner(name, addr, path):
    if path.parts[1:3] in (("matchings", "lua"), ("nonmatchings", "lua")):
        return "lua"
    if name.startswith("be"):
        return "game"
    if name.startswith("ig"):
        return "alchemy"
    region = REGIONS[0][1]
    for start, r in REGIONS:
        if addr >= start:
            region = r
    return region


def summary(show_chunks):
    pending = set()
    for src in Path("src").rglob("*.c*"):
        pending |= set(INCLUDE_ASM.findall(src.read_text()))
    funcs = scan_functions()
    addrs = sorted(funcs)
    # [done, pending, uncut] x (functions, bytes)
    stats = defaultdict(lambda: [[0, 0], [0, 0], [0, 0]])
    chunks = defaultdict(lambda: [0, 0, 0, 0])  # functions, bytes, be*, ig*
    for i, addr in enumerate(addrs):
        name, path = funcs[addr]
        size = (addrs[i + 1] if i + 1 < len(addrs) else TEXT_END) - addr
        if path.parts[1] == "text":
            state = 2
            c = chunks[path.stem]
            c[0] += 1
            c[1] += size
            c[2] += name.startswith("be")
            c[3] += name.startswith("ig")
        elif path.parts[1] == "nonmatchings" and name in pending:
            state = 1
        else:
            state = 0
        for key in (owner(name, addr, path), "total"):
            stats[key][state][0] += 1
            stats[key][state][1] += size

    print(f"\n{'region':14} {'done':>18} {'pending':>18} {'uncut':>18} {'bytes':>9}")
    for key in sorted(stats, key=lambda k: (k == "total", -sum(s[1] for s in stats[k]))):
        s = stats[key]
        total = sum(x[1] for x in s)
        cells = " ".join(f"{n:5} {100 * b / total:5.1f}% {b // 1024:4}K" for n, b in s)
        print(f"{key:14} {cells} {total:9}")
    print("(cells: functions, % of the region's bytes, KB)")

    if show_chunks:
        print(f"\n{'chunk':10} {'functions':>9} {'bytes':>8} {'be*':>5} {'ig*':>5}")
        for stem in sorted(chunks):
            n, b, be, ig = chunks[stem]
            print(f"{stem:10} {n:9} {b:8} {be:5} {ig:5}")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--chunks", action="store_true")
    args = ap.parse_args()

    rows = []
    for src in sorted(Path("src").rglob("*.c*")):
        unit = src.relative_to("src").with_suffix("")
        # The unit's functions come from splat; which are done comes from the
        # source, so a new match counts without re-running splat.
        funcs = {**sizes(Path("asm/matchings") / unit), **sizes(Path("asm/nonmatchings") / unit)}
        asm = set(INCLUDE_ASM.findall(src.read_text()))
        done = {n: s for n, s in funcs.items() if n not in asm}
        todo = {n: s for n, s in funcs.items() if n in asm}
        rows.append((str(src), len(done), len(done) + len(todo), sum(done.values()),
                     sum(done.values()) + sum(todo.values())))

    print(f"{'unit':28} {'functions':>12} {'bytes':>17}")
    for name, fd, ft, bd, bt in rows:
        pct = f"{100 * bd / bt:5.1f}%" if bt else "  (no asm)"
        print(f"{name:28} {fd:5}/{ft:<6} {bd:7}/{bt:<7} {pct}")
    done = sum(r[3] for r in rows)
    print(f"\n.text total: {done}/{TEXT_SIZE} bytes ({100 * done / TEXT_SIZE:.3f}%)")
    summary(args.chunks)


if __name__ == "__main__":
    main()
