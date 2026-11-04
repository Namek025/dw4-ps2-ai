#!/usr/bin/env python3
"""Compile a C++ function with MWCC and write it as pnach patches over an original function.

    python3 tools/mkpnach.py mods/src/chip_pickup.cpp func_003D4EA0 [MAX_SIZE]

The source defines one function named like the one it replaces (its address comes from the
name or from config/symbol_addrs.txt). Every other symbol it uses must exist in the game:
named in config/symbol_addrs.txt or called func_XXXXXXXX / D_XXXXXXXX. Code and data (float
constants, tables) are linked together at the function's address; the result has to fit in
MAX_SIZE bytes (the space up to the next function). Prints the patch lines and leaves the
disassembly in build/mods/<name>.dis for review.

Tools: WIBO / MWCC env vars (default tools/wibo and tools/mwcps2/3.0b52-030722/mwccps2.exe),
mips-linux-gnu-{ld,objcopy,objdump,nm} in PATH.
"""
import os
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
WIBO = os.environ.get("WIBO", str(ROOT / "tools/wibo"))
MWCC = os.environ.get("MWCC", str(ROOT / "tools/mwcps2/3.0b52-030722/mwccps2.exe"))
# -sdatathreshold 0: nothing in $gp small data (the patch can't add to the game's .sdata).
# No exceptions: no exception tables to place.
MWCC_FLAGS = ["-c", "-O4,s", "-sdatathreshold", "0", "-lang", "c++", "-Cpp_exceptions", "off",
              "-Iinclude", "-I."]
CROSS = "mips-linux-gnu-"
NAMED = re.compile(r"^(?:func|D|jtbl)_([0-9A-Fa-f]{8})$")


def load_symbols():
    syms = {}
    for line in (ROOT / "config/symbol_addrs.txt").read_text().splitlines():
        line = line.split("//")[0].strip()
        m = re.match(r"^(\S+)\s*=\s*(0x[0-9A-Fa-f]+)\s*;", line)
        if m:
            syms[m.group(1)] = int(m.group(2), 16)
    return syms


def address_of(name, syms):
    if name in syms:
        return syms[name]
    m = NAMED.match(name)
    if m:
        return int(m.group(1), 16)
    return None


def run(cmd, **kw):
    r = subprocess.run(cmd, capture_output=True, text=True, **kw)
    if r.returncode != 0:
        sys.exit(f"{' '.join(cmd)}\n{r.stdout}{r.stderr}")
    return r.stdout


def main():
    if len(sys.argv) < 3:
        sys.exit(__doc__)
    src = Path(sys.argv[1])
    func = sys.argv[2]
    max_size = int(sys.argv[3], 0) if len(sys.argv) > 3 else None
    syms = load_symbols()
    base = address_of(func, syms)
    if base is None:
        sys.exit(f"{func}: no address")

    out = ROOT / "build/mods"
    out.mkdir(parents=True, exist_ok=True)
    obj = out / f"{func}.o"
    elf = out / f"{func}.elf"
    binf = out / f"{func}.bin"
    lds = out / f"{func}.ld"

    env = dict(os.environ, MWCIncludes=".")
    run([WIBO, MWCC, *MWCC_FLAGS, "-o", str(obj.relative_to(ROOT)), str(src)], cwd=ROOT, env=env)

    undefined = run([CROSS + "nm", "-u", str(obj)]).split()
    undefined = [u for u in undefined if u != "U"]
    defs = []
    for name in undefined:
        a = address_of(name, syms)
        if a is None:
            sys.exit(f"{name}: unknown symbol (name it in config/symbol_addrs.txt or use func_/D_)")
        defs.append(f"{name} = 0x{a:08X};")
    lds.write_text(
        "\n".join(defs)
        + f"""
SECTIONS {{
    . = 0x{base:08X};
    .patch : {{
        *(.text) *(.text.*) *(.rodata) *(.rodata.*) *(.data) *(.data.*) *(.sdata) *(.sdata.*)
        *(.bss) *(.sbss) *(COMMON)
    }}
    /DISCARD/ : {{ *(.comment) *(.mwcats*) *(.reginfo) *(.mdebug*) *(.pdr) *(.gnu.attributes) }}
}}
""")
    run([CROSS + "ld", "-EL", "-T", str(lds), "-o", str(elf), str(obj)])

    sections = run([CROSS + "objdump", "-h", str(elf)])
    extra = [l.split()[1] for l in sections.splitlines()
             if re.match(r"^\s*\d+\s+\S", l) and l.split()[1] != ".patch"
             and int(l.split()[2], 16) != 0]
    if extra:
        sys.exit(f"sections left out of the patch: {extra}")
    nm = run([CROSS + "nm", str(elf)])
    for line in nm.splitlines():
        parts = line.split()
        if len(parts) == 3 and parts[2] == func and int(parts[0], 16) != base:
            sys.exit(f"{func} linked at 0x{parts[0]}, not at the start of the patch")

    run([CROSS + "objcopy", "-O", "binary", "-j", ".patch", str(elf), str(binf)])
    data = binf.read_bytes()
    data += b"\0" * (-len(data) % 4)
    (out / f"{func}.dis").write_text(run([CROSS + "objdump", "-d", "-z", "-mmips:5900", str(elf)]))
    if max_size is not None and len(data) > max_size:
        sys.exit(f"0x{len(data):X} bytes, more than 0x{max_size:X}")
    print(f"// {src.as_posix()}: {func}, 0x{len(data):X} bytes" +
          (f" of 0x{max_size:X}" if max_size else ""))
    for i in range(0, len(data), 4):
        word = int.from_bytes(data[i:i + 4], "little")
        print(f"patch=1,EE,{base + i:08X},word,{word:08X}")


if __name__ == "__main__":
    main()
