# Evolutions (shinka)

## Forms

Every player has 16 forms, the rows of the `shinka` table (`MODEL_NAME`):

| # | Model | # | Model |
|---|---|---|---|
| 0 | `p_agumon` | 8 | `p_beelzeb` (Beelzemon) |
| 1 | `p_vmon` (Veemon) | 9 | `p_alpha` (Alphamon) |
| 2 | `p_girumon` (Guilmon) | 10 | `p_bwargrey` (BlackWarGreymon) |
| 3 | `p_dorumon` | 11 | `p_impdrafm` (Imperialdramon FM) |
| 4 | `p_weregaru` (WereGarurumon) | 12 | `p_impdrapm` (Imperialdramon PM) |
| 5 | `p_hekabut` (HerculesKabuterimon) | 13 | `p_metalgaru` (MetalGarurumon) |
| 6 | `p_wargrey` (WarGreymon) | 14 | `p_dukecrim` (Gallantmon CM) |
| 7 | `p_angelr` | 15 | `p_susanoo` (Susanoomon) |

`shinka` (16 rows, 73 columns) has no requirements: only stats (`*_INI`, `*_GROW`, resistances,
`<TYPE>_GROW`), 6 techniques with their level (`TECHn_LEVEL`) and skill thresholds
(`TECHn_SKILL1..5`), initial weapons, name and description.

## Player data

Per player and per form: `BASE_ISUSE[16]` (unlocked), `BASE_LEVEL[16]`, `BASE_EXP[16]`, skills and
`UPCNT`. Every form keeps its own level. Switching form = `CURRENT2BASE` (save the current one) +
`BASE2CURRENT` (load the other), `src/beNDMWGameRam.cpp`.

## Code

- `SYSctlShinka("SETSHINKA", player, d)` sets `BASE_ISUSE[d]`; `"GETLEVEL"`/`"GETEXP"` read
  `BASE_LEVEL`/`BASE_EXP` (-1 if locked). `SYSisGetShinkaDigimon(player, d)` reads `BASE_ISUSE[d]`.
- Evolve menu (`func_004432A0`, `func_004435C0`, `beNDMWShinkaCtrl`): lists the forms with
  `BASE_ISUSE` set and fills a `beNDMWShinkaObject` per form (level, stats from
  `beNDMWGameRam_calcHpMpBase` / `calcStatBase`, skills). It checks nothing else.
- `func_003D3640` (player model setup): if the player's `ISUSE` is false it sends `GAMESTART`
  and sets every `BASE_ISUSE` to true (`D_005549E0` = 1). Probably the path for a stage started
  without a game (debug); in normal play the forms are locked.
- `func_003EE6D0` / `func_003EF5D0`: save data load / store (copies `BASE_ISUSE` / `BASE_LEVEL`).

So every requirement is in the Lua scripts.

## Scripts (`NDMW.AFS`, `tools/luadis.py --grep Shinka`)

Lobby script (entry 820): computes flags each time:

| Flag | Meaning |
|---|---|
| 931 / 932 / 933 | difficulty Normal / Hard / Very Hard |
| 908 / 909 / 910 | player 1 level ≥ 16 / 46 / 76 |
| 922 / 923 / 924 | forms unlocked ≥ 2 / 5 / 8 |
| 925 / 926 / 927 | cards ≥ 25 / 35 / 45 |
| 928 / 929 / 930 | quests 426-432 / 433-439 / 440-446 all done |
| 935 / 936 | 1 player / ≤ 2 players |
| 912..920 | `COUNTER 1` ≥ 1000 .. 3956000 |

The quest NPCs (`n_digilfm` 1..11) only appear with a story flag (89..98) and, per difficulty, one
of those thresholds (e.g. NPC 1: level ≥ 16/46/76; NPC 2: ≥ 2/5/8 forms; NPC 4: ≥ 25/35/45 cards).

Unlocks (always for player 1, `SETSHINKA 0 d`; in multiplayer the others get BIT). `i` is an index
0..3 the script computes as the maximum of 4 values (not decoded yet):

| Where | Normal | Hard | Very Hard |
|---|---|---|---|
| Quest NPC 2 result (flag 427/434/441) | score 100 → `i` 0..3 = 6, 7, 5, 4 | `COUNTER 0` ≥ 600 and score 100 → 7, 6, 4, 5 | items |
| Quest NPC 3 result (428/435/442) | armor | `COUNTER 0` ≥ 600, flag 562 off → 4, 5, 7, 6 | → 15 (`i` 0, 2) / 11 (`i` 1, 3) |
| Quest NPC 4 result (429/436/443) | BIT | flag 566 → 9 (`i` 0), 8 (1, 2), 13 (3) | flag 566 → 11 / 15 |
| Quest NPC 9 result (432/439/446, entry 824) | → 5, 4, 6, 7 | → 8, 13, 9 | armor |
| NPC 10 (entry 824) | weapon | → 10 | → 12 |
| Quest NPC 8 result (431/438/445, entry 824) | weapon | owns weapon 208 → 13 / 9 | owns weapon 212 → 14 |
| `n_digilff` talk (entry 824) | another rookie: Agumon→3, Veemon→2, Guilmon→0, Dorumon→1 (for the player who talks) | same | same |

Entry 603 has debug scripts: one unlocks all 16 forms for every player, and one per form.

In short: Normal only gives forms 4-7 (two quests, one needing a perfect score). Forms 8-15 need
Hard or Very Hard, which are post-game (`docs/difficulty.md`), and every new form starts at its own
level.

## Mods

**Shared level** (`mods/src/shared_level.cpp`, pnach `Evolutions: shared level`): rewrites
`CURRENT2BASE` to save LEVEL, EXP, SKILL and UPCNT into all 16 `BASE_*` slots instead of the
current form's. The game calls it after every level up (`P2EEXPUP`) and when switching, and
`BASE2CURRENT` loads the new form's slot, so every form has the player's level, skills and UPCNT;
stats still come from each form's own columns at that level. The evolve menu shows the slots, so it
shows the shared level too (skills raised since the last level up show after the next one).
`startPlayer` (new game) also goes through `BASE2CURRENT`, unchanged.

Tested in game: Dorumon level 10 → HerculesKabuterimon / Imperialdramon PM keep level 10, and the
stats match `tools/shinka_stats.py 10` plus the equipped weapon. The weapon is the power socket:
it adds its `TSUYOSA` + revision to STR and its `SUBAYASA` to SPD (also `KASHIKOSA` / `SEISHIN` if
it has them); DEF and HP come from the main / sub slots (`recalcPlayer`, `docs/gameram.md`). At
level 999 Imperialdramon PM's base MHP / MMP pass the 99999 cap (STR 2273, DEF 1940, WIS 2096,
SPI 2449, SPD 1920 before equipment).

**Debug: all forms** (pnach `Debug\All evolutions unlocked`): `0x3D3C4C` `bnez` →
`b 0x3D3CC8` in `func_003D3640`, so every player model spawn runs the `BASE_ISUSE` fill (without the
`GAMESTART` of the original path). The unlock is saved with the game.
