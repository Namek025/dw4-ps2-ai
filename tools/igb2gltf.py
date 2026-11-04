#!/usr/bin/env python3
"""Convert a DW4 character IGB (`_common/p_*.igb` and the like) to glTF binary (.glb).

    igb2gltf.py FILE.igb OUT.glb [--appearance High|Low|Kage|all]

Exports the skinned meshes (igSkin "HighAppearance", "LowAppearance", "KageAppearance" =
shadow), their textures (igbtex decoding), the skeleton (igSkeleton bone infos) and the
skin (one bone per vertex: igVertexArray blend index -> igBlendMatrixSelect index list ->
joint; inverse bind matrices from _invJointArray). Animations are not exported yet
(igEnbayaAnimationSource is a compressed format still to decode).

Geometry (docs/models.md): igGeometry -> igGeometryAttr1_5 (_primType 4 = triangle strips,
_stripLengths) -> igVertexArray1_1, whose _vdata is a table of 20 memory refs: slot 0
positions (vec3f), 1 normals (vec3f), 11 texture coordinates (vec2f). Y is up.
Opens in Blender (File > Import > glTF 2.0) or the Windows 3D Viewer. Needs numpy + Pillow.
"""
import io
import json
import struct
import sys
from pathlib import Path

import numpy as np

sys.path.insert(0, str(Path(__file__).resolve().parent))
from igb import IGB  # noqa: E402
import igbtex  # noqa: E402

POS, NRM, UV = 0, 1, 11


def strips_to_tris(pos, lens):
    tris = []
    start = 0
    for n in lens:
        for i in range(n - 2):
            a, b, c = start + i, start + i + 1, start + i + 2
            if i & 1:
                a, b = b, a
            if len({tuple(pos[a]), tuple(pos[b]), tuple(pos[c])}) == 3:
                tris.append((a, b, c))
        start += n
    return np.array(tris, np.uint32).reshape(-1, 3)


class Glb:
    def __init__(self):
        self.bin = bytearray()
        self.j = {"asset": {"version": "2.0", "generator": "dw4 igb2gltf"}, "buffers": [], "bufferViews": [],
                  "accessors": [], "images": [], "textures": [], "samplers": [{"magFilter": 9729, "minFilter": 9729}],
                  "materials": [], "meshes": [], "nodes": [], "skins": [], "scenes": [{"nodes": []}], "scene": 0}

    def view(self, data, target=None):
        while len(self.bin) % 4:
            self.bin.append(0)
        v = {"buffer": 0, "byteOffset": len(self.bin), "byteLength": len(data)}
        if target:
            v["target"] = target
        self.bin += data
        self.j["bufferViews"].append(v)
        return len(self.j["bufferViews"]) - 1

    def accessor(self, arr, ctype, atype, target=None, minmax=False):
        a = {"bufferView": self.view(arr.tobytes(), target), "componentType": ctype, "count": len(arr), "type": atype}
        if minmax:
            a["min"] = arr.min(0).tolist()
            a["max"] = arr.max(0).tolist()
        self.j["accessors"].append(a)
        return len(self.j["accessors"]) - 1

    def node(self, n):
        self.j["nodes"].append(n)
        return len(self.j["nodes"]) - 1

    def write(self, path):
        j = {k: v for k, v in self.j.items() if v != []}
        j["buffers"] = [{"byteLength": len(self.bin)}]
        js = json.dumps(j).encode()
        js += b" " * (-len(js) % 4)
        self.bin += b"\0" * (-len(self.bin) % 4)
        total = 12 + 8 + len(js) + 8 + len(self.bin)
        out = struct.pack("<III", 0x46546C67, 2, total)
        out += struct.pack("<II", len(js), 0x4E4F534A) + js
        out += struct.pack("<II", len(self.bin), 0x004E4942) + bytes(self.bin)
        Path(path).write_bytes(out)


def skin_parts(igb, skin):
    """(geometry name, vertex array, strip lengths, texture image or None, blend index list)."""
    bms = skin["_skinnedGraph"].get()
    tex = None
    for a in igb.list_items(bms["_attributes"].get()):
        o = a.get()
        if o["_class"] == "igTextureBindAttr":
            tex = o["_texture"].get()["_image"].get()
    bmi = igb.list_items(bms["_blendMatrixIndices"].get())
    for g in igb.list_items(bms["_childList"].get()):
        geom = g.get()
        for a in igb.list_items(geom["_attributes"].get()):
            ga = a.get()
            if ga["_class"].startswith("igGeometryAttr"):
                pl = ga["_stripLengths"].get()
                lens = struct.unpack("<%dI" % pl["_numStrips"], pl["_lengthData"].get()[:4 * pl["_numStrips"]])
                yield geom["_name"], ga["_vertexArray"].get(), lens, tex, bmi


def main():
    src, dst = Path(sys.argv[1]), sys.argv[2]
    want = sys.argv[sys.argv.index("--appearance") + 1] if "--appearance" in sys.argv else "High"
    igb = IGB(src.read_bytes())
    g = Glb()

    # Skeleton: one node per bone info; joints = bones with a blend matrix (_bmIdx >= 0)
    sk = next(igb.all("igSkeleton"))
    infos = [r.get() for r in igb.list_items(sk["_boneInfoList"].get())]
    trans = igb.resolve(sk["_boneTranslationArray"].i)
    inv = igb.resolve(sk["_invJointArray"].i)
    njoint = sk["_jointCount"]
    inv_m = [np.array(struct.unpack_from("<16f", inv, 64 * k)).reshape(4, 4) for k in range(njoint)]
    world = []
    for k, b in enumerate(infos):
        if b["_bmIdx"] >= 0:
            w = np.linalg.inv(inv_m[b["_bmIdx"]])  # row-vector convention: translation in row 3
        else:
            t = struct.unpack_from("<3f", trans, 12 * k)
            w = np.eye(4)
            w[3, :3] = t
            if b["_parentIdx"] >= 0:
                w = w @ world[b["_parentIdx"]]
        world.append(w)
    bone_nodes = []
    for k, b in enumerate(infos):
        local = world[k] @ np.linalg.inv(world[b["_parentIdx"]]) if b["_parentIdx"] >= 0 else world[k]
        n = {"name": b["_name"] or "root"}
        t = local[3, :3]
        if np.abs(local[:3, :3] - np.eye(3)).max() > 1e-5:
            n["matrix"] = local.reshape(-1).tolist()  # row-major row-vector == glTF column-major
        elif np.abs(t).max() > 0:
            n["translation"] = t.tolist()
        bone_nodes.append(g.node(n))
    for k, b in enumerate(infos):
        kids = [bone_nodes[c] for c, cb in enumerate(infos) if cb["_parentIdx"] == k]
        if kids:
            g.j["nodes"][bone_nodes[k]]["children"] = kids
    roots = [bone_nodes[k] for k, b in enumerate(infos) if b["_parentIdx"] < 0]
    joints = [None] * njoint
    for k, b in enumerate(infos):
        if b["_bmIdx"] >= 0:
            joints[b["_bmIdx"]] = bone_nodes[k]
    ibm = np.array([m.reshape(-1) for m in inv_m], np.float32)
    g.j["skins"].append({"joints": joints, "inverseBindMatrices": g.accessor(ibm, 5126, "MAT4"),
                         "skeleton": roots[0], "name": sk["_name"]})

    images = {}
    mesh_nodes = []
    for skin in igb.all("igSkin"):
        name = skin["_name"]
        if want != "all" and not name.startswith(want):
            continue
        prims = []
        for gname, va, lens, tex, bmi in skin_parts(igb, skin):
            n = va["_numVerts"]
            refs = struct.unpack("<20i", va["_vdata"].get())
            pos = np.frombuffer(igb.resolve(refs[POS]), "<f4").reshape(n, 3).copy()
            attrs = {"POSITION": g.accessor(pos, 5126, "VEC3", 34962, True)}
            if refs[NRM] >= 0:
                nrm = np.frombuffer(igb.resolve(refs[NRM]), "<f4").reshape(n, 3).copy()
                nrm /= np.maximum(np.linalg.norm(nrm, axis=1, keepdims=True), 1e-8)
                attrs["NORMAL"] = g.accessor(nrm.astype(np.float32), 5126, "VEC3", 34962)
            if refs[UV] >= 0:
                uv = np.frombuffer(igb.resolve(refs[UV]), "<f4").reshape(n, 2).copy()
                attrs["TEXCOORD_0"] = g.accessor(uv, 5126, "VEC2", 34962)
            bi = np.frombuffer(igb.resolve(va["_blendIndices"].i), np.uint8)[:n]
            jnt = np.zeros((n, 4), np.uint8)
            jnt[:, 0] = [bmi[x] for x in bi]
            wgt = np.zeros((n, 4), np.float32)
            wgt[:, 0] = 1.0
            attrs["JOINTS_0"] = g.accessor(jnt, 5121, "VEC4", 34962)
            attrs["WEIGHTS_0"] = g.accessor(wgt, 5126, "VEC4", 34962)
            tris = strips_to_tris(pos, lens)
            prim = {"attributes": attrs, "indices": g.accessor(tris.reshape(-1), 5125, "SCALAR", 34963), "mode": 4}
            mat = {"name": gname, "doubleSided": True,
                   "pbrMetallicRoughness": {"metallicFactor": 0.0, "roughnessFactor": 1.0}}
            if tex is not None:
                key = tex["_ref"]
                if key not in images:
                    clut = tex["_pClut"].get()
                    buf = io.BytesIO()
                    igbtex.decode(igb, tex, clut).save(buf, "PNG")
                    g.j["images"].append({"bufferView": g.view(buf.getvalue()), "mimeType": "image/png",
                                          "name": tex.get("_pNameString")})
                    g.j["textures"].append({"source": len(g.j["images"]) - 1, "sampler": 0})
                    images[key] = len(g.j["textures"]) - 1
                mat["pbrMetallicRoughness"]["baseColorTexture"] = {"index": images[key]}
            else:
                mat["pbrMetallicRoughness"]["baseColorFactor"] = [0.1, 0.1, 0.1, 1.0]
            g.j["materials"].append(mat)
            prim["material"] = len(g.j["materials"]) - 1
            prims.append(prim)
            print("%s/%s: %d vertices, %d triangles, texture %s"
                  % (name, gname, n, len(tris), tex and tex.get("_pNameString")))
        g.j["meshes"].append({"name": name, "primitives": prims})
        mesh_nodes.append(g.node({"name": name, "mesh": len(g.j["meshes"]) - 1, "skin": 0}))
    g.j["scenes"][0]["nodes"] = roots + mesh_nodes
    g.write(dst)
    print("wrote", dst)


if __name__ == "__main__":
    main()
