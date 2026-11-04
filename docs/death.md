# Death and the backup chip

When a player dies, its BIT and part of its EXP go into a "backup chip" left at the place of
death. The chip is a model (`beNDMWMdlPlayer2`, info group `"PLAYER2"`, one per player: the ram
number is the owner's player index). In the original game the only way to get the chip back is
Raise (the skill or the Raise Disk) near it.

## Data (GAMERAM common fields, `docs/gameram.md`)

| Field | Type | Per player |
|---|---|---|
| `BKCHIPSTAGE` | string ×4 | stage where the chip is (empty = no chip) |
| `BKCHIPPOS` | float ×12 | position (x, y, z) |
| `BKCHIPDATA` | int ×8 | `[2i]` BIT, `[2i+1]` EXP |
| `BKCHIPNAME` | string ×4 | Digimon name when it died |

## On death

- `func_003D45F0` (`beNDMWMdlPlayer`) sends `BKCHIPBITEXP` to the GAMERAM: all the BIT and 20%
  of the current level's EXP span go into the chip (never below the level start; `docs/exp.md`).
- `func_003D4A20` stores `BKCHIPSTAGE` / `BKCHIPPOS`.

## The chip (`beNDMWMdlPlayer2`)

Manager size 0x30 (parent `beModelCtrl`): `_messenger` +0x14, `_lua` +0x20, `_padManager` +0x2C.
Its work (`beNDMWMdlPlayer2InfoWork`): `_isPowerOn` +0xEC, `_timerCount` +0xF0, `_bit` +0xF4,
`_exp` +0xF8.

**Update, `func_003D4EA0` (0x21C bytes):**
1. Creates the ram's data list if missing; gets the work.
2. `_timerCount` counts up to 10 (the chip ignores `RAISE` before that).
3. Calls `bePadManager_getPad(_padManager, ramNo)` and drops the result.
4. `beTargetObj_search(chip matrix, "PLAYER", "NEAR")`: nearest player. If found, with the squared
   distance `d`:
   - off and `d < 900` (30 units): power on (`_isPowerOn = 1`, vtable `+0x8C` with the on action);
   - on and `!(d <= 900)`: power off (same call, off action).
   If no player: runs a Lua script (`func_00389FF0`).

**Handler, `func_003D50C0` (vtable `+0x90`), message `"RAISE"`** (`D_005166A0`; arguments
matrix, int `a`, int `b`):
1. Ignored if `_timerCount < 10`.
2. If the owner is dead: sets its HP-like int (`D_005166C0`) to `b` and respawns it at the matrix.
3. `BIT += BKCHIPDATA[2i]`, slot cleared.
4. If the current Digimon name equals `BKCHIPNAME[i]`: `EXP += BKCHIPDATA[2i+1] * a / 100`, slot
   cleared. (A different Digimon gets no EXP back.)
5. Deletes the chip (delayed call) and clears `BKCHIPSTAGE[i]`.

Another message (`D_00516660`) places the chip at `BKCHIPPOS` when the stage matches.

**Raise** (`beNDMWGameRam.cpp`, technique 50 = Raise skill, -50 = Raise Disk with `a = D_004CF600[0]`,
`b = D_004CF620[0]`): finds `"PLAYER2"` members within 30 units (`func_003FC780`) and sends each
`select("PLAYER2", NULL, ramNo); pushMatrix; pushInt(a); pushInt(b); send("RAISE")`.

## Mod: pick up your chip with X

`mods/src/chip_pickup.cpp`, pnach section `Death: recover your chip with X`. Tested in game.

Rewrites the update (`func_003D4EA0`) in C++ and builds it with `tools/mkpnach.py` (0x200 of the
0x220 bytes up to the next function):

- Same logic, but the chip powers on at 15 units (`CHIP_RANGE`, squared 225) instead of 30, so the
  light shows when X works.
- While it is on, if the nearest player (`beTargetObj_getTargetRamNo`) is the owner and the owner's
  pad has X in `_trigger` (`+0x48`, mask `0x4000`): sends the chip `"RAISE"` like the Raise Disk,
  with `a = 100` (all the EXP back) and `b = D_004CF620[0]`.
- Clears X from the pad's `_trigger`. The chip updates before the player in the frame, so the
  player doesn't attack.

Notes:
- Only the owner can pick up its chip. Allies are still revived with Raise.
- If an ally stands closer to the chip than the owner, X doesn't work until they move.
- To make it fit: `getPad` only when needed, the on/off actions share one call (`_isPowerOn ^= 1`),
  and early returns so the action pointer needs no saved register.
