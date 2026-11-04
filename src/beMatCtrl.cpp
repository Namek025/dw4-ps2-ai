#include "game.h"

/* beMatCtrl: material switches (MWCC C++). */
extern "C" {
igObject* func_0035A6B0(void); /* beMatCtrl's message names */
void func_001DC870(igSwitch* sw); /* all children on */
void func_001DC8D0(igSwitch* sw); /* all children off */
extern igMetaObject* beLua_Meta;

extern igMetaObject* beMatCtrl_Meta;
extern igMetaObject* beMatCtrlInfo_Meta;
extern char D_0050ED58[]; /* "MATCTRL" */
void beMessenger_entry(beMessenger* m, beBaseInfoRam* ram, const char* group, const char* info, s32 ramNo);

/* -- */

/* Is the material being checked on? */
bool func_0038ACA0(beMatCtrl* mgr) {
    bool r = false;

    if (((s32*)mgr->_wkMatOnList->_data)[mgr->_wkChkMat] != 0) {
        r = true;
    }
    return r;
}

/* Is the material on for every existing player? */
bool func_0038ACD0(beMatCtrl* mgr) {
    bool r = true;
    s32 i;

    for (i = 0; i < 4; i++) {
        if (mgr->_wkPlExist[i]) {
            s32 bit = 1 << i;

            if (bit != (bit & ((s32*)mgr->_wkMatOnList->_data)[mgr->_wkChkMat])) {
                r = false;
                break;
            }
        }
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/beMatCtrl", func_0038AD40);

/* Turned on for player pl this frame. */
bool func_0038ADD0(beMatCtrl* mgr, s32 pl) {
    pl = 1 << pl;

    if (pl == (pl & ((s32*)mgr->_wkMatOnList->_data)[mgr->_wkChkMat]) && pl != (pl & ((s32*)mgr->_wkMatOldOnList->_data)[mgr->_wkChkMat])) {
        return true;
    }
    return false;
}

/* Turned off for player pl this frame. */
bool func_0038AE30(beMatCtrl* mgr, s32 pl) {
    pl = 1 << pl;

    if (pl != (pl & ((s32*)mgr->_wkMatOnList->_data)[mgr->_wkChkMat]) && pl == (pl & ((s32*)mgr->_wkMatOldOnList->_data)[mgr->_wkChkMat])) {
        return true;
    }
    return false;
}

void func_0038AE90(beMatCtrl* mgr, bool flag) {
    mgr->_wkChgFlagNO = mgr->_wkChkMat;
    mgr->_wkChgFlag = flag;
}

INCLUDE_ASM("asm/nonmatchings/beMatCtrl", func_0038AEA0);

void beMatCtrl_prepare(beMatCtrl* mgr) {
    mgr->_lua = (beLua*)func_00236F80(mgr->_insight, beLua_Meta);
    if (mgr->_wkMatOnList != NULL) {
        mgr->_wkMatOnList->_count = 0;
        igUnref(mgr->_wkMatOnList);
        mgr->_wkMatOnList = NULL;
    }
    if (mgr->_wkMatOldOnList != NULL) {
        mgr->_wkMatOldOnList->_count = 0;
        igUnref(mgr->_wkMatOldOnList);
        mgr->_wkMatOldOnList = NULL;
    }
}

igMetaObject* beMatCtrl_getInfoMeta(void) {
    return beMatCtrlInfo_Meta;
}

void beMatCtrl_entry(beMatCtrl* mgr, beBaseInfoRam* ram) {
    beMessenger_entry(mgr->_messenger, ram, D_0050ED58, NULL, -1);
}

s32 func_0018B630(igAABox* box, f32* pos);

#ifdef NON_MATCHING
/* Nonmatching: 11 instructions off; the i = 0 store and the store-block registers differ. */
void func_0038B080(s32 arg0, igIntList* arg1, beMatCtrlSearchList* arg2, s32 arg3, f32* arg4, beMatCtrlDataList* arg5) {
    s32 i;
    s32 count;
    s32 mask;
    s32 idx;
    s32 off;

    count = arg2->_count;
    if (count != 0 && 0 < count) {
        i = 0;
        mask = 1 << arg3;
        off = 0;
        do {
            if (((beMatCtrlSearch*)*(s32*)((char*)arg2->_data + off))->_minX <= *arg4 &&
                !(((beMatCtrlSearch*)*(s32*)((char*)arg2->_data + off))->_maxX < *arg4)) {
                idx = ((beMatCtrlSearch*)*(s32*)((char*)arg2->_data + off))->_matNO;
                if (idx != -1) {
                    if (func_0018B630(((beMatCtrlData**)arg5->_data)[idx]->_chkBox, arg4) == 2) {
                        s32 v;
                        s32* p = &((s32*)arg1->_data)[idx];
                        s32 n = arg1->_count;

                        v = *p;

                        if (n != 0) {
                            v |= mask;
                            if (idx >= 0 && idx < n) {
                                *p = v;
                            }
                        }
                    }
                }
                func_0038B080(arg0, arg1, ((beMatCtrlSearch*)*(s32*)((char*)arg2->_data + off))->_memberList, arg3, arg4, arg5);
            }
            i += 1;
            off += 4;
        } while (i < count);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/beMatCtrl", func_0038B080);
#endif

INCLUDE_ASM("asm/nonmatchings/beMatCtrl", beMatCtrl_updateRam);

bool beMatCtrl_handle(beMatCtrl* mgr, beBaseInfoRam* ram, beMessengerWork* work) {
    beMatCtrlInfo* info = (beMatCtrlInfo*)ram->_parentInfo;

    switch (beMessenger_getMessageIndex(mgr->_messenger, func_0035A6B0(), work->_message)) {
    case 0:
        func_001DC870(info->_switchgraph);
        break;
    case 1:
        func_001DC8D0(info->_switchgraph);
        break;
    case 2:
        mgr->_isLuaCallDisp = true;
        break;
    case 3:
        mgr->_isLuaCallDisp = false;
        break;
    case 4:
        ram->_isRun = false;
        break;
    case 5:
        ram->_isRun = true;
        break;
    default:
        return false;
    }
    return true;
}

igMetaObject* beMatCtrl_getMeta(void) {
    return beMatCtrl_Meta;
}

}
