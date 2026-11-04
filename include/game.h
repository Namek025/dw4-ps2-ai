#ifndef GAME_H
#define GAME_H

/* Prototypes of the game functions other units call (MWCC C++ units). */

#include "alchemy.h"

extern "C" {

/* src/beMessenger.cpp */
void beMessenger_entryRam(beMessenger* m, beBaseInfoRam* ram, const char* group);
void beMessenger_select(beMessenger* m, const char* group, const char* info, s32 ramNo);
void beMessenger_selectRamNo(beMessenger* m, const char* group, s32 ramNo);
void beMessenger_selectGroup(beMessenger* m, const char* group);
void beMessenger_send(beMessenger* m, const char* message);
void beMessenger_call(beMessenger* m, const char* group, const char* info, s32 ramNo, const char* message);
void beMessenger_callRamNo(beMessenger* m, const char* group, s32 ramNo, const char* message);
void beMessenger_callGroup(beMessenger* m, const char* group, const char* message);
void beMessenger_delaySelect(beMessenger* m, const char* group, const char* info, s32 ramNo);
void beMessenger_delaySend(beMessenger* m, const char* message, s64 delay);
void beMessenger_push(beMessenger* m);
void beMessenger_pop(beMessenger* m);
s32 beMessenger_getMessageIndex(beMessenger* m, igObject* table, const char* name);
s32 beMessenger_getBoolCount(beMessenger* m);
void beMessenger_pushBool(beMessenger* m, bool v);
void beMessenger_clearBools(beMessenger* m);
bool beMessenger_getBool(beMessenger* m, s32 i);
s32 beMessenger_getIntCount(beMessenger* m);
void beMessenger_pushInt(beMessenger* m, s32 v);
void beMessenger_clearInts(beMessenger* m);
s32 beMessenger_getInt(beMessenger* m, s32 i);
void beMessenger_pushFloat(beMessenger* m, f32 v);
f32 beMessenger_getFloat(beMessenger* m, s32 i);
void beMessenger_pushVec3f(beMessenger* m, const igVec3f* v);
void beMessenger_pushMatrix(beMessenger* m, const igMatrix44f* v);
s32 beMessenger_getObjectCount(beMessenger* m);
void beMessenger_pushObject(beMessenger* m, igObject* v);
igObject* beMessenger_getObject(beMessenger* m, s32 i);
s32 beMessenger_getStringCount(beMessenger* m);
void beMessenger_pushString(beMessenger* m, const char* s);
char* beMessenger_getString(beMessenger* m, s32 i);

/* src/beDBManager.cpp */
beDataObjInt* beDataObjList_getInt(beDataObjList* list, const char* name);
beDataObjIntList* beDataObjList_getIntList(beDataObjList* list, const char* name);
beDataObjBool* beDataObjList_getBool(beDataObjList* list, const char* name);
beDataObjBoolList* beDataObjList_getBoolList(beDataObjList* list, const char* name);
beDataObjString* beDataObjList_getString(beDataObjList* list, const char* name);
beDataObjStringList* beDataObjList_getStringList(beDataObjList* list, const char* name);
beDataObjObjectList* beDataObjList_getObjectList(beDataObjList* list, const char* name);
beDataObjFloatList* beDataObjList_getFloatList(beDataObjList* list, const char* name);
s32 beDataBase_getInt(beDataBase* db, s32 record, const char* field);
bool beDataBase_getBool(beDataBase* db, s32 record, const char* field);
const char* beDataBase_getString(beDataBase* db, s32 record, const char* field);
s32 beDataBase_getCount(beDataBase* db);

/* Alchemy helpers used by the game */
void func_00301E30(igObjectList* list, igObject* o);     /* append, with a reference */
void* func_00301000(void* o);                            /* memory pool of an object */
void* func_002F88E0(void* p);
void func_002FECB0(igNamedObject* o, const char* name);  /* set name */
void func_001CA620(igStringRefList* list, char** str);   /* append a pooled string */
void func_001C64E0(char** out, igStringRefList* list, s32 index); /* pooled string by value */
void func_002D6CF0(igDataList* list, s32 index);         /* remove */
void func_002D6B80(igDataList* list, s32 index, s32 n, void* src); /* insert */
void func_002D6680(igDataList* list, s32 capacity, s32 elemSize);  /* set capacity */
void* func_00236F80(igInsightCore* insight, igMetaObject* meta);   /* manager of that class */

s32 strcmp(const char* a, const char* b);
char* strcpy(char* dst, const char* src);
s32 sprintf(char* buf, const char* fmt, ...);

}

/* Append a pooled copy of s (an igStringRef temporary, made before the list
   is read). */
#define APPEND_STR(list, s)                                                    \
    {                                                                          \
        char* str_ = (s) == NULL ? NULL : func_002EAF60(GetStrPool(), (s));    \
                                                                               \
        func_001CA620((list), &str_);                                          \
        ReleaseStr(str_);                                                      \
    }

template <class L>
static inline void ClearStrList(L* list) {
    s32 n = list->_count;
    s32 i;

    for (i = 0; i < n; i++) {
        char** p = &((char**)list->_data)[i];
        char* str = func_002EAF60(GetStrPool(), NULL);

        ReleaseStr(*p);
        *p = str;
    }
    list->_count = 0;
}

#endif /* GAME_H */
