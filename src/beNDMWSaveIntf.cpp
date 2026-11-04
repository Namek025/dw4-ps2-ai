#include "game.h"

/* beNDMWSaveIntf: save screen interface (MWCC C++). */

/* Unknown object used by func_00414640 / func_00412600. */
struct UnkSaveObj {
    u8 pad0[0x8];
    igInsightCore* unk8; /* 0x8 */
    beMessenger* unkC; /* 0xC */
    bePadManager* unk10; /* 0x10 */
    s32 unk14; /* 0x14 */
    u8 pad18[0x4];
    s32 unk1C; /* 0x1C */
    s32 unk20; /* 0x20 */
    s32 unk24; /* 0x24 */
    s32 unk28; /* 0x28 */
    u8 pad2C[0x4];
    s32 unk30; /* 0x30 */
    u8 pad34[0x4];
    struct UnkSaveObj* unk38; /* 0x38 */
    u8 pad3C[0x8];
    igObjectList* unk44; /* 0x44 */
    igObjectList* unk48; /* 0x48 */
    igObjectList* unk4C; /* 0x4C */
    u8 pad50[0x4];
    s32 unk54; /* 0x54 */
    s32 unk58; /* 0x58 */
    s32 unk5C; /* 0x5C */
    beDataObjList* unk60; /* 0x60 */
    igObjectList* unk64; /* 0x64 */
    igObjectList* unk68; /* 0x68 */
    igObjectList* unk6C; /* 0x6C */
    igObjectList* unk70; /* 0x70 */
    igObjectList* unk74; /* 0x74 */
    igObjectList* unk78; /* 0x78 */
};

extern "C" {
void func_00301C90(igObjectList* list, s32 n);
s32 func_00447C10(igObject* o, beNDMWStatus* status, const char* name);
extern igMetaObject* beNDMWStatus_Meta;
extern char D_00519FF8[]; /* "LEVEL" */
extern char D_0051A000[]; /* "LVPOINT" */
extern igMetaObject* bePadManager_Meta;
extern char D_00519908[]; /* "SAVEINTF_ALL" */
extern char D_00519918[]; /* "SAVEINTF" */
extern char D_00519F78[]; /* "DIGIMONNO" */
extern char D_00519F88[]; /* "SOUBI" */
extern char D_00519F90[]; /* "SKILL" */
extern char D_00519F98[]; /* "SKILLPOINT" */
extern char D_00519BA0[]; /* "GAMERAM" */
extern char D_00519C10[]; /* "SUBWND" */
extern char D_00519C40[]; /* "RECALCPLAYER" */
extern char D_00519C50[]; /* "UPDATE" */
extern char D_00519F40[]; /* "TXT" */
extern char D_00519C28[]; /* "TEXTCTL" */
extern char D_00519B50[]; /* "STWINFONT29" */
extern char D_005199E0[]; /* "font000" */
extern char D_00519F30[]; /* "TEXTSET" */
extern char D_00519B78[]; /* "DISPON" */
extern char D_00519B00[]; /* "DISPOFF" */
extern char D_00519B08[]; /* "SWITCH" */
extern char D_00519DB0[]; /* "STWINFONT11" */
extern char D_00519D98[]; /* "font100" */
extern char D_0051A018[]; /* "font200" */
extern char D_00519DC0[]; /* "font101" */
extern char D_0051A020[]; /* "font201" */
extern char D_00519DC8[]; /* "font102" */
extern char D_0051A028[]; /* "font202" */
extern char D_00519DD0[]; /* "font103" */
extern char D_0051A030[]; /* "font203" */
extern char D_00519DD8[]; /* "font104" */
extern char D_0051A038[]; /* "font204" */
extern char D_00519A90[]; /* "MODELCTRL" */
extern char D_00519A40[]; /* "i_stcursorleft" */
extern char D_00519A70[]; /* "i_stcursorright" */
extern char D_00519A80[]; /* "SQ_SELECT" */
extern char D_00519AA0[]; /* "SQ_MOVEEND" */
extern char D_00519AB0[]; /* "SQ_MOVENEXT" */
extern char D_00519E78[]; /* "i_stsoubi" */
extern char D_0051A008[]; /* "SQ_SUBOPEN" */
extern char D_00519A58[]; /* "SQ_CLOSE" */
bePadData* bePadManager_getPad(bePadManager* pads, s32 i);
const char* func_00447CB0(igObject* o, beNDMWStatus* status, const char* name);
void func_0030B3C0(igStringObj* so, const char* s);                    /* set */
void func_0030B460(igStringObj* so, const char* s, s32 at, u32 n);     /* insert */
extern igMetaObject* beSystem_Meta;
extern char* D_004C22F0; /* igStringObj's empty buffer */
extern char* D_004CF670;
extern char* D_004CF674;
extern char D_00519DA0[]; /* "STWINFONT10" */
extern char D_00519B60[]; /* " " */
extern char D_00519B68[]; /* "LEFT" */
extern char D_005199E8[]; /* "font001" */
extern char D_005199F0[]; /* "font002" */
u32 strlen(const char* s);
void beMessenger_entry(beMessenger* m, beBaseInfoRam* ram, const char* group, const char* info, s32 ramNo);
void func_00411950(UnkSaveObj* obj, igObjectList* list, s32 a2, s32 a3);
void __dl__FPv(void* p);
beBaseInfoDataList* func_0036AE80(void* pool); /* new beBaseInfoDataList */
extern igMetaObject* beNDMWSaveIntfInfo_Meta;
extern igMetaObject* beNDMWSaveIntf_Meta;

void func_00412600(UnkSaveObj* obj, igObjectList* list, s32 a2, s32 a3);

void beNDMWSaveIntf_prepare(beNDMWSaveIntf* mgr) {
    if (mgr->_padManager == NULL) {
        igAssign(mgr->_padManager, (bePadManager*)func_00236F80(mgr->_insight, bePadManager_Meta));
    }
}

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", beNDMWSaveIntf_updateRam);

/* Out-of-line copy of an inline used by beNDMWSaveIntf_updateRam: a new list into a ref. */
struct SaveListRef {
    beBaseInfoDataList* p;
};
struct SavePoolHolder {
    void* p;
};

SaveListRef* func_0040C180(SaveListRef* r, SavePoolHolder* pool) {
    r->p = func_0036AE80(pool->p);
    return r;
}

igMetaObject* beNDMWSaveIntf_getInfoMeta(void) {
    return beNDMWSaveIntfInfo_Meta;
}

void beNDMWSaveIntf_entry(beNDMWSaveIntf* mgr, beBaseInfoRam* ram) {
    beMessenger_entryRam(mgr->_messenger, ram, D_00519908);
    beMessenger_entry(mgr->_messenger, ram, D_00519918, NULL, ram->_ramNo);
}

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", beNDMWSaveIntf_handle);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_0040DA50);

igMetaObject* beNDMWSaveIntf_getMeta(void) {
    return beNDMWSaveIntf_Meta;
}

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_0040DAE0);

bool func_0040DBF0(UnkSaveObj* obj) {
    bePadData* pad;
    s32 trig;
    s32 rep;
    s32 lStk;
    bool padOk;
    bool stkOk;
    s32 count;
    bool moved;

    pad = bePadManager_getPad(obj->unk10, obj->unk14);
    trig = pad->_trigger;
    rep = pad->_repeat;
    lStk = pad->_lStk;
    padOk = true;
    if (!((pad->_lTrg | pad->_lRpt) & 8) && !((trig | rep) & 0x80)) {
        padOk = false;
    }
    stkOk = true;
    if (!(lStk & 1) && !(lStk & 4)) {
        stkOk = false;
    }
    count = obj->unk54;
    moved = false;
    if ((count > 0) && padOk && !stkOk) {
        obj->unk54 = count - 1;
        moved = true;
        if (obj->unk54 == 0) {
            beMessenger_call(obj->unkC, D_00519A90, D_00519A40, 0, D_00519AA0);
        } else {
            beMessenger_call(obj->unkC, D_00519A90, D_00519A40, 0, D_00519AB0);
        }
        beMessenger_call(obj->unkC, D_00519A90, D_00519A70, 0, D_00519A80);
    }
    return moved;
}

bool func_0040DD30(UnkSaveObj* obj) {
    bePadData* pad;
    s32 trig;
    s32 rep;
    s32 lStk;
    bool padOk;
    bool stkOk;
    s32 count;
    bool moved;

    pad = bePadManager_getPad(obj->unk10, obj->unk14);
    trig = pad->_trigger;
    rep = pad->_repeat;
    lStk = pad->_lStk;
    padOk = true;
    if (!((pad->_lTrg | pad->_lRpt) & 2) && !((trig | rep) & 0x20)) {
        padOk = false;
    }
    stkOk = true;
    if (!(lStk & 1) && !(lStk & 4)) {
        stkOk = false;
    }
    count = obj->unk54;
    moved = false;
    if ((count < (obj->unk58 - 1)) && padOk && !stkOk) {
        obj->unk54 = count + 1;
        moved = true;
        if (obj->unk54 == (obj->unk58 - 1)) {
            beMessenger_call(obj->unkC, D_00519A90, D_00519A70, 0, D_00519AA0);
        } else {
            beMessenger_call(obj->unkC, D_00519A90, D_00519A70, 0, D_00519AB0);
        }
        beMessenger_call(obj->unkC, D_00519A90, D_00519A40, 0, D_00519A80);
    }
    return moved;
}

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_0040DE80);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_0040EAE0);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_0040EB60);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_0040EBE0);

void** func_0040EC60(void** arg0, s16 arg1) {
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

void** func_0040ECE0(void** arg0, s16 arg1) {
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

void** func_0040ED60(void** arg0, s16 arg1) {
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

void** func_0040EDE0(void** arg0, s16 arg1) {
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

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_0040EE60);

void** func_0040F5A0(void** arg0, s16 arg1) {
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

void** func_0040F620(void** arg0, s16 arg1) {
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

void** func_0040F6A0(void** arg0, s16 arg1) {
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

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_0040F720);

void** func_00411080(void** arg0, s16 arg1) {
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

void** func_00411100(void** arg0, s16 arg1) {
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

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00411180);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_004115C0);

void beNDMWWindow_setText(beNDMWWindow* w, const char* type, const char* name, const char* text, const char* align);

void func_00411750(beNDMWWindow* w) {
    beNDMWWindow_setText(w, D_00519B50, D_00519D98, D_00519B60, D_00519B68);
    beNDMWWindow_setText(w, D_00519DA0, D_004CF670, D_00519B60, D_00519B68);
    beNDMWWindow_setText(w, D_00519DA0, D_004CF674, D_00519B60, D_00519B68);
    beNDMWWindow_setText(w, D_00519DB0, D_005199E0, D_00519B60, D_00519B68);
    beNDMWWindow_setText(w, D_00519DB0, D_005199E8, D_00519B60, D_00519B68);
    beNDMWWindow_setText(w, D_00519DB0, D_005199F0, D_00519B60, D_00519B68);
    beNDMWWindow_setText(w, D_00519DB0, D_00519D98, D_00519B60, D_00519B68);
    beNDMWWindow_setText(w, D_00519DB0, D_00519DC0, D_00519B60, D_00519B68);
    beNDMWWindow_setText(w, D_00519DB0, D_00519DC8, D_00519B60, D_00519B68);
    beNDMWWindow_setText(w, D_00519DB0, D_00519DD0, D_00519B60, D_00519B68);
    beNDMWWindow_setText(w, D_00519DB0, D_00519DD8, D_00519B60, D_00519B68);
    beNDMWWindow_setText(w, D_00519B50, D_00519D98, D_00519B60, D_00519B68);
}

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00411950);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00412600);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00412CC0);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00413220);

#ifdef NON_MATCHING
/* Nonmatching: so/len s-registers swapped and one extra nop before the destructor's null check (distance 6). */
IG_REF(igStringObj);

/* Item at unk18 (its number) and the unk64 list's element type. */
struct UnkSaveItem {
    u8 pad0[0x18];
    s32 unk18; /* 0x18 */
};

void func_00413480(UnkSaveObj* obj) {
    igObject* item;
    s32 len;
    const char* text;
    const char* msg;
    const char* disp;
    char buf[2];
    s32 idx;

    item = NULL;
    idx = obj->unk30;
    if (idx != -1) {
        item = ((igObject**)obj->unk64->_data)[idx];
    }
    if (item != NULL) {
        text = func_00447CB0(item, (beNDMWStatus*)func_00236F80(obj->unk8, beNDMWStatus_Meta), D_00519F40);
        buf[1] = 0;
        buf[0] = (char)(((UnkSaveItem*)item)->unk18 + 0x30);
        {
            s32 len;
            igStringObjRef so(((beSystem*)func_00236F80(obj->unk8, beSystem_Meta))->_globalStrObj);

            func_0030B3C0(so, text);
            len = so->_length;
            func_0030B460(so, buf, len, strlen(buf));
            msg = (const char*)so->_buffer;
            if (msg == NULL) {
                msg = D_004C22F0;
            }
        }
        beMessenger_selectGroup(obj->unkC, D_00519C28);
        beMessenger_pushString(obj->unkC, D_00519B50);
        beMessenger_pushString(obj->unkC, D_005199E0);
        beMessenger_pushString(obj->unkC, msg);
        beMessenger_send(obj->unkC, D_00519F30);
        disp = D_00519B78;
    } else {
        disp = D_00519B00;
    }
    beMessenger_selectGroup(obj->unkC, D_00519C28);
    beMessenger_pushString(obj->unkC, D_00519B50);
    beMessenger_pushString(obj->unkC, D_005199E0);
    beMessenger_pushString(obj->unkC, disp);
    beMessenger_send(obj->unkC, D_00519B08);
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00413480);
#endif

void func_00413670(UnkSaveObj* s) {
    switch (s->unk30) {
    case 0:
        s->unk20 = 1;
        s->unk28 = 0;
        s->unk24 = 0;
        break;
    case 1:
    case 2:
        s->unk20 = 1;
        s->unk28 = 0;
        s->unk24 = 0;
        s->unk28 = 1;
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_004136D0);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00413C20);

void** func_00413FD0(void** arg0, s16 arg1) {
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

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00414050);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_004141A0);

void** func_00414420(void** arg0, s16 arg1) {
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

s32 func_004144A0(UnkSaveObj* obj, s32 arg1) {
    beNDMWStatus* status;
    s32 digimonNo;
    igObject* item;
    s32 soubi;
    igIntList* skillList;
    s32 skill;
    s32 skillPoint;

    item = ((igObject**)obj->unk78->_data)[arg1];
    status = (beNDMWStatus*)func_00236F80(obj->unk8, beNDMWStatus_Meta);
    digimonNo = beDataObjList_getInt(obj->unk60, D_00519F78)->_data;
    soubi = (1 << digimonNo) & func_00447C10(item, status, D_00519F88);
    skillList = beDataObjList_getIntList(obj->unk60, D_00519F90)->_data;
    skill = func_00447C10(item, status, D_00519F90);
    skillPoint = func_00447C10(item, status, D_00519F98);
    if (soubi != 0 && ((s32*)skillList->_data)[skill] >= skillPoint) {
        return 1;
    }
    return 0;
}

/* UnkSaveObj's virtuals. */
class UnkSaveObjV {
public:
    IG_VPAD_38
    virtual void f38(); /* +0xA0 */
};

void func_004145B0(UnkSaveObj* obj) {
    func_00411950(obj, obj->unk64, obj->unk54, obj->unk30);
    if (obj->unk54 == 0) {
        ((UnkSaveObjV*)obj)->f38();
    }
    obj->unk38->unk38->unk30 = 100;
}

void func_00414610(UnkSaveObj* obj, s32 n) {
    func_00412600(obj, obj->unk70, obj->unk5C, n);
}

void func_00414640(UnkSaveObj* obj, s32 n) {
    if (n != -1) {
        func_00412600(obj, obj->unk64, 0, n);
    }
}

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00414670);

void func_004148D0(UnkSaveObj* obj) {
    const char* text;
    s32 item;
    s32 idx;
    const char* disp;

    item = 0;
    idx = obj->unk30;
    if (idx != -1) {
        item = ((s32*)obj->unk70->_data)[idx];
    }
    if (item != 0) {
        text = func_00447CB0((igObject*)item, (beNDMWStatus*)func_00236F80(obj->unk8, beNDMWStatus_Meta), D_00519F40);
        beMessenger_selectGroup(obj->unkC, D_00519C28);
        beMessenger_pushString(obj->unkC, D_00519B50);
        beMessenger_pushString(obj->unkC, D_005199E0);
        beMessenger_pushString(obj->unkC, text);
        beMessenger_send(obj->unkC, D_00519F30);
        disp = D_00519B78;
    } else {
        disp = D_00519B00;
    }
    beMessenger_selectGroup(obj->unkC, D_00519C28);
    beMessenger_pushString(obj->unkC, D_00519B50);
    beMessenger_pushString(obj->unkC, D_005199E0);
    beMessenger_pushString(obj->unkC, disp);
    beMessenger_send(obj->unkC, D_00519B08);
}

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00414A00);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00414D10);

void func_00414E40(UnkSaveObj* obj) {
    func_00411950(obj, obj->unk70, obj->unk5C, obj->unk30);
}

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00414E70);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00414EE0);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00415440);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00415680);

void func_00415870(UnkSaveObj* s) {
    s->unk20 = 1;
    s->unk28 = 0;
    s->unk24 = 0;
}

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00415890);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00415DE0);

/* UnkSaveObj's virtuals at +0x68 and +0x84 (the table before f38). */
class UnkSaveObjV2 {
public:
    IG_VPAD_24
    virtual void f68(); /* +0x68 */
    virtual void p25();
    virtual void p26();
    virtual void p27();
    virtual void p28();
    virtual void p29();
    virtual void p30();
    virtual void f84(s32 a, s32 b); /* +0x84 */
};

void func_00416130(UnkSaveObj* obj) {
    s32 state;

    state = obj->unk30;
    switch (state) {
    case 0:
        obj->unk20 = 4;
        obj->unk28 = 0;
        obj->unk24 = 0;
        break;
    case 1:
        obj->unk20 = 2;
        obj->unk28 = 0;
        obj->unk24 = 0;
        break;
    case 2:
        obj->unk30 = -1;
        ((UnkSaveObjV2*)obj)->f84(2, state);
        obj->unk30 = obj->unk54;
        ((UnkSaveObjV2*)obj)->f68();
        beMessenger_select(obj->unkC, D_00519BA0, NULL, 0);
        beMessenger_pushInt(obj->unkC, obj->unk14);
        beMessenger_send(obj->unkC, D_00519C40);
        beMessenger_selectGroup(obj->unkC, D_00519C10);
        beMessenger_pushInt(obj->unkC, obj->unk14);
        beMessenger_send(obj->unkC, D_00519C50);
        obj->unk1C = 0;
        obj->unk28 = 0;
        obj->unk24 = 0;
        obj->unk20 = 0;
        obj->unk20 = 1;
        obj->unk28 = 0;
        obj->unk24 = 0;
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00416250);

/* Whether entry i's level is at least its LVPOINT value. */
bool func_004164D0(UnkSaveObj* obj, s32 i) {
    beNDMWStatus* st;
    igObject* e;
    s32 level;

    e = ((igObject**)obj->unk78->_data)[i];
    st = (beNDMWStatus*)func_00236F80(obj->unk8, beNDMWStatus_Meta);
    level = beDataObjList_getInt(obj->unk60, D_00519FF8)->_data;
    return !(level < func_00447C10(e, st, D_0051A000));
}

void func_00416550(UnkSaveObj* obj) {
    func_00411950(obj, obj->unk68, obj->unk54, obj->unk30);
    if (obj->unk54 == 0) {
        ((UnkSaveObjV*)obj)->f38();
    }
}

void func_004165A0(UnkSaveObj* obj, s32 n) {
    func_00412600(obj, obj->unk74, obj->unk5C, n);
}

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_004165D0);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_004168F0);

void func_00416B50(UnkSaveObj* obj) {
    igObject* o;
    const char* txt;
    const char* disp;
    s32 state;

    o = NULL;
    state = obj->unk30;
    if (state != -1) {
        o = ((igObject**)obj->unk74->_data)[state];
    }
    if (o != NULL) {
        txt = func_00447CB0(o, (beNDMWStatus*)func_00236F80(obj->unk8, beNDMWStatus_Meta), D_00519F40);
        beMessenger_selectGroup(obj->unkC, D_00519C28);
        beMessenger_pushString(obj->unkC, D_00519B50);
        beMessenger_pushString(obj->unkC, D_005199E0);
        beMessenger_pushString(obj->unkC, txt);
        beMessenger_send(obj->unkC, D_00519F30);
        disp = D_00519B78;
    } else {
        disp = D_00519B00;
    }
    beMessenger_selectGroup(obj->unkC, D_00519C28);
    beMessenger_pushString(obj->unkC, D_00519B50);
    beMessenger_pushString(obj->unkC, D_005199E0);
    beMessenger_pushString(obj->unkC, disp);
    beMessenger_send(obj->unkC, D_00519B08);
}

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00416C80);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00416F90);

void func_004170C0(UnkSaveObj* obj) {
    func_00411950(obj, obj->unk74, obj->unk5C, obj->unk30);
}

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_004170F0);

void beNDMWWindow_playModelAtOrigin(beNDMWWindow* w, const char* info, s32 ramNo, const char* seq, const char* closeSeq);
void beNDMWWindow_playCloseModels(beNDMWWindow* w);
void beNDMWWindow_clearAllText(beNDMWWindow* w);

/* Close path (arg1 == 0): swap out the pooled strings of unk44 and unk4C, clear the lists, then clear the window text. */
void func_00417160(UnkSaveObj* obj, s32 arg1) {
    if (arg1 != 0) {
        beNDMWWindow_playModelAtOrigin((beNDMWWindow*)obj, D_00519E78, 0, D_0051A008, D_00519A58);
        return;
    }
    beNDMWWindow_playCloseModels((beNDMWWindow*)obj);
    ClearStrList(obj->unk44);
    obj->unk48->_count = 0;
    ClearStrList(obj->unk4C);
    beNDMWWindow_clearAllText((beNDMWWindow*)obj);
}

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00417350);

void func_00417580(UnkSaveObj* obj) {
    igObject* o;
    const char* txt;
    const char* disp;
    s32 state;

    o = NULL;
    state = obj->unk30;
    if (state != -1) {
        o = ((igObject**)obj->unk6C->_data)[state];
    }
    if (o != NULL) {
        txt = func_00447CB0(o, (beNDMWStatus*)func_00236F80(obj->unk8, beNDMWStatus_Meta), D_00519F40);
        beMessenger_selectGroup(obj->unkC, D_00519C28);
        beMessenger_pushString(obj->unkC, D_00519B50);
        beMessenger_pushString(obj->unkC, D_005199E0);
        beMessenger_pushString(obj->unkC, txt);
        beMessenger_send(obj->unkC, D_00519F30);
        disp = D_00519B78;
    } else {
        disp = D_00519B00;
    }
    beMessenger_selectGroup(obj->unkC, D_00519C28);
    beMessenger_pushString(obj->unkC, D_00519B50);
    beMessenger_pushString(obj->unkC, D_005199E0);
    beMessenger_pushString(obj->unkC, disp);
    beMessenger_send(obj->unkC, D_00519B08);
}

void func_004176B0(UnkSaveObj* s) {
    s->unk20 = 1;
    s->unk28 = 0;
    s->unk24 = 0;
    s->unk28 = 1;
}

#ifdef NON_MATCHING
/* Nonmatching: 34 off; loop entry block (lui of D_00519B00 after the init, jumped to from the loop start) is laid out before the loop instead of after it. */
void func_004176D0(UnkSaveObj* obj) {
    s32 i;
    const char* off;
    const char* font1;
    const char* font2;

    off = D_00519B00;
    for (i = 0; i < 5; i++) {
        switch (i) {
        default:
            font1 = D_00519D98;
            font2 = D_0051A018;
            break;
        case 1:
            font1 = D_00519DC0;
            font2 = D_0051A020;
            break;
        case 2:
            font1 = D_00519DC8;
            font2 = D_0051A028;
            break;
        case 3:
            font1 = D_00519DD0;
            font2 = D_0051A030;
            break;
        case 4:
            font1 = D_00519DD8;
            font2 = D_0051A038;
            break;
        }
        beMessenger_selectGroup(obj->unkC, D_00519C28);
        beMessenger_pushString(obj->unkC, D_00519DB0);
        beMessenger_pushString(obj->unkC, font1);
        beMessenger_pushString(obj->unkC, off);
        beMessenger_send(obj->unkC, D_00519B08);
        beMessenger_selectGroup(obj->unkC, D_00519C28);
        beMessenger_pushString(obj->unkC, D_00519DB0);
        beMessenger_pushString(obj->unkC, font2);
        beMessenger_pushString(obj->unkC, off);
        beMessenger_send(obj->unkC, D_00519B08);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_004176D0);
#endif

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00417880);

void func_00417BA0(UnkSaveObj* obj) {
    s32 state;

    state = obj->unk30;
    switch (state) {
    case 0:
        obj->unk20 = 4;
        obj->unk28 = 0;
        obj->unk24 = 0;
        break;
    case 1:
        obj->unk20 = 2;
        obj->unk28 = 0;
        obj->unk24 = 0;
        break;
    case 2:
        obj->unk30 = -1;
        ((UnkSaveObjV2*)obj)->f84(2, state);
        obj->unk30 = obj->unk54;
        ((UnkSaveObjV2*)obj)->f68();
        beMessenger_select(obj->unkC, D_00519BA0, NULL, 0);
        beMessenger_pushInt(obj->unkC, obj->unk14);
        beMessenger_send(obj->unkC, D_00519C40);
        beMessenger_selectGroup(obj->unkC, D_00519C10);
        beMessenger_pushInt(obj->unkC, obj->unk14);
        beMessenger_send(obj->unkC, D_00519C50);
        obj->unk1C = 0;
        obj->unk28 = 0;
        obj->unk24 = 0;
        obj->unk20 = 0;
        obj->unk20 = 1;
        obj->unk28 = 0;
        obj->unk24 = 0;
        break;
    }
}

extern "C" {
igObject* func_003BAF10(igMemoryPool* pool); /* instantiate beNDMWStatusCtrlD0 */
void beNDMWWindow_setup(beNDMWWindow* w, beNDMWWindow* mother, igInsightCore* insight, s32 playerNo, s32 entCode);
void beNDMWWindow_callLua(beNDMWWindow* w, const char* name, s32 arg);
}
extern bool D_00554C14;
extern void* D_00587110;
extern s32 D_004CF6F0;
extern char D_00519B90[]; /* script name for beNDMWWindow_callLua */

static inline void* GetPool(void* o) {
    void* a = func_00301000(o);

    return D_00554C14 ? a : func_002F88E0(D_00587110);
}

IG_REF(beNDMWStatusCtrlD0);

void func_00417CC0(UnkSaveObj* obj) {
    beNDMWStatusCtrlD0Ref w((beNDMWStatusCtrlD0*)func_003BAF10((igMemoryPool*)GetPool(obj)), IG_ADOPT);
    s32 n;
    igIntList* kinds;

    beNDMWWindow_setup((beNDMWWindow*)w.p, (beNDMWWindow*)obj, obj->unk8, obj->unk14, 0);
    func_00301E30((igObjectList*)((beNDMWWindow*)obj)->_subWindow, w.p);
    APPEND_STR(w->_textCloseTypeList, D_00519B50);
    APPEND_STR(w->_textCloseNameList, D_00519D98);
    beNDMWWindow_callLua((beNDMWWindow*)w.p, D_00519B90, 0x8B);
    kinds = w->_kindList;
    n = kinds->_count;
    if (n < kinds->_capacity) {
        (*(volatile s32*)&kinds->_count)++;
    } else {
        func_002D6790(kinds, n + 1, 4);
    }
    ((s32*)kinds->_data)[n] = D_004CF6F0;
    w->_isEquipDisable = true;
    w->_isUnknownDisable = true;
    w->_isAutoClose = false;
}

bool func_00417F40(UnkSaveObj* obj, s32 i) {
    beNDMWStatus* st;
    igObject* e;
    s32 level;

    e = ((igObject**)obj->unk78->_data)[i];
    st = (beNDMWStatus*)func_00236F80(obj->unk8, beNDMWStatus_Meta);
    level = beDataObjList_getInt(obj->unk60, D_00519FF8)->_data;
    return !(level < func_00447C10(e, st, D_0051A000));
}

void func_00417FC0(UnkSaveObj* obj) {
    func_00411950(obj, obj->unk6C, obj->unk54, obj->unk30);
}

void func_00417FF0(void) {
}

void func_00418000(UnkSaveObj* obj, s32 n) {
    func_00412600(obj, obj->unk6C, obj->unk54, n);
}

void func_00418030(void) {
}

void func_00418040(void) {
}

void func_00418050(void) {
}

void func_00418060(void) {
}

void func_00418070(void) {
}

void func_00418080(void) {
}

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00418090);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00418AC0);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_004191F0);

s32 func_00419680(UnkSaveObj* obj) {
    s32 base;
    if (obj->unk30 == -1) {
        return -1;
    }
    base = obj->unk54 * 12;
    return base + obj->unk30;
}

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_004196C0);

void** func_0041A380(void** arg0, s16 arg1) {
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

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_0041A400);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_0041AAA0);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_0041ACF0);

void** func_0041D240(void** arg0, s16 arg1) {
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

void func_0041D2C0(igObjectList* list, s32 start, s32 n) {
    s32 end = start + n;
    s32 i;
    char** slot;
    char* old;
    char* str;

    for (i = start; i < end; i++) {
        slot = (char**)list->_data + i;
        str = func_002EAF60(GetStrPool(), NULL);
        old = *slot;
        ReleaseStr(old);
        *slot = str;
    }
}

void func_0041D3B0(igObjectList* list, s32 start, s32 n) {
    s32 i;
    s32 end = start + n;

    for (i = start; i < end; i++) {
        ((igObject**)list->_data)[i] = NULL;
    }
}

void func_0041D3F0(igObjectList* list, s32 start, s32 n) {
    s32 end = start + n;
    s32 i;
    igObject* o;

    for (i = start; i < end; i++) {
        o = ((igObject**)list->_data)[i];
        igUnref(o);
    }
}

/* Out-of-line copy of a ref's operator=(T*). */
struct SaveObjRef {
    igObject* p;
};

SaveObjRef* func_0041D490(SaveObjRef* r, igObject* o) {
    igIncRef(o);
    igUnref(r->p);
    r->p = o;
    return r;
}

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_0041D510);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_0041D810);

void** func_0041DDC0(void** arg0, s16 arg1) {
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

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_0041DE40);

void** func_0041F110(void** arg0, s16 arg1) {
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

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_0041F190);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00420230);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_004207A0);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00422C00);

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveIntf", func_00423000);

}
