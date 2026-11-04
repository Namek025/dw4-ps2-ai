#include "game.h"

extern "C" {
extern char D_00515B80[]; /* "NDMWPANEL" */
void beMessenger_entry(beMessenger* m, beBaseInfoRam* ram, const char* group, const char* info, s32 ramNo);
extern igMetaObject* bePadManager_Meta;
extern igMetaObject* beNDMWPanelObjectItem_Meta;
extern igMetaObject* beNDMWPanelObjectStat_Meta;
extern igMetaObject* beNDMWPanelObjectWaza_Meta;
extern igMetaObject* beNDMWPanelWazaInfo_Meta;
extern igMetaObject* beNDMWPanelWaza_Meta;
}

void beNDMWPanelWaza_prepare(beNDMWPanelWaza* mgr) {
    if (mgr->_padManager == NULL) {
        mgr->_padManager = (bePadManager*)func_00236F80(mgr->_insight, bePadManager_Meta);
    }
}

igMetaObject* beNDMWPanelWaza_getInfoMeta(void) {
    return beNDMWPanelWazaInfo_Meta;
}

void beNDMWPanelWaza_entry(beNDMWPanelWaza* mgr, beBaseInfoRam* ram) {
    beMessenger_entry(mgr->_messenger, ram, D_00515B80, NULL, ram->_ramNo);
    ram->_isRun = false;
}

INCLUDE_ASM("asm/nonmatchings/beNDMWPanelWaza", beNDMWPanelWaza_updateRam);

INCLUDE_ASM("asm/nonmatchings/beNDMWPanelWaza", func_003D0760);

extern "C" {
extern char D_005159F0[];
extern char D_005159F8[];
extern char D_00515A08[];
extern char D_00515A20[];
s32 func_00447C10(void* item, void* mgr, const char* name);
}

/* The virtuals of a panel object, as called from here. */
class beNDMWPanelObjectV {
public:
    IG_VPAD_21
    virtual void unk5C();   /* +0x5C */
};

s32 func_003D08F0(beNDMWPanelWaza* mgr, beNDMWPanelWazaInfoRam* ram) {
    igObjectList* wazaList;
    igObject* player;
    s32 n;
    igObject* waza;
    s32 i;
    beNDMWPanelWazaInfoWork* work;
    s32 mp;
    s32 off;
    s32 ofs;
    s32 kind;

    work = ram->_work;
    if (work->_panelWaza->_isNoHealing != 0) {
        return 0;
    }
    beMessenger_selectGroup(mgr->_messenger, D_005159F0);
    beMessenger_pushInt(mgr->_messenger, ram->_ramNo);
    beMessenger_send(mgr->_messenger, D_005159F8);
    player = beMessenger_getObject(mgr->_messenger, 0);
    mp = beDataObjList_getInt((beDataObjList*)player, D_00515A20)->_data;
    wazaList = beDataObjList_getObjectList((beDataObjList*)player, D_00515A08)->_data;
    n = wazaList->_count;
    i = n - 1;
    off = i * 4;
    while (i >= 0) {
        waza = ((igObject**)wazaList->_data)[i];
        kind = ((beDataObjInt*)waza)->_data;
        n -= 1;
        if (kind >= 0x28 && kind < 0x32 && mp >= func_00447C10(waza, mgr, D_00515A20)) {
            ofs = n - 3;
            if (ofs < 0) {
                ofs = 0;
            } else {
                n -= ofs;
            }
            ((beNDMWPanelObjectV*)work->_panelWaza)->unk5C();
            work->_panelWaza->_cursor = n;
            work->_panelWaza->_offset = ofs;
            work->_panelWaza->_isEmg = true;
            return 1;
        }
        i -= 1;
        off -= 4;
    }
    return 0;
}

void func_003D0A90(beNDMWPanelWaza* mgr, beNDMWPanelWazaInfoRam* ram, s32 page, beNDMWMdlPlayerInfoWork* playerWork) {
    beNDMWPanelWazaInfoWork* work;
    beNDMWPanelObjectStat* stat;

    work = ram->_work;
    if (ram->_isRun) {
        if (ram->_st0 == 3) {
            ram->_isRun = true;
            work->_isWindowGenerate = true;
            work->_isCursorGenerate = false;
        }
        ram->_st0 = 0;
        ram->_st3 = 0;
        ram->_st2 = 0;
        ram->_st1 = 0;
    } else {
        ram->_isRun = true;
        work->_isWindowGenerate = true;
        work->_isCursorGenerate = false;
        ram->_st0 = 0;
        ram->_st3 = 0;
        ram->_st2 = 0;
        ram->_st1 = 0;
    }
    work->_page = page;
    work->_retIsFire = false;
    igIncRef(playerWork);
    igUnref(work->_playerWork);
    work->_playerWork = playerWork;
    stat = work->_panelStat;
    igIncRef(playerWork);
    igUnref(stat->_playerWork);
    stat->_playerWork = playerWork;
}

INCLUDE_ASM("asm/nonmatchings/beNDMWPanelWaza", beNDMWPanelWaza_handle);

igMetaObject* beNDMWPanelWaza_getMeta(void) {
    return beNDMWPanelWaza_Meta;
}

igMetaObject* beNDMWPanelObjectStat_getMeta(void) {
    return beNDMWPanelObjectStat_Meta;
}

void func_003D1450(void) {
}

igMetaObject* beNDMWPanelObjectItem_getMeta(void) {
    return beNDMWPanelObjectItem_Meta;
}

igMetaObject* beNDMWPanelObjectWaza_getMeta(void) {
    return beNDMWPanelObjectWaza_Meta;
}
