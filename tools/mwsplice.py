#!/usr/bin/env python3
"""Build one assembly file for an MWCC translation unit.

MWCC can't use our GNU-style INCLUDE_ASM, so in an MWCC unit INCLUDE_ASM
expands to nothing and only the decompiled functions are compiled. This script
puts the unit back together in original address order:

- a function MWCC compiled is emitted as `.word`s, with a `.reloc` for every
  relocation in the object (REL: the addend is already in the word);
- every other function is `.include`d from its splat file, if the source still
  has its INCLUDE_ASM (a .s left over from an earlier split is ignored, as a
  fresh split would not write it there);
- a compiled function that is not in the unit (splat's asm/matchings/ and
  asm/nonmatchings/) is dropped if nothing kept calls it: the Metrowerks
  linker strips unreferenced functions.

Data the unit defines (string literals, tables, switch tables, const bool&
temporaries, ...) is placed at its original address, read back from the target
rom: from the instructions that use it (the code matches, so the original
instruction holds the real address), from config/symbol_addrs.txt for named
globals, and from pointers inside data already placed. The compiled bytes are
checked against the rom, so a wrong guess fails the build.

If splat gives the unit a `.rodata` subsegment, that range is emitted here as a
real `.rodata` section: the compiled data with labels and relocations, zero
padding, and the rom's bytes for whatever the compiled C doesn't produce (data
of functions still in asm). Otherwise the data stays in the original bin and
relocations point at absolute addresses.

Usage: mwsplice.py OBJ ASM_DIR OUT.s
"""
import re
import struct
import sys
from pathlib import Path

R_MIPS_32 = 2
RELOC_NAMES = {4: "R_MIPS_26", 5: "R_MIPS_HI16", 6: "R_MIPS_LO16", 7: "R_MIPS_GPREL16",
               8: "R_MIPS_LITERAL"}
GP_RELOCS = (7, 8)  # GPREL16, LITERAL (gp-relative constant)
SYMBOLS = "config/symbol_addrs.txt"
SPLAT = "config/splat.yaml"
ROM = "build/target/SLUS_208.36.rom"
BASE = 0x100000
GP = 0x55C870
# C++ exception tables: the original's copy is still in the data bin.
IGNORED_SECTIONS = (".exception", ".exceptix")


def s16(x):
    return x - 0x10000 if x & 0x8000 else x


def demangle(name):
    """MWCC C++ function name -> plain name (`luaD_call__FP9lua_Statei` -> `luaD_call`).

    The rest of the build (splat, symbol_addrs, other units) uses plain names;
    overloads don't exist in the units we compile this way."""
    i = name.find("__F", 1)
    return name[:i] if i > 0 else name


class Obj:
    def __init__(self, path):
        self.path = path
        d = self.d = Path(path).read_bytes()
        shoff, = struct.unpack_from("<I", d, 0x20)
        shentsize, shnum = struct.unpack_from("<HH", d, 0x2E)
        shstrndx, = struct.unpack_from("<H", d, 0x32)
        # name, type, flags, addr, offset, size, link, info, align, entsize
        self.secs = [struct.unpack_from("<10I", d, shoff + i * shentsize) for i in range(shnum)]
        self.names = [self.cstr(self.secs[shstrndx][4] + s[0]) for s in self.secs]
        symtab = next(s for s in self.secs if s[1] == 2)
        strtab = self.secs[symtab[6]]
        self.syms = []
        self.syms_bind = []
        for i in range(symtab[5] // 16):
            nm, val, size, info, _, shn = struct.unpack_from("<IIIBBH", d, symtab[4] + 16 * i)
            self.syms.append((self.cstr(strtab[4] + nm), val, size, info & 0xF, shn))
            self.syms_bind.append(info >> 4)
        # relocations per target section: {shn: [(offset, type, symbol index)]}
        self.relocs = {}
        for s in self.secs:
            if s[1] != 9:
                continue
            for j in range(s[5] // 8):
                off, info = struct.unpack_from("<II", d, s[4] + 8 * j)
                self.relocs.setdefault(s[7], []).append((off, info & 0xFF, info >> 8))

    def cstr(self, off):
        return self.d[off:self.d.index(b"\0", off)].decode()

    def is_code(self, shn):
        return 0 < shn < len(self.secs) and self.secs[shn][2] & 4

    def is_data(self, shn):
        s = self.secs[shn] if 0 < shn < len(self.secs) else None
        return s is not None and s[2] & 2 and not s[2] & 4 and self.names[shn] not in IGNORED_SECTIONS

    def funcs(self):
        out = {}
        for name, val, size, typ, shn in self.syms:
            if typ != 2:  # STT_FUNC
                continue
            code = self.d[self.secs[shn][4] + val:self.secs[shn][4] + val + size]
            rel = [(o - val, t, si) for o, t, si in self.relocs.get(shn, []) if val <= o < val + size]
            out[name] = (code, rel)
        return out

    def sym_name(self, si):
        """Plain name of a relocation target. MWCC puts each function in its own
        section and may relocate against that section's symbol (static calls)."""
        name, val, size, typ, shn = self.syms[si]
        if typ == 3 and self.is_code(shn):  # STT_SECTION
            name = next(n for n, v, _, t, h in self.syms if t == 2 and h == shn and v == 0)
        return name if name in KNOWN else demangle(name)


def rodata_range(unit):
    """[start, end) of the unit's `.rodata` subsegment in splat.yaml, or None."""
    import yaml
    subs = yaml.safe_load(Path(SPLAT).read_text())["segments"][0]["subsegments"]
    starts = [s[0] if isinstance(s, list) else s["start"] for s in subs]
    for i, sub in enumerate(subs):
        typ, name = (sub[1:3] if len(sub) >= 3 else (None, None)) if isinstance(sub, list) else (sub.get("type"), sub.get("name"))
        if typ == ".rodata" and name == unit:
            return BASE + starts[i], BASE + starts[i + 1]
    return None


def next_segment_start(unit):
    """Rom offset of the segment after the unit, or None."""
    import yaml
    subs = yaml.safe_load(Path(SPLAT).read_text())["segments"][0]["subsegments"]
    starts = [s[0] if isinstance(s, list) else s["start"] for s in subs]
    for i, sub in enumerate(subs[:-1]):
        name = sub[2] if isinstance(sub, list) and len(sub) >= 3 else None
        if name == unit:
            return starts[i + 1]
    return None


def include_asm_names(asm_dir):
    """Functions the unit's source still takes from asm."""
    unit = Path(asm_dir).relative_to("asm/nonmatchings")
    for ext in (".cpp", ".c"):
        src = Path("src") / unit.with_name(unit.name + ext)
        if src.exists():
            return set(re.findall(r'INCLUDE_ASM\("[^"]*",\s*([\w.]+)\)', src.read_text()))
    sys.exit(f"no source for {asm_dir}")


KNOWN = dict(
    (m.group(1), int(m.group(2), 16))
    for m in re.finditer(r"^(\w+) = 0x([0-9A-Fa-f]+);", Path(SYMBOLS).read_text(), re.M)
)


def main():
    obj_path, asm_dir, out = sys.argv[1:]
    obj = Obj(obj_path)
    rom = Path(ROM).read_bytes()
    rom_word = lambda a: struct.unpack_from("<I", rom, a - BASE)[0]
    funcs = {n if n in KNOWN else demangle(n): f for n, f in obj.funcs().items()}

    order = []
    active = include_asm_names(asm_dir)
    stale = set()
    for p in Path(asm_dir).glob("*.s"):
        if p.stem not in active:
            stale.add(p.stem)
            continue
        m = re.search(r"/\* \w+ ([0-9A-F]{8}) ", p.read_text())
        order.append((int(m.group(1), 16), p))
    # splat writes no file for functions already in the source: place them by
    # address, taken from their func_XXXXXXXX name or config/symbol_addrs.txt.
    known = KNOWN
    have = {p.stem for _, p in order}
    # Every function of the unit, done or not (splat's disassemble_all writes the
    # done ones to asm/matchings/).
    unit = have | stale | {p.stem for p in Path(str(asm_dir).replace("nonmatchings", "matchings", 1)).glob("*.s")}
    addr_of = {}
    for name in funcs:
        m = re.fullmatch(r"func_([0-9A-F]{8})", name)
        addr = int(m.group(1), 16) if m else known.get(name)
        if addr is not None and name in unit:
            addr_of[name] = addr
    # Functions not in the unit were dropped by the Metrowerks linker (deadstrip):
    # fine as long as nothing we keep calls them.
    called = {obj.sym_name(si) for n in addr_of for _, t, si in funcs[n][1] if t == 4}
    for name in list(funcs):
        if name not in addr_of:
            if name in called:
                sys.exit(f"{obj_path}: no address for {name}")
            del funcs[name]
        elif name not in have:
            order.append((addr_of[name], Path(asm_dir) / f"{name}.s"))
    order.sort()

    # Point relocations against the object's own data at the original address.
    placed = {}  # symbol index -> original address of the symbol (value 0 of its section/object)

    def place(si, addr, where):
        if placed.setdefault(si, addr) != addr:
            sys.exit(f"{obj_path}: {obj.syms[si][0] or obj.names[obj.syms[si][4]]} placed at "
                     f"{placed[si]:08X} and {addr:08X} ({where})")

    for name, (code, rel) in funcs.items():
        base = addr_of[name]
        word = lambda o: struct.unpack_from("<I", code, o)[0]
        last_hi = {}
        for o, typ, si in rel:
            if typ not in RELOC_NAMES:
                sys.exit(f"{obj_path}: {name}+{o:#x}: unhandled relocation type {typ}")
            shn = obj.syms[si][4]
            if not obj.is_data(shn):
                continue
            orig = rom_word(base + o)
            if typ == 5:
                last_hi[si] = o
            elif typ == 6 and si in last_hi:
                ho = last_hi[si]
                target = ((rom_word(base + ho) & 0xFFFF) << 16) + s16(orig & 0xFFFF)
                addend = ((word(ho) & 0xFFFF) << 16) + s16(word(o) & 0xFFFF)
                place(si, target - addend, f"{name}+{o:#x}")
            elif typ in GP_RELOCS:
                place(si, GP + s16(orig & 0xFFFF) - s16(word(o) & 0xFFFF), f"{name}+{o:#x}")

    # Named globals defined here, then data only reachable through pointers in
    # data already placed (string tables, ...).
    for si, (nm, val, size, typ, shn) in enumerate(obj.syms):
        if typ == 1 and obj.is_data(shn) and nm in KNOWN:
            place(si, KNOWN[nm], "symbol_addrs")
    secaddr = {}  # data section -> original address of its start

    def place_sections():
        for si, addr in placed.items():
            nm, val, size, typ, shn = obj.syms[si]
            if secaddr.setdefault(shn, addr - val) != addr - val:
                sys.exit(f"{obj_path}: section {obj.names[shn]} of {nm} placed twice")
    place_sections()
    grew = True
    while grew:
        grew = False
        for shn, base in list(secaddr.items()):
            for o, typ, si in obj.relocs.get(shn, []):
                tshn = obj.syms[si][4]
                if typ != R_MIPS_32 or not obj.is_data(tshn) or si in placed:
                    continue
                addend, = struct.unpack_from("<I", obj.d, obj.secs[shn][4] + o)
                place(si, rom_word(base + o) - addend, f"pointer at {base + o:08X}")
                grew = True
        place_sections()

    # Check the compiled data against the rom (skip words the data relocates).
    for si, addr in placed.items():
        nm, val, size, typ, shn = obj.syms[si]
        sec = obj.secs[shn]
        if sec[1] == 8:  # NOBITS (.bss)
            continue
        start, end = (val, val + size) if typ != 3 else (0, sec[5])  # 3 = STT_SECTION
        skip = {o for o, _, _ in obj.relocs.get(shn, [])}
        for o in range(start, end):
            if any(r <= o < r + 4 for r in skip):
                continue
            mine = obj.d[sec[4] + o]
            theirs = rom[addr - BASE + o]
            if mine != theirs:
                sys.exit(f"{obj_path}: data {nm or obj.names[shn]}+{o - start:#x} placed at "
                         f"{addr + o:08X} is {mine:#04x}, original {theirs:#04x}")

    def sym_name(si):
        if si in placed:
            return f"__mwsplice_{placed[si]:08X}"
        if obj.syms[si][3] == 3 and obj.syms[si][4] in secaddr:  # data section symbol
            return f"__mwsplice_{secaddr[obj.syms[si][4]]:08X}"
        return obj.sym_name(si)

    # The unit's .rodata block, if splat gives it one.
    block = []
    labels = set(placed.values()) | set(secaddr.values())
    rng = rodata_range(str(Path(asm_dir).relative_to("asm/nonmatchings")))
    if rng:
        lo, hi = rng
        buf = bytearray(rom[lo - BASE:hi - BASE])  # rom bytes where the C produces nothing
        relocs = {}
        for shn, addr in secaddr.items():
            if obj.names[shn] != ".rodata":
                continue
            size = obj.secs[shn][5]
            if not lo <= addr <= addr + size <= hi:
                sys.exit(f"{obj_path}: {obj.names[shn]} at {addr:08X} is outside the unit's .rodata {lo:08X}-{hi:08X}")
            buf[addr - lo:addr - lo + size] = obj.d[obj.secs[shn][4]:obj.secs[shn][4] + size]
            for o, typ, si in obj.relocs.get(shn, []):
                if typ != R_MIPS_32:
                    sys.exit(f"{obj_path}: unhandled data relocation type {typ}")
                relocs[addr - lo + o] = sym_name(si)
        inblock = {a for a in labels if lo <= a < hi}
        labels -= inblock
        # Global data (e.g. luaO_nilobject) is defined here under its own name,
        # and so is data the unit's asm functions use (`D_XXXXXXXX`).
        exported = {}
        for _, f in order:
            if f.stem not in funcs:
                for d in set(re.findall(r"\bD_([0-9A-F]{8})\b", f.read_text())):
                    if lo <= int(d, 16) < hi:
                        exported.setdefault(int(d, 16), []).append(f"D_{d}")
                        inblock.add(int(d, 16))
        for si, (nm, val, size, typ, shn) in enumerate(obj.syms):
            if typ == 1 and obj.syms_bind[si] == 1 and shn in secaddr and lo <= secaddr[shn] + val < hi:
                exported.setdefault(secaddr[shn] + val, []).append(nm)
                inblock.add(secaddr[shn] + val)
        block = [".section .rodata"]
        k = 0
        while k < hi - lo:
            if lo + k in inblock:
                block.append(f"__mwsplice_{lo + k:08X}:")
                for nm in exported.get(lo + k, []):
                    block += [f".global {nm}", f".type {nm}, @object", f"{nm}:"]
            if k in relocs:
                block += [f"    .reloc ., R_MIPS_32, {relocs[k]}",
                          f"    .word 0x{struct.unpack_from('<I', buf, k)[0]:08X}"]
                k += 4
                continue
            n = 1
            while k + n < hi - lo and n < 16 and lo + k + n not in inblock and k + n not in relocs:
                n += 1
            block.append("    .byte " + ", ".join(f"0x{c:02X}" for c in buf[k:k + n]))
            k += n

    lines = ['.include "labels.inc"', ".set noat", ".set noreorder", ".set gp=64", ".section .text"]
    for addr in sorted(labels):
        lines.append(f".set __mwsplice_{addr:08X}, 0x{addr:08X}")
    for _, p in order:
        if p.stem not in funcs:
            lines.append(f'.include "{p.as_posix()}"')
            continue
        code, rel = funcs[p.stem]
        gprel = {o: placed[si] for o, t, si in rel if t in GP_RELOCS and si in placed}
        rel = {o: (RELOC_NAMES[t], sym_name(si)) for o, t, si in rel if o not in gprel}
        lines += [".align 3", f".global {p.stem}", f".type {p.stem}, @function", f"{p.stem}:"]
        for i in range(0, len(code), 4):
            w = struct.unpack_from("<I", code, i)[0]
            if i in gprel:
                # gp is fixed: write the final offset (gas can't relocate GPREL16
                # against an absolute symbol).
                w = (w & 0xFFFF0000) | ((gprel[i] + s16(w & 0xFFFF) - GP) & 0xFFFF)
            elif i in rel:
                lines.append(f"    .reloc ., {rel[i][0]}, {rel[i][1]}")
            lines.append(f"    .word 0x{w:08X}")
        lines += [f".size {p.stem}, . - {p.stem}", ".balign 16"]
    # What follows may not be 16-aligned (zutil is followed by handwritten asm at 0x2252D8).
    nxt = next_segment_start(str(Path(asm_dir).relative_to("asm/nonmatchings")))
    if nxt is not None and nxt % 16 and lines[-1] == ".balign 16":
        lines.pop()
    lines += block
    Path(out).write_text("\n".join(lines) + "\n")


if __name__ == "__main__":
    main()
