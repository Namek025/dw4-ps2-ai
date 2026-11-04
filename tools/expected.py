#!/usr/bin/env python3
"""Write the "expected" assembly of one source unit, for objdiff.

objdiff compares two objects per unit: ours (base) and the original (target).
The original object is rebuilt from splat's disassembly: every function of the
unit, done (asm/matchings/, needs `disassemble_all`) or not (asm/nonmatchings/),
`.include`d in address order and assembled with the normal `as`.

Usage: expected.py UNIT OUT.s   (e.g. expected.py script_bind build/expected/src/script_bind.s)
"""
import re
import sys
from pathlib import Path


def main():
    unit, out = sys.argv[1:]
    files = {}
    for d in ("matchings", "nonmatchings"):
        for p in (Path("asm") / d / unit).glob("*.s"):
            files.setdefault(p.stem, p)
    order = []
    for p in files.values():
        m = re.search(r"/\* \w+ ([0-9A-F]{8}) ", p.read_text())
        order.append((int(m.group(1), 16), p))
    lines = ['.include "labels.inc"', ".set noat", ".set noreorder", ".set gp=64", ".section .text"]
    lines += [f'.include "{p.as_posix()}"' for _, p in sorted(order)]
    Path(out).write_text("\n".join(lines) + "\n")


if __name__ == "__main__":
    main()
