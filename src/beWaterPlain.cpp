#include "game.h"

extern "C" {
igObject* func_00340C60(void); /* beWaterPlain's message names */
s32 beMessenger_getVec3fCount(beMessenger* m);
void beMessenger_getVec3f(igVec3f* out, beMessenger* m, s32 i);
bool func_003A0D40(beWaterPlain* mgr, beWaterPlainInfoRam* ram, igVec3f* pos);
void func_003A0C20(beWaterPlain* mgr, beWaterPlainInfoRam* ram, igFloatList* grid, igVec3f* pos, f32 power);
extern char D_0050FB88[]; /* "WATERPLAIN" */
void beMessenger_entry(beMessenger* m, beBaseInfoRam* ram, const char* group, const char* info, s32 ramNo);
extern igMetaObject* beWaterPlainInfo_Meta;
extern igMetaObject* beWaterPlain_Meta;
}

extern "C" void beWaterPlain_prepare(void) {
}

INCLUDE_ASM("asm/nonmatchings/beWaterPlain", beWaterPlain_updateRam);

igMetaObject* beWaterPlain_getInfoMeta(void) {
    return beWaterPlainInfo_Meta;
}

void beWaterPlain_entry(beWaterPlain* mgr, beBaseInfoRam* ram) {
    beMessenger_entry(mgr->_messenger, ram, D_0050FB88, NULL, -1);
}

bool beWaterPlain_handle(beWaterPlain* mgr, beWaterPlainInfoRam* ram, beMessengerWork* work) {
    switch (beMessenger_getMessageIndex(mgr->_messenger, func_00340C60(), work->_message)) {
    case 0:
        if (beMessenger_getVec3fCount(mgr->_messenger) > 0 && ram->_st0 == 1) {
            igVec3f pos;

            beMessenger_getVec3f(&pos, mgr->_messenger, 0);
            if (func_003A0D40(mgr, ram, &pos)) {
                func_003A0C20(mgr, ram, ((igFloatList**)ram->_gridLists->_data)[0], &pos, 1.0f);
            }
        }
        break;
    default:
        return false;
    }
    return true;
}

igMetaObject* beWaterPlain_getMeta(void) {
    return beWaterPlain_Meta;
}
