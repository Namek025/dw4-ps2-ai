#!/usr/bin/env python3
"""Write a ready-to-paste decomp.me source for one function of an MWCC unit: the unit
preprocessed with MWCC (-DNON_MATCHING, so the function's C is in), without line markers,
cut right after the function. Every type, helper and extern it needs comes along, so the
scratch needs no Context. Target asm: asm/nonmatchings/<unit>/<FUNC>.s.
Compiler flags to use on decomp.me are printed.

    decompme_func.py src/UNIT.cpp FUNC [OUT]   # default build/decompme_FUNC.cpp
"""
import os
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ".")
import configure  # noqa: E402

PRELUDE = "/* decomp.me source: %s preprocessed up to %s (tools/decompme_func.py). */\n#define NULL 0\n"


def main():
    src, func = Path(sys.argv[1]), sys.argv[2]
    out = sys.argv[3] if len(sys.argv) > 3 else "build/decompme_%s.cpp" % func
    flags = configure.mwcc_flags(src).split()
    defines = [f for f in flags if f.startswith(("-D", "-I"))]
    pp = subprocess.run([configure.WIBO, configure.MWCC, "-E", "-lang", "c++", "-DNON_MATCHING",
                         *defines, "-Iinclude", "-I.", str(src)],
                        env={**os.environ, "MWCIncludes": "."}, capture_output=True, text=True)
    if pp.returncode:
        sys.exit(pp.stdout + pp.stderr)
    lines = [l.rstrip() for l in pp.stdout.splitlines()
             if l.strip() and not l.startswith(("#", "/* #line"))]
    text = "\n".join(lines) + "\n"
    m = re.search(r"^[^\n;]*\b%s\([^;{]*\)\s*\{" % re.escape(func), text, re.M)
    if not m:
        sys.exit("%s: no C body for %s (is it under #ifdef NON_MATCHING?)" % (src, func))
    def braces(start, end, depth):
        """Scan text[start:end] (or until depth hits 0 when end is None); return (pos, depth)."""
        i = start
        while (i < end) if end is not None else depth:
            c = text[i]
            if c in "\"'":
                i = text.index(c, i + 1)
                while text[i - 1] == "\\":
                    i = text.index(c, i + 1)
            depth += {"{": 1, "}": -1}.get(c, 0)
            i += 1
        return i, depth

    # Blocks still open around the function (extern "C" { ... }) get closed after it.
    _, open_blocks = braces(0, m.start(), 0)
    # MWCC -E drops indentation: find the body's end by counting braces, then re-indent it.
    i, _ = braces(m.end(), None, 1)
    body, depth = [], 0
    for line in text[m.start():i].split("\n"):
        line = line.strip()
        depth -= line.startswith("}")
        body.append("    " * (depth - line.startswith(("case ", "default:"))) + line)
        depth += line.count("{") - line.count("}") + line.startswith("}")
    os.makedirs(os.path.dirname(out), exist_ok=True)
    with open(out, "w") as f:
        f.write(PRELUDE % (src, func) + text[:m.start()] + "\n".join(body) + "\n" + "}\n" * open_blocks)
    print("%s: %d KB" % (out, os.path.getsize(out) // 1024))
    print("target asm: asm/nonmatchings/%s/%s.s" % (src.stem, func))
    print("flags: %s" % " ".join(f for f in flags if not f.startswith(("-D", "-I"))))


if __name__ == "__main__":
    main()
