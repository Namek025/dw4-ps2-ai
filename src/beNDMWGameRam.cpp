#include "game.h"

/* beNDMWGameRam: the "GAMERAM" message handler (player data, stats, fight
   calculations) and the helpers around it (MWCC C++). */
extern "C" {

beDataObjInt* beDataObjList_getInt(beDataObjList* list, const char* name);
beDataObjBool* beDataObjList_getBool(beDataObjList* list, const char* name);
beDataObjObjectList* beDataObjList_getObjectList(beDataObjList* list, const char* name);
s32 func_00447C10(beNDMWItemRevisionBase* item, beNDMWGameRam* gr, const char* param); /* item parameter */
const char* func_00447CB0(beNDMWItemRevisionBase* item, beNDMWGameRam* gr, const char* param); /* string parameter */
s32 strcmp(const char* a, const char* b);
extern char D_00518A88[]; /* "POWERSOCKET" */
extern char D_00518AB0[]; /* "POWERPOCKET" */
extern char D_00518AC0[]; /* "MAINSLOT" */
extern char D_00518AD0[]; /* "MAINPOCKET" */
extern char D_00518AE0[]; /* "SUBSLOT" */
extern char D_00518C88[]; /* "SHUSEINOURYOKU" */
extern char D_00518928[]; /* "WAZALIST" */
extern char D_00518888[]; /* "ISUSE" */
extern char D_00518760[]; /* "NINZU" */

/* Player data lists, one per player (_playerDataListList). */
static inline beDataObjList* PlayerData(beNDMWGameRam* gr, s32 player) {
    return ((beDataObjList**)gr->_playerDataListList->_data)[player];
}

extern char D_00518A40[]; /* "DEVICEFOLDER" */
extern char D_00518848[]; /* "DIGIMONNO" */
extern char D_00518860[]; /* "SKILL" */
extern char D_00518B50[]; /* "BASE_SKILL" */
extern char D_00518C10[]; /* "SOUBI" */
extern char D_00518C18[]; /* "SKILLPOINT" */
extern char D_00518B20[]; /* "BASE_ISUSE" */
extern char D_00518898[]; /* "MHP" */
extern char D_00518890[]; /* "HP" */
extern char D_00518C28[]; /* "METERCTRL" */
extern char D_00518C38[]; /* "hpmt" */
extern char D_00518C40[]; /* "SETPERCENT" */
extern char D_00518C50[]; /* "NUMBERCTRL" */
extern char D_00518C60[]; /* "hpnm" */
extern char D_00518C68[]; /* "SETVALUE" */
extern char D_005188A8[]; /* "MMP" */
extern char D_005188A0[]; /* "MP" */
extern char D_00518C78[]; /* "mpmt" */
extern char D_00518C80[]; /* "mpnm" */
beDataObjIntList* beDataObjList_getIntList(beDataObjList* list, const char* name);
beDataObjBoolList* beDataObjList_getBoolList(beDataObjList* list, const char* name);
void beMessenger_select(beMessenger* m, const char* group, const char* info, s32 ramNo);
void beMessenger_send(beMessenger* m, const char* message);
void beMessenger_pushFloat(beMessenger* m, f32 v);
void beMessenger_pushInt(beMessenger* m, s32 v);
s32 func_003F80E0(beNDMWGameRam* gr, const char* name);
void beNDMWGameRam_base2Current(beNDMWGameRam* gr, s32 player, s32 digimon);
void beNDMWGameRam_giveStartWeapons(beNDMWGameRam* gr, s32 player);
void beNDMWGameRam_equipPowerSocket(beNDMWGameRam* gr, s32 player, bool strong);
void beNDMWGameRam_recalcPlayer(beNDMWGameRam* gr, s32 player, bool equip);

static inline void SetTrue(beDataObjBool* b) {
    b->_data = true;
}

static inline void BoolListSet(igDataList* list, s32 i, const bool& v) {
    if (list->_count != 0 && i >= 0 && i < list->_count) {
        ((bool*)list->_data)[i] = v;
    }
}

extern char D_00519438[]; /* "DATABASE" */
extern char D_00519430[]; /* "shinka" */
extern char D_00519420[]; /* "GETDATABASE" */
extern char D_00518BC8[]; /* "MODEL_NAME" */
extern char D_00518BB8[]; /* "gameram" */
extern char D_00518BC0[]; /* "GAMERAM" */
extern void* D_00587108;  /* default memory pool (fallback) */
const char* beDataBase_getString(beDataBase* db, s32 record, const char* field);
igObject* beMessenger_getObject(beMessenger* m, s32 i);
void beMessenger_entryRam(beMessenger* m, beBaseInfoRam* ram, const char* group);
void func_00301E30(igObjectList* list, igObject* o); /* append, with a reference */
void* func_00301000(void* o);                        /* memory pool of an object */
void* func_002F88E0(void* p);
void func_002FECB0(igNamedObject* o, const char* name); /* set name */
void beBaseInfoManager_begin(beNDMWGameRam* gr);               /* beBaseInfoManager init */
beDataObjList* func_00364050(void* pool);            /* new beDataObjList */
beDataObjListList* func_00363E80(void* pool);        /* new beDataObjListList */
beNDMWGameRamInfo* func_003CA380(void* pool);        /* new beNDMWGameRamInfo */
beNDMWGameRamInfoRam* func_003CA1A0(void* pool);     /* new beNDMWGameRamInfoRam */
void beNDMWGameRam_initData(beNDMWGameRam* gr, s32 player);
void beNDMWGameRam_startPlayer(beNDMWGameRam* gr, s32 player, const char* digimonName);

/* Pooled string held by a smart pointer (released when it goes out of scope). */
struct igStringRef {
    char* p;

    igStringRef(const char* s) : p(s == NULL ? NULL : func_002EAF60(GetStrPool(), s)) {}
    ~igStringRef() { ReleaseStr(p); }
};

extern bool D_00554C14;

static inline void* PoolOf(void* o) {
    void* a = func_00301000(o);

    return D_00554C14 ? a : func_002F88E0(D_00587108);
}

extern void* D_00587110;

static inline void* PoolOf2(void* o) {
    void* a = func_00301000(o);

    return D_00554C14 ? a : func_002F88E0(D_00587110);
}

/* The "shinka" database, fetched through the messenger on first use. */
static inline beDataBase* GetDBShinka(beNDMWGameRam* gr) {
    if (gr->_dBShinka == NULL) {
        beMessenger_call(gr->_messenger, D_00519438, D_00519430, 0, D_00519420);
        {
            beDataBase* db = (beDataBase*)beMessenger_getObject(gr->_messenger, 0);

            igIncRef(db);
            igUnref(gr->_dBShinka);
            gr->_dBShinka = db;
        }
    }
    return gr->_dBShinka;
}

/* Set the state machine to st0 with the sub-states cleared (beBaseInfoRam and
   beNDMWWindow use the same _st0.._st3 layout). */
template <class T>
static inline void SetState(T* o, s32 st) {
    o->_st0 = st;
    o->_st3 = 0;
    o->_st2 = 0;
    o->_st1 = 0;
}

/* Pooled strings are unique: equal strings are the same pointer. MWCC evaluates
   call arguments right to left, so s.p is read before the lookup. */
static inline bool SameStr(const char* a, const char* b) {
    return a == b;
}

IG_REF(beDataObjList);
IG_REF(beDataObjListList);
IG_REF(beNDMWGameRamInfo);
IG_REF(beNDMWGameRamInfoRam);

extern char D_00518858[]; /* "LEVEL" */
extern char D_00518B30[]; /* "BASE_LEVEL" */
extern char D_00518880[]; /* "EXP" */
extern char D_00518B40[]; /* "BASE_EXP" */
extern char D_00518868[]; /* "UPCNT" */
extern char D_00518B60[]; /* "BASE_UPCNT" */
extern char D_00518870[]; /* "DIGIMONNAME" */
extern char D_00518AE8[]; /* "BASE_FLAG" */
extern char D_00518798[]; /* "FLAG" */
extern char D_00518B00[]; /* "BASE_FLAGFOLDER" */
extern char D_005187A0[]; /* "FLAGFOLDER" */
extern char D_00518B10[]; /* "BASE_COUNTER" */
extern char D_005187B0[]; /* "COUNTER" */

static inline void IntListSet(igDataList* list, s32 i, s32 v) {
    if (list->_count != 0 && i >= 0 && i < list->_count) {
        ((s32*)list->_data)[i] = v;
    }
}

static inline s32 IntListGet(igDataList* list, s32 i) {
    return ((s32*)list->_data)[i];
}

extern char D_00518C98[]; /* "MHP_INI" */
extern char D_00518CA0[]; /* "MHP_GROW" */
extern char D_00518CB0[]; /* "MMP_INI" */
extern char D_00518CB8[]; /* "MMP_GROW" */
extern char* D_004CF250[]; /* "STR_INI", "AC_INI", "INT_INI", "MND_INI", "DEX_INI" */
extern char* D_004CF270[]; /* "STR_GROW", ... "DEX_GROW" */
/* Growth curve: segment by level (D_004CF130 = segment ends), value at the
   segment start and slope per level, for HP/MP and for the 5 stats. */
extern s32 D_004CF130[]; /* 5, 10, 20, 30, 50, 70, 110, 150, 230, 999 */
extern s32 D_004CF160[]; /* HP/MP base */
extern s32 D_004CF190[]; /* segment start levels */
extern s32 D_004CF1C0[]; /* HP/MP slope */
extern s32 D_004CF1F0[]; /* stat base */
extern s32 D_004CF220[]; /* stat slope */

static inline s32 CurveSegment(s32 level) {
    s32 k = 0;
    s32* p = D_004CF130;

    for (;;) {
        if (*p >= level) {
            break;
        }
        p++;
        k++;
    }
    return k;
}

static inline s32 StatCurve(s32 level) {
    s32 k = CurveSegment(level);

    return D_004CF1F0[k] + D_004CF220[k] * (level - D_004CF190[k]);
}

static inline s32 HpMpCurve(s32 level) {
    s32 k = CurveSegment(level);

    return D_004CF160[k] + D_004CF1C0[k] * (level - D_004CF190[k]);
}

extern char D_00518828[]; /* "PLAYERNAME" */

/* Drop every object of an object list (keeping its capacity). */
static inline s32 GetCount(igDataList* l) {
    return l->_count;
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

extern char D_00519030[]; /* "GETMATRIX" */
extern char D_00519040[]; /* "GETHP" */
extern void* D_00587110;
igIntList* func_002CAA70(void* pool); /* new igIntList */
void beMessenger_getMatrix(igMatrix44f* out, beMessenger* m, s32 i);
void beMessenger_setRunCheck(beMessenger* m, bool v);
void func_00190A30(const igMatrix44f* m, igVec3f* pos); /* translation */

IG_REF(igIntList);

static inline void* PoolOf2(void* o);

extern char D_00518A40[]; /* "DEVICEFOLDER" */
extern char* D_004CF120[]; /* "INI_WEAPON1", "INI_WEAPON2", "INI_WEAPON3" */
beNDMWItemWeapon* func_003C8C90(void* pool); /* new beNDMWItemWeapon */
void func_004482C0(beNDMWItemWeapon* item, s32 recNo, s32, s32);

IG_REF(beNDMWItemWeapon);

extern char D_00518AA0[]; /* "POWERSOCKETNAME" */
extern char D_00518C08[]; /* "MODEL" */
extern char D_005188B0[]; /* "TSUYOSA" */
void func_00301C90(igObjectList* list, igObject* o); /* fill with o */

template <class L>
static inline void ObjListSet(L* list, s32 i, igObject* o) {
    igIncRef(o);
    igUnref(((igObject**)list->_data)[i]);
    if (list->_count != 0 && i >= 0 && i < list->_count) {
        ((igObject**)list->_data)[i] = o;
    }
}

/* One entry of a data schema table (D_004CED40, D_004CEDF0). */
typedef struct {
    s32 type; /* 0 int, 1 int list, 2 bool, 3 bool list, 4 string, 5 string list, 6 object list, 7 float list, -1 end */
    s32 size;
    const char* name;
} DataDesc;

extern const s32 D_004CF100; /* 0 */
extern const f32 D_004CF108; /* 0.0f */
beDataObjInt* func_00365510(void* pool);
beDataObjIntList* func_00365240(void* pool);
beDataObjBool* func_00365000(void* pool);
beDataObjBoolList* func_00364D30(void* pool);
beDataObjString* func_00364A90(void* pool);
beDataObjStringList* func_003647C0(void* pool);
beDataObjObjectList* func_003644F0(void* pool);
beDataObjFloatList* func_00364220(void* pool);
void func_0030CDF0(igStringRefList* list, s32 n); /* set count */
void func_00301EE0(igObjectList* list, s32 n);    /* set count */

IG_REF(beDataObjInt);
IG_REF(beDataObjIntList);
IG_REF(beDataObjBool);
IG_REF(beDataObjBoolList);
IG_REF(beDataObjString);
IG_REF(beDataObjStringList);
IG_REF(beDataObjObjectList);
IG_REF(beDataObjFloatList);

/* Make room for n elements. */
template <class L>
static inline void ListReserve(L* list, s32 n, s32 elemSize) {
    if ((n < list->_count) == false) {
        func_002D6680(list, n, elemSize);
    }
}

template <class L>
static inline void ListSetCount(L* list, s32 n, s32 elemSize) {
    if (n >= 0) {
        if (n <= list->_capacity) {
            list->_count = n;
        } else {
            func_002D6790(list, n, elemSize);
        }
    }
}

template <class L, class T>
static inline void ListFill(L* list, const T& v) {
    s32 n = list->_count;
    T* p = (T*)list->_data;
    s32 i;

    for (i = 0; i < n; i++) {
        p[i] = v;
    }
}

extern char D_00518B70[]; /* "UPDATA" */
extern char D_00518FC8[]; /* "MAXHP" */
extern char D_00518FD0[]; /* "MAXMP" */
extern char D_00518B78[]; /* "DOWNDATA" */
extern char D_005188B8[]; /* "MAMORI" */
extern char D_005188C0[]; /* "KASHIKOSA" */
extern char D_005188D0[]; /* "SEISHIN" */
extern char D_005188D8[]; /* "SUBAYASA" */
extern char D_00518FD8[]; /* "RES_FIRE" */
extern char D_005188E8[]; /* "HONOOTEIKO" */
extern char D_00518FE8[]; /* "RES_ICE" */
extern char D_005188F8[]; /* "KOORITEIKO" */
extern char D_00518FF0[]; /* "RES_THUNDER" */
extern char D_00518908[]; /* "KAMINARITEIKO" */
extern char D_00519000[]; /* "RES_DARK" */
extern char D_00518918[]; /* "YAMITEIKO" */
extern char D_00519460[]; /* "DATABASE" */
extern char D_00519458[]; /* "pcTech" */
extern char D_00519448[]; /* "GETDATABASE" */
extern char D_00518938[]; /* "NEXTEXP" */
extern char D_00519010[]; /* "DELAY" */
extern char D_00518940[]; /* "ATTACKDELAY" */
extern char D_00518950[]; /* "MEICHURITSU" */
extern char D_00518960[]; /* "CHIMEIRITSU" */
extern char D_00518970[]; /* "MAHIRITSU" */
extern char D_00518A10[]; /* "CHOHATURITSU" */
extern char D_00518980[]; /* "KAIHIRITSU" */
extern char D_00518990[]; /* "BOUGYORITSU" */
extern char D_005189A0[]; /* "MAHITEIKOU" */
extern char D_005189B0[]; /* "DOKUTEIKOU" */
extern char D_005189C0[]; /* "BITRITSU" */
extern char D_005189D0[]; /* "EXPRITSU" */
extern char D_005189E0[]; /* "LUCKRITSU" */
extern char D_005189F0[]; /* "ATTACKZOKUSEI" */
extern char D_00518AB0[]; /* "POWERPOCKET" */
extern char D_00519018[]; /* "ZOKUSEI" */
extern char D_00518A00[]; /* "HPDRAIN" */
extern char D_00518A08[]; /* "MPDRAIN" */
extern char D_00518A20[]; /* "WEAPONRECNO" */
extern char D_00518A30[]; /* "WEAPONSKILLNO" */
extern char* D_004CF290[]; /* "TECH1".."TECH6" */
extern char* D_004CF2B0[]; /* "TECH1_LEVEL".."TECH6_LEVEL" */
extern char* D_004CF2D0[][5]; /* [TECHn][SKILLr]: "TECH1_SKILL1".."TECH6_SKILL5" */
extern char* D_004CF350[]; /* "fire", "ice", "thunder", "dark" */
beNDMWItemWaza* func_003C8040(void* pool); /* new beNDMWItemWaza */
void func_00448F30(beNDMWItemWaza* w, s32 recNo, s32 kindNo);
s32 beNDMWGameRam_calcHpMpBase(beNDMWGameRam* gr, s32 player, s32 digimon, bool hp);
s32 beNDMWGameRam_calcStatBase(beNDMWGameRam* gr, s32 player, s32 digimon, s32 stat);
void beNDMWGameRam_addPowerSocket(beNDMWGameRam* gr, beDataObjList* pl, u32* sum, const char* param, bool addRevision);
void beNDMWGameRam_addPowerPocket(beNDMWGameRam* gr, beDataObjList* pl, u32* sum, const char* param);
void beNDMWGameRam_addMainSlot(beNDMWGameRam* gr, beDataObjList* pl, u32* sum, const char* param, bool addRevision);
void beNDMWGameRam_addMainPocket(beNDMWGameRam* gr, beDataObjList* pl, u32* sum, const char* param);
void beNDMWGameRam_addSubSlot(beNDMWGameRam* gr, beDataObjList* pl, u32* sum, const char* param, bool addRevision);

IG_REF(beNDMWItemWaza);

static inline beDataBase* GetDBPcTech(beNDMWGameRam* gr) {
    if (gr->_dBPcTech == NULL) {
        beMessenger_call(gr->_messenger, D_00519460, D_00519458, 0, D_00519448);
        {
            beDataBase* db = (beDataBase*)beMessenger_getObject(gr->_messenger, 0);

            igIncRef(db);
            igUnref(gr->_dBPcTech);
            gr->_dBPcTech = db;
        }
    }
    return gr->_dBPcTech;
}

static inline s32 UpData(beDataObjList* pl, s32 i) {
    return IntListGet(beDataObjList_getIntList(pl, D_00518B70)->_data, i);
}

static inline s32 DownData(beDataObjList* pl, s32 i) {
    return IntListGet(beDataObjList_getIntList(pl, D_00518B78)->_data, i);
}

static inline s32 UpCnt(beDataObjList* pl, s32 i) {
    return IntListGet(beDataObjList_getIntList(pl, D_00518868)->_data, i);
}

/* -- */

/* Fill a string list with s (one reference per slot). */
static inline void StrListFill(igDataList* l, const igStringRef& s) {
    s32 n = l->_count;
    char** p = (char**)l->_data;
    s32 i;

    for (i = 0; i < n; i++) {
        if (s.p != NULL) {
            ((StrHeader*)s.p - 1)->refs++;
        }
        ReleaseStr(*p);
        *p = s.p;
        p++;
    }
}

/* Build named data objects from a schema table into list. The table is walked by
   index (desc[k]): MWCC turns it into a pointer of its own, which gets the last
   s-register. The closing count is read volatile (else count < count folds). */
void beNDMWGameRam_buildData(beNDMWGameRam* gr, beDataObjList* list, DataDesc* desc) {
    bool done = false;
    s32 k = 0;

    do {
        switch (desc[k].type) {
        case 0: {
            beDataObjIntRef o(func_00365510(PoolOf(gr)), IG_ADOPT);

            func_002FECB0(o, desc[k].name);
            func_00301E30(list, o);
            break;
        }
        case 1: {
            beDataObjIntListRef o(func_00365240(PoolOf(gr)), IG_ADOPT);

            func_002FECB0(o, desc[k].name);
            ListReserve(o->_data, desc[k].size, 4);
            ListSetCount(o->_data, desc[k].size, 4);
            {
                s32 n = o->_data->_count;
                s32* p = (s32*)o->_data->_data;
                s32 i;

                for (i = 0; i < n; i++) {
                    p[i] = D_004CF100;
                }
            }
            func_00301E30(list, o);
            break;
        }
        case 2: {
            beDataObjBoolRef o(func_00365000(PoolOf(gr)), IG_ADOPT);

            func_002FECB0(o, desc[k].name);
            func_00301E30(list, o);
            break;
        }
        case 3: {
            beDataObjBoolListRef o(func_00364D30(PoolOf(gr)), IG_ADOPT);

            func_002FECB0(o, desc[k].name);
            ListReserve(o->_data, desc[k].size, 1);
            ListSetCount(o->_data, desc[k].size, 1);
            ListFill(o->_data, false);
            func_00301E30(list, o);
            break;
        }
        case 4: {
            beDataObjStringRef o(func_00364A90(PoolOf(gr)), IG_ADOPT);

            func_002FECB0(o, desc[k].name);
            func_00301E30(list, o);
            break;
        }
        case 5: {
            beDataObjStringListRef o(func_003647C0(PoolOf(gr)), IG_ADOPT);

            func_002FECB0(o, desc[k].name);
            ListReserve(o->_data, desc[k].size, 4);
            func_0030CDF0(o->_data, desc[k].size);
            StrListFill(o->_data, igStringRef(NULL));
            func_00301E30(list, o);
            break;
        }
        case 6: {
            beDataObjObjectListRef o(func_003644F0(PoolOf(gr)), IG_ADOPT);

            func_002FECB0(o, desc[k].name);
            ListReserve(o->_data, desc[k].size, 4);
            func_00301EE0(o->_data, desc[k].size);
            func_00301C90(o->_data, NULL);
            func_00301E30(list, o);
            break;
        }
        case 7: {
            beDataObjFloatListRef o(func_00364220(PoolOf(gr)), IG_ADOPT);

            func_002FECB0(o, desc[k].name);
            ListReserve(o->_data, desc[k].size, 4);
            ListSetCount(o->_data, desc[k].size, 4);
            ListFill(o->_data, D_004CF108);
            func_00301E30(list, o);
            break;
        }
        case -1:
            done = true;
            break;
        }
        k++;
    } while (done == false);
    {
        igDataList* l = list;

        ListReserve(l, ((volatile igDataList*)list)->_count, 4);
    }
}

/* Out-of-line destructor of one of buildData's beDataObj*Ref smart pointers (deleting form). */
beDataObjFloatListRef* func_003F6F90(beDataObjFloatListRef* r, s16 flags) {
    if (r != NULL) {
        igUnref(r->p);
        if (flags > 0) {
            operator delete(r);
        }
    }
    return r;
}

/* Out-of-line destructor of buildData's beDataObjObjectListRef (deleting form). */
beDataObjObjectListRef* func_003F7010(beDataObjObjectListRef* r, s16 flags) {
    if (r != NULL) {
        igUnref(r->p);
        if (flags > 0) {
            operator delete(r);
        }
    }
    return r;
}

/* Out-of-line destructor of buildData's beDataObjStringListRef (deleting form). */
beDataObjStringListRef* func_003F7090(beDataObjStringListRef* r, s16 flags) {
    if (r != NULL) {
        igUnref(r->p);
        if (flags > 0) {
            operator delete(r);
        }
    }
    return r;
}

/* Out-of-line destructor of buildData's beDataObjBoolListRef (deleting form). */
beDataObjBoolListRef* func_003F7110(beDataObjBoolListRef* r, s16 flags) {
    if (r != NULL) {
        igUnref(r->p);
        if (flags > 0) {
            operator delete(r);
        }
    }
    return r;
}

/* Out-of-line destructor of a buildData smart pointer (deleting form); beDataObjBoolRef by
   reverse order of first use, the code is the same for every ref type. */
beDataObjBoolRef* func_003F7190(beDataObjBoolRef* r, s16 flags) {
    if (r != NULL) {
        igUnref(r->p);
        if (flags > 0) {
            operator delete(r);
        }
    }
    return r;
}

/* Out-of-line destructor of a buildData smart pointer (deleting form); same code for every ref type. */
beDataObjStringRef* func_003F7210(beDataObjStringRef* r, s16 flags) {
    if (r != NULL) {
        igUnref(r->p);
        if (flags > 0) {
            operator delete(r);
        }
    }
    return r;
}

/* Out-of-line destructor of buildData's beDataObjIntListRef (deleting form). */
beDataObjIntListRef* func_003F7290(beDataObjIntListRef* r, s16 flags) {
    if (r != NULL) {
        igUnref(r->p);
        if (flags > 0) {
            operator delete(r);
        }
    }
    return r;
}

extern DataDesc D_004CED40[]; /* common data schema */
extern DataDesc D_004CEDF0[]; /* per player data schema */
extern const s32 D_004CF110;  /* BASE_LEVEL start value */
extern const s32 D_004CF118;  /* BASE_SKILL start value */
extern char D_00518A50[];     /* "DISKFOLDER" */
beNDMWItemDisk* func_003C85B0(void* pool); /* new beNDMWItemDisk */
void func_00448710(beNDMWItemDisk* disk, s32 kind);
void beNDMWGameRam_buildData(beNDMWGameRam* gr, beDataObjList* list, DataDesc* desc);

IG_REF(beNDMWItemDisk);

/* ObjListSet with the object first (it is read before the list). */
template <class L>
static inline void ObjListSetObj(igObject* o, L* list, s32 i) {
    igIncRef(o);
    igUnref(((igObject**)list->_data)[i]);
    if (list->_count != 0 && i >= 0 && i < list->_count) {
        ((igObject**)list->_data)[i] = o;
    }
}

static inline void ObjListAppend(igObjectList* list, igObject* o) {
    func_00301E30(list, o);
}

/* (Re)create the data of a player (player 0 also rebuilds the common data). */
void beNDMWGameRam_initData(beNDMWGameRam* gr, s32 player) {
    s32 i;
    s32 j;

    if (player == 0) {
        ObjListClear(gr->_commonDataList);
        beNDMWGameRam_buildData(gr, gr->_commonDataList, D_004CED40);
    }
    if (gr->_playerDataListList->_count != 4) {
        if (((volatile beDataObjListList*)gr->_playerDataListList)->_count < 5) {
            func_002D6680(gr->_playerDataListList, 4, 4);
        }
        func_00301EE0(gr->_playerDataListList, 4);
    }
    {
        beDataObjListRef pl(func_00364050(PoolOf(gr)), IG_ADOPT);

        beNDMWGameRam_buildData(gr, pl, D_004CEDF0);
        for (i = 0; i < 16; i++) {
            IntListSet(beDataObjList_getIntList(pl, D_00518B30)->_data, i, D_004CF110);
        }
        for (j = 0; j < 16; j++) {
            s32 k;

            for (k = 0; k < 9; k++) {
                IntListSet(beDataObjList_getIntList(pl, D_00518B50)->_data, j * 9 + k, D_004CF118);
            }
        }
        for (i = 0; i < 12; i++) {
            beNDMWItemDiskRef disk(func_003C85B0(PoolOf(gr)), IG_ADOPT);

            func_00448710(disk, i);
            ObjListAppend(beDataObjList_getObjectList(pl, D_00518A50)->_data, disk);
        }
        BoolListSet(beDataObjList_getBoolList(pl, D_00518AE8)->_data, 960, true);
        ObjListSetObj(pl, gr->_playerDataListList, player);
    }
}

/* Create the common and per-player data lists. */
void beNDMWGameRam_begin(beNDMWGameRam* gr) {
    s32 i;

    beBaseInfoManager_begin(gr);
    {
        beDataObjListRef l(func_00364050(PoolOf(gr)), IG_ADOPT);

        igAssign(gr->_commonDataList, l.p);
    }
    {
        beDataObjListListRef l(func_00363E80(PoolOf(gr)), IG_ADOPT);

        igAssign(gr->_playerDataListList, l.p);
    }
    for (i = 0; i < 4; i++) {
        beNDMWGameRam_initData(gr, i);
    }
}

/* Empty the common and per-player data lists. */
void beNDMWGameRam_end(beNDMWGameRam* gr) {
    ObjListClear(gr->_commonDataList);
    if ((gr->_commonDataList->_count > 0) == false) {
        func_002D6680(gr->_commonDataList, 0, 4);
    }
    ObjListClear(gr->_playerDataListList);
    if ((gr->_playerDataListList->_count > 0) == false) {
        func_002D6680(gr->_playerDataListList, 0, 4);
    }
}

/* Drop the databases. */
void beNDMWGameRam_prepare(beNDMWGameRam* gr) {
    igAssign(gr->_dBShinka, (beDataBase*)NULL);
    igAssign(gr->_dBPcTech, (beDataBase*)NULL);
    igAssign(gr->_dBWeapon, (beDataBase*)NULL);
    igAssign(gr->_dBArmor1, (beDataBase*)NULL);
    igAssign(gr->_dBArmor2, (beDataBase*)NULL);
    igAssign(gr->_dBAbChip, (beDataBase*)NULL);
    igAssign(gr->_dBEnemy, (beDataBase*)NULL);
    igAssign(gr->_dBEneatk, (beDataBase*)NULL);
}

/* Create the "gameram" info with one ram and register it as GAMERAM. */
s32 beNDMWGameRam_appendInfo(beNDMWGameRam* gr) {
    if (gr->_infoList->_count == 0) {
        beNDMWGameRamInfoRef info(func_003CA380(PoolOf2(gr)), IG_ADOPT);
        beNDMWGameRamInfoRamRef ram(func_003CA1A0(PoolOf2(gr)), IG_ADOPT);

        func_002FECB0(info, D_00518BB8);
        ram->_parentInfo = info;
        ram->_parentManager = gr;
        SetState(ram.p, 0);
        ram->_ramNo = 0;
        ram->_isRun = true;
        func_00301E30(info->_ramList, ram);
        func_00301E30(gr->_infoList, info);
        beMessenger_entryRam(gr->_messenger, ram, D_00518BC0);
    }
    return 0;
}



void beNDMWGameRam_updateRam(void) {
}

/* shinka record whose MODEL_NAME is name (pooled strings compare by pointer). */
s32 func_003F80E0(beNDMWGameRam* gr, const char* name) {
    igStringRef s(name);
    s32 i;

    for (i = 0; i < 16; i++) {
        if (SameStr(beDataBase_getString(GetDBShinka(gr), i, D_00518BC8), s.p)) {
            return i;
        }
    }
    return 0;
}

/* BASE2CURRENT: switch the player to digimon d, loading its saved level, EXP,
   skills and UPCNT; player 0 also shares its flags and counters as the
   common ones. */
/* Shared list: o->_data = l, with references. */
template <class O, class L>
static inline void AssignData(O* o, L* l) {
    igIncRef(l);
    igUnref(o->_data);
    o->_data = l;
}

void beNDMWGameRam_base2Current(beNDMWGameRam* gr, s32 player, s32 d) {
    beDataObjList* pl = ((beDataObjList**)gr->_playerDataListList->_data)[player];
    s32 base;
    s32 i;
    s32 v;

    beDataObjList_getInt(pl, D_00518858);

    beDataObjList_getInt(pl, D_00518848)->_data = d;
    {
        const char* name = beDataBase_getString(GetDBShinka(gr), d, D_00518BC8);
        beDataObjString* s = beDataObjList_getString(pl, D_00518870);
        char* str = func_002EAF60(GetStrPool(), name);

        ReleaseStr(s->_data);
        s->_data = str;
    }
    beDataObjList_getInt(pl, D_00518858)->_data = IntListGet(beDataObjList_getIntList(pl, D_00518B30)->_data, d);
    beDataObjList_getInt(pl, D_00518880)->_data = IntListGet(beDataObjList_getIntList(pl, D_00518B40)->_data, d);
    base = d * 9;
    for (i = 0; i < 9; i++) {
        v = IntListGet(beDataObjList_getIntList(pl, D_00518B50)->_data, base + i);
        IntListSet(beDataObjList_getIntList(pl, D_00518860)->_data, i, v);
    }
    {
        s32 base2 = d * 11;
        s32 j;

        for (j = 0; j < 11; j++) {
            s32 v2 = IntListGet(beDataObjList_getIntList(pl, D_00518B60)->_data, base2 + j);

            IntListSet(beDataObjList_getIntList(pl, D_00518868)->_data, j, v2);
        }
    }
    if (player == 0) {
        AssignData(beDataObjList_getBoolList(gr->_commonDataList, D_00518798),
                   beDataObjList_getBoolList(pl, D_00518AE8)->_data);
        AssignData(beDataObjList_getBoolList(gr->_commonDataList, D_005187A0),
                   beDataObjList_getBoolList(pl, D_00518B00)->_data);
        AssignData(beDataObjList_getIntList(gr->_commonDataList, D_005187B0),
                   beDataObjList_getIntList(pl, D_00518B10)->_data);
    }
}

/* CURRENT2BASE: save the current digimon's level, EXP, skills and UPCNT into
   its BASE_* slot. */
void beNDMWGameRam_current2Base(beNDMWGameRam* gr, s32 player) {
    s32 b9;
    s32 i;
    beDataObjList* pl;
    s32 dv[2]; /* [0] = digimon, [1] = value being copied */

    pl = ((beDataObjList**)gr->_playerDataListList->_data)[player];
    dv[0] = beDataObjList_getInt(pl, D_00518848)->_data;
    {
        dv[1] = beDataObjList_getInt(pl, D_00518858)->_data;
        IntListSet(beDataObjList_getIntList(pl, D_00518B30)->_data, dv[0], dv[1]);
    }
    {
        dv[1] = beDataObjList_getInt(pl, D_00518880)->_data;
        IntListSet(beDataObjList_getIntList(pl, D_00518B40)->_data, dv[0], dv[1]);
    }
    b9 = dv[0] * 9;
    for (i = 0; i < 9; i++) {
        dv[1] = IntListGet(beDataObjList_getIntList(pl, D_00518860)->_data, i);
        IntListSet(beDataObjList_getIntList(pl, D_00518B50)->_data, b9 + i, dv[1]);
    }
    {
        s32 b11 = dv[0] * 11;
        s32 j;

        for (j = 0; j < 11; j++) {
            s32 v2 = IntListGet(beDataObjList_getIntList(pl, D_00518868)->_data, j);

            IntListSet(beDataObjList_getIntList(pl, D_00518B60)->_data, b11 + j, v2);
        }
    }
}

/* Give the player the digimon's three starting weapons (shinka INI_WEAPONn). */
void beNDMWGameRam_giveStartWeapons(beNDMWGameRam* gr, s32 player) {
    beDataObjList* pl = ((beDataObjList**)gr->_playerDataListList->_data)[player];
    igObjectList* folder = beDataObjList_getObjectList(pl, D_00518A40)->_data;
    s32 d = beDataObjList_getInt(pl, D_00518848)->_data;
    s32 i;

    ObjListClear(folder);
    for (i = 0; i < 3; i++) {
        beNDMWItemWeaponRef item(func_003C8C90(PoolOf(gr)), IG_ADOPT);

        func_004482C0(item, beDataBase_getInt(GetDBShinka(gr), d, D_004CF120[i]), 0, 0);
        func_00301E30(folder, item);
    }
}

/* Fill the power socket: the first three unequipped devices, or (strong) the
   strongest one (TSUYOSA) this digimon can use. */
void beNDMWGameRam_equipPowerSocket(beNDMWGameRam* gr, s32 player, bool strong) {
    s32 best;
    beDataObjList* pl;
    igObjectList* folder;
    igObjectList* socket;
    igStringRefList* names;
    const char* model;
    s32 n;
    s32 i;
    s32 digimon;
    igIntList* skill;
    beNDMWItemRevisionBase* d;
    s32 bestStr;
    s32 bit;

    pl = ((beDataObjList**)gr->_playerDataListList->_data)[player];
    folder = beDataObjList_getObjectList(pl, D_00518A40)->_data;
    if (strong == false) {
        socket = beDataObjList_getObjectList(pl, D_00518A88)->_data;
        names = beDataObjList_getStringList(pl, D_00518AA0)->_data;
        ObjListClear(socket);
        ClearStrList(names);
        {
            s32 c = folder->_count;
            s32 k;

            for (k = 0; k < c; k++) {
                beNDMWItemRevisionBase* e;

                if (socket->_count == 3) {
                    break;
                }
                e = ((beNDMWItemRevisionBase**)folder->_data)[k];
                if (e->_kindNo == 0) {
                    const char* m;

                    func_00301E30(socket, e);
                    m = func_00447CB0(e, gr, D_00518C08);
                    APPEND_STR(names, m)
                    e->_isEquip = true;
                }
            }
        }
    } else {
        igStringRefList* names2;

        digimon = beDataObjList_getInt(pl, D_00518848)->_data;
        skill = beDataObjList_getIntList(pl, D_00518860)->_data;
        socket = beDataObjList_getObjectList(pl, D_00518A88)->_data;
        names2 = beDataObjList_getStringList(pl, D_00518AA0)->_data;
        {
            s32 c = folder->_count;
            s32 k;

            for (k = 0; k < c; k++) {
                beNDMWItemRevisionBase* e = ((beNDMWItemRevisionBase**)folder->_data)[k];

                if (e->_kindNo == 0) {
                    e->_isEquip = false;
                }
            }
        }
        func_00301C90(socket, NULL);
        ClearStrList(names2);
        bestStr = 0;
        best = -1;
        bit = 1 << digimon;
        n = folder->_count;
        for (i = 0; i < n; i++) {
            d = ((beNDMWItemRevisionBase**)folder->_data)[i];
            if (d->_kindNo == 0) {
                s32 soubi = bit & func_00447C10(d, gr, D_00518C10);
                s32 sk = func_00447C10(d, gr, D_00518860);
                s32 pt = func_00447C10(d, gr, D_00518C18);

                if (soubi != 0 && ((s32*)skill->_data)[sk] >= pt &&
                    (best < 0 || bestStr < func_00447C10(d, gr, D_005188B0))) {
                    bestStr = func_00447C10(d, gr, D_005188B0);
                    best = i;
                }
            }
        }
        if (best >= 0) {
            d = ((beNDMWItemRevisionBase**)folder->_data)[best];
            ObjListSet(socket, 0, d);
            model = func_00447CB0(d, gr, D_00518C08);
            APPEND_STR(names2, model)
            d->_isEquip = true;
        }
    }
}


/* Whether the player has an unequipped device that can teach a skill to
   digimon: SOUBI has the digimon's bit and the player's SKILL (or BASE_SKILL
   row for another digimon) reaches the device's SKILLPOINT. */
bool beNDMWGameRam_canLearnSkill(beNDMWGameRam* gr, s32 player, s32 digimon) {
    beDataObjList* pl;
    igObjectList* folder;
    igIntList* skill;
    s32 base;
    s32 n;
    s32 i;
    s32 bit;

    pl = ((beDataObjList**)gr->_playerDataListList->_data)[player];
    folder = beDataObjList_getObjectList(pl, D_00518A40)->_data;
    {
        s32 cur = beDataObjList_getInt(pl, D_00518848)->_data;

        if (cur == digimon) {
            skill = beDataObjList_getIntList(pl, D_00518860)->_data;
            base = 0;
        } else {
            skill = beDataObjList_getIntList(pl, D_00518B50)->_data;
            base = digimon * 9;
        }
    }
    bit = 1 << digimon;
    n = folder->_count;
    for (i = 0; i < n; i++) {
        beNDMWItemRevisionBase* d = ((beNDMWItemRevisionBase**)folder->_data)[i];

        if (d->_kindNo == 0) {
            s32 soubi = bit & func_00447C10(d, gr, D_00518C10);
            s32 sk = func_00447C10(d, gr, D_00518860);
            s32 pt = func_00447C10(d, gr, D_00518C18);

            if (soubi != 0 && ((s32*)skill->_data)[base + sk] >= pt) {
                return true;
            }
        }
    }
    return false;
}

/* GAMESTART for a player: set up digimon, stats and skills, mark ISUSE and
   recount NINZU. */
void beNDMWGameRam_startPlayer(beNDMWGameRam* gr, s32 player, const char* digimonName) {
    s32 n;
    s32 i;
    beDataObjList* common;
    beDataObjList* pl;
    s32 d;

    common = gr->_commonDataList;
    pl = ((beDataObjList**)gr->_playerDataListList->_data)[player];
    d = func_003F80E0(gr, digimonName);
    beNDMWGameRam_base2Current(gr, player, d);
    beNDMWGameRam_giveStartWeapons(gr, player);
    beNDMWGameRam_equipPowerSocket(gr, player, 0);
    beNDMWGameRam_recalcPlayer(gr, player, 1);
    SetTrue(beDataObjList_getBool(pl, D_00518888));
    BoolListSet(beDataObjList_getBoolList(pl, D_00518B20)->_data, d, true);
    n = 0;
    for (i = 0; i < 4; i++) {
        if (beDataObjList_getBool(PlayerData(gr, i), D_00518888)->_data) {
            n++;
        }
    }
    beDataObjList_getInt(common, D_00518760)->_data = n;
}

extern igMetaObject* beFont_Meta;
beUnicodeObj* func_003614F0(void* pool);         /* new beUnicodeObj */
void func_003843C0(beUnicodeObj* o, s32 n);      /* make room for n characters */

IG_REF(beUnicodeObj);

beUnicodeObjRef func_00382EB0(beFont* font, const char* text); /* text to unicode */

/* Copy text (converted by the font manager) into dst, 8 characters, zero padded. */
static inline void SetUnicode8(beUnicodeObjRef& dst, beFont* font, const char* s) {
    beUnicodeObjRef text = func_00382EB0(font, s);
    u16* src = (u16*)text->_buffer;
    u16* d = (u16*)dst->_buffer;
    s32 i;

    for (i = 0; i < 8; i++) {
        d[i] = src[i];
        if (src[i] == 0) {
            break;
        }
    }
    for (; i < 8; i++) {
        d[i] = 0;
    }
}

/* GAMESTART with a name string: start the player with the digimon, then set
   PLAYERNAME to the name in unicode (8 characters, zero padded). */
void func_003F96E0(beNDMWGameRam* gr, s32 player, const char* digimonName, char** name) {
    beDataObjList* pl;
    igObjectList* list;

    beNDMWGameRam_startPlayer(gr, player, digimonName);
    pl = PlayerData(gr, player);
    beUnicodeObjRef str(func_003614F0(PoolOf(gr)), IG_ADOPT);
    func_003843C0(str, 8);
    SetUnicode8(str, (beFont*)func_00236F80(gr->_insight, beFont_Meta), *name);
    list = beDataObjList_getObjectList(pl, D_00518828)->_data;
    if (*(volatile s32*)&list->_count != 0) {
        ObjListClear(list);
    }
    func_00301E30(list, str);
}

/* GAMESTART with a name object: start the player with the digimon, then set
   PLAYERNAME. */
void func_003F99F0(beNDMWGameRam* gr, s32 player, const char* digimonName, igObject** name) {
    igObjectList* list;

    beNDMWGameRam_startPlayer(gr, player, digimonName);
    list = beDataObjList_getObjectList(((beDataObjList**)gr->_playerDataListList->_data)[player], D_00518828)->_data;
    if (*(volatile s32*)&list->_count != 0) {
        ObjListClear(list);
    }
    func_00301E30(list, *name);
}

/* Player leaves: clear ISUSE and recount NINZU. */
void beNDMWGameRam_removePlayer(beNDMWGameRam* gr, s32 player) {
    s32 n;
    s32 i;
    beDataObjList* common = gr->_commonDataList;

    beDataObjList_getBool(PlayerData(gr, player), D_00518888)->_data = false;
    n = 0;
    for (i = 0; i < 4; i++) {
        if (beDataObjList_getBool(PlayerData(gr, i), D_00518888)->_data) {
            n++;
        }
    }
    beDataObjList_getInt(common, D_00518760)->_data = n;
}

/* ADDHP: add delta to the player's HP (or set it to value unless -1), clamped
   to 0..MHP; a dead player (HP 0) gets nothing. Updates the HP meter and number. */
bool beNDMWGameRam_addHp(beNDMWGameRam* gr, s32 player, s32 delta, s32 value) {
    s32 hp;
    beDataObjList* pl;
    s32 mhp;

    pl = ((beDataObjList**)gr->_playerDataListList->_data)[player];
    mhp = beDataObjList_getInt(pl, D_00518898)->_data;
    hp = beDataObjList_getInt(pl, D_00518890)->_data;

    if (value == -1) {
        if (hp == 0) {
            return false;
        }
        hp += delta;
    } else {
        hp = value;
    }
    if (hp < 0) {
        hp = 0;
    }
    if (hp > mhp) {
        hp = mhp;
    }
    beDataObjList_getInt(pl, D_00518890)->_data = hp;
    beMessenger_select(gr->_messenger, D_00518C28, D_00518C38, player);
    beMessenger_pushFloat(gr->_messenger, 100.0f * (f32)hp / (f32)mhp);
    beMessenger_send(gr->_messenger, D_00518C40);
    beMessenger_select(gr->_messenger, D_00518C50, D_00518C60, player);
    beMessenger_pushInt(gr->_messenger, hp);
    beMessenger_send(gr->_messenger, D_00518C68);
    return true;
}

/* ADDMP: add delta to the player's MP, clamped to 0..MMP. */
void beNDMWGameRam_addMp(beNDMWGameRam* gr, s32 player, s32 delta) {
    s32 mp;
    beDataObjList* pl;
    s32 mmp;

    pl = ((beDataObjList**)gr->_playerDataListList->_data)[player];
    mmp = beDataObjList_getInt(pl, D_005188A8)->_data;
    mp = beDataObjList_getInt(pl, D_005188A0)->_data;
    mp += delta;

    if (mp < 0) {
        mp = 0;
    }
    if (mp > mmp) {
        mp = mmp;
    }
    beDataObjList_getInt(pl, D_005188A0)->_data = mp;
    beMessenger_select(gr->_messenger, D_00518C28, D_00518C78, player);
    beMessenger_pushFloat(gr->_messenger, 100.0f * (f32)mp / (f32)mmp);
    beMessenger_send(gr->_messenger, D_00518C40);
    beMessenger_select(gr->_messenger, D_00518C50, D_00518C80, player);
    beMessenger_pushInt(gr->_messenger, mp);
    beMessenger_send(gr->_messenger, D_00518C68);
}

/* Add the power socket item's param (and its revision if asked) to *sum.
   The sums are unsigned (the revision add matches only that way). */
void beNDMWGameRam_addPowerSocket(beNDMWGameRam* gr, beDataObjList* pl, u32* sum, const char* param, bool addRevision) {
    if (beDataObjList_getObjectList(pl, D_00518A88)->_data->_count != 0) {
        beNDMWItemRevisionBase* item = ((beNDMWItemRevisionBase**)beDataObjList_getObjectList(pl, D_00518A88)->_data->_data)[0];

        if (item != NULL) {
            *sum += func_00447C10(item, gr, param);
            if (addRevision) {
                *sum += item->_revision;
            }
        }
    }
}

void beNDMWGameRam_addPowerPocket(beNDMWGameRam* gr, beDataObjList* pl, u32* sum, const char* param) {
    s32 n = beDataObjList_getObjectList(pl, D_00518AB0)->_data->_count;
    s32 i;

    for (i = 0; i < n; i++) {
        beNDMWItemRevisionBase* item = ((beNDMWItemRevisionBase**)beDataObjList_getObjectList(pl, D_00518AB0)->_data->_data)[i];

        if (item != NULL) {
            *sum += func_00447C10(item, gr, param);
        }
    }
}

void beNDMWGameRam_addMainSlot(beNDMWGameRam* gr, beDataObjList* pl, u32* sum, const char* param, bool addRevision) {
    if (beDataObjList_getObjectList(pl, D_00518AC0)->_data->_count != 0) {
        beNDMWItemRevisionBase* item = ((beNDMWItemRevisionBase**)beDataObjList_getObjectList(pl, D_00518AC0)->_data->_data)[0];

        if (item != NULL) {
            *sum += func_00447C10(item, gr, param);
            if (addRevision) {
                *sum += item->_revision;
            }
        }
    }
}

void beNDMWGameRam_addMainPocket(beNDMWGameRam* gr, beDataObjList* pl, u32* sum, const char* param) {
    s32 n = beDataObjList_getObjectList(pl, D_00518AD0)->_data->_count;
    s32 i;

    for (i = 0; i < n; i++) {
        beNDMWItemRevisionBase* item = ((beNDMWItemRevisionBase**)beDataObjList_getObjectList(pl, D_00518AD0)->_data->_data)[i];

        if (item != NULL) {
            *sum += func_00447C10(item, gr, param);
        }
    }
}

/* Sub slot: the revision only counts when the item's SHUSEINOURYOKU (main
   ability) is the param asked for. */
void beNDMWGameRam_addSubSlot(beNDMWGameRam* gr, beDataObjList* pl, u32* sum, const char* param, bool addRevision) {
    if (beDataObjList_getObjectList(pl, D_00518AE0)->_data->_count != 0) {
        beNDMWItemRevisionBase* item = ((beNDMWItemRevisionBase**)beDataObjList_getObjectList(pl, D_00518AE0)->_data->_data)[0];

        if (item != NULL) {
            *sum += func_00447C10(item, gr, param);
            if (addRevision && strcmp(func_00447CB0(item, gr, D_00518C88), param) == 0) {
                *sum += item->_revision;
            }
        }
    }
}

/* Max HP (hp != 0) or max MP of digimon (current one if < 0) at its level:
   UPCNT + INI + GROW * curve(level) / 100, max 99999. */
s32 beNDMWGameRam_calcHpMpBase(beNDMWGameRam* gr, s32 player, s32 digimon, bool hp) {
    beDataObjList* pl = ((beDataObjList**)gr->_playerDataListList->_data)[player];
    igIntList* upcnt;
    s32 base;
    s32 level;
    s32 ini;
    s32 grow;
    s32 u;
    s32 v;

    if (digimon < 0) {
        digimon = beDataObjList_getInt(pl, D_00518848)->_data;
        upcnt = beDataObjList_getIntList(pl, D_00518868)->_data;
        base = 0;
        level = beDataObjList_getInt(pl, D_00518858)->_data;
    } else {
        upcnt = beDataObjList_getIntList(pl, D_00518B60)->_data;
        base = digimon * 11;
        level = ((s32*)beDataObjList_getIntList(pl, D_00518B30)->_data->_data)[digimon];
    }
    if (hp) {
        ini = beDataBase_getInt(GetDBShinka(gr), digimon, D_00518C98);
        grow = beDataBase_getInt(GetDBShinka(gr), digimon, D_00518CA0);
        u = ((s32*)upcnt->_data)[base];
    } else {
        ini = beDataBase_getInt(GetDBShinka(gr), digimon, D_00518CB0);
        grow = beDataBase_getInt(GetDBShinka(gr), digimon, D_00518CB8);
        s32 k = base + 1;

        u = ((s32*)upcnt->_data)[k];
    }
    v = u + (ini + grow * HpMpCurve(level) / 100);
    if (v > 99999) {
        v = 99999;
    }
    return v;
}

/* Stat stat (0 strength .. 4 speed) of digimon (current one if < 0) at its
   level: UPCNT + INI + GROW * curve(level) / 100, max 9999. */
s32 beNDMWGameRam_calcStatBase(beNDMWGameRam* gr, s32 player, s32 digimon, s32 stat) {
    beDataObjList* pl = ((beDataObjList**)gr->_playerDataListList->_data)[player];
    igIntList* upcnt;
    s32 base;
    s32 level;
    s32 ini;
    s32 grow;
    s32 u;
    s32 v;

    if (digimon < 0) {
        digimon = beDataObjList_getInt(pl, D_00518848)->_data;
        upcnt = beDataObjList_getIntList(pl, D_00518868)->_data;
        base = 0;
        level = beDataObjList_getInt(pl, D_00518858)->_data;
    } else {
        upcnt = beDataObjList_getIntList(pl, D_00518B60)->_data;
        base = digimon * 11;
        level = ((s32*)beDataObjList_getIntList(pl, D_00518B30)->_data->_data)[digimon];
    }
    ini = beDataBase_getInt(GetDBShinka(gr), digimon, D_004CF250[stat]);
    grow = beDataBase_getInt(GetDBShinka(gr), digimon, D_004CF270[stat]);
    u = ((s32*)upcnt->_data)[base + 2 + stat];
    v = u + (ini + grow * StatCurve(level) / 100);
    if (v > 9999) {
        v = 9999;
    }
    return v;
}

static inline s32 GetInt(beDataObjList* pl, const char* name) {
    return beDataObjList_getInt(pl, name)->_data;
}

/* RECALCPLAYER: all stats from the growth curve, buffs (UPDATA / DOWNDATA) and,
   with equip, the equipped items; then techniques, NEXTEXP and the derived rates.
   See docs/gameram.md. */
void beNDMWGameRam_recalcPlayer(beNDMWGameRam* gr, s32 player, bool equip) {
    s32 mhp;
    s32 mmp;
    s32 str;
    s32 def;
    s32 wis;
    s32 spi;
    s32 spd;
    s32 resFire;
    s32 resIce;
    s32 resThunder;
    s32 resDark;
    beDataObjList* pl = PlayerData(gr, player);
    s32 d = GetInt(pl, D_00518848);
    s32 level = GetInt(pl, D_00518858);
    bool dead = false;

    mhp = beNDMWGameRam_calcHpMpBase(gr, player, -1, true);
    mhp += UpData(pl, 6);
    if (equip) {
        beNDMWGameRam_addMainSlot(gr, pl, (u32*)&mhp, D_00518FC8, false);
        beNDMWGameRam_addSubSlot(gr, pl, (u32*)&mhp, D_00518FC8, false);
    }
    if (mhp > 99999) {
        mhp = 99999;
    }
    beDataObjList_getInt(pl, D_00518898)->_data = mhp;
    {
        s32 hp = beDataObjList_getInt(pl, D_00518890)->_data;

        if (hp == 0) {
            dead = true;
        }
        if (hp == 0 || mhp < hp) {
            beDataObjList_getInt(pl, D_00518890)->_data = mhp;
        }
    }

    mmp = beNDMWGameRam_calcHpMpBase(gr, player, -1, false);
    if (equip) {
        beNDMWGameRam_addMainSlot(gr, pl, (u32*)&mmp, D_00518FD0, false);
        beNDMWGameRam_addSubSlot(gr, pl, (u32*)&mmp, D_00518FD0, false);
    }
    if (mmp > 99999) {
        mmp = 99999;
    }
    beDataObjList_getInt(pl, D_005188A8)->_data = mmp;
    {
        s32 mp = beDataObjList_getInt(pl, D_005188A0)->_data;

        if (dead || mmp < mp) {
            beDataObjList_getInt(pl, D_005188A0)->_data = mmp;
        }
    }

    str = beNDMWGameRam_calcStatBase(gr, player, -1, 0);
    str += UpData(pl, 4);
    str -= DownData(pl, 4);
    if (equip) {
        beNDMWGameRam_addPowerSocket(gr, pl, (u32*)&str, D_005188B0, true);
        beNDMWGameRam_addPowerPocket(gr, pl, (u32*)&str, D_005188B0);
        beNDMWGameRam_addSubSlot(gr, pl, (u32*)&str, D_005188B0, true);
    }
    if (str <= 0) {
        str = 1;
    }
    if (str > 9999) {
        str = 9999;
    }
    beDataObjList_getInt(pl, D_005188B0)->_data = str;

    def = beNDMWGameRam_calcStatBase(gr, player, -1, 1);
    def += UpData(pl, 5);
    def -= DownData(pl, 5);
    if (equip) {
        beNDMWGameRam_addMainSlot(gr, pl, (u32*)&def, D_005188B8, true);
        beNDMWGameRam_addSubSlot(gr, pl, (u32*)&def, D_005188B8, true);
    }
    if (def <= 0) {
        def = 1;
    }
    if (def > 9999) {
        def = 9999;
    }
    beDataObjList_getInt(pl, D_005188B8)->_data = def;

    wis = beNDMWGameRam_calcStatBase(gr, player, -1, 2);
    if (equip) {
        beNDMWGameRam_addPowerSocket(gr, pl, (u32*)&wis, D_005188C0, false);
        beNDMWGameRam_addPowerPocket(gr, pl, (u32*)&wis, D_005188C0);
        beNDMWGameRam_addMainSlot(gr, pl, (u32*)&wis, D_005188C0, false);
        beNDMWGameRam_addMainPocket(gr, pl, (u32*)&wis, D_005188C0);
        beNDMWGameRam_addSubSlot(gr, pl, (u32*)&wis, D_005188C0, true);
    }
    if (wis <= 0) {
        wis = 1;
    }
    if (wis > 9999) {
        wis = 9999;
    }
    beDataObjList_getInt(pl, D_005188C0)->_data = wis;

    spi = beNDMWGameRam_calcStatBase(gr, player, -1, 3);
    if (equip) {
        beNDMWGameRam_addPowerSocket(gr, pl, (u32*)&spi, D_005188D0, false);
        beNDMWGameRam_addPowerPocket(gr, pl, (u32*)&spi, D_005188D0);
        beNDMWGameRam_addMainSlot(gr, pl, (u32*)&spi, D_005188D0, false);
        beNDMWGameRam_addMainPocket(gr, pl, (u32*)&spi, D_005188D0);
        beNDMWGameRam_addSubSlot(gr, pl, (u32*)&spi, D_005188D0, false);
    }
    if (spi <= 0) {
        spi = 1;
    }
    if (spi > 9999) {
        spi = 9999;
    }
    beDataObjList_getInt(pl, D_005188D0)->_data = spi;

    spd = beNDMWGameRam_calcStatBase(gr, player, -1, 4);
    spd += UpData(pl, 0);
    spd -= DownData(pl, 0);
    if (equip) {
        beNDMWGameRam_addPowerSocket(gr, pl, (u32*)&spd, D_005188D8, false);
        beNDMWGameRam_addPowerPocket(gr, pl, (u32*)&spd, D_005188D8);
        beNDMWGameRam_addMainSlot(gr, pl, (u32*)&spd, D_005188D8, false);
        beNDMWGameRam_addMainPocket(gr, pl, (u32*)&spd, D_005188D8);
        beNDMWGameRam_addSubSlot(gr, pl, (u32*)&spd, D_005188D8, true);
    }
    if (spd <= 0) {
        spd = 1;
    }
    if (spd > 9999) {
        spd = 9999;
    }
    beDataObjList_getInt(pl, D_005188D8)->_data = spd;

#define RECALC_RES(var, col, idx, field)                                              \
    var = beDataBase_getInt(GetDBShinka(gr), d, col);                                \
    var += UpCnt(pl, idx);                                                           \
    var += UpData(pl, idx);                                                          \
    if (equip) {                                                                     \
        beNDMWGameRam_addMainSlot(gr, pl, (u32*)&var, col, false);                   \
        beNDMWGameRam_addMainPocket(gr, pl, (u32*)&var, col);                        \
        beNDMWGameRam_addSubSlot(gr, pl, (u32*)&var, col, true);                     \
    }                                                                                \
    if (var <= 0) {                                                                  \
        var = 1;                                                                     \
    }                                                                                \
    if (var > 9999) {                                                              \
        var = 9999;                                                                  \
    }                                                                                \
    beDataObjList_getInt(pl, field)->_data = var;

    RECALC_RES(resFire, D_00518FD8, 7, D_005188E8)
    RECALC_RES(resIce, D_00518FE8, 8, D_005188F8)
    RECALC_RES(resThunder, D_00518FF0, 9, D_00518908)
    RECALC_RES(resDark, D_00519000, 10, D_00518918)
#undef RECALC_RES

    /* Techniques: TECHn once LEVEL reaches TECHn_LEVEL, at the highest rank whose
       TECHn_SKILLr the skill level reaches. */
    {
        igObjectList* waza = beDataObjList_getObjectList(pl, D_00518928)->_data;
        s32 i;

        ObjListClear(waza);
        for (i = 0; i < 6; i++) {
            s32 tech = beDataBase_getInt(GetDBShinka(gr), d, D_004CF290[i]);
            s32 sk = beDataBase_getInt(GetDBPcTech(gr), tech, D_00518860);
            s32 skLv = ((s32*)beDataObjList_getIntList(pl, D_00518860)->_data->_data)[sk];

            if (beDataBase_getInt(GetDBShinka(gr), d, D_004CF2B0[i]) <= level) {
                s32 r;

                for (r = 4; r >= 0; r--) {
                    if (beDataBase_getInt(GetDBShinka(gr), d, D_004CF2D0[i][r]) <= skLv) {
                        beNDMWItemWazaRef w(func_003C8040(PoolOf(gr)), IG_ADOPT);

                        func_00448F30(w, tech + r, tech);
                        func_00301E30(waza, w);
                        break;
                    }
                }
            }
        }
    }

    beDataObjList_getInt(pl, D_00518938)->_data = level == 999 ? 0 : level * 200 + (level + 140) * (level * level);

    if (equip) {
        s32 delay;
        s32 hit;
        s32 crit;
        s32 stun;
        s32 evade;
        s32 guard;
        s32 resStun;
        s32 resPoison;
        s32 bit;
        s32 exp;
        s32 luck;
        s32 hpDrain;
        s32 mpDrain;

        delay = DownData(pl, 6);
        beNDMWGameRam_addPowerSocket(gr, pl, (u32*)&delay, D_00519010, false);
        if (delay > 70) {
            delay = 70;
        }
        beDataObjList_getInt(pl, D_00518940)->_data = delay;
        hit = 0;
        beNDMWGameRam_addPowerSocket(gr, pl, (u32*)&hit, D_00518950, false);
        beDataObjList_getInt(pl, D_00518950)->_data = hit;
        crit = 0;
        crit += UpData(pl, 1);
        crit -= DownData(pl, 1);
        beNDMWGameRam_addPowerSocket(gr, pl, (u32*)&crit, D_00518960, false);
        beNDMWGameRam_addPowerPocket(gr, pl, (u32*)&crit, D_00518960);
        if (crit <= 0) {
            crit = 1;
        }
        beDataObjList_getInt(pl, D_00518960)->_data = crit;
        stun = 0;
        beNDMWGameRam_addPowerSocket(gr, pl, (u32*)&stun, D_00518970, false);
        beDataObjList_getInt(pl, D_00518970)->_data = stun;
        evade = 0;
        evade += UpData(pl, 3);
        evade -= DownData(pl, 3);
        beNDMWGameRam_addMainPocket(gr, pl, (u32*)&evade, D_00518A10);
        if (evade <= 0) {
            evade = 1;
        }
        beDataObjList_getInt(pl, D_00518980)->_data = evade;
        guard = 0;
        beNDMWGameRam_addMainSlot(gr, pl, (u32*)&guard, D_00518990, false);
        beDataObjList_getInt(pl, D_00518990)->_data = guard;
        resStun = 0;
        resStun += UpData(pl, 2);
        resStun -= DownData(pl, 2);
        beNDMWGameRam_addMainSlot(gr, pl, (u32*)&resStun, D_005189A0, false);
        if (resStun <= 0) {
            resStun = 1;
        }
        beDataObjList_getInt(pl, D_005189A0)->_data = resStun;
        resPoison = 0;
        beNDMWGameRam_addMainSlot(gr, pl, (u32*)&resPoison, D_005189B0, false);
        beDataObjList_getInt(pl, D_005189B0)->_data = resPoison;
        bit = 0;
        beNDMWGameRam_addSubSlot(gr, pl, (u32*)&bit, D_005189C0, false);
        beDataObjList_getInt(pl, D_005189C0)->_data = bit;
        exp = 0;
        beNDMWGameRam_addSubSlot(gr, pl, (u32*)&exp, D_005189D0, false);
        beDataObjList_getInt(pl, D_005189D0)->_data = exp;
        luck = 0;
        beNDMWGameRam_addSubSlot(gr, pl, (u32*)&luck, D_005189E0, false);
        beDataObjList_getInt(pl, D_005189E0)->_data = luck;

        /* Weapon element: the first power pocket item with a known ZOKUSEI. */
        {
            beDataObjInt* z = beDataObjList_getInt(pl, D_005189F0);

            z->_data = -1;
        }
        {
            bool found = false;
            s32 n = beDataObjList_getObjectList(pl, D_00518AB0)->_data->_count;
            s32 i;

            for (i = 0; i < n; i++) {
                beNDMWItemRevisionBase* item = ((beNDMWItemRevisionBase**)beDataObjList_getObjectList(pl, D_00518AB0)->_data->_data)[i];

                if (item != NULL) {
                    const char* z = func_00447CB0(item, gr, D_00519018);
                    s32 k;

                    for (k = 0; k < 4; k++) {
                        if (strcmp(z, D_004CF350[k]) == 0) {
                            beDataObjList_getInt(pl, D_005189F0)->_data = k;
                            found = true;
                            break;
                        }
                    }
                    if (found) {
                        break;
                    }
                }
            }
        }
        hpDrain = 0;
        beNDMWGameRam_addPowerPocket(gr, pl, (u32*)&hpDrain, D_00518A00);
        beDataObjList_getInt(pl, D_00518A00)->_data = hpDrain;
        mpDrain = 0;
        beNDMWGameRam_addPowerPocket(gr, pl, (u32*)&mpDrain, D_00518A08);
        beDataObjList_getInt(pl, D_00518A08)->_data = mpDrain;
        beDataObjList_getInt(pl, D_00518A10)->_data = beDataObjList_getInt(pl, D_00518980)->_data;
    }
    if (equip) {
        beNDMWItemRevisionBase* w = ((beNDMWItemRevisionBase**)beDataObjList_getObjectList(pl, D_00518A88)->_data->_data)[0];
        s32 recNo = 0;
        s32 skill = 0;

        if (w != NULL) {
            recNo = w->_recNo;
            skill = func_00447C10(w, gr, D_00518860);
        }
        beDataObjList_getInt(pl, D_00518A20)->_data = recNo;
        beDataObjList_getInt(pl, D_00518A30)->_data = skill;
        {
            igObjectList* socket = beDataObjList_getObjectList(pl, D_00518A88)->_data;
            igStringRefList* names = beDataObjList_getStringList(pl, D_00518AA0)->_data;
            s32 n;
            s32 i;

            ClearStrList(names);
            n = socket->_count;
            for (i = 0; i < n; i++) {
                beNDMWItemRevisionBase* item = ((beNDMWItemRevisionBase**)socket->_data)[i];

                if (item != NULL) {
                    const char* model = func_00447CB0(item, gr, D_00518C08);

                    APPEND_STR(names, model)
                }
            }
        }
    }
}

/* Out-of-line destructor of recalcPlayer's beNDMWItemWazaRef (deleting form). */
beNDMWItemWazaRef* func_003FC670(beNDMWItemWazaRef* r, s16 flags) {
    if (r != NULL) {
        igUnref(r->p);
        if (flags > 0) {
            operator delete(r);
        }
    }
    return r;
}

/* Technique record of the player's technique with kind kindNo. */
void beNDMWGameRam_findTechnique(beNDMWGameRam* gr, beDataObjList* pl, s32 kindNo, s32* recNo) {
    s32 i;
    beNDMWItemRevisionBase* w;
    igObjectList* list = beDataObjList_getObjectList(pl, D_00518928)->_data;
    s32 n = list->_count;

    for (i = 0; i < n; i++) {
        w = ((beNDMWItemRevisionBase**)list->_data)[i];

        if (kindNo == w->_revision) {
            *recNo = w->_recNo;
            return;
        }
    }
}

/* Ram numbers of the living members of group (other than exclude) within
   30 units of center (squared distance below 900), stored in *out. */
s32 func_003FC780(igIntListRef* out, beNDMWGameRam* gr, const char* group, s32 exclude, const igVec3f* center) {
    igIntListRef list(func_002CAA70(PoolOf2(gr)), IG_ADOPT);
    s32 n;
    s32 i;

    beMessenger_callGroup(gr->_messenger, group, D_00519030);
    n = beMessenger_getIntCount(gr->_messenger);
    for (i = 0; i < n; i++) {
        s32 ramNo = beMessenger_getInt(gr->_messenger, i);

        if (ramNo != exclude) {
            igMatrix44f m;
            igVec3f pos;

            beMessenger_getMatrix(&m, gr->_messenger, i);
            func_00190A30(&m, &pos);
            pos.x = pos.x - center->x;
            pos.y = pos.y - center->y;
            pos.z = pos.z - center->z;
            if (pos.x * pos.x + pos.y * pos.y + pos.z * pos.z < 900.0f) {
                beMessenger_push(gr->_messenger);
                beMessenger_select(gr->_messenger, group, NULL, ramNo);
                beMessenger_setRunCheck(gr->_messenger, true);
                beMessenger_send(gr->_messenger, D_00519040);
                if (beMessenger_getInt(gr->_messenger, 0) != 0) {
                    s32 k;
                    igDataList* l = list.p;

                    k = l->_count;
                    if (k < l->_capacity) {
                        (*(volatile s32*)&l->_count)++;
                    } else {
                        func_002D6790(l, k + 1, 4);
                    }
                    ((s32*)l->_data)[k] = ramNo;
                }
                beMessenger_pop(gr->_messenger);
            }
        }
    }
    out->p = list.p;
    igIncRef(out->p);
    return 0;
}

#ifdef NON_MATCHING
/* Nonmatching (work in progress): all of the handler is written; ~98% of the
   instructions line up, ~67% exactly. Left: register allocation (the original
   has gr in s4, i.e. four values outrank it, ours s0), stack slots, scheduling.
   Variables shared between cases are declared at function level (C89 style),
   which moved it from 53% to 67%. Formulas: docs/gameram.md, docs/exp.md. */
extern "C" {
extern char D_005187D8[]; /* "BKCHIPDATA" */
extern char D_005187E8[]; /* "BKCHIPNAME" */
extern char D_00518840[]; /* "BIT" */
extern char D_00518A50[]; /* "DISKFOLDER" */
extern char D_00518FA8[]; /* "fire" */
extern char D_00518FB0[]; /* "ice" */
extern char D_00518FB8[]; /* "thunder" */
extern char D_00518FC0[]; /* "dark" */
extern char D_00519020[]; /* "RES_POISON" */
extern char D_005190A8[]; /* "PLAYER" */
extern char D_005190B0[]; /* "x_250healhit00" */
extern char D_005190C0[]; /* "SQ_HEALEFONLY" */
extern char D_005190D0[]; /* "SETEFFECT" */
extern char D_005190E0[]; /* "SET" */
extern char D_005190E8[]; /* "ADD" */
extern char D_005190F0[]; /* "GETMHP" */
extern char D_005190F8[]; /* "GETMP" */
extern char D_00519100[]; /* "GETMMP" */
extern char D_00519108[]; /* "CLR" */
extern char D_00519110[]; /* "IS" */
extern char D_00519118[]; /* "ISMAX" */
extern char D_00519120[]; /* "ISEQUIP" */
extern char D_00519128[]; /* "GET" */
extern char D_00519130[]; /* "DEVICE" */
extern char D_00519138[]; /* "weapon" */
extern char D_00519140[]; /* "armor1" */
extern char D_00519148[]; /* "armor2" */
extern char D_00519150[]; /* "abChip" */
extern char D_00519158[]; /* "DISK" */
extern char D_00519160[]; /* "poison" */
extern char D_00519168[]; /* "MODELCTRL" */
extern char D_00519178[]; /* "i_getitempnl01" */
extern char D_00519188[]; /* "SQ_TECUP" */
extern char D_00519198[]; /* "SQ_LVUP" */
extern char D_005191A0[]; /* "lvnm" */
extern char D_005191A8[]; /* "RES_STUN" */
extern char D_005191B8[]; /* "TIME" */
extern char D_005191C0[]; /* "MHIRITSU" */
extern char D_005191D0[]; /* "TYPE" */
extern char D_005191D8[]; /* "SEQUENCE" */
extern char D_005191E8[]; /* "ONE" */
extern char D_005191F0[]; /* "KOUKA" */
extern char D_005191F8[]; /* "doku" */
extern char D_00519200[]; /* "KOUKARANK" */
extern char D_00519210[]; /* "KOUKATIME" */
extern char D_00519220[]; /* "DAMAGE" */
extern char D_00519228[]; /* "GEDOKU" */
extern char D_00519230[]; /* "sleep" */
extern char D_00519238[]; /* "ashidome" */
extern char D_00519248[]; /* "freeze" */
extern char D_00519250[]; /* "powerdown" */
extern char D_00519260[]; /* "GETPEBASEWORK" */
extern char D_00519270[]; /* "x_250curehit00" */
extern char D_00519280[]; /* "SQ_CUREEFONLY" */
extern char D_00519290[]; /* "PLAYER2" */
extern char D_00519298[]; /* "RAISE" */
extern char D_005192A0[]; /* "SETSPEEDUP" */
extern char D_005192B0[]; /* "x_250uphit00" */
extern char D_005192C0[]; /* "SQ_UPEFONLY" */
extern char D_005192D0[]; /* "SETPOWERUP" */
extern char D_005192E0[]; /* "FIRE" */
extern char D_005192E8[]; /* "ICE" */
extern char D_005192F0[]; /* "THUNDER" */
extern char D_005192F8[]; /* "DARK" */
extern char D_00519300[]; /* "SETREFRACT" */
extern char D_00519470[]; /* "GETDATABASE" */
extern char D_00519480[]; /* "enemy" */
extern char D_00519488[]; /* "DATABASE" */
extern char D_00519498[]; /* "GETDATABASE" */
extern char D_005194A8[]; /* "eneatk" */
extern char D_005194B0[]; /* "DATABASE" */
}

/* ---- beNDMWGameRam_handle ----

   The handler was compiled with an inline depth of 1 (#pragma inline_depth(1)):
   the inline helpers it calls are expanded, but what those call is not, so the
   smart pointer / string accessors one level down are calls, and MWCC emits
   their out-of-line copies right after the function (0x406DA0..0x407050, in
   reverse order of first use). The ones an earlier unit also needed (0x3E9xxx)
   were kept from that unit by the linker. */

extern "C" {

void beNDMWGameRam_current2Base(beNDMWGameRam* gr, s32 player);
s32 beMessenger_getObjectCount(beMessenger* m);
void beMessenger_delaySelect(beMessenger* m, const char* group, const char* info, s32 ramNo);
void beMessenger_delaySend(beMessenger* m, const char* message, s64 delay);
void beMessenger_getMatrix(igMatrix44f* out, beMessenger* m, s32 i);
void func_003F96E0(beNDMWGameRam* gr, s32 player, const char* digimonName, char** name);
void func_003F99F0(beNDMWGameRam* gr, s32 player, const char* digimonName, igObject** name);
igMetaObject* func_003C9C40(void); /* "Message" name table */

igObject* func_003C9BE0(void);                                         /* "FightCom" name table */
igResult func_002FBA30(igObject* table, const char* name, s32* value); /* index of a name */

extern const igVec3f D_004E1058; /* (0, 0, 0) */
extern void* D_00587108;         /* default memory pool (fallback) */
extern void* D_00587110;
extern igMetaObject* D_00585BE0; /* meta used for lookups by name */
extern char* D_004CF360[];       /* "RES_FIRE", "RES_ICE", "RES_THUNDER", "RES_DARK", "RES_POISON" */
extern char* D_004CF380[];       /* weapon element resistance columns of the enemy table */
extern s16 D_004CF590[21];       /* skill-up: skill thresholds */
extern s16 D_004CF390[4][21][3]; /* skill-up: chance by level band, skill row, charged */
extern char* D_004CF5C0[];       /* "<TYPE>_GROW" columns of the shinka table by skill */
extern s32 D_004CF538[];         /* RAISE parameters by technique */
extern s32 D_004CF558[];
extern s32 D_004CF600[];
extern s32 D_004CF620[];

/* Inline helpers of an earlier unit (beNDMWLoadIntf2): their copies are there. */
beDataObjList* func_003E9260(beDataObjListList* l, s32 i);   /* list element */
void func_003E90D0(igObject* o);                             /* incref */
void func_003E9110(igObject* o);                             /* decref */
void func_003E9220(igVec3f* dst, const igVec3f* src);        /* copy */
s32* func_003E9090(igIntList* l);                            /* list data */
void* func_003E9370(void** fallback, void* pool);            /* pool to use */
struct PoolHolderD { void* p; };
struct DataListRefH { beBaseInfoDataList* p; };
beBaseInfoDataList* func_003E93B0(beBaseInfoDataList* const* r); /* get */
void func_003E9360(PoolHolderD* h, void* pool);
beBaseInfoDataList** func_003E92B0(beBaseInfoDataList** dst, DataListRefH* src); /* assign */
void func_0033E9C0(DataListRefH* r, s32 flag);               /* destructor */
beBaseInfoDataList* func_003E92A0(beBaseInfoDataList* const* r); /* get */
igNamedObject* func_003E9280(igObjectList* list, igMetaObject* meta, const char* name); /* find */
beBaseInfoDataList* func_0036AE80(void* pool);               /* new beBaseInfoDataList */

beNDMWItemWeapon* func_003C8C90(void* pool); /* new beNDMWItemWeapon */
igObject* func_003C8B00(void* pool);         /* new armor1 item */
igObject* func_003C8970(void* pool);         /* new armor2 item */
igObject* func_003C87F0(void* pool);         /* new ability chip */
void func_004482C0(beNDMWItemWeapon* item, s32 recNo, s32, s32);
void func_00448410(igObject* item, s32 recNo, s32, s32);
void func_00448560(igObject* item, s32 recNo, s32);
void func_00448660(igObject* item, s32 recNo);
s32 func_00448050(igObjectList* list, s32 recNo); /* index of a disk */
void func_00190A30(const igMatrix44f* m, igVec3f* pos);       /* translation */
void func_00190380(igMatrix44f* m);                           /* identity */
void func_001908F0(igMatrix44f* dst, const igMatrix44f* src); /* copy */
bool func_00447B00(igObject* work);                           /* poisoned */
void func_00447B40(igObject* work, igObject* base);           /* cure */
void func_00229FB0(char** dst, char** src);                   /* assign a pooled string */

struct PoolHolder0 { void* p; };
struct PoolHolder1 { void* p; };
struct PoolHolder2 { void* p; };
struct PoolHolder3 { void* p; };
struct IntListRefH { igIntList* p; };

/* ---- the accessors one level down: these are the 0x406DA0..0x407050 copies ---- */

inline const char* func_00407050(char* const* s) { return *s; }
inline beDataObjList* func_00407040(beDataObjList* const* r) { return *r; }
inline beDataObjListList* func_00407030(beDataObjListList* const* r) { return *r; }
inline StrPool* func_00406FE0(void) {
    if (D_00586F20 == NULL) {
        StrPool* pool = (StrPool*)func_002EB010(0x10);
        if (pool != NULL) {
            pool = func_002EADF0(pool);
        }
        D_00586F20 = pool;
    }
    return D_00586F20;
}
inline void func_00406FA0(char** s) {
    char* p = *s;

    if (p != NULL) {
        StrHeader* h = (StrHeader*)p - 1;
        if (--h->refs == 0) {
            func_0030C940(h->owner, h);
        }
    }
}
inline s32 func_00406F90(beDataObjInt* o) { return o->_data; }
inline igObjectList* func_00406F80(igObjectList* const* r) { return *r; }
inline void func_00406F70(PoolHolder0* h, void* pool) { h->p = pool; }
inline void func_00406F60(PoolHolder1* h, void* pool) { h->p = pool; }
inline void func_00406F50(PoolHolder2* h, void* pool) { h->p = pool; }
inline void func_00406F40(PoolHolder3* h, void* pool) { h->p = pool; }
inline void func_00406F20(igObjectList* l, s32 i) { func_002D6CF0(l, i); }
inline igBoolList* func_00406F10(igBoolList* const* r) { return *r; }
inline bool* func_00406F00(igBoolList* l) { return (bool*)l->_data; }
inline igIntList* func_00406EF0(igIntList* const* r) { return *r; }
inline beDataBase* func_00406EE0(beDataBase* const* r) { return *r; }
inline beDataBase** func_00406E60(beDataBase** r, beDataBase* o) {
    if (o != NULL) {
        o->_refCount++;
    }
    {
        beDataBase* old = *r;

        if (old != NULL) {
            old->_refCount--;
            if ((old->_refCount & 0x7FFFFF) == 0) {
                func_002FF820(old);
            }
        }
    }
    *r = o;
    return r;
}
inline igRandomNumber* func_00406E50(igRandomNumber* const* r) { return *r; }
inline igIntList* func_00406E40(beDataObjIntList* o) { return o->_data; }
inline DataListRefH* func_00406E10(DataListRefH* r, PoolHolderD* pool) {
    r->p = func_0036AE80(pool->p);
    return r;
}
inline beModelCtrlInfoWork* func_00406E00(beModelCtrlInfoWork* const* r) { return *r; }
inline beModelCtrlMoveObjectList* func_00406DF0(beModelCtrlMoveObjectList* const* r) { return *r; }
inline igObject* func_00406DD0(beModelCtrlMoveObjectList* l, s32 i) { return ((igObject**)l->_data)[i]; }
inline igIntList* func_00406DC0(IntListRefH* r) { return r->p; }
inline igStringRefList* func_00406DB0(beDataObjStringList* o) { return o->_data; }
inline char** func_00406DA0(igStringRefList* l) { return (char**)l->_data; }

}

class igRandomNumberV {
public:
    IG_VPAD_22
    virtual u32 next(); /* +0x60 */
};

/* ---- level-1 helpers: expanded in the handler ---- */

/* Local references (constructor and destructor expanded, the reference counting
   inside is a call). */
struct ObjRefL {
    igObject* p;

    ObjRefL(igObject* o) : p(o) { func_003E90D0(p); }
    ObjRefL(const ObjRefL& r) : p(r.p) { func_003E90D0(p); }
    ~ObjRefL() { func_003E9110(p); }
};

/* A reference that takes over a new object. */
struct NewRefL {
    igObject* p;

    NewRefL(igObject* o) : p(o) {}
    ~NewRefL() { func_003E9110(p); }
};

struct IntListRefL {
    igIntList* p;

    IntListRefL() {}
    ~IntListRefL() { func_003E9110((igObject*)p); }
};

/* Pooled string held by a local. */
struct StrRefL {
    char* p;

    StrRefL(const char* s) : p(s == NULL ? NULL : func_002EAF60(func_00406FE0(), s)) {}
    ~StrRefL() { func_00406FA0(&p); }
};

/* The same functions, seen with their by-value reference argument (the caller builds the
   copy as a temporary and passes its address); C++ linkage, mwsplice maps them back. */
extern "C++" {
void func_003F99F0(beNDMWGameRam* gr, s32 player, const char* digimonName, ObjRefL name);
void func_003F96E0(beNDMWGameRam* gr, s32 player, const char* digimonName, StrRefL name);
}

static inline beDataObjList* GrData(beNDMWGameRam* gr, s32 i) {
    return func_003E9260(func_00407030(&gr->_playerDataListList), i);
}

static inline beDataObjList* Common(beNDMWGameRam* gr) {
    return func_00407040(&gr->_commonDataList);
}

static inline const char* Message(beMessengerWork* work) {
    return func_00407050(&work->_message);
}

static inline s32 IntGet(beDataObjInt* o) {
    return func_00406F90(o);
}

static inline u32 Rand(beNDMWGameRam* gr) {
    return ((igRandomNumberV*)func_00406E50(&gr->_system->_random))->next();
}

#define DB_GETTER(name, field, sDb, sName, sGet)                               \
    static inline beDataBase* name(beNDMWGameRam* gr) {                        \
        if (func_00406EE0(&gr->field) == NULL) {                               \
            beMessenger_call(gr->_messenger, sDb, sName, 0, sGet);             \
            func_00406E60(&gr->field, (beDataBase*)beMessenger_getObject(gr->_messenger, 0)); \
        }                                                                      \
        return func_00406EE0(&gr->field);                                      \
    }

DB_GETTER(DBEnemy, _dBEnemy, D_00519488, D_00519480, D_00519470)
DB_GETTER(DBPcTech, _dBPcTech, D_00519460, D_00519458, D_00519448)
DB_GETTER(DBShinka, _dBShinka, D_00519438, D_00519430, D_00519420)
DB_GETTER(DBEneatk, _dBEneatk, D_005194B0, D_005194A8, D_00519498)

static inline s32 Skill(beDataObjList* pl, s32 n) {
    return func_003E9090(func_00406EF0(&beDataObjList_getIntList(pl, D_00518860)->_data))[n];
}

static inline s32 Skill7(beDataObjList* pl) {
    return func_003E9090(func_00406EF0(&beDataObjList_getIntList(pl, D_00518860)->_data))[7];
}

static inline s32 Skill8(beDataObjList* pl) {
    return func_003E9090(func_00406EF0(&beDataObjList_getIntList(pl, D_00518860)->_data))[8];
}

/* A stat lowered on the enemy (its work's DOWNDATA list), 0 without one. */
static inline s32 Down(beNDMWMdlPEBaseInfoWork* ew, s32 i) {
    if (func_00406EF0(&ew->_downDataList) == NULL) {
        return 0;
    }
    return func_003E9090(func_00406EF0(&ew->_downDataList))[i];
}

static inline const char* DigimonName(beDataObjList* pl) {
    return func_00407050(&beDataObjList_getString(pl, D_00518870)->_data);
}

static inline igObjectList* ObjListOf(beDataObjObjectList* o) {
    return func_00406F80(&o->_data);
}

static inline igBoolList* BoolListOf(beDataObjBoolList* o) {
    return func_00406F10(&o->_data);
}

static inline bool BoolAt(igBoolList* l, s32 i) {
    return func_00406F00(l)[i];
}

static inline void BoolSet(igBoolList* l, s32 i, const bool& v) {
    if (l->_count != 0 && i >= 0 && i < l->_count) {
        func_00406F00(l)[i] = v;
    }
}

static inline void ListRemove(igObjectList* l, s32 i) {
    func_00406F20(l, i);
}

static inline igIntList* IntListOf(beDataObjIntList* o) {
    return func_00406E40(o);
}

static inline void IntSet(igIntList* l, s32 i, const s32& v) {
    if (l->_count != 0 && i >= 0 && i < l->_count) {
        func_003E9090(l)[i] = v;
    }
}

static inline igStringRefList* StrListOf(beDataObjStringList* o) {
    return func_00406DB0(o);
}

static inline void StrSet(igStringRefList* l, s32 i, const StrRefL& s) {
    if (l->_count != 0 && i >= 0 && i < l->_count) {
        func_00229FB0(&func_00406DA0(l)[i], (char**)&s.p);
    }
}

static inline s32 SubIndex(igObject* table, StrRefL& name, s32* sub) {
    func_002FBA30(table, func_00407050(&name.p), sub);
    return 0;
}

static inline void AddSelf(IntListRefL& l, s32 v) {
    igIntList* list = func_00406DC0((IntListRefH*)&l);
    s32 k = list->_count;

    if (k < list->_capacity) {
        (*(volatile s32*)&list->_count)++;
    } else {
        func_002D6790(list, k + 1, 4);
    }
    func_003E9090(list)[k] = v;
}

static inline igObject* NewWeapon(beNDMWGameRam* gr) {
    PoolHolder0 h;

    func_00406F70(&h, func_003E9370(&D_00587108, func_00301000(gr)));
    return (igObject*)func_003C8C90(h.p);
}

static inline igObject* NewArmor1(beNDMWGameRam* gr) {
    PoolHolder1 h;

    func_00406F60(&h, func_003E9370(&D_00587108, func_00301000(gr)));
    return func_003C8B00(h.p);
}

static inline igObject* NewArmor2(beNDMWGameRam* gr) {
    PoolHolder2 h;

    func_00406F50(&h, func_003E9370(&D_00587108, func_00301000(gr)));
    return func_003C8970(h.p);
}

static inline igObject* NewAbChip(beNDMWGameRam* gr) {
    PoolHolder3 h;

    func_00406F40(&h, func_003E9370(&D_00587108, func_00301000(gr)));
    return func_003C87F0(h.p);
}

/* The ram's data list, made on first use. */
static inline beBaseInfoDataList* RamDataList(beBaseInfoRam* ram) {
    if (func_003E93B0(&ram->_dataList) == NULL) {
        PoolHolderD pool;
        DataListRefH list;

        func_003E9360(&pool, func_003E9370(&D_00587110, func_00301000(ram)));
        func_00406E10(&list, &pool);
        func_003E92B0(&ram->_dataList, &list);
        func_0033E9C0(&list, -1);
    }
    return func_003E92A0(&ram->_dataList);
}

static inline beModelCtrlInfoWork* WorkOf(beBaseInfoDataList* datas) {
    return func_00406E00(&((beModelCtrlInfoRamData*)func_003E9280(datas, D_00585BE0, D_005191E8))->_work);
}

static inline beModelCtrlMoveObjectList* MovesOf(beModelCtrlInfoWork* w) {
    return func_00406DF0(&w->_moveObjectList);
}

static inline beModelCtrlMoveObject* MoveAt(beModelCtrlMoveObjectList* l, s32 i) {
    return (beModelCtrlMoveObject*)func_00406DD0(l, i);
}

/* ---- code macros (written out in the handler) ---- */

/* Element index of a technique's ZOKUSEI string (-1: none). */
#define ELEM_OF(str, e)                                                        \
    if (strcmp(str, D_00518FA8) == 0) {                                        \
        e = 0;                                                                 \
    } else if (strcmp(str, D_00518FB0) == 0) {                                 \
        e = 1;                                                                 \
    } else if (strcmp(str, D_00518FB8) == 0) {                                 \
        e = 2;                                                                 \
    } else if (strcmp(str, D_00518FC0) == 0) {                                 \
        e = 3;                                                                 \
    } else if (strcmp(str, D_00519160) == 0) {                                 \
        e = 4;                                                                 \
    } else {                                                                   \
        e = -1;                                                                \
    }

#define CLAMP_99999(x)                                                         \
    if (x < 0) {                                                               \
        x = 0;                                                                 \
    } else if (x > 99999) {                                                    \
        x = 99999;                                                             \
    }

/* A vector copied into a temporary (copy constructor out of line: func_003E9220). */
struct Vec3T : igVec3f {
    Vec3T(const igVec3f& v) { func_003E9220(this, &v); }
};

/* By-value views of messenger calls (hidden result pointer / reference argument); C++
   linkage, mwsplice maps them back to the same functions. */
extern "C++" {
void beMessenger_pushVec3f(beMessenger* m, const Vec3T& v);
igMatrix44f beMessenger_getMatrix(beMessenger* m, s32 i);
void func_00190A30(const igMatrix44f& m, igVec3f* pos);
}

/* A model effect on a player (SETEFFECT). */
#define PLAYER_EFFECT(gr, group, pl, player, eff, seq)                         \
    {                                                                          \
                                                                               \
        beMessenger_delaySelect((gr)->_messenger, group, DigimonName(pl), (player)); \
        beMessenger_pushString((gr)->_messenger, eff);                         \
        beMessenger_pushString((gr)->_messenger, seq);                         \
        beMessenger_pushFloat((gr)->_messenger, 1.0f);                         \
        beMessenger_pushVec3f((gr)->_messenger, Vec3T(D_004E1058));            \
        beMessenger_pushBool((gr)->_messenger, false);                         \
        beMessenger_pushBool((gr)->_messenger, true);                          \
        beMessenger_delaySend((gr)->_messenger, D_005190D0, 1);                \
    }

#define HEAL_EFFECT(gr, player) PLAYER_EFFECT(gr, D_005190A8, GrData(gr, player), player, D_005190B0, D_005190C0)

/* The player's resistance to an enemy attack's element (ZOKUSEI). */
#define PLAYER_RES(str, pl, res, elem)                                         \
    if (strcmp(str, D_00518FA8) == 0) {                                        \
        res = beDataObjList_getInt(pl, D_005188E8)->_data;                     \
        elem = 0;                                                              \
    } else if (strcmp(str, D_00518FB0) == 0) {                                 \
        res = beDataObjList_getInt(pl, D_005188F8)->_data;                     \
        elem = 1;                                                              \
    } else if (strcmp(str, D_00518FB8) == 0) {                                 \
        res = beDataObjList_getInt(pl, D_00518908)->_data;                     \
        elem = 2;                                                              \
    } else if (strcmp(str, D_00518FC0) == 0) {                                 \
        res = beDataObjList_getInt(pl, D_00518918)->_data;                     \
        elem = 3;                                                              \
    }

/* Damage spread: 95%..102.5% plus a random part; the top band is critical. */
#define SPREAD(gr, dmg, critical)                                              \
    if (dmg != 0) {                                                            \
        s32 k_ = Rand(gr) & 3;                                                 \
        u32 r_ = Rand(gr) % (u32)(dmg * 25);                                   \
                                                                               \
        switch (k_) {                                                          \
        case 0:                                                                \
            dmg = (s32)(r_ + dmg * 950) / 1000;                                \
            break;                                                             \
        case 1:                                                                \
            dmg = (s32)(r_ + dmg * 975) / 1000;                                \
            break;                                                             \
        case 2:                                                                \
            dmg = (s32)(r_ + dmg * 1000) / 1000;                               \
            break;                                                             \
        case 3:                                                                \
            critical = 1;                                                      \
            dmg = (s32)(r_ + dmg * 1025) / 1000;                               \
            break;                                                             \
        }                                                                      \
    }

/* Status time of a technique against the enemy's SEISHIN (sleep, bind, attack and
   power down): chance per mille from the skill, time = TIME + KASHIKOSA / 2, -1 if
   it fails. */
#define STATUS_TIME(gr, pl, enemy, attack)                                     \
    {                                                                          \
        s32 tech_ = 0;                                                         \
        s32 ok_ = 0;                                                           \
        s32 n_;                                                                \
        s32 skill_;                                                            \
        s32 spirit_;                                                           \
        s32 chance_;                                                           \
        s32 time_;                                                             \
                                                                               \
        beNDMWGameRam_findTechnique(gr, pl, (attack) % 10000, &tech_);         \
        n_ = beDataBase_getInt(DBPcTech(gr), tech_, D_00518860);               \
        skill_ = Skill(pl, n_);                                                \
        spirit_ = beDataBase_getInt(DBEnemy(gr), enemy, D_005188D0);           \
        if (skill_ <= 1000) {                                                  \
            chance_ = (skill_ / 4 + 550) * 100 / spirit_;                      \
        } else if (skill_ <= 2000) {                                           \
            chance_ = ((skill_ - 1000) / 5 + 800) * 100 / spirit_;             \
        } else {                                                               \
            chance_ = ((skill_ - 2000) / 10 + 1000) * 100 / spirit_;           \
        }                                                                      \
        if ((s32)(Rand(gr) % 1000) < chance_) {                                \
            ok_ = 1;                                                           \
        }                                                                      \
        if (ok_) {                                                             \
            s32 wis_ = beDataObjList_getInt(pl, D_005188C0)->_data;            \
                                                                               \
            time_ = beDataBase_getInt(DBPcTech(gr), tech_, D_005191B8) + wis_ / 2; \
        } else {                                                               \
            time_ = 0;                                                         \
        }                                                                      \
        if (time_ == 0) {                                                      \
            time_ = -1;                                                        \
        }                                                                      \
        beMessenger_clearInts((gr)->_messenger);                               \
        beMessenger_pushInt((gr)->_messenger, time_);                          \
    }

/* A stat lowered by a technique: tech.col - skill / 20, at least -80% of the
   enemy's stat; pushed negated. */
#define STAT_DOWN(gr, pl, enemy, attack, col)                                  \
    {                                                                          \
        s32 tech_ = 0;                                                         \
        s32 n_;                                                                \
        s32 skill_;                                                            \
        s32 floor_;                                                            \
        s32 v_;                                                                \
                                                                               \
        beNDMWGameRam_findTechnique(gr, pl, (attack) % 10000, &tech_);         \
        n_ = beDataBase_getInt(DBPcTech(gr), tech_, D_00518860);               \
        skill_ = Skill(pl, n_);                                                \
        floor_ = -beDataBase_getInt(DBEnemy(gr), enemy, col) * 80 / 100;       \
        v_ = beDataBase_getInt(DBPcTech(gr), tech_, col) - skill_ / 20;        \
        if (v_ < floor_) {                                                     \
            v_ = floor_;                                                       \
        }                                                                      \
        beMessenger_clearInts((gr)->_messenger);                               \
        beMessenger_pushInt((gr)->_messenger, -v_);                            \
    }

/* A technique's value, pushed negated. */
#define TECH_VALUE(gr, pl, attack, col)                                        \
    {                                                                          \
        s32 tech_;                                                             \
        s32 v_;                                                                \
                                                                               \
        beNDMWGameRam_findTechnique(gr, pl, (attack) % 10000, &tech_);         \
        v_ = beDataBase_getInt(DBPcTech(gr), tech_, col);                      \
        beMessenger_clearInts((gr)->_messenger);                               \
        beMessenger_pushInt((gr)->_messenger, -v_);                            \
    }

/* An enemy status attack lowers a player stat: clear the old down value, recalc,
   then push stat * (rank + KASHIKOSA*150/SEISHIN/100 - SEISHIN/80, 20..80) / 100. */
#define STAT_DOWN_E2P(gr, player, pl, enemy, atk, idx, col)                    \
    {                                                                          \
        igIntList* down_ = IntListOf(beDataObjList_getIntList(pl, D_00518B78)); \
        s32 rank_;                                                             \
        s32 wis_;                                                              \
        beDataObjInt* spirit_;                                                 \
        s32 pct_;                                                              \
                                                                               \
        IntSet(down_, idx, 0);                                                 \
        beNDMWGameRam_recalcPlayer(gr, player, true);                          \
        rank_ = beDataBase_getInt(DBEneatk(gr), atk, D_00519200);              \
        wis_ = beDataBase_getInt(DBEnemy(gr), enemy, D_005188C0);              \
        spirit_ = beDataObjList_getInt(pl, D_005188D0);                        \
        pct_ = rank_ + wis_ * 150 / spirit_->_data / 100 - spirit_->_data / 80; \
        if (pct_ < 20) {                                                       \
            pct_ = 20;                                                         \
        }                                                                      \
        if (pct_ > 80) {                                                       \
            pct_ = 80;                                                         \
        }                                                                      \
        {                                                                      \
            s32 stat_ = beDataObjList_getInt(pl, col)->_data;                  \
                                                                               \
            beMessenger_clearInts((gr)->_messenger);                           \
            beMessenger_pushInt((gr)->_messenger, stat_ * pct_ / 100);         \
        }                                                                      \
    }

/* Time of an enemy status effect named kouka (freeze, power down). */
#define KOUKA_TIME(gr, pl, enemy, atk, kouka)                                  \
    {                                                                          \
        s32 time_;                                                             \
        const char* k_ = beDataBase_getString(DBEneatk(gr), atk, D_005191F0);  \
                                                                               \
        time_ = 0;                                                             \
        if (strcmp(k_, kouka) == 0) {                                          \
            s32 rank_;                                                         \
            s32 wis_;                                                          \
            s32 spirit_;                                                       \
            u32 r_;                                                            \
            s32 f_;                                                            \
                                                                               \
            rank_ = beDataBase_getInt(DBEneatk(gr), atk, D_00519200);          \
            wis_ = beDataBase_getInt(DBEnemy(gr), enemy, D_005188C0);          \
            spirit_ = beDataObjList_getInt(pl, D_005188D0)->_data;             \
            r_ = Rand(gr) % 10000;                                             \
            if ((s32)r_ < rank_ * 160 / 100 * (f_ = wis_ * 3 / spirit_ + 100 - spirit_ / 40)) { \
                s32 t_ = beDataBase_getInt(DBEneatk(gr), atk, D_00519210);     \
                                                                               \
                time_ = t_ * f_ / 100;                                         \
            }                                                                  \
            if (time_ == 0) {                                                  \
                time_ = -1;                                                    \
            }                                                                  \
        }                                                                      \
        beMessenger_clearInts((gr)->_messenger);                               \
        beMessenger_pushInt((gr)->_messenger, time_);                          \
    }

/* Players near player (matrix from its model). */
#define NEARBY(gr, pl, player, group, exclude, out)                            \
    {                                                                          \
        igVec3f pos_;                                                          \
                                                                               \
        beMessenger_call((gr)->_messenger, D_005190A8, DigimonName(pl), player, D_00519030); \
        func_00190A30(beMessenger_getMatrix((gr)->_messenger, 0), &pos_);      \
        func_003FC780((igIntListRef*)&(out), gr, group, exclude, &pos_);       \
    }

/* The skill a support technique uses (7 and 8 named out). */
#define TECH_SKILL(pl, n, v)                                                   \
    switch (n) {                                                               \
    default:                                                                   \
        v = Skill(pl, n);                                                      \
        break;                                                                 \
    case 7:                                                                    \
        v = Skill(pl, 7);                                                      \
        break;                                                                 \
    case 8:                                                                    \
        v = Skill(pl, 8);                                                      \
        break;                                                                 \
    }

#pragma inline_depth(1)
extern "C" bool beNDMWGameRam_handle(beNDMWGameRam* gr, beBaseInfoRam* ram, beMessengerWork* work) {
    s32 player;
    s32 i;
    beDataObjList* pl;
    s32 attack;
    const char* message = Message(work);

    switch (beMessenger_getMessageIndex(gr->_messenger, (igObject*)func_003C9C40(), message)) {
    case 0: /* GETCOMMON */
        beMessenger_pushObject(gr->_messenger, Common(gr));
        return true;
    case 1: { /* GETPLAYER */
        s32 player = beMessenger_getInt(gr->_messenger, 0);

        beMessenger_pushObject(gr->_messenger, GrData(gr, player));
        return true;
    }
    case 2: /* GETPLAYER1 */
        beMessenger_pushObject(gr->_messenger, GrData(gr, 0));
        return true;
    case 3:
        beMessenger_pushObject(gr->_messenger, GrData(gr, 1));
        return true;
    case 4:
        beMessenger_pushObject(gr->_messenger, GrData(gr, 2));
        return true;
    case 5:
        beMessenger_pushObject(gr->_messenger, GrData(gr, 3));
        return true;
    case 11: { /* RECALCPLAYER */
        s32 player = beMessenger_getInt(gr->_messenger, 0);
        bool equip = true;

        if (beMessenger_getBoolCount(gr->_messenger) != 0) {
            equip = beMessenger_getBool(gr->_messenger, 0);
        }
        beNDMWGameRam_recalcPlayer(gr, player, equip);
        return true;
    }
    case 12: { /* BASE2CURRENT */
        s32 player = beMessenger_getInt(gr->_messenger, 0);

        beNDMWGameRam_base2Current(gr, player, func_003F80E0(gr, beMessenger_getString(gr->_messenger, 0)));
        return true;
    }
    case 13: /* CURRENT2BASE */
        beNDMWGameRam_current2Base(gr, beMessenger_getInt(gr->_messenger, 0));
        return true;
    case 14: { /* WEAPONEQUIPCHECK */
        s32 player = beMessenger_getInt(gr->_messenger, 0);

        beMessenger_pushBool(gr->_messenger, beNDMWGameRam_canLearnSkill(gr, player, beMessenger_getInt(gr->_messenger, 1)));
        return true;
    }
    case 15: /* WEAPONEQUIPSTRONG */
        beNDMWGameRam_equipPowerSocket(gr, beMessenger_getInt(gr->_messenger, 0), true);
        return true;
    case 6: /* GAMEDATAINIT */
        beNDMWGameRam_initData(gr, beMessenger_getInt(gr->_messenger, 0));
        return true;
    case 7: { /* GAMESTART */
        s32 player = beMessenger_getInt(gr->_messenger, 0);
        const char* digimonName = beMessenger_getString(gr->_messenger, 0);

        if (beMessenger_getObjectCount(gr->_messenger) == 1) {
            ObjRefL name(beMessenger_getObject(gr->_messenger, 0));

            func_003F99F0(gr, player, digimonName, name);
        } else if (beMessenger_getStringCount(gr->_messenger) == 2) {
            func_003F96E0(gr, player, digimonName, StrRefL(beMessenger_getString(gr->_messenger, 1)));
        }
        return true;
    }
    case 8: /* GAMEREMOVE */
        beNDMWGameRam_removePlayer(gr, beMessenger_getInt(gr->_messenger, 0));
        return true;
    case 16: { /* DEADRESTART */

        for (i = 0; i < 4; i++) {
            pl = GrData(gr, i);

            if (beDataObjList_getBool(pl, D_00518888)->_data) {
                beDataObjList_getInt(pl, D_00518890)->_data = IntGet(beDataObjList_getInt(pl, D_00518898));
                beDataObjList_getInt(pl, D_005188A0)->_data = IntGet(beDataObjList_getInt(pl, D_005188A8));
            }
        }
        return true;
    }
    case 9: { /* ADDHP */
        player = beMessenger_getInt(gr->_messenger, 0);
        s32 delta = beMessenger_getInt(gr->_messenger, 1);
        s32 effect;
        s32 value;

        if (beMessenger_getIntCount(gr->_messenger) < 3) {
            effect = 0;
        } else {
            effect = beMessenger_getInt(gr->_messenger, 2);
        }
        if (beMessenger_getIntCount(gr->_messenger) < 4) {
            value = -1;
        } else {
            value = beMessenger_getInt(gr->_messenger, 3);
        }
        if (beNDMWGameRam_addHp(gr, player, delta, value) && effect) {
            HEAL_EFFECT(gr, player);
        }
        return true;
    }
    case 10: { /* ADDMP */
        player = beMessenger_getInt(gr->_messenger, 0);
        s32 delta = beMessenger_getInt(gr->_messenger, 1);
        s32 effect = 0;

        if (beMessenger_getIntCount(gr->_messenger) == 3) {
            effect = beMessenger_getInt(gr->_messenger, 2);
        }
        beNDMWGameRam_addMp(gr, player, delta);
        if (effect) {
            HEAL_EFFECT(gr, player);
        }
        return true;
    }
    case 21: { /* CTRLPLAYER */
        const char* kind = beMessenger_getString(gr->_messenger, 0);
        s32 result = 0;

        if (strcmp(kind, D_00518880) == 0) { /* "EXP" */
            const char* op = beMessenger_getString(gr->_messenger, 1);
            player = beMessenger_getInt(gr->_messenger, 0);
            s32 v = beMessenger_getInt(gr->_messenger, 1);
            beDataObjInt* exp = beDataObjList_getInt(GrData(gr, player), D_00518880);

            if (strcmp(op, D_005190E0) == 0) { /* "SET" */
                exp->_data = v;
            } else if (strcmp(op, D_005190E8) == 0) { /* "ADD" */
                exp->_data = v + IntGet(exp);
            }
        } else if (strcmp(kind, D_00518890) == 0) { /* "HP" */
            const char* op = beMessenger_getString(gr->_messenger, 1);
            player = beMessenger_getInt(gr->_messenger, 0);
            pl = GrData(gr, player);

            if (strcmp(op, D_00519040) == 0) { /* "GETHP" */
                result = beDataObjList_getInt(pl, D_00518890)->_data;
            } else if (strcmp(op, D_005190F0) == 0) { /* "GETMHP" */
                result = beDataObjList_getInt(pl, D_00518898)->_data;
            }
        } else if (strcmp(kind, D_005188A0) == 0) { /* "MP" */
            const char* op = beMessenger_getString(gr->_messenger, 1);
            player = beMessenger_getInt(gr->_messenger, 0);
            pl = GrData(gr, player);

            if (strcmp(op, D_005190F8) == 0) { /* "GETMP" */
                result = beDataObjList_getInt(pl, D_005188A0)->_data;
            } else if (strcmp(op, D_00519100) == 0) { /* "GETMMP" */
                result = beDataObjList_getInt(pl, D_005188A8)->_data;
            }
        }
        beMessenger_clearInts(gr->_messenger);
        beMessenger_pushInt(gr->_messenger, result);
        return true;
    }
    case 17: { /* CTRLITEM: target (DEVICE/DISK/FLAG/BIT), op, item type; ints player, id, a, b */
        const char* target = beMessenger_getString(gr->_messenger, 0);
        const char* opName = beMessenger_getString(gr->_messenger, 1);
        const char* type = beMessenger_getString(gr->_messenger, 2);
        player = beMessenger_getInt(gr->_messenger, 0);
        s32 id = beMessenger_getInt(gr->_messenger, 1);
        s32 a = beMessenger_getInt(gr->_messenger, 2);
        s32 b = beMessenger_getInt(gr->_messenger, 3);
        s32 op;

        if (strcmp(opName, D_005190E0) == 0) { /* "SET" */
            op = 0;
        } else if (strcmp(opName, D_00519108) == 0) { /* "CLR" */
            op = 1;
        } else if (strcmp(opName, D_00519110) == 0) { /* "IS" */
            op = 2;
        } else if (strcmp(opName, D_00519118) == 0) { /* "ISMAX" */
            op = 3;
        } else if (strcmp(opName, D_00519120) == 0) { /* "ISEQUIP" */
            op = 4;
        } else if (strcmp(opName, D_00519128) == 0) { /* "GET" */
            op = 5;
        } else if (strcmp(opName, D_005190E8) == 0) { /* "ADD" */
            op = 6;
        } else {
            return true;
        }
        if (strcmp(target, D_00519130) == 0) { /* "DEVICE" */
            igObjectList* list =
                ObjListOf(beDataObjList_getObjectList(GrData(gr, player), D_00518A40));
            s32 kind = 0;

            if (op != 3) {
                if (strcmp(type, D_00519138) == 0) { /* "weapon" */
                    kind = 0;
                } else if (strcmp(type, D_00519140) == 0) { /* "armor1" */
                    kind = 1;
                } else if (strcmp(type, D_00519148) == 0) { /* "armor2" */
                    kind = 2;
                } else if (strcmp(type, D_00519150) == 0) { /* "abChip" */
                    kind = 3;
                } else {
                    return true;
                }
            }
            switch (op) {
            case 0: /* add a device (36 at most) */
                if (list->_count == 36) {
                    beMessenger_pushBool(gr->_messenger, false);
                } else {
                    switch (kind) {
                    case 0: {
                        igObject* o = NewWeapon(gr);
                        NewRefL item(o);

                        func_004482C0((beNDMWItemWeapon*)item.p, id, b, a);
                        func_00301E30(list, item.p);
                        break;
                    }
                    case 1: {
                        igObject* o = NewArmor1(gr);
                        NewRefL item(o);

                        func_00448410(item.p, id, b, a);
                        func_00301E30(list, item.p);
                        break;
                    }
                    case 2: {
                        igObject* o = NewArmor2(gr);
                        NewRefL item(o);

                        func_00448560(item.p, id, a);
                        func_00301E30(list, item.p);
                        break;
                    }
                    case 3: {
                        igObject* o = NewAbChip(gr);
                        NewRefL item(o);

                        func_00448660(item.p, id);
                        func_00301E30(list, item.p);
                        break;
                    }
                    }
                    beMessenger_pushBool(gr->_messenger, true);
                }
                break;
            case 1: { /* remove an unequipped device */
                s32 n = list->_count;
                bool done = false;

                for (i = 0; i < n; i++) {
                    beNDMWItemBase* it = ((beNDMWItemBase**)list->_data)[i];

                    if (kind == it->_kindNo && id == it->_recNo && it->_isEquip == false) {
                        func_003E9110((igObject*)func_003E9260((beDataObjListList*)list, i));
                        ListRemove(list, i);
                        done = true;
                        ((igObject**)list->_data)[list->_count] = NULL;
                        break;
                    }
                }
                beMessenger_pushBool(gr->_messenger, done);
                break;
            }
            case 2: { /* has the device */
                s32 n = list->_count;

                for (i = 0; i < n; i++) {
                    beNDMWItemBase* it = ((beNDMWItemBase**)list->_data)[i];

                    if (kind == it->_kindNo && id == it->_recNo) {
                        beMessenger_pushBool(gr->_messenger, true);
                        return true;
                    }
                }
                beMessenger_pushBool(gr->_messenger, false);
                break;
            }
            case 3: /* device folder full */
                if (list->_count == 36) {
                    beMessenger_pushBool(gr->_messenger, true);
                } else {
                    beMessenger_pushBool(gr->_messenger, false);
                }
                break;
            case 4: { /* the device is equipped */
                s32 n = list->_count;
                bool equipped = false;

                for (i = 0; i < n; i++) {
                    beNDMWItemBase* it = ((beNDMWItemBase**)list->_data)[i];

                    if (kind == it->_kindNo && id == it->_recNo && it->_isEquip) {
                        equipped = true;
                        break;
                    }
                }
                beMessenger_pushBool(gr->_messenger, equipped);
                break;
            }
            }
        } else if (strcmp(target, D_00519158) == 0) { /* "DISK": count 0..9 */
            igObjectList* list =
                ObjListOf(beDataObjList_getObjectList(GrData(gr, player), D_00518A50));

            switch (op) {
            case 0: {
                beNDMWItemDisk* d = (beNDMWItemDisk*)func_003E9260((beDataObjListList*)list, func_00448050(list, id));

                if (d->_kazu == 9) {
                    beMessenger_pushBool(gr->_messenger, false);
                } else {
                    s32 k = d->_kazu + b;

                    if (k < 10) {
                        d->_kazu = k;
                    } else {
                        d->_kazu = 9;
                    }
                    beMessenger_pushBool(gr->_messenger, true);
                }
                break;
            }
            case 1: {
                beNDMWItemDisk* d = (beNDMWItemDisk*)func_003E9260((beDataObjListList*)list, func_00448050(list, id));

                if (d->_kazu == 0) {
                    beMessenger_pushBool(gr->_messenger, false);
                } else {
                    s32 k = d->_kazu - b;

                    if (k >= 0) {
                        d->_kazu = k;
                    } else {
                        d->_kazu = 0;
                    }
                    beMessenger_pushBool(gr->_messenger, true);
                }
                break;
            }
            case 2:
                if (((beNDMWItemDisk*)func_003E9260((beDataObjListList*)list, func_00448050(list, id)))->_kazu != 0) {
                    beMessenger_pushBool(gr->_messenger, true);
                } else {
                    beMessenger_pushBool(gr->_messenger, false);
                }
                break;
            case 3:
                if (((beNDMWItemDisk*)func_003E9260((beDataObjListList*)list, func_00448050(list, id)))->_kazu == 9) {
                    beMessenger_pushBool(gr->_messenger, true);
                } else {
                    beMessenger_pushBool(gr->_messenger, false);
                }
                break;
            }
        } else if (strcmp(target, D_00518798) == 0) { /* "FLAG": game flags in the common data */
            igBoolList* flags =
                BoolListOf(beDataObjList_getBoolList(Common(gr), D_005187A0));

            switch (op) {
            case 0:
                if (BoolAt(flags, id)) {
                    beMessenger_pushBool(gr->_messenger, false);
                } else {
                    BoolSet(flags, id, true);
                    beMessenger_pushBool(gr->_messenger, true);
                }
                break;
            case 1:
                if (!BoolAt(flags, id)) {
                    beMessenger_pushBool(gr->_messenger, false);
                } else {
                    BoolSet(flags, id, false);
                    beMessenger_pushBool(gr->_messenger, true);
                }
                break;
            case 2:
                beMessenger_pushBool(gr->_messenger, BoolAt(flags, id));
                break;
            }
        } else if (strcmp(target, D_00518840) == 0) { /* "BIT": money, 0..9999999 */
            pl = GrData(gr, player);

            switch (op) {
            case 0:
                beDataObjList_getInt(pl, D_00518840)->_data = id;
                break;
            case 5:
                beMessenger_clearInts(gr->_messenger);
                beMessenger_pushInt(gr->_messenger, beDataObjList_getInt(pl, D_00518840)->_data);
                break;
            case 6: {
                s32 v = id + beDataObjList_getInt(pl, D_00518840)->_data;

                if (v < 0) {
                    v = 0;
                } else if (v > 9999999) {
                    v = 9999999;
                }
                beDataObjList_getInt(pl, D_00518840)->_data = v;
                break;
            }
            }
        }
        return true;
    }
    case 19: /* DISPFIGHTDATA */
        gr->_isDispFightData = beMessenger_getBool(gr->_messenger, 0);
        return true;
    case 18: { /* FIGHTCALC: sub-message in string 0 */
        StrRefL name(beMessenger_getString(gr->_messenger, 0));
        s32 sub;

        SubIndex(func_003C9BE0(), name, &sub);
        switch (sub) {
        case 1: { /* P2EISHITCHECK */
            player = beMessenger_getInt(gr->_messenger, 0);
            s32 enemy = beMessenger_getInt(gr->_messenger, 1);
            attack = beMessenger_getInt(gr->_messenger, 2);
            beNDMWMdlPEBaseInfoWork* ew = (beNDMWMdlPEBaseInfoWork*)beMessenger_getObject(gr->_messenger, 0);

            switch (attack / 10000) {
            case 4:
                beMessenger_pushBool(gr->_messenger, true);
                break;
            default: {
                pl = GrData(gr, player);
                s32 skillNo = beDataObjList_getInt(pl, D_00518A30)->_data;
                s32 speed = beDataObjList_getInt(pl, D_005188D8)->_data;
                s32 skill = Skill(pl, skillNo);
                s32 hit = beDataObjList_getInt(pl, D_00518950)->_data;
                s32 enemySpeed;
                s32 down;

                enemySpeed = beDataBase_getInt(DBEnemy(gr), enemy, D_005188D8);
                down = Down(ew, 3);
                if ((s32)(Rand(gr) % 10000) <
                    hit * (speed * 30 + skill * 20 + 6000) / (enemySpeed * (100 - down) / 100)) {
                    beMessenger_pushBool(gr->_messenger, true);
                } else {
                    beMessenger_pushBool(gr->_messenger, false);
                }
            }
            }
            break;
        }
        case 2: { /* P2EISCRITICAL */
            player = beMessenger_getInt(gr->_messenger, 0);
            s32 enemy = beMessenger_getInt(gr->_messenger, 1);
            attack = beMessenger_getInt(gr->_messenger, 2);
            pl = GrData(gr, player);
            bool critical;

            if (attack / 10000 != 4) {
                s32 rate = beDataObjList_getInt(pl, D_00518960)->_data;

                if ((s32)(Rand(gr) % 200) < rate) {
                    critical = true;
                } else {
                    critical = false;
                }
            } else {
                s32 tech;
                const char* elemName;
                s32 elem;
                s32 rate;

                beNDMWGameRam_findTechnique(gr, pl, attack % 1000, &tech);
                elemName = beDataBase_getString(DBPcTech(gr), tech, D_00519018);
                ELEM_OF(elemName, elem);
                rate = beDataBase_getInt(DBPcTech(gr), tech, D_00518960);
                if (elem >= 0) {
                    s32 res;

                    if (elem >= 0) {
                        res = beDataBase_getInt(DBEnemy(gr), enemy, D_004CF360[elem]);
                    } else {
                        res = 0;
                    }
                    if ((s32)(Rand(gr) % 1000) < (rate + 100) * (10000 / (res * 12 / 10 + 50)) / 100) {
                        critical = true;
                    } else {
                        critical = false;
                    }
                } else {
                    if ((s32)(Rand(gr) % 100) < rate) {
                        critical = true;
                    } else {
                        critical = false;
                    }
                }
            }
            beMessenger_pushBool(gr->_messenger, critical);
            break;
        }
        case 5: { /* P2EDAMAGE: player -> enemy */
            player = beMessenger_getInt(gr->_messenger, 0);
            s32 enemy = beMessenger_getInt(gr->_messenger, 1);
            attack = beMessenger_getInt(gr->_messenger, 2);
            s32 charged = beMessenger_getInt(gr->_messenger, 3);
            beNDMWMdlPEBaseInfoWork* ew = (beNDMWMdlPEBaseInfoWork*)beMessenger_getObject(gr->_messenger, 0);
            pl = GrData(gr, player);
            s32 kind = attack / 10000;
            s32 elem = -1;
            s32 skill;
            s32 dmg;
            s32 critical;

            if (kind != 4) {
                s32 skillNo = beDataObjList_getInt(pl, D_00518A30)->_data;
                s32 str = beDataObjList_getInt(pl, D_005188B0)->_data;
                s32 down;

                skill = Skill(pl, skillNo);
                down = Down(ew, 5);
                dmg = (str * 6 + skill * 4) / (beDataBase_getInt(DBEnemy(gr), enemy, D_005188B8) - down);
                CLAMP_99999(dmg);
                if (dmg != 0) {
                    elem = beDataObjList_getInt(pl, D_005189F0)->_data;
                    if (elem != -1) {
                        s32 res;

                        res = beDataBase_getInt(DBEnemy(gr), enemy, D_004CF380[elem]);
                        if (res < (s32)(Rand(gr) % 1000)) {
                            dmg += dmg * (1000 - res) / 1000;
                        }
                    }
                }
            } else {
                s32 tech;
                s32 power;

                beNDMWGameRam_findTechnique(gr, pl, attack % 1000, &tech);
                power = -beDataBase_getInt(DBPcTech(gr), tech, D_00518890);
                if (power != 0) {
                    s32 spirit;

                    {
                    s32 n = beDataBase_getInt(DBPcTech(gr), tech, D_00518860);

                    skill = Skill(pl, n);
                }
                    spirit = beDataBase_getInt(DBEnemy(gr), enemy, D_005188D0);
                    dmg = (skill * 5 + power * 20 + beDataObjList_getInt(pl, D_005188C0)->_data * 10) / spirit;
                    CLAMP_99999(dmg);
                } else {
                    dmg = 0;
                }
                if (dmg != 0) {
                    const char* elemName;
                    s32 res;

                    elemName = beDataBase_getString(DBPcTech(gr), tech, D_00519018);
                    ELEM_OF(elemName, elem);
                    if (elem >= 0) {
                        res = beDataBase_getInt(DBEnemy(gr), enemy, D_004CF360[elem]);
                    } else {
                        res = 0;
                    }
                    dmg = dmg * (1000 - res) / 1000;
                }
            }
            CLAMP_99999(dmg);
            critical = 0;
            if (dmg != 0) {
                if (charged != 0) {
                    s32 bonus;

                    if (skill <= 1000) {
                        bonus = dmg * (skill / 20 + 30) / 100;
                    } else if (skill <= 2000) {
                        bonus = dmg * ((skill - 1000) / 50 + 80) / 100;
                    } else {
                        bonus = dmg * ((skill - 2000) / 100 + 100) / 100;
                    }
                    dmg += bonus;
                }
                if (dmg != 0) {
                    s32 k = Rand(gr) & 3;
                    u32 r = Rand(gr) % (u32)(dmg * 25);

                    switch (k) {
                    case 0:
                        dmg = (s32)(r + dmg * 950) / 1000;
                        break;
                    case 1:
                        dmg = (s32)(r + dmg * 975) / 1000;
                        break;
                    case 2:
                        dmg = (s32)(r + dmg * 1000) / 1000;
                        break;
                    case 3:
                        critical = 1;
                        dmg = (s32)(r + dmg * 1025) / 1000;
                        break;
                    }
                }
                CLAMP_99999(dmg);
                if (kind != 4) {
                    s32 drain = beDataObjList_getInt(pl, D_00518A00)->_data;

                    if (drain != 0) {
                        beNDMWGameRam_addHp(gr, player, dmg * drain / 100, -1);
                    }
                    drain = beDataObjList_getInt(pl, D_00518A08)->_data;
                    if (drain != 0) {
                        beNDMWGameRam_addMp(gr, player, dmg * drain / 100);
                    }
                }
            }
            beMessenger_clearInts(gr->_messenger);
            beMessenger_pushInt(gr->_messenger, dmg);
            beMessenger_pushInt(gr->_messenger, elem);
            beMessenger_pushInt(gr->_messenger, critical);
            break;
        }
        case 21: { /* GETPATKZOKUSEI: element of a player's attack */
            player = beMessenger_getInt(gr->_messenger, 0);
            attack = beMessenger_getInt(gr->_messenger, 1);
            pl = GrData(gr, player);
            s32 elem;

            if (attack / 10000 != 4) {
                elem = beDataObjList_getInt(pl, D_005189F0)->_data;
            } else {
                s32 tech;
                const char* elemName;

                beNDMWGameRam_findTechnique(gr, pl, attack % 10000, &tech);
                elemName = beDataBase_getString(DBPcTech(gr), tech, D_00519018);
                ELEM_OF(elemName, elem);
            }
            beMessenger_clearInts(gr->_messenger);
            beMessenger_pushInt(gr->_messenger, elem);
            break;
        }
        case 13: { /* P2ESKILLUP: chance to raise the skill used */
            player = beMessenger_getInt(gr->_messenger, 0);
            attack = beMessenger_getInt(gr->_messenger, 1);
            s32 charged;
            s32 level;
            s32 cap;
            s32 kind;
            s32 skillNo;
            s32 skill;

            beMessenger_getInt(gr->_messenger, 2);
            charged = beMessenger_getInt(gr->_messenger, 3);
            pl = GrData(gr, player);
            level = beDataObjList_getInt(pl, D_00518858)->_data;
            if (level <= 36) {
                cap = level * 30;
            } else if (level <= 107) {
                cap = level * 20 + 360;
            } else if (level <= 227) {
                cap = level * 10 + 1430;
            } else {
                cap = level * 8 + 1884;
            }
            kind = attack / 10000;
            if (kind != 9 && kind != 8 && kind != 4) {
                skillNo = beDataObjList_getInt(pl, D_00518A30)->_data;
                skill = Skill(pl, skillNo);
            } else {
                s32 tech;

                beNDMWGameRam_findTechnique(gr, pl, attack % 10000, &tech);
                skillNo = beDataBase_getInt(DBPcTech(gr), tech, D_00518860);
                skill = Skill(pl, skillNo);
            }
            if (skill < cap) {
                s32 band;
                s32 col;
                s32 row;
                s32 chance;
                s32 permille;

                if (level <= 36) {
                    band = 0;
                } else if (level <= 107) {
                    band = 1;
                } else if (level <= 227) {
                    band = 2;
                } else {
                    band = 3;
                }
                col = 0;
                if (charged == 1) {
                    col = 2;
                }
                for (i = 0; i < 21; i++) {
                    if (D_004CF590[i] >= skill) {
                        row = i;
                        break;
                    }
                }
                chance = D_004CF390[band][row][col];
                if (kind != 9 && kind != 8 && kind != 4) {
                    s32 digimon = beDataObjList_getInt(pl, D_00518848)->_data;

                    permille = chance * beDataBase_getInt(DBShinka(gr), digimon, D_004CF5C0[skillNo]) * 500 / 1000;
                } else {
                    permille = chance * 100;
                }
                if ((s32)(Rand(gr) % 100000) < permille) {
                    igIntList* skills = IntListOf(beDataObjList_getIntList(pl, D_00518860));

                    IntSet(skills, skillNo, skill + 1);
                    beNDMWGameRam_recalcPlayer(gr, player, true);
                    beNDMWGameRam_current2Base(gr, player);
                    PLAYER_EFFECT(gr, D_00519168, pl, player, D_00519178, D_00519188);
                }
            }
            break;
        }
        case 6: { /* P2EEXPUP: enemy defeated */
            s32 killer = beMessenger_getInt(gr->_messenger, 0);
            s32 enemy = beMessenger_getInt(gr->_messenger, 1);
            s32 ninzu = beDataObjList_getInt(Common(gr), D_00518760)->_data;
            s32 killerExp = 0;

            for (i = 0; i < 4; i++) {
                pl = GrData(gr, i);

                if (beDataObjList_getBool(pl, D_00518888)->_data && beDataObjList_getInt(pl, D_00518890)->_data) {
                    s32 level = beDataObjList_getInt(pl, D_00518858)->_data;

                    if (level != 999) {
                        s32 exp;
                        s32 rate;
                        s32 share;
                        s32 total;
                        bool up;

                        exp = beDataBase_getInt(DBEnemy(gr), enemy, D_00518880);
                        rate = beDataObjList_getInt(pl, D_005189D0)->_data;
                        if (rate != 0) {
                            exp = exp * (rate + 100) / 100;
                        }
                        switch (ninzu) {
                        default:
                            share = exp * 100 / 2;
                            break;
                        case 2:
                            share = exp * 140 / 3;
                            break;
                        case 3:
                            share = exp * 160 / 4;
                            break;
                        case 4:
                            share = exp * 200 / 5;
                            break;
                        }
                        if (i == killer) {
                            share = share / 50;
                            killerExp = share;
                        } else {
                            share = share / 100;
                        }
                        total = beDataObjList_getInt(pl, D_00518880)->_data + share;
                        beDataObjList_getInt(pl, D_00518880)->_data = total;
                        up = false;
                        while (total >= beDataObjList_getInt(pl, D_00518938)->_data) {
                            level++;
                            beDataObjList_getInt(pl, D_00518858)->_data = level;
                            beNDMWGameRam_recalcPlayer(gr, i, true);
                            beNDMWGameRam_current2Base(gr, i);
                            up = true;
                            if (level == 999) {
                                break;
                            }
                        }
                        if (up) {
                            s32 mhp = beDataObjList_getInt(pl, D_00518898)->_data;
                            s32 mmp = beDataObjList_getInt(pl, D_005188A8)->_data;

                            PLAYER_EFFECT(gr, D_00519168, pl, i, D_00519178, D_00519198);
                            beDataObjList_getInt(pl, D_00518890)->_data = mhp;
                            beDataObjList_getInt(pl, D_005188A0)->_data = mmp;
                            beMessenger_delaySelect(gr->_messenger, D_00518C50, D_005191A0, i);
                            beMessenger_pushInt(gr->_messenger, level);
                            beMessenger_delaySend(gr->_messenger, D_00518C68, 1);
                            beMessenger_delaySelect(gr->_messenger, D_00518C28, D_00518C38, i);
                            beMessenger_pushFloat(gr->_messenger, 100.0f);
                            beMessenger_delaySend(gr->_messenger, D_00518C40, 1);
                            beMessenger_delaySelect(gr->_messenger, D_00518C50, D_00518C60, i);
                            beMessenger_pushInt(gr->_messenger, mhp);
                            beMessenger_delaySend(gr->_messenger, D_00518C68, 1);
                            beMessenger_delaySelect(gr->_messenger, D_00518C28, D_00518C78, i);
                            beMessenger_pushFloat(gr->_messenger, 100.0f);
                            beMessenger_delaySend(gr->_messenger, D_00518C40, 1);
                            beMessenger_delaySelect(gr->_messenger, D_00518C50, D_00518C80, i);
                            beMessenger_pushInt(gr->_messenger, mmp);
                            beMessenger_delaySend(gr->_messenger, D_00518C68, 1);
                        }
                    }
                }
            }
            beMessenger_clearInts(gr->_messenger);
            beMessenger_pushInt(gr->_messenger, killerExp);
            break;
        }
        case 14: { /* P2ESTUNTIME */
            player = beMessenger_getInt(gr->_messenger, 0);
            s32 enemy = beMessenger_getInt(gr->_messenger, 1);
            s32 time = beMessenger_getInt(gr->_messenger, 2);
            s32 power = beMessenger_getInt(gr->_messenger, 3);
            s32 rate;
            s32 res;
            s32 stun;

            beMessenger_getInt(gr->_messenger, 4);
            rate = beDataObjList_getInt(GrData(gr, player), D_00518970)->_data;
            res = beDataBase_getInt(DBEnemy(gr), enemy, D_005191A8);
            stun = 0;
            if ((s32)(Rand(gr) % 100) < (rate + power * 50 / time + 100 - res) / 2) {
                stun = (200 - res) * (power * 30 / time + rate * 3) / 400;
            }
            beMessenger_clearInts(gr->_messenger);
            beMessenger_pushInt(gr->_messenger, stun);
            break;
        }
        case 15: { /* P2EPOISONTIME */
            StrRefL what(beMessenger_getString(gr->_messenger, 1));
            player = beMessenger_getInt(gr->_messenger, 0);
            s32 enemy = beMessenger_getInt(gr->_messenger, 1);
            attack = beMessenger_getInt(gr->_messenger, 2);
            pl = GrData(gr, player);

            if (strcmp(what.p, D_005191B8) == 0) { /* "TIME" */
                s32 tech = 0;
                s32 ok = 0;
                s32 skill;
                s32 res;
                s32 chance;
                s32 time;

                beNDMWGameRam_findTechnique(gr, pl, attack % 10000, &tech);
                {
                    s32 n = beDataBase_getInt(DBPcTech(gr), tech, D_00518860);

                    skill = Skill(pl, n);
                }
                res = beDataBase_getInt(DBEnemy(gr), enemy, D_00519020);
                if (skill <= 1000) {
                    chance = (skill / 4 + 750) * (1000 - res) / 1000;
                } else if (skill <= 2000) {
                    chance = ((skill - 1000) / 5 + 1000) * (1000 - res) / 1000;
                } else {
                    chance = ((skill - 2000) / 10 + 1200) * (1000 - res) / 1000;
                }
                if ((s32)(Rand(gr) % 1000) < chance) {
                    ok = 1;
                }
                if (ok) {
                    s32 wis = beDataObjList_getInt(pl, D_005188C0)->_data;

                    time = beDataBase_getInt(DBPcTech(gr), tech, D_005191B8) + wis / 2;
                } else {
                    time = 0;
                }
                if (time == 0) {
                    time = -1;
                }
                beMessenger_clearInts(gr->_messenger);
                beMessenger_pushInt(gr->_messenger, time);
            } else {
                s32 tech;
                s32 v;

                beNDMWGameRam_findTechnique(gr, pl, attack % 10000, &tech);
                v = -(beDataBase_getInt(DBPcTech(gr), tech, D_00518890) / 5);
                beMessenger_clearInts(gr->_messenger);
                beMessenger_pushInt(gr->_messenger, v);
            }
            break;
        }
        case 16: { /* P2ESLEEPTIME */
            player = beMessenger_getInt(gr->_messenger, 0);
            s32 enemy = beMessenger_getInt(gr->_messenger, 1);
            attack = beMessenger_getInt(gr->_messenger, 2);
            pl = GrData(gr, player);

            STATUS_TIME(gr, pl, enemy, attack);
            break;
        }
        case 17: { /* P2EBINDTIME */
            player = beMessenger_getInt(gr->_messenger, 0);
            s32 enemy = beMessenger_getInt(gr->_messenger, 1);
            attack = beMessenger_getInt(gr->_messenger, 2);
            pl = GrData(gr, player);

            STATUS_TIME(gr, pl, enemy, attack);
            break;
        }
        case 18: { /* P2EATKDWNTIME: "TIME" or the stat it lowers */
            StrRefL what(beMessenger_getString(gr->_messenger, 1));
            player = beMessenger_getInt(gr->_messenger, 0);
            s32 enemy = beMessenger_getInt(gr->_messenger, 1);
            attack = beMessenger_getInt(gr->_messenger, 2);
            pl = GrData(gr, player);

            if (strcmp(what.p, D_005191B8) == 0) { /* "TIME" */
                STATUS_TIME(gr, pl, enemy, attack);
            } else if (strcmp(what.p, D_005188D8) == 0) { /* "SUBAYASA" */
                STAT_DOWN(gr, pl, enemy, attack, D_005188D8);
            } else if (strcmp(what.p, D_00518960) == 0) { /* "CHIMEIRITSU" */
                TECH_VALUE(gr, pl, attack, D_00518960);
            } else if (strcmp(what.p, D_00518970) == 0) { /* "MAHIRITSU" */
                TECH_VALUE(gr, pl, attack, D_005191C0);
            } else if (strcmp(what.p, D_00518980) == 0) { /* "KAIHIRITSU" */
                TECH_VALUE(gr, pl, attack, D_00518980);
            }
            break;
        }
        case 19: { /* P2EPOWDWNTIME */
            StrRefL what(beMessenger_getString(gr->_messenger, 1));
            player = beMessenger_getInt(gr->_messenger, 0);
            s32 enemy = beMessenger_getInt(gr->_messenger, 1);
            attack = beMessenger_getInt(gr->_messenger, 2);
            pl = GrData(gr, player);

            if (strcmp(what.p, D_005191B8) == 0) { /* "TIME" */
                STATUS_TIME(gr, pl, enemy, attack);
            } else if (strcmp(what.p, D_005188B0) == 0) { /* "TSUYOSA" */
                STAT_DOWN(gr, pl, enemy, attack, D_005188B0);
            } else if (strcmp(what.p, D_005188B8) == 0) { /* "MAMORI" */
                STAT_DOWN(gr, pl, enemy, attack, D_005188B8);
            }
            break;
        }
        case 0: { /* E2PISHITCHECK: enemy hits player */
            player = beMessenger_getInt(gr->_messenger, 0);
            s32 enemy = beMessenger_getInt(gr->_messenger, 1);
            attack = beMessenger_getInt(gr->_messenger, 2);
            pl = GrData(gr, player);
            s32 atk = attack % 10000;
            bool hit = false;

            if (beDataBase_getInt(DBEneatk(gr), atk, D_005191D0) != 0) {
                hit = true;
            } else {
                s32 guard = beDataObjList_getInt(pl, D_00518990)->_data;

                if ((s32)(Rand(gr) % 100) >= guard) {
                    s32 speed = beDataObjList_getInt(pl, D_005188D8)->_data;
                    s32 dodge = beDataObjList_getInt(pl, D_00518980)->_data;
                    s32 enemySpeed;
                    s32 acc;

                    enemySpeed = beDataBase_getInt(DBEnemy(gr), enemy, D_005188D8);
                    acc = beDataBase_getInt(DBEneatk(gr), atk, D_00518950);
                    if ((s32)(Rand(gr) % 100) < acc * enemySpeed / (speed * (dodge + 100) / 100)) {
                        hit = true;
                    }
                }
            }
            beMessenger_clearBools(gr->_messenger);
            beMessenger_pushBool(gr->_messenger, hit);
            break;
        }
        case 3: { /* E2PDAMAGE: enemy -> player */
            player = beMessenger_getInt(gr->_messenger, 0);
            s32 enemy = beMessenger_getInt(gr->_messenger, 1);
            attack = beMessenger_getInt(gr->_messenger, 2);
            beNDMWMdlPEBaseInfoWork* ew = (beNDMWMdlPEBaseInfoWork*)beMessenger_getObject(gr->_messenger, 0);
            pl = GrData(gr, player);
            s32 atk = attack % 10000;
            s32 type;
            s32 dmg;
            s32 elem;
            s32 critical;

            type = beDataBase_getInt(DBEneatk(gr), atk, D_005191D0);
            dmg = 0;
            elem = -1;
            switch (type) {
            case 0: { /* physical */
                s32 def = beDataObjList_getInt(pl, D_005188B8)->_data;
                s32 down;
                s32 str;
                const char* elemName;
                s32 res;

                down = Down(ew, 4);
                str = beDataBase_getInt(DBEnemy(gr), enemy, D_005188B0) - down;
                dmg = str * beDataBase_getInt(DBEneatk(gr), atk, D_005188B0) / def;
                CLAMP_99999(dmg);
                elemName = beDataBase_getString(DBEneatk(gr), atk, D_00519018);
                res = 0;
                PLAYER_RES(elemName, pl, res, elem);
                if (res != 0) {
                    if ((s32)(Rand(gr) % 1000) >= res / 5) {
                        s32 bonus;

                        if (res <= 1000) {
                            bonus = dmg * ((1000 - res) / 20 + 50) / 100;
                        } else if (res <= 5000) {
                            bonus = dmg * ((5000 - res) / 80) / 100;
                        } else {
                            bonus = 0;
                        }
                        dmg += bonus;
                    }
                }
                if (dmg != 0) {
                    s32 critDown;
                    s32 crit;
                    s32 atkCrit;

                    critDown = Down(ew, 1);
                    crit = beDataBase_getInt(DBEnemy(gr), enemy, D_00518960) - critDown;
                    atkCrit = beDataBase_getInt(DBEneatk(gr), atk, D_00518960);
                    if ((s32)(Rand(gr) % 10000) >= (100 - crit) * (100 - atkCrit)) {
                        dmg += dmg * (atkCrit / 2 + 90) / 100;
                    }
                }
                break;
            }
            case 1: { /* magic */
                s32 power;

                power = beDataBase_getInt(DBEneatk(gr), atk, D_005188B0);
                if (power != 0) {
                    s32 wis;
                    const char* elemName;
                    s32 res;

                    wis = beDataBase_getInt(DBEnemy(gr), enemy, D_005188C0);
                    dmg = wis * power / beDataObjList_getInt(pl, D_005188D0)->_data;
                    CLAMP_99999(dmg);
                    res = 0;
                    elemName = beDataBase_getString(DBEneatk(gr), atk, D_00519018);
                    PLAYER_RES(elemName, pl, res, elem);
                    if (res != 0) {
                        if (res <= 1000) {
                            dmg = dmg * ((1000 - res) / 20 + 50) / 100;
                        } else if (res <= 4200) {
                            dmg = dmg * ((5000 - res) / 80) / 100;
                        } else {
                            dmg = dmg / 10;
                        }
                    }
                    if (dmg != 0) {
                        s32 atkCrit;

                        atkCrit = beDataBase_getInt(DBEneatk(gr), atk, D_00518960);
                        if (atkCrit != 0) {
                            s32 limit;

                            if (res <= 1000) {
                                limit = (1000 - res) + atkCrit * 100;
                            } else {
                                limit = atkCrit * 100 - (res - 1000) / 10;
                            }
                            if ((s32)(Rand(gr) % 10000) >= limit) {
                                dmg += (dmg + (atkCrit / 2 + 90)) / 100;
                            }
                        }
                    }
                }
                break;
            }
            case 2: { /* percent of MHP */
                s32 power;
                s32 mhp;
                const char* elemName;
                s32 res;

                power = beDataBase_getInt(DBEneatk(gr), atk, D_005188B0);
                mhp = beDataObjList_getInt(pl, D_00518898)->_data;
                dmg = mhp * power / 100;
                CLAMP_99999(dmg);
                res = 0;
                elemName = beDataBase_getString(DBEneatk(gr), atk, D_00519018);
                PLAYER_RES(elemName, pl, res, elem);
                if (res != 0) {
                    if (res <= 1000) {
                        dmg += dmg * ((1000 - res) / 20 + 50) / 100;
                    } else if (res <= 5000) {
                        dmg += dmg * ((5000 - res) / 80) / 100;
                    }
                }
                break;
            }
            }
            CLAMP_99999(dmg);
            critical = 0;
            SPREAD(gr, dmg, critical);
            CLAMP_99999(dmg);
            beMessenger_clearInts(gr->_messenger);
            beMessenger_pushInt(gr->_messenger, dmg);
            beMessenger_pushInt(gr->_messenger, elem);
            beMessenger_pushInt(gr->_messenger, critical);
            break;
        }
        case 4: { /* E2PDAMAGERANK: hit reaction size (-1 none, 0 small .. 2 big, 3 dead) */
            player = beMessenger_getInt(gr->_messenger, 0);
            attack = beMessenger_getInt(gr->_messenger, 1);
            s32 hp = beMessenger_getInt(gr->_messenger, 2);
            s32 dmg = beMessenger_getInt(gr->_messenger, 3);
            beBaseInfoRam* er = (beBaseInfoRam*)beMessenger_getObject(gr->_messenger, 0);
            s32 rank = 0;

            if (hp <= 0) {
                rank = 3;
            } else {
                pl = GrData(gr, player);
                s32 atk = attack % 10000;
                s32 power;
                s32 seq;
                s32 mhp;

                power = beDataBase_getInt(DBEneatk(gr), atk, D_005188B0);
                seq = beDataBase_getInt(DBEneatk(gr), atk, D_005191D8);
                mhp = beDataObjList_getInt(pl, D_00518898)->_data;
                switch (seq) {
                case 0:
                    if (power == 0) {
                        rank = -1;
                    } else {
                        s32 pct = dmg * 100 / mhp;

                        if (pct < 10) {
                            rank = 0;
                        } else if (pct < 40) {
                            rank = 1;
                        } else {
                            rank = 2;
                        }
                    }
                    break;
                case 1:
                    rank = -1;
                    break;
                case 2:
                    rank = 0;
                    break;
                case 3:
                    rank = 2;
                    break;
                case 4:
                    rank = 1;
                    break;
                }
            }
            if (rank == 2) {
                beBaseInfoDataList* datas = RamDataList(er);
                beModelCtrlInfoWork* w = WorkOf(datas);
                beModelCtrlMoveObjectList* moves = MovesOf(w);

                if (moves->_count != 0 && MoveAt(moves, 1)->_isRun) {
                    rank = 1;
                }
            }
            beMessenger_clearInts(gr->_messenger);
            beMessenger_pushInt(gr->_messenger, rank);
            break;
        }
        case 7: { /* E2PSTUNTIME */
            player = beMessenger_getInt(gr->_messenger, 0);
            s32 enemy = beMessenger_getInt(gr->_messenger, 1);
            attack = beMessenger_getInt(gr->_messenger, 2);
            s32 dmg = beMessenger_getInt(gr->_messenger, 3);
            s32 kind = beMessenger_getInt(gr->_messenger, 4);
            beNDMWMdlPEBaseInfoWork* ew = (beNDMWMdlPEBaseInfoWork*)beMessenger_getObject(gr->_messenger, 0);
            pl = GrData(gr, player);
            s32 atk = attack % 10000;
            s32 type;
            s32 enemyRate;
            s32 atkRate;
            bool can;
            s32 time;

            type = beDataBase_getInt(DBEneatk(gr), atk, D_005191D0);
            enemyRate = 0;
            atkRate = 0;
            can = false;
            switch (type) {
            case 0:
                switch (kind) {
                case 1:
                case 2: {
                    s32 down;

                    down = Down(ew, 2);
                    enemyRate = beDataBase_getInt(DBEnemy(gr), enemy, D_00518970) - down;
                    can = true;
                }
                }
                break;
            case 1:
                atkRate = beDataBase_getInt(DBEneatk(gr), atk, D_00518970);
                if (atkRate > 0) {
                    can = true;
                }
                break;
            case 2:
                switch (kind) {
                case 1:
                case 2:
                    can = true;
                }
                break;
            }
            time = 0;
            if (can) {
                s32 mhp = beDataObjList_getInt(pl, D_00518898)->_data;
                s32 res = beDataObjList_getInt(pl, D_005189A0)->_data;
                s32 rate = atkRate + enemyRate;
                u32 r = Rand(gr) % 100;
                s32 part = dmg * 50 / mhp;

                if ((s32)r < (res + (part + 100 + rate)) / 2) {
                    time = (res + (part + 30 + rate)) / 4;
                }
                if (time == 0) {
                    time = -1;
                }
            }
            beMessenger_clearInts(gr->_messenger);
            beMessenger_pushInt(gr->_messenger, time);
            break;
        }
        case 8: { /* E2PPOISONTIME: "TIME", "DAMAGE" or "GEDOKU" */
            StrRefL what(beMessenger_getString(gr->_messenger, 1));
            player = beMessenger_getInt(gr->_messenger, 0);
            attack = beMessenger_getInt(gr->_messenger, 1);
            pl = GrData(gr, player);
            s32 atk = attack % 10000;

            if (strcmp(what.p, D_005191B8) == 0) { /* "TIME" */
                s32 time = 0;

                if (strcmp(beDataBase_getString(DBEneatk(gr), atk, D_005191F0), D_005191F8) == 0) {
                    s32 rank;
                    s32 res;

                    rank = beDataBase_getInt(DBEneatk(gr), atk, D_00519200);
                    res = beDataObjList_getInt(pl, D_005189B0)->_data;
                    if ((s32)(Rand(gr) % 100) < res + (rank * 15 + 30)) {
                        s32 t;

                        t = beDataBase_getInt(DBEneatk(gr), atk, D_00519210);
                        time = res / 2 + t;
                    }
                    if (time == 0) {
                        time = -1;
                    }
                }
                beMessenger_clearInts(gr->_messenger);
                beMessenger_pushInt(gr->_messenger, time);
            } else if (strcmp(what.p, D_00519220) == 0) { /* "DAMAGE" */
                s32 mhp = beDataObjList_getInt(pl, D_00518898)->_data;
                s32 rank;

                rank = beDataBase_getInt(DBEneatk(gr), atk, D_00519200);
                beMessenger_clearInts(gr->_messenger);
                beMessenger_pushInt(gr->_messenger, mhp * rank / 100);
            } else if (strcmp(what.p, D_00519228) == 0) { /* "GEDOKU" */
                s32 v;

                v = beDataBase_getInt(DBEneatk(gr), atk, D_00519228);
                beMessenger_clearInts(gr->_messenger);
                beMessenger_pushInt(gr->_messenger, v);
            }
            break;
        }
        case 9: { /* E2PSLEEPTIME */
            player = beMessenger_getInt(gr->_messenger, 0);
            s32 enemy = beMessenger_getInt(gr->_messenger, 1);
            attack = beMessenger_getInt(gr->_messenger, 2);
            pl = GrData(gr, player);
            s32 atk = attack % 10000;
            s32 time;

            const char* k = beDataBase_getString(DBEneatk(gr), atk, D_005191F0);

            time = 0;
            if (strcmp(k, D_00519230) == 0) {
                s32 rank;
                s32 wis;
                s32 spirit;
                u32 r;
                s32 f;

                rank = beDataBase_getInt(DBEneatk(gr), atk, D_00519200);
                wis = beDataBase_getInt(DBEnemy(gr), enemy, D_005188C0);
                spirit = beDataObjList_getInt(pl, D_005188D0)->_data;
                r = Rand(gr) % 10000;
                if ((s32)r < rank * 5 * (f = wis * 3 / spirit + 100 - spirit / 40)) {
                    s32 t;

                    t = beDataBase_getInt(DBEneatk(gr), atk, D_00519210);
                    time = t * f / 100;
                }
                if (time == 0) {
                    time = -1;
                }
            }
            beMessenger_clearInts(gr->_messenger);
            beMessenger_pushInt(gr->_messenger, time);
            break;
        }
        case 10: { /* E2PBINDTIME */
            player = beMessenger_getInt(gr->_messenger, 0);
            s32 enemy = beMessenger_getInt(gr->_messenger, 1);
            attack = beMessenger_getInt(gr->_messenger, 2);
            pl = GrData(gr, player);
            s32 atk = attack % 10000;
            s32 time;

            const char* k = beDataBase_getString(DBEneatk(gr), atk, D_005191F0);

            time = 0;
            if (strcmp(k, D_00519238) == 0) {
                s32 rank;
                s32 type;
                bool ok;

                rank = beDataBase_getInt(DBEneatk(gr), atk, D_00519200);
                type = beDataBase_getInt(DBEneatk(gr), atk, D_005191D0);
                ok = true;
                if (type != 2) {
                    s32 wis;
                    s32 spirit;

                    wis = beDataBase_getInt(DBEnemy(gr), enemy, D_005188C0);
                    spirit = beDataObjList_getInt(pl, D_005188D0)->_data;
                    if ((s32)(Rand(gr) % 10000) >= rank * 5 * (wis * 3 / spirit + 100 - spirit / 40)) {
                        ok = false;
                    }
                }
                if (ok) {
                    if (type == 2) {
                        time = beDataBase_getInt(DBEneatk(gr), atk, D_00519210);
                    } else {
                        s32 t;
                        s32 wis;
                        beDataObjInt* spirit;
                        s32 f;

                        t = beDataBase_getInt(DBEneatk(gr), atk, D_00519210);
                        wis = beDataBase_getInt(DBEnemy(gr), enemy, D_005188C0);
                        spirit = beDataObjList_getInt(pl, D_005188D0);
                        f = wis * 3 / spirit->_data + 100 - spirit->_data / 40;
                        time = t * f / 100;
                    }
                }
                if (time == 0) {
                    time = -1;
                }
            }
            beMessenger_clearInts(gr->_messenger);
            beMessenger_pushInt(gr->_messenger, time);
            break;
        }
        case 11: { /* E2PATKDWNTIME: "TIME", "SUBAYASA" or "MEICHURITSU" */
            StrRefL what(beMessenger_getString(gr->_messenger, 1));
            player = beMessenger_getInt(gr->_messenger, 0);
            s32 enemy = beMessenger_getInt(gr->_messenger, 1);
            attack = beMessenger_getInt(gr->_messenger, 2);
            pl = GrData(gr, player);
            s32 atk = attack % 10000;

            if (strcmp(what.p, D_005191B8) == 0) { /* "TIME" */
                KOUKA_TIME(gr, pl, enemy, atk, D_00519248);
            } else if (strcmp(what.p, D_005188D8) == 0) { /* "SUBAYASA" */
                STAT_DOWN_E2P(gr, player, pl, enemy, atk, 0, D_005188D8);
            } else if (strcmp(what.p, D_00518950) == 0) { /* "MEICHURITSU" */
                STAT_DOWN_E2P(gr, player, pl, enemy, atk, 1, D_00518950);
            }
            break;
        }
        case 12: { /* E2PPOWDWNTIME: "TIME", "TSUYOSA" or "MAMORI" */
            StrRefL what(beMessenger_getString(gr->_messenger, 1));
            player = beMessenger_getInt(gr->_messenger, 0);
            s32 enemy = beMessenger_getInt(gr->_messenger, 1);
            attack = beMessenger_getInt(gr->_messenger, 2);
            pl = GrData(gr, player);
            s32 atk = attack % 10000;

            if (strcmp(what.p, D_005191B8) == 0) { /* "TIME" */
                KOUKA_TIME(gr, pl, enemy, atk, D_00519250);
            } else if (strcmp(what.p, D_005188B0) == 0) { /* "TSUYOSA" */
                STAT_DOWN_E2P(gr, player, pl, enemy, atk, 4, D_005188B0);
            } else if (strcmp(what.p, D_005188B8) == 0) { /* "MAMORI" */
                STAT_DOWN_E2P(gr, player, pl, enemy, atk, 5, D_005188B8);
            }
            break;
        }
        case 20: { /* P2PKOUKAMAGIC: a player's support technique */
            player = beMessenger_getInt(gr->_messenger, 0);
            s32 target = beMessenger_getInt(gr->_messenger, 1);
            attack = beMessenger_getInt(gr->_messenger, 2);
            pl = GrData(gr, player);

            if (player == target) {
                s32 tech = attack % 10000;

                if (tech == 40 || tech == 45) { /* heal (45: also the players around) */
                    s32 rec;
                    s32 n;
                    s32 skill;
                    s32 heal;

                    beNDMWGameRam_findTechnique(gr, pl, tech, &rec);
                    n = beDataBase_getInt(DBPcTech(gr), rec, D_00518860);
                    TECH_SKILL(pl, n, skill);
                    heal = beDataBase_getInt(DBPcTech(gr), rec, D_00518890);
                    heal = heal + skill + beDataObjList_getInt(pl, D_005188C0)->_data;
                    beNDMWGameRam_addHp(gr, player, heal, -1);
                    if (tech == 45) {
                        IntListRefL near;
                        s32 c;

                        NEARBY(gr, pl, player, D_005190A8, player, near);
                        c = near.p->_count;
                        for (i = 0; i < c; i++) {
                            s32 t = func_003E9090(near.p)[i];

                            beNDMWGameRam_addHp(gr, t, heal, -1);
                            PLAYER_EFFECT(gr, D_005190A8, GrData(gr, t), t, D_005190B0, D_005190C0);
                        }
                    }
                } else if (tech == 55 || tech == 60) { /* cure poison (60: also the players around) */
                    s32 rec;
                    s32 n;
                    s32 skill;
                    igObject* base;
                    igObject* work;

                    beNDMWGameRam_findTechnique(gr, pl, tech, &rec);
                    n = beDataBase_getInt(DBPcTech(gr), rec, D_00518860);
                    TECH_SKILL(pl, n, skill);
                    beMessenger_call(gr->_messenger, D_005190A8, DigimonName(pl),
                                     player, D_00519260);
                    base = beMessenger_getObject(gr->_messenger, 0);
                    work = beMessenger_getObject(gr->_messenger, 1);
                    if (func_00447B00(work)) {
                        s32 chance = rec - tech + 5 - ((beNDMWMdlPEBaseInfoWork*)work)->_poisonEndurance;

                        if (chance >= (s32)(Rand(gr) % 10)) {
                            func_00447B40(work, base);
                        }
                    }
                    if (tech == 60) {
                        IntListRefL near;
                        s32 c;

                        NEARBY(gr, pl, player, D_005190A8, player, near);
                        c = near.p->_count;
                        for (i = 0; i < c; i++) {
                            s32 t = func_003E9090(near.p)[i];
                            beDataObjList* tp = GrData(gr, t);

                            beMessenger_call(gr->_messenger, D_005190A8,
                                             DigimonName(tp), t, D_00519260);
                            base = beMessenger_getObject(gr->_messenger, 0);
                            work = beMessenger_getObject(gr->_messenger, 1);
                            if (func_00447B00(work)) {
                                s32 chance = rec - tech + 5 - ((beNDMWMdlPEBaseInfoWork*)work)->_poisonEndurance;

                        if (chance >= (s32)(Rand(gr) % 10)) {
                                    func_00447B40(work, base);
                                }
                            }
                            PLAYER_EFFECT(gr, D_005190A8, tp, t, D_00519270, D_00519280);
                        }
                    }
                } else if (tech == 50 || tech == -50) { /* raise the fallen players around */
                    s32 a;
                    s32 b;
                    igMatrix44f mtx;
                    igMatrix44f m;
                    igVec3f pos;
                    IntListRefL near;
                    s32 c;

                    if (tech >= 0) {
                        s32 rec;

                        beNDMWGameRam_findTechnique(gr, pl, tech, &rec);
                        b = D_004CF558[rec];
                        a = D_004CF538[rec];
                    } else {
                        a = D_004CF600[0];
                        b = D_004CF620[0];
                    }
                    func_00190380(&mtx);
                    beMessenger_call(gr->_messenger, D_005190A8, DigimonName(pl),
                                     player, D_00519030);
                    beMessenger_getMatrix(&m, gr->_messenger, 0);
                    func_001908F0(&mtx, &m);
                    func_00190A30(&mtx, &pos);
                    func_003FC780((igIntListRef*)&near, gr, D_00519290, -1, &pos);
                    c = near.p->_count;
                    for (i = 0; i < c; i++) {
                        beMessenger_select(gr->_messenger, D_00519290, NULL, func_003E9090(near.p)[i]);
                        beMessenger_pushMatrix(gr->_messenger, &mtx);
                        beMessenger_pushInt(gr->_messenger, a);
                        beMessenger_pushInt(gr->_messenger, b);
                        beMessenger_send(gr->_messenger, D_00519298);
                    }
                } else if (tech == 65) { /* speed up */
                    s32 rec;
                    s32 n;
                    s32 wis;
                    s32 time;
                    s32 skill;
                    s32 speed;
                    IntListRefL near;
                    s32 c;

                    beNDMWGameRam_findTechnique(gr, pl, tech, &rec);
                    n = beDataBase_getInt(DBPcTech(gr), rec, D_00518860);
                    wis = beDataObjList_getInt(pl, D_005188C0)->_data;
                    time = beDataBase_getInt(DBPcTech(gr), rec, D_005191B8) + wis;
                    skill = Skill(pl, n);
                    speed = skill / 5 + beDataBase_getInt(DBPcTech(gr), rec, D_005188D8);
                    NEARBY(gr, pl, player, D_005190A8, player, near);
                    AddSelf(near, player);
                    c = near.p->_count;
                    for (i = 0; i < c; i++) {
                        s32 t = func_003E9090(near.p)[i];
                        beDataObjList* tp = GrData(gr, t);
                        s32 crit;
                        s32 stun;
                        s32 dodge;

                        crit = beDataBase_getInt(DBPcTech(gr), rec, D_00518960);
                        stun = beDataBase_getInt(DBPcTech(gr), rec, D_005191C0);
                        dodge = beDataBase_getInt(DBPcTech(gr), rec, D_00518980);
                        beMessenger_push(gr->_messenger);
                        beMessenger_select(gr->_messenger, D_005190A8, DigimonName(tp), t);
                        beMessenger_pushInt(gr->_messenger, time);
                        beMessenger_pushInt(gr->_messenger, speed);
                        beMessenger_pushInt(gr->_messenger, crit);
                        beMessenger_pushInt(gr->_messenger, stun);
                        beMessenger_pushInt(gr->_messenger, dodge);
                        beMessenger_send(gr->_messenger, D_005192A0);
                        if (t != player) {
                            PLAYER_EFFECT(gr, D_005190A8, tp, t, D_005192B0, D_005192C0);
                        }
                        beMessenger_pop(gr->_messenger);
                    }
                } else if (tech == 80) { /* power up */
                    s32 rec;
                    s32 wis;
                    s32 time;
                    s32 n;
                    s32 skill;
                    s32 bonus;
                    s32 maxHp;
                    s32 str;
                    s32 def;
                    IntListRefL near;
                    s32 c;

                    beNDMWGameRam_findTechnique(gr, pl, tech, &rec);
                    wis = beDataObjList_getInt(pl, D_005188C0)->_data;
                    time = beDataBase_getInt(DBPcTech(gr), rec, D_005191B8) + wis;
                    n = beDataBase_getInt(DBPcTech(gr), rec, D_00518860);
                    skill = Skill(pl, n);
                    {
                        beDataBase* db = DBPcTech(gr);

                        bonus = skill / 5;
                        maxHp = bonus + beDataBase_getInt(db, rec, D_00518FC8);
                    }
                    str = bonus + beDataBase_getInt(DBPcTech(gr), rec, D_005188B0);
                    def = bonus + beDataBase_getInt(DBPcTech(gr), rec, D_005188B8);
                    NEARBY(gr, pl, player, D_005190A8, player, near);
                    AddSelf(near, player);
                    c = near.p->_count;
                    for (i = 0; i < c; i++) {
                        s32 t = func_003E9090(near.p)[i];
                        beDataObjList* tp = GrData(gr, t);

                        beMessenger_push(gr->_messenger);
                        beMessenger_select(gr->_messenger, D_005190A8, DigimonName(tp), t);
                        beMessenger_pushInt(gr->_messenger, time);
                        beMessenger_pushInt(gr->_messenger, str);
                        beMessenger_pushInt(gr->_messenger, def);
                        beMessenger_pushInt(gr->_messenger, maxHp);
                        beMessenger_send(gr->_messenger, D_005192D0);
                        if (t != player) {
                            PLAYER_EFFECT(gr, D_005190A8, tp, t, D_005192B0, D_005192C0);
                        }
                        beMessenger_pop(gr->_messenger);
                    }
                } else if (tech == 75) { /* element barrier */
                    s32 rec;
                    s32 time;
                    s32 fire;
                    s32 ice;
                    s32 thunder;
                    s32 dark;
                    IntListRefL near;
                    s32 c;

                    beNDMWGameRam_findTechnique(gr, pl, tech, &rec);
                    time = beDataBase_getInt(DBPcTech(gr), rec, D_005191B8);
                    fire = beDataBase_getInt(DBPcTech(gr), rec, D_005192E0);
                    ice = beDataBase_getInt(DBPcTech(gr), rec, D_005192E8);
                    thunder = beDataBase_getInt(DBPcTech(gr), rec, D_005192F0);
                    dark = beDataBase_getInt(DBPcTech(gr), rec, D_005192F8);
                    NEARBY(gr, pl, player, D_005190A8, player, near);
                    AddSelf(near, player);
                    c = near.p->_count;
                    for (i = 0; i < c; i++) {
                        s32 t = func_003E9090(near.p)[i];
                        beDataObjList* tp = GrData(gr, t);

                        beMessenger_push(gr->_messenger);
                        beMessenger_select(gr->_messenger, D_005190A8, DigimonName(tp), t);
                        beMessenger_pushInt(gr->_messenger, time);
                        beMessenger_pushInt(gr->_messenger, fire);
                        beMessenger_pushInt(gr->_messenger, ice);
                        beMessenger_pushInt(gr->_messenger, thunder);
                        beMessenger_pushInt(gr->_messenger, dark);
                        beMessenger_send(gr->_messenger, D_00519300);
                        if (t != player) {
                            PLAYER_EFFECT(gr, D_005190A8, tp, t, D_005192B0, D_005192C0);
                        }
                        beMessenger_pop(gr->_messenger);
                    }
                }
                beMessenger_clearInts(gr->_messenger);
                beMessenger_pushInt(gr->_messenger, 1);
            }
            break;
        }
        }
        return true;
    }
    case 20: { /* BKCHIPBITEXP: store a player's name, BIT and EXP progress in the backup chip */
        player = beMessenger_getInt(gr->_messenger, 0);
        beDataObjList* common = Common(gr);
        pl = GrData(gr, player);
        s32 slot;
        s32 bit;
        igIntList* data;
        s32 level;

        {
            const char* s = DigimonName(pl);

            StrSet(StrListOf(beDataObjList_getStringList(common, D_005187E8)), player, StrRefL(s));
        }
        slot = player * 2;
        bit = beDataObjList_getInt(pl, D_00518840)->_data;
        data = IntListOf(beDataObjList_getIntList(common, D_005187D8));
        IntSet(data, slot, bit);
        beDataObjList_getInt(pl, D_00518840)->_data = 0;
        slot = slot + 1;
        level = beDataObjList_getInt(pl, D_00518858)->_data;
        if (level != 999 && level != 1) {
            s32 exp = beDataObjList_getInt(pl, D_00518880)->_data;
            s32 l1 = level - 1;
            s32 l2 = level - 2;
            s32 cur = l1 * 200 + (level + 139) * (l1 * l1);
            s32 keep = exp - (cur - (l2 * 200 + (level + 138) * (l2 * l2))) * 2 / 10;

            if (keep < cur) {
                keep = cur;
            }
            data = IntListOf(beDataObjList_getIntList(common, D_005187D8));
            IntSet(data, slot, exp - keep);
            beDataObjList_getInt(pl, D_00518880)->_data = keep;
        }
        return true;
    }
    }
    return false;
}
/* (inline_depth stays at 1: only trivial functions follow) */
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWGameRam", beNDMWGameRam_handle);
#endif

/* func_00406DA0 is the out-of-line copy of the inline above, emitted for
   beNDMWGameRam_handle. While handle is still asm, the same inline and a
   caller (not in the unit, so mwsplice drops it) make MWCC emit the copy. */
#ifndef NON_MATCHING
extern "C" {
inline char** func_00406DA0(igStringRefList* l) { return (char**)l->_data; }
}
#pragma dont_inline on
extern "C" char** emit_func_00406DA0(igStringRefList* l) { return func_00406DA0(l); }
#pragma dont_inline off
#endif

/* func_00406DB0: out-of-line copy of the inline above, same scheme as func_00406DA0. */
#ifndef NON_MATCHING
extern "C" {
inline igStringRefList* func_00406DB0(beDataObjStringList* o) { return o->_data; }
}
#pragma dont_inline on
extern "C" igStringRefList* emit_func_00406DB0(beDataObjStringList* o) { return func_00406DB0(o); }
#pragma dont_inline off
#endif

/* func_00406DC0: out-of-line copy of the inline above, same scheme as func_00406DA0. */
#ifndef NON_MATCHING
struct IntListRefH { igIntList* p; };
extern "C" {
inline igIntList* func_00406DC0(IntListRefH* r) { return r->p; }
}
#pragma dont_inline on
extern "C" igIntList* emit_func_00406DC0(IntListRefH* r) { return func_00406DC0(r); }
#pragma dont_inline off
#endif

/* func_00406DD0: out-of-line copy of the inline above, same scheme as func_00406DA0. */
#ifndef NON_MATCHING
extern "C" {
inline igObject* func_00406DD0(beModelCtrlMoveObjectList* l, s32 i) { return ((igObject**)l->_data)[i]; }
}
#pragma dont_inline on
extern "C" igObject* emit_func_00406DD0(beModelCtrlMoveObjectList* l, s32 i) { return func_00406DD0(l, i); }
#pragma dont_inline off
#endif

/* func_00406DF0: out-of-line copy of the inline above, same scheme as func_00406DA0. */
#ifndef NON_MATCHING
extern "C" {
inline beModelCtrlMoveObjectList* func_00406DF0(beModelCtrlMoveObjectList* const* r) { return *r; }
}
#pragma dont_inline on
extern "C" beModelCtrlMoveObjectList* emit_func_00406DF0(beModelCtrlMoveObjectList* const* r) { return func_00406DF0(r); }
#pragma dont_inline off
#endif

/* func_00406E00: out-of-line copy of the inline above, same scheme as func_00406DA0. */
#ifndef NON_MATCHING
extern "C" {
inline beModelCtrlInfoWork* func_00406E00(beModelCtrlInfoWork* const* r) { return *r; }
}
#pragma dont_inline on
extern "C" beModelCtrlInfoWork* emit_func_00406E00(beModelCtrlInfoWork* const* r) { return func_00406E00(r); }
#pragma dont_inline off
#endif

/* func_00406E10: out-of-line copy of the inline above, same scheme as func_00406DA0. */
#ifndef NON_MATCHING
struct PoolHolderDH { void* p; };
struct DataListRefHH { beBaseInfoDataList* p; };
extern "C" {
beBaseInfoDataList* func_0036AE80(void* pool);
inline DataListRefHH* func_00406E10(DataListRefHH* r, PoolHolderDH* pool) {
    r->p = func_0036AE80(pool->p);
    return r;
}
}
#pragma dont_inline on
extern "C" DataListRefHH* emit_func_00406E10(DataListRefHH* r, PoolHolderDH* pool) { return func_00406E10(r, pool); }
#pragma dont_inline off
#endif

/* func_00406E40: out-of-line copy of the inline above, same scheme as func_00406DA0. */
#ifndef NON_MATCHING
extern "C" {
inline igIntList* func_00406E40(beDataObjIntList* o) { return o->_data; }
}
#pragma dont_inline on
extern "C" igIntList* emit_func_00406E40(beDataObjIntList* o) { return func_00406E40(o); }
#pragma dont_inline off
#endif

/* func_00406E50: out-of-line copy of the inline above, same scheme as func_00406DA0. */
#ifndef NON_MATCHING
extern "C" {
inline igRandomNumber* func_00406E50(igRandomNumber* const* r) { return *r; }
}
#pragma dont_inline on
extern "C" igRandomNumber* emit_func_00406E50(igRandomNumber* const* r) { return func_00406E50(r); }
#pragma dont_inline off
#endif

/* func_00406E60: out-of-line copy of the inline above, same scheme as func_00406DA0. */
#ifndef NON_MATCHING
extern "C" {
inline beDataBase** func_00406E60(beDataBase** r, beDataBase* o) {
    if (o != NULL) {
        o->_refCount++;
    }
    {
        beDataBase* old = *r;

        if (old != NULL) {
            old->_refCount--;
            if ((old->_refCount & 0x7FFFFF) == 0) {
                func_002FF820(old);
            }
        }
    }
    *r = o;
    return r;
}
}
#pragma dont_inline on
extern "C" beDataBase** emit_func_00406E60(beDataBase** r, beDataBase* o) { return func_00406E60(r, o); }
#pragma dont_inline off
#endif

/* func_00406EE0: out-of-line copy of the inline above, same scheme as func_00406DA0. */
#ifndef NON_MATCHING
extern "C" {
inline beDataBase* func_00406EE0(beDataBase* const* r) { return *r; }
}
#pragma dont_inline on
extern "C" beDataBase* emit_func_00406EE0(beDataBase* const* r) { return func_00406EE0(r); }
#pragma dont_inline off
#endif

/* func_00406EF0: out-of-line copy of the inline above, same scheme as func_00406DA0. */
#ifndef NON_MATCHING
extern "C" {
inline igIntList* func_00406EF0(igIntList* const* r) { return *r; }
}
#pragma dont_inline on
extern "C" igIntList* emit_func_00406EF0(igIntList* const* r) { return func_00406EF0(r); }
#pragma dont_inline off
#endif

/* func_00406F00..func_00406F90: out-of-line copies of the inlines above, same scheme as
   func_00406DA0 (the PoolHolderN setters take the holder as void**). */
#ifndef NON_MATCHING
extern "C" {
inline bool* func_00406F00(igBoolList* l) { return (bool*)l->_data; }
inline igBoolList* func_00406F10(igBoolList* const* r) { return *r; }
}
#pragma dont_inline on
extern "C" bool* emit_func_00406F00(igBoolList* l) { return func_00406F00(l); }
extern "C" igBoolList* emit_func_00406F10(igBoolList* const* r) { return func_00406F10(r); }
#pragma dont_inline off
#endif

/* func_00406F20: out-of-line copy of the inline above, same scheme as func_00406DA0. */
#ifndef NON_MATCHING
extern "C" {
inline void func_00406F20(igObjectList* l, s32 i) { func_002D6CF0(l, i); }
}
#pragma dont_inline on
extern "C" void emit_func_00406F20(igObjectList* l, s32 i) { func_00406F20(l, i); }
#pragma dont_inline off
#endif

#ifndef NON_MATCHING
extern "C" {
inline void func_00406F40(void** h, void* pool) { *h = pool; }
inline void func_00406F50(void** h, void* pool) { *h = pool; }
inline void func_00406F60(void** h, void* pool) { *h = pool; }
inline void func_00406F70(void** h, void* pool) { *h = pool; }
inline igObjectList* func_00406F80(igObjectList* const* r) { return *r; }
inline s32 func_00406F90(beDataObjInt* o) { return o->_data; }
}
#pragma dont_inline on
extern "C" void emit_func_00406F40(void** h, void* pool) { func_00406F40(h, pool); }
extern "C" void emit_func_00406F50(void** h, void* pool) { func_00406F50(h, pool); }
extern "C" void emit_func_00406F60(void** h, void* pool) { func_00406F60(h, pool); }
extern "C" void emit_func_00406F70(void** h, void* pool) { func_00406F70(h, pool); }
extern "C" igObjectList* emit_func_00406F80(igObjectList* const* r) { return func_00406F80(r); }
extern "C" s32 emit_func_00406F90(beDataObjInt* o) { return func_00406F90(o); }
#pragma dont_inline off
#endif

#ifndef NON_MATCHING
extern "C" {
inline void func_00406FA0(char** s) {
    char* p = *s;

    if (p != NULL) {
        StrHeader* h = (StrHeader*)p - 1;
        if (--h->refs == 0) {
            func_0030C940(h->owner, h);
        }
    }
}
}
#pragma dont_inline on
extern "C" void emit_func_00406FA0(char** s) { func_00406FA0(s); }
#pragma dont_inline off
#endif

/* func_00406FE0: out-of-line copy of the StrPool getter (GetStrPool), same scheme as
   func_00406DA0. */
#ifndef NON_MATCHING
extern "C" {
inline StrPool* func_00406FE0(void) {
    if (D_00586F20 == NULL) {
        StrPool* pool = (StrPool*)func_002EB010(0x10);
        if (pool != NULL) {
            pool = func_002EADF0(pool);
        }
        D_00586F20 = pool;
    }
    return D_00586F20;
}
}
#pragma dont_inline on
extern "C" StrPool* emit_func_00406FE0(void) { return func_00406FE0(); }
#pragma dont_inline off
#endif

#ifndef NON_MATCHING
extern "C" {
inline beDataObjListList* func_00407030(beDataObjListList* const* r) { return *r; }
inline beDataObjList* func_00407040(beDataObjList* const* r) { return *r; }
inline const char* func_00407050(char* const* s) { return *s; }
}
#pragma dont_inline on
extern "C" beDataObjListList* emit_func_00407030(beDataObjListList* const* r) { return func_00407030(r); }
extern "C" beDataObjList* emit_func_00407040(beDataObjList* const* r) { return func_00407040(r); }
extern "C" const char* emit_func_00407050(char* const* s) { return func_00407050(s); }
#pragma dont_inline off
#endif

extern igMetaObject* beNDMWGameRam_Meta;

igMetaObject* beNDMWGameRam_getMeta(void) {
    return beNDMWGameRam_Meta;
}

igMetaObject* beNDMWGameRam_getInfoMeta(void) {
    return NULL;
}

}
