#!/usr/bin/env python3
"""Export / import the textures (igImage) of an IGB file as PNG.

DW4 model textures are 8-bit (or 4-bit) indexed images with an RGBA32 palette (igClut,
_fmt 7), stored in plain order (no GS CSM1 swizzle: checked on p_agumon), alpha 0..0x80
(0x80 = opaque).

    igbtex.py export FILE.igb DIR            # DIR/<_pNameString>.png, one per igImage
    igbtex.py import FILE.igb DIR OUT.igb    # put DIR/*.png back (same size; requantized)

Import only rewrites the pixel and palette memory blocks in place, so the file keeps its
size and layout. A PNG may be RGBA or paletted; it is quantized to the image's palette size.
Needs Pillow.
"""
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from igb import IGB  # noqa: E402

from PIL import Image  # noqa: E402


def images(igb):
    for img in igb.all("igImage"):
        clut = img["_pClut"].get() if img.get("_pClut") else None
        name = img.get("_pNameString") or "image%d" % img["_ref"]
        yield name.rsplit(".", 1)[0], img, clut


def mem_index(igb, ref):
    kind, k = igb.refmap[ref.i]
    assert kind == "mem"
    return k


def decode(igb, img, clut):
    w, h, bits = img["_px"], img["_py"], img["_bitsIdx"]
    if bits not in (4, 8) or clut is None or clut["_fmt"] != 7:
        raise ValueError("unsupported image: %dx%d bits %d clut %s" % (w, h, bits, clut and clut["_fmt"]))
    raw = clut["_pData"].get()
    n = clut["_numEntries"]
    pal = [struct.unpack_from("<4B", raw, 4 * i) for i in range(n)]
    pal = [(r, g, b, min(255, a * 2)) for r, g, b, a in pal]
    data = img["_pImage"].get()
    if bits == 8:
        idx = data[:w * h]
    else:
        idx = bytes(v for byte in data[:w * h // 2] for v in (byte & 15, byte >> 4))
    out = Image.new("RGBA", (w, h))
    out.putdata([pal[i] for i in idx])
    return out


def encode(img_meta, clut, png):
    w, h, bits = img_meta["_px"], img_meta["_py"], img_meta["_bitsIdx"]
    n = clut["_numEntries"]
    im = Image.open(png).convert("RGBA")
    if im.size != (w, h):
        raise ValueError("%s is %dx%d, the texture is %dx%d" % (png, *im.size, w, h))
    exact = im.getcolors(n)  # None if the PNG has more than n colors
    if exact is not None:
        entries = [c for _, c in exact]
        lut = {c: i for i, c in enumerate(entries)}
        px = im.tobytes()
        idx = bytes(lut[tuple(px[i:i + 4])] for i in range(0, len(px), 4))
        entries += [(0, 0, 0, 0)] * (n - len(entries))
    else:
        q = im.quantize(colors=n, method=Image.Quantize.FASTOCTREE)
        pal = q.getpalette("RGBA")[:4 * n]
        pal += [0] * (4 * n - len(pal))
        entries = [tuple(pal[4 * i:4 * i + 4]) for i in range(n)]
        idx = q.tobytes()
    entries = [(r, g, b, (a + 1) // 2) for r, g, b, a in entries]
    clut_bytes = b"".join(struct.pack("<4B", *e) for e in entries)
    if bits == 4:
        idx = bytes(idx[i] | (idx[i + 1] << 4) for i in range(0, len(idx), 2))
    return idx, clut_bytes


def main():
    cmd, path = sys.argv[1], Path(sys.argv[2])
    igb = IGB(path.read_bytes())
    if cmd == "export":
        out = Path(sys.argv[3])
        out.mkdir(parents=True, exist_ok=True)
        for name, img, clut in images(igb):
            decode(igb, img, clut).save(out / (name + ".png"))
            print("%s.png %dx%d %d-bit" % (name, img["_px"], img["_py"], img["_bitsIdx"]))
    elif cmd == "import":
        src, dst = Path(sys.argv[3]), Path(sys.argv[4])
        d = bytearray(igb.d)
        for name, img, clut in images(igb):
            png = src / (name + ".png")
            if not png.exists():
                continue
            idx, clut_bytes = encode(img, clut, png)
            for ref, blob in ((img["_pImage"], idx), (clut["_pData"], clut_bytes)):
                k = mem_index(igb, ref)
                if len(blob) > len(igb.mem[k]):
                    raise ValueError("%s: data larger than the block" % name)
                o = igb.mem_off[k]
                d[o:o + len(blob)] = blob
            print("replaced", name)
        dst.write_bytes(bytes(d))
    else:
        sys.exit(__doc__)


if __name__ == "__main__":
    main()
