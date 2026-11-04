#include "game.h"

/* beSwitchCtrl: turns switch nodes on and off (MWCC C++). */
extern "C" {

extern igMetaObject* beSwitchCtrlInfo_Meta;
extern igMetaObject* beSwitchCtrl_Meta;
extern char D_0050FAB0[]; /* "SWITCHCTRL" */
igObject* func_00343430(void); /* message table */
void func_001DC870(igSwitch* sw); /* all children on */
void func_001DC8D0(igSwitch* sw); /* all children off */

/* -- */

void beSwitchCtrl_prepare(beSwitchCtrl* mgr) {
}

igMetaObject* beSwitchCtrl_getInfoMeta(void) {
    return beSwitchCtrlInfo_Meta;
}

void beSwitchCtrl_entry(beSwitchCtrl* mgr, beBaseInfoRam* ram) {
    beMessenger_entryRam(mgr->_messenger, ram, D_0050FAB0);
}

void beSwitchCtrl_updateRam(beSwitchCtrl* mgr) {
}

bool beSwitchCtrl_handle(beSwitchCtrl* mgr, beBaseInfoRam* ram, beMessengerWork* work) {
    beSwitchCtrlInfo* info = (beSwitchCtrlInfo*)ram->_parentInfo;

    switch (beMessenger_getMessageIndex(mgr->_messenger, func_00343430(), (const char*)work)) {
    case 0:
        func_001DC870(info->_switchNode);
        return true;
    case 1:
        func_001DC8D0(info->_switchNode);
        return true;
    }
    return false;
}

igMetaObject* beSwitchCtrl_getMeta(void) {
    return beSwitchCtrl_Meta;
}

}
