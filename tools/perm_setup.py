#!/usr/bin/env python3
"""Set up a decomp-permuter directory for one function of an MWCC C++ unit.

    tools/perm_setup.py src/alchemy/igIGBFile.cpp func_002E7390
    tools/decomp-permuter/permuter.py permuter/func_002E7390 -j8

The permuter only parses C, so the rest of the unit goes into PERM_IGNORE (passed
through untouched), the type names the function uses are declared to the parser with
PERM_PRETEND. The rest of the unit stays as it is (MWCC's code depends on what it knows
about the callees in the same file); scoring looks at this function only. The function's C is taken from its
`#ifdef NON_MATCHING` block, or from a plain definition.
"""
import os
import re
import stat
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MWCC = "tools/mwcps2/3.0b52-030722/mwccps2.exe"
FLAGS = {
    "alchemy/": "-c -O4,s -sdatathreshold 1 -lang c++ -Iinclude -I.",
    "": "-c -O4,p -sdatathreshold 1 -lang c++ -Iinclude -I.",
}
DEF_RE = re.compile(r"^(?!static|template|inline)[A-Za-z_][\w\s\*&<>]*?\b(\w+)\(([^;{]*)\)\s*\{\s*$")


def split_function(lines, i):
    """Index of the closing `}` (column 0) of the definition starting at line i."""
    j = i
    while lines[j].rstrip() != "}":
        j += 1
    return j


def build_prelude(src, func):
    lines = src.split("\n")
    out = []
    target = None
    i = 0
    while i < len(lines):
        line = lines[i]
        if line.startswith("INCLUDE_ASM("):
            i += 1
            continue
        if line.strip() == "#ifdef NON_MATCHING":
            # #ifdef NON_MATCHING <comment> <definition> #else INCLUDE_ASM #endif
            j = i + 1
            while lines[j].strip() != "#else":
                j += 1
            k = j
            while lines[k].strip() != "#endif":
                k += 1
            block = lines[i + 1:j]
            for b, l in enumerate(block):
                m = DEF_RE.match(l)
                if m and m.group(1) == func:
                    target = "\n".join(block[b:])
                    out.append("int PERM_SPLIT_MARKER;")
                    break
            if target is None or not any(DEF_RE.match(l) and DEF_RE.match(l).group(1) == func for l in block):
                out.extend(lines[i:k + 1])
            i = k + 1
            continue
        m = DEF_RE.match(line)
        if m:
            end = split_function(lines, i)
            if m.group(1) == func:
                target = "\n".join(lines[i:end + 1])
                out.append("int PERM_SPLIT_MARKER;")
            else:
                out.extend(lines[i:end + 1])
            i = end + 1
            continue
        out.append(line)
        i += 1
    if target is None:
        sys.exit(f"{func}: no C definition found")
    return "\n".join(out), target


PRIMS = {"s8": "signed char", "u8": "unsigned char", "s16": "short", "u16": "unsigned short",
         "s32": "int", "u32": "unsigned int", "f32": "float", "bool": "unsigned char"}


def parse_meta():
    """struct name -> (base, [(type, field)]) from include/meta.h."""
    structs = {}
    cur = None
    for line in open("include/meta.h"):
        m = re.match(r"struct (\w+)(?: : (\w+))? \{", line)
        if m:
            cur = (m.group(1), m.group(2), [])
            structs[cur[0]] = cur[1:]
            continue
        if cur and line.startswith("};"):
            cur = None
            continue
        if cur:
            m = re.match(r"\s+(.+?)\s*\b(\w+)(\[[^\]]*\])?;", line)
            if m:
                cur[2].append((m.group(1).strip(), m.group(2) + (m.group(3) or "")))
    return structs


def pretend_types(target, unit_types=frozenset()):
    """C declarations that let pycparser (and the randomizer) understand the function."""
    meta = parse_meta()
    words = set(re.findall(r"\b\w+\b", target))
    methods = set(re.findall(r"->\s*(\w+)\s*\(", target))
    used = {w for w in words if w in meta}
    refs = {w for w in words if w.endswith("Ref") and w[:-3] in meta}
    vcls = {w for w in words if re.match(r"(ig|be)\w+V$", w)}
    used |= {r[:-3] for r in refs}
    # closure over pointer fields and bases
    todo = list(used)
    while todo:
        t = todo.pop()
        base, fields = meta[t]
        for x in [base] + [ft.rstrip("*").strip() for ft, _ in fields]:
            if x and x in meta and x not in used:
                used.add(x)
                todo.append(x)

    def flat(t):
        base, fields = meta[t]
        return (flat(base) if base else [("void*", "__vtbl"), ("int", "_refCount")] if t == "igObject" else []) + fields

    out = ["typedef %s %s;" % (c, n) for n, c in PRIMS.items()]
    out += [f"typedef struct {t} {t};" for t in sorted(used | refs | vcls | {"igResult"})]
    for t in sorted(used):
        body = []
        for ft, fn in flat(t):
            if t == "igObject" and fn in ("__vtbl", "_refCount"):
                body.append(f"{ft} {fn};")
                continue
            core = ft.rstrip("*").strip()
            if core not in PRIMS and core not in used:
                ft = "void" + ft[len(core):] if ft.endswith("*") else "int"
            body.append(f"{ft} {fn};")
        out.append(f"struct {t} {{ " + " ".join(body) + " };")
    for r in sorted(refs):
        out.append(f"struct {r} {{ {r[:-3]}* p; }};")
    for v in sorted(vcls):
        out.append(f"struct {v} {{ " + " ".join(f"int (*{m})();" for m in sorted(methods)) + " };")
    out.append("struct igResult { int code; };")
    # unit-local types (typedef'd structs like DataDesc, StrHeader, or the unit's own
    # `struct beXxx { ... }`): opaque, pycparser only needs the name to parse declarations
    # and casts
    known = set(PRIMS) | used | refs | vcls | {"igResult"}
    local = {w for w in re.findall(r"\b([A-Z]\w*)\s*\*", target) if w not in known and not w.startswith("D_")}
    local |= {w for w in re.findall(r"\b(\w+)\s*\*", target) if w in unit_types and w not in known}
    for t in sorted(local):
        out.append(f"typedef struct {t} {{ int _unk; }} {t};")
    # functions and globals, so that the randomizer knows every identifier
    kw = {"if", "for", "while", "switch", "return", "sizeof", "do"}
    calls = set(re.findall(r"(?<![>.\w])(\w+)\s*\(", target)) - kw - set(PRIMS) - used - refs - vcls
    for f in sorted(calls):
        if not re.match(r"(ig|be)\w+V$", f):
            out.append(f"int {f}();")
    for g in sorted(set(re.findall(r"\b(D_[0-9A-F]{8})\b", target)) - calls):
        out.append(f"int {g};")
    return "\n".join(out) + "\n"


def main():
    if len(sys.argv) != 3:
        sys.exit(__doc__)
    unit, func = sys.argv[1], sys.argv[2]
    os.chdir(ROOT)
    prelude, target = build_prelude(open(unit).read(), func)
    # The permuter runs cpp on its input: hand it MWCC-preprocessed code with no directives.
    tmp = os.path.join("build", f"perm_{func}.cpp")
    before, after = prelude.split("int PERM_SPLIT_MARKER;")
    open(tmp, "w").write(f"{before}\nint PERM_SPLIT_MARKER;\n{target}\nint PERM_SPLIT_MARKER;\n{after}\n")
    pp = subprocess.run(f"MWCIncludes=. tools/wibo {MWCC} -E -lang c++ -Iinclude -I. {tmp}", shell=True,
                        check=True, capture_output=True, text=True).stdout
    os.remove(tmp)
    pp = "\n".join(l for l in pp.split("\n") if not l.startswith("/* #line") and l.strip())
    before, target, after = pp.split("int PERM_SPLIT_MARKER;")
    pretend = pretend_types(target, set(re.findall(r"\bstruct\s+(\w+)\s*[{:]", open(unit).read())))
    names = pretend.split("\n")
    base = (f"PERM_IGNORE(\n{before}\n)\n"
            f"PERM_PRETEND(\n{pretend})\n"
            f"{target}\n"
            f"PERM_IGNORE(\n{after}\n)\n")
    d = os.path.join("permuter", func)
    os.makedirs(d, exist_ok=True)
    open(os.path.join(d, "base.c"), "w").write(base)
    flags = next(v for k, v in FLAGS.items() if unit.startswith(os.path.join("src", k)) or k == "")
    sh = os.path.join(d, "compile.sh")
    open(sh, "w").write(
        "#!/bin/bash\n"
        'IN=$(realpath "$1"); OUT=$(realpath "$3")\n'
        f"cd {ROOT}\n"
        'IN=$(realpath --relative-to=. "$IN"); OUT=$(realpath --relative-to=. "$OUT")\n'
        'cp "$IN" "$IN.cpp"\n'
        f'MWCIncludes=. tools/wibo {MWCC} {flags} -o "$OUT" "$IN.cpp"\n'
        'rc=$?; rm -f "$IN.cpp"; exit $rc\n')
    os.chmod(sh, os.stat(sh).st_mode | stat.S_IEXEC)
    asm = os.path.join("asm", "nonmatchings", os.path.splitext(os.path.relpath(unit, "src"))[0], func + ".s")
    s = os.path.join(d, "target.s")
    open(s, "w").write(f'.include "labels.inc"\n.set noat\n.set noreorder\n.set gp=64\n.section .text\n'
                       f'.include "{asm}"\n')
    subprocess.run(["mips-linux-gnu-as", "-EL", "-march=r5900", "-mabi=32", "-G0", "-no-pad-sections",
                    "-Iinclude", "-o", os.path.join(d, "target.o"), s], check=True)
    # only this function (MWCC also emits out-of-line copies of inline helpers), as R5900
    open(os.path.join(d, "settings.toml"), "w").write(
        f'func_name = "{func}"\ncompiler_type = "mwcc"\n'
        f'objdump_command = "mips-linux-gnu-objdump -drz -m mips:5900 --disassemble={func}"\n')
    print(f"{d}: ok ({len(names)} pretend lines)")


if __name__ == "__main__":
    main()
