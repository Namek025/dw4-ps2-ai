# GAMERAM: player data, stats and fight formulas

`beNDMWGameRam` (`src/beNDMWGameRam.cpp`, `0x3F6620`–`0x407080`) owns the player data and does
every stat and battle calculation. Scripts and the engine talk to it through the messenger
(`docs/meta_classes.md`, `src/beMessenger.cpp`): select group `"GAMERAM"`, push arguments, send a
message name. The handler is `beNDMWGameRam_handle` (the class's vtable slot `+0x90`), a switch on the
message index from the `Message` name table.

Read from m2c output of the original code, not from matched C yet: formulas are exact, names of
helpers are descriptive. Integer math, C division (truncates toward zero). `rand()` is the system
random generator (`_system` → `+0x30`, virtual `+0x60`), taken `% n`.

## Data

`beNDMWGameRam` fields (reflection names): `_dBShinka`, `_dBPcTech`, `_dBWeapon`, `_dBArmor1`,
`_dBArmor2`, `_dBAbChip`, `_dBEnemy`, `_dBEneatk` (`beDataBase` tables, loaded on first use by
calling `DATABASE` with `"shinka"`, `"pcTech"`, `"enemy"`, `"eneatk"`, ...), `_commonDataList` and
`_playerDataListList` (one `beDataObjList` per player, 4 players).

A `beDataBase` stores records column by column, per type (char/short/int/float/bool/string groups,
each with a name list): `beDataBase_getInt(db, record, "FIELD")` (`src/beDBManager.cpp`).

Player data objects used here (`beDataObjInt` unless noted): `DIGIMONNO`, `DIGIMONNAME` (string),
`LEVEL`, `EXP`, `NEXTEXP`, `HP`, `MHP`, `MP`, `MMP`, `TSUYOSA` (strength), `MAMORI` (defense),
`KASHIKOSA` (wisdom), `SEISHIN` (spirit), `SUBAYASA` (speed), `RES_FIRE`.. resistances,
`MEICHURITSU` (accuracy), `KAIHIRITSU` (evasion), `BOUGYORITSU` (guard), `CHIMEIRITSU` (critical),
`MAHIRITSU` (stun rate), `EXPRITSU` (EXP bonus %), `HPDRAIN`, `MPDRAIN`, `WEAPONSKILLNO`,
`ATTACKZOKUSEI` (weapon element), `HONOOTEIKO` / `KOORITEIKO` / `KAMINARITEIKO` / `YAMITEIKO`
(fire / ice / thunder / dark resistance), int lists `SKILL` (skill level per skill), `UPCNT`,
`UPDATA`, `DOWNDATA` (status buffs), `BASE_*` copies per digimon, object lists `POWERSOCKET`,
`POWERPOCKET`, `MAINSLOT`, `MAINPOCKET`, `SUBSLOT`, `DEVICEFOLDER`, `WAZALIST` (techniques).
Common: `NINZU` (players in game), `ISUSE` per player.

## Data schema

`beNDMWGameRam_initData` builds the data lists from two descriptor tables (`func_003F6620`):
type, size (list length or capacity), name. This is everything a save stores.

Common (`_commonDataList`, `D_004CED40`):

| Name | Type | Size |
|---|---|---|
| `NINZU` | int |  |
| `DIFFICULTY` | int |  |
| `CAMERAMODE` | int |  |
| `LIGHTMODE` | int |  |
| `FLAG` | bool list | 1024 |
| `FLAGFOLDER` | bool list | 12 |
| `COUNTER` | int list | 16 |
| `BKCHIPSTAGE` | string list | 4 |
| `BKCHIPPOS` | float list | 12 |
| `BKCHIPDATA` | int list | 8 |
| `BKCHIPNAME` | string list | 4 |
| `GATESTAGENAME` | string |  |
| `GATESTAGENO` | int |  |

Per player (`_playerDataListList[n]`, `D_004CEDF0`):

| Name | Type | Size |
|---|---|---|
| `UNIQUE` | int |  |
| `PLAYERNAME` | object list |  |
| `XDATA` | int |  |
| `BIT` | int |  |
| `DIGIMONNO` | int |  |
| `LEVEL` | int |  |
| `SKILL` | int list | 9 |
| `UPCNT` | int list | 11 |
| `DIGIMONNAME` | string |  |
| `EXP` | int |  |
| `ISUSE` | bool |  |
| `HP` | int |  |
| `MHP` | int |  |
| `MP` | int |  |
| `MMP` | int |  |
| `TSUYOSA` | int |  |
| `MAMORI` | int |  |
| `KASHIKOSA` | int |  |
| `SEISHIN` | int |  |
| `SUBAYASA` | int |  |
| `HONOOTEIKO` | int |  |
| `KOORITEIKO` | int |  |
| `KAMINARITEIKO` | int |  |
| `YAMITEIKO` | int |  |
| `WAZALIST` | object list |  |
| `NEXTEXP` | int |  |
| `ATTACKDELAY` | int |  |
| `MEICHURITSU` | int |  |
| `CHIMEIRITSU` | int |  |
| `MAHIRITSU` | int |  |
| `KAIHIRITSU` | int |  |
| `BOUGYORITSU` | int |  |
| `MAHITEIKOU` | int |  |
| `DOKUTEIKOU` | int |  |
| `BITRITSU` | int |  |
| `EXPRITSU` | int |  |
| `LUCKRITSU` | int |  |
| `ATTACKZOKUSEI` | int |  |
| `HPDRAIN` | int |  |
| `MPDRAIN` | int |  |
| `CHOHATURITSU` | int |  |
| `WEAPONRECNO` | int |  |
| `WEAPONSKILLNO` | int |  |
| `DEVICEFOLDER` | object list |  |
| `DISKFOLDER` | object list |  |
| `CARDLIST` | bool list | 50 |
| `BANKDEVICE` | object list |  |
| `BANKBIT` | int |  |
| `POWERSOCKET` | object list | 3 |
| `POWERSOCKETNAME` | string list |  |
| `POWERPOCKET` | object list | 5 |
| `MAINSLOT` | object list | 1 |
| `MAINPOCKET` | object list | 5 |
| `SUBSLOT` | object list | 1 |
| `BASE_FLAG` | bool list | 1024 |
| `BASE_FLAGFOLDER` | bool list | 12 |
| `BASE_COUNTER` | int list | 16 |
| `BASE_ISUSE` | bool list | 16 |
| `BASE_LEVEL` | int list | 16 |
| `BASE_EXP` | int list | 16 |
| `BASE_SKILL` | int list | 144 |
| `BASE_UPCNT` | int list | 176 |
| `UPDATA` | int list | 11 |
| `DOWNDATA` | int list | 7 |

Defaults: ints 0, int lists 0, `BASE_LEVEL` 1, `BASE_SKILL` 1, `FLAG` 960 set.

## Messages

| # | Message | Does |
|---|---|---|
| 0 | `GETCOMMON` | push the common data list |
| 1 | `GETPLAYER` | push player n's data (int 0) |
| 2–5 | `GETPLAYER1`–`4` | push that player's data |
| 6 | `GAMEDATAINIT` | `func_003F7310(player)` |
| 7 | `GAMESTART` | start a player with a digimon (`func_003F99F0` / `func_003F96E0`) |
| 8 | `GAMEREMOVE` | player leaves: `ISUSE` off, recount `NINZU` (`beNDMWGameRam_removePlayer`) |
| 9 | `ADDHP` | `beNDMWGameRam_addHp`: HP += delta (or = value), clamp 0..MHP, update meters; optional heal effect |
| 10 | `ADDMP` | `beNDMWGameRam_addMp`: same for MP |
| 11 | `RECALCPLAYER` | `beNDMWGameRam_recalcPlayer`: recompute all stats (below) |
| 12 | `BASE2CURRENT` | `func_003F82A0`: load a digimon's saved stats |
| 13 | `CURRENT2BASE` | `func_003F8760`: save them |
| 14 | `WEAPONEQUIPCHECK` | `beNDMWGameRam_canLearnSkill`: can an unequipped device teach a skill to this digimon |
| 15 | `WEAPONEQUIPSTRONG` | `func_003F8C90(player, 1)` |
| 16 | `DEADRESTART` | every player in game: HP = MHP, MP = MMP |
| 17 | `CTRLITEM` | item handling |
| 18 | `FIGHTCALC` | battle formulas, sub-message in string 0 (below) |
| 19 | `DISPFIGHTDATA` | `_isDispFightData` = bool 0 |
| 20 | `BKCHIPBITEXP` | |
| 21 | `CTRLPLAYER` | `"EXP"` `SET`/`ADD`, `"HP"` `GETHP`/`GETMHP`, `"MP"` `GETMP`/`GETMMP` |

## Stats (`RECALCPLAYER`)

Each stat is a base value from the digimon's growth curve, plus buffs, plus equipment when the
flag is set (`func_003F9F80` power socket, `func_003FA040` power pocket, `func_003FA120` main
slot, `func_003FA1E0` main pocket, `func_003FA2C0` sub slot: each adds the item's parameter of
that name, some also the item's `_revision`).

Base value (`beNDMWGameRam_calcHpMpBase`, `beNDMWGameRam_calcStatBase`), with the digimon's record
in the `shinka` table:

    value = UPCNT[slot] + INI + GROW * curve(LEVEL) / 100

`INI` / `GROW` columns: `MHP_INI`/`MHP_GROW`, `MMP_INI`/`MMP_GROW`, `STR_`, `AC_`, `INT_`, `MND_`,
`DEX_` (strength, defense, wisdom, spirit, speed). `curve` is piecewise linear in the level
(`D_004CF130`..):

| level up to | 5 | 10 | 20 | 30 | 50 | 70 | 110 | 150 | 230 | 999 |
|---|---|---|---|---|---|---|---|---|---|---|
| HP/MP: value at segment start | 0 | 40 | 140 | 640 | 1440 | 3840 | 7440 | 17840 | 25040 | 34640 |
| HP/MP: per level | 10 | 20 | 50 | 80 | 120 | 180 | 260 | 180 | 120 | 80 |
| stats: value at segment start | 0 | 40 | 85 | 165 | 235 | 355 | 455 | 615 | 735 | 895 |
| stats: per level | 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 |

(segments start at levels 1, 5, 10, 20, 30, 50, 70, 110, 150, 230). Then:

- `MHP` = base + `UPDATA[6]` (+ main/sub slot `MAXHP`), max 99999. HP is refilled if it was 0
  or above the new MHP. `MMP` likewise (`MAXMP`).
- `TSUYOSA` = base + `UPDATA[4]` − `DOWNDATA[4]` (+ socket/pocket/sub slot), 1..9999.
  `MAMORI`: index 5, main/sub slot. `KASHIKOSA`, `SEISHIN`: all five slots, no buffs.
  `SUBAYASA`: index 0.
- Resistances: `RES_FIRE` = shinka `RES_FIRE` + `UPCNT[7]` + `UPDATA[7]` + slots, and so on.

## Fight formulas (`FIGHTCALC`)

Arguments are ints pushed by the caller (player, enemy record, attack id, ...). An attack id
`/ 10000 == 4` is a technique (its record comes from `WAZALIST` by `id % 1000` / `% 10000`),
anything else a weapon attack. Results are clamped to 0..99999 between steps.

**`P2EISHITCHECK`** (player hits enemy): weapon attacks hit when

    rand % 10000 < MEICHURITSU * (SUBAYASA*30 + SKILL[WEAPONSKILLNO]*20 + 6000)
                   / (enemy.SUBAYASA * (100 - x) / 100)

(`x` from the enemy work object, +0x108). Techniques always hit.

**`P2EISCRITICAL`**: weapon: `rand % 200 < CHIMEIRITSU`. Technique: element of the technique
(`ZOKUSEI`: fire/ice/thunder/dark/poison) against the enemy's `RES_<element>` `r`:
`rand % 1000 < (tech.CHIMEIRITSU + 100) * (10000 / (r*12/10 + 50)) / 100`; without element
`rand % 100 < tech.CHIMEIRITSU`.

**`P2EDAMAGE`** (player → enemy):

- Weapon: `dmg = (TSUYOSA*6 + SKILL[WEAPONSKILLNO]*4) / (enemy.MAMORI − defenseDown)`. With a
  weapon element `e` (`ATTACKZOKUSEI`, fire/ice/thunder/dark): if `enemy.RES_e < rand % 1000`,
  `dmg += dmg * (1000 − RES_e) / 1000`.
- Technique: `dmg = (skill*5 − tech.HP*20 + KASHIKOSA*10) / enemy.SEISHIN` (only if the
  technique has an `HP` value), then `dmg = dmg * (1000 − enemy.RES_element) / 1000`.
- Charged (int 3): `dmg += dmg * bonus / 100`, bonus = `skill/20 + 30` (skill ≤ 1000),
  `(skill−1000)/50 + 80` (≤ 2000), else `(skill−2000)/100 + 100`.
- Spread: `k = rand & 3`, `dmg = (rand % (dmg*25) + dmg * {950, 975, 1000, 1025}[k]) / 1000`;
  `k == 3` is the critical flag.
- Weapon only: heal `dmg * HPDRAIN / 100` HP and `dmg * MPDRAIN / 100` MP.
- Returns damage, element, critical.

**`E2PISHITCHECK`** (enemy hits player): always if the enemy attack's `TYPE` != 0; otherwise the
player guards when `rand % 100 < BOUGYORITSU`, else hit when
`rand % 100 < atk.MEICHURITSU * enemy.SUBAYASA / (SUBAYASA * (KAIHIRITSU + 100) / 100)`.

**`E2PDAMAGE`** (enemy → player), by the attack's `TYPE`:

- 0 physical: `dmg = (enemy.TSUYOSA − x) * atk.TSUYOSA / MAMORI`; element resistance `R`
  (`HONOOTEIKO`, ...): unless `rand % 1000 < R/5`, `dmg += dmg * f / 100` with
  `f = (1000−R)/20 + 50` (R ≤ 1000) or `(5000−R)/80` (≤ 5000) or 0. Critical: unless
  `rand % 10000 < (100 − (enemy.CHIMEIRITSU − y)) * (100 − atk.CHIMEIRITSU)`,
  `dmg += dmg * (atk.CHIMEIRITSU/2 + 90) / 100`.
- 1 magic: `dmg = enemy.KASHIKOSA * atk.TSUYOSA / SEISHIN`, element: `dmg = dmg * f / 100` with the
  same `f` (or `dmg / 10` above 4200).
- 2 percent: `dmg = MHP * atk.TSUYOSA / 100`, element as type 0.
- Then the same spread as `P2EDAMAGE`. Returns damage, element, critical.

**`P2EEXPUP`** (enemy defeated, details and mod addresses in `docs/exp.md`): for every living player below level 999,
`exp = enemy.EXP * (EXPRITSU + 100) / 100`, scaled by party size (`NINZU` 1: ×50, 2: ×140/3,
3: ×40, 4: ×40), then `/50` for the player who killed it and `/100` for the others. Level up while
`EXP ≥ NEXTEXP` (recalc + refill HP/MP, `SQ_LVUP` effect).

**`P2ESKILLUP`**: chance to raise the skill used: a cap from the level
(`LEVEL*30` up to 36, `LEVEL*20+360` to 107, `*10+1430` to 227, `*8+1884` after); below the cap,
the chance comes from a table by skill level and the digimon's `<TYPE>_GROW` column in `shinka`
(`BLUNT_GROW`, `SLASH_GROW`, ...); on success `SKILL[n] + 1`, recalc, `SQ_TECUP` effect.

**`P2ESTUNTIME`**: if `rand % 100 < (MAHIRITSU + power*50/time + 100 − enemy.RES_STUN) / 2`,
stun time = `(200 − RES_STUN) * (power*30/time + MAHIRITSU*3) / 400`.

**`P2EPOISONTIME`** (`"TIME"`): chance per mille `(750 + skill/4) * (1000 − RES_POISON) / 1000`
(steeper above skill 1000/2000); time = `tech.TIME + KASHIKOSA/2`.

The other sub-messages (`E2P*TIME`, `P2E*TIME` for sleep/bind/attack down/power down,
`E2PDAMAGERANK`, `P2PKOUKAMAGIC`, `GETPATKZOKUSEI`) follow the same patterns; see `beNDMWGameRam_handle`.
