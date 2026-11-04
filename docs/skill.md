# Weapon skill growth (`P2ESKILLUP`)

After a hit the game may raise the skill of the weapon type used (`SKILL[WEAPONSKILLNO]`) by 1:

    cap = LEVEL*30 (LEVEL <= 36), LEVEL*20+360 (<= 107), LEVEL*10+1430 (<= 227), LEVEL*8+1884
    if (skill < cap):
        row     = first i with D_004CF590[i] >= skill   /* 1..10, 20, 30, 100, 300, 500, 1000, ... */
        chance  = D_004CF390[band][row][charged ? 2 : 0]  /* band = the cap's level range */
        permille = chance * shinka.<TYPE>_GROW * 500 / 1000
        if (rand % 100000 < permille) skill++

`<TYPE>_GROW` is 130 for the digimon's own weapon type, 100 otherwise. Column 1 of the table is
not read here.

The table grows with the skill, so the first points are the slowest: at skill 0 a hit has 0.65%
(main type, normal hit), at 10 6.9%, at 30 10.7%. Reaching the level 1 cap (30) takes ~980 hits
(~1280 with another weapon type). Damage is `(TSUYOSA*6 + skill*4) / enemy.MAMORI`, so a capped
skill matters early: Dorumon at level 5 goes from ~9 to ~16 per hit at the cap (estimate).

Raising the weight of a skill point is a bad lever: late game skills reach thousands and already
dominate the formula. The cap already limits the gain per level, so faster pacing is safe.

## Mod: Faster weapon skill (`patches/SLUS-20836_B871C1D0.pnach` in dw4-mods)

Band 0 (levels 1-36), rows 0-11 (skill 0-30): normal 231, charged 289. ~15% per hit, cap 30 in ~200
hits (260 with another type). Band 1+ untouched (starting a new weapon type after level 36 is
still slow: band 1 rows are half of band 0).

Tested in PCSX2 (2026-10-26): ~7 skill points from the first 3 Goburimon with Dorumon.

## Scaling (original table, main weapon type, normal hits)

Chance per hit by skill and level band:

| Skill | Lv 1-36 | Lv 37-107 | Lv 108-227 | Lv 228+ |
|---|---|---|---|---|
| 0 | 0.65% | 0.33% | 0.13% | 0.07% |
| 10 | 6.9% | 3.4% | 1.7% | 0.84% |
| 100 | 13.5% | 6.7% | 3.3% | 1.6% |
| 1000 | 26.3% | 13.1% | 6.6% | 3.3% |
| 3000 | - | 20.5% | 10.3% | 5.1% |

Each band halves the chances. Keeping up with the cap costs ~100-200 hits per level at any level
(the cap grows less per level as the bands drop: +30, +20, +10, +8). Level 1 is the exception (~980).
Starting a weapon type from 0 is what hurts: 0 to 100 takes ~1500 hits at levels 1-36, ~3000 at
37-107, ~6400 at 108-227; 0 to the cap at level 50 ~13400 hits (+30% for a non-main type).
