#!/usr/bin/env python3
"""Propose translation-unit cuts from the named functions in symbol_addrs.txt.

Walks the named functions of a range in address order and starts a new unit
where the class prefix (text before the first '_') changes. Prefixes where one
starts with the other (beBkColor / beBkColorInfo) stay in one unit. Meta
registration functions (_register, _registerFields) live elsewhere and are
skipped; unnamed (func_) and mangled (__dt__) names never start a unit.
Cuts are at named function starts, so they are valid splat boundaries; the
exact end of a unit (unnamed functions before the next class) is a guess.

Usage: python3 tools/propose_units.py START END [--check]
  --check: compare with the cpp segments already in config/splat.yaml
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SKIP_SUFFIX = ("_register", "_registerFields")


def named_funcs():
    out = []
    for line in (ROOT / "config/symbol_addrs.txt").read_text().splitlines():
        m = re.match(r"(\w+) = 0x([0-9A-Fa-f]+); // type:func", line)
        if m:
            out.append((int(m.group(2), 16), m.group(1)))
    return sorted(out)


def prefix(name):
    if name.startswith(("func_", "__", "D_")) or name.endswith(SKIP_SUFFIX):
        return None
    return name.split("_")[0]


def related(a, b):
    return a.startswith(b) or b.startswith(a)


def propose(start, end):
    units = []  # [start, prefix, names]
    for addr, name in named_funcs():
        if not start <= addr < end:
            continue
        p = prefix(name)
        if p is None:
            if units:
                units[-1][2].append(name)
            continue
        if units and related(units[-1][1], p):
            units[-1][2].append(name)
            if len(p) < len(units[-1][1]):
                units[-1][1] = p
            continue
        units.append([addr, p, [name]])
    return units


def existing():
    out = {}
    for line in (ROOT / "config/splat.yaml").read_text().splitlines():
        m = re.match(r"\s*- \[0x([0-9A-F]+), cpp, (\S+)\]", line)
        if m:
            out[int(m.group(1), 16) + 0x100000] = m.group(2)
    return out


def main():
    start, end = int(sys.argv[1], 16), int(sys.argv[2], 16)
    have = existing()
    for addr, p, names in propose(start, end):
        mark = ""
        if "--check" in sys.argv:
            mark = f"  [splat: {have[addr]}]" if addr in have else ""
        print(f"0x{addr:08X}  {p:32} {len(names):3} named{mark}")
    if "--check" in sys.argv:
        props = {a for a, _, _ in propose(start, end)}
        missed = [f"0x{a:08X} {n}" for a, n in have.items() if start <= a < end and a not in props]
        if missed:
            print("splat units not proposed:", ", ".join(missed))


if __name__ == "__main__":
    main()
