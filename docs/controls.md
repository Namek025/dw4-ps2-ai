# Controls

Pad masks: the standard libpad layout (the backup chip mod uses X = `0x4000` and works).

| Button | Mask | Button | Mask |
|---|---|---|---|
| SELECT | `0x0001` | L2 | `0x0100` |
| L3 | `0x0002` | R2 | `0x0200` |
| R3 | `0x0004` | L1 | `0x0400` |
| START | `0x0008` | R1 | `0x0800` |
| Up / Right / Down / Left | `0x10` / `0x20` / `0x40` / `0x80` | △ / ○ / X / □ | `0x1000` / `0x2000` / `0x4000` / `0x8000` |

`bePadData` (`bePadManager_getPad(manager, port)`): `_LStick` `+0xC`, `_RStick` `+0x14`,
`_button` `+0x44` (held), `_trigger` `+0x48` (pressed this frame), `_repeat` `+0x4C`.

## Field (`func_003D22F0`, beNDMWMdlPlayer update, pad manager at `+0x30`)

Reads `_trigger` and handles the first match:

| Button | Action |
|---|---|
| □ | model sequence `D_00516120` |
| ○ | model sequence `D_00516118` |
| △ | message `D_00516170` to group `D_00516148` |
| L1 | message `D_005161A0` (same group) |
| R1 | message `D_005161B0` |
| R2 | message `D_005161C0`: the status / effects panel (in game) |
| X | attack; evolution when one is pending (`+0x125`) |
| START | alone: pause / menu |

L2 is not read by any game code (the `0x100` tests are all in the Sony SDK, Alchemy and CRI
libraries): free for a mod. Found with `tools/xref.py --re 'andi .*, 0x(100|200|300)$'`.

## Technique panel (`beNDMWPanelWaza_updateRam`, `0x3CFE50`)

R1 / L1 / R2 select tab 0 / 1 / 2 (nothing if already there).
