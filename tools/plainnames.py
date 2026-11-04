#!/usr/bin/env python3
"""Copy an MWCC object with C++ function names made plain, for objdiff.

The target objects (from splat's asm) use plain names; MWCC mangles C++ ones
(`luaD_call__FP9lua_Statei`). Same rule as tools/mwsplice.py.

Usage: plainnames.py IN.o OUT.o
"""
import subprocess
import sys
import tempfile

sys.path.insert(0, "tools")
from mwsplice import KNOWN, Obj, demangle  # noqa: E402


def main():
    src, out = sys.argv[1:]
    renames = {n: demangle(n) for n, _, _, _, _ in Obj(src).syms
               if n and n not in KNOWN and demangle(n) != n}
    with tempfile.NamedTemporaryFile("w", suffix=".map") as f:
        f.write("".join(f"{a} {b}\n" for a, b in renames.items()))
        f.flush()
        subprocess.run(["mips-linux-gnu-objcopy", f"--redefine-syms={f.name}", src, out], check=True)


if __name__ == "__main__":
    main()
