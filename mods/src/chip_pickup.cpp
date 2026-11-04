/* Mod: pick up your own backup chip by standing next to it and pressing X; no Raise Disk needed.
 *
 * Replaces beNDMWMdlPlayer2's per-frame update (func_003D4EA0, 0x21C bytes in the original) with
 * the same logic, the power-on range cut to CHIP_RANGE, plus one case: while the chip is "powered
 * on", if the nearest player is the chip's owner and the owner's pad just pressed X, the chip
 * sends itself "RAISE" the way the Raise Disk does (beNDMWGameRam technique -50), with 100% EXP
 * back, and clears the press from the pad (the chip updates before the player: no attack).
 * The chip's own handler (func_003D50C0) does the rest: BIT and EXP back from BKCHIPDATA, chip
 * removed, BKCHIPSTAGE cleared. See docs/death.md.
 *
 * Build: python3 tools/mkpnach.py mods/src/chip_pickup.cpp func_003D4EA0 0x220
 */
typedef int s32;
typedef unsigned char u8;
typedef float f32;

struct Messenger;

struct PadData {
    u8 pad0[0x48];
    s32 trigger; /* +0x48 buttons pressed this frame */
};

struct Work { /* beNDMWMdlPlayer2InfoWork */
    u8 pad0[0xEC];
    u8 isPowerOn;   /* +0xEC */
    u8 pad1[3];
    s32 timerCount; /* +0xF0 */
};

struct Model {
    u8 pad0[0x20];
    f32 matrix[16]; /* +0x20 */
};

struct Data {
    u8 pad0[0x10];
    Model* model; /* +0x10 */
    Work* work;   /* +0x14 */
};

struct Ram { /* beBaseInfoRam */
    void* vtable;
    s32 refCount;
    s32 ramNo; /* +0x8: owner player */
    u8 pad0[0x28 - 0xC];
    void* dataList; /* +0x28 */
};

struct Mgr;

struct MgrVtable {
    u8 pad0[0x8C];
    void (*setAction)(Mgr* m, Ram* ram, const char* name); /* +0x8C */
};

struct Mgr { /* beNDMWMdlPlayer2 */
    MgrVtable* vtable;
    u8 pad0[0x14 - 0x4];
    Messenger* messenger; /* +0x14 */
    u8 pad1[0x20 - 0x18];
    void* lua; /* +0x20 */
    u8 pad2[0x2C - 0x24];
    void* padManager; /* +0x2C */
};

#define PAD_CROSS 0x4000
/* Squared distance. The original powers the chip on at 900 (30 units); here it powers on where
   X works, at 15 units, so the light tells you when you can pick it up. */
#define CHIP_RANGE (15.0f * 15.0f)

extern "C" {
extern u8 D_00554C14;
extern void* D_00587110;
extern void* D_00585BE0;     /* data meta */
extern char D_005165C8[];    /* work data name */
extern char D_00516608[];    /* target type (player) */
extern char D_00516610[];    /* target mode */
extern char D_00516618[];    /* action: power on */
extern char D_00516620[];    /* action: power off */
extern char D_00516630[];    /* script when no player around */
extern char D_00519290[];    /* "PLAYER2" */
extern char D_005166A0[];    /* "RAISE" */
extern s32 D_004CF620[];     /* Raise Disk: HP of a revived player */

void* func_00301000(void* o);
void* func_002F88E0(void* p);
void* func_0036AE80(void* pool);
Data* func_003022C0(void* list, void* meta, const char* name);
PadData* bePadManager_getPad(void* pads, s32 i);
bool beTargetObj_search(const f32* m, const char* type, const char* mode);
f32 beTargetObj_getTargetDist(void);
s32 beTargetObj_getTargetRamNo(void);
void func_00389FF0(void* lua, const char* name, const char* dir);
void beMessenger_select(Messenger* m, const char* group, const char* info, s32 ramNo);
void beMessenger_pushMatrix(Messenger* m, const f32* v);
void beMessenger_pushInt(Messenger* m, s32 v);
void beMessenger_send(Messenger* m, const char* message);

void func_003D4EA0(Mgr* m, Ram* ram) {
    Data* d;
    Work* w;
    PadData* pad;
    const char* action;
    f32 dist;

    if (ram->dataList == 0) {
        /* like the original: func_00301000 (a getter) always called, used when the flag is set */
        void* pool = func_00301000(ram);

        if (!D_00554C14) {
            pool = func_002F88E0(D_00587110);
        }
        ram->dataList = func_0036AE80(pool); /* old list is NULL: nothing to release */
    }
    d = func_003022C0(ram->dataList, D_00585BE0, D_005165C8);
    w = d->work;
    if (w->timerCount < 10) {
        w->timerCount++;
    }
    if (!beTargetObj_search(d->model->matrix, D_00516608, D_00516610)) {
        func_00389FF0(m->lua, D_00516630, 0);
        return;
    }
    /* written to fit in the original's 0x220 bytes: action is used right after it is set
       (no s-register), and both actions share the call (the flag just flips) */
    dist = beTargetObj_getTargetDist();
    if (!w->isPowerOn) {
        if (!(dist < CHIP_RANGE)) {
            return;
        }
        action = D_00516618;
    } else if (!(dist <= CHIP_RANGE)) {
        action = D_00516620;
    } else {
        if (beTargetObj_getTargetRamNo() == ram->ramNo &&
            ((pad = bePadManager_getPad(m->padManager, ram->ramNo))->trigger & PAD_CROSS)) {
            Messenger* g = m->messenger;

            pad->trigger ^= PAD_CROSS; /* eat the press so the player doesn't attack too */
            beMessenger_select(g, D_00519290, 0, ram->ramNo);
            beMessenger_pushMatrix(g, d->model->matrix);
            beMessenger_pushInt(g, 100);
            beMessenger_pushInt(g, D_004CF620[0]);
            beMessenger_send(g, D_005166A0);
        }
        return;
    }
    w->isPowerOn ^= 1;
    m->vtable->setAction(m, ram, action);
}
}
