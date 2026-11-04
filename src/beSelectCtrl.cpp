#include "game.h"

extern "C" {
extern char D_0050F7F0[]; /* "SELECTCTRL" */
extern igMetaObject* bePadManager_Meta;
extern igMetaObject* beSelectCtrlInfo_Meta;
extern igMetaObject* beSelectCtrl_Meta;
}

void beSelectCtrl_prepare(beSelectCtrl* mgr) {
    if (mgr->_padManager == NULL) {
        mgr->_padManager = (bePadManager*)func_00236F80(mgr->_insight, bePadManager_Meta);
    }
}

igMetaObject* beSelectCtrl_getInfoMeta(void) {
    return beSelectCtrlInfo_Meta;
}

void beSelectCtrl_entry(beSelectCtrl* mgr, beBaseInfoRam* ram) {
    beMessenger_entryRam(mgr->_messenger, ram, D_0050F7F0);
}

INCLUDE_ASM("asm/nonmatchings/beSelectCtrl", beSelectCtrl_updateRam);

INCLUDE_ASM("asm/nonmatchings/beSelectCtrl", beSelectCtrl_unhandled);

igMetaObject* beSelectCtrl_getMeta(void) {
    return beSelectCtrl_Meta;
}
