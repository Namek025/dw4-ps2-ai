/* Mod: one level for all the forms of a player.
 *
 * Every form keeps its own LEVEL, EXP, weapon skills (SKILL, 9) and stat-up counts (UPCNT, 11) in
 * the BASE_* lists, so an evolution starts at its own (low) level. CURRENT2BASE
 * (beNDMWGameRam_current2Base) saves the current values into the current form's slot; the game
 * calls it after every level up and before switching forms (BASE2CURRENT then loads the new
 * form's slot). This rewrite saves them into all 16 slots instead: every form shares the player's
 * level, EXP, skills and UPCNT, and its stats come from its own shinka columns at that level
 * (INI + GROW * curve(level)). The evolve menu (which shows the BASE_* slots) shows the shared
 * values too. See docs/evolution.md.
 *
 * Build: python3 tools/mkpnach.py mods/src/shared_level.cpp beNDMWGameRam_current2Base 0x230
 */
typedef int s32;
typedef unsigned char u8;

struct IntList { /* igIntList */
    u8 pad0[8];
    s32 count;  /* +0x8 */
    s32 cap;    /* +0xC */
    s32* data;  /* +0x10 */
};

struct ObjList { /* igObjectList */
    u8 pad0[0x10];
    void** data; /* +0x10 */
};

struct DataInt { /* beDataObjInt */
    u8 pad0[0xC];
    s32 value; /* +0xC */
};

struct DataIntList { /* beDataObjIntList */
    u8 pad0[0xC];
    IntList* value; /* +0xC */
};

struct GameRam { /* beNDMWGameRam */
    u8 pad0[0x44];
    ObjList* playerDataListList; /* +0x44 */
};

#define FORMS 16
#define SKILLS 9
#define UPCNTS 11

extern "C" {
extern char D_00518858[]; /* "LEVEL" */
extern char D_00518880[]; /* "EXP" */
extern char D_00518860[]; /* "SKILL" */
extern char D_00518868[]; /* "UPCNT" */
extern char D_00518B30[]; /* "BASE_LEVEL" */
extern char D_00518B40[]; /* "BASE_EXP" */
extern char D_00518B50[]; /* "BASE_SKILL" */
extern char D_00518B60[]; /* "BASE_UPCNT" */

DataInt* beDataObjList_getInt(void* list, const char* name);
DataIntList* beDataObjList_getIntList(void* list, const char* name);

/* IntListSet: store if the index is inside the list */
#define SET(l, i, v) \
    if ((i) < (l)->count) { \
        (l)->data[i] = (v); \
    }

void beNDMWGameRam_current2Base(GameRam* gr, s32 player) {
    void* pl = gr->playerDataListList->data[player];
    s32 level = beDataObjList_getInt(pl, D_00518858)->value;
    s32 exp = beDataObjList_getInt(pl, D_00518880)->value;
    IntList* skill = beDataObjList_getIntList(pl, D_00518860)->value;
    IntList* upcnt = beDataObjList_getIntList(pl, D_00518868)->value;
    IntList* baseLevel = beDataObjList_getIntList(pl, D_00518B30)->value;
    IntList* baseExp = beDataObjList_getIntList(pl, D_00518B40)->value;
    IntList* baseSkill = beDataObjList_getIntList(pl, D_00518B50)->value;
    IntList* baseUpcnt = beDataObjList_getIntList(pl, D_00518B60)->value;
    s32 d;
    s32 i;

    for (d = 0; d < FORMS; d++) {
        SET(baseLevel, d, level)
        SET(baseExp, d, exp)
        for (i = 0; i < SKILLS; i++) {
            SET(baseSkill, d * SKILLS + i, skill->data[i])
        }
        for (i = 0; i < UPCNTS; i++) {
            SET(baseUpcnt, d * UPCNTS + i, upcnt->data[i])
        }
    }
}
}
