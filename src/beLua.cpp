#include "game.h"

/* beLua: Lua script manager and its state (beLuaState) (MWCC C++). */
extern "C" {

extern igMetaObject* beLua_Meta;
extern igMetaObject* beLuaInfo_Meta;
extern igMetaObject* beLuaState_Meta;
extern igMetaObject* beSystem_Meta;
extern bool D_00554C14;
extern void* D_00587110;
void func_00236BE0(igInsightCore* insight, void* o);
void func_00236C40(igInsightCore* insight, void* o);
void func_002ED4C0(void* buffer, void* pool); /* free */
void igLuaState_userRelease(igLuaState* s);
void __dl__FPv(void* p);
s32 igLuaState_runString(igLuaState* s, const char* str);
s32 igLuaState_runBuffer(igLuaState* s, const char* buf, s32 size, const char* name);

}

static inline void* GetPool(void* o) {
    void* a = func_00301000(o);

    return D_00554C14 ? a : func_002F88E0(D_00587110);
}

extern "C" {

/* -- */

void beLua_registerToInsight(beLua* mgr) {
    func_00236BE0(mgr->_insight, mgr);
}

void beLua_unRegisterFromInsight(beLua* mgr) {
    func_00236C40(mgr->_insight, mgr);
}

void beLua_begin(beLua* mgr) {
    mgr->_system = (beSystem*)func_00236F80(mgr->_insight, beSystem_Meta);
    igUnref(mgr->_luaState);
    mgr->_luaState = NULL;
}

void beLua_end(beLua* mgr) {
}

bool beLua_appendInfo(beLua* mgr, igObject* info) {
    if (func_00300CD0(info, beLuaInfo_Meta)) {
        func_00301E30((igObjectList*)mgr->_infoList, info);
        return true;
    }
    return false;
}

void beLua_removeAllInfos(beLua* mgr) {
    beBaseInfoList* list;
    s32 n;
    s32 i;

    list = mgr->_infoList;
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
    igUnref(mgr->_luaState);
    mgr->_luaState = NULL;
}

void beLua_update(beLua* mgr) {
}

igMetaObject* beLua_getMeta(void) {
    return beLua_Meta;
}

void beLuaState_userRelease(beLuaState* s) {
    if (s->_lp != NULL) {
        func_002ED4C0(s->_lp, GetPool(s));
    }
    igLuaState_userRelease(s);
}

s32 func_0038A6C0(beLuaState* s, const char* str) {
    return igLuaState_runString(s, str);
}

INCLUDE_ASM("asm/nonmatchings/beLua", func_0038A6E0);

void** func_0038AB70(void** arg0, s16 arg1) {
    igObject* o;

    if (arg0 != NULL) {
        o = (igObject*)*arg0;
        igUnref(o);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void** func_0038ABF0(void** arg0, s16 arg1) {
    igObject* o;

    if (arg0 != NULL) {
        o = (igObject*)*arg0;
        igUnref(o);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

s32 func_0038AC70(beLuaState* s, const char* buf, s32 size, const char* name) {
    return igLuaState_runBuffer(s, buf, size, name);
}

igMetaObject* beLuaState_getMeta(void) {
    return beLuaState_Meta;
}

}
