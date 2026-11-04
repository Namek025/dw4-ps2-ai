#!/usr/bin/env python3
"""Instruction distance between a compiled function and the original.

Aligns fcmp's two listings with difflib (lines fcmp reports equal, relocations
masked, count as identical) and prints the number of differing instructions;
-v shows the differing runs.

Usage: python3 tools/fdist.py OBJ FUNC [-v]
"""
import difflib
import re
import subprocess
import sys


def distance(obj, func, verbose=False):
    out = subprocess.run([sys.executable, "tools/fcmp.py", obj, func],
                         capture_output=True, text=True).stdout
    if f"{func}: MATCH" in out:
        return 0, []
    a, b = [], []
    norm = lambda s: re.sub(r"0x[0-9a-f]+|<[^>]*>", "", s).strip()
    for l in out.splitlines():
        m = re.match(r"\s*(!!)?\s*[0-9A-F]{8}\s+(.*?)\s*\|\s*(.*)$", l)
        if not m:
            continue
        x, y = m.group(2), m.group(3)
        if not m.group(1):
            x = y = "EQ " + norm(x)
        if x:
            a.append(norm(x))
        if y:
            b.append(norm(y))
    if not a:
        return None, []
    d, runs = 0, []
    for op, i1, i2, j1, j2 in difflib.SequenceMatcher(None, a, b, autojunk=False).get_opcodes():
        if op != "equal":
            d += max(i2 - i1, j2 - j1)
            runs.append((op, a[i1:i2], b[j1:j2]))
    return d, runs


def main():
    d, runs = distance(sys.argv[1], sys.argv[2])
    if "-v" in sys.argv:
        for op, x, y in runs:
            print(op, "ORIG", x, "OURS", y)
    print("distance", d)


if __name__ == "__main__":
    main()
