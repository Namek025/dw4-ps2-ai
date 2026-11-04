# Experience (EXP) and levels

Everything about EXP lives in the GAMERAM manager (`beNDMWGameRam`, `src/beNDMWGameRam.cpp`).
Scripts reach it through the Lua bindings; the C side is the `FIGHTCALC` / `P2EEXPUP` message of
`beNDMWGameRam_handle` (`0x3FCA10`), the `NEXTEXP` step of `beNDMWGameRam_recalcPlayer`
(`0x3FAB90`), `CTRLPLAYER` and `BKCHIPBITEXP`.

File offset in `SLUS_208.36` = address − `0x100000` + `0x80`.

## Data

Per player (`GETPLAYERn` data list, see `docs/gameram.md`):

| Field | Type | Meaning |
|---|---|---|
| `LEVEL` | int | 1..999 |
| `EXP` | int | total EXP (cumulative, not per level) |
| `NEXTEXP` | int | total EXP needed for the next level (0 at 999) |
| `EXPRITSU` | int | EXP bonus in %, sum of the equipped sub slot items' `EXPRITSU` |
| `ISUSE`, `HP` | bool, int | only players in the game and alive get EXP |

Common data: `NINZU` (players in the game), `BKCHIPDATA` / `BKCHIPNAME` (backup chip).
Enemy table (`DATABASE` / `enemy`): column `EXP` per enemy record.

## Who calls what

| Lua command | Binding | GAMERAM message |
|---|---|---|
| `BLTcalcEXPUp()` | `0x003714D0` | `FIGHTCALC` `"P2EEXPUP"` (killer player, enemy record from the battle globals `D_0058A208`, `D_0058A1F4`); returns the killer's EXP |
| `SYSctlPlayer("EXP", "SET"/"ADD", player, value)` | `SYSctlPlayer` | `CTRLPLAYER`: set or add EXP directly (no level up) |
| `SYSgetPlayerLevel` | `0x00370AC8` | reads `LEVEL` |

## EXP for a defeated enemy (`P2EEXPUP`, `0x3FF748`–`0x3FFD20`)

For each player `i` (0..3) with `ISUSE` and `HP != 0` and `LEVEL != 999`:

    exp = enemy.EXP
    if (EXPRITSU != 0) exp = exp * (EXPRITSU + 100) / 100
    switch (NINZU) {
    case 2:  share = exp * 140 / 3; break;   /* x46.67 */
    case 3:  share = exp * 160 / 4; break;   /* x40 */
    case 4:  share = exp * 200 / 5; break;   /* x40 */
    default: share = exp * 100 / 2; break;   /* 1 player: x50 */
    }
    if (i == killer) share = share / 50;     /* killer: x1 (alone), x0.93 (2), x0.8 (3, 4) */
    else             share = share / 100;    /* others: half of that */
    EXP += share

Then the level up loop:

    while (EXP >= NEXTEXP) {
        LEVEL++;
        recalcPlayer(i, true);   /* new stats and new NEXTEXP */
        current2Base(i);
        if (LEVEL == 999) break;
    }

After a level up: HP = MHP, MP = MMP, `SQ_LVUP` effect on the model (`MODELCTRL` /
`i_getitempnl01`), and the level, HP and MP numbers and meters are refreshed (`NUMBERCTRL` `lvnm`,
`hpnm`, `mpnm`; `METERCTRL` `hpmt`, `mpmt` at 100%).

The message returns the killer's share (`clearInts` + `pushInt`).

In short: alone you get the enemy's `EXP` as is; in a party the killer gets less and the
others half of the killer's share. `EXPRITSU` items add a percentage before that.

## EXP needed per level (`NEXTEXP`, in `recalcPlayer`, `0x3FBE88`–`0x3FBED0`)

    NEXTEXP = LEVEL * 200 + (LEVEL + 140) * LEVEL * LEVEL     /* 0 at level 999 */

It is a total, not a delta. Some values:

| LEVEL | NEXTEXP |
|---|---|
| 1 | 341 |
| 2 | 968 |
| 5 | 4,625 |
| 10 | 17,000 |
| 20 | 68,000 |
| 50 | 485,000 |
| 100 | 2,420,000 |
| 200 | 13,640,000 |
| 500 | 160,100,000 |
| 998 | 1,133,652,152 |

## Backup chip (`BKCHIPBITEXP`, `0x406AD0`–`0x406D60`)

Stores the player's name and money in the backup chip and moves part of its EXP there:

    BKCHIPNAME[player] = DIGIMONNAME
    BKCHIPDATA[player*2] = BIT;  BIT = 0
    if (LEVEL != 999 && LEVEL != 1) {
        cur  = NEXTEXP of LEVEL-1                       /* EXP at which this level started */
        prev = NEXTEXP of LEVEL-2
        keep = EXP - (cur - prev) * 2 / 10              /* lose 20% of the previous level's span */
        if (keep < cur) keep = cur                      /* never below the start of the level */
        BKCHIPDATA[player*2 + 1] = EXP - keep
        EXP = keep
    }

So a level is never lost.

## Where to change it (mods)

The multiplications are done with shifts and the divisions with magic-number multiplies, so the
factors are not single immediates. What can be patched easily:

| What | Address | Instruction | Notes |
|---|---|---|---|
| Killer share `/ 50` | `0x3FF9CC` | `sra $2, $2, 4` (after `mult` by `0x51EB851F`) | `sra 3` = `/ 25` (double EXP for the killer), `sra 5` = `/ 100` |
| Others' share `/ 100` | `0x3FF9FC` | `sra $2, $2, 5` (after `mult` by `0x51EB851F`) | `sra 4` = `/ 50` (same as the killer) |
| `EXPRITSU` base `+ 100` | `0x3FF88C` | `addiu $3, $2, 0x64` | |
| Party scaling | `0x3FF8E0`–`0x3FF9A4` | shift/add chains | e.g. 1 player: `exp*100 >> 1` |
| Level cap 999 | `0x3FF7F4`, `0x3FFA8C` (EXP), `0x3FBE8C` (`NEXTEXP`) | `addiu $x, $0, 0x3E7` | also used elsewhere (stats, backup chip) |
| `NEXTEXP` curve | `0x3FBEA0`–`0x3FBEBC` | `LEVEL*200` by shifts, `+ 0x8C` (140) | `0x3FBEA4` `addiu $5, $2, 0x8C` is the 140 |

The data route is simpler: the `EXP` column of the enemy table (in the game archives, read
through `DATABASE` / `enemy`) scales everything without touching code.

Once `beNDMWGameRam_handle` matches, these become C changes (`docs/review.md`); its C is in
`src/beNDMWGameRam.cpp` under `NON_MATCHING` (sub-message `case 6` of `FIGHTCALC`).

## Mod: EXP per group (`patches/SLUS-20836_B871C1D0.pnach` in dw4-mods)

PCSX2 patch (the game's CRC is `B871C1D0`), written to try the formula before the handler matches:

    switch (NINZU) { case 2: share = exp * 80; case 3: exp * 60; case 4: exp * 50; default: exp * 100; }
    share = share / 100;   /* same for the killer and the others */

Each case becomes `li v0, K` / `b 0x3FF9A8` / `mult s5, s0, v0` (R5900 3-operand `mult`), and the
killer's `sra 4` (`/ 50`) at `0x3FF9CC` becomes `sra 5` (`/ 100`). Install: copy it to PCSX2's
`patches` folder and enable each section in the game's properties (Patches).

Tested in PCSX2 (2026-10-26), 2 players, Goburimon (base EXP 10), killed by P1: original gives
P1 9 / P2 4 (`10*140/3 = 466`, `/50`, `/100`); with the patch both get 8. Disabling the cheat does not
restore the code, and save states keep the patched RAM: compare from a fresh boot.

## Mod: EXP x2 (same pnach)

`0x3FF870` `move s0, v0` (the enemy's `EXP` from the database) becomes `addu s0, v0, v0`. Same effect
as halving `NEXTEXP`, but safe with existing saves: halving the curve would level up a saved player
several times on the next kill, and turning it off later would leave them below their level's EXP.
Tested in PCSX2 (2026-10-26) with both sections: 16 EXP per Goburimon for each of 2 players. PCSX2 only
picks up a changed pnach file after closing and reopening the VM.
