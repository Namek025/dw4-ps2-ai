#include "game.h"

/* Virtual views of the Alchemy window / visual context (same slots as beBkColor). */
class igVisualContextV {
public:
    IG_VPAD_176
    virtual void setClearColorU32(u32 rgba); /* +0x2C8 */
};

class igWindowV {
public:
    IG_VPAD_36
    virtual igVisualContextV* getVisualContext(); /* +0x98 */
};

IG_REF(igStringObj);

extern "C" {
extern char D_005155C0[]; /* "STAGECTL" */
extern char D_005155D0[]; /* "SOUND" */
extern char D_005155E0[]; /* "ENVSTOP" */
extern char D_005155E8[]; /* "SESTOP" */
extern char D_00515698[]; /* ".igb" */
extern char D_005156A0[]; /* "ENABLE" */
extern char D_005156A8[]; /* "DISABLE" */
extern char D_005156B0[]; /* "STATUS" */
extern char* D_004C22F0; /* igStringObj's empty buffer */
igObject* func_003BBD40(void);
void func_0030B3C0(igStringObj* so, const char* s);
void func_0030B460(igStringObj* so, const char* s, s32 at, u32 n);
u32 strlen(const char* s);
void beMessenger_entry(beMessenger* m, beBaseInfoRam* ram, const char* group, const char* info, s32 ramNo);
extern igMetaObject* beNDMWStageCtlInfo_Meta;
extern igMetaObject* beNDMWStageCtl_Meta;
extern igMetaObject* bePadManager_Meta;
extern char D_005155D8[]; /* "BGMSTOP" */
extern char D_005155F0[]; /* "SUBWND" */
extern char D_005155F8[]; /* "ISRUN" */
extern char D_00515600[]; /* "REMOVE" */
extern char D_00515608[]; /* "LAYERCTL" */
extern char D_00515618[]; /* "ALL" */
extern char D_00515620[]; /* "DISPOFF" */
extern char D_00515628[]; /* "SWITCH" */
extern char D_00515630[]; /* "MODELCTRL" */
extern char D_00515640[]; /* "ONFREEZE" */
extern char D_00515650[]; /* "GENERATER" */
extern char D_00515660[]; /* "stagis10" */
extern char D_00515670[]; /* "NEXTSTAGE" */
void beMessenger_setFreeze(beMessenger* m, s32 on);
}

bePadData* bePadManager_getPad(bePadManager* mgr, s32 n);

extern "C" void beNDMWStageCtl_prepare(void* self) {
}

void beNDMWStageCtl_entry(beNDMWStageCtl* mgr, beBaseInfoRam* ram) {
    beMessenger_entry(mgr->_messenger, ram, D_005155C0, NULL, -1);
}

igMetaObject* beNDMWStageCtl_getInfoMeta(void) {
    return beNDMWStageCtlInfo_Meta;
}

#ifdef NON_MATCHING
/* Nonmatching: the pad test's button load stays in v1 (the original hoists it into the branch delay slot in a0), and the case 0 end-of-block store isn't in the delay slot of the final b. */
void beNDMWStageCtl_updateRam(beNDMWStageCtl* self, beBaseInfoRam* ram) {
    bePadData* pad;
    bool go;

    switch (ram->_st0) {
    case 0:
        if (self->_system->_isSResetEnable != false) {
            pad = bePadManager_getPad((bePadManager*)func_00236F80(self->_insight, bePadManager_Meta), 0);
            go = (pad->_trigger & 8) != 0;
            if (go) {
                go = (pad->_button & 1) != 0;
            }
            if (go) {
                ram->_st0 = ram->_st0 + 1;
                ram->_st3 = 0;
                ram->_st2 = 0;
                ram->_st1 = 0;
            }
        }
        break;
    case 1:
        if (self->_system->_isSResetEnable == false) {
            ram->_st0 = 0;
            ram->_st3 = 0;
            ram->_st2 = 0;
            ram->_st1 = 0;
            return;
        }
        beMessenger_callGroup(self->_messenger, D_005155D0, D_005155D8);
        beMessenger_callGroup(self->_messenger, D_005155D0, D_005155E0);
        beMessenger_callGroup(self->_messenger, D_005155D0, D_005155E8);
        beMessenger_callGroup(self->_messenger, D_005155F0, D_005155F8);
        if (beMessenger_getBool(self->_messenger, 0) & 0xFF) {
            beMessenger_callGroup(self->_messenger, D_005155F0, D_00515600);
            ((igWindowV*)self->_insight->_window)->getVisualContext()->setClearColorU32(0xFF000000);
            beMessenger_selectGroup(self->_messenger, D_00515608);
            beMessenger_pushString(self->_messenger, D_00515618);
            beMessenger_pushString(self->_messenger, D_00515620);
            beMessenger_send(self->_messenger, D_00515628);
        }
        ram->_st0 = ram->_st0 + 1;
        ram->_st3 = 0;
        ram->_st2 = 0;
        ram->_st1 = 0;
        return;
    case 2:
        beMessenger_callGroup(self->_messenger, D_005155F0, D_005155F8);
        if (!(beMessenger_getBool(self->_messenger, 0) & 0xFF)) {
            beMessenger_selectGroup(self->_messenger, D_00515608);
            beMessenger_pushString(self->_messenger, D_00515618);
            beMessenger_pushString(self->_messenger, D_00515620);
            beMessenger_send(self->_messenger, D_00515628);
            beMessenger_callGroup(self->_messenger, D_00515630, D_00515640);
            beMessenger_callGroup(self->_messenger, D_00515650, D_00515640);
            beMessenger_setFreeze(self->_messenger, 1);
    case 3:
    case 4:
            ram->_st0 = ram->_st0 + 1;
            ram->_st3 = 0;
            ram->_st2 = 0;
            ram->_st1 = 0;
        }
        break;
    case 5:
        beMessenger_selectGroup(self->_messenger, D_005155C0);
        beMessenger_pushString(self->_messenger, D_00515660);
        beMessenger_send(self->_messenger, D_00515670);
        break;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWStageCtl", beNDMWStageCtl_updateRam);
#endif

#ifdef NON_MATCHING
/* Nonmatching: the s-registers of mgr and work come out swapped (s1/s0 vs the original's s0/s1), which shifts the rest of the body's register choices. */
s32 beNDMWStageCtl_handle(beNDMWStageCtl* mgr, beBaseInfoRam* ram, beMessengerWork* work) {
    beNDMWStageCtl* self = mgr;
    s32 idx;

    idx = beMessenger_getMessageIndex(self->_messenger, func_003BBD40(), work->_message);
    switch (idx) {
    case 0: {
        igStringObjRef so(self->_system->_globalStrObj);
        s32 len;
        char* name;
        char* next;
        beSystem* sys;

        func_0030B3C0(so, beMessenger_getString(self->_messenger, 0));
        len = so->_length;
        func_0030B460(so, D_00515698, len, strlen(D_00515698));
        ((igWindowV*)self->_insight->_window)->getVisualContext()->setClearColorU32(0xFF000000);
        beMessenger_callGroup(self->_messenger, D_005155D0, D_005155E8);
        beMessenger_callGroup(self->_messenger, D_005155D0, D_005155E0);
        name = (char*)so->_buffer;
        if (name == NULL) {
            name = D_004C22F0;
        }
        sys = self->_system;
        next = func_002EAF60(GetStrPool(), name);
        ReleaseStr(sys->_nextStage);
        sys->_nextStage = next;
        self->_system->_isStageLoop = false;
    }
        return 1;
    case 1:
        if (beMessenger_getStringCount(self->_messenger) > 0) {
            char* name = beMessenger_getString(self->_messenger, 0);
            bePadManager* pad = (bePadManager*)func_00236F80(self->_insight, bePadManager_Meta);

            if (strcmp(name, D_005156A0) == 0) {
                pad->_isPadDisable = false;
            } else if (strcmp(name, D_005156A8) == 0) {
                pad->_isPadDisable = true;
            } else if (strcmp(name, D_005156B0) == 0) {
                beMessenger_clearBools(self->_messenger);
                beMessenger_pushBool(self->_messenger, pad->_isPadDisable);
            }
        }
        return 1;
    case 2:
        beMessenger_pushBool(self->_messenger, ram->_st0 != 0);
        return 1;
    default:
        return 0;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWStageCtl", beNDMWStageCtl_handle);
#endif

igMetaObject* beNDMWStageCtl_getMeta(void) {
    return beNDMWStageCtl_Meta;
}
