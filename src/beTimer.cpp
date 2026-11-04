#include "game.h"

/* beTimer: count-down timers that call a Lua file when they run out
   (MWCC C++). */
extern "C" {

extern igMetaObject* beSystem_Meta;
extern igMetaObject* bePadManager_Meta;
extern igMetaObject* beLua_Meta;
extern igMetaObject* beTimer_Meta;
extern char D_0050FF10[]; /* "NOPADDATA" */
bool beSystem_isRunning(beSystem* sys);                  /* active and not first time */
bePadData* bePadManager_getPad(bePadManager* pads, s32 i); /* pad data of a port */
void func_00389FF0(beLua* lua, const char* name, s32);
bool beTimer_isPadPressed(beTimer* t);

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

void beTimer_registerToInsight(beTimer* t) {
}

void beTimer_unRegisterFromInsight(beTimer* t) {
}

/* Look up the managers it uses. */
void beTimer_begin(beTimer* t) {
    t->_system = (beSystem*)func_00236F80(t->_insight, beSystem_Meta);
    t->_padManager = (bePadManager*)func_00236F80(t->_insight, bePadManager_Meta);
    t->_luaManager = (beLua*)func_00236F80(t->_insight, beLua_Meta);
}

void beTimer_end(beTimer* t) {
}

s32 beTimer_appendInfo(beTimer* t) {
    return 0;
}

/* Drop all timers. */
void beTimer_removeAllInfos(beTimer* t) {
    ObjListClear(t->_dataList);
}

/* Stick at rest. */
static inline bool IsFree(bePadData* p) {
    return !(p->_lStk != 0);
}

/* Any stick or button held on any of the 4 pads. */
bool beTimer_isPadPressed(beTimer* t) {
    s32 i;

    for (i = 0; i < 4; i++) {
        bePadData* p = bePadManager_getPad(t->_padManager, i);

        if (!IsFree(p)) {
            return true;
        }
        if (p->_button != 0) {
            return true;
        }
    }
    return false;
}

/* Count the timers down by the world time since the last frame; a timer with
   reset condition "NOPADDATA" restarts while a pad is used. When one runs out
   its Lua file is called and it is removed. */
void beTimer_update(beTimer* t) {
    s64 elapsed;
    s32 n;
    s32 i;

    if (t->_dataList->_count == 0) {
        return;
    }
    {
        s64 now = t->_insight->_worldTime;

        elapsed = now - t->_oldWorldTime;
        t->_oldWorldTime = now;
    }
    if (beSystem_isRunning(t->_system) == false) {
        return;
    }
    n = t->_dataList->_count;
    for (i = 0; i < n; i++) {
        beTimerData* d = ((beTimerData**)t->_dataList->_data)[i];

        if (d->_resetCondition != NULL) {
            bool reset = strcmp(d->_resetCondition, D_0050FF10) == 0 && beTimer_isPadPressed(t);

            if (reset) {
                d->_downTimer = d->_startTime;
            }
        }
        if ((d->_downTimer -= elapsed) < 0) {
            beTimerDataList* list;

            func_00389FF0(t->_luaManager, d->_callLuaFile, 0);
            list = t->_dataList;
            igUnref(((igObject**)list->_data)[i]);
            func_002D6CF0(list, i);
            ((igObject**)list->_data)[list->_count] = NULL;
            n--;
            i--;
        }
    }
}

igMetaObject* beTimer_getMeta(void) {
    return beTimer_Meta;
}

}
