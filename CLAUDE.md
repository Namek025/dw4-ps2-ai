# Digimon World 4 (PS2) — Matching Decompilation

## Goal

1. Matching decompilation of the main ELF: C/C++ that compiles byte-for-byte to the original.
2. Later: native PC port on top of the recovered source (replace GS/VU1/DMA/IOP code with a PC backend).
3. Later: mods (stats, new playable characters, game modes, missions).

## Hard rules

- **Never commit game data.** `disc/`, `*.iso`, the ELF, extracted assets and generated asm from the game stay out of git (see `.gitignore`).
- Never modify the original ELF. It is the reference.
- A match is only a match if the build's SHA-1 equals the target's SHA-1 (or objdiff shows 100% for that function).
- Don't touch splat config, linker script or build scripts as a side effect of decompiling a function. Changes there are deliberate and explained.
- Period C is C89: declarations before use, variables at block start, no `for (int i ...)`.

## Target

| Property | Value |
|---|---|
| Game | Digimon World 4, NTSC-U, SLUS-20836, disc `VER = 1.01` |
| ELF | `disc/SLUS_208.36` (4,541,744 bytes) |
| ELF SHA-1 | `d6c8bbcd6b3a0b3f135f7559d098af088ba06e2a` |
| Entry point | `0x00100008` |
| Symbols | None. `.symtab` empty, section names stripped |
| Compiler banner (`.comment`) | `MW MIPS C Compiler (2.4.1.01)` (Metrowerks CodeWarrior) |
| Sony SDK libs | `Append: GCC2096 SCE2810`: SDK 2.8.1.0, prebuilt with GCC 2.96 |
| Engine | Intrinsic Alchemy v3.2 -0810 (Static/Release), C++, linked statically |
| Scripting | Lua 4.0.1, embedded. 116 game commands bound to C, see `docs/script_bindings.md` |
| Audio/archives | CRI ADX (`CRI_ADXI.IRX`), CRI AFS containers (`AREA.AFS`, `NDMW.AFS`) |

### Memory map (program headers)

| Range | Size | Contents |
|---|---|---|
| `0x00100000`–`0x00467120` | `0x367120` | Main code (`.text`) |
| `0x00467120`–`0x0051FD00` | | Data. Includes Sony kernel-patch code stored as data around `0x481F28` (refs `0x80075300`) |
| `0x0051FD00`–`0x00522440` | `0x2740` | C++ static initializers (16 functions), code in the middle of data |
| `0x00522440`–`0x00522480` | `0x40` | Pointer table to those 16 initializers, walked by `func_001029D0` (called from `_start`) |
| `0x00522480`–`0x00554B00` | | Data |
| `0x00554B00`–`0x0058D280` | `0x38780` | BSS (MemSiz − FileSiz) |
| `0x0058D280` | 0 | Empty LOAD segment, likely heap start |

File offset of the first segment: `0x80` (vaddr `0x100000` ↔ file offset `0x80`).

### Compiler identification

**Most of `.text` is MWCC C++** (corrected 2026-10-25): `0x130000`–`0x250000` and `0x2B5B68`–`0x467120`
are all 16-byte aligned functions that save with `sq`, with no branch-likely instructions. That is the
Alchemy engine and the game (`be*` classes). Use the script_bind flags (below) for new units there.

**EE-GCC 2.96, `-O2 -G0`** (delay slots filled by GCC) is only `0x100000`–`0x130000` (Sony libs, libc)
and `0x250000`–`0x2B5B68` (CRI ADX/Sofdec middleware: `src/unk_2B4358.c`). Some units add
`-fno-delayed-branch` (per-file `CC_EXTRA` in `configure.py`). Per 64 KB region, `sq` saves vs
branch-likely tell them apart.

Evidence:
- `func_002B4330` and `func_002B5B38` match 100% on decomp.me (preset "EE GCC 2.96", which adds `-fno-delayed-branch`). No MWCCPS2 build (2.3.3, 2.4, 3.0, 3.0.1 b87/b95/b103/b119) gets the address-materialization pattern right (`lui`+`addiu` once, then `0(reg)`).
- Almost every non-leaf function in `0x100000`–`0x467120` saves callee-saved regs with `sd`. Per the Persona 4 decomp, MWCCPS2 3.0.1 uses `sq` and ee-gcc 2.96 uses `sd`.
- The `.comment` banner `MW MIPS C Compiler (2.4.1.01)` comes from the **Metrowerks linker**, not the C compiler (also per the Persona 4 decomp).
- Delay slots (corrected 2026-10-25): branch-likely instructions (`beql`, `bc1tl`, ...) are everywhere around
  `0x2B3800`–`0x2B5B68`, and only GCC makes those, so most code is plain `-O2`; `func_002B4468` matches
  only with delay slots on. `func_002B4330` is the odd one: its branch slot is empty
  and no source shape reproduces that with delay slots on, so it is its own unit with `-fno-delayed-branch`
  (GCC leaves slots empty and **gas fills them** in reorder mode when safe). Branchless functions don't tell.

**Exception: `src/script_bind.cpp` (`0x370780`–`0x375DF0`) is MWCC C++**, `-O4,p -sdatathreshold 1 -lang c++`
(C++ exceptions on, the default). Found 2026-10-24 while matching the first 25 bindings:
- Global loads use `lui $at` + `lw rX, %lo(sym)($at)`. ee-gcc never emits that (its `as` expands
  `lw rX, sym` through rX). In all of `.text` this pattern only appears in `0x37185C`–`0x375DD4`.
- Every function starts on a 16-byte boundary. Callee-saved regs saved with `sq`.
- `-sdatathreshold 1`: `const bool&` temporaries (1 byte) are `$gp` constants in `.sdata` (`0x5549BC`..),
  but `"be"` (3 bytes) is not small data. A 1-byte global accessed absolutely (`D_0058A210`) must
  then be part of a larger object.
- No tail calls: `f(a){ g(a, 0); }` keeps a full frame. That only happens with C++ exceptions on.
- Version: **3.0b50 / 3.0b52**. 3.0b38 matches all of script_bind too, but loses on Lua (320 vs 328 exact in the same early pass).
  3.0, 3.0.1, 3.0.3 miss a `nop` before a branch target after FPU stores (`MDLsetWorldPosTarget_0`);
  2.3.3/2.4 and 3.0.1b44+ are further off. Using 3.0b52-030722.
- Build: MWCC compiles only the decompiled functions (`INCLUDE_ASM` is empty under `__MWERKS__`),
  `tools/mwsplice.py` rebuilds the unit as one `.s` in address order, then normal `as`.
  Data the unit defines (literals, tables, switch tables, `const bool&` temporaries) is placed at its
  original address, read from the target rom (instructions that use it, `symbol_addrs.txt`, pointers
  in data already placed), and the compiled bytes are checked against the rom, so a wrong data guess
  fails the build. Exception tables are dropped. If splat gives the unit a `.rodata` subsegment
  (`linker_section_order: .data`, in the data area), mwsplice emits that range as a real `.rodata`
  section from the compiled data, with labels and relocations; otherwise the data stays in the
  original bin and relocations point at absolute addresses. Lua's `.rodata` is split this way. Names are unmangled via `extern "C"` (no symbols to say otherwise).
  For C++ units without `extern "C"` (Lua), mwsplice strips the mangling (`f__FPi` -> `f`), and drops
  compiled functions that are not in the unit's splat asm (the Metrowerks linker deadstrips unreferenced ones).

Empty delay slot + `b`/`nop` pairs come from a result variable assigned in each branch
(`r = n; ... return r;`, matched `DMgetCount`). Still open: `MDLisCallName`, `MDLisLive` (notes in the source).

**Lua 4.0.1 (`0x246D30`–`0x2590C0`, `src/lua/`) is MWCC too**, but `-O4,s -str readonly -lang c++
-Cpp_exceptions off`, with `long` as 32 bits in the core files only (flags per path in `configure.py`), and
newlib's libc (`include/libc/`: ctype macros, `_impure_ptr`, MWCC's `va_start` builtins). Stock source, one
unit per file (plus a few `port:` changes); 430/431 functions match (`docs/lua.md`).

**Alchemy engine units (`src/alchemy/`) are MWCC 3.0b52 too, but `-O4,s`** (found 2026-10-28 in
`igIGBFile.cpp`): with `-O4,p` a call right before a branch target gets an extra `nop`. Flags per path
in `configure.py` (`ALCHEMY_DIR`). C++ exceptions stay on; a tail call (`j`) needs
`#pragma exceptions off` around that one function.

Still open: flags for the Sony SDK libs (`0x102000`–`0x126000` region, strings `PsIIlib*2800`).

### Alchemy reflection data (found 2026-10-25)

Every Alchemy/game class registers a meta object at startup (`metaRegister`, `0x2FE9C0`): name, instance
size, parent, field names / types / offsets, and a create function that gives the vtable.
`tools/dump_meta.py` extracts all of it: **1075 classes, 3680 fields with their original names**
(`docs/meta_classes.md`; `--symbols` names `<Class>_Meta`, `_register`, `_registerFields`, `_getMeta`
in `symbol_addrs.txt` between generated markers). `tools/gen_meta_header.py` writes `include/meta.h`
(every struct, parents as base classes; MWCC agrees with every size and offset). Use those types.

Registration boilerplate (found 2026-11-01): each library has a block of `_register` /
`_registerFields` plus, per class, createMeta (`0x44`), getClassMeta (`0x4C`), an ark-register
trampoline (`0x20`), the `_getMetaObjectFunction` thunk (`0x1C`), retrieveVTable, instantiate (`0x34`)
and enum getters (`0x54`). `include/ig_meta.h` has one macro each; `tools/meta_src.py src/UNIT.cpp..
--check` recognizes them in the asm, writes the macros + prototypes, compiles and reverts any that
differ. `_registerFields` go through a small symbolic run over the asm: per field `IG_FIELD(k)` /
`IG_FIELD_OVERRIDE(name)` (copy an inherited field, change it, put it back), `IG_SET(type, off, v)`,
`IG_CALL(setter, v)`. Not covered yet: retrieveVTable of classes with a real constructor/destructor
(they build the class on the stack: need real class definitions), the deleting destructors (`0x80`,
`func_002FF820` + `__dl__FPv`) and out-of-line inline constructors next to them.
Tricks: the inherited field and its copy share one variable (else s0/s1 swap); Alchemy tail calls need
`#pragma exceptions off` around the function (`_Pragma` and `throw()` don't work in MWCC).
Ndmw block: `src/libNdmwRuntimePlugin.cpp`. Bec block: `src/libBecRuntimePlugin.cpp`. Alchemy: 37
`src/alchemy/meta_*.cpp` units cut from `tools/meta_blocks.py` runs (-O4,s: thunks and ark trampolines
are tail calls, so the generator wraps them in `#pragma exceptions off` / `reset`), plus 3
`src/meta_igViewer*.cpp` (the igViewer library is built `-O4,p`, like the game).

Game classes start with `be` (458 of them), e.g. `beNDMWGameRam` (the "GAMERAM" handler: `_dBShinka`,
`_dBWeapon`, `_dBEnemy` databases), `beNDMWShinkaObject` (evolution stats), `beNDMWDegiData`.
Messages (`src/beMessenger.cpp`): objects (`beBaseInfoRam`) register under a group name; a call selects
a group/info/ram and sends a message name; the ram's manager (`_parentManager`) handles it in its
vtable slot `+0x90` (`handle`, a switch on the name; e.g. `beNDMWGameRam`'s is `func_003FCA10`, ~41 KB).

### Code regions (rough, from strings and xrefs)

| Range | Contents |
|---|---|
| ~`0x102000`–`0x126000` | Sony SDK libs (`PsIIlib*2800`) |
| `0x130000`–`0x460000` | Alchemy engine, spread out: 643 functions reference 622 `ig*` class-name strings (likely per-class metadata registration; a way to name them later). Densest at `0x130000`–`0x1B0000`, `0x2B0000`–`0x2D0000`, `0x440000`–`0x460000` |
| ~`0x122000`–`0x12D000` | C library (`memcpy`, `sprintf`, `setjmp`, math), named from Lua's calls |
| ~`0x230000` | MWCC soft-float `double` runtime (`fptodp`, `litodp`, `dpadd`, `dptoli`, ...) |
| ~`0x231000`–`0x246D30` | Alchemy, MWCC-style (16-byte aligned, `sq`): `igMessenger`, `igInsight*`, `igCollision*` (~`0x231000`), then `igLuaState`/`igLuaGlue` (~`0x23C5A0`): Alchemy's C++/Lua binding (`Cannot find field "%s" on class "%s"`), plus the game's own `lua_dofile` (`0x246B30`) |
| `0x246D30`–`0x2590C0` | **Lua 4.0.1**, `src/lua/` (`docs/lua.md`). MWCC C++ |
| `0x370780`–`0x375E00` | Script binding dispatcher + 116 command implementations: **game logic** (MWCC C++) |
| `0x38D560`–`0x38F970` | `src/beMessenger.cpp`: message system (select / send / delayed / argument lists) |
| `0x3FCA10`–`0x406D68` | `beNDMWGameRam` message handler (GETPLAYERn, ADDHP, RECALCPLAYER, FIGHTCALC, ...) |
| `0x3D4EA0`, `0x3D50C0` | `beNDMWMdlPlayer2`: backup chip left on death (update, `RAISE` handler). `docs/death.md` |
| `0x3D22F0`, `0x3CFE50` | Player control (pad buttons) and technique panel. `docs/controls.md` |
| `0x3F82A0`, `0x3F8760`, `0x4432A0` | `BASE2CURRENT` / `CURRENT2BASE` (form switch) and the evolve menu. `docs/evolution.md` |
| `0x2B4330`–`0x2B4358` | `src/unk_2B4330.c` (`func_002B4330`, `-fno-delayed-branch`) |
| `0x2B4358`–`0x2B5B68` | `src/unk_2B4358.c` |

Reference project: Persona 4 PS2 decomp (`github.com/Raikaru/Persona4-Decompilation`), MWCC game code + ee-gcc 2.96 for CRI/Sony units.

## MWCC matching tricks and tools

See `docs/matching_tricks.md`. Read it before working on a function.

## Conventions

- Unknown functions: `func_XXXXXXXX` (address). Unknown data: `D_XXXXXXXX`.
- Unknown struct fields: `unk` + hex offset (`unk0`, `unk4`, `unk1C`).
- Lua command implementations: the script name as is (`SYSgetPlayerLevel`), once verified.
- Rename as soon as meaning is known; keep `symbol_addrs.txt` as the source of truth for names.

## Strategy and roadmap (decided 2026-11-01)

Goal behind everything: **mods** (new playable characters, quests) and later a **PC port**. So:

- **Matching where it pays**: game code (`be*`) and the Alchemy core must match (matching is what proves
  the C is right; the mods touch exactly that code). Sony SDK and CRI stay asm for now: the port replaces
  them (SDL / OpenGL, an ADX decoder) and they add nothing to mods. They can be done last, with GCC.
- **Order by value, not by address**: player / GameRam / evolution code first, then the rest of the game,
  then Alchemy core (scene graph, IGB loading, resources), Alchemy rendering last.
- **Generators before agents** for repetitive code: the meta registration boilerplate (~7000 functions)
  went through `tools/meta_src.py` in minutes; the agent loop is for real logic.
- **Mods don't wait for the decomp**: quests are likely Lua 4.0 scripts in the AFS archives
  (`tools/luadis.py`), characters are IGB models + databases (`tools/igb.py --db`), hard limits can be
  patched now with pnach (`tools/mkpnach.py`). Data tooling (AFS repack, IGB edit, Lua recompile) can
  advance in parallel.
- **Port notes**: Alchemy had PC backends (`igVisualContext` for DX8 / OpenGL), so the port is a new
  visual context + audio + input, not a rewrite. Build 32-bit first (IGB files load straight into
  structs with 32-bit pointers, see `meta.h`). Watch for the PS2 FPU (no inf/NaN, clamps), VU0 inline
  asm, timing tied to VSync. Option to explore: a hybrid with static recompilation (N64Recomp-style)
  for functions still in asm, so the port can run before the decomp is complete.

### Next steps, in order

1. ~~Regenerate igViewer / Alchemy meta units after b054198~~ (done, 73d45ad). Refresh the progress
   table above with `tools/progress.py | tail -10`.
2. **Agent loop on the game units** (`tools/agent_loop.py run`, smallest first): ~535 KB already cut and
   pending. Priority units: `beNDMWGameRam`, `beNDMWMdlPlayer`, `beNDMWStatus`, `beNDMWLoadIntf2`,
   `beCri`, `beFont`, `beHitLandModel`. Review its `report` and add new tricks to this file.
3. **Cut the rest of the game code** (~160 KB uncut): `tools/progress.py --chunks` lists the chunks with
   `be*` names; `tools/propose_units.py START END --check` proposes cuts. Notably `text/423300`
   (`beNDMWStatus*` sub-objects) and the gaps left between units (comments in `splat.yaml`).
4. **Extend the meta generator** to retrieveVTable with constructors and the `0x80` deleting destructors
   (~150 functions per library block; needs class definitions from `meta.h` fields: string refs and
   object refs have inline ctor/dtor). Then rerun `meta_src.py` on every meta unit.
5. **Cut Alchemy core units** (scene graph, IGB, resources) around the existing `alchemy/igIGBFile` and
   `alchemy/igResource`, and feed them to the agent loop.
6. Later: split `.data`/`.rodata` per unit (needed to relink for the port), full ELF SHA, Sony/CRI.

## Tooling

| Tool | Where | Status |
|---|---|---|
| build-essential, git, python3-venv, ninja-build, binutils-mips-linux-gnu, libc6:i386 | WSL | Installed |
| splat64[mips] 0.50.1 (in `.venv`) | WSL | Installed |
| wibo 1.2.0 (decompals) | WSL `tools/wibo` | Installed, runs MWCC |
| MWCC PS2 (decomp.me compilers release) | WSL `tools/mwcps2/<ver>` | Installed (3.0b52-030722 used; others for testing) |
| EE-GCC 2.96 (decomp.me compilers release) | WSL `tools/ee-gcc2.96` | Installed |
| objdiff-cli 3.8.2 (encounter) | WSL `tools/objdiff-cli` | Installed. `diff -u src/<unit> NAME`, `report generate` |
| m2c (matt-kempster) | WSL | Pending; decomp.me has it built in |
| decomp.me | Web | In use (PS2 scratches) |

## Agent loop (unattended decompilation)

`tools/agent_loop.py` runs `claude -p` once per pending function (an `INCLUDE_ASM` in an MWCC unit,
smallest first), then checks the work itself: only the unit / `include/` (not `meta.h`) /
`symbol_addrs.txt` changed, `ninja` passes, and the `INCLUDE_ASM` is gone (commit `match: FUNC`) or
moved under `#else` of `#ifdef NON_MATCHING` (commit `nonmatching: FUNC`). Otherwise it reverts.
Never pushes. Logs in `.agent/` (not in git). Needs the full environment and a clean tree.

```
python3 tools/agent_loop.py queue [--unit U] [--max-size B]   # what's pending
python3 tools/agent_loop.py run -n 20 --max-size 512           # attempt 20 functions
python3 tools/agent_loop.py report                             # match rate by size, turns, time
```

When running inside the loop (prompt starts with "Decompile `FUNC`"): work on that function only,
follow the prompt's rules, don't commit, end with `MATCH`, `NONMATCHING <distance>` or `GIVEUP <reason>`.
A trick that fixed a function and isn't in "MWCC matching tricks" yet: mention it in the final message.

Next: run it over the game units (see "Strategy and roadmap", step 2). The meta registration
boilerplate is not for the agent: `tools/meta_src.py` generates it. Note: `be*` `_register` /
`_registerFields` live in the library's meta block (`src/libBecRuntimePlugin.cpp`,
`src/libNdmwRuntimePlugin.cpp`), apart from the class methods (`beTimer_*` `0x3A52C0`), so unit
boundaries can't come from the meta functions alone.

## Asm reading cheatsheet

- Args `a0`–`a3` (`$4`–`$7`), return `v0` (`$2`), return address `ra` (`$31`).
- Instruction after a branch/jump = delay slot, runs on both paths.
- `lw/sw` 32-bit, `lh/lhu/sh` 16-bit, `lb/lbu/sb` 8-bit → field size.
- `sll x,16` + `sra x,16` = `(short)`; `andi x,0xFFFF` = `(unsigned short)`.
- Offset gaps = padding or unused fields.
- `sq/lq` = R5900 128-bit store/load.
- `nop` pairs in short loops = likely R5900 short-loop-bug padding.
