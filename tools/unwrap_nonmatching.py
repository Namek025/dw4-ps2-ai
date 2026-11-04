#!/usr/bin/env python3
"""Turn NON_MATCHING functions back into plain C once they match.

Undoes `#ifdef NON_MATCHING <C> #else <prototype> INCLUDE_ASM(..., NAME); #endif`
for each NAME given, keeping the C. Also restores forward declarations that were
wrapped the same way (static in C, global for the asm build).

Usage: python3 tools/unwrap_nonmatching.py FILE NAME [NAME ...]
"""
import re
import sys
from pathlib import Path


def main():
    path, names = Path(sys.argv[1]), sys.argv[2:]
    s = path.read_text()
    for name in names:
        # The body may not cross another block's #ifdef/#else/#endif.
        body = r"((?:(?!^#ifdef NON_MATCHING$|^#else$|^#endif$).*\n)*?)"
        func = re.compile(r"^#ifdef NON_MATCHING\n" + body + r"#else\n[^\n]*\nINCLUDE_ASM\(\"[^\"]+\", "
                          + re.escape(name) + r"\);\n#endif\n", re.M)
        s, n = func.subn(lambda m: m.group(1), s)
        if n != 1:
            sys.exit(f"{path}: {name}: found {n} NON_MATCHING blocks")
        decl = re.compile(r"#ifdef NON_MATCHING\n(static [^\n]*\b" + re.escape(name)
                          + r"\s*\([^#]*?;)\n#else\n[^#]*?\n#endif\n", re.S)
        s = decl.sub(lambda m: m.group(1) + "\n", s)
    if "INCLUDE_ASM" not in s:
        s = s.replace('#include "include_asm.h"\n', "")
    path.write_text(s)


if __name__ == "__main__":
    main()
