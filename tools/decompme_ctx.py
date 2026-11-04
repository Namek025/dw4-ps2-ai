#!/usr/bin/env python3
"""Write a decomp.me context for the MWCC C++ units: include/game.h preprocessed with MWCC
(common.h types, alchemy.h, every class from meta.h, the beMessenger_* prototypes), without
line markers. Paste it into the scratch's Context; compile with
`-O4,p -sdatathreshold 1 -lang c++` (game) or `-O4,s ...` (Alchemy).

    decompme_ctx.py [OUT]   # default build/decompme_ctx.cpp
"""
import os
import subprocess
import sys

MWCC = "tools/mwcps2/3.0b52-030722/mwccps2.exe"

# Macros are gone after preprocessing; put back the ones the code itself uses.
PRELUDE = """/* decomp.me context: include/game.h preprocessed (tools/decompme_ctx.py). */
#define NULL 0
"""


def main():
    out = sys.argv[1] if len(sys.argv) > 1 else "build/decompme_ctx.cpp"
    src = "build/decompme_ctx_in.cpp"
    os.makedirs(os.path.dirname(out), exist_ok=True)
    with open(src, "w") as f:
        f.write('#include "game.h"\n')
    pp = subprocess.run(f"MWCIncludes=. tools/wibo {MWCC} -E -lang c++ -Iinclude -I. {src}",
                        shell=True, capture_output=True, text=True, check=True).stdout
    os.remove(src)
    lines = []
    for line in pp.splitlines():
        if line.startswith(("#", "/* #line")) or not line.strip():
            continue
        lines.append(line.rstrip())
    with open(out, "w") as f:
        f.write(PRELUDE + "\n".join(lines) + "\n")
    print("%s: %d lines, %d KB" % (out, len(lines), os.path.getsize(out) // 1024))


if __name__ == "__main__":
    main()
