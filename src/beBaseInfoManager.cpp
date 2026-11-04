#include "game.h"

/* beBaseInfoManager: base of the game's managers. Keeps the infos of its
   class, numbers their rams and runs them every frame (MWCC C++). */
extern "C" {

extern igMetaObject* beSystem_Meta;
extern igMetaObject* beMessenger_Meta;
extern igMetaObject* beBaseInfoManager_Meta;
void func_00236BE0(igInsightCore* insight, void* o);
void func_00236C40(igInsightCore* insight, void* o);
bool beSystem_isRunning(beSystem* sys);

}

/* The virtuals of a manager, as called from here. */
class beBaseInfoManagerV {
public:
    IG_VPAD_28
    virtual void onAddInfo(beBaseInfo* info);   /* +0x78 */
    virtual void entry(beBaseInfoRam* ram);     /* +0x7C */
    virtual igMetaObject* getInfoMeta();        /* +0x80 */
    virtual void updateRam(beBaseInfoRam* ram); /* +0x84 */
    virtual void prepare();                     /* +0x88 */
};

/* igObject's virtuals. */
class igObjectV {
public:
    IG_VPAD_20
    virtual igMetaObject* getMeta(); /* +0x58 */
};

#define V(mgr) ((beBaseInfoManagerV*)(mgr))

template <class T>
static inline void SetState(T* o, s32 st) {
    o->_st0 = st;
    o->_st3 = 0;
    o->_st2 = 0;
    o->_st1 = 0;
}

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

extern "C" {

/* -- */

s32 beBaseInfoManager_unhandled(beBaseInfoManager* mgr) {
    return 0;
}

void beBaseInfoManager_registerToInsight(beBaseInfoManager* mgr) {
}

void beBaseInfoManager_unRegisterFromInsight(beBaseInfoManager* mgr) {
}

void beBaseInfoManager_begin(beBaseInfoManager* mgr) {
    V(mgr)->prepare();
    mgr->_system = (beSystem*)func_00236F80(mgr->_insight, beSystem_Meta);
    mgr->_messenger = (beMessenger*)func_00236F80(mgr->_insight, beMessenger_Meta);
}

void beBaseInfoManager_end(beBaseInfoManager* mgr) {
}

/* Take an info of this manager's class: keep it, number its rams and register
   them. */
bool beBaseInfoManager_appendInfo(beBaseInfoManager* mgr, beBaseInfo* info) {
    igMetaObject* meta = V(mgr)->getInfoMeta();

    if (meta == ((igObjectV*)info)->getMeta()) {
        s32 n;
        s32 i;

        func_00301E30(mgr->_infoList, info);
        V(mgr)->onAddInfo(info);
        n = info->_ramList->_count;
        for (i = 0; i < n; i++) {
            beBaseInfoRam* ram = ((beBaseInfoRam**)info->_ramList->_data)[i];

            SetState(ram, 0);
            ram->_parentManager = mgr;
            ram->_parentInfo = info;
            ram->_ramNo = i;
            V(mgr)->entry(ram);
        }
        func_00236BE0(mgr->_insight, mgr);
        mgr->_isUpdateRun = true;
        return true;
    }
    return false;
}

void beBaseInfoManager_entry(beBaseInfoManager* mgr) {
}

/* Drop all infos (their rams lose their manager). */
void beBaseInfoManager_removeAllInfos(beBaseInfoManager* mgr) {
    s32 n;
    s32 i;

    V(mgr)->prepare();
    n = mgr->_infoList->_count;
    for (i = 0; i < n; i++) {
        beBaseInfoRamList* rams = ((beBaseInfo**)mgr->_infoList->_data)[i]->_ramList;
        s32 c = rams->_count;
        s32 j;

        for (j = 0; j < c; j++) {
            beBaseInfoRam* ram = ((beBaseInfoRam**)rams->_data)[j];

            ram->_parentManager = NULL;
            ram->_parentInfo = NULL;
        }
    }
    ObjListClear(mgr->_infoList);
    if ((mgr->_infoList->_count > 0) == false) {
        func_002D6680(mgr->_infoList, 0, 4);
    }
    func_00236C40(mgr->_insight, mgr);
    mgr->_isUpdateRun = false;
}

/* Every frame (unless skipped): run every ram. */
void beBaseInfoManager_update(beBaseInfoManager* mgr) {
    if (beSystem_isRunning(mgr->_system) && mgr->_isUpdateRun && !mgr->_system->_isFrameSkip) {
        s32 n = mgr->_infoList->_count;
        s32 i;

        for (i = 0; i < n; i++) {
            beBaseInfoRamList* rams = ((beBaseInfo**)mgr->_infoList->_data)[i]->_ramList;
            s32 c = rams->_count;
            s32 j;

            for (j = 0; j < c; j++) {
                V(mgr)->updateRam(((beBaseInfoRam**)rams->_data)[j]);
            }
        }
    }
}

igMetaObject* beBaseInfoManager_getMeta(void) {
    return beBaseInfoManager_Meta;
}

}
