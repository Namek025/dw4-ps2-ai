#!/usr/bin/env python3
"""Base stats of every form at a level, from the shinka table (docs/gameram.md, RECALCPLAYER).

    shinka_stats.py LEVEL [CSV]       # CSV: build/db/shinka.csv (tools/igb.py <pack> --db --csv build/db)

value = INI + GROW * curve(LEVEL) / 100 (integer division). The game then adds UPCNT (stat-up
items), buffs and the equipped items, so compare with nothing equipped and no stat-up items used.
"""
import csv
import sys

STARTS = (1, 5, 10, 20, 30, 50, 70, 110, 150, 230)
HPMP = ((0, 10), (40, 20), (140, 50), (640, 80), (1440, 120), (3840, 180), (7440, 260),
        (17840, 180), (25040, 120), (34640, 80))
STAT = ((0, 10), (40, 9), (85, 8), (165, 7), (235, 6), (355, 5), (455, 4), (615, 3), (735, 2),
        (895, 1))
COLS = (("MHP", "MHP", HPMP), ("MMP", "MMP", HPMP), ("STR", "STR", STAT), ("DEF", "AC", STAT),
        ("WIS", "INT", STAT), ("SPI", "MND", STAT), ("SPD", "DEX", STAT))


def curve(table, level):
    seg = max(i for i, s in enumerate(STARTS) if level >= s)
    start, per = table[seg]
    return start + per * (level - STARTS[seg])


def main():
    level = int(sys.argv[1])
    path = sys.argv[2] if len(sys.argv) > 2 else "build/db/shinka.csv"
    rows = list(csv.DictReader(open(path, encoding="latin-1")))
    print("level %d: HP/MP curve %d, stat curve %d" % (level, curve(HPMP, level), curve(STAT, level)))
    print("%-3s %-12s" % ("#", "model") + "".join("%7s" % name for name, _, _ in COLS))
    for r in rows:
        vals = [int(r[col + "_INI"]) + int(r[col + "_GROW"]) * curve(table, level) // 100
                for _, col, table in COLS]
        print("%-3s %-12s" % (r["#"], r["MODEL_NAME"]) + "".join("%7d" % v for v in vals))


if __name__ == "__main__":
    main()
