#!/usr/bin/env python3
"""Generate build.ninja for the DW4 decompilation.

Usage:
    python3 configure.py          # split (if needed) + write build.ninja
    python3 configure.py --clean  # wipe asm/, assets/, build/ and re-split
    ninja                         # build and check the SHA-1
"""
import argparse
import json
import re
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent
ELF_ORIG = "disc/SLUS_208.36"
ROM_TARGET = "build/target/SLUS_208.36.rom"
ELF_OUT = "build/SLUS_208.36.elf"
ROM_OUT = "build/SLUS_208.36.rom"
LD_SCRIPT = "build/SLUS_208.36.ld"
SPLAT_YAML = "config/splat.yaml"

CROSS = "mips-linux-gnu-"
AS_FLAGS = "-EL -march=r5900 -mabi=32 -G0 -no-pad-sections -Iinclude"

# Game code: EE-GCC 2.96 (see CLAUDE.md). It runs its own bundled `as`, which
# fills delay slots the same way the original build did.
CC = "tools/ee-gcc2.96/bin/ee-gcc"
CC_FLAGS = "-O2 -G0 -Iinclude -I."
# Most game code fills delay slots itself (branch-likely instructions, which
# the assembler never makes). A few units leave them to the assembler.
CC_EXTRA = {
    "src/unk_2B4330.c": "-fno-delayed-branch",
}

# Some game units were built with Metrowerks CodeWarrior (MWCC) as C++ instead:
# global loads go through $at, functions are 16-byte aligned, callee-saved regs
# are saved with sq, and no tail calls (C++ exceptions on). See CLAUDE.md.
WIBO = "tools/wibo"
MWCC_VERSION = "3.0b52-030722"
MWCC = f"tools/mwcps2/{MWCC_VERSION}/mwccps2.exe"
# -sdatathreshold 1: 1-byte compiler constants (const bool& temporaries) land in
# .sdata ($gp), while 3-byte strings like "be" do not.
MWCC_FLAGS = "-c -O4,p -sdatathreshold 1 -lang c++ -Iinclude -I."
# Lua 4.0.1 (src/lua/, stock source): optimized for size, C++ without exceptions,
# strings in .rodata even when tiny (-str readonly), and `long` as 32 bits in the core
# but not in lib/
# See docs/lua.md.
LUA_DIR = Path("src/lua")
LUA_FLAGS = ("-c -O4,s -sdatathreshold 1 -str readonly -lang c++ -Cpp_exceptions off"
             " -I- -Iinclude/libc -Iinclude -Isrc/lua")


# Alchemy engine (src/alchemy/): Intrinsic's prebuilt libraries, same compiler as the game but
# optimized for size (-O4,s, like Lua; with -O4,p a call before a branch target gets an extra nop).
ALCHEMY_DIR = Path("src/alchemy")
ALCHEMY_FLAGS = "-c -O4,s -sdatathreshold 1 -lang c++ -Iinclude -I."


def mwcc_flags(src: Path) -> str:
    if LUA_DIR in src.parents:
        return LUA_FLAGS + ("" if "lib" in src.parts else " -Dlong=int")
    if ALCHEMY_DIR in src.parents:
        return ALCHEMY_FLAGS
    return MWCC_FLAGS


def unit_name(src: Path) -> str:
    """Unit path as splat names it: src/lua/lapi.c -> lua/lapi."""
    return str(src.relative_to("src").with_suffix(""))


def make_target_rom() -> None:
    out = ROOT / ROM_TARGET
    if out.exists():
        return
    out.parent.mkdir(parents=True, exist_ok=True)
    subprocess.run(
        [f"{CROSS}objcopy", "-O", "binary", "--gap-fill=0x00", ELF_ORIG, ROM_TARGET],
        cwd=ROOT,
        check=True,
    )


def split() -> None:
    subprocess.run(
        [sys.executable, "-m", "splat", "split", SPLAT_YAML], cwd=ROOT, check=True
    )


GP_REL_LINE = re.compile(r"^(\s*/\* \w+ \w+ )([0-9A-F]{8})( \*/\s+)(.*%gp_rel\(.*)$")


def fix_nonmatchings() -> None:
    # INCLUDE_ASM files are assembled by the old EE `as`, which doesn't know
    # `%gp_rel(sym)`. Data addresses are fixed (data is still a bin), so emit
    # those instructions as raw words taken from the comment's bytes.
    for p in (ROOT / "asm" / "nonmatchings").rglob("*.s"):
        text = p.read_text()
        if "%gp_rel" not in text:
            continue
        out = []
        for line in text.splitlines():
            m = GP_REL_LINE.match(line)
            if m:
                word = int.from_bytes(bytes.fromhex(m.group(2)), "little")
                line = f"{m.group(1)}{m.group(2)}{m.group(3)}.word 0x{word:08X} /* {m.group(4).strip()} */"
            out.append(line)
        p.write_text("\n".join(out) + "\n")


def write_ninja() -> None:
    # asm/nonmatchings/ holds functions pulled in by INCLUDE_ASM from C files;
    # they are assembled as part of their C file, not on their own.
    # asm/matchings/ (already decompiled functions) is only for objdiff's target.
    asm = sorted(
        p.relative_to(ROOT)
        for p in (ROOT / "asm").rglob("*.s")
        if "nonmatchings" not in p.parts and "matchings" not in p.parts
    )
    # MWCC units: C++ sources, plus Lua (.c files the game built as C++).
    srcs = sorted(p.relative_to(ROOT) for p in (ROOT / "src").rglob("*.c")
                  if LUA_DIR not in p.relative_to(ROOT).parents)
    cpps = sorted(p.relative_to(ROOT) for p in (ROOT / "src").rglob("*.c*")
                  if p.suffix == ".cpp" or LUA_DIR in p.relative_to(ROOT).parents)
    lua_headers = " ".join(sorted(str(p.relative_to(ROOT)) for d in ("src/lua", "include/libc")
                                  for p in (ROOT / d).glob("*.h")))
    game_headers = " ".join(sorted(str(p.relative_to(ROOT)) for p in (ROOT / "include").glob("*.h")))
    bins = sorted(p.relative_to(ROOT) for p in (ROOT / "assets").rglob("*.bin"))

    lines = [
        "# Generated by configure.py. Do not edit.",
        "ninja_required_version = 1.10",
        "",
        "rule as",
        f"  command = {CROSS}as {AS_FLAGS} -o $out $in",
        "  description = AS $in",
        "",
        "rule cc",
        # The old EE `as` writes a .symtab modern ld rejects (local symbol after
        # sh_info); a pass through modern objcopy rewrites it in valid order.
        f"  command = {CC} {CC_FLAGS} $flags -Wp,-MD,$out.d -c -o $out.tmp $in"
        f" && {CROSS}objcopy $out.tmp $out && rm $out.tmp",
        "  depfile = $out.d",
        "  deps = gcc",
        "  description = CC $in",
        "",
        "rule mwcc",
        # MWCC writes one .text section per function and can't do GNU-style
        # INCLUDE_ASM; mwsplice rebuilds the unit as one .s in address order.
        f"  command = MWCIncludes=. {WIBO} {MWCC} $flags -o $out $in",
        "  description = MWCC $in",
        "",
        "rule mwsplice",
        f"  command = {sys.executable} tools/mwsplice.py $in $asmdir $out",
        "  description = SPLICE $out",
        "",
        "rule plainnames",
        f"  command = {sys.executable} tools/plainnames.py $in $out",
        "  description = PLAIN $out",
        "",
        "rule expected",
        f"  command = {sys.executable} tools/expected.py $unit $out",
        "  description = EXPECTED $out",
        "",
        "rule bin",
        # Go through `as` (not objcopy) so the object gets the same ABI flags as the code.
        f"  command = printf '.section .data\\n.incbin \"$in\"\\n'"
        f" | {CROSS}as {AS_FLAGS} -o $out -",
        "  description = BIN $in",
        "",
        "rule ld",
        f"  command = {CROSS}ld -EL -T {LD_SCRIPT}"
        " -T config/undefined_funcs_auto.txt -T config/undefined_syms_auto.txt"
        " -Map build/SLUS_208.36.map --no-check-sections --no-warn-rwx-segments"
        # GCC objects are eabi64, the splat asm is o32: same bytes, different ELF flags.
        " --no-warn-mismatch -o $out",
        "  description = LD $out",
        "",
        "rule rom",
        f"  command = {CROSS}objcopy -O binary --gap-fill=0x00 $in $out",
        "  description = ROM $out",
        "",
        "rule check",
        f"  command = cmp $in {ROM_TARGET} && sha1sum $in && touch $out",
        "  description = CHECK $in",
        "",
    ]

    objs = []
    for s in asm:
        o = Path("build") / s.with_suffix(".o")
        objs.append(str(o))
        lines.append(f"build {o}: as {s}")
    for c in srcs:
        o = Path("build") / c.with_suffix(".o")
        objs.append(str(o))
        # The depfile does not see files pulled in by INCLUDE_ASM; depend on them explicitly.
        inc = sorted((ROOT / "asm" / "nonmatchings" / unit_name(c)).glob("*.s"))
        implicit = " ".join(str(p.relative_to(ROOT)) for p in inc)
        lines.append(f"build {o}: cc {c}" + (f" | {implicit}" if implicit else ""))
        if str(c) in CC_EXTRA:
            lines.append(f"  flags = {CC_EXTRA[str(c)]}")
    for c in cpps:
        mw = Path("build") / c.with_suffix(".mw.o")
        s = Path("build") / c.with_suffix(".s")
        o = Path("build") / c.with_suffix(".o")
        objs.append(str(o))
        asmdir = Path("asm") / "nonmatchings" / unit_name(c)
        inc = sorted((ROOT / asmdir).glob("*.s"))
        implicit = " ".join(str(p.relative_to(ROOT)) for p in inc)
        headers = lua_headers if LUA_DIR in c.parents else game_headers
        lines.append(f"build {mw}: mwcc {c} | include/include_asm.h {headers}")
        lines.append(f"  flags = {mwcc_flags(c)}")
        lines.append(f"build {s}: mwsplice {mw} | tools/mwsplice.py config/symbol_addrs.txt config/splat.yaml {ROM_TARGET} {implicit}")
        lines.append(f"  asmdir = {asmdir}")
        lines.append(f"build {o}: as {s} | {implicit}")
    # objdiff: per unit, our object (base) against the original rebuilt from
    # splat's asm (target). MWCC units diff the compiler's own object.
    units = []
    for c in srcs + cpps:
        fs = sorted(p.relative_to(ROOT) for d in ("matchings", "nonmatchings")
                    for p in (ROOT / "asm" / d / unit_name(c)).glob("*.s"))
        es = Path("build") / "expected" / c.with_suffix(".s")
        eo = es.with_suffix(".o")
        lines.append(f"build {es}: expected | tools/expected.py {' '.join(map(str, fs))}")
        lines.append(f"  unit = {unit_name(c)}")
        lines.append(f"build {eo}: as {es} | {' '.join(map(str, fs))}")
        base = Path("build") / c.with_suffix(".o")
        if c in cpps:
            base = Path("build") / c.with_suffix(".plain.o")
            lines.append(f"build {base}: plainnames {base.with_suffix('').with_suffix('.mw.o')} | tools/plainnames.py")
        units.append({"name": str(c.with_suffix("")), "target_path": str(eo),
                      "base_path": str(base), "metadata": {"source_path": str(c)}})
    for b in bins:
        o = Path("build") / b.with_suffix(".o")
        objs.append(str(o))
        lines.append(f"build {o}: bin {b}")

    lines += [
        "",
        f"build {ELF_OUT}: ld | {' '.join(objs)} {LD_SCRIPT}",
        f"build {ROM_OUT}: rom {ELF_OUT}",
        f"build build/ok: check {ROM_OUT} | {ROM_TARGET}",
        "rule progress",
        f"  command = {sys.executable} tools/progress.py",
        "  description = PROGRESS",
        "  pool = console",
        "",
        "build progress: progress build/ok",
        f"build expected: phony {' '.join(u['target_path'] + ' ' + u['base_path'] for u in units)}",
        "default build/ok expected",
        "",
    ]
    (ROOT / "build.ninja").write_text("\n".join(lines))
    objdiff = {
        "$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
        "custom_make": "ninja",
        "build_target": False,
        "build_base": True,
        "watch_patterns": ["*.c", "*.cpp", "*.h", "*.s"],
        "units": units,
    }
    (ROOT / "objdiff.json").write_text(json.dumps(objdiff, indent=2) + "\n")


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--clean", action="store_true", help="remove generated files first")
    args = ap.parse_args()

    if args.clean:
        for d in ("asm", "assets", "build"):
            shutil.rmtree(ROOT / d, ignore_errors=True)

    make_target_rom()
    if args.clean or not (ROOT / "asm").exists():
        split()
        fix_nonmatchings()
    write_ninja()
    print("Wrote build.ninja. Now run: ninja")


if __name__ == "__main__":
    main()
