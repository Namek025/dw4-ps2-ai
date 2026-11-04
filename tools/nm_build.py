#!/usr/bin/env python3
"""Compile one MWCC unit with -DNON_MATCHING (its usual flags), for fcmp.py / fcov.py / fdist.py.

    nm_build.py src/UNIT.cpp [OUT]   # default build/nm_UNIT.o
    nm_build.py src/UNIT.cpp FUNC..  # compile to a temp file and print fcmp's diff for each FUNC
"""
import os
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ".")
import configure  # noqa: E402


def build(src, out):
    flags = configure.mwcc_flags(src).split() + ["-DNON_MATCHING"]
    r = subprocess.run([configure.WIBO, configure.MWCC, *flags, "-o", str(out), str(src)],
                       env={**os.environ, "MWCIncludes": "."}, capture_output=True, text=True)
    if r.returncode:
        sys.exit(r.stdout + r.stderr)


def main():
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    src = Path(sys.argv[1])
    rest = sys.argv[2:]
    if rest and not rest[0].endswith(".o"):
        with tempfile.TemporaryDirectory() as d:
            out = Path(d) / "unit.o"
            build(src, out)
            r = subprocess.run([sys.executable, "tools/fcmp.py", str(out)] + rest, capture_output=True, text=True)
            print(r.stdout + r.stderr, end="")
        return
    out = Path(rest[0] if rest else "build/nm_%s.o" % src.stem)
    build(src, out)
    print(out)


if __name__ == "__main__":
    main()
