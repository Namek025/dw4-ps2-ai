# Player models (`_common/*.igb`) and textures

Found 2026-11-02 from the disc files. Goal: replace or add a playable character (see
`docs/evolution.md` for the 16 forms and the `shinka` table).

## Where the files are

AFS entry names are only 4 characters (`_com`, `stag`, `dng1`, ...). The real paths are in
`disc/NDMW.IGB`, an `igAdxAfsFileList` of `igAdxAfsFile` objects (`_name`, `_pathList` = the
directory as a list of `igNamedObject`, `_ptNumber` = AFS partition, 0 = `NDMW.AFS`,
1 = `AREA.AFS`, `_fileNumber` = entry):

    python3 tools/afs_names.py --dir _common      # partition, entry, path

`_common/` is `NDMW.AFS` entries 833-971: four files per form, each block in alphabetical order
of the model name (`agumon, alpha, angelr, beelzeb, bwargrey, dorumon, dukecrim, girumon,
hekabut, impdrafm, impdrapm, metalgaru, susanoo, vmon, wargrey, weregaru`):

| Entries | Prefix | Contents (Agumon) | Role |
|---|---|---|---|
| 833-848 | `m_` | 380 KB: 2 textures, 24 animations, skeleton | **player model in town** (tested) |
| 849-864 | `p_` | 628 KB: 2 textures, 82 animations, 3 skinned meshes, model control | **player model in dungeons** (tested) |
| 865-880 | `q_` | 445 KB: 1 texture 64x64, same 82 animations | low-detail copy (not seen with 1-2 players, town or dungeon) |
| 881-896 | `s_` | 257 KB: 1 texture 256x256, 2 animations, viewport/projection attrs | **evolution screen model** (tested) |

Then `sbiallse.igb` (897) and the weapons `w_<type><n>.igb` (898-971). Stage packs (`stag`,
`dng*`) name `p_agumon` ~40 times each: references (file lists, scripts), not the model.

**In-game test (2026-11-02)**, each file recolored differently: in town the playable Agumon used
`m_agumon` (`m_` = *machi*, town? it has no combat animations), and the Agumon NPCs walking
around used their own copy: `P_agumon01` 64x64 is also inside 10 unnamed stage packs
(`NDMW.AFS` 449, 518, 526, 534, 536, 538, 540, 820, 824, 828; next to `stagml*` packs).
In the first dungeon the player used `p_agumon` (with combat), and the town NPCs kept their stage-pack copy.
The evolution screen used `s_agumon`.

Contents compared: `q_` is `p_` with the same 82 animation names and the same model control
data (179 actions, 63 labels, 236 hitboxes), but without the 256x256 mesh: its `HighAppearance`
points to the low mesh (`P_agumon_Skin0Shape2`, 64x64 texture), and there is no `LowAppearance`
(8 actors instead of 12). `m_` keeps all three meshes and both textures but only 24 animations
(`wait*`, `run*`, `jump*`, `dcng*`, no attacks), 37 actions and no hitboxes: a town model. `q_` did not show up anywhere tested (town with 2 players:
both `m_`; dungeon with 2 players: both `p_`), and no file on the disc names `q_agumon`
(the ELF has no `q_` string either). Model files are loaded by path from the stages' file
lists (`_common/p_agumon.igb` in dungeon packs, `_common/m_agumon.igb` in the town packs 816-828,
`_common/s_agumon.igb` in 832), and no list in either AFS names `_common/q_*.igb`: **`q_` is very
likely unused** (a leftover, maybe for a cut 4-player mode). Same for all 16 forms: outside the
model files, each `m_<form>` is named by 4 entries, `p_<form>` by 852, `s_<form>` by 1, and
`q_<form>` only by the AFS index `NDMW.IGB` (which lists every file).

NPC Agumon = `n_agumon` (scripts `n_agumon_00..02[h|n|v].lua`, `hit_n_agumon.lua`,
`aim_n_agumon.lua`), with its own 64x64 copy of `P_agumon01` in each stage's main pack. The
unnamed entry before each `<stage>/separate/` block is that stage's pack (the one before it, a
small file list naming the stage), so the 10 packs are: town `stagml11` (820), `stagml12` (824),
`stagml13` (828); quest dungeons `dngq2b1e` (449), `dngq4b2h` (518), `dngq4b2v` (526),
`dngq5b1e/h/v` (534/536/538, three Agumon scripts each: `n_agumon_00`, `_01`, `_02`), `dngq6b01`
(540, also `g_agumon01/02MapBlock`). `h_agumon` (`H_agumon_Skin0Shape1`, no texture) appears in
stage-control packs 806 and 812. Icons, portraits and the pause menu kept the original colors: they are 2D images
somewhere else, not these textures. A reskin has to patch every copy.

## `p_agumon.igb`

IGB version 6 (`tools/igb.py FILE --stats`, field names from `build/meta.json`):

- **Textures**: `igTextureAttr` → `igImage` + `igClut`. `P_agumon01.png` 256x256 (body) and
  `P_agumon02.png` 64x64, both 8-bit indexed (`_bitsIdx` 8, `_pfmt` 65536), palette of 256
  RGBA32 entries (`igClut._fmt` 7), **plain order** (no GS CSM1 swizzle), alpha 0..0x80.
  Unused texture areas are filled with green.
- **Mesh**: three `igSkin` appearances, each `igSkin._skinnedGraph` → `igBlendMatrixSelect`
  (attributes: texture bind, material, ...) → `igGeometry` → `igGeometryAttr1_5`:

  | Appearance | Geometry | Vertices | Triangles | Texture |
  |---|---|---|---|---|
  | `HighAppearance` | `P_agumon_Skin0Shape1` | 3074 | 1966 | `P_agumon01` 256x256 |
  | `LowAppearance` | `P_agumon_Skin0Shape2` | 1166 | 634 | `P_agumon02` 64x64 |
  | `KageAppearance` (kage = shadow) | `P_agumon_Skin0Shape0` | 164 | 102 | none |

  So the 64x64 texture belongs to the low-detail mesh. Four `igLod` nodes hold three
  `igActor`s each (High / Low / Kage), probably one per player.
  `_primType` 4 = one triangle strip (`_stripLengths`, no index array; degenerate triangles
  join the pieces). `igVertexArray1_1._vdata` is a table of 20 memory refs: slot 0 positions
  (vec3f), 1 normals (vec3f), 11 texture coordinates (vec2f, v = 0 at the top row). Y is up,
  the model is in its bind pose (T-pose), about 9 units tall.
- **Skinning**: rigid, one bone per vertex: `_blendWeights` is one float (always 1.0) and
  `_blendIndices` one byte per vertex, an index into the `igBlendMatrixSelect`'s
  `_blendMatrixIndices`, which gives the joint (`igSkeletonBoneInfo._bmIdx`).
- **Skeleton**: `defaultSkeleton`, 25 bone infos (name, `_parentIdx`, `_bmIdx`), 20 of them
  joints with an inverse bind matrix (`_invJointArray`, 4x4 row-vector matrices, pure
  translations here) and `_boneTranslationArray` (local offsets): `RotateRoot`, `JointRoot`,
  `Back`, `Abdomen`, `chest`, `Head`, `Chin01`, `LArm01..04`, `RArm01..04`, `LegRoot`,
  `LLeg01..04`, `RLeg01..04`.
- **Animations**: 82 `igAnimation` (Enbaya compressed tracks, one per bone), named by action
  and stance: `wait*`, `run*`, `guard*`, `dead*`, `stun*`, `sleep*`, `sdmgD`, `ldmgD01..06`,
  `jump*01..03`, `dcng*` (digivolve), `fireD`, `spellD`, `item*`, attack sets `vatc*`, `katc*`,
  `hatc*`, `batc*`. Stance suffix: `D`, `S`, `A`, `G` (weapon grips?).
- **Game control data**: `beModelCtrl*` (179 `ACTION`s like `DEADA1`, `HFIRE5`, `CUTER1`;
  63 `LABEL`s per technique / weapon: `fire1..5`, `ice1..5`, `saber`, `gun`, ...; 236 `HITBOX`es),
  `beAction2MOTIONSET` (animation name, blend time `_hokanTime`, loop mode `LP`/`OS`).

A replacement model must keep this contract: the animation names and actions the game asks
for, a skeleton the actions/hitboxes refer to, and the `beModelCtrl*` / `beAction2*` data.

## Tools

    python3 tools/afs_names.py --dir _common
    .venv/bin/python tools/igbtex.py export assets/common/p_agumon.igb build/tex/p_agumon
    .venv/bin/python tools/igbtex.py import assets/common/p_agumon.igb build/tex/p_agumon OUT.igb
    .venv/bin/python tools/modiso.py "disc/Digimon World 4 (USA).iso" build/dw4_test.iso NDMW.AFS:849=OUT.igb
    .venv/bin/python tools/igb2gltf.py assets/common/p_agumon.igb build/p_agumon.glb [--appearance all]

- `igb2gltf.py` (needs numpy + Pillow): mesh, texture, skeleton and skin to glTF binary, to
  look at the model in Blender (File > Import > glTF 2.0) or the Windows 3D Viewer. Bind pose
  only: animations (`igEnbayaAnimationSource`, compressed) are not decoded yet. Checked: every
  joint's world matrix times its inverse bind matrix is the identity.
- `igbtex.py` (needs Pillow): export writes one PNG per `igImage` (`_pNameString`); import
  takes PNGs of the same size, keeps the exact colors when there are ≤ 256 (round trip is
  pixel exact), otherwise quantizes, and rewrites only the pixel and palette blocks in place.
- `modiso.py` copies the ISO and writes AFS entries inside the copy (in place: the new file
  must fit before the next entry, entries are 0x800 aligned) and updates the entry size.
  The original ISO is not touched.
- Extracting one entry: `afs.py extract disc/NDMW.AFS DIR --grep NAME`, or by entry number
  (`build/digimon_files.sh agumon` writes the four files of a form to `assets/common/`).

## Test pending

`build/dw4_test.iso`: `p_agumon` with red and blue swapped (blue Agumon). To check in PCSX2:
which screens show the change (field, menus, multiplayer), which tells what `m_`/`q_`/`s_` are.

## Open questions

- Roles of `m_`, `q_`, `s_`; whether menus / portraits use other files (`Load`, `stag` packs).
- How the game picks the model: `MODEL_NAME` from `shinka` (`p_agumon`) + prefix?
  (`func_003D3640`, player model setup.)
- Which animations / actions each technique and weapon needs (`beModelCtrl*`, `beAction2*`
  classes in `docs/meta_classes.md`).
- Writing a new mesh / skeleton / animations means an IGB writer (today: in-place patches only).
- Animations: decode `igEnbayaAnimationSource` (Alchemy's compressed tracks) from the code, to
  export / play them.
