#!/usr/bin/env python3
"""Try every order of the declarations marked /*P*/ in one function.

MWCC hands out s-registers following declaration order, so permuted registers
are often fixed by reordering locals. Mark the lines (one declaration each) with
/*P*/, then run this: it rebuilds the unit's MWCC object for each order, keeps
the best one in the source, and stops at a match. Remove the markers after.

Usage: python3 tools/permute_decls.py src/UNIT.cpp FUNC [--greedy]

--greedy: for many declarations, move one line at a time to every other slot
and keep improvements until nothing helps (instead of every permutation).
"""
import itertools
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent))
from fdist import distance  # noqa: E402


def main():
    src, func = Path(sys.argv[1]), sys.argv[2]
    obj = f"build/{src.with_suffix('.mw.o')}"
    text = src.read_text()
    m = re.search(r"^[^\n]*\b" + re.escape(func) + r"\([^;]*?\)\s*\{\n(.*?)^\}", text, re.S | re.M)
    if not m:
        sys.exit(f"{func} not found in {src}")
    lines = m.group(1).split("\n")
    idx = [i for i, l in enumerate(lines) if "/*P*/" in l]
    def score(perm):
        ls = list(lines)
        for i, d in zip(idx, perm):
            ls[i] = d
        t = text[:m.start(1)] + "\n".join(ls) + text[m.end(1):]
        src.write_text(t)
        ok = subprocess.run(["ninja", obj], capture_output=True).returncode == 0
        d = distance(obj, func)[0] if ok else None
        return (10**6 if d is None else d), t

    best = None
    decls = [lines[i] for i in idx]
    if "--greedy" in sys.argv:
        cur = decls
        best = score(cur)
        print(best[0], flush=True)
        improved = True
        while improved and best[0]:
            improved = False
            for a in range(len(cur)):
                for b in range(len(cur)):
                    if a == b:
                        continue
                    cand = list(cur)
                    cand.insert(b, cand.pop(a))
                    r = score(cand)
                    if r[0] < best[0]:
                        best, cur, improved = r, cand, True
                        print(best[0], [x.strip() for x in cur], flush=True)
                        break
                if improved or not best[0]:
                    break
    else:
        for perm in itertools.permutations(decls):
            d, t = score(perm)
            if best is None or d < best[0]:
                best = (d, t)
                print(d, [x.strip() for x in perm], flush=True)
            if d == 0:
                break
    src.write_text(best[1])
    subprocess.run(["ninja", obj], capture_output=True)
    print("best", best[0])


if __name__ == "__main__":
    main()
