# Lua 4.0.1 in the ELF

The game embeds Lua 4.0.1 (`www.lua.org/ftp/lua-4.0.1.tar.gz`) at `0x246D30`–`0x2590C0`.
All 431 functions there are named in `config/symbol_addrs.txt`. The source is in
`src/lua/` (stock 4.0.1, MIT, see `src/lua/COPYRIGHT`), one unit per file: 430
functions compile byte for byte from it with the settings below; the other 1 is
`#ifdef NON_MATCHING` C with the original asm (`INCLUDE_ASM`) in the build.

## Build settings (found 2026-10-24)

| Setting | Value | Evidence |
|---|---|---|
| Compiler | MWCC **3.0b50 / 3.0b52** | 328 vs 320 exact (3.0b38) in the same early pass; 3.0.x and 3.0.1bNN far worse |
| Optimization | `-O4,s` (size) | `-O4,p` fills fewer delay slots (`ld $ra` into a `b` slot) |
| Language | **C++**, `-Cpp_exceptions off` | Struct copies are member by member (`lw`+`sw` ttype, then `ld`+`sd` value), bool results get `andi 0xff`; tail calls exist (`lua_pushcclosure` is 8 bytes) |
| Small data | `-sdatathreshold 1` | same as `script_bind` |
| Strings | `-str readonly` | even `""` is in `.rodata`, each literal 8-aligned (not pooled: `-str pool` gets the code right but not the layout) |
| `long` in the core (`src/*.c`) | **32 bits** | `lua_State.TMtable` at 72, `TString.str` at 20, 32-bit `Instruction` |
| `long` in the libraries (`src/lib/*.c`) | 64 bits (MWCC default on EE) | `lstrlib` `Capture.len` is `ld`, stride 16 |
| `jmp_buf` | 208 bytes | `lua_longjmp.status` at 212 (`luaD_breakrun`) |
| libc | newlib | `isalnum()` etc. are macros over `_ctype_` (0x4D3D10); `stderr` is `_impure_ptr->_stderr` |
| `va_start` | MWCC builtins | `__builtin_next_arg(last) - (n >= 8 ? 0 : (8 - n) * 8)`, `n = __builtin_args_info(2)` (named args in registers) |

So the port turned `long` into `int` in the core and the headers, not in the library
`.c` files: we use `-Dlong=int` on `src/lua/*.c` (in `configure.py`), and the one header
spot that matters for the libraries is edited (`luaL_check_long` / `luaL_opt_long` in
`lauxlib.h` convert to `int`; `lstrlib`'s positions are truncated to 32 bits before
`posrelat`, while its `Capture.len` stays 64-bit). Lua numbers are `double`, done in software:
`lua_pushnumber` gets the value in one 64-bit register, and the game converts with
`fptodp` / `litodp` before pushing.

## Layout

Files follow each other in alphabetical order (Makefile link order). `ldblib.c` and
`liolib.c` are not linked. The Metrowerks linker drops unreferenced functions
(e.g. `luaA_pushobject`, `lua_equal`, `lua_newuserdata`, `parse_file`).

| File | Range | Functions | Exact | Not exact yet |
|---|---|---|---|---|
| `lapi.c` | `00246D30`–`00247EE8` | 47 | 47 |  |
| `lauxlib.c` | `00247EF0`–`00248714` | 18 | 18 |  |
| `lbaselib.c` | `00248720`–`0024A048` | 38 | 38 |  |
| `lcode.c` | `0024A050`–`0024B618` | 33 | 33 |  |
| `ldebug.c` | `0024B620`–`0024C2DC` | 21 | 21 |  |
| `ldo.c` | `0024C2E0`–`0024CDD8` | 22 | 22 |  |
| `lfunc.c` | `0024CDE0`–`0024D0CC` | 7 | 7 |  |
| `lgc.c` | `0024D0D0`–`0024DC80` | 21 | 21 |  |
| `llex.c` | `0024DC80`–`0024F734` | 13 | 13 |  |
| `lmathlib.c` | `0024F740`–`00250144` | 25 | 25 |  |
| `lmem.c` | `00250150`–`0025030C` | 2 | 2 |  |
| `lobject.c` | `00250310`–`002506CC` | 6 | 6 |  |
| `lparser.c` | `002506D0`–`00252F70` | 65 | 65 |  |
| `lstate.c` | `00252F70`–`0025326C` | 4 | 4 |  |
| `lstring.c` | `00253270`–`0025381C` | 10 | 10 |  |
| `lstrlib.c` | `00253820`–`002555B0` | 29 | 29 |  |
| `ltable.c` | `002555B0`–`00256070` | 16 | 16 |  |
| `ltm.c` | `00256070`–`0025667C` | 12 | 12 |  |
| `lundump.c` | `00256680`–`00257368` | 21 | 21 |  |
| `lvm.c` | `00257370`–`00258F84` | 18 | 17 | `luaV_gettable` |
| `lzio.c` | `00258F90`–`002590B4` | 3 | 3 |  |

The not-exact ones were named by position (same order and similar size as the
source between two exact neighbours). Left: `luaV_gettable` (reloads `hvalue(t)`
before `luaH_get`).

Port changes to the source (marked `port:`):
- `long` -> `int` in the core and headers (see above).
- `lua_State` has one more field at 0x70, and `lua_open` takes it as a second
  argument: the game's allocator object.
- `luaM_realloc` is rewritten: it calls that allocator (C++ virtual methods at
  vtable 0xD8 alloc, 0xE8 realloc, 0x108 free) when set, else the global heap
  (`func_002ED200` alloc, `func_002ED440` realloc, `func_002ED4B0` free).

`lua_dofile` is the game's own, at `0x246B30`, just before `lapi.c` (`luaB_dofile`
calls it; the `ldo.c` one is not linked). `0x246A90` next to it is probably related.

## How it was identified

1. Strings from each Lua source file found in the rom, and the functions that use
   them: gives the file order and rough boundaries.
2. Every Lua file compiled with MWCC (minimal libc stub headers), each function
   compared to every rom function in the region with relocated fields masked.
3. Exact matches as anchors (longest chain in both source and address order), then
   a DP per gap scoring bytes and agreement of `jal` targets with names already known.
4. Check: in every exact function, each call to another Lua function lands on the
   address we named it at. No contradictions.

The same pass named the libc and MWCC soft-float functions Lua calls (`memcpy`,
`sprintf`, `setjmp`, `sin`, `dpadd`, `dptoli`, ... in `symbol_addrs.txt`).
So `~0x230000` is the MWCC double runtime, not Lua, and `~0x122000`–`0x12D000` is libc.

## Data

Lua's `.rodata` (`0x4EAAB8`–`0x4EC870`, 7608 bytes) is built from source: one splat
`.rodata` subsegment per unit, in link order, which `tools/mwsplice.py` fills with the
compiled strings and tables (265 pointers relocated: library function tables, the
`luaV_execute` jump table, string tables). The only bytes still copied from the rom are
the string `luaV_gettable` uses (it is the one function still in asm). Unreferenced data
(`lua_ident`, strings of `parse_file`) was dropped by the linker, like the functions.
`double` constants loaded through `$gp` (`.lit8`) stay in the bin: the linker mixed
them across units.

## Build

- `include/libc/`: minimal libc headers (only what Lua uses, `extern "C"`).
- `tools/mwsplice.py` turns MWCC's C++ names into plain ones (`luaD_call__FP9lua_Statei`
  -> `luaD_call`), and drops compiled functions that are not in the unit (deadstripped).
- objdiff gets a copy of the object with plain names (`tools/plainnames.py`). It shows
  functions that load a `double` constant through `$gp` (`R_MIPS_LITERAL`, e.g. `math_sin`,
  `LoadHeader`) at ~99.7%: the target points at a `D_` symbol, ours at the literal section.
  The rom SHA-1 says they match.
