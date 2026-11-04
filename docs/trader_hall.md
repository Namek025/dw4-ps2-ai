# Trader Hall (item trades with NPCs)

Found 2026-11-02. A late-game area where the four starters, a DigiElf and a few other NPCs trade
items. Stage packs `NDMW.AFS` 534, 536, 538 (unnamed; by their position probably `dngq5b1e`,
`dngq5b1h`, `dngq5b1v`, one per difficulty). The three packs carry the same trade tables.

## NPCs (stage `main`, `MODELCTRL ... GENERATE` with position / rotation)

3x `n_agumon`, 3x `n_girumon` (Guilmon), 3x `n_dorumon`, 3x `n_vmon` (Veemon), 2x `n_digilff`
(female DigiElf), 2x `n_prmame` (PrinceMamemon), `n_weregaru`, `n_bwargrey`. They use the NPC
models in the stage pack (64x64 texture, see `docs/models.md`), standing animations only.

## How a trade works

Each NPC script (`n_<name>_00h/n/v.lua`, `_01h/n/v.lua`, `_02.lua`: the `h/n/v` suffix is likely the
difficulty) checks a priority list from top to bottom and offers the **first** item you own:

    if SYSctlFlagFolder("IS", 11) == 1 then ... g_SItemDB = "impItem"; g_SItemID = 11; ...   -- key item
    if SYSctlDeviceFolder("IS", 0, "armor2", 24, 0, 0) == 1 then ... g_SItemDB = "armor2"; ...

`g_SItemDB/ID` = the item you give, `g_TItemDB/ID` = the one you get; on "yes" it runs
`itemchange<N>.lua` (`DMsetLua`), which does the swap. Item ids are 0-based rows of the
`weapon`, `armor1`, `armor2`, `impItem` databases (`tools/igb.py <AREA pack> --db weapon`), every
weapon comes in 5 consecutive rows, levels I-V (the name ends in a level glyph). So the "specific
order" is the list order: to get an item further down, you must not carry the items above it.

Dump: `python3 tools/luadis.py <pack>` (Lua 4.0 chunks inside the IGB). Which chunk is which NPC
instance is not mapped yet (the speaker name comes from `funcOpenMessage("talk", NPC, ...)`).

## Trade tables (pack 534)

### 1. `n_agumon` (`itemchange97.lua`)

| # | Das | Recibís |
|---|---|---|
| 1 | Offering | Beam Magnum II |
| 2 | Beast Board I | Slasher I |
| 3 | Armor Board I | Hook Katar I |
| 4 | Brave Core II | Blaster Gun III |
| 5 | Atlas Board I | Dbl Axe III |
| 6 | Resist Board I | Denji Prince III |
| 7 | Battle Hawk II | Dyno Tooth I |
| 8 | Dyno Tooth II | Kamikiri Maru I |
| 9 | Kamikiri Maru II | Grand Blade I |
| 10 | Battle Hawk III | Achilles Board II |
| 11 | Dyno Tooth III | BIT Board I |

### 2. `n_agumon` (`itemchange97.lua`)

| # | Das | Recibís |
|---|---|---|
| 1 | Offering | Beam Magnum I |
| 2 | Mighty Talon I | Battle Hawk III |
| 3 | Battle Hawk I | Long Sword III |
| 4 | Kamikiri Maru I | Sword Break I |
| 5 | Apocalypse I | War Sword V |
| 6 | Mighty Talon III | Dyno Tooth III |
| 7 | Divine Core I | Ankokuten V |
| 8 | Dbl Axe III | Brave Core II |
| 9 | Bone Blade III | War Sword V |
| 10 | Mighty Talon IV | Soul Burn V |

### 3. `n_dorumon` (`itemchange40.lua`)

| # | Das | Recibís |
|---|---|---|
| 1 | Offering | Rune Blade I |
| 2 | Bolt Board I | Soul Burn I |
| 3 | Crusher I | Beam Magnum III |
| 4 | Victor Sword I | Beam Magnum I |
| 5 | Denji Prince I | Bone Blade II |
| 6 | Beam Magnum II | Fang Smasher II |
| 7 | Kamikiri Maru II | Mana Board I |
| 8 | Dyno Tooth II | Battle Hawk IV |
| 9 | Kamikiri Maru IV | Ankokuten I |

### 4. `n_dorumon` (`itemchange40.lua`)

| # | Das | Recibís |
|---|---|---|
| 1 | Offering | Dyno Tooth II |
| 2 | Beast Board I | Dyno Tooth III |
| 3 | Dark Board I | Pain Scimitar I |
| 4 | Achilles Board I | Van Blade II |
| 5 | EXP Board I | Blaster Gun III |
| 6 | Long Sword II | Ice Board I |
| 7 | Kamikiri Maru II | Cipher Core II |
| 8 | Mighty Talon II | Fire Board I |
| 9 | Dyno Tooth I | Long Sword III |
| 10 | Victor Sword III | Achilles Board II |
| 11 | Beam Magnum III | Mana Board I |

### 5. `n_dorumon` (`itemchange40.lua`)

| # | Das | Recibís |
|---|---|---|
| 1 | Offering | Long Sword I |
| 2 | Mighty Talon I | Victor Sword I |
| 3 | Dyno Tooth I | Sword Break I |
| 4 | Beam Magnum I | Hekatoncheir I |
| 5 | Crusher I | Bone Blade IV |
| 6 | Grand Blade III | War Sword V |
| 7 | Beam Magnum III | War Sword V |
| 8 | Battle Hawk IV | Dbl Rapier IV |
| 9 | Element Core II | Onslaught V |
| 10 | Long Sword V | Mana Board III |

### 6. `n_girumon` (`itemchange21.lua`)

| # | Das | Recibís |
|---|---|---|
| 1 | Offering | Victor Sword I |
| 2 | Crusher I | Mighty Talon I |
| 3 | Rune Blade I | Grand Blade III |
| 4 | Tri-edge I | Battle Hawk II |
| 5 | Fire Board I | Spirit Core I |
| 6 | Mighty Talon II | Armor Board I |
| 7 | Dbl Rapier II | Dark Crimson II |
| 8 | Dyno Tooth II | BIT Board I |
| 9 | Long Sword IV | Dragoon I |

### 7. `n_girumon` (`itemchange21.lua`)

| # | Das | Recibís |
|---|---|---|
| 1 | Offering | Long Sword II |
| 2 | Fire Board I | Battle Hawk I |
| 3 | Meteor Board I | Battle Hawk III |
| 4 | Atlas Board I | Red Moon II |
| 5 | Battle Hawk II | Dbl Axe I |
| 6 | Kamikiri Maru II | Dark Board I |
| 7 | Beam Magnum II | Bolt Board I |
| 8 | Kamikiri Maru I | Bone Blade III |
| 9 | Grand Blade III | EXP Board I |
| 10 | Mighty Talon III | Rack Board I |
| 11 | D Slayer III | Rune Blade III |

### 8. `n_girumon` (`itemchange21.lua`)

| # | Das | Recibís |
|---|---|---|
| 1 | Offering | Battle Hawk I |
| 2 | Long Sword I | Sword Break I |
| 3 | Dyno Tooth I | Rune Blade I |
| 4 | Kamikiri Maru I | Crusher I |
| 5 | Apocalypse I | Kamikiri Maru IV |
| 6 | Cipher Core II | Drill Bunker V |
| 7 | Mighty Talon III | War Sword V |
| 8 | Battle Hawk III | War Sword V |
| 9 | Bone Blade III | Dbl Axe IV |
| 10 | Dbl Rapier IV | Long Sword V |

### 9. `n_girumon` (`itemchange78.lua`)

| # | Das | Recibís |
|---|---|---|
| 1 | Offering | Hekatoncheir I |
| 2 | Crusher I | Beam Magnum II |
| 3 | Victor Sword I | Mighty Talon II |
| 4 | Rune Blade I | Dbl Rapier III |
| 5 | Bone Blade II | Achilles Board I |
| 6 | Kamikiri Maru II | Aegis Core I |
| 7 | Long Sword II | Strider II |
| 8 | Battle Hawk IV | Soul Burn I |
| 9 | Dyno Tooth IV | Onslaught I |

### 10. `n_girumon` (`itemchange78.lua`)

| # | Das | Recibís |
|---|---|---|
| 1 | Offering | Mighty Talon II |
| 2 | Bolt Board I | Sword Cannon I |
| 3 | Ice Board I | Kamikiri Maru III |
| 4 | Fire Board I | Mighty Talon III |
| 5 | Meteor Board I | Rage Hammer I |
| 6 | Mana Board I | D Slayer III |
| 7 | Long Sword II | Bone Blade I |
| 8 | Dyno Tooth II | Atlas Board I |
| 9 | Beam Magnum II | Achilles Board I |
| 10 | Dbl Rapier III | Resist Board I |
| 11 | Blaster Gun III | Resist Board I |

### 11. `n_girumon` (`itemchange78.lua`)

| # | Das | Recibís |
|---|---|---|
| 1 | Offering | Kamikiri Maru I |
| 2 | Battle Hawk I | Beam Magnum III |
| 3 | Long Sword I | Dbl Axe III |
| 4 | Beam Magnum I | Sword Break I |
| 5 | Tri-edge I | Beam Magnum IV |
| 6 | Rune Blade I | War Sword V |
| 7 | Grand Blade III | Element Core II |
| 8 | Kamikiri Maru III | War Sword V |
| 9 | Dyno Tooth IV | Photon Edge V |
| 10 | Dbl Axe IV | Long Sword V |

### 12. `n_prmame` (`itemchange116.lua`)

| # | Das | Recibís |
|---|---|---|
| 1 | Rune Blade I | Apocalypse I |
| 2 | Hekatoncheir I | Dbl Axe II |
| 3 | Tri-edge I | Battle Hawk III |
| 4 | Beam Magnum II | D Slayer I |
| 5 | Sword Break I | Ice Board I |
| 6 | Long Sword II | Dyno Tooth IV |
| 7 | Aegis Core I | Dragoon I |
| 8 | Soul Burn I | Meteor Force I |
| 9 | Dragoon I | Photon Edge I |

### 13. `n_prmame` (`itemchange116.lua`)

| # | Das | Recibís |
|---|---|---|
| 1 | Mighty Talon I | Kamikiri Maru III |
| 2 | Kamikiri Maru I | Sword Break I |
| 3 | Beam Magnum I | Dbl Rapier III |
| 4 | Crusher I | War Sword V |
| 5 | Victor Sword I | Cipher Core II |
| 6 | Battle Hawk III | Vulture Core I |
| 7 | Battle Hawk IV | War Sword V |
| 8 | Kamikiri Maru IV | Long Sword V |
| 9 | Grand Blade IV | Omega Raptor V |

### 14. `n_prmame` (`itemchange173.lua`)

| # | Das | Recibís |
|---|---|---|
| 1 | Long Sword IV | Meteor Force V |
| 2 | Brave Core II | Infinity V |
| 3 | Dragoon V | Dbl Rapier V |
| 4 | Long Sword V | EXP Board III |

### 15. `n_vmon` (`itemchange59.lua`)

| # | Das | Recibís |
|---|---|---|
| 1 | Offering | Denji Prince I |
| 2 | Beast Board I | Astral Core I |
| 3 | Rune Blade I | Dbl Rapier II |
| 4 | Hekatoncheir I | Dyno Tooth II |
| 5 | Tri-edge I | Long Sword III |
| 6 | Grand Blade II | Blaster Gun I |
| 7 | Bone Blade II | Atlas Board I |
| 8 | Dbl Axe II | EXP Board I |
| 9 | Flame Rose II | Dbl Rapier IV |

### 16. `n_vmon` (`itemchange59.lua`)

| # | Das | Recibís |
|---|---|---|
| 1 | Offering | Kamikiri Maru II |
| 2 | Armor Board I | Grand Blade III |
| 3 | Bolt Board I | Beam Magnum III |
| 4 | Ice Board I | Long Sword I |
| 5 | Achilles Board II | Crusher III |
| 6 | Battle Hawk II | Meteor Board I |
| 7 | Mighty Talon II | Brave Core II |
| 8 | Beam Magnum II | Element Core II |
| 9 | Dyno Tooth I | Kurogane Maru II |
| 10 | Bone Blade III | EXP Board I |
| 11 | Kamikiri Maru III | Rack Board I |

### 17. `n_vmon` (`itemchange59.lua`)

| # | Das | Recibís |
|---|---|---|
| 1 | Offering | Dyno Tooth I |
| 2 | Mighty Talon I | Sword Break I |
| 3 | Battle Hawk I | Grand Blade III |
| 4 | Long Sword I | Mighty Talon III |
| 5 | Victor Sword I | War Sword V |
| 6 | Hekatoncheir I | War Sword V |
| 7 | Long Sword III | Grand Blade IV |
| 8 | Kamikiri Maru III | Mighty Talon IV |
| 9 | Vulture Core I | Dragoon V |
| 10 | Soul Burn V | Dbl Rapier V |

### 18. `n_weregaru` (`itemchange135.lua`)

| # | Das | Recibís |
|---|---|---|
| 1 | Victor Sword I | Sword Break I |
| 2 | Denji Prince I | Kamikiri Maru II |
| 3 | Hekatoncheir I | Dyno Tooth III |
| 4 | Atlas Board I | Onslaught I |
| 5 | Spirit Core I | Drill Bunker I |
| 6 | Grand Blade II | Beast Board I |
| 7 | Apocalypse I | Meteor Board I |
| 8 | Battle Hawk II | Rack Board I |
| 9 | Dragoon I | Infinity I |

### 19. `n_weregaru` (`itemchange135.lua`)

| # | Das | Recibís |
|---|---|---|
| 1 | Long Sword I | Bone Blade III |
| 2 | Dyno Tooth I | Sword Break I |
| 3 | Beam Magnum I | Battle Hawk IV |
| 4 | Tri-edge I | War Sword V |
| 5 | Beam Magnum III | Wizard Core I |
| 6 | Long Sword III | War Sword V |
| 7 | Dbl Rapier III | Divine Core I |
| 8 | Beam Magnum IV | Long Sword V |
| 9 | Long Sword V | Rack Board III |

### 20. `n_agumon` (`itemchange2.lua`)

| # | Das | Recibís |
|---|---|---|
| 1 | Offering | Crusher I |
| 2 | Denji Prince I | Bone Blade III |
| 3 | Hekatoncheir I | Kamikiri Maru III |
| 4 | Tri-edge I | Long Sword II |
| 5 | Sword Break I | Bolt Board I |
| 6 | Apocalypse I | Fire Board I |
| 7 | Astral Core I | Omega Raptor I |
| 8 | Dbl Axe II | Long Sword IV |
| 9 | Fang Smasher II | Grand Blade IV |

### 21. `n_agumon` (`itemchange2.lua`)

| # | Das | Recibís |
|---|---|---|
| 1 | Offering | Battle Hawk II |
| 2 | Dark Board I | Dbl Rapier III |
| 3 | Brave Core II | War Sword II |
| 4 | Achilles Board I | Victor Sword III |
| 5 | Rack Board I | Victor Sword III |
| 6 | Long Sword II | Beast Board I |
| 7 | Dyno Tooth II | Dbl Rapier I |
| 8 | Mighty Talon II | Armor Board I |
| 9 | Kamikiri Maru I | Light Blade I |
| 10 | Long Sword III | Mana Board I |
| 11 | Dbl Axe III | D Slayer III |

### 22. `n_agumon` (`itemchange2.lua`)

| # | Das | Recibís |
|---|---|---|
| 1 | Offering | Mighty Talon I |
| 2 | Battle Hawk I | Sword Break I |
| 3 | Dyno Tooth I | Tri-edge I |
| 4 | Kamikiri Maru I | Apocalypse I |
| 5 | Rune Blade I | Long Sword IV |
| 6 | Hekatoncheir I | Dyno Tooth IV |
| 7 | Dbl Axe III | War Sword V |
| 8 | Dbl Rapier III | War Sword V |
| 9 | Dyno Tooth III | Long Sword V |
| 10 | Onslaught V | Dbl Rapier V |

### 23. `n_agumon` (`itemchange97.lua`)

| # | Das | Recibís |
|---|---|---|
| 1 | Offering | Tri-edge I |
| 2 | Crusher I | Grand Blade II |
| 3 | Victor Sword I | Mighty Talon III |
| 4 | Denji Prince I | Dbl Axe III |
| 5 | Armor Board I | Dragoon I |
| 6 | Mighty Talon II | Dark Board I |
| 7 | Dbl Rapier II | Flame Rose II |
| 8 | Battle Hawk II | Kamikiri Maru IV |
| 9 | Onslaught I | Doom Bringer I |

### 24. `n_bwargrey` (`itemchange154.lua`)

| # | Das | Recibís |
|---|---|---|
| 1 | Bone Blade IV | Doom Bringer V |
| 2 | Wizard Core I | Dbl Rapier V |
| 3 | Long Sword V | Resist Board III |
| 4 | Long Sword V | BIT Board III |
