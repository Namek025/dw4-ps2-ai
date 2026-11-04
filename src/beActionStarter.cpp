#include "game.h"

/* beActionStarter: starts effects on a model when it broadcasts an action
   (MWCC C++). */
extern "C" {

extern igMetaObject* beActionStarterInfo_Meta;
extern igMetaObject* beActionStarterData2_Meta;
extern igMetaObject* beActionStarter_Meta;
extern char D_004E0858[]; /* "" */
extern char D_0050E110[]; /* "ACTION_BROADCAST" */
extern char D_0050E128[]; /* "MODELCTRL" */
extern char D_0050E138[]; /* "SETEFFECT" */
void beMessenger_entry(beMessenger* m, beBaseInfoRam* ram, const char* group, const char* info, s32 ramNo);
void beMessenger_setRunCheck(beMessenger* m, bool v);

}

/* igObject's virtuals. */
class igObjectV {
public:
    IG_VPAD_20
    virtual igMetaObject* getMeta(); /* +0x58 */
};

static inline const char* NameOf(igNamedObject* o) {
    return o->_name ? o->_name : D_004E0858;
}

extern "C" {

/* -- */

void beActionStarter_prepare(beActionStarter* mgr) {
}

void beActionStarter_entry(beActionStarter* mgr, beBaseInfoRam* ram) {
    beMessenger_entry(mgr->_messenger, ram, D_0050E110, NULL, -1);
}

igMetaObject* beActionStarter_getInfoMeta(void) {
    return beActionStarterInfo_Meta;
}

void beActionStarter_updateRam(beActionStarter* mgr) {
}

#ifdef NON_MATCHING
/* Nonmatching: the original rereads the model count in the first loop and
   keeps the info in s7 (ramNo spilled); ours hoists the count. */
/* An action was broadcast: send the model the effects bound to it. */
bool beActionStarter_handle(beActionStarter* mgr, beBaseInfoRam* ram, beMessengerWork* work) {
    beActionStarterInfo* info = (beActionStarterInfo*)ram->_parentInfo;
    const char* message = work->_message;
    const char* infoName = work->_infoName;
    s32 ramNo = work->_ramNo;
    beAsTargetModelDataList* models = info->_modelList;
    bool found = false;
    s32 i;

    for (i = 0; i < models->_count; i++) {
        if (infoName == NameOf(((beAsTargetModelData**)models->_data)[i])) {
            found = true;
            break;
        }
    }
    if (!found) {
        return true;
    }
    {
        beActionStarterDataList* datas = ((beAsTargetModelData**)models->_data)[i]->_dataList;
        s32 j;

        for (j = 0; j < datas->_count; j++) {
            beActionStarterData* d = ((beActionStarterData**)datas->_data)[j];

            if (NameOf(d) == message) {
                beMessenger_select(mgr->_messenger, D_0050E128, infoName, ramNo);
                beMessenger_pushString(mgr->_messenger, NameOf(info));
                beMessenger_pushString(mgr->_messenger, d->_message);
                beMessenger_pushFloat(mgr->_messenger, d->_delay);
                {
                    igVec3f v = d->_offset;

                    beMessenger_pushVec3f(mgr->_messenger, &v);
                }
                beMessenger_pushBool(mgr->_messenger, d->_isSeqDead);
                beMessenger_pushBool(mgr->_messenger, d->_isPositionLock);
                if (beActionStarterData2_Meta == ((igObjectV*)d)->getMeta()) {
                    igVec3f v = ((beActionStarterData2*)d)->_rotation;

                    beMessenger_pushVec3f(mgr->_messenger, &v);
                }
                beMessenger_setRunCheck(mgr->_messenger, true);
                beMessenger_send(mgr->_messenger, D_0050E138);
            }
        }
    }
    return true;
}
#else
INCLUDE_ASM("asm/nonmatchings/beActionStarter", beActionStarter_handle);
#endif

igMetaObject* beActionStarter_getMeta(void) {
    return beActionStarter_Meta;
}

}
