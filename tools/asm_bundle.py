#!/usr/bin/env python3
"""Move function asm between the full environment and a machine without the ELF.

export (full env): writes a text bundle with, per function, its splat asm, the m2c
draft and the data it references (16 bytes at each D_XXXXXXXX: hex, f32, string).
import (no ELF): splits a bundle back into asm/nonmatchings/UNIT/FUNC.s and
build/m2c/FUNC.c (+ FUNC.data.txt), and writes the instruction bytes into the
sparse build/target/SLUS_208.36.rom, so fcmp/fdist can compare against them.

Usage:
  python3 tools/asm_bundle.py export FUNC... [-o FILE]
  python3 tools/asm_bundle.py export --pending [--unit U] [--max-size B] [-o FILE]
  python3 tools/asm_bundle.py import FILE
"""
import argparse
import re
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
ROM = ROOT / "build/target/SLUS_208.36.rom"
BASE = 0x100000
INSN = re.compile(r"/\* [0-9A-F]+ ([0-9A-F]{8}) ([0-9A-F]{8}) \*/")


def find_asm(func):
    for p in (ROOT / "asm" / "nonmatchings").rglob(f"{func}.s"):
        return p
    sys.exit(f"{func}: no asm/nonmatchings/**/{func}.s")


def data_lines(body, rom):
    out = []
    for a in sorted(set(re.findall(r"\bD_([0-9A-F]{8})", body))):
        o = int(a, 16) - BASE
        b = rom[o:o + 16] if 0 <= o < len(rom) else b""
        if not b:
            continue
        f = struct.unpack("<4f", b.ljust(16, b"\0"))
        e = b.find(b"\0")
        s = b[:e if e >= 0 else 16]
        txt = s.decode() if s and all(32 <= c < 127 for c in s) else ""
        out.append(f"D_{a}: {b.hex(' ')} | f32 {f[0]:g} {f[1]:g} {f[2]:g} {f[3]:g}"
                   + (f' | "{txt}"' if txt else ""))
    return out


def export(args):
    funcs = list(args.funcs)
    if args.pending:
        sys.path.insert(0, str(ROOT / "tools"))
        from agent_loop import pending
        ns = argparse.Namespace(unit=args.unit, max_size=args.max_size, retry=False)
        funcs += [i["func"] for i in pending(ns)]
    rom = ROM.read_bytes()
    parts = []
    for func in funcs:
        p = find_asm(func)
        unit = p.parent.relative_to(ROOT / "asm" / "nonmatchings").as_posix()
        body = p.read_text()
        parts.append(f"### asm {unit} {func}\n{body.rstrip()}\n")
        m2c = subprocess.run([sys.executable, "tools/m2c_func.py", func], cwd=ROOT,
                             capture_output=True, text=True).stdout
        parts.append(f"### m2c {unit} {func}\n{m2c.rstrip()}\n")
        parts.append(f"### data {unit} {func}\n" + "\n".join(data_lines(body, rom)) + "\n")
    text = "".join(parts)
    if args.o:
        Path(args.o).write_text(text)
        print(f"{len(funcs)} functions -> {args.o} ({len(text)} bytes)")
    else:
        sys.stdout.write(text)


def do_import(args):
    text = Path(args.file).read_text()
    rom = bytearray(ROM.read_bytes()) if ROM.exists() else bytearray()
    n = 0
    for kind, unit, func, body in re.findall(r"^### (asm|m2c|data) (\S+) (\S+)\n(.*?)(?=^### |\Z)",
                                             text, re.S | re.M):
        if kind == "asm":
            p = ROOT / "asm" / "nonmatchings" / unit / f"{func}.s"
            p.parent.mkdir(parents=True, exist_ok=True)
            p.write_text(body)
            for addr, word in INSN.findall(body):
                o = int(addr, 16) - BASE
                if len(rom) < o + 4:
                    rom.extend(b"\0" * (o + 4 - len(rom)))
                rom[o:o + 4] = bytes.fromhex(word)
            n += 1
        else:
            p = ROOT / "build" / "m2c" / (f"{func}.c" if kind == "m2c" else f"{func}.data.txt")
            p.parent.mkdir(parents=True, exist_ok=True)
            p.write_text(body)
    ROM.parent.mkdir(parents=True, exist_ok=True)
    ROM.write_bytes(rom)
    print(f"{n} functions imported; rom {len(rom)} bytes")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    e = sub.add_parser("export")
    e.add_argument("funcs", nargs="*")
    e.add_argument("--pending", action="store_true")
    e.add_argument("--unit")
    e.add_argument("--max-size", type=int)
    e.add_argument("-o")
    i = sub.add_parser("import")
    i.add_argument("file")
    args = ap.parse_args()
    export(args) if args.cmd == "export" else do_import(args)


if __name__ == "__main__":
    main()
