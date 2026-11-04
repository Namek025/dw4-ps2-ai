#!/usr/bin/env python3
"""Functions that reference a symbol, from splat's asm (asm/text and asm/nonmatchings).

    xref.py SYM [SYM ...] [--dump]
    xref.py --re REGEX [REGEX ...]      # matching instruction lines, with their function
    e.g. python3 tools/xref.py D_005163F0 D_0051D1E8 --dump > build/xref.txt
         python3 tools/xref.py --re 'andi .*, 0x(100|200|300)$'

Prints "SYM  function  start  size  file" per referencing function; --dump also prints the
whole asm of each function once.
"""
import re
import sys
from pathlib import Path

GLABEL = re.compile(r"^glabel (\S+)")
NONMATCHING = re.compile(r"^nonmatching (\S+), (0x[0-9A-Fa-f]+)")
ADDR = re.compile(r"^\s*/\* [0-9A-F]+ ([0-9A-F]{8}) ")


def functions(path):
    """(name, size, lines) for each function in a .s file."""
    name, size, lines = None, None, []
    pending_size = None
    for line in path.read_text(errors="replace").splitlines():
        m = NONMATCHING.match(line)
        if m:
            pending_size = int(m.group(2), 16)
            continue
        m = GLABEL.match(line)
        if m and not line.startswith("glabel .") and pending_size is not None:
            if name:
                yield name, size, lines
            name, size, lines = m.group(1), pending_size, []
            pending_size = None
        if name:
            lines.append(line)
    if name:
        yield name, size, lines


def main():
    syms = [a for a in sys.argv[1:] if not a.startswith("--")]
    dump = "--dump" in sys.argv
    regex = "--re" in sys.argv
    if not syms:
        sys.exit(__doc__)
    if regex:
        pats = {s: re.compile(s) for s in syms}
    else:
        pats = {s: re.compile(r"\b%s\b" % re.escape(s)) for s in syms}
    found = {}
    for path in sorted(Path("asm").rglob("*.s")):
        if "matchings" in path.parts and "nonmatchings" not in path.parts:
            continue
        for name, size, lines in functions(path):
            if regex:
                for line in lines:
                    for p in pats.values():
                        if p.search(line):
                            print("%-40s %s" % (name, line.strip()))
                continue
            text = "\n".join(lines)
            for s, p in pats.items():
                if p.search(text):
                    start = next((m.group(1) for m in map(ADDR.match, lines) if m), "?")
                    print("%-12s %-40s %s 0x%-5X %s" % (s, name, start, size, path))
                    found.setdefault(name, lines)
    if dump:
        for name, lines in found.items():
            print("\n" + "\n".join(lines))


if __name__ == "__main__":
    main()
