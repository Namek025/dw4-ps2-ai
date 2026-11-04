#include "game.h"

extern "C" {
extern bool D_00554E70; /* set by TITLE2's message 1 */
extern char D_00518310[]; /* "GAMERAM" */
extern char D_00518318[]; /* "GETCOMMON" */
extern char D_00518328[]; /* "CAMERAMODE" */
extern char D_00518338[]; /* "LIGHTMODE" */
extern igMetaObject* bePadManager_Meta;
extern char D_00518290[]; /* "LOADCTRL2" */
extern igMetaObject* beNDMWLoadCtrl2Info_Meta;
extern igMetaObject* beNDMWLoadCtrl2_Meta;
void __dl__FPv(void* p);
}

void beNDMWLoadCtrl2_prepare(beNDMWLoadCtrl2* mgr) {
    if (mgr->_padManager == NULL) {
        igAssign(mgr->_padManager, (bePadManager*)func_00236F80(mgr->_insight, bePadManager_Meta));
    }
}

INCLUDE_ASM("asm/nonmatchings/beNDMWLoadCtrl2", beNDMWLoadCtrl2_updateRam);

igMetaObject* beNDMWLoadCtrl2_getInfoMeta(void) {
    return beNDMWLoadCtrl2Info_Meta;
}

void beNDMWLoadCtrl2_entry(beNDMWLoadCtrl2* mgr, beBaseInfoRam* ram) {
    beMessenger_entryRam(mgr->_messenger, ram, D_00518290);
}

INCLUDE_ASM("asm/nonmatchings/beNDMWLoadCtrl2", beNDMWLoadCtrl2_handle);

void** func_003F2BC0(void** arg0, s16 arg1) {
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

void** func_003F2C40(void** arg0, s16 arg1) {
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

void** func_003F2CC0(void** arg0, s16 arg1) {
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

igMetaObject* beNDMWLoadCtrl2_getMeta(void) {
    return beNDMWLoadCtrl2_Meta;
}

/* Camera and light modes, read from the game RAM (GETCOMMON) or defaults. */
struct UnkLoadModes {
    u8 pad0[0x10];
    beMessenger* unk10;
    s32 unk14;
    u8 pad18[0x8];
    s32 unk20; /* camera mode */
    s32 unk24; /* light mode */
};

void func_003F2D50(UnkLoadModes* o, beMessenger* m) {
    o->unk10 = m;
    if (D_00554E70 == true) {
        beDataObjList* common;

        beMessenger_callGroup(o->unk10, D_00518310, D_00518318);
        common = (beDataObjList*)beMessenger_getObject(o->unk10, 0);
        o->unk20 = beDataObjList_getInt(common, D_00518328)->_data;
        o->unk24 = beDataObjList_getInt(common, D_00518338)->_data;
        o->unk14 = 0;
    } else {
        o->unk20 = 0;
        o->unk24 = 1;
        o->unk14 = 0;
    }
}
