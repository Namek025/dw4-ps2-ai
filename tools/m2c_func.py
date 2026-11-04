#!/usr/bin/env python3
"""Run m2c on one function of the splat asm, with its jump tables.

splat leaves the data as bins, so jump tables are read from the target rom and
their targets get labels; string literals in the output are shown inline.

Usage: python3 tools/m2c_func.py FUNC [OUT.c] [--keep-syms] [m2c args...]

--keep-syms: write string literals as D_XXXXXXXX /* "text" */ instead of the text.
"""
import re
import struct
import subprocess
import sys
from pathlib import Path

ROM = Path("build/target/SLUS_208.36.rom").read_bytes()
BASE = 0x100000


def cstr(a):
    o = a - BASE
    if not 0 <= o < len(ROM):
        return None
    e = ROM.find(b"\0", o)
    s = ROM[o:e]
    return s.decode() if 0 < len(s) < 80 and all(32 <= c < 127 for c in s) else None


def main():
    fn = sys.argv[1]
    out = Path(sys.argv[2]) if len(sys.argv) > 2 and not sys.argv[2].startswith("-") else None
    keep = "--keep-syms" in sys.argv
    extra = [a for a in sys.argv[2:] if a.startswith("-") and a != "--keep-syms"]
    body = None
    for p in Path("asm").rglob("*.s"):
        t = p.read_text()
        m = re.search(rf"^glabel {re.escape(fn)}$.*?^endlabel {re.escape(fn)}$", t, re.S | re.M)
        if m:
            body = m.group(0)
            break
    if body is None:
        sys.exit(f"{fn} not found in asm/")
    addrs = [int(a, 16) for a in re.findall(r"/\* \w+ ([0-9A-F]{8}) ", body)]
    lo, hi = min(addrs), max(addrs)
    labels = set(re.findall(r"^\s*\.(L[0-9A-F]{8}):", body, re.M))
    tables, need = [], set()
    for m in sorted(set(re.findall(r"jtbl_([0-9A-F]{8})", body))):
        o, ents = int(m, 16) - BASE, []
        while True:
            w = struct.unpack("<I", ROM[o:o + 4])[0]
            if not (lo <= w <= hi and w % 4 == 0):
                break
            ents.append(f"L{w:08X}")
            need.add(w)
            o += 4
        tables.append(f".section .rodata\nglabel jtbl_{m}\n" + "".join(f".word .{e}\n" for e in ents))
    lines = []
    for l in body.splitlines():
        mm = re.match(r"\s*/\* \w+ ([0-9A-F]{8}) ", l)
        if mm and int(mm.group(1), 16) in need and "L" + mm.group(1) not in labels:
            lines.append(".L" + mm.group(1) + ":")
            labels.add("L" + mm.group(1))
        lines.append(l)
    src = ".set noat\n.set noreorder\n.section .text\n" + "\n".join(lines) + "\n" + "\n".join(tables)
    tmp = Path("build/m2c") / f"{fn}.s"
    tmp.parent.mkdir(parents=True, exist_ok=True)
    tmp.write_text(src)
    m2c = Path(sys.executable).with_name("m2c")
    r = subprocess.run([str(m2c), "-t", "mipsee-mwcc-c++", *extra, str(tmp)],
                       capture_output=True, text=True)
    def lit(m):
        s = cstr(int(m.group(1), 16))
        if not s:
            return m.group(0)
        return f'D_{m.group(1)} /* "{s}" */' if keep else f'"{s}"'

    text = re.sub(r"&D_([0-9A-F]{8})", lit, r.stdout)
    if out:
        out.write_text(text)
    else:
        sys.stdout.write(text)
    sys.stderr.write(r.stderr[-2000:])


if __name__ == "__main__":
    main()
