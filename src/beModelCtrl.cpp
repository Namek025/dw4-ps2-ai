#include "game.h"

extern "C" {
extern char D_0050F4A0[]; /* "SOUND" */
extern char D_0050F4B0[]; /* "DOWN" */
extern char D_0050F4B8[]; /* "VOLUMECTL" */
void beMessenger_pushLong(beMessenger* m, s64 v);
s64 beMessenger_getLong(beMessenger* m, s32 i);
extern char D_0050F4A8[]; /* "PLAY" */
extern char D_0050F468[]; /* "MODELCTRL" */
extern char D_0050F498[]; /* "SQ_ERAS" */
extern void* D_00562640; /* traversal callback table */
void func_001E06C0(void* table, igMetaObject* meta, void (*fn)(void*, void*));
igMetaObject* func_0034FD90(void); /* beModelCtrlNode2 */
igMetaObject* func_0034E870(void); /* beNodeAnim */
igMetaObject* func_0034E5D0(void); /* beLoadingNode */
void func_00398E50(void* a, void* b);
void func_00398EF0(void* a, void* b);
void func_0039A930(void* a, void* b);
void func_0039AB20(void* a, void* b);
void func_0039ADC0(void* a, void* b);
extern igMetaObject* beModelCtrlInfoRideOn2_Meta;
extern igMetaObject* beModelCtrlInfoRideOn3_Meta;
extern igMetaObject* beModelCtrlInfoRideOn4_Meta;
extern igMetaObject* beModelCtrlSCEffect_Meta;
extern igMetaObject* beModelCtrlSCHitBox_Meta;
extern igMetaObject* beModelCtrlSCSound_Meta;
extern igMetaObject* beModelCtrl_Meta;
void __dl__FPv(void* p);
}

INCLUDE_ASM("asm/nonmatchings/beModelCtrl", beModelCtrl_updateRam);

void func_003960E0(void) {
}

INCLUDE_ASM("asm/nonmatchings/beModelCtrl", func_003960F0);

INCLUDE_ASM("asm/nonmatchings/beModelCtrl", func_00396340);

INCLUDE_ASM("asm/nonmatchings/beModelCtrl", func_00396740);

INCLUDE_ASM("asm/nonmatchings/beModelCtrl", func_00396940);

INCLUDE_ASM("asm/nonmatchings/beModelCtrl", beModelCtrl_unhandled);

extern "C" {

void** func_00398C20(void** arg0, s16 arg1) {
    igObject* o;

    if (arg0 != NULL) {
        o = (igObject*)*arg0;
        igUnref(o);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

}

extern "C" {

void** func_00398CA0(void** arg0, s16 arg1) {
    igObject* o;

    if (arg0 != NULL) {
        o = (igObject*)*arg0;
        igUnref(o);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

}

extern "C" {

void** func_00398D20(void** arg0, s16 arg1) {
    igObject* o;

    if (arg0 != NULL) {
        o = (igObject*)*arg0;
        igUnref(o);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

}

void func_00398DA0(void) {
}

void func_00398DB0(void) {
}

void func_00398DC0(void) {
}

void func_00398DD0(void) {
}

void func_00398DE0(void) {
}

void func_00398DF0(void) {
}

s32 func_00398E00(void) {
    return 0;
}

igMetaObject* beModelCtrlInfoRideOn4_getMeta(void) {
    return beModelCtrlInfoRideOn4_Meta;
}

igMetaObject* beModelCtrlInfoRideOn3_getMeta(void) {
    return beModelCtrlInfoRideOn3_Meta;
}

igMetaObject* beModelCtrlInfoRideOn2_getMeta(void) {
    return beModelCtrlInfoRideOn2_Meta;
}

igMetaObject* func_00398E40(void) {
    return beModelCtrl_Meta;
}

void func_00398E50(void* a, void* b) {
    func_00398EF0(b, a);
}

/* Fixed-point time (18 fraction bits) scaled by a float. */
struct UnkTimeScale {
    u8 pad0[0x50];
    f32 unk50;
    u8 pad54[0x4];
    s64 unk58;
    u8 pad60[0x18];
    f32 unk78;
    u8 pad7C[0x4];
    s64 unk80;
};

s64 func_00398E80(UnkTimeScale* o, s64 t) {
    f32 f = (f32)(u32)(t >> 18);
    s64 r;

    f *= o->unk78;
    r = (s64)f << 18;
    return r + o->unk80;
}

INCLUDE_ASM("asm/nonmatchings/beModelCtrl", func_00398EF0);

void func_003993A0(void) {
    func_001E06C0(D_00562640, func_0034FD90(), func_00398E50);
}

INCLUDE_ASM("asm/nonmatchings/beModelCtrl", func_003993E0);

/* Object with a timer (current at 0xC, end at 0x1C) and a virtual at slot 21. */
struct UnkTimed {
    void* vtable;
    u8 pad4[0x8];
    f32 unkC;
    u8 pad10[0xC];
    f32 unk1C;
};

class UnkTimedV {
public:
    IG_VPAD_21
    virtual void f21(); /* +0x5C */
};

s32 func_003998F0(UnkTimed* o) {
    if (!(o->unkC < o->unk1C)) {
        return 0;
    }
    ((UnkTimedV*)o)->f21();
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/beModelCtrl", func_00399940);

/* Sends SQ_ERAS to the model control ram it names. */
struct UnkModelRef {
    u8 pad0[0x8];
    bool unk8;
    u8 pad9[0xB];
    s32 unk14; /* ram number */
    char* unk18; /* info name */
};

void func_00399BF0(UnkModelRef* o, beBaseInfoRam* ram) {
    if (o->unk8 && o->unk14 != -1) {
        beMessenger_call(ram->_parentManager->_messenger, D_0050F468, o->unk18, o->unk14, D_0050F498);
    }
}

/* Plays the sound named by the info; keeps the handle it returns (0 or -1 = none). */
struct UnkSoundPlay {
    u8 pad0[0x14];
    const char* unk14; /* sound name */
    s64 unk18; /* handle */
};

s32 func_00399C50(UnkSoundPlay* o, beBaseInfoRam* ram) {
    beBaseInfoManager* mgr = ram->_parentManager;
    s64 h;

    beMessenger_selectGroup(mgr->_messenger, D_0050F4A0);
    beMessenger_pushString(mgr->_messenger, o->unk14);
    beMessenger_send(mgr->_messenger, D_0050F4A8);
    h = beMessenger_getLong(mgr->_messenger, 0);
    if (h == -1) {
        return 0;
    }
    if (h == 0) {
        return 0;
    }
    if (beMessenger_getInt(mgr->_messenger, 0) != 4) {
        return 0;
    }
    o->unk18 = h;
    return 1;
}

/* Asks the sound manager to turn the volume down for a while. */
struct UnkVolumeDown {
    u8 pad0[0x18];
    s64 unk18; /* time */
};

void func_00399D10(UnkVolumeDown* o, beBaseInfoRam* ram) {
    beBaseInfoManager* mgr = ram->_parentManager;

    if (o->unk18 != 0) {
        beMessenger_selectGroup(mgr->_messenger, D_0050F4A0);
        beMessenger_pushString(mgr->_messenger, D_0050F4B0);
        beMessenger_pushLong(mgr->_messenger, o->unk18);
        beMessenger_send(mgr->_messenger, D_0050F4B8);
    }
}

/* Runs for a time: start() once it ends, update() after that. */
struct UnkTimed2 {
    void* vtable;
    u8 pad4[0x4];
    bool unk8;
    u8 pad9[0x3];
    f32 unkC; /* elapsed */
    f32 unk10; /* duration */
};

class UnkTimed2V {
public:
    IG_VPAD_21
    virtual bool f21(); /* +0x5C */
    virtual bool f22(); /* +0x60 */
};

#ifdef NON_MATCHING
/* 3 instructions off: the original materializes the 1 stored into unk8 first (li a2,1). */
bool func_00399D90(UnkTimed2* o, beBaseInfoRam* ram) {
    bool r = true;
    f32 t = o->unkC + ram->_parentManager->_system->_frameTimeF;

    o->unkC = t;
    if (!o->unk8) {
        if (!(t < o->unk10)) {
            o->unk8 = true;
            r = ((UnkTimed2V*)o)->f21();
        }
    } else {
        r = ((UnkTimed2V*)o)->f22();
    }
    return r;
}
#else
INCLUDE_ASM("asm/nonmatchings/beModelCtrl", func_00399D90);
#endif

/* Owner of a list of timed objects (list at 0x8). */
struct UnkTimedOwner {
    u8 pad0[0x8];
    igDataList* unk8;
};

bool func_00399D90(UnkTimed2* o, beBaseInfoRam* ram);

/* Runs each timed object; the ones that report done are unreferenced and removed. */
void func_00399E20(UnkTimedOwner* o, beBaseInfoRam* ram) {
    s32 n = o->unk8->_count;
    s32 i = 0;

    if (n > 0) {
        do {
            if (func_00399D90((UnkTimed2*)((igObject**)o->unk8->_data)[i], ram) == false) {
                igDataList* list = o->unk8;

                igUnref(((igObject**)list->_data)[i]);
                func_002D6CF0(list, i);
                i--;
                n--;
                ((igObject**)list->_data)[list->_count] = NULL;
            }
            i++;
        } while (i < n);
    }
}

INCLUDE_ASM("asm/nonmatchings/beModelCtrl", func_00399F20);

/* igObject's getMeta (+0x58) and a update virtual (+0x64) on the same objects. */
class UnkMetaV {
public:
    IG_VPAD_20
    virtual igMetaObject* getMeta(); /* +0x58 */
    virtual void f21(); /* +0x5C */
    virtual void f22(); /* +0x60 */
    virtual void f23(beBaseInfoRam* ram); /* +0x64 */
};

/* Runs the objects whose meta is `meta`, then removes them from the list. */
void func_0039A130(UnkTimedOwner* o, beBaseInfoRam* ram, igMetaObject* meta) {
    s32 n = o->unk8->_count;
    s32 i = 0;

    if (n > 0) {
        do {
            igObject* item = ((igObject**)o->unk8->_data)[i];

            if (meta == ((UnkMetaV*)item)->getMeta()) {
                ((UnkMetaV*)item)->f23(ram);
                igDataList* list = o->unk8;

                igUnref(((igObject**)list->_data)[i]);
                func_002D6CF0(list, i);
                i--;
                n--;
                ((igObject**)list->_data)[list->_count] = NULL;
            }
            i++;
        } while (i < n);
    }
}

INCLUDE_ASM("asm/nonmatchings/beModelCtrl", func_0039A250);

extern "C" {

void** func_0039A480(void** arg0, s16 arg1) {
    igObject* o;

    if (arg0 != NULL) {
        o = (igObject*)*arg0;
        igUnref(o);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

}

INCLUDE_ASM("asm/nonmatchings/beModelCtrl", func_0039A500);

extern "C" {

void** func_0039A6D0(void** arg0, s16 arg1) {
    igObject* o;

    if (arg0 != NULL) {
        o = (igObject*)*arg0;
        igUnref(o);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

}

INCLUDE_ASM("asm/nonmatchings/beModelCtrl", func_0039A750);

extern "C" {

void** func_0039A880(void** arg0, s16 arg1) {
    igObject* o;

    if (arg0 != NULL) {
        o = (igObject*)*arg0;
        igUnref(o);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

}

igMetaObject* beModelCtrlSCSound_getMeta(void) {
    return beModelCtrlSCSound_Meta;
}

igMetaObject* beModelCtrlSCEffect_getMeta(void) {
    return beModelCtrlSCEffect_Meta;
}

igMetaObject* beModelCtrlSCHitBox_getMeta(void) {
    return beModelCtrlSCHitBox_Meta;
}

void func_0039A930(void* a, void* b) {
    func_0039AB20(b, a);
}

void func_0039A960(UnkTimeScale* o, s64 base, s64 t) {
    f32 f = (f32)(u32)(t >> 18);

    f *= o->unk50;
    o->unk58 = base - ((s64)f << 18);
}

INCLUDE_ASM("asm/nonmatchings/beModelCtrl", func_0039A9E0);

INCLUDE_ASM("asm/nonmatchings/beModelCtrl", func_0039AB20);

void func_0039AD80(void) {
    func_001E06C0(D_00562640, func_0034E870(), func_0039A930);
}

void func_0039ADC0(void* a, void* b) {
    func_0039AB20(b, a);
}

/* Used by beFileListInfoManager_update: 64-bit counters (times?). */
struct UnkTimer {
    u8 pad0[0x38];
    s64 unk38;
    u8 pad40[0x28];
    s64 unk68;
    s64 unk70;
    s64 unk78;
};

void func_0039ADF0(UnkTimer* t) {
    t->unk38 = t->unk70;
}

bool func_0039AE00(UnkTimer* t) {
    return t->unk68 > t->unk78;
}

void func_0039AE10(void) {
    func_001E06C0(D_00562640, func_0034E5D0(), func_0039ADC0);
}
