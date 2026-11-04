# Lua script bindings

The game embeds **Lua 4.0.1** (strings: `"Lua 4.0.1"` @ `0x4EB1B8`, `LUA_ERRRUN`… @ `0x4EA9C8`,
`"value for `lua_getinfo' is not a function"` @ `0x4EB3C0`). Game logic exposes 116 commands to scripts (107 names, some overloaded).

## How it is wired

- `func_003719D0` registers the table:
  `func_0023F4F0(L, 0, func_00370780, D_004C8560, 0x74, D_0050CFC0, 0)`.
- `D_004C8560`: string pool, 0x74 pointers. Not "the name of command i": descriptors index into it,
  and some entries are method names the implementations look up (`GETCOMMON`, `GETPLAYER1`..`3`) or
  unused (`Bec`, `be`, three empty strings).
- `D_0050CFC0`: 116 variable-length descriptors, `2 + nargs` words each:
  - word 0: bits 0-7 **command id**, bits 8-12 nargs, bits 13+ name index in `D_004C8560`
  - word 1: return type, `0x200 | type`
  - one type code per argument
- `func_0023F4F0` stores the id at `+0x28` of each command object. `func_00370780` (size `0x105C`) is the
  dispatcher: `switch` on that id via `jtbl_0050D620`.
- Each case calls the implementation first, then helpers that push the result / read arguments
  (`func_0023D070` push string, `lua_pushnumber`; numbers go through the MWCC runtime first: `litodp` int, `_d_utod` bool, `fptodp` float, since Lua numbers are doubles);
  `func_00247290` with -1/-2 at the start fetches the command object). This isn't stock Lua 4.0.1:
  the method lookup (`"Can't find method %s::%s"`) points to a C++ binding layer on top.

**Implementation** = first `jal` of the case. Checked for all 116: it comes before any branch, hits a
distinct function start, and is never one of the helpers. Extra callers are other implementations only.

**Mapping fixed 2026-10-24.** The first version paired the case index with the string pool index,
which is wrong (`MDLisLive` was really `SYSgetPlayerNinzu`). The descriptor ids are right: the method
strings each implementation uses match its name (`SYSgetPlayerNinzu` -> `"NINZU"`,
`SYSgetLightMode` -> `"LIGHTMODE"`, `SYSgetGateStageName` -> `"GATESTAGENAME"`, ...).

**Overloads** (same name, different nargs) get `_<n>` in table order: `sd_0`..`sd_3`,
`DMsetSD_0`..`_3`, `DMchkSD_0`/`_1`, `DMswtSD_0`/`_1`, `MDLsetWorldPosTarget_0`/`_1`.

**Type codes** (from how the dispatcher reads/pushes them): 5 int, 7 s64 (`ld`), 9 bool (`lbu`), 11 float (`lwc1`), 13 string, 15 void (return only).

Name prefixes (guesses): `MDL` model/entity, `BLT`/`BLP` battle (target / player?), `SYS` system,
`CFL` card folder, `FLG` flags, `MAT`, `DM`. `Shinka` = evolution, `Kouka` = effect.

## Table

Regenerate with `python3 tools/dump_script_bindings.py` (`--symbols` for `symbol_addrs.txt` lines).

| id | Command | Symbol | Args | Ret | Case | Implementation |
|---|---|---|---|---|---|---|
| 0 | `SYSisJoinPlayer` | `SYSisJoinPlayer` | 5 | 9 | `003707F0` | `00371C70` |
| 1 | `SYSgetPlayerName` | `SYSgetPlayerName` | 5 | 13 | `00370818` | `00371CA0` |
| 2 | `SYSgetPlayerNinzu` | `SYSgetPlayerNinzu` | - | 5 | `00370838` | `00371D70` |
| 3 | `SYSgetWeaponName` | `SYSgetWeaponName` | 5, 5 | 13 | `00370860` | `00371CD0` |
| 4 | `SYSgetRandom` | `SYSgetRandom` | 5 | 5 | `00370888` | `00371F10` |
| 5 | `SYSgetDifficulty` | `SYSgetDifficulty` | - | 5 | `003708B0` | `00371DA0` |
| 6 | `SYSsetDifficulty` | `SYSsetDifficulty` | 5 | 15 | `003708D8` | `00371DD0` |
| 7 | `SYSgetCameraMode` | `SYSgetCameraMode` | - | 5 | `003708E8` | `00371E10` |
| 8 | `SYSgetLightMode` | `SYSgetLightMode` | - | 5 | `00370910` | `00371E70` |
| 9 | `SYSgetArea` | `SYSgetArea` | - | 5 | `00370938` | `00371ED0` |
| 10 | `SYSgetCountry` | `SYSgetCountry` | - | 5 | `00370960` | `00371EE0` |
| 11 | `SYSgetTarget` | `SYSgetTarget` | - | 5 | `00370988` | `00371EF0` |
| 12 | `SYSgetIsPackOnly` | `SYSgetIsPackOnly` | - | 5 | `003709B0` | `00371F00` |
| 13 | `SYSsetGateStage` | `SYSsetGateStage` | 13, 5 | 15 | `003709D8` | `00371F60` |
| 14 | `SYSgetGateStageName` | `SYSgetGateStageName` | - | 13 | `003709F0` | `003720F0` |
| 15 | `SYSgetGateStageNo` | `SYSgetGateStageNo` | - | 5 | `00370A10` | `00372150` |
| 16 | `SYSctlDeviceFolder` | `SYSctlDeviceFolder` | 13, 5, 13, 5, 5, 5 | 9 | `00370A38` | `003721B0` |
| 17 | `SYSctlDiskFolder` | `SYSctlDiskFolder` | 13, 5, 5, 5 | 9 | `00370A70` | `003722C0` |
| 18 | `SYSctlFlagFolder` | `SYSctlFlagFolder` | 13, 5 | 9 | `00370AA0` | `003723C0` |
| 19 | `SYSgetPlayerLevel` | `SYSgetPlayerLevel` | 5 | 5 | `00370AC8` | `00372590` |
| 20 | `SYSgetPlayerDigimon` | `SYSgetPlayerDigimon` | 5 | 5 | `00370AF0` | `003725C0` |
| 21 | `SYSisGetShinkaDigimon` | `SYSisGetShinkaDigimon` | 5, 5 | 9 | `00370B18` | `003725F0` |
| 22 | `SYSctlCardFolder` | `SYSctlCardFolder` | 13, 5 | 5 | `00370B40` | `00372640` |
| 23 | `SYSctlBit` | `SYSctlBit` | 13, 5, 5 | 5 | `00370B68` | `003724A0` |
| 24 | `SYSctlShinka` | `SYSctlShinka` | 13, 5, 5 | 5 | `00370B98` | `003727D0` |
| 25 | `SYSctlPlayer` | `SYSctlPlayer` | 13, 13, 5, 5 | 5 | `00370BC8` | `00372970` |
| 26 | `CFLclear` | `CFLclear` | - | 15 | `00370BF8` | `00372A30` |
| 27 | `CFLsetName` | `CFLsetName` | 13 | 15 | `00370C08` | `00372B50` |
| 28 | `CFLgetFolderName` | `CFLgetFolderName` | - | 13 | `00370C18` | `00372CE0` |
| 29 | `sd` | `sd_0` | 13, 13, 5, 13, 13 | 15 | `00370C38` | `00372FF0` |
| 30 | `sd` | `sd_1` | 13, 13, 5, 13 | 15 | `00370C58` | `00373060` |
| 31 | `sd` | `sd_2` | 13, 13, 5, 13, 13, 7 | 15 | `00370C78` | `003730A0` |
| 32 | `sd` | `sd_3` | 13, 13, 5, 13, 7 | 15 | `00370CA0` | `00373120` |
| 33 | `MATisPlayer` | `MATisPlayer` | - | 9 | `00370CC0` | `00373160` |
| 34 | `MATisAllPlayer` | `MATisAllPlayer` | - | 9 | `00370CE8` | `00373180` |
| 35 | `MATgetPlayerNo` | `MATgetPlayerNo` | - | 5 | `00370D10` | `003731A0` |
| 36 | `MATisRidePlayer` | `MATisRidePlayer` | 5 | 5 | `00370D38` | `003731C0` |
| 37 | `MATisRideOffPlayer` | `MATisRideOffPlayer` | 5 | 5 | `00370D60` | `003731F0` |
| 38 | `MATgetPlayerMatStat` | `MATgetPlayerMatStat` | 5 | 5 | `00370D88` | `00373220` |
| 39 | `MATsetRun` | `MATsetRun` | 9 | 15 | `00370DB0` | `00373250` |
| 40 | `DMsetSD` | `DMsetSD_0` | 13, 13, 5, 13, 13, 7 | 15 | `00370DC0` | `00373280` |
| 41 | `DMsetSD` | `DMsetSD_1` | 13, 13, 5, 13, 13 | 15 | `00370DE8` | `003732D0` |
| 42 | `DMsetSD` | `DMsetSD_2` | 13, 13, 5, 13, 7 | 15 | `00370E08` | `00373310` |
| 43 | `DMsetSD` | `DMsetSD_3` | 13, 13, 5, 13 | 15 | `00370E28` | `00373350` |
| 44 | `DMchkSD` | `DMchkSD_0` | 13, 13, 5, 13, 13, 13 | 15 | `00370E48` | `00373390` |
| 45 | `DMchkSD` | `DMchkSD_1` | 13, 13, 5, 13, 13 | 15 | `00370E70` | `003734B0` |
| 46 | `DMswtSD` | `DMswtSD_0` | 13, 13, 5, 13, 13 | 15 | `00370E90` | `00373620` |
| 47 | `DMswtSD` | `DMswtSD_1` | 13, 13, 5, 13 | 15 | `00370EB0` | `00373660` |
| 48 | `DMswtEnd` | `DMswtEnd` | - | 15 | `00370ED0` | `003736A0` |
| 49 | `DMcase` | `DMcase` | 5 | 15 | `00370EE0` | `003736C0` |
| 50 | `DMsetLua` | `DMsetLua` | 13 | 15 | `00370EF0` | `003735C0` |
| 51 | `DMmsgBox` | `DMmsgBox` | 13, 5 | 15 | `00370F00` | `003735F0` |
| 52 | `DMrun` | `DMrun` | - | 15 | `00370F18` | `003736F0` |
| 53 | `DMgetCount` | `DMgetCount` | - | 5 | `00370F28` | `00373710` |
| 54 | `FLGisOn` | `FLGisOn` | 5 | 9 | `00370F50` | `003737B0` |
| 55 | `FLGset` | `FLGset` | 5, 9 | 15 | `00370F78` | `00373840` |
| 56 | `FLGflip` | `FLGflip` | 5 | 15 | `00370F90` | `003738E0` |
| 57 | `COUNTERset` | `COUNTERset` | 5, 5 | 15 | `00370FA0` | `003739F0` |
| 58 | `COUNTERget` | `COUNTERget` | 5 | 5 | `00370FB8` | `00373AA0` |
| 59 | `MDLgetInfoName` | `MDLgetInfoName` | - | 13 | `00370FE0` | `00373B30` |
| 60 | `MDLgetRamNo` | `MDLgetRamNo` | - | 5 | `00371000` | `00373B20` |
| 61 | `MDLgetRecNo` | `MDLgetRecNo` | - | 5 | `00371028` | `003742D0` |
| 62 | `MDLgetMHp` | `MDLgetMHp` | - | 5 | `00371050` | `00373D00` |
| 63 | `MDLgetHp` | `MDLgetHp` | - | 5 | `00371078` | `00373D10` |
| 64 | `MDLgetPosStr` | `MDLgetPosStr` | 11, 11, 11 | 13 | `003710A0` | `00373B40` |
| 65 | `MDLgetRotStr` | `MDLgetRotStr` | 11, 11, 11 | 13 | `003710C8` | `00373C10` |
| 66 | `MDLaddHp` | `MDLaddHp` | 5 | 15 | `003710F0` | `00373D20` |
| 67 | `MDLsetSeq` | `MDLsetSeq` | 13 | 15 | `00371100` | `00373D50` |
| 68 | `MDLsetSD` | `MDLsetSD` | 13, 13 | 15 | `00371110` | `00373D90` |
| 69 | `MDLisLive` | `MDLisLive` | 13, 5 | 9 | `00371128` | `00373DD0` |
| 70 | `MDLgetBulletKind` | `MDLgetBulletKind` | - | 5 | `00371150` | `00373E80` |
| 71 | `MDLgetBulletName` | `MDLgetBulletName` | - | 13 | `00371178` | `00373E90` |
| 72 | `MDLgetBulletBoxNo` | `MDLgetBulletBoxNo` | - | 5 | `00371198` | `00373EE0` |
| 73 | `MDLdspDamage` | `MDLdspDamage` | 5 | 15 | `003711C0` | `00374100` |
| 74 | `MDLdspDamageP` | `MDLdspDamageP` | 5 | 15 | `003711D0` | `00374120` |
| 75 | `MDLdspExp` | `MDLdspExp` | 5 | 15 | `003711E0` | `00374140` |
| 76 | `MDLsetDMGTurn` | `MDLsetDMGTurn` | 5 | 15 | `003711F0` | `00374160` |
| 77 | `MDLisCallName` | `MDLisCallName` | 13 | 9 | `00371200` | `00374180` |
| 78 | `MDLsetWorldPosTarget` | `MDLsetWorldPosTarget_0` | 11, 11, 11 | 15 | `00371228` | `003741D0` |
| 79 | `MDLsetWorldPosTarget` | `MDLsetWorldPosTarget_1` | 13 | 15 | `00371240` | `00374200` |
| 80 | `MDLgetKyoriSquare` | `MDLgetKyoriSquare` | 11, 11, 11 | 11 | `00371250` | `00374260` |
| 81 | `MDLisSeqName` | `MDLisSeqName` | 13 | 9 | `00371280` | `003742E0` |
| 82 | `MDLisStatus` | `MDLisStatus` | 13, 5, 13 | 9 | `003712A8` | `00374410` |
| 83 | `BLTisPlayerBullet` | `BLTisPlayerBullet` | - | 9 | `003712D8` | `00374800` |
| 84 | `BLTgetShootRamNo` | `BLTgetShootRamNo` | - | 5 | `00371300` | `00374880` |
| 85 | `BLTisHitCheck` | `BLTisHitCheck` | - | 9 | `00371328` | `00374890` |
| 86 | `BLTgetDamageValue` | `BLTgetDamageValue` | 9 | 5 | `00371350` | `00374A40` |
| 87 | `BLTgetDamageRank` | `BLTgetDamageRank` | 5, 9 | 5 | `00371378` | `00374BB0` |
| 88 | `BLTisCritical` | `BLTisCritical` | - | 9 | `003713A0` | `003749A0` |
| 89 | `BLTgetStunTime` | `BLTgetStunTime` | 5, 9 | 5 | `003713C8` | `00374D00` |
| 90 | `BLTgetPoisonTime` | `BLTgetPoisonTime` | 13 | 5 | `003713F0` | `00374DE0` |
| 91 | `BLTgetSleepTime` | `BLTgetSleepTime` | - | 5 | `00371418` | `00374F00` |
| 92 | `BLTgetBindTime` | `BLTgetBindTime` | - | 5 | `00371440` | `00375010` |
| 93 | `BLTgetAttackDownTime` | `BLTgetAttackDownTime` | 13 | 5 | `00371468` | `00375120` |
| 94 | `BLTgetPowerDownTime` | `BLTgetPowerDownTime` | 13 | 5 | `00371490` | `00375240` |
| 95 | `BLTcalcSkillUp` | `BLTcalcSkillUp` | 9, 9 | 15 | `003714B8` | `00375360` |
| 96 | `BLTcalcEXPUp` | `BLTcalcEXPUp` | - | 5 | `003714D0` | `00375410` |
| 97 | `BLTgetAttributeStr` | `BLTgetAttributeStr` | - | 13 | `003714F8` | `00374CC0` |
| 98 | `BLPisHitCheck` | `BLPisHitCheck` | - | 9 | `00371518` | `003754A0` |
| 99 | `BLPgetDamageValue` | `BLPgetDamageValue` | - | 5 | `00371540` | `00375540` |
| 100 | `BLPgetAttackZokusei` | `BLPgetAttackZokusei` | - | 13 | `00371568` | `00375690` |
| 101 | `BLPgetDamageRank` | `BLPgetDamageRank` | 5, 5 | 5 | `00371588` | `00375730` |
| 102 | `BLPgetStunTime` | `BLPgetStunTime` | 5, 5 | 5 | `003715B0` | `00375830` |
| 103 | `BLPgetPoisonTime` | `BLPgetPoisonTime` | 13 | 5 | `003715D8` | `00375980` |
| 104 | `BLPgetSleepTime` | `BLPgetSleepTime` | - | 5 | `00371600` | `00375A30` |
| 105 | `BLPgetBindTime` | `BLPgetBindTime` | - | 5 | `00371628` | `00375AD0` |
| 106 | `BLPgetAttackDownTime` | `BLPgetAttackDownTime` | 13 | 5 | `00371650` | `00375B70` |
| 107 | `BLPgetPowerDownTime` | `BLPgetPowerDownTime` | 13 | 5 | `00371678` | `00375C30` |
| 108 | `BLPcalcKoukaMagic` | `BLPcalcKoukaMagic` | - | 5 | `003716A0` | `00375CF0` |
| 109 | `TXTgetCallNo` | `TXTgetCallNo` | - | 5 | `003716C8` | `003747F0` |
| 110 | `PLAYERgetPos` | `PLAYERgetPos` | - | 5 | `003716F0` | `003744A0` |
| 111 | `PLAYERgetDataStr` | `PLAYERgetDataStr` | 5, 5, 11 | 13 | `00371718` | `003744E0` |
| 112 | `PLAYERgetPosStr` | `PLAYERgetPosStr` | 5, 11, 11, 11 | 13 | `00371740` | `00374620` |
| 113 | `LISsetDifficultMax` | `LISsetDifficultMax` | 5 | 15 | `00371770` | `00375D90` |
| 114 | `MsgBox` | `MsgBox` | 13, 5 | 15 | `00371780` | `00374730` |
| 115 | `ShutDown` | `ShutDown` | - | 15 | `00371798` | `00374740` |
