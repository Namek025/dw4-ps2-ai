#!/usr/bin/env python3
"""Dump the Alchemy reflection data (meta objects) found in the code.

Every Alchemy class registers a meta object at startup through func_002FE9C0
(name, size, parent, constructor, and a function that registers its fields).
The field function calls func_002FDF40(meta, types, n) and
func_002FE000(meta, names, field_metas, offsets, first), where types[] are
getters of the field meta classes (igIntMetaField, ...), names[] C strings and
offsets[] byte offsets in the instance. Together they give the layout of every
registered class, with the original field names.

Reads splat's asm (asm/) and the target rom. Writes docs/meta_classes.md and
a JSON with everything (for scripts).

Usage: python3 tools/dump_meta.py [--symbols] [OUT.json]
"""
import json
import re
import struct
import sys
from pathlib import Path

ROM = Path("build/target/SLUS_208.36.rom").read_bytes()
BASE = 0x100000
REG_META = 0x2FE9C0    # metaRegister
REG_TYPES = 0x2FDF40   # metaSetFieldTypes
REG_FIELDS = 0x2FE000  # metaSetFieldNames
GET_FIELD = 0x2FDF00   # metaGetField(meta, index)

REGS = {"$a0": 4, "$a1": 5, "$a2": 6, "$a3": 7, "$t0": 8, "$t1": 9, "$t2": 10, "$t3": 11,
        "$v0": 2, "$v1": 3, "$zero": 0, "$at": 1}
for i in range(32):
    REGS[f"${i}"] = i
for i, n in enumerate(["$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7"]):
    REGS[n] = 16 + i


def word(a):
    o = a - BASE
    return struct.unpack("<I", ROM[o:o + 4])[0]


def cstr(a):
    o = a - BASE
    if not 0 <= o < len(ROM):
        return None
    e = ROM.find(b"\0", o)
    s = ROM[o:e]
    if not s or len(s) > 200 or not all(32 <= c < 127 for c in s):
        return None
    return s.decode()


SYMS = {m.group(1): int(m.group(2), 16) for m in re.finditer(
    r"^(\w+) = 0x([0-9A-Fa-f]+);", Path("config/symbol_addrs.txt").read_text(), re.M)}


def sym_addr(s):
    m = re.match(r"(?:func|D)_([0-9A-F]{8})$", s)
    return int(m.group(1), 16) if m else SYMS.get(s)


ABI = ["$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
       "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
       "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
       "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"]
NUM_REG = re.compile(r"\$(\d+)\b")


def load_funcs():
    """name -> list of (addr, op, args) for every function in the splat asm."""
    funcs = {}
    names = {}
    cur = None
    for p in sorted(Path("asm").rglob("*.s")):
        for line in p.read_text().splitlines():
            m = re.match(r"glabel (\S+)", line)
            if m:
                cur = funcs.setdefault(m.group(1), [])
                continue
            if line.startswith("endlabel"):
                cur = None
                continue
            m = re.match(r"\s*/\* \w+ ([0-9A-F]{8}) ([0-9A-F]{8}) \*/\s+(\S+)\s*(.*)", line)
            if m and cur is not None:
                a = int(m.group(1), 16)
                raw = int.from_bytes(bytes.fromhex(m.group(2)), "little")
                # splat may print numeric registers ($5): use ABI names ($a1)
                args = NUM_REG.sub(lambda r: ABI[int(r.group(1))], m.group(4).split("#")[0].strip())
                cur.append((a, m.group(3), args, raw))
    for n, ins in funcs.items():
        if ins:
            names[ins[0][0]] = n
    return funcs, names


def resolve(arg, names):
    """Symbol in %hi/%lo or a plain name -> address."""
    m = re.search(r"%(?:hi|lo)\(([^)]+)\)", arg)
    s = m.group(1) if m else arg
    a = sym_addr(s)
    if a is None:
        for addr, n in names.items():
            if n == s:
                return addr
    return a


def track(ins, names, upto):
    """Very small register tracker over a linear run of instructions."""
    r = {}
    stack = {}
    for a, op, args, raw in ins:
        if a >= upto:
            break
        ops = [x.strip() for x in args.split(",")]
        if op == "lui" and len(ops) == 2:
            v = resolve(ops[1], names) if "%hi" in ops[1] else None
            if v is None:
                try:
                    v = int(ops[1].split()[0], 0) << 16
                except ValueError:
                    v = None
            r[ops[0]] = ("hi", v)
        elif op == "addiu" and len(ops) == 3:
            if "%lo" in ops[2]:
                r[ops[0]] = ("val", resolve(ops[2], names))
            elif ops[1] in ("$zero", "$0"):
                r[ops[0]] = ("val", int(ops[2], 0))
            else:
                r.pop(ops[0], None)
        elif op == "ori" and len(ops) == 3 and ops[1] in r and r[ops[1]][0] == "hi":
            r[ops[0]] = ("val", (r[ops[1]][1] or 0) | int(ops[2], 0))
        elif op in ("daddu", "addu", "or") and len(ops) == 3 and ops[2] in ("$zero", "$0"):
            if ops[1] in ("$zero", "$0"):
                r[ops[0]] = ("val", 0)
            elif ops[1] in r:
                r[ops[0]] = r[ops[1]]
            else:
                r.pop(ops[0], None)
        elif op in ("sd", "sw") and len(ops) == 2:
            m = re.match(r"(-?0x[0-9A-Fa-f]+|\d+)\(\$(?:sp|29)\)", ops[1])
            if m:
                stack[int(m.group(1), 0)] = r.get(ops[0], ("val", 0) if ops[0] in ("$zero", "$0") else None)
        elif op == "lw" and len(ops) == 2:
            m = re.search(r"%lo\(([^)]+)\)", ops[1])
            r[ops[0]] = ("mem", resolve(m.group(1), names)) if m else None
        elif op == "jal":
            for k in ("$v0", "$v1"):
                r.pop(k, None)
        elif ops and ops[0].startswith("$"):
            r.pop(ops[0], None)
    return r, stack


def val(r, reg):
    v = r.get(reg)
    return v[1] if v and v[0] in ("val", "hi") else None


def calls(ins, target):
    """Addresses just past each `jal target` (and its delay slot)."""
    out = []
    for a, op, args, raw in ins:
        if op == "jal" and raw >> 26 == 3 and (raw & 0x3FFFFFF) << 2 == target:
            out.append(a + 8)
    return out


SYM_BEGIN = "// BEGIN generated by tools/dump_meta.py --symbols (Alchemy meta objects)"
SYM_END = "// END generated by tools/dump_meta.py"
CORE = {
    REG_META: "metaRegister",
    REG_TYPES: "metaSetFieldTypes",
    REG_FIELDS: "metaSetFieldNames",
}


# Virtual slots (vtable index after the 8-byte header) of classes many others derive from.
# (class that introduces the slot, slot) -> method. Unlisted slots stay unnamed.
VIRTUAL_SLOTS = {
    ("igObject", 0): "__bootstrapDestruct",
    ("igObject", 1): "userAllocateFields",
    ("igObject", 2): "userDeallocateFields",
    ("igObject", 3): "userCopyDeep",
    ("igObject", 4): "userCopyShallow",
    ("igObject", 5): "userIsAlikeDeep",
    ("igObject", 6): "userIsAlikeShallow",
    ("igObject", 7): "userInstantiate",
    ("igObject", 8): "userRelease",
    ("igObject", 11): "userResetFields",
    ("igObject", 12): "preWrite",
    ("igObject", 13): "preFileWrite",
    ("igObject", 14): "postRead",
    ("igObject", 15): "postFileRead",
    ("igObject", 16): "clean",
    ("igObject", 17): "makeAbstract",
    ("igObject", 18): "makeConcrete",
    ("igObject", 19): "createCopy",
    ("igDataList", 21): "getElementSize",
    ("igObjectList", 22): "getElementType",
    ("igInfo", 21): "attemptResolve",
    ("igInfo", 22): "resolve",
    ("igInfoManager", 21): "registerToInsight",
    ("igInfoManager", 22): "unRegisterFromInsight",
    ("igInfoManager", 23): "begin",
    ("igInfoManager", 24): "end",
    ("igInfoManager", 25): "appendInfo",
    ("igInfoManager", 26): "removeAllInfos",
    ("igInfoManager", 27): "update",
    # igMetaField reset handlers (func_002FC500).
    ("igMetaField", 26): "resetByValue",
    ("igMetaField", 27): "resetByNoReset",
    ("igMetaField", 28): "resetByReference",
    # DW4's igObject vtable has two extra entries before igNode's scene-graph slots.
    ("igNode", 21): "getParentCount",
    ("igNode", 22): "getParent",
    ("igNode", 23): "hasChildren",
    ("igNode", 24): "isTransformable",
    ("igNode", 25): "transform",
    ("igNode", 26): "setFlagsEnabled",
    ("igNode", 27): "setFlags",
    ("igNode", 28): "getInherantFlags",
    ("igNode", 29): "reset",
    ("igNode", 30): "insertGroupAbove",
    ("igNode", 31): "addParent",
    ("igNode", 32): "removeParent",
    ("igGroup", 33): "insertChild",
    ("igGroup", 34): "isChildrenOrdered",
    # igMetaField's copy methods, through convertMemoryToString.
    ("igMetaField", 34): "copyByValue",
    ("igMetaField", 35): "copyByReference",
    ("igMetaField", 36): "copyByValueShallow",
    ("igMetaField", 37): "copyByReset",
    ("igMetaField", 38): "copyByNoCopy",
    ("igMetaField", 39): "print",
    ("igMetaField", 40): "traverse",
    ("igMetaField", 41): "addToDirectory",
    ("igMetaField", 42): "compare",
    ("igMetaField", 43): "isAlikeCompareExactly",
    ("igMetaField", 44): "isAlikeCompareDeep",
    ("igMetaField", 45): "isAlikeCompareShallow",
    ("igMetaField", 46): "isAlikeCompareNoCompare",
    ("igMetaField", 47): "getIoSize",
    ("igMetaField", 48): "readFromMemory",
    ("igMetaField", 49): "writeToMemory",
    ("igMetaField", 50): "writeRawFieldMemory",
    ("igMetaField", 51): "readRawFieldMemory",
    ("igMetaField", 52): "endianSwapMemory",
    ("igMetaField", 53): "endianSwap",
    ("igMetaField", 54): "setByString",
    ("igMetaField", 55): "getByString",
    ("igMetaField", 56): "setMemoryByString",
    ("igMetaField", 57): "getStringFromMemory",
    ("igMetaField", 58): "convertMemoryToString",
    # igAttr methods.
    ("igAttr", 21): "applyContext",
    ("igAttr", 22): "applyVisualContext",
    ("igAttr", 23): "setUnitID",
    ("igAttr", 24): "synchronizeDefaultVisualContext",
    ("igAttr", 25): "synchronizeDefaultContext",
    ("igAttr", 26): "reset",
    ("igAttr", 29): "getUnitCount",
    # The game's own manager base (names from the decompiled callers, src/beBaseInfoManager.cpp
    # and src/beMessenger.cpp)
    ("beBaseInfoManager", 28): "onAddInfo",
    ("beBaseInfoManager", 29): "entry",
    ("beBaseInfoManager", 30): "getInfoMeta",
    ("beBaseInfoManager", 31): "updateRam",
    ("beBaseInfoManager", 32): "prepare",
    ("beBaseInfoManager", 33): "unhandled",
    ("beBaseInfoManager", 34): "handle",
}


def write_symbols(out, funcs):
    """Name meta storage, registration and getter functions in symbol_addrs.txt."""
    path = Path("config/symbol_addrs.txt")
    text = path.read_text()
    if SYM_BEGIN in text:
        pre, rest = text.split(SYM_BEGIN, 1)
        post = rest.split(SYM_END, 1)[1]
    else:
        pre, post = text.rstrip("\n") + "\n\n", "\n"
    taken = set(re.findall(r"^(\w+) = 0x([0-9A-Fa-f]+);", pre + post, re.M))
    taken_names = {n for n, _ in taken}
    taken_addrs = {int(a, 16) for _, a in taken}
    used = {}  # name -> addr

    used_addrs = set()

    def add(name, addr, kind):
        if addr is None or name in taken_names or addr in taken_addrs:
            return
        if name in used or addr in used_addrs:
            return
        used[name] = (addr, kind)
        used_addrs.add(addr)

    def faddr(fname):
        ins = funcs.get(fname)
        return ins[0][0] if ins else None

    for f, n in CORE.items():
        add(n, f, "func")
    seen_reg = {}
    for c in out:
        seen_reg[c["registered_in"]] = seen_reg.get(c["registered_in"], 0) + 1
    for c in out:
        n = c["name"]
        add(f"{n}_Meta", c["meta"], "data")
        if seen_reg[c["registered_in"]] == 1:
            add(f"{n}_register", faddr(c["registered_in"]), "func")
        if c["fields_fn"]:
            add(f"{n}_registerFields", faddr(c["fields_fn"]), "func")
        if len(c["getters"]) == 1:
            add(f"{n}_getMeta", faddr(c["getters"][0]), "func")
    # Virtual methods: name each implementation after the topmost class (below the class that
    # introduces the slot) whose vtable has it
    for (intro, slot), meth in VIRTUAL_SLOTS.items():
        for f, owner in virtual_owners(out, intro, slot):
            a = faddr(f)
            if a is not None:
                add(f"{owner}_{meth}", a, "func")
    lines = [SYM_BEGIN]
    for name, (addr, kind) in sorted(used.items(), key=lambda x: x[1][0]):
        t = " // type:func" if kind == "func" else ""
        lines.append(f"{name} = 0x{addr:08X};{t}")
    lines.append(SYM_END)
    path.write_text(pre + "\n".join(lines) + post)
    print(f"{len(used)} symbols written to {path}")
    # Decompiled sources may still call newly named functions by their func_XXXXXXXX name, and
    # mwsplice drops compiled functions whose name is not in the unit's splat asm: rename them.
    by_addr = {f"func_{a:08X}": n for n, (a, k) in used.items() if k == "func"}
    for src in list(Path("src").rglob("*.c")) + list(Path("src").rglob("*.cpp")):
        text = src.read_text()
        new = re.sub(r"\bfunc_[0-9A-F]{8}\b", lambda m: by_addr.get(m.group(0), m.group(0)), text)
        if new != text:
            src.write_text(new)
            print(f"renamed functions in {src}")


def virtual_owners(out, intro, slot):
    """(function, owning class) for every implementation of `slot` in `intro` and below.
    igObject has no vtable of its own here: its defaults are igNamedObject's."""
    by_name = {c["name"]: c for c in out}

    def under(c):
        while c is not None:
            if c["name"] == intro:
                return True
            c = by_name.get(c["parent"]) if c["parent"] else None
        return intro == "igObject"

    def slot_fn(name):
        v = (by_name.get(name) or {}).get("virtuals") or []
        return v[slot] if slot < len(v) else None

    def chain(name):
        r = []
        while name:
            r.append(name)
            name = (by_name.get(name) or {}).get("parent")
        return r

    users = {}  # function -> classes (under intro) whose vtable has it at this slot
    for c in out:
        if under(c):
            f = slot_fn(c["name"])
            if f is not None:
                users.setdefault(f, []).append(c["name"])
    res = []
    for f, cls in users.items():
        # owner: the closest common ancestor of every class using this implementation
        common = chain(cls[0])
        for n in cls[1:]:
            ch = set(chain(n))
            common = [x for x in common if x in ch]
        owner = common[0] if common else cls[0]
        if intro == "igObject" and f == slot_fn("igNamedObject") and owner in ("igNamedObject", "igObject"):
            owner = "igObject"
        if owner not in chain(intro) or owner == intro or intro == "igObject" or True:
            res.append((f, owner))
    return res


def depth(c, by_name):
    d = 0
    while c and c.get("parent"):
        c = by_name.get(c["parent"])
        d += 1
    return d


def main():
    funcs, names = load_funcs()
    metas = {}  # meta storage address -> info
    for fname, ins in funcs.items():
        for at in calls(ins, REG_META):
            r, stack = track(ins, names, at)
            info = {
                "meta": val(r, "$a1"),
                "name": cstr(val(r, "$t1") or 0),
                "size": val(r, "$t2"),
                "parent_fn": names.get(val(r, "$a2")),
                "create_fn": names.get(val(r, "$t3")),
                "ctor_fn": names.get(val(r, "$t0")),
                "fields_fn": names.get((stack.get(0) or (None, None))[1]),
                "registered_in": fname,
            }
            if info["meta"]:
                metas[info["meta"]] = info

    # Getter functions -> the meta they return (first meta global they read)
    getter_meta = {}
    for fname, ins in funcs.items():
        for a, op, args, raw in ins[:40]:
            if op == "lw":
                m = re.search(r"%lo\(([^)]+)\)", args)
                if m:
                    addr = resolve(m.group(1), names)
                    if addr in metas:
                        getter_meta.setdefault(fname, addr)
                        break

    by_reg = {i["registered_in"]: i for i in metas.values()}
    for info in metas.values():
        # a2 is the parent's registration function (called first, so the
        # parent exists), a3 the parent's meta getter.
        info["parent"] = by_reg[info["parent_fn"]]["name"] if info["parent_fn"] in by_reg else None
        info["fields"] = []
        fins = funcs.get(info["fields_fn"] or "", [])
        types = []
        for at in calls(fins, REG_TYPES):
            r, _ = track(fins, names, at)
            tbl, n = val(r, "$a1"), val(r, "$a2")
            if tbl and n:
                for i in range(n):
                    fn = names.get(word(tbl + 4 * i))
                    m = getter_meta.get(fn)
                    types.append(metas[m]["name"] if m else fn)
        # Object ref fields: field = metaGetField(meta, first + k); field->+0x38 = T::getMeta()
        reftypes = {}
        k = None
        pending = None
        seq = list(fins)
        i = 0
        while i < len(seq) - 1:  # run delay slots before their jal
            if seq[i][1] == "jal":
                seq[i], seq[i + 1] = seq[i + 1], seq[i]
                i += 1
            i += 1
        for a, op, args, raw in seq:
            ops = [x.strip() for x in args.split(",")]
            if op in ("addiu", "daddu") and ops and ops[0] == "$a1" and len(ops) == 3:
                if op == "addiu" and ops[1] != "$zero" and "%" not in ops[2]:
                    k = int(ops[2], 0)
                elif op == "daddu" and ops[2] == "$zero" and ops[1] != "$zero":
                    k = 0
            if op == "jal" and raw >> 26 == 3:
                t = (raw & 0x3FFFFFF) << 2
                if t == GET_FIELD:
                    pending = ("field", k)
                elif pending and pending[0] == "field":
                    pending = ("type", pending[1], names.get(t))
            if op == "sw" and pending and pending[0] == "type" and ops[0] == "$v0" and ops[1].startswith("0x38("):
                m = getter_meta.get(pending[2])
                if m is not None and pending[1] is not None:
                    reftypes[pending[1]] = metas[m]["name"]
                pending = None
        for at in calls(fins, REG_FIELDS):
            r, _ = track(fins, names, at)
            tn, to = val(r, "$a1"), val(r, "$a3")
            if tn and to:
                for i in range(len(types)):
                    info["fields"].append({
                        "name": cstr(word(tn + 4 * i)),
                        "type": types[i],
                        "offset": word(to + 4 * i),
                        "ref": reftypes.get(i),
                    })

    # Per-class functions: a plain getter (lw v0, meta; jr) and the getter that
    # registers the class first if needed (calls the registration function).
    raw_getters = {}
    for fname, ins in funcs.items():
        ops = [op for _, op, _, _ in ins]
        if ops == ["lui", "jr", "lw"] and getter_meta.get(fname):
            raw_getters.setdefault(getter_meta[fname], []).append(fname)
    for info in metas.values():
        info["getters"] = raw_getters.get(info["meta"], [])

    # vtable: the create function builds a temporary instance and stores the
    # vtable address into it (first data address it materializes and stores).
    for info in metas.values():
        info["vtable"] = None
        cins = funcs.get(info["create_fn"] or "", [])
        regs = {}
        for a, op, args, raw in cins:
            ops = [x.strip() for x in args.split(",")]
            if op == "addiu" and len(ops) == 3 and "%lo" in ops[2]:
                v = resolve(ops[2], names)
                if v and 0x467120 <= v < 0x554B00:
                    regs[ops[0]] = v
            if op == "sw" and ops and ops[0] in regs:
                info["vtable"] = regs[ops[0]]
                break
        if info["vtable"]:
            ents = []
            for i in range(0, 0x200, 4):
                w = word(info["vtable"] + 8 + i)
                if not 0x100000 <= w < 0x467120:
                    break
                ents.append(names.get(w, f"0x{w:X}"))
            info["virtuals"] = ents

    out = sorted(metas.values(), key=lambda x: (x["name"] or ""))
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    jpath = Path(args[0] if args else "build/meta_classes.json")
    jpath.parent.mkdir(parents=True, exist_ok=True)
    jpath.write_text(json.dumps(out, indent=1, default=str))

    lines = ["# Alchemy meta classes", "",
             "Generated by `tools/dump_meta.py` from the reflection data the game registers at",
             "startup (class name, instance size, parent, and named fields with types and offsets).",
             "Field names are the original ones.", ""]
    for c in out:
        if not c["name"]:
            continue
        lines.append(f"## {c['name']}")
        lines.append("")
        par = f", parent `{c['parent']}`" if c["parent"] else ""
        size = f"0x{c['size']:X}" if isinstance(c["size"], int) else "?"
        lines.append(f"Size {size}{par}. Meta at 0x{c['meta']:08X} (`{c['name']}_Meta`), registered in `{c['registered_in']}`.")
        if c["fields"]:
            lines.append("")
            lines.append("| Offset | Field | Type |")
            lines.append("|---|---|---|")
            for f in c["fields"]:
                ref = f" → `{f['ref']}`" if f.get("ref") else ""
                lines.append(f"| 0x{f['offset']:X} | `{f['name']}` | {f['type']}{ref} |")
        lines.append("")
    Path("docs/meta_classes.md").write_text("\n".join(lines) + "\n")
    if "--symbols" in sys.argv:
        write_symbols(out, funcs)
    named = sum(1 for c in out if c["name"])
    nf = sum(len(c["fields"]) for c in out)
    print(f"{len(out)} meta objects, {named} named, {nf} fields")


if __name__ == "__main__":
    main()
