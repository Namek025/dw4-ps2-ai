#!/usr/bin/env python3
"""Compare compiled functions against the original, one by one.

Reads an ELF32-LE object (MWCC or GCC), finds each function's original address
(func_XXXXXXXX name or config/symbol_addrs.txt) and compares its bytes with the
target rom. Relocated fields are masked (the object has 0 there).

Usage: python3 tools/fcmp.py OBJ [NAME ...]
  e.g. python3 tools/fcmp.py build/src/script_bind.mw.o SYSgetPlayerNinzu
With NAMEs, a DIFF prints both disassemblies side by side.
"""
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

ROM = "build/target/SLUS_208.36.rom"
SYMBOLS = "config/symbol_addrs.txt"
BASE = 0x100000
MASKS = {4: 0xFC000000, 5: 0xFFFF0000, 6: 0xFFFF0000, 7: 0xFFFF0000}  # R_MIPS_26 / HI16 / LO16 / GPREL16


def demangle(name):
    """MWCC C++ function name -> plain name (`luaD_call__FP9lua_Statei` -> `luaD_call`)."""
    i = name.find("__F", 1)
    return name[:i] if i > 0 else name


def read_funcs(path):
    d = Path(path).read_bytes()
    shoff, = struct.unpack_from("<I", d, 0x20)
    shentsize, shnum = struct.unpack_from("<HH", d, 0x2E)
    secs = [struct.unpack_from("<10I", d, shoff + i * shentsize) for i in range(shnum)]
    symtab = next(s for s in secs if s[1] == 2)
    strtab = secs[symtab[6]]
    syms = []
    for i in range(symtab[5] // 16):
        nm, val, size, info, _, shn = struct.unpack_from("<IIIBBH", d, symtab[4] + 16 * i)
        name = d[strtab[4] + nm:d.index(b"\0", strtab[4] + nm)].decode()
        syms.append((name, val, size, info & 0xF, shn))
    masks = {}
    for s in secs:
        if s[1] == 9:  # SHT_REL
            for j in range(s[5] // 8):
                off, info = struct.unpack_from("<II", d, s[4] + 8 * j)
                masks[(s[7], off)] = (MASKS.get(info & 0xFF, 0), syms[info >> 8][0])
    funcs = {}
    for name, val, size, typ, shn in syms:
        if typ == 2 and size:
            words = []
            for i in range(0, size, 4):
                w, = struct.unpack_from("<I", d, secs[shn][4] + val + i)
                m, sym = masks.get((shn, val + i), (0xFFFFFFFF, None))
                words.append((w, m, sym))
            funcs[name] = words
    return funcs


def disasm(words, addr):
    with tempfile.NamedTemporaryFile(suffix=".bin") as f:
        f.write(b"".join(struct.pack("<I", w) for w in words))
        f.flush()
        out = subprocess.run(
            ["mips-linux-gnu-objdump", "-D", "-z", "-b", "binary", "-m", "mips:5900", "-EL",
             f"--adjust-vma={addr:#x}", f.name], capture_output=True, text=True).stdout
    return [re.sub(r"^\s*[0-9a-f]+:\s+[0-9a-f]+\s+", "", l)
            for l in out.splitlines() if re.match(r"^\s*[0-9a-f]+:\t", l)]


def main():
    obj, wanted = sys.argv[1], sys.argv[2:]
    rom = Path(ROM).read_bytes()
    known = {m.group(1): int(m.group(2), 16) for m in
             re.finditer(r"^(\w+) = 0x([0-9A-Fa-f]+);", Path(SYMBOLS).read_text(), re.M)}
    # MWCC C++ units: compare under plain names (as tools/mwsplice.py does).
    funcs = {n if n in known else demangle(n): w for n, w in read_funcs(obj).items()}
    ok = total = 0
    for name, words in funcs.items():
        if wanted and name not in wanted:
            continue
        m = re.fullmatch(r"func_([0-9A-F]{8})", name)
        addr = int(m.group(1), 16) if m else known.get(name)
        if addr is None:
            print(f"{name}: no address")
            continue
        orig = [struct.unpack_from("<I", rom, addr - BASE + 4 * i)[0] for i in range(len(words))]
        good = all((w & mk) == (o & mk) for (w, mk, _), o in zip(words, orig))
        total += 1
        ok += good
        if good:
            if wanted:
                print(f"{name}: MATCH")
            continue
        print(f"{name}: DIFF")
        if wanted:
            mine = disasm([w for w, _, _ in words], addr)
            theirs = disasm(orig, addr)
            for i, (a, b) in enumerate(zip(theirs, mine)):
                sym = words[i][2]
                mark = "  " if (words[i][0] & words[i][1]) == (orig[i] & words[i][1]) else "!!"
                b = f"{b}  <{sym}>" if sym else b
                print(f"  {mark} {addr + 4 * i:08X}  {a:32.32s} | {b}")
    print(f"{ok}/{total} match")


if __name__ == "__main__":
    main()
