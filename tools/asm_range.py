#!/usr/bin/env python3
"""Print splat's asm for an address range, from every .s file under asm/ (labels included).

    asm_range.py START END        # hex vaddrs, END exclusive
    e.g. python3 tools/asm_range.py 331F30 333860
"""
import re
import sys
from pathlib import Path

ADDR = re.compile(r"^\s*/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/")


def main():
    lo, hi = int(sys.argv[1], 16), int(sys.argv[2], 16)
    hits = []
    for path in sorted(Path("asm").rglob("*.s")):
        if "matchings" in path.parts and "nonmatchings" not in path.parts:
            continue  # asm/matchings/ duplicates code that is already decompiled
        pending, inside, first = [], False, None
        for line in path.read_text(errors="replace").splitlines():
            m = ADDR.match(line)
            if m:
                a = int(m.group(1), 16)
                inside = lo <= a < hi
                if inside:
                    if first is None:
                        first = a
                    hits.extend((a, l) for l in pending)
                    hits.append((a, line))
                pending = []
            elif line.strip():
                pending.append(line)  # labels / directives: kept if the next instruction is in range
    seen = set()
    for a, line in sorted(hits, key=lambda h: h[0]):
        if (a, line) in seen:
            continue
        seen.add((a, line))
        print(line)


if __name__ == "__main__":
    main()
