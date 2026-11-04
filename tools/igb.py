#!/usr/bin/env python3
"""Read Alchemy IGB files (version 6, as used by Digimon World 4).

Layout (all little endian):
  header      12 x u32: entry buf size/count, meta object buf size/count, object buf
              size/count, memory buf size/count, meta field buf size/count, 0xFADA, version
  meta fields count x (name len, major, minor), then the names
  external    self-sized block (u32 size first): external directory names
  meta objs   count x (name len, major, minor, n fields, parent, n slots), then per meta
              object its name and n x (u16 meta field index, u16 slot, u16 size)
  2 blocks    self-sized: alignment list, memory pool names
  entries     directory: count x (u32 meta object, u32 size, fields); igObjectDirEntry
              gives an object's meta object, igMemoryDirEntry a memory block's size
  index       u32 size, u32 count, u16 per reference: its directory entry. References
              0..objects-1 are the objects in buffer order, then the memory blocks
  objects     u32 root reference, then per object (u32 meta object, u32 size, fields)
  memory      the memory blocks, in order, each 4-byte aligned from the buffer start

Field names are not stored: a field is identified by its slot, which is its position in
the class's field list (parents first) as registered in the code. build/meta.json
(tools/dump_meta.py) gives those names.

    igb.py FILE [--class NAME]              # dump objects
    igb.py FILE --stats                     # per-class counts (objects, fields, strings, memory)
    igb.py FILE --db [NAME] [--csv DIR]     # dump beDataBase tables (by beDBManagerInfo name)
"""
import json
import struct
import sys
from pathlib import Path

META_JSON = Path(__file__).resolve().parent.parent / "build" / "meta.json"

SIMPLE = {
    "igIntMetaField": "<i", "igUnsignedIntMetaField": "<I", "igFloatMetaField": "<f",
    "igShortMetaField": "<h", "igUnsignedShortMetaField": "<H", "igCharMetaField": "<b",
    "igUnsignedCharMetaField": "<B", "igBoolMetaField": "<B", "igLongMetaField": "<q",
    "igUnsignedLongMetaField": "<Q", "igEnumMetaField": "<i", "igDoubleMetaField": "<d",
}
VEC = {"igVec2fMetaField": 2, "igVec3fMetaField": 3, "igVec4fMetaField": 4}
LIST_ELEM = {  # list class -> struct format of one element in its memory block
    "igIntList": "i", "igFloatList": "f", "igShortList": "h", "igCharList": "b",
    "igBoolList": "B", "igUnsignedIntList": "I", "igUnsignedCharList": "B",
    "igVec3fList": "3f", "igVec2fList": "2f", "igVec4fList": "4f",
}


def load_meta():
    if not META_JSON.exists():
        sys.exit("build/meta.json missing: run .venv/bin/python tools/dump_meta.py build/meta.json")
    by_name = {m["name"]: m for m in json.loads(META_JSON.read_text())}
    cache = {}

    def fields(name):
        if name not in cache:
            m = by_name.get(name)
            if m is None:
                cache[name] = None
            else:
                base = fields(m["parent"]) if m["parent"] else ["_meta", "_refCount"]
                cache[name] = (base or []) + [f["name"] for f in m["fields"]]
        return cache[name]
    return fields


class Ref:
    def __init__(self, igb, i):
        self.igb, self.i = igb, i

    def get(self):
        return self.igb.resolve(self.i)

    def __repr__(self):
        return "<ref %d>" % self.i


class IGB:
    def __init__(self, data, fields=None):
        self.d = d = data
        self.fields = fields or load_meta()
        (ent_buf, ent_n, mo_buf, mo_n, obj_buf, obj_n, mem_buf, mem_n,
         mf_buf, mf_n, magic, ver) = struct.unpack_from("<12I", d, 0)
        if magic != 0xFADA:
            raise ValueError("not an IGB file")
        p = 0x30
        lens = [struct.unpack_from("<I", d, p + 12 * i)[0] for i in range(mf_n)]
        q = p + 12 * mf_n
        self.mf = []
        for n in lens:
            self.mf.append(d[q:q + n].split(b"\0")[0].decode())
            q += n
        p += mf_buf
        p += struct.unpack_from("<I", d, p)[0]  # external directories
        heads = [struct.unpack_from("<6I", d, p + 24 * i) for i in range(mo_n)]
        q = p + 24 * mo_n
        self.mo = []
        for n, _, _, nf, parent, _ in heads:
            name = d[q:q + n].split(b"\0")[0].decode()
            q += n
            fl = [struct.unpack_from("<3H", d, q + 6 * k) for k in range(nf)]
            q += 6 * nf
            self.mo.append((name, [(self.mf[t], slot, size) for t, slot, size in fl]))
        p += mo_buf
        # Optional blocks, by the flags in the high bits of the version (igIGBFile_readFile):
        # external directories (v >= 5, _hasExternal), memory pool names (_hasMemoryPoolNames).
        self.version = ver & 0xFFFF
        self.flags = ver & 0xFFFF0000
        has_info = bool(ver & 0x80000000)
        has_external = bool(ver & 0x40000000)
        shared = bool(ver & 0x20000000)
        has_pool_names = bool(ver & 0x10000000)
        if has_external and self.version >= 5:
            p += struct.unpack_from("<I", d, p)[0]
        if has_pool_names:
            p += struct.unpack_from("<I", d, p)[0]
        self.entries = []
        for _ in range(ent_n):
            t, s = struct.unpack_from("<2I", d, p)
            self.entries.append((self.mo[t][0], struct.unpack_from("<%di" % ((s - 8) // 4), d, p + 8)))
            p += s
        if shared:
            s, n = struct.unpack_from("<2I", d, p)
            self.index = struct.unpack_from("<%dH" % n, d, p + 8)
            p += s
        else:  # one reference per directory entry, in order
            self.index = tuple(range(ent_n))
        self.root = None
        if has_info:
            self.root = struct.unpack_from("<I", d, p)[0]
            p += 4
        self.objects = []
        for _ in range(obj_n):
            t, s = struct.unpack_from("<2I", d, p)
            self.objects.append((t, p + 8, s - 8))
            p += s
        # A reference is a position in the index; objects and memory blocks are interleaved
        # there, and each buffer keeps its own kind in reference order.
        self.refmap = []
        no = nm = 0
        mem_sizes = []
        for e in self.index:
            name, vals = self.entries[e]
            if name == "igMemoryDirEntry":
                self.refmap.append(("mem", nm))
                mem_sizes.append(vals[1])
                nm += 1
            else:
                self.refmap.append(("obj", no))
                no += 1
        self.obj_ref = {k: r for r, (kind, k) in enumerate(self.refmap) if kind == "obj"}
        self.mem = []
        self.mem_off = []  # file offset of each memory block (for in-place patches)
        base = p  # blocks are 4-byte aligned relative to the buffer (which may be odd)
        for size in mem_sizes:
            self.mem.append(d[p:p + size])
            self.mem_off.append(p)
            p = base + ((p - base + size + 3) & ~3)
        self.cache = {}

    def resolve(self, i):
        if i < 0 or i >= len(self.refmap):
            return None
        kind, k = self.refmap[i]
        if kind == "mem":
            return self.mem[k]
        if k not in self.cache:
            self.cache[k] = self.decode(k)
        return self.cache[k]

    def decode(self, i):
        t, p, size = self.objects[i]
        cls, flds = self.mo[t]
        names = self.fields(cls)
        obj = {"_class": cls, "_ref": self.obj_ref.get(i)}
        end = p + size
        for ftype, slot, fsize in flds:
            fname = names[slot] if names and slot < len(names) else "slot%d" % slot
            if ftype in ("igStringMetaField",):
                n = struct.unpack_from("<I", self.d, p)[0]
                obj[fname] = self.d[p + 4:p + 4 + n].split(b"\0")[0].decode("latin-1") if n else None
                p += 4 + n
            elif ftype in ("igObjectRefMetaField", "igMemoryRefMetaField"):
                v = struct.unpack_from("<i", self.d, p)[0]
                obj[fname] = Ref(self, v) if v >= 0 else None
                p += 4
            elif ftype in VEC:
                obj[fname] = struct.unpack_from("<%df" % VEC[ftype], self.d, p)
                p += 4 * VEC[ftype]
            elif ftype in SIMPLE:
                obj[fname] = struct.unpack_from(SIMPLE[ftype], self.d, p)[0]
                p += max(4, struct.calcsize(SIMPLE[ftype]))
            else:
                obj[fname] = self.d[p:p + fsize].hex()
                p += fsize
        if p != end:
            obj["_undecoded"] = self.d[p:end].hex()
        return obj

    def list_items(self, obj):
        """Elements of an igDataList-derived object (_count, _data memory block)."""
        mem = obj.get("_data")
        if mem is None:
            return []
        raw = mem.get()
        n = obj.get("_count", 0)
        fmt = LIST_ELEM.get(obj["_class"])
        if fmt is None:  # object lists: one reference per element
            return [Ref(self, r) if r >= 0 else None for r in struct.unpack_from("<%di" % n, raw)]
        k = struct.calcsize("<" + fmt)
        out = [struct.unpack_from("<" + fmt, raw, j * k) for j in range(n)]
        return [x[0] if len(x) == 1 else x for x in out]

    def all(self, cls=None):
        for k, (t, _, _) in enumerate(self.objects):
            if cls is None or self.mo[t][0] == cls:
                if k not in self.cache:
                    self.cache[k] = self.decode(k)
                yield self.cache[k]


DB_GROUPS = ("char", "short", "int", "float", "bool", "string")


def read_db(igb, db):
    """beDataBase -> (columns, rows). Value index = column + columns_in_group * record
    (beDataBase_getIndex); strings are igNamedObjects (their _name)."""
    cols, data = [], []
    for g in DB_GROUPS:
        names = [r.get()["_name"] for r in igb.list_items(db["_%sOffsetList" % g].get())]
        vals = igb.list_items(db["_%sData" % g].get()) if db["_%sData" % g] else []
        if g == "string":
            vals = [r.get()["_name"] if r else None for r in vals]
        cols.append((g, names, vals))
    nrec = 0
    for g, names, vals in cols:
        if names:
            nrec = len(vals) // len(names)
            break
    header = [n for g, names, vals in cols for n in names]
    rows = []
    for r in range(nrec):
        row = []
        for g, names, vals in cols:
            n = len(names)
            row.extend(vals[c + n * r] for c in range(n))
        rows.append(row)
    return header, rows


def databases(igb):
    """name -> beDataBase for every beDBManagerInfo in the file."""
    out = {}
    for info in igb.all("beDBManagerInfo"):
        if info.get("_dataBase"):
            out[info["_name"]] = info["_dataBase"].get()
    return out


def stats(igb):
    """Load-cost summary: objects, fields, strings and memory blocks per class."""
    from collections import defaultdict
    d = igb.d
    hdr = struct.unpack_from("<12I", d, 0)
    print("version %d, flags %08X" % (igb.version, igb.flags))
    print("file %d bytes; entries %d (%d B), meta objects %d (%d B), objects %d (%d B), "
          "memory blocks %d (%d B), meta fields %d (%d B)"
          % (len(d), hdr[1], hdr[0], hdr[3], hdr[2], hdr[5], hdr[4], hdr[7], hdr[6], hdr[9], hdr[8]))
    per = defaultdict(lambda: [0, 0, 0, 0, 0, 0])  # objects, obj bytes, fields, strings, mem blocks, mem bytes
    owned = set()
    for k, (t, _, size) in enumerate(igb.objects):
        cls, flds = igb.mo[t]
        row = per[cls]
        row[0] += 1
        row[1] += size
        row[2] += len(flds)
        row[3] += sum(1 for f, _, _ in flds if f == "igStringMetaField")
        o = igb.decode(k)
        for v in o.values():
            if isinstance(v, Ref):
                kind, m = igb.refmap[v.i] if 0 <= v.i < len(igb.refmap) else (None, None)
                if kind == "mem" and m not in owned:
                    owned.add(m)
                    row[4] += 1
                    row[5] += len(igb.mem[m])
    tot = [sum(r[i] for r in per.values()) for i in range(6)]
    print("%-40s %7s %9s %8s %7s %6s %10s" % ("class", "objs", "obj B", "fields", "strings", "mems", "mem B"))
    for cls, r in sorted(per.items(), key=lambda kv: -(kv[1][1] + kv[1][5])):
        print("%-40s %7d %9d %8d %7d %6d %10d" % (cls, *r))
    print("%-40s %7d %9d %8d %7d %6d %10d" % ("TOTAL", *tot))
    print("memory blocks not owned by a field: %d (%d B)"
          % (len(igb.mem) - len(owned), sum(len(b) for i, b in enumerate(igb.mem) if i not in owned)))


def main():
    path = sys.argv[1]
    cls = sys.argv[sys.argv.index("--class") + 1] if "--class" in sys.argv else None
    igb = IGB(Path(path).read_bytes())
    if "--stats" in sys.argv:
        stats(igb)
        return
    if "--db" in sys.argv:
        import csv
        k = sys.argv.index("--db") + 1
        want = sys.argv[k] if k < len(sys.argv) and not sys.argv[k].startswith("--") else None
        outdir = Path(sys.argv[sys.argv.index("--csv") + 1]) if "--csv" in sys.argv else None
        for name, db in databases(igb).items():
            if want and name != want:
                continue
            header, rows = read_db(igb, db)
            print("%s: %d records, %d columns" % (name, len(rows), len(header)))
            if outdir:
                outdir.mkdir(parents=True, exist_ok=True)
                with open(outdir / ("%s.csv" % name), "w", newline="") as f:
                    w = csv.writer(f)
                    w.writerow(["#"] + header)
                    for i, row in enumerate(rows):
                        w.writerow([i] + row)
            elif want:
                print(",".join(["#"] + header))
                for i, row in enumerate(rows):
                    print(",".join(str(x) for x in [i] + row))
        return
    for o in igb.all(cls):
        shown = {k: (v if not isinstance(v, Ref) else v) for k, v in o.items()}
        if "_data" in o and o["_data"] is not None and "_count" in o:
            shown["_items"] = igb.list_items(o)
        print(shown)


if __name__ == "__main__":
    main()
