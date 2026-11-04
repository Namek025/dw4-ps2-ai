#include "game.h"

/* beSeCtl: sound effects bound to model actions (MWCC C++). */
extern "C" {

extern igMetaObject* beSound_Meta;
extern igMetaObject* beSeInfo_Meta;
extern igMetaObject* beSeCtl_Meta;
extern char D_004E0858[]; /* "" */
extern char D_0050F760[]; /* "ACTION_BROADCAST" */
extern char D_0050F778[]; /* "MODELCTRL" */
extern char D_0050F788[]; /* "SETSOUND" */
extern void* D_00562640;  /* traversal callback table */
void beMessenger_entry(beMessenger* m, beBaseInfoRam* ram, const char* group, const char* info, s32 ramNo);
s32 func_0039C460(void* node, void* traversal);
igMetaObject* func_0034B4A0(void);
void func_001E06C0(void* table, igMetaObject* meta, s32 (*fn)(void*, void*));
f32 func_0022FD70(s16 frames);

/* -- */

s32 func_0039C430(void* traversal, void* node) {
    return func_0039C460(node, traversal);
}

INCLUDE_ASM("asm/nonmatchings/beSeCtl", func_0039C460);

void func_0039C8C0(void) {
    func_001E06C0(D_00562640, func_0034B4A0(), func_0039C430);
}

void beSeCtl_connect(beSeCtl* mgr) {
    if (mgr->_sndMng == NULL) {
        mgr->_sndMng = (beSound*)func_00236F80(mgr->_insight, beSound_Meta);
    }
}

void beSeCtl_entry(beSeCtl* mgr, beBaseInfoRam* ram) {
    beMessenger_entry(mgr->_messenger, ram, D_0050F760, NULL, -1);
}

igMetaObject* beSeCtl_getInfoMeta(void) {
    return beSeInfo_Meta;
}

void beSeCtl_updateRam(beSeCtl* mgr) {
}

/* An action of a model was broadcast: send it the sounds bound to it. */
bool beSeCtl_handle(beSeCtl* mgr, beBaseInfoRam* ram, beMessengerWork* work) {
    s32 found = -1;
    beBaseInfoDataList* datas = ram->_parentInfo->_dataList;
    const char* message = work->_message;
    const char* infoName = work->_infoName;
    s32 ramNo = work->_ramNo;
    s32 i;
    beSeDataList* actions;
    s32 j;

    for (i = 0; i < datas->_count; i++) {
        beBaseInfoData* d = ((beBaseInfoData**)datas->_data)[i];

        if (strcmp(d->_name ? d->_name : D_004E0858, infoName) == 0) {
            found = i;
        }
    }
    if (found == -1) {
        return true;
    }
    actions = ((beSeInfoData**)datas->_data)[found]->_actionList;
    for (j = 0; j < actions->_count; j++) {
        s16 delay;
        beSeData* se = ((beSeData**)actions->_data)[j];

        if (strcmp(se->_name ? se->_name : D_004E0858, message) == 0) {
            delay = se->_delay;
            beMessenger_select(mgr->_messenger, D_0050F778, infoName, ramNo);
            beMessenger_pushString(mgr->_messenger, se->_sound);
            beMessenger_pushFloat(mgr->_messenger, func_0022FD70(delay));
            beMessenger_send(mgr->_messenger, D_0050F788);
        }
    }
    return true;
}

igMetaObject* beSeCtl_getMeta(void) {
    return beSeCtl_Meta;
}

}
