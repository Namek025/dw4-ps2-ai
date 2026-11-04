#!/usr/bin/env python3
"""Print the Lua script-command table as a Markdown table.

func_003719D0 registers 0x74 commands through func_0023F4F0, passing the
dispatcher func_00370780, a string pool D_004C8560 and a descriptor list
D_0050CFC0. Each descriptor is `2 + nargs` words:

    word 0: bits 0-7 command id (stored at +0x28, the dispatcher's switch
            index into jtbl_0050D620), bits 8-12 nargs, bits 13+ index of
            the name in the string pool
    word 1: return type (0x200 | type code)
    then one type code per argument

The implementation is the first `jal` of each case (checked for all 116).
Overloads (same name, several ids) get a `_<n>` suffix in table order.

Usage: python3 tools/dump_script_bindings.py [--symbols]
       (needs build/target/*.rom; --symbols prints symbol_addrs.txt lines)
"""
import collections
import struct
import sys

ROM = "build/target/SLUS_208.36.rom"
BASE = 0x100000
NAMES = 0x004C8560
DESCS = 0x0050CFC0
JTBL = 0x0050D620
COUNT = 0x74

rom = open(ROM, "rb").read()


def word(addr):
    return struct.unpack_from("<I", rom, addr - BASE)[0]


def cstr(addr):
    end = rom.index(b"\0", addr - BASE)
    return rom[addr - BASE:end].decode("latin1")


def first_jal(addr):
    for _ in range(64):
        ins = word(addr)
        if ins >> 26 == 3:
            return ((ins & 0x3FFFFFF) << 2) | (addr & 0xF0000000)
        addr += 4
    return None


def commands():
    p = DESCS
    out = []
    for _ in range(COUNT):
        desc = word(p)
        nargs = (desc >> 8) & 0x1F
        ret = word(p + 4)
        args = [word(p + 8 + 4 * k) for k in range(nargs)]
        out.append({"id": desc & 0xFF, "name": cstr(word(NAMES + 4 * (desc >> 13))),
                    "ret": ret & 0xFF, "args": args})
        p += 8 + 4 * nargs
    seen = collections.Counter(c["name"] for c in out)
    nth = collections.Counter()
    for c in out:
        c["sym"] = c["name"]
        if seen[c["name"]] > 1:
            c["sym"] = f"{c['name']}_{nth[c['name']]}"
            nth[c["name"]] += 1
        c["case"] = word(JTBL + 4 * c["id"])
        c["impl"] = first_jal(c["case"])
    return sorted(out, key=lambda c: c["id"])


def main():
    cmds = commands()
    if "--symbols" in sys.argv:
        for c in sorted(cmds, key=lambda c: c["impl"]):
            print(f"{c['sym']} = 0x{c['impl']:08X}; // type:func")
        return
    print("| id | Command | Symbol | Args | Ret | Case | Implementation |")
    print("|---|---|---|---|---|---|---|")
    for c in cmds:
        args = ", ".join(str(a) for a in c["args"]) or "-"
        print(f"| {c['id']} | `{c['name']}` | `{c['sym']}` | {args} | {c['ret']} "
              f"| `{c['case']:08X}` | `{c['impl']:08X}` |")


if __name__ == "__main__":
    main()
