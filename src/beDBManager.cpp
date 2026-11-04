#include "alchemy.h"

/* beDBManager: named data objects (beDataObj*) and database tables (beDataBase)
   (MWCC C++). */
extern "C" {

extern igMetaObject* D_00585BE0;          /* meta used for lookups by name */
extern igMetaObject* beDBManagerInfo_Meta;
extern char D_004E0858[];                 /* "" */
extern char D_005102A0[];                 /* "DATABASE" */
igNamedObject* func_003022C0(igObjectList* list, igMetaObject* meta, const char* name); /* find by name */
void beMessenger_entryRam(beMessenger* m, beBaseInfoRam* ram, const char* group);
void func_00236C40(igInsightCore* insight, void* o);
igObject* func_00366070(void); /* message table */
s32 beMessenger_getMessageIndex(beMessenger* m, igObject* table, beMessengerWork* work); /* message index */
void beMessenger_pushObject(beMessenger* m, igObject* v);
s32 beDataBase_getIndex(beDataBase* db, s32 record, const char* field, igNamedObjectList* columns);
extern igMetaObject* beDBManager_Meta;

struct ObjKey {
    igObject* p;
};

static inline s32 IndexOf(igObjectList* list, igObject* const& o) {
    s32 n = list->_count;
    s32 i;

    for (i = 0; i < n; i++) {
        if (o == ((igObject**)list->_data)[i]) {
            return i;
        }
    }
    return -1;
}

/* -- */

beDataObjInt* beDataObjList_getInt(beDataObjList* list, const char* name) {
    return (beDataObjInt*)func_003022C0(list, D_00585BE0, name);
}

beDataObjIntList* beDataObjList_getIntList(beDataObjList* list, const char* name) {
    return (beDataObjIntList*)func_003022C0(list, D_00585BE0, name);
}

beDataObjBool* beDataObjList_getBool(beDataObjList* list, const char* name) {
    return (beDataObjBool*)func_003022C0(list, D_00585BE0, name);
}

beDataObjBoolList* beDataObjList_getBoolList(beDataObjList* list, const char* name) {
    return (beDataObjBoolList*)func_003022C0(list, D_00585BE0, name);
}

beDataObjString* beDataObjList_getString(beDataObjList* list, const char* name) {
    return (beDataObjString*)func_003022C0(list, D_00585BE0, name);
}

beDataObjStringList* beDataObjList_getStringList(beDataObjList* list, const char* name) {
    return (beDataObjStringList*)func_003022C0(list, D_00585BE0, name);
}

beDataObjObjectList* beDataObjList_getObjectList(beDataObjList* list, const char* name) {
    return (beDataObjObjectList*)func_003022C0(list, D_00585BE0, name);
}

beDataObjFloatList* beDataObjList_getFloatList(beDataObjList* list, const char* name) {
    return (beDataObjFloatList*)func_003022C0(list, D_00585BE0, name);
}

/* Nonmatching: the original keeps the found column in a stack slot for the
   search loop (sw/lw 0x4C(sp), frame 0x50) and adds i + n * record. */
#ifdef NON_MATCHING
/* Index of field in record of a column group (-1 if the table has no such column). */
s32 beDataBase_getIndex(beDataBase* db, s32 record, const char* field, igNamedObjectList* columns) {
    s32 n = columns->_count;
    igNamedObject* col;

    if (n == 0) {
        return -1;
    }
    col = func_003022C0(columns, D_00585BE0, field);
    if (col == NULL) {
        return -1;
    }
    return IndexOf(columns, (igObject*)col) + n * record;
}
#else
INCLUDE_ASM("asm/nonmatchings/beDBManager", beDataBase_getIndex);
#endif

s32 beDataBase_getInt(beDataBase* db, s32 record, const char* field) {
    s32 i;

    i = beDataBase_getIndex(db, record, field, db->_charOffsetList);
    if (i != -1) {
        s8* p = (s8*)db->_charData->_data;

        return p[i];
    }
    i = beDataBase_getIndex(db, record, field, db->_shortOffsetList);
    if (i != -1) {
        return ((s16*)db->_shortData->_data)[i];
    }
    i = beDataBase_getIndex(db, record, field, db->_intOffsetList);
    if (i != -1) {
        return ((s32*)db->_intData->_data)[i];
    }
    return 0;
}

bool beDataBase_getBool(beDataBase* db, s32 record, const char* field) {
    s32 i = beDataBase_getIndex(db, record, field, db->_boolOffsetList);
    bool* p = (bool*)db->_boolData->_data;

    return p[i];
}

const char* beDataBase_getString(beDataBase* db, s32 record, const char* field) {
    s32 i = beDataBase_getIndex(db, record, field, db->_stringOffsetList);
    char* s = ((igNamedObject**)db->_stringData->_data)[i]->_name;

    return s ? s : D_004E0858;
}

/* Number of records: values per column group / columns in it. */
s32 beDataBase_getCount(beDataBase* db) {
    if (db->_stringOffsetList->_count != 0) {
        return db->_stringData->_count / db->_stringOffsetList->_count;
    }
    if (db->_charOffsetList->_count != 0) {
        return db->_charData->_count / db->_charOffsetList->_count;
    }
    if (db->_shortOffsetList->_count != 0) {
        return db->_shortData->_count / db->_shortOffsetList->_count;
    }
    if (db->_intOffsetList->_count != 0) {
        return db->_intData->_count / db->_intOffsetList->_count;
    }
    if (db->_floatOffsetList->_count != 0) {
        return db->_floatData->_count / db->_floatOffsetList->_count;
    }
    if (db->_boolOffsetList->_count != 0) {
        return db->_boolData->_count / db->_boolOffsetList->_count;
    }
    return 0;
}

void beDBManager_prepare(void) {
}

igMetaObject* beDBManagerInfo_getMeta(void) {
    return beDBManagerInfo_Meta;
}

void beDBManager_entry(beDBManager* mgr, beBaseInfoRam* ram) {
    beMessenger_entryRam(mgr->_messenger, ram, D_005102A0);
}

void beDBManager_setup(beDBManager* mgr) {
    func_00236C40(mgr->_insight, mgr);
}

/* "DATABASE" handler: returns the info's database. */
bool beDBManager_handle(beDBManager* mgr, beBaseInfoRam* ram, beMessengerWork* work) {
    beDBManagerInfo* info = (beDBManagerInfo*)ram->_parentInfo;

    switch (beMessenger_getMessageIndex(mgr->_messenger, func_00366070(), work)) {
    case 0:
        beMessenger_pushObject(mgr->_messenger, info->_dataBase);
        return true;
    }
    return false;
}

igMetaObject* beDBManager_getMeta(void) {
    return beDBManager_Meta;
}

}
