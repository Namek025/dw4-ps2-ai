# Difficulty

`DIFFICULTY` is an int in the GAMERAM common block (`GETCOMMON`): **0 = Normal, 1 = Hard, 2 = Very Hard**
(file suffixes `N`, `H`, `V`). The title menu (`func_003F1A80`) stores the chosen value at game start;
scripts read it with `SYSgetDifficulty` and can change it with `SYSsetDifficulty`.
`LISsetDifficultMax` (`SETDIFFICULTMAX`) unlocks the higher ones.

## What reads it

C code (only three readers):

| Function | What |
|---|---|
| `func_003F1A80` | title menu: writes `DIFFICULTY`, `CAMERAMODE`, `LIGHTMODE`, then `TITLE2` `GAMESTART` |
| `func_00384800` | item generator: picks the drop table list by `DIFFICULTY` (`conItem`, `weapon`, `armor1/2`, `abChip`, ...) |
| `func_00448AE0` | BIT pickup: reads it but does not use it (money depends on level and `BITRITSU` only) |

Stage scripts (Lua 4.0 bytecode inside the stage packages of `NDMW.AFS`, read with `tools/luadis.py`):

- Every stage loads `<folder>/separate/generator{N,H,V}.igb` by difficulty. These are the enemy
  spawn points (`beGeneraterInfo` / `beGeneraterData`).
- Clearing a dungeon on Hard or above sets an extra flag (`FLGset(598..606, 1)`), probably a record
  for unlocking.
- A few events spawn extra things on higher difficulties (`e_mummy`, `g_ddamagefloor`).

## The generator files

In `NDMW.AFS` each stage has 8 consecutive entries; the three of equal size are
`generatorh`, `generatorn`, `generatorv` (alphabetical), e.g. 183/184/185 for the first `dng1` stage.
They are identical except for one int per spawn point (`GENE-01`..`GENE-07`):

| | N | H | V |
|---|---|---|---|
| value | 13 | 17 | 21 |

Confirmed with `tools/igb.py`: it is `beGeneraterData._geneEnemyId`, the row of the `enemy`
database. Enemy strength per difficulty comes entirely from those rows, not from code.

## The enemy database

`enemy` (649 rows, 17 columns; `tools/igb.py <pack> --db enemy`): `CHIMEIRITSU`, `MAHIRITSU`,
`RARITY`, `TSUYOSA` (attack), `MAMORI` (defense), `KASHIKOSA`, `SEISHIN`, `SUBAYASA`,
`RES_FIRE`..`RES_POISON`, `EXP`, `HPMAX`, `MODEL`.

Each regular enemy has 12 consecutive rows: 4 Normal, 4 Hard, 4 Very Hard (the 4 rows of a tier
are used by successive dungeons). Goburimon (`e_goburi`):

| Row | Tier | TSUYOSA | MAMORI | EXP | HPMAX |
|---|---|---|---|---|---|
| 0 | Normal | 16 | 90 | 10 | 50 |
| 3 | Normal | 56 | 93 | 34 | 77 |
| 4 | Hard | 1188 | 125 | 200 | 600 |
| 8 | Very Hard | 21084 | 180 | 600 | 2400 |

Row 0's EXP 10 matches the in-game test (`docs/exp.md`). Hard and Very Hard are post-game tiers
(levels carry over), so they are no use as a "slightly harder Normal": rescale the Normal rows instead.
Damage formulas using these columns: `docs/gameram.md` (`TSUYOSA`, `MAMORI`, `SEISHIN`).
`eneatk` (134 rows) holds the enemy attacks (`TSUYOSA` multiplier, hit rate, status effects).

### Bosses

Bosses have one row per tier (N, H, V) with empty rows between them, not 4. Normal rows
(dump: `tools/igb.py <pack> --db enemy`; first pack with the databases is `AREA.AFS` entry 3):

| Model | Row | TSUYOSA | HPMAX |
|---|---|---|---|
| `e_cockatri` | 91 | 58 | 90 |
| `e_arachne` | 178 | 120 | 420 |
| `e_blosso` (Blossomon) | 76 / 82 | 130 / 156 | 250 / 275 |
| `e_mammos` | 97 / 98 | 187 / 720 | 302 / 750 |
| `f_apocaly` | 106 | 224 | 666 |
| `e_anomalo` | 196 / 197 | 216 / 738 | 1100 / 1691 |
| `e_skulgrey` | 187 / 183 | 180 / 518 | 1000 / 1250 |
| `f_belvam` | 205 | 259 | 1510 |
| `e_tonogeko` | 280 / 276 / 281 | 360 / 521 / 797 | 1522 / 1903 / 2000 |
| `e_diabolo` | 289 | 518 | 1842 |
| `f_luce2` | 633 | 311 | 1861 |
| `f_luce` | 298 | 622 | 2326 |
| `e_ldknight` | 348 / 349 | 864 / 1037 | 2200 / 2794 |
| `e_mecha4bs` | 68 | 664 | 2420 |
| `f_devl1b`, `f_devl1`, `f_devl22`, `f_devl3` | 357, 360, 363, 366 | 829 .. 3359 | 2662 .. 5645 |

Against the first dungeons' Normal mobs (TSUYOSA 16-64, HPMAX 50-87, same MAMORI ~90), Blossomon
hits 2-8x harder and has 3-5x the HP. Nothing scales with the number of players (`NINZU` is only
used for EXP), so a solo player takes all of it. Rebalancing (pending, waits for building the ISO):
lower HPMAX / TSUYOSA / KASHIKOSA of the Normal boss rows in every copy of `enemy`.

The 12 databases (`abChip`, `armor1`, `armor2`, `conItem`, `enemy`, `eneatk`, `impItem`, `insItem`,
`pcTech`, `shinka`, `weapon`, `wazaPanel`) are copied into every `AREA.AFS` stage pack: 665
identical copies of `enemy`. A data mod has to patch all of them (values are fixed size, so
in place in a copy of the ISO).

## Data files

- `NDMW.AFS`, `AREA.AFS`: CRI AFS (`tools/afs.py list/extract`). Entry names are only folder
  prefixes (`dng1`, `stag`, ...); real names live in `NDMW.IGB` (`igAdxAfsFileList`).
- Every entry is an Alchemy IGB file (magic `0xFADA` at +0x28).
- Databases (`beDataBase`, column store: per-type offset lists + data lists) are in the `AREA.AFS`
  stage packs, 7 language copies each. Column names seen: `HP`, `MP`, `MODEL`, `EXP`, `BIT`,
  `BOUGYORITSU`, `MEICHURITSU`, `EXPRITSU`, `BITRITSU`, ...
