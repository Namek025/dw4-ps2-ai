#include "game.h"

/* beGenerater: object generators (MWCC C++). */
extern "C" {

extern igMetaObject* beGenerater_Meta;
extern igMetaObject* beGeneraterInfo_Meta;
extern igMetaObject* beLua_Meta;
extern char D_004E0858[]; /* "" */
extern char D_0050E850[]; /* "GENERATER" */
void beMessenger_entry(beMessenger* m, beBaseInfoRam* ram, const char* group, const char* info, s32 ramNo);

/* -- */

void beGenerater_prepare(beGenerater* mgr) {
    if (mgr->_luaManager == NULL) {
        mgr->_luaManager = (beLua*)func_00236F80(mgr->_insight, beLua_Meta);
    }
}

igMetaObject* beGenerater_getInfoMeta(void) {
    return beGeneraterInfo_Meta;
}

void beGenerater_entry(beGenerater* mgr, beBaseInfoRam* ram) {
    s32 ramNo = ram->_ramNo;

    beMessenger_entry(mgr->_messenger, ram, D_0050E850, ram->_parentInfo->_name ? ram->_parentInfo->_name : D_004E0858, ramNo);
}

INCLUDE_ASM("asm/nonmatchings/beGenerater", func_00384540);

INCLUDE_ASM("asm/nonmatchings/beGenerater", func_00384800);

INCLUDE_ASM("asm/nonmatchings/beGenerater", beGenerater_updateRam);

INCLUDE_ASM("asm/nonmatchings/beGenerater", beGenerater_unhandled);

igMetaObject* beGenerater_getMeta(void) {
    return beGenerater_Meta;
}

}
