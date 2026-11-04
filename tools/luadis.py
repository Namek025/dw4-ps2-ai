#!/usr/bin/env python3
"""Find Lua 4.0 compiled chunks inside a file and print them as pseudo-Lua.

The game's chunks (inside NDMW.AFS stage packages) are standard Lua 4.0 bytecode:
little endian, 4-byte int/size_t/Instruction, double numbers. The output is a stack
simulation: calls and assignments become statements, jumps become `if ... goto Lnn`.

    luadis.py FILE [--grep NAME]   # only chunks whose strings contain NAME
"""
import re
import struct
import sys

OPS = ("END RETURN CALL TAILCALL PUSHNIL POP PUSHINT PUSHSTRING PUSHNUM PUSHNEGNUM "
       "PUSHUPVALUE GETLOCAL GETGLOBAL GETTABLE GETDOTTED GETINDEXED PUSHSELF CREATETABLE "
       "SETLOCAL SETGLOBAL SETTABLE SETLIST SETMAP ADD ADDI SUB MULT DIV POW CONCAT MINUS "
       "NOT JMPNE JMPEQ JMPLT JMPLE JMPGT JMPGE JMPT JMPF JMPONT JMPONF JMP PUSHNILJMP "
       "FORPREP FORLOOP LFORPREP LFORLOOP CLOSURE").split()
MAXARG_S = ((1 << 26) - 1) >> 1
CMP = {"JMPNE": "~=", "JMPEQ": "==", "JMPLT": "<", "JMPLE": "<=", "JMPGT": ">", "JMPGE": ">="}
BIN = {"ADD": "+", "SUB": "-", "MULT": "*", "DIV": "/", "POW": "^"}
HEADER = b"\x1bLua\x40\x01\x04\x04\x04\x20\x06\x09\x08"


class Reader:
    def __init__(self, d, p):
        self.d, self.p = d, p

    def int(self):
        v = struct.unpack_from("<i", self.d, self.p)[0]
        self.p += 4
        return v

    def byte(self):
        self.p += 1
        return self.d[self.p - 1]

    def str(self):
        n = self.int()
        s = self.d[self.p:self.p + n - 1].decode("latin-1") if n else None
        self.p += n
        return s

    def func(self):
        f = {"source": self.str(), "line": self.int(), "nparams": self.int()}
        f["vararg"] = self.byte()
        f["maxstack"] = self.int()
        f["locals"] = [(self.str(), self.int(), self.int()) for _ in range(self.int())]
        n = self.int()  # line info
        self.p += 4 * n
        f["kstr"] = [self.str() for _ in range(self.int())]
        n = self.int()
        f["knum"] = list(struct.unpack_from("<%dd" % n, self.d, self.p))
        self.p += 8 * n
        f["kproto"] = [self.func() for _ in range(self.int())]
        n = self.int()
        f["code"] = list(struct.unpack_from("<%dI" % n, self.d, self.p))
        self.p += 4 * n
        return f


def num(x):
    return str(int(x)) if x == int(x) else repr(x)


def local_name(f, i, pc):
    live = [n for n, s, e in f["locals"] if s <= pc < e]
    return live[i] if i < len(live) else "local%d" % i


def dump(f, name, depth, out):
    ind = "    " * depth
    params = ", ".join(local_name(f, i, 0) for i in range(f["nparams"]))
    out.append("%sfunction %s(%s)" % (ind, name, params))
    ind += "    "
    code = f["code"]
    targets = set()
    for pc, ins in enumerate(code):
        op = OPS[ins & 0x3F]
        if op.startswith("JMP") or op in ("FORPREP", "FORLOOP", "LFORPREP", "LFORLOOP"):
            targets.add(pc + 1 + (ins >> 6) - MAXARG_S)
    st = []
    nlocals = f["nparams"]

    def pop(n=1):
        r = [st.pop() if st else "?" for _ in range(n)]
        return r[::-1]

    for pc, ins in enumerate(code):
        op = OPS[ins & 0x3F]
        u = ins >> 6
        s = u - MAXARG_S
        a, b = ins >> 15, (ins >> 6) & 0x1FF
        if pc in targets:
            out.append("%s::L%d::" % (ind[:-4], pc))
        if op == "END":
            break
        elif op == "RETURN":
            vals = st[u:] if u <= len(st) else st
            out.append("%sreturn %s" % (ind, ", ".join(vals)))
            del st[u:]
        elif op in ("CALL", "TAILCALL"):
            args = st[a + 1:]
            fn = st[a] if a < len(st) else "?"
            del st[a:]
            e = "%s(%s)" % (fn, ", ".join(args))
            if op == "TAILCALL":
                out.append("%sreturn %s" % (ind, e))
            elif b == 0:
                out.append(ind + e)
            else:
                st.extend([e] if b == 1 else ["%s[%d]" % (e, k) for k in range(b if b != 255 else 1)])
        elif op == "PUSHNIL":
            st.extend(["nil"] * u)
        elif op == "POP":
            del st[len(st) - u:]
        elif op == "PUSHINT":
            st.append(str(s))
        elif op == "PUSHSTRING":
            st.append('"%s"' % f["kstr"][u])
        elif op in ("PUSHNUM", "PUSHNEGNUM"):
            st.append(("-" if op == "PUSHNEGNUM" else "") + num(f["knum"][u]))
        elif op == "PUSHUPVALUE":
            st.append("upval%d" % u)
        elif op == "GETLOCAL":
            st.append(local_name(f, u, pc))
        elif op == "GETGLOBAL":
            st.append(f["kstr"][u])
        elif op == "GETTABLE":
            i, = pop()
            t, = pop()
            st.append("%s[%s]" % (t, i))
        elif op == "GETDOTTED":
            t, = pop()
            st.append("%s.%s" % (t, f["kstr"][u]))
        elif op == "GETINDEXED":
            t, = pop()
            st.append("%s[%s]" % (t, local_name(f, u, pc)))
        elif op == "PUSHSELF":
            t, = pop()
            st.extend(["%s:%s" % (t, f["kstr"][u]), "<self>"])
        elif op == "CREATETABLE":
            st.append("{}")
        elif op == "SETLOCAL":
            v, = pop()
            out.append("%s%s = %s" % (ind, local_name(f, u, pc + 1), v))
        elif op == "SETGLOBAL":
            v, = pop()
            out.append("%s%s = %s" % (ind, f["kstr"][u], v))
        elif op == "SETTABLE":
            v = st[-1] if st else "?"
            t = st[len(st) - a] if a <= len(st) else "?"
            i = st[len(st) - a + 1] if 0 < a <= len(st) + 1 and len(st) - a + 1 < len(st) else "?"
            out.append("%s%s[%s] = %s" % (ind, t, i, v))
            del st[len(st) - b:]
        elif op == "SETLIST":
            vals = pop(b + 1)
            t, vals = vals[0], vals[1:]
            st.append(t if t != "{}" else "{%s}" % ", ".join(vals))
        elif op == "SETMAP":
            kv = pop(2 * (u + 1))
            pairs = ["[%s]=%s" % (kv[k], kv[k + 1]) for k in range(0, len(kv), 2)]
            t, = pop()
            st.append("{%s}" % ", ".join(pairs) if t == "{}" else t)
        elif op in BIN:
            y, = pop()
            x, = pop()
            st.append("(%s %s %s)" % (x, BIN[op], y))
        elif op == "ADDI":
            x, = pop()
            st.append("(%s + %d)" % (x, s))
        elif op == "CONCAT":
            st.append("(%s)" % " .. ".join(pop(u)))
        elif op == "MINUS":
            x, = pop()
            st.append("-" + x)
        elif op == "NOT":
            x, = pop()
            st.append("not " + x)
        elif op in CMP:
            y, = pop()
            x, = pop()
            out.append("%sif %s %s %s then goto L%d end" % (ind, x, CMP[op], y, pc + 1 + s))
        elif op in ("JMPT", "JMPF", "JMPONT", "JMPONF"):
            x, = pop()
            neg = "" if op in ("JMPT", "JMPONT") else "not "
            out.append("%sif %s%s then goto L%d end" % (ind, neg, x, pc + 1 + s))
        elif op == "JMP":
            out.append("%sgoto L%d" % (ind, pc + 1 + s))
        elif op == "PUSHNILJMP":
            st.append("nil")
        elif op in ("FORPREP", "LFORPREP"):
            out.append("%sfor %s ... (end L%d)" % (ind, ", ".join(st[-3:]), pc + 1 + s))
        elif op in ("FORLOOP", "LFORLOOP"):
            out.append("%snext (loop L%d)" % (ind, pc + 1 + s))
        elif op == "CLOSURE":
            st[len(st) - b:] = []
            sub = []
            dump(f["kproto"][a], "", depth + 1, sub)
            st.append("<closure%d>" % a)
            out.append("%s-- closure%d:" % (ind, a))
            out.extend(sub)
    out.append("%send" % ind[:-4])


def main():
    path = sys.argv[1]
    grep = sys.argv[sys.argv.index("--grep") + 1] if "--grep" in sys.argv else None
    d = open(path, "rb").read()
    for m in re.finditer(re.escape(HEADER), d):
        r = Reader(d, m.start() + len(HEADER) + 8)
        try:
            f = r.func()
        except (struct.error, IndexError, UnicodeDecodeError):
            continue
        blob = d[m.start():r.p]
        if grep and grep.encode() not in blob:
            continue
        print("-- chunk at 0x%X (%d bytes), source %s" % (m.start(), len(blob), f["source"]))
        out = []
        dump(f, "main", 0, out)
        print("\n".join(out))
        print()


if __name__ == "__main__":
    main()
