#include "game.h"

extern "C" {
extern igMetaObject* bePadManager_Meta;
extern char D_005194C0[]; /* "SAVECTRL" */
extern igMetaObject* beNDMWSaveCtrlInfo_Meta;
extern igMetaObject* beNDMWSaveCtrl_Meta;
}

void beNDMWSaveCtrl_prepare(beNDMWSaveCtrl* mgr) {
    if (mgr->_padManager == NULL) {
        igAssign(mgr->_padManager, (bePadManager*)func_00236F80(mgr->_insight, bePadManager_Meta));
    }
}

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveCtrl", beNDMWSaveCtrl_updateRam);

igMetaObject* beNDMWSaveCtrl_getInfoMeta(void) {
    return beNDMWSaveCtrlInfo_Meta;
}

void beNDMWSaveCtrl_entry(beNDMWSaveCtrl* mgr, beBaseInfoRam* ram) {
    beMessenger_entryRam(mgr->_messenger, ram, D_005194C0);
}

INCLUDE_ASM("asm/nonmatchings/beNDMWSaveCtrl", beNDMWSaveCtrl_handle);

extern "C" {
igObject** func_00408070(igObject** self, s16 flags) {
    if (self != NULL) {
        igUnref(*self);
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}
}

igMetaObject* beNDMWSaveCtrl_getMeta(void) {
    return beNDMWSaveCtrl_Meta;
}
