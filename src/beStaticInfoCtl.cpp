#include "game.h"

/* beStaticInfoCtl: list of the static infos, handed out on request
   (MWCC C++). */
extern "C" {

extern igMetaObject* beMessenger_Meta;
extern igMetaObject* beStaticInfo_Meta;
extern igMetaObject* beStaticInfoCtl_Meta;
extern char D_004E0858[]; /* "" */
extern char D_0050FF68[]; /* "STATICINFO" */
void beBaseInfoManager_end(beBaseInfoManager* mgr);
void beMessenger_entry(beMessenger* m, beBaseInfoRam* ram, const char* group, const char* info, s32 ramNo);
igObject* func_00343700(void); /* message table */

}

/* beStaticInfoCtl's virtuals used here. */
class beStaticInfoCtlV {
public:
    IG_VPAD_32
    virtual void connect(); /* +0x88 */
};

template <class L>
static inline void ObjListClear(L* list) {
    s32 n = list->_count;
    s32 i;

    for (i = 0; i < n; i++) {
        igUnref(((igObject**)list->_data)[i]);
    }
    {
        s32 c = list->_count;
        s32 j;

        for (j = 0; j < c; j++) {
            ((igObject**)list->_data)[j] = NULL;
        }
    }
    list->_count = 0;
}

static inline bool IsNull(void* p) {
    return !(p != NULL);
}

extern "C" {

/* -- */

void beStaticInfoCtl_end(beStaticInfoCtl* mgr) {
    ObjListClear(mgr->_staticInfoList);
    beBaseInfoManager_end(mgr);
}

void beStaticInfoCtl_connect(beStaticInfoCtl* mgr) {
    if (mgr->_messenger == NULL) {
        mgr->_messenger = (beMessenger*)func_00236F80(mgr->_insight, beMessenger_Meta);
    }
}

igMetaObject* beStaticInfoCtl_getInfoMeta(void) {
    return beStaticInfo_Meta;
}

void beStaticInfoCtl_updateRam(beStaticInfoCtl* mgr) {
}

/* Register a ram with the messenger and keep its info. */
void beStaticInfoCtl_entry(beStaticInfoCtl* mgr, beBaseInfoRam* ram) {
    beBaseInfo* info = ram->_parentInfo;

    while (IsNull(mgr->_messenger)) {
        ((beStaticInfoCtlV*)mgr)->connect();
    }
    {
        s32 ramNo = ram->_ramNo;

        beMessenger_entry(mgr->_messenger, ram, D_0050FF68, info->_name ? info->_name : D_004E0858, ramNo);
    }
    func_00301E30(mgr->_staticInfoList, info);
}

/* Message handler: returns the list of static infos. */
bool beStaticInfoCtl_handle(beStaticInfoCtl* mgr, beBaseInfoRam* ram, beMessengerWork* work) {
    switch (beMessenger_getMessageIndex(mgr->_messenger, func_00343700(), work->_message)) {
    case 0:
        beMessenger_pushObject(mgr->_messenger, mgr->_staticInfoList);
        break;
    default:
        return false;
    }
    return true;
}

igMetaObject* beStaticInfoCtl_getMeta(void) {
    return beStaticInfoCtl_Meta;
}

}
