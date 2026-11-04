# Loading times

A stage transition takes ~16.5 s in PCSX2. EE at 300%: ~11 s. Fast CDVD: no change. So about
8 s of CPU work (IGB processing) and 8 s that depend on neither the EE nor the disc.

## What is loaded

Per transition, ~4-6 MB: the stage pack (`NDMW.AFS`, 2.9-4.9 MB), the area pack (`AREA.AFS`,
~450 KB: fonts, texts, the 12 databases, reloaded every time), the `Load` screen model (~125 KB),
player and weapon models (`_common/`), camera, lights, generator. A stage's files are contiguous.

## Loader state machine (`func_0037FA30`)

One state per frame. States 2 and 4 wait for `func_003810C0`, which processes at most one loaded
file per call after `func_00309740` says its read is done (`file->+0x30 == 0`). State 101 waits for
`SOUND` `ISDONETRANS`. State 4 also needs the loading animation (`beLoadingNode`, from the `Load`
IGB) to have played `_playLimitTime` (1.67 s); then `_stopTime = _lockTime` (4.73 s) jumps the
animation to its end (no wait).

## File reads (`igAdxFile`, CRI ADXF)

Files are read through CRI ADXF (`ADXF/PS2EE Ver.7.18`), executed by the IOP, in buffered chunks.
`igAdxFile` defaults (`igAdxFile_registerFields`): `_bUseAsync`, `_bUseAlchemyAsync`, `_bUseBuffer`
= true, `_iBufferSize` = 0x20000, `_iSectorSize` = 0x800, `_lSleepTime` = 16666666 ns (1 frame).
After each request `func_00333990` loops until `ADXF_GetStat == 3`: in async mode it sleeps
`_lSleepTime` (`igPsx2Thread` slot 0x64 = `func_0032F9A0` -> `func_0020D360(ms)`: alarm + WaitSema),
then syncs with the IOP (`func_00332CF0` -> RPC `func_00282038`). Every poll costs 16 ms even if the
disc is instant, which matches the Fast CDVD result. `ADXF_SetReqRdSct` is never called.

Experiment (`mods/`, "Loading: 1 ms read wait"): `0x3339C8` `ld a2, 0x58(s0)` -> `lui a2, 0x10`.

Result: no change (the alarm behind the sleep is timer based, so 1 ms really was 1 ms).

## Timeline (breakpoints, 2026-10-26)

- 0 -> ~3.5 s: before the loading screen (fade, previous stage teardown, file list).
- Sound transfer (`0x380640` -> `0x3806AC`): ~0 s.
- ~3.5 -> 14 s: stage files (state 2 done at `0x380818`).
- 14 -> ~16.5 s: `func.lua`, `boot.lua`, `start.lua`, texts, fade in.

Threads during loading: the loader is an Alchemy thread (entry `0x32F7F0`) at priority 127, the
main thread (24) waits on a semaphore. Samples: loader in `memset`, kernel idle thread (loader
waiting on a semaphore), loader in `func_003016F0` (ref assignment).

## memset

`memset` (`0x127250`) takes the `sq` path (32 bytes per loop) only for 16-byte aligned
destinations and otherwise goes byte by byte for the whole size. Alchemy's allocator returns
`block + 8`, so most large clears are byte by byte (seen: `0x915008`, ~200 KB).
Experiment "Loading: memset aligned to 8": `0x127258` `andi 0xF` -> `andi 0x7` and `0x12727C`
`bnez t2` -> `b` (always the `sd` path; `sq` would need 16-byte alignment).

Result: no gain (19-20 s, maybe noise, maybe the lost `sq` path). Both experiments were removed
from the pnach.

## Conclusion so far

The ~10 s file phase is not dominated by one hot spot: not the disc, not the 16 ms poll, not
memset. It is spread between IGB processing on a priority-127 thread and waits on the IOP. The
practical gain today is the EE overclock (~16.5 -> ~11 s). A real fix means loading less (the area
pack with fonts and the 12 databases, ~5000 objects, is reloaded on every transition) or a proper
profile.

## The loader in source (`src/beFileListInfoManager.cpp`)

All functions match except `beFileListInfoManager_update` (NON_MATCHING, ~94.5% exact). States
(`_proc`, one step per frame):

| State | What |
|---|---|
| 0 | `_system->_isActive = false`; stage name from the first `beFileListInfo`; start reading `loadingUSA/ld<stage minus 2 chars>.igb` |
| 1 | when it is in, add its infos (the loading animation). First time only: queue `BecText.igb` and `_common/sbiallse.igb` (`_isReadOnetime`, `_onetimeCount = 2`: those go to the object pool, the rest to the stage pool). Queue the names whose file part starts with `s`; 100 if any, else 102 |
| 100 | `readStep` until done, hand the infos over, then `SOUND SEARCH` -> 101 (`MAKEBINLIST`) or 102 |
| 101 | wait for `SOUND ISDONETRANS` |
| 102 | queue the other names -> 2 |
| 2 | `readStep` until done -> 3 |
| 3 | `func.lua`, `boot.lua` (`<stage>/lua`) |
| 4 | `readStep` over `_system->_readFileList`; wait for the loading node's minimum time, end it, move `_importantInfoList` to `_readInfoList` |
| 20, 21 | one frame each |
| 5 | hand the remaining infos to Alchemy (`func_00237030`) |
| 6 | `LAYERCTL ORDER`, `LOADING SET SWITCH`, attach each scene graph to its layer (`GETLAYERNODE`) |
| 7 | `TEXTCTL INITIALIZE`, `start.lua`; 8: `_isActive = true`; 9: `first.lua`, soft reset enabled except on `stagIS08/09/10/12` |

`readStep` is strictly serial: a file is requested only after the previous one was read and
processed, at most one per frame. Each file read is a full `igResource` load (IOP side) of a
whole IGB, then its infos are sorted (Lua runs at once; scenes to `_infoList` or
`_importantInfoList`; the rest to `_readInfoList`).

Stage file lists are short: `dng101b1` (NDMW entry 80) lists only `[0.178]/s` (222 KB) and
`[0.179]` (the 2.9 MB stage pack); names are `[partition.entry]` AFS references. So pipelining the
reads would save a frame or two. Experiment: `_iBufferSize` 128 KB -> 512 KB (`0x335014`
`lui a1, 0x2` -> `0x8`): no gain (18-19 s; not verified that the patch lands before the
registration runs). Removed from the pnach. Loading work parked; the EE overclock is the practical fix.

## Savestate analysis (2026-10-26)

PCSX2 savestates (`.p2s`, zip with `eeMemory.bin` and `PCSX2 Internal Structures.dat`; the EE
GPRs start at 0x162 there, pc at +680) taken during a load give a cheap profile.
- First round: the busy samples were in `func_00127148` (memmove) and memset, byte by byte,
  called from `func_0030C670` / `func_0030C600`: insert / remove in Alchemy's sorted string pool
  array. Mod "Loading: 4-byte memmove" (`tools/memmove_fast.s`): ~1 s less before the loading screen.
- Second round, with that mod: the CPU is idle. The loader thread (`igResource`, entry
  `func_00304E60`, kernel-saved sp found in kernel memory) sleeps in `func_00333990`, the
  igAdxFile read wait, under the IGB reader (`func_002E95E0`, `func_00309640`).
- CRI ADXF: default request is 0x200 sectors (`func_00261778`); the CRI threads (`func_00263EB8`
  woken per vsync, file thread `func_00263FC0` -> `func_0027EAE0`) move data once per frame, so
  reads advance at frame rate whatever the disc speed.
- Experiment "empujar el servidor de CRI": `0x3339B4` `beqz v0` -> `b`, the wait loop always runs
  `func_00332CD0` (CRI server, locked) instead of sleeping.
  Result: hang before the loading screen (music kept playing): the loader thread got stuck inside
  the CRI server next to CRI's own threads. Removed.
- CRI server levels (`func_0027E910`, table `D_005709E0`, registered in `func_002632C0`): 2 vsync
  (`func_0026CBD8`), 4 files (`func_00262950`: ADXF `func_00262398`, streams `func_0026A908`;
  reentrancy guard `D_0049C8C0`) run by thread `D_0049DA24` (`func_00263FC0`), 5 (`func_00277340`,
  also run by the igAdxFile wait loop), 6 main (`D_0049DA2C`, `func_002640A0`).
- Experiment "despertar al hilo de archivos de CRI": the wait loop calls `func_002643D0(D_0049DA24)`
  (CRI's wakeup) instead of sleeping, so the level 4 server runs on its own thread as often as the
  loader polls.
  Result: hang with the music stopped too: with no sleep the loop called the IOP RPC
  (`func_00282038`) nonstop and the loader blocked inside it. Second version: wake the CRI file
  thread, then sleep 1 ms (`func_0020D360(1)`), keeping the RPC rate of the stable 1 ms test.
  Result: hang again, music stopped: the loader waits forever in `func_0020D360` right after the
  wakeup. Running CRI's file thread outside its vsync cadence breaks the EE/IOP coordination.
  Removed. Further gains would need the IOP side (`CRI_ADXI.IRX`, R3000 code), not done.

## Practical result (emulator)

Since loading advances per frame (CRI's per-vsync file thread, one loader step per frame),
PCSX2's turbo / fast-forward (Tab) during the load works: ~16.5 s -> ~9 s with the
"Loading: 4-byte memmove" mod. The EE overclock (cycle rate) speeds up the CPU part on top of it.
On real hardware the frame rate is fixed; a real fix needs the IOP side or the PC port.

## IOP side (quick look)

`disc/MODULES`: `CRI_ADXI.IRX` (ADXI 9.31s281) imports libsd, vblank, threads, SIF, but no
cdvdman/ioman: it is CRI's audio driver only. `IGFILEIO.IRX` (Intrinsic Alchemy File IO, imports
cdvdman) is Alchemy's IOP reader, not used by the igAdxFile path. Disc reads go EE-side:
ADXF -> CVFS (`CVFS/PS2EE 2.37`, `cvFsAddDev` = `func_0026F728`; devices `CDV` `func_00273988`,
`HST`, `MFS`) -> dvCi (`dvCiReqRd` `func_002740A0`: one request for the whole nsct, through
`func_00271798`) -> Sony libcdvd -> standard cdvdfsv in `IOPRP280.IMG`. So the IOP is not where
the pacing lives; the EE CRI code (GCC region, 0x250000-0x2B5B68) is. `func_00282038`, called every
poll by the igAdxFile wait loop, is the RPC to CRI's audio driver: hammering it is what killed the
music in the experiments.

## Read pipeline measurements (savestates, 2026-10-26)

- igAdxFile `[0.614]` (4.8 MB stage pack) is read in 64-sector requests at ~0.5-1 MB/s, same
  with Fast CDVD. ADXF handle (igAdxFile+0x60): +0xC size in sectors, +0x14 sectors read,
  +0x1C request size. Stream: ADXF+4 (requests the whole 64 sectors).
- CRI's CD layer has one request slot `D_004A9288` (state at +2: 1 issue, 2 reading, 3 done;
  +8 lsn, +0xC nsct, +0x10 buf), request counter `D_004A92E8`, processed by `func_00272118`
  (`func_00271D50`: sceCdRead `func_00117690`, sceCdSync `func_00116C80`).
- Server run counters `D_004BB000[level]`; CRI thread loop counters `D_0049D9F0/F8/DA00`.
  Level 4 runs once per vsync, but only one CD request completes every ~6-12 runs.
- In all 6 samples the slot was "reading", 4 of them music (12-15 sectors at LSN ~0x59000)
  and data at LSN ~0x7F000: the music stream and the data alternate on the single slot.
- Test pnach "no CRI music server": `func_00277340` (ADXT server) returns at once.
  Result: no change; the small reads continue. ISO map (`dw4.iso`): AREA.AFS LSN 0xB52,
  NDMW.AFS LSN 0x3DA22. The small reads at LSN ~0x59000 are NDMW entry 58 (`soun`, 7.5 MB),
  the background music, ~55 KB/s (48 kHz stereo ADX) in 12-15 sector requests, ~2 per second;
  it is played by CRI_ADXI on the IOP, not by the EE ADXT server. Test removed.
  Data reads (entry 614) run at ~0.6-1 MB/s, about 1x DVD, and the request rate was the same
  in the run where Fast CDVD was said to be on: to be checked.
- Frame counters are comparable between runs started from the same savestate. Reading entry 614:
  ~0.75 MB/s without Fast CDVD, ~0.3-0.4 MB/s with Fast CDVD confirmed on: the emulated disc is
  not the limit; the per-request latency of the CRI chain (~10 frames per 128 KB request, single
  slot shared with the music) is. Retrying the 512 KB request size, now verifiable in a savestate
  (igAdxFile +0x40).
  The registerFields patch never took effect: the default is registered at boot, before PCSX2
  applies patches (savestate: patch word present, igAdxFile +0x40 still 0x20000). Now patched at
  the 8 use sites of +0x40 (func_00333860 allocation, func_00333D90 reads): `lw rX,0x40(..)` ->
  `lui rX,0x8`.
  Result (verified: requests of 0x100 sectors): ~0.68 MB/s, no gain. Removed.

## Summary of the read bottleneck

| Change | Faster reads? |
|---|---|
| Fast CDVD (instant disc) | no |
| 4x bigger requests | no |
| EE overclock | only the CPU part |
| PCSX2 turbo (all clocks, IOP included) | yes |

Reads run at ~0.7 MB/s whatever the disc speed or request size; only speeding up the whole
system helps. Most likely limit: the IOP (37 MHz) moving the sectors to the EE while it also
decodes the ADX music (CRI_ADX_Driver). IOP module map (savestate iopMemory, loadcore list):
cdvd_driver 0x22530, cdvd_ee_driver 0x3D430, libsd 0x4AE30, igfileio 0x50830,
CRI_ADX_Driver 0x9C630-0xA6DA0, SNDF_Driver 0xB1830. Not confirmed (IOP registers not located
in the savestate format yet).

## Stage 1: is the IOP saturated? No

PCSX2 savestate internals: EE `cpuRegs` at 0x162 (after the "cpuRegs" tag), IOP `psxRegs`
right after it at 0x5EA (GPRs, then CP0/CP2D/CP2C, pc at +520). IOP loaded modules from the
loadcore list in `iopMemory.bin` (see the map above). In 15 of 19 savestates, including every
one taken while the stage pack was being read, the IOP pc is `ThreadMan+0x46C` (0xBB9C,
`j 0xBB9C`): the IOP idle loop. The other samples are CRI_ADX_Driver around transitions.

So during the data reads the EE is idle, the IOP is idle and CRI's single CD slot is "reading":
the time goes into the emulated drive (PCSX2 models seek and rotation; the music at LSN ~0x59000
and the data at ~0x7F000 make it seek back and forth). Only the turbo, which speeds up emulated
time, helps. Fast CDVD did not, which is still unexplained. The IOP thread priority idea (CRI_ADXI
`thpri=` / `pthpri=` / `dthpri=` module args, the EE passes `sdinit=0 spucore=1` from
0x4FFF10, 19 bytes, `func_0032CF50` at 0x3329E4) is moot.

## Music stream refill threshold

CRI streams: 0x60-byte objects from `D_004A7F10` (0x28 of them, server `func_0026A908` ->
`func_0026A728` -> `func_0026A330`). The BGM stream (NDMW entry 58) at 0x4A7F10 has buffer
0x27800 (+0x18/+0x40) and threshold +0x1C = 0x21800. `func_0026A330` issues a read when
`+0x40 - sj free < +0x1C`, i.e. as soon as 24 KB (~0.4 s of music) were consumed: 12-19 sector reads,
each a seek away from the data being loaded. `func_0026A9D8` is ADXSTM_SetBufSize(stm, threshold,
max), called from the ADXT start / reload setters (ADXT +0x3E threshold, +0x3C max, in sectors).
Mod "Loading: music requests bigger blocks": threshold = max / 4 for every audio stream (applies when a
stream starts).
GameCube note: the GC version loads faster.
  First version (in SetBufSize) only reached streams started after the patch: in the usual
  transition the BGM keeps playing from before, threshold still 0x21800, no gain. In another
  transition where the music restarted, data reads went from ~0.7 to >=1.4 MB/s and the music was
  fine. Now patched at the compare in `func_0026A330` (0x26A578 `lw a0,0x1C(s3)` ->
  `srl a0,v1,2`), which covers running streams too.
  Result (usual transition, BGM running): music requests of 59-60 sectors, data reads ~1.1 MB/s
  (was ~0.75), music fine. Retrying the 512 KB data requests on top of it.

## Music stream buffer size (igAdxAudio)

`igAdxAudio` (`_pWorkArea` +0x8, `_iWorkSize` +0xC) wraps one ADXT handle. Its init
`func_00331FB0(this, maxCh +0x14, type +0x18, maxStreams +0x1C, maxFreq +0x20)` computes
`stream = round_up_2048((maxStreams + 1) * (maxFreq / 1000 * maxCh * 25000 / 44) + 0x800)` and
`_iWorkSize = 0x40C0 * maxCh + stream + 0x24 + 0x40`, allocates it from the object's pool
(`func_002ED2B0(size, 16, pool)`), and `func_003320D0` passes it to ADXT_Create
(`func_0026B348(maxCh, work, size)`). With 2 channels, 48 kHz and maxStreams 2 that is 0x28000,
the 0x27800 stream buffer seen at 0x4A7F10.
Mod "Loading: music buffer x4": `0x332030` `sra a0,a0,3` -> `sra a0,a0,1` (/44 -> /11). Applies to every
igAdxAudio created after the patch (check the BGM stream's +0x18 in a savestate: 0x27800 -> ~0x9F800).

**Result (2026-10-31): with "Loading: music requests bigger blocks" + "Loading: music buffer x4" only (no
memmove mod, no overclock, no turbo, normal speed), a full stage transition takes ~9 s (was ~16.5 s),
with the same BGM continuing and with a different one. Music and sound effects fine.** So the
loading time was mostly the music stream taking the single CRI CD slot (a seek away from the data)
every ~0.4 s. Next: x8, and adding the memmove mod (CPU side).
x8 (`sra a0,a0,0`): still ~9 s, no further gain from the music side.
Adding "Loading: 4-byte memmove" on top: 8-9 s depending on the transition, barely noticeable.

## Per-file timing (2026-10-28, PCSX2 breakpoints, stag0101 load)

- `igIGBFile_readFile` (`0x2E95E0`) runs ~11 times per load. A breakpoint at `0x2E9614` (after its
  `sprintf`) shows the file name in the `readFile(%s)` buffer at `0x586E20`.
- One file takes ~9 s, the others 1 s or less. It is almost surely `[0.614]` (NDMW.AFS entry 614,
  the 4.79 MB stage pack); the 5th file is `stag0101/separate/generatorN.igb`. Not yet confirmed
  through the name buffer.
- EE cycle rate 300%: that file drops to ~5 s. So roughly 6 s CPU (IGB processing) + 3 s I/O or
  waits (stopwatch precision). Consistent with the generic -> PS2 conversion above.
- The engine has a `Total Time to load %s - %f seconds` message (near `0x4F9B90`), disabled in
  the release build; re-enabling it would give exact per-file times.
