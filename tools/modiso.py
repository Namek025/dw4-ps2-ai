#!/usr/bin/env python3
"""Copy a DW4 disc image and replace AFS entries inside the copy.

    modiso.py IN.iso OUT.iso NDMW.AFS:ENTRY=FILE [...]

The file is written over the entry in place (AFS entries are 0x800-aligned; the new data
must fit before the next entry) and the entry's size in the AFS table is updated. The ISO's
directory is not touched, so the AFS keeps its size and position.
"""
import shutil
import struct
import sys

SECTOR = 0x800


def find_file(iso, name):
    """(byte offset, size) of a root-directory file in an ISO 9660 image."""
    iso.seek(16 * SECTOR + 156)  # primary volume descriptor: root directory record
    root = iso.read(34)
    lba, size = struct.unpack_from("<I", root, 2)[0], struct.unpack_from("<I", root, 10)[0]
    iso.seek(lba * SECTOR)
    data = iso.read(size)
    p = 0
    while p < len(data):
        n = data[p]
        if n == 0:  # records don't cross sectors
            p = (p // SECTOR + 1) * SECTOR
            continue
        flba, fsize = struct.unpack_from("<I", data, p + 2)[0], struct.unpack_from("<I", data, p + 10)[0]
        nlen = data[p + 32]
        fname = data[p + 33:p + 33 + nlen].decode("latin-1").split(";")[0]
        if fname.upper() == name.upper():
            return flba * SECTOR, fsize
        p += n
    raise SystemExit("%s not found in the ISO root" % name)


def main():
    if len(sys.argv) < 4:
        sys.exit(__doc__)
    src, dst = sys.argv[1], sys.argv[2]
    if src != dst:
        shutil.copyfile(src, dst)
    with open(dst, "r+b") as iso:
        for spec in sys.argv[3:]:
            target, path = spec.split("=", 1)
            afs_name, entry = target.split(":")
            entry = int(entry)
            base, afs_size = find_file(iso, afs_name)
            iso.seek(base)
            head = iso.read(8)
            assert head[:4] == b"AFS\0", "not an AFS"
            count = struct.unpack_from("<I", head, 4)[0]
            iso.seek(base + 8)
            table = [struct.unpack("<II", iso.read(8)) for _ in range(count)]
            off, size = table[entry]
            limit = table[entry + 1][0] if entry + 1 < count else afs_size
            room = limit - off
            data = open(path, "rb").read()
            if len(data) > room:
                raise SystemExit("%s: %d bytes, entry %d has room for %d" % (path, len(data), entry, room))
            iso.seek(base + off)
            iso.write(data + b"\0" * max(0, size - len(data)))
            iso.seek(base + 8 + 8 * entry + 4)
            iso.write(struct.pack("<I", len(data)))
            print("%s entry %d: %d -> %d bytes (room %d)" % (afs_name, entry, size, len(data), room))


if __name__ == "__main__":
    main()
