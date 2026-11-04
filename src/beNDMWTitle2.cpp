#include "game.h"

/* beNDMWTitle2: title screen manager (MWCC C++). */
extern "C" {
extern igMetaObject* bePadManager_Meta;
extern bool D_00554E70;
igMetaEnum* func_003B2840(void); /* beNDMWTitle2's message names */
extern igMetaObject* beNDMWTitle2Info_Meta;
extern igMetaObject* beNDMWTitle2_Meta;

extern char D_005186A8[]; /* "TITLE2" */

void beNDMWTitle2_prepare(beNDMWTitle2* mgr) {
    if (mgr->_padManager == NULL) {
        igAssign(mgr->_padManager, (bePadManager*)func_00236F80(mgr->_insight, bePadManager_Meta));
    }
    if (mgr->_mcUtilCtrl != NULL) {
        igUnref(mgr->_mcUtilCtrl);
        mgr->_mcUtilCtrl = NULL;
    }
}

INCLUDE_ASM("asm/nonmatchings/beNDMWTitle2", beNDMWTitle2_updateRam);

/* Out-of-line destructor of a beNDMWTitle2 smart pointer (deleting form). The Ref type is
   not known; the code is the same for every IG_REF type. */
igObject** func_003F5210(igObject** self, s16 flags) {
    if (self != NULL) {
        igUnref(*self);
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}

/* Out-of-line destructor of a second beNDMWTitle2 smart pointer (deleting form). */
igObject** func_003F5290(igObject** self, s16 flags) {
    if (self != NULL) {
        igUnref(*self);
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}

igMetaObject* beNDMWTitle2_getInfoMeta(void) {
    return beNDMWTitle2Info_Meta;
}

void beNDMWTitle2_entry(beNDMWTitle2* mgr, beBaseInfoRam* ram) {
    beMessenger_entryRam(mgr->_messenger, ram, D_005186A8);
}

bool beNDMWTitle2_handle(beNDMWTitle2* mgr, beBaseInfoRam* ram, beMessengerWork* work) {
    if (ram->_dataList == NULL) {
        return true;
    }
    switch (beMessenger_getMessageIndex(mgr->_messenger, (igObject*)func_003B2840(), work->_message)) {
    case 0:
        ram->_st0 = 1;
        ram->_st3 = 0;
        ram->_st2 = 0;
        ram->_st1 = 0;
        break;
    case 1:
        D_00554E70 = true;
        break;
    }
    return true;
}

igMetaObject* beNDMWTitle2_getMeta(void) {
    return beNDMWTitle2_Meta;
}

}
