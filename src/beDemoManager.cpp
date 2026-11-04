#include "game.h"

/* beDemoManager: demo (cutscene) works (MWCC C++). */
extern "C" {

extern igMetaObject* beDemoManager_Meta;
extern igMetaObject* beMessenger_Meta;
extern igMetaObject* beLua_Meta;
void func_00236BE0(igInsightCore* insight, void* o);
void func_00236C40(igInsightCore* insight, void* o);

/* -- */

void beDemoManager_registerToInsight(beDemoManager* mgr) {
    func_00236BE0(mgr->_insight, mgr);
}

void beDemoManager_unRegisterFromInsight(beDemoManager* mgr) {
    func_00236C40(mgr->_insight, mgr);
}

void beDemoManager_begin(beDemoManager* mgr) {
    mgr->_messenger = (beMessenger*)func_00236F80(mgr->_insight, beMessenger_Meta);
    mgr->_lua = (beLua*)func_00236F80(mgr->_insight, beLua_Meta);
}

void beDemoManager_end(beDemoManager* mgr) {
}

s32 beDemoManager_appendInfo(beDemoManager* mgr) {
    return 0;
}

void beDemoManager_removeAllInfos(beDemoManager* mgr) {
    beDemoManagerWorkList* list;
    s32 n;
    s32 i;

    if (mgr->_disableRemoveAll == true) {
        mgr->_calledRemoveAll = true;
        return;
    }
    list = mgr->_activeDataList;
    n = list->_count;
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
    if ((mgr->_activeDataList->_count > 0) == false) {
        func_002D6680(mgr->_activeDataList, 0, 4);
    }
    igUnref(mgr->_buildData);
    mgr->_buildData = NULL;
    mgr->_turningOK = false;
    mgr->_calledRemoveAll = false;
}

INCLUDE_ASM("asm/nonmatchings/beDemoManager", beDemoManager_update);

igMetaObject* beDemoManager_getMeta(void) {
    return beDemoManager_Meta;
}

}
