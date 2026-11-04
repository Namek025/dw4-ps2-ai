#!/usr/bin/env python3
"""List or extract a CRI AFS archive (AREA.AFS, NDMW.AFS).

Layout: "AFS\\0", u32 count, count x (u32 offset, u32 size), then (offset, size) of the
name table: count x 0x30-byte records (32-byte name, 6 x u16 date, u32 size).

    afs.py list disc/NDMW.AFS
    afs.py extract disc/NDMW.AFS assets/NDMW [--grep PATTERN]
"""
import os
import re
import struct
import sys


def read_entries(f):
    head = f.read(8)
    if head[:4] != b"AFS\0":
        sys.exit("not an AFS archive")
    count = struct.unpack("<I", head[4:])[0]
    table = [struct.unpack("<II", f.read(8)) for _ in range(count)]
    noff, nsize = struct.unpack("<II", f.read(8))
    names = []
    if noff and nsize:
        f.seek(noff)
        raw = f.read(nsize)
        for i in range(count):
            names.append(raw[i * 0x30:i * 0x30 + 32].split(b"\0")[0].decode("latin-1"))
    else:
        names = ["%05d.bin" % i for i in range(count)]
    return [(i, names[i], off, size) for i, (off, size) in enumerate(table)]


def main():
    cmd, path = sys.argv[1], sys.argv[2]
    with open(path, "rb") as f:
        entries = read_entries(f)
        if cmd == "list":
            for i, name, off, size in entries:
                print("%5d %08X %9d %s" % (i, off, size, name))
            return
        out = sys.argv[3]
        pat = None
        if "--grep" in sys.argv:
            pat = re.compile(sys.argv[sys.argv.index("--grep") + 1].encode())
        os.makedirs(out, exist_ok=True)
        n = 0
        for i, name, off, size in entries:
            f.seek(off)
            data = f.read(size)
            if pat and not pat.search(data):
                continue
            with open(os.path.join(out, "%05d_%s" % (i, name)), "wb") as o:
                o.write(data)
            n += 1
        print("%d files" % n)


if __name__ == "__main__":
    main()
