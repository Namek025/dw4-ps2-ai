#!/usr/bin/env python3
"""List the game's file table (disc/NDMW.IGB): AFS partition (0 = NDMW.AFS), entry, path.

    python3 tools/afs_names.py [--dir SUBSTR] [--name SUBSTR]
"""
import re
import subprocess
import sys

out = subprocess.run([sys.executable, "tools/igb.py", "disc/NDMW.IGB"], capture_output=True, text=True).stdout
objs = {}
for line in out.splitlines():
    if not line.startswith("{"):
        continue
    ref = int(re.search(r"'_ref': (\d+)", line).group(1))
    objs[ref] = line


def name_of(ref):
    line = objs.get(ref, "")
    m = re.search(r"'_name': '([^']*)'", line)
    return m.group(1) if m else None


def path_of(ref):
    """_pathList is an igNamedObjectList: join the names of its elements."""
    line = objs.get(ref, "")
    refs = [int(x) for x in re.findall(r"<ref (\d+)>", line)]
    return "/".join(n for n in (name_of(r) for r in refs) if n)


dsub = sys.argv[sys.argv.index("--dir") + 1] if "--dir" in sys.argv else None
nsub = sys.argv[sys.argv.index("--name") + 1] if "--name" in sys.argv else None
rows = []
for ref, line in objs.items():
    if "'_class': 'igAdxAfsFile'" not in line:
        continue
    m = re.search(r"'_name': '([^']*)', '_pathList': <ref (\d+)>, '_ptNumber': (\d+), '_fileNumber': (\d+)", line)
    if not m:
        continue
    name, pref, pt, fn = m.group(1), int(m.group(2)), int(m.group(3)), int(m.group(4))
    d = path_of(pref)
    if dsub and dsub not in d:
        continue
    if nsub and nsub not in name:
        continue
    rows.append((pt, fn, d, name))
for pt, fn, d, name in sorted(rows):
    print(f"{pt} {fn:5d} {d}/{name}")
