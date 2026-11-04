#include "game.h"

extern "C" {
bool func_003A7E90(beBaseInfoRam* ram, const char* name); /* timer done? */
void func_003A8580(beBaseInfoRam* ram, const char* name);
void func_00302000(s32* out, igObjectList* list, igObject* o, s32); /* remove */
extern char D_0050DFD0[]; /* "FLASH" */
void func_00377090(beAction2* mgr, beAction2InfoRam* ram);
void func_00376DE0(beAction2* mgr, beAction2InfoRam* ram);
void func_00376680(beAction2* mgr, beAction2InfoRam* ram);
void func_00376340(beAction2* mgr, beAction2InfoRam* ram);
void func_00375E70(beAction2* mgr, beAction2InfoRam* ram);
extern char D_0050DFC8[]; /* "ACTION" */
extern f32 D_005626E0;
extern igMetaObject* beAction2Info_Meta;
extern igMetaObject* beAction2_Meta;
extern igMetaObject* be_Meta;
}

igMetaObject* be_getMeta(void) {
    return be_Meta;
}

extern "C" void beAction2_prepare(void) {
}

igMetaObject* beAction2_getInfoMeta(void) {
    return beAction2Info_Meta;
}

void beAction2_entry(beAction2* mgr, beBaseInfoRam* ram) {
    beMessenger_entryRam(mgr->_messenger, ram, D_0050DFC8);
    ram->_isRun = false;
    D_005626E0 = -1.0f / 30.0f;
}

#ifdef NON_MATCHING
/* Nonmatching: the switch tests case 0 first (MWCC order differs from the original's `li v1,1` + `beq` on case 1), so the top of the function and the epilogue are shifted. */
void func_00375E70(beAction2* mgr, beAction2InfoRam* ram) {
    beAction2InfoWork* work = ram->_work;
    beModelCtrlNode2* node;
    igObjectList* list;
    s32 sp4C;
    s32 sp48;
    s32 sp44;

    switch (work->_flashSt0) {
    case 1:
        if (func_003A7E90(ram, D_0050DFD0)) {
            node = ram->_node;
            list = (igObjectList*)node->_overrideAttrList;
            if (work->_blendStateAttr == NULL) {
                func_00302000(&sp4C, list, work->_colorAttr, 0);
                igUnref(work->_colorAttr);
                work->_colorAttr = NULL;
                func_00302000(&sp48, list, work->_lightingStateAttr, 0);
                igUnref(work->_lightingStateAttr);
                work->_lightingStateAttr = NULL;
            }
            func_00302000(&sp44, list, work->_textureStateAttr, 0);
            igUnref(work->_textureStateAttr);
            work->_textureStateAttr = NULL;
            func_003A8580(ram, D_0050DFD0);
            work->_flashSt0 = 0;
        }
        /* fallthrough */
    case 0:
        return;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/beAction2", func_00375E70);
#endif

INCLUDE_ASM("asm/nonmatchings/beAction2", func_00375FF0);

INCLUDE_ASM("asm/nonmatchings/beAction2", func_00376340);

INCLUDE_ASM("asm/nonmatchings/beAction2", func_003764C0);

INCLUDE_ASM("asm/nonmatchings/beAction2", func_00376680);

INCLUDE_ASM("asm/nonmatchings/beAction2", func_00376930);

INCLUDE_ASM("asm/nonmatchings/beAction2", func_00376DE0);

#ifdef NON_MATCHING
/* Nonmatching: the 1e9 constant and the start-time float take the other FP/int registers (distance 16); the tail scheduling of the time-bias load also differs. */
extern "C" s64 __fixsfdi(f32);

static inline s64 SecToTicks(f32 sec) {
    return __fixsfdi(1e9f * sec / 60.0f);
}

/* Starts a joint action: sets the joint state and time bias on the model node. */
s32 func_00376F40(beAction2* mgr, beAction2InfoRam* ram, beAction2JOINTSET* set) {
    beAction2InfoWork* work = ram->_work;
    beModelCtrlNode2* node;
    s64 startTime;
    s64 tm;
    f32 f;
    s32 v;

    if (set->_loopMode[0] == 'L') {
        work->_jointSt0 = 1;
    } else {
        work->_jointSt0 = 2;
    }
    work->_jointStartTimeL = SecToTicks(set->_startTime);
    work->_jointEndTimeL = SecToTicks(set->_endTime);

    tm = *(s64*)((char*)*(void**)((char*)mgr + 0xC) + 0x50);
    v = (s32)(tm >> 18);
    node = ram->_node;
    if (v >= 0) {
        f = (f32)v;
    } else {
        s32 h = ((u32)v >> 1) | (v & 1);
        f = (f32)h;
        f = f + f;
    }
    startTime = work->_jointStartTimeL;
    f = f * node->_timeScale;
    node->_timeBias = startTime - (__fixsfdi(f) << 18);
    work->_jointIsEnd = false;
    return 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/beAction2", func_00376F40);
#endif

INCLUDE_ASM("asm/nonmatchings/beAction2", func_00377090);

INCLUDE_ASM("asm/nonmatchings/beAction2", func_00377340);

INCLUDE_ASM("asm/nonmatchings/beAction2", func_00377700);

/* Runs the active parts of an action: motion, joint, fade, wait, flash. */
void beAction2_updateRam(beAction2* mgr, beAction2InfoRam* ram) {
    if (ram->_isRun) {
        beAction2InfoWork* work = ram->_work;

        if (!work->_isFreeze) {
            if (work->_motionSt0 != 0) {
                func_00377090(mgr, ram);
            }
            if (work->_jointSt0 != 0) {
                func_00376DE0(mgr, ram);
            }
            if (work->_fadeSt0 != 0) {
                func_00376680(mgr, ram);
            }
            if (work->_waitSt0 != 0) {
                func_00376340(mgr, ram);
            }
            if (work->_flashSt0 != 0) {
                func_00375E70(mgr, ram);
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/beAction2", beAction2_unhandled);

igMetaObject* beAction2_getMeta(void) {
    return beAction2_Meta;
}

extern "C" void beBaseInfoManager_onAddInfo(void) {
}

extern "C" s32 beBaseInfoManager_handle(void) {
    return 0;
}
