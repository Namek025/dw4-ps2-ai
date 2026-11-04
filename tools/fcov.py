"""fcov.py OBJ FUNC ADDR SIZE [-R] [-K] [-v] [-g] [-S] [-d A-B]: how much of a (big) original
function the compiled one reproduces, for work in progress where the sizes differ.

difflib over the disassembly of the original (ADDR, SIZE in hex) and of FUNC in OBJ.
  (default) immediates masked
  -R        register names masked too (structure only)
  -K        keep small constants, mask only addresses / %lo / stack offsets (use with -R)
  -v        list the original ranges that are missing
  -g        list every mismatch, however small (op, address, sizes, first instructions)
  -S        stack slot map: each original sp offset >= 160 with ours (from aligned instructions),
            the shift and where it is first used; slots out of the common shift show the layout
            differences (locals in another order, a temporary declared as a local, ...)
  -d A-B    aligned side-by-side view of that address range ("!!" = not reproduced, "~" = same
            instruction up to registers/offsets)

Example: compile the unit with -DNON_MATCHING, then
  python3 tools/fcov.py build/x.o beNDMWGameRam_handle 3FCA10 A388 -R -K
"""
import difflib, re, struct, sys
sys.path.insert(0, 'tools')
import fcmp
obj, func, addr, size = sys.argv[1], sys.argv[2], int(sys.argv[3], 16), int(sys.argv[4], 16)
rom = open(fcmp.ROM, 'rb').read()
funcs = {fcmp.demangle(n): w for n, w in fcmp.read_funcs(obj).items()}
words = funcs[func]
orig = [struct.unpack_from('<I', rom, addr - fcmp.BASE + 4 * i)[0] for i in range(size // 4)]
R = '-R' in sys.argv
K = '-K' in sys.argv
def norm(s):
    if K:
        s = re.sub(r'0x[0-9a-f]+', 'N', s)
        s = re.sub(r'-?\d+\(sp\)', 'N(sp)', s)
        s = re.sub(r'(addiu\t\w+,sp,)-?\d+', r'\1N', s)
        s = re.sub(r'(lui\t\w+,)\S+', r'\1N', s)
        s = re.sub(r'-?\d+\((at|gp)\)', r'N(\1)', s)
        s = re.sub(r'(addiu\t\w+,(\w+),)(-?\d+)$', lambda m: m.group(1) + ('N' if m.group(2) != 'zero' and (int(m.group(3)) == 0 or abs(int(m.group(3))) > 255) else m.group(3)), s)
        s = s.strip()
    else:
        s = re.sub(r'0x[0-9a-f]+|-?\b\d+\b', 'N', s).strip()
    if R:
        s = re.sub(r'\$?\b(s[0-8]|a[0-3]|v[01]|t[0-9]|at)\b', 'r', s)
    return s
a = [norm(x) for x in fcmp.disasm(orig, addr)]
b = [norm(x) for x in fcmp.disasm([w for w, _, _ in words], addr)]
sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
eq = sum(bl.size for bl in sm.get_matching_blocks())
print(f"reproduced {eq}/{len(a)} ({100*eq/len(a):.1f}%), ours {len(b)}")
if '-v' in sys.argv:
    last = 0
    for bl in sm.get_matching_blocks():
        if bl.a > last + 8:
            print(f"  missing {addr+4*last:08X}..{addr+4*(bl.a-1):08X} ({bl.a-last})")
        last = bl.a + bl.size
if '-g' in sys.argv or '-S' in sys.argv:
    ra = fcmp.disasm(orig, addr)
    rb = fcmp.disasm([w for w, _, _ in words], addr)
if '-g' in sys.argv:
    for op, i1, i2, j1, j2 in sm.get_opcodes():
        if op != 'equal':
            print(f"{op:8s} {addr+4*i1:08X} orig {i2-i1:3d} ours {j2-j1:3d}  | "
                  f"{' ; '.join(ra[i1:min(i2, i1+3)])[:70]}  || {' ; '.join(rb[j1:min(j2, j1+3)])[:70]}")
if '-S' in sys.argv:
    from collections import Counter, defaultdict
    slot = re.compile(r"(?:,|\b)(\d+)\(sp\)|sp,(\d+)$")
    mp, first = defaultdict(Counter), {}
    for bl in sm.get_matching_blocks():
        for k in range(bl.size):
            mx, my = slot.search(ra[bl.a + k]), slot.search(rb[bl.b + k])
            if mx and my:
                ox, oy = int(mx.group(1) or mx.group(2)), int(my.group(1) or my.group(2))
                if ox >= 160:
                    mp[ox][oy] += 1
                    first.setdefault(ox, addr + 4 * (bl.a + k))
    for ox in sorted(mp):
        oy = mp[ox].most_common(1)[0][0]
        other = "" if len(mp[ox]) == 1 else "  " + str(dict(mp[ox]))
        print(f"{ox:5d} -> {oy:5d}  {oy - ox:+d}  first {first[ox]:08X}{other}")
if '-d' in sys.argv:
    lo, hi = [int(x, 16) for x in sys.argv[sys.argv.index('-d') + 1].split('-')]
    ra = fcmp.disasm(orig, addr)
    rb = fcmp.disasm([w for w, _, _ in words], addr)
    for op, i1, i2, j1, j2 in sm.get_opcodes():
        if addr + 4 * i2 < lo or addr + 4 * i1 > hi:
            continue
        if op == 'equal':
            for k in range(i2 - i1):
                ia = i1 + k
                if lo <= addr + 4 * ia <= hi:
                    mark = '  ' if ra[ia] == rb[j1 + k] else ' ~'
                    print(f"{mark} {addr+4*ia:08X} {ra[ia]:34.34s} | {rb[j1+k]}")
        else:
            n = max(i2 - i1, j2 - j1)
            for k in range(n):
                x = ra[i1 + k] if i1 + k < i2 else ''
                y = rb[j1 + k] if j1 + k < j2 else ''
                at = f"{addr+4*(i1+k):08X}" if i1 + k < i2 else '        '
                print(f"!! {at} {x:34.34s} | {y}")
