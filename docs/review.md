# Functions to review together

Hard or interesting functions left for a joint look once the bulk pass is done.
"Near" = C written, a few instructions off; "open" = not attempted in depth yet.

## src/unk_2B4358.c (CRI SFX/Sofdec-style code: `SFXZ_*`, `sfxcnv_*` error strings)

| Function | State | Notes |
|---|---|---|
| `func_002B4380` | near | Two lookups into a header, stores `{1, a, b}` triples at +0x18/+0x28. Store order in the zero branch and in/out pair order differ |
| `func_002B45B0` | near | Same lookup with an early-out; register choice (`a0` vs `t1`) and branch shape |
| `func_002B5770` | near | Scans a buffer for `"<SUDPS_>"` headers with `memcmp`; loop shape (double `blezl` guard) |
| `func_002B4C98` | near | Dispatch on `*arg1` (2/3); the constant 2 shares a register with the error call's 4th arg in the original |
| `func_002B5848` | near | Hardware timer init (`D_004F5A70`/`D_004F5A80` register tables, env lookups `func_0026F4B0`) |
| `func_002B5A10` | near | Timer read: 16-bit counter extended to 64 bits on wrap |
| `func_002B4648`, `func_002B4960`, `func_002B4DC8`, `func_002B4F58` | open | Larger table builders / converters |

Matched with a trick worth showing:

- `func_002B5068`: a `switch` with an extra dead `case 0x8` reproduces GCC's decision tree.
- `func_002B52D8`, `func_002B56F8`: 2/3-byte struct copies (`lb`/`sb` pairs) instead of char assignments.
- `func_002B50E8`: `void* volatile` global.
- `func_002B4498`: `%lx` reads `long`, which is 64-bit here.

## Game code (MWCC C++)

The biggest and most interesting one:

| Function | State | Notes |
|---|---|---|
| `beNDMWGameRam_handle` (`0x3FCA10`) | open | The GAMERAM message handler, ~10.400 instructions: stat recalculation, damage / hit / critical / EXP / skill-up formulas (`docs/gameram.md`). Read from m2c; matching it needs the out-of-line helpers it calls (`0x406DA0`..) as real inline methods |
| `beNDMWGameRam_recalcPlayer` (`0x3FAB90`) | near | All stats from the growth curve + buffs + equipment (1720 instructions). C written in full under `NON_MATCHING`; stack layout (352-byte frame, spills) and `pl`/`player` registers differ |

Near (C written, registers or one block off):

| Function | Notes |
|---|---|
| `beMessenger_send` | The "SEARCH" loop's guard sits at the end of the function and `_data` is reloaded every iteration |
| `beMessenger_delaySelect`, `beMessenger_delaySend`, `beMessenger_update` | Registers only (parameter / smart pointer temporaries) |
| `beDataBase_getIndex` | The original keeps the found column in a stack slot (address taken somewhere) |
| `beCameraMode_update` | Stack slots of the model-change record vs the matrix; `dt + now` operand order |
| `beActionStarter_handle` | The original rereads the model count in the first search loop and keeps the info in `s7` (ram number spilled); ours hoists the count |
| `beNDMWWindow_playCloseModels`, `beNDMWWindow_isCloseDone` | string + int list loop: s-registers rotate |
| `beNDMWGameRam_buildData` (`0x3F6620`) | The data schema builder (8 object types); registers all over the fill loops |
| `beNDMWGameRam_equipPowerSocket` | Registers and spill slots, most s-registers in use |
| `beNDMWGameRam_current2Base`, `_base2Current` | `d * 9 + i` recomputed each iteration in the original |
| `beNDMWGameRam_calcHpMpBase` | `(base + 1) * 4` not folded into the load offset |
| `beMeterCtrl_handle`, `beMeterCtrl_setMax` | Float register choice |
| `MDLisCallName` (script_bind) | Dead `nop` aligning the else block; returns bool without `andi` |
| `MDLisLive` (script_bind) | `sltu`/`xori` test on `getBool` with no `andi`, as if it returned int |

Matched with a trick worth showing:

- `beMessenger_pushInt` & co.: the count is reread for the increment: `(*(volatile s32*)&list->_count)++`.
- `beMessenger_select`: `w.p->_infoName` next to `w->_infoName` makes MWCC reload the field.
- `beMessenger_getMessageIndex`: the lookup returns a result struct compared with `operator==`.
- `beDBManager_handle`: a one-case `switch` on the message index.
- `SYSgetWeaponName`: `names->count <= slot` (not `slot >= names->count`) compares into `$at`.
- `DMgetCount`: a result variable assigned in each branch keeps the original's empty delay slot and `b`/`nop`.
- `beNDMWWindow_callLua`: per-class `igDynamicCast` (meta read inside the inline).
- `beNDMWGameRam_canLearnSkill`: `1 << digimon` in its own variable, the current digimon in a local before the compare, and the declaration order from `tools/permute_decls.py`.
- `beNDMWGameRam_addPowerSocket`: the sums are unsigned (`u32*`), or the add's operands swap.

## Elsewhere

- `src/script_bind.cpp`: 5 left (`SYSctlShinka`, `func_00372CF0`, `MDLisLive`, `MDLisCallName`, `MDLisSeqName`).
- `src/lua/lvm.c`: `luaV_gettable` (reloads `hvalue(t)` before `luaH_get`).

Not attempted yet (engine-level, inside new units):

| Function | Notes |
|---|---|
| `func_0039C460` (`src/beSeCtl.cpp`) | Scene-graph traversal callback (igModelViewMatrixAttr, attribute stack push/pop), Alchemy internals |
