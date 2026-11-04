# MWCC matching tricks and tools

Found while matching this game's code with MWCC 3.0b52.

- Global pointer read once in the asm but MWCC reloads after each store → copy it to a local first.
- `a->b->c[i]` vs `t = a->b; t->c[i]`, and `if` branch order, change delay slots and registers.
- A value calculated as a bool (`slt` then `bnez`) instead of a direct `bgtz`: write `(x > 0) == false`.
- Code repeated on both sides of an `if` → a C++ `inline` helper called twice. MWCC stops inlining
  at about 3 nested levels (seen with `SetStr` → `AssignStr` → `GetStrPool`).
- Indirect call via `$t9` (`lw $25, off($25)`; `jalr $25`) → C++ virtual call; vtable has an 8-byte header.
- `(double)x` emits a call to `fptodp`. Magic `0x68DB8BAD` multiply = `/ 10000`.
- Several floats loaded before any store → pass them through an inline helper's parameters.
  Same for an argument loaded into an s-register before an earlier call (`lw s1,0(s1)` before
  `jal`): MWCC defers `*p` past the call, a local doesn't help, an inline's parameter does
  (`SetUnicode8` in `beNDMWGameRam`). An inline returning a class by value is not expanded.
- Method returning a struct by value: result pointer in `a0`, `this` in `a1`.
- `%gp_rel` loads of 0/1 bytes = `const bool&` temporaries (e.g. `Set(i, true)`); mwsplice maps
  them to the original `.sdata` slots.
- `new T()` with a cleanup block (`operator delete` + `__throw` after an unreachable `b`): class
  `static operator new(size_t)` / `operator delete(void*, size_t)` inline around a global heap, and a
  real C++ constructor (named with MWCC mangling in `symbol_addrs.txt`, e.g. `__ct__3BecFv`).
- A bool result widened to an unsigned int before conversion: `(double)(u32)b` gives the `andi`.
- A by-value `igVec3f` copied by the caller at every call site (to a fresh stack slot): it is a class
  with a user copy constructor (`struct Vec3 : igVec3f` in `src/beTargetObj.cpp`). Without one, MWCC passes a pointer and doesn't copy.
- `c.le.s best, d` + `bc1t skip` = `if (!(best <= d))`; `sltu`+`xori 1`+branch on a bool = an inline
  returning `!(x != 0)`, tested with `!` (`!IsFree(p)` in `beTimer`).
- `andi v,x,0xFF` before testing a bool local or a bool return value: write `x == false`.
- A count reloaded for the increment after the compare (`lw; addiu; sw`, often with an empty delay
  slot): `(*(volatile s32*)&list->_count)++`. With `count = n + 1` there is no reload.
  For a plain reload in a later test, `*(volatile s32*)&` may compute the address apart
  (`addiu v1,a0,8`); `((volatile T*)list)->_count` keeps `lw v0,8(a0)` (`beNDMWGameRam_initData`).
- Inline parameter order sets register priority: when an inline's object and list come out in
  swapped s-registers, put the object parameter first (`ObjListSetObj(o, list, i)`).
- `igResult` (alchemy.h) has a user copy constructor and an int `operator=`: functions return it by
  value (hidden result pointer in `a0`) and the copy goes through an int register, not `$f0`. Compare
  a call result directly (`f(x) == D_004FB418`): the temporary is read through its address.
- `sltu`+`xori 1`+`beqz` on an `isOfType` result: an inline returning `!(f(o, meta) != 0)` over a
  C++-linkage overload of `func_00300CD0` that returns `s32` (mwsplice demangles it back).
- A field read straight vs through a one-line inline getter changes register choice (`GetRefList(file)` in
  `func_002E79A0`, found by the permuter). Locals reused across similar blocks: a separate pair for
  the first block can fix an s-register swap (`pool0`/`f0` in `igResource`).
- `func_0030C940(owner, header)` frees a pooled string: 2 arguments, which is why the header sits in `a1`.
- Smart pointers (`IG_REF(T)` in `include/alchemy.h`) live on the stack (destructor + exceptions), so
  every use reloads them. Out-of-line destructors (`__dt__<n>TRefFv`, deleting form; `<n>` is the length of the class name) appear after the
  first function that needs them; a type whose destructor is already in an earlier unit gets none
  (the linker keeps one copy). Name them in `symbol_addrs.txt` with the mangled name.
- Mixing `r->x` (operator->) and `r.p->x` on the same smart pointer makes MWCC reload the field.
- Register allocation follows declaration order: when the s-registers come out permuted, move the
  declarations (e.g. to the top of the function).
- An inline taking a derived list type through a base pointer can permute registers: a template helper
  (`template <class L> f(L* list)`) avoids the conversion.
- `x[n - 1]` folded into `-4(v)` while the original computes `(n - 1) * 4`: use a local `s32 last = n - 1`.
- `addu v0, product, x` vs `addu v0, x, product`: compute the product into its own local first
  (`base = n * 12; return base + x;`, `func_00419680`). A loop bound `start + n` in its own local
  (`end`) swaps the registers of the bound and the offset (`func_0041D3B0`).
- An argument read before the others (`lw t0, ramNo` in a branch delay slot ahead of a computed
  name): read it into a local before the call (`beNumberCtrl_entry`).
- `(s64)f` calls `__fixsfdi` (`0x230110`, MWCC runtime). The float temp lands in `$f12` (the
  call's argument) only when written as a local mutated in place: `f = (f32)(u32)x; f *= k;`.
- `sqrt.s` (splat prints `c1 0x4`): `extern "C" float sqrtf(float);`, MWCC inlines it. A kept
  `+ 0.0f` (`add.s` with `mtc1 zero`) comes from an inline `Len2(x, 0.0f)` (`bePadData_userInstantiate`).
- `if (--t == 0) {A} else {B}` comes out with the blocks swapped: write `if (--t != 0) {B} else {A}`.
- 64-bit `/` calls `__divdi3` (`0x22E4B8`); `x * 1000000000` shows up as a long shift/add chain.
- Stores through a smart pointer reload it per field; one reload per igVec3f means an inline
  `SetVec3(igVec3f& v, x, y, z)` (`func_003D22F0`). Separate loops over the same kind of index
  each with their own variable (`i`, `j`, `k`) can fix swapped index/offset registers.
  The last loop in its own `{ }` block with its own index/base/value locals fixed both
  `base2Current` and `current2Base`. Two scalars as one `s32 dv[2]` array changes their
  register priority (permuter find, `current2Base`) when declaration order doesn't.
- A pointer that walks a table (`addiu s0,s0,12` per iteration) sitting in the *lowest* s-register,
  below inline locals: the source indexed it (`desc[k]`, `k++`); MWCC strength-reduces that into a
  pointer temporary of its own, colored last (`beNDMWGameRam_buildData`). `desc++` gives it a high one.
- A string address kept in an s-register across repeated calls (`lui s6`/`addiu s6` once, then
  `move a3,s6`) where the original rematerializes it per call: the argument went through an `(s32)`
  cast; declare the parameter `const char*` (a C++ overload of the prototype if needed).
- mwsplice "@NNN placed at A and B": a string literal the original keeps twice (one copy per
  function); use the `D_` symbol of that copy in one of the functions.
- A pointer reached through a cast (`((T*)p)->field` in a loop) gets its field addresses hoisted out of
  the loop; type the variable (or struct member) as `T*` instead (287 -> 10 off in `func_003DCC60`).
- A table base kept in an s-register next to a walking pointer (`lui s3` + `move s1,s3`, used again
  after the loop): the source had a local `s32* tbl = D_xxx;`.
- A value stored after a call (`f(x)->field = K`) with K loaded before the call into an s-register:
  write `T* o = f(x); o->field = K;`.
- Extra argument registers kept free (`lw a2,..` where `a1` would do): the callee takes one more
  parameter than declared (`func_003D8740(self, ram)`).
- A pad flag built with branches (`bnez c1; daddiu t,1` ... `move t,zero`, then `andi up,t,0xff`)
  while the next flags use `sltu`: a bool temporary,
  `bool u = true; if ((stick & 1) == 0 && ((trig | rep) & 0x10) == 0) u = false; up = u;`, and the
  declaration order `trig, up, rep, down` for the a-registers (`func_003DDB80`).
- A `nop` before a call that is a branch target, after `if (x >= n) x = m;`: write the value as a
  ternary (`s32 next = a + 1 >= n ? m : a + 1;`, `func_003DE480`).
- Compare-and-branch through `$at` (`slti at,v0,10000`; `lui at`/`ori at`/`slt at` for big constants)
  comes from `>` / `<` (`x > 9999`, `X <= level`); `x >= 10000` computes into `v0` and shares the `lui`
  with a nearby constant (`recalcPlayer`).
- Table walks whose rows advance by a constant (`+20` per i, `-4` per r) are a 2D array
  (`D_004CF2D0[i][r]`, `extern char* D_004CF2D0[][5]`), not `t[i * 5 + r]`; the extra pointers in
  s-registers also push other locals to the stack, which is where a 16-byte frame difference came from.
- Address-taken locals (`(u32*)&x` arguments, smart pointers) sit in the frame in declaration order,
  including block-local ones: a smart pointer in the middle of the stat slots means the later stats are
  declared in a later block.
- Spilled locals ordered differently with no declaration order fixing it: look at how each is read (a
  value read through `IntListGet` vs indexed directly swapped two spill slots in `recalcPlayer`).
- `ListReserve(list, list->_count, 4)` where the original reloads `_count` for the compare: MWCC folds
  `n < n`; pass the list through another pointer and read the count volatile.
- `andi v,x,0xFF` + `bnez` on a bool local: `if (x == false)` / `x == false &&` (inverted logic).
- Big functions: write them whole, then fix the diff top-down; a branch-target offset that runs
  through the whole function usually comes from one block of the wrong length.
- Ref-counted pointer fields: `igIncRef(new); igUnref(field); field = new;` (a `RefSet` template).
- `c.lt.s` + `bc1t call` + `b return 0`: write the negated early return,
  `if (!(a < b)) return 0; f(); return 1;` (`func_003998F0`).
- `slt v0` instead of `slt at` for an early `return NULL`: flip the comparison (`n <= x` vs `x >= n`).
- A result struct compared with `==` (`lw` through the temporary's address, constant loaded late): give
  the struct an inline `operator==` and compare against a global of that struct type.
- Message handlers (`handle`, vtable `+0x90`): a `switch` on the message index, even with one case.
- Checked casts: `IG_DYNAMIC_CAST(T)` (alchemy.h); a version taking the meta as a parameter loads it once.
- Inline nesting: past ~3 levels MWCC emits an out-of-line copy (`GetStrPool: no address` in fcmp);
  flatten the helper (`PoolStr` inside `AppendStr` was one level too many).
- An inline that takes the list as a parameter reads it before its body; when the original builds the
  pooled string first, use a macro (`APPEND_STR`).
- Call arguments are evaluated right to left: `AppendShort(list, f(x) - y)` makes the call before
  reading `list` (`func_002E63E0`); with the value first, `list` is loaded (and kept in an s-reg) before the call.
- A sum that adds in the "wrong" operand order may be unsigned (`*sum += (u32)x`; `(u32)f->_offset + f->_size`).
- Spilled locals get stack slots in declaration order (descending addresses), then inline locals,
  then compiler temporaries (strides hoisted out of loops). So `desc += n` on an `s16*` (stride
  left to the compiler) and `desc = (char*)desc + size` with a declared `size` spill differently.
- A later block reusing an earlier local (`j`, `desc`) instead of declaring a new one changes which
  value gets an s-register and which gets spilled.
- `((s16*)data)[i]` with a hoisted `i*2` gives `addu off,data`; `*(s16*)((char*)data + i * 2)` gives `addu data,off`.
- **A function compiled with `#pragma inline_depth(1)`** (seen in `beNDMWGameRam_handle`): helpers it
  calls are expanded, what *they* call is not, so one-line accessors (smart pointer / string getters,
  `GetStrPool`, incref) show up as calls, and their out-of-line copies sit right after the function
  **in reverse order of first use**. Copies an earlier unit also needed are missing (the linker kept
  that unit's). Reproduce it with real `inline` accessors called only from level-1 helpers, and RAII
  local refs (they live on the stack). A level-1 call passed straight into another level-1 call that
  takes a reference may not be expanded: hoist it to a local.
- `switch` cases are tested in reverse source order; a one-case `switch` gives `beq` + `b default`.
- Case bodies are laid out in source order (m2c prints them that way), and a `char buf[N]` declared in
  each case block gets its own stack slot (`beNDMWStatus_handle`).
- `slt at,v0,zero` + `beqz` (where `x < 0` gives `bgez`): write `0 > x` (`func_003DEF80`).
- A string literal inside an `inline` function is named `@STRING@<mangled func>`, and two of them
  collide once plainnames demangles ("target of more than one redefinition"): use the `D_` symbol there.
- Still open: a 5-byte / 2-byte copy of "----" / "0" as `lwc1`+`lbu` / `lh` with both addresses in
  registers (looks like an expanded `strcpy`; `strcpy`/`memcpy` are never expanded with our flags,
  and `__strcpy` exists but takes `(char, char)`). Closest: a `{ f32; u8 }` struct copy through
  pointer locals in an inline (`StatusCopy5` in `beNDMWStatus_handle`).
- Values shared by several `case`s declared once at function level (C89 style) change the whole
  register allocation (priority = weighted uses, loops weigh more).
- Tool: `tools/fcov.py` (coverage of a big work-in-progress function, aligned view; `-g` every
  mismatch, `-S` stack slot map original -> ours, to find locals laid out in another order).
  `tools/nm_build.py src/UNIT.cpp [FUNC..]` compiles a unit with `-DNON_MATCHING` (to
  `build/nm_UNIT.o`, or prints fcmp's diff for the FUNCs).
- **Virtual slots** (index after the 8-byte header; offset = 8 + 4*slot), listed in `VIRTUAL_SLOTS` in `tools/dump_meta.py`. igObject: 0
  `__bootstrapDestruct`, 1 `userAllocateFields`, 2 `userDeallocateFields`, 3 `userCopyDeep`, 4
  `userCopyShallow`, 5/6 `userIsAlike*`, 7 `userInstantiate`, 8 `userRelease`, 11 `userResetFields`,
  16 `clean`, 19 `createCopy`, 20 `getMeta`. igDataList 21 `getElementSize`, igObjectList 22
  `getElementType`, igInfo 21 `attemptResolve` 22 `resolve`, igInfoManager 21-27
  `registerToInsight` `unRegisterFromInsight` `begin` `end` `appendInfo` `removeAllInfos` `update`,
  igNode 31 `addParent`, igGroup 33 `insertChild` 34 `isChildrenOrdered`; the game's
  beBaseInfoManager 28-34 `onAddInfo` `entry` `getInfoMeta` `updateRam` `prepare` `unhandled`
  `handle` (34 = each manager's message handler).
  `tools/dump_meta.py --symbols` names every implementation `<owner>_<method>` (owner = closest
  common ancestor of the classes using it) and renames stale `func_` references in `src/`.
  Then `configure.py --clean`. If a configure fails, `asm/` is gone: do not run dump_meta then.
- Tools: `tools/m2c_func.py FUNC` (m2c with jump tables and strings), `tools/fdist.py OBJ FUNC -v`
  (aligned instruction diff), `tools/permute_decls.py src/UNIT.cpp FUNC` (tries every order of the
  locals marked `/*P*/`; fixes most s-register permutations).
- Mods in C++: `tools/mkpnach.py mods/src/FILE.cpp func_XXXXXXXX MAX_SIZE` compiles with MWCC, links at
  the function's address (game symbols from `symbol_addrs.txt` / `func_`/`D_` names) and prints pnach lines.
  Mods so far: `mods/src/chip_pickup.cpp` (`docs/death.md`), `mods/src/shared_level.cpp` (`docs/evolution.md`).
- Asm search: `tools/xref.py SYM.. [--dump]` (functions referencing a symbol, optionally their asm),
  `tools/xref.py --re REGEX` (matching instructions with their function), `tools/asm_range.py START END`.
- Progress and cutting: `tools/progress.py [--chunks]` (per unit + per region over all `.text`; chunks
  with their `be*`/`ig*` names), `tools/propose_units.py START END --check` (unit cuts at class changes),
  `tools/meta_blocks.py` (runs of meta boilerplate in the chunks, ready as splat lines).
- Meta boilerplate: `tools/meta_src.py src/UNIT.cpp.. --check` (see "Alchemy reflection data"). Safe to
  rerun: it regenerates from the asm, keeps macros whose asm is missing, and reverts what doesn't match.
- Trivial functions (empty, `return N`, `*_Meta` getters, `*p = 0; return p`): `tools/trivial_src.py
  src/UNIT.cpp.. --check` writes them as plain C and reverts any that differ.
- Moving asm between machines: `tools/asm_bundle.py export FUNC.. | --pending --unit U -o FILE`
  (machine with the ELF) / `import FILE` (machine without it: writes asm, m2c drafts and a sparse target rom).
- Data: `tools/igb.py PACK --db [NAME] [--csv DIR]` (databases), `tools/shinka_stats.py LEVEL` (base stats
  of the 16 forms; `docs/gameram.md` for the formula and the equipment added on top).
- Models (`docs/models.md`): `tools/afs_names.py [--dir D]` (real paths of AFS entries, from
  `disc/NDMW.IGB`), `tools/igbtex.py export|import` (IGB textures <-> PNG, in place),
  `tools/modiso.py IN.iso OUT.iso NDMW.AFS:ENTRY=FILE` (patch AFS entries in a copy of the ISO).
- decomp.me context: `tools/decompme_ctx.py` writes `build/decompme_ctx.cpp` (`game.h` preprocessed
  with MWCC: every `meta.h` class, `s32`.., `NULL`, `beMessenger_*`), checked to match on real functions.
  For one `NON_MATCHING` function with all the unit's helpers: `tools/decompme_func.py src/UNIT.cpp
  FUNC` writes `build/decompme_FUNC.cpp` (the unit preprocessed up to that function; no Context needed).
- **decomp-permuter** (`tools/decomp-permuter`, not in git): `tools/perm_setup.py src/UNIT.cpp FUNC` builds
  `permuter/FUNC/` from the function's `NON_MATCHING` C (the rest of the unit, MWCC-preprocessed, goes in
  `PERM_IGNORE`; flattened `meta.h` structs in `PERM_PRETEND`; scoring on that function only). Run with
  `cd tools/decomp-permuter && PATH=$HOME/dw4/tools/perm-bin:$PATH ../../.venv/bin/python permuter.py
  ../../permuter/FUNC -j10 --stop-on-zero --best-only` (`tools/perm-bin/cpp` stands in for cpp).
  It parses C (pycparser): a function with C++ constructor-style locals (`TRef o(f(x), IG_ADOPT);`)
  fails with "Syntax error in base.c" and, with `--best-only` piped through `tail`, looks like a
  run that found nothing. Check that it prints `base score` before trusting a silent run.

