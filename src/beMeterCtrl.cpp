#include "game.h"

/* beMeterCtrl: on-screen meters (MWCC C++). */
extern "C" {

extern char D_0050EF18[]; /* "MODELCTRL" */
extern char D_0050EF10[]; /* "HIT" */
extern char D_0050EF30[]; /* "HITPLAYERBULLET" */
extern char D_0050EF40[]; /* "HITPLAYERTALK" */
extern char D_0050EF50[]; /* "HITENEMYBULLET" */
extern igMetaObject* D_00585BE0;
extern igMetaObject* beLua_Meta;
extern igMetaObject* beTargetObj_Meta;
extern igMetaObject* beModelCtrlInfo_Meta;
igNamedObject* func_003022C0(igObjectList* list, igMetaObject* meta, const char* name); /* find by name */
bool beBaseInfoManager_appendInfo(beBaseInfoManager* mgr, beBaseInfo* info);

/* beModelCtrl's virtuals used here. */
class beModelCtrlV {
public:
    IG_VPAD_37
    virtual void entryRam(beBaseInfoRam* ram); /* +0x9C: beBaseInfoManager's */
};

extern char D_0050EE88[]; /* "MODELCTRL" */
extern char D_0050EE98[]; /* "GETHP" */
igObject* func_00357600(void); /* "MsgAction" table: GETVALUE .. SETMODELHP */
void beMeterCtrl_setMax(void* mgr, beMeterCtrlOneData* d);

static inline f32 ClampF(f32 v, f32 lo, f32 hi) {
    if (v <= hi) {
        if (v < lo) {
            v = lo;
        }
    } else {
        v = hi;
    }
    return v;
}

/* -- */

extern igMetaObject* beMeterCtrlInfo_Meta;
extern char D_0050EE78[]; /* "METERCTRL" */
void beMessenger_entryRam(beMessenger* m, beBaseInfoRam* ram, const char* group);

igMetaObject* beMeterCtrl_getInfoMeta(void) {
    return beMeterCtrlInfo_Meta;
}

/* Register a meter ram with the messenger. */
void beMeterCtrl_entry(beBaseInfoManager* mgr, beBaseInfoRam* ram) {
    beMessenger_entryRam(mgr->_messenger, ram, D_0050EE78);
}

#ifdef NON_MATCHING
/* Nonmatching: float register choice around the two divisions. */
/* Set up a meter's maximum: digit count, fill rate and step. */
void beMeterCtrl_setMax(void* mgr, beMeterCtrlOneData* d) {
    s32 n = 1;
    s32 v = (s32)d->_maxValueF;

    for (;;) {
        v /= 10;
        if (v == 0) {
            break;
        }
        n++;
    }
    d->_ketaKazu = n;
    d->_ratioTimeValueF = 2000000000.0f / d->_maxValueF;
    d->_addMaxValueF = d->_maxValueF / 60.0f;
    if (d->_addMaxValueF < 1.0f) {
        d->_addMaxValueF = 1.0f;
    }
    d->_isChange = true;
}
#else
INCLUDE_ASM("asm/nonmatchings/beMeterCtrl", beMeterCtrl_setMax);
#endif

INCLUDE_ASM("asm/nonmatchings/beMeterCtrl", beMeterCtrl_updateRam);

#ifdef NON_MATCHING
/* Nonmatching: float registers in ADDPERCENT and the bool test in SETPERCENT. */
/* Meter messages: GETVALUE, SETVALUE, ADDVALUE, GETPERCENT, SETPERCENT,
   ADDPERCENT, SETMAXVALUE, SETMODELHP (follow a model's HP / MHP objects). */
bool beMeterCtrl_handle(beMeterCtrl* mgr, beBaseInfoRam* ram, beMessengerWork* work) {
    beMeterCtrlOneData* d = ((beMeterCtrlOneData**)((beMeterCtrlInfo*)ram->_parentInfo)->_oneDataList->_data)[ram->_ramNo];

    switch (beMessenger_getMessageIndex(mgr->_messenger, func_00357600(), (const char*)work)) {
    case 0:
        beMessenger_pushFloat(mgr->_messenger, d->_nowValueF);
        return true;
    case 1: {
        f32 v = beMessenger_getFloat(mgr->_messenger, 0);

        d->_nowValueF = v;
        d->_dspValueF = v;
        d->_isChange = true;
        return true;
    }
    case 2: {
        f32 v = beMessenger_getFloat(mgr->_messenger, 0);

        v += d->_nowValueF;
        f32 max = d->_maxValueF;

        if (v > max) {
            v = max;
        } else if (v < 0.0f) {
            v = 0.0f;
        }
        d->_nowValueF = v;
        return true;
    }
    case 3:
        beMessenger_pushFloat(mgr->_messenger, d->_nowValueF / d->_maxValueF);
        return true;
    case 4:
        if (d->_hp == NULL) {
            f32 p = beMessenger_getFloat(mgr->_messenger, 0);
            bool nz = true;
            f32 v;

            if (p == 0.0f) {
                nz = false;
            }
            v = p * d->_maxValueF / 100.0f;
            if (v < 1.0f && nz != false) {
                v = 1.0f;
            }
            d->_nowValueF = v;
        }
        return true;
    case 5: {
        f32 m = d->_maxValueF;
        f32 v = beMessenger_getFloat(mgr->_messenger, 0);

        v = v * m / 100.0f + d->_nowValueF;
        f32 max = d->_maxValueF;

        if (v > max) {
            v = max;
        } else if (v < 0.0f) {
            v = 0.0f;
        }
        d->_nowValueF = v;
        return true;
    }
    case 6:
        d->_maxValueF = beMessenger_getFloat(mgr->_messenger, 0);
        beMeterCtrl_setMax(mgr, d);
        return true;
    case 7: {
        const char* name = beMessenger_getString(mgr->_messenger, 0);
        s32 ramNo = beMessenger_getInt(mgr->_messenger, 0);

        beMessenger_push(mgr->_messenger);
        beMessenger_call(mgr->_messenger, D_0050EE88, name, ramNo, D_0050EE98);
        igAssign(d->_hp, (beDataObjInt*)beMessenger_getObject(mgr->_messenger, 0));
        igAssign(d->_mHp, (beDataObjInt*)beMessenger_getObject(mgr->_messenger, 1));
        beMessenger_pop(mgr->_messenger);
        return true;
    }
    }
    return false;
}
#else
INCLUDE_ASM("asm/nonmatchings/beMeterCtrl", beMeterCtrl_handle);
#endif

extern igMetaObject* beMeterCtrl_Meta;

igMetaObject* beMeterCtrl_getMeta(void) {
    return beMeterCtrl_Meta;
}

void func_00390320(void* a, void* b, void* c);

void func_003902F0(void* a, void* b) {
    func_00390320(b, a, b);
}

INCLUDE_ASM("asm/nonmatchings/beMeterCtrl", func_00390320);

extern void* D_00562640; /* traversal callback table */
igMetaObject* func_00356D10(void);
void func_001E06C0(void* table, igMetaObject* meta, void (*fn)(void*, void*));

void func_00390890(void) {
    func_001E06C0(D_00562640, func_00356D10(), func_003902F0);
}

void func_003908D0(void) {
}

INCLUDE_ASM("asm/nonmatchings/beMeterCtrl", func_003908E0);

void beModelCtrl_prepare(beModelCtrl* mgr) {
    if (mgr->_luaManager == NULL) {
        mgr->_luaManager = (beLua*)func_00236F80(mgr->_insight, beLua_Meta);
    }
    if (mgr->_targetObj == NULL) {
        mgr->_targetObj = (beTargetObj*)func_00236F80(mgr->_insight, beTargetObj_Meta);
    }
}

igMetaObject* beModelCtrl_getInfoMeta(void) {
    return beModelCtrlInfo_Meta;
}

/* beModelCtrl's virtuals used by appendInfo. */
class beModelCtrlMetaV {
public:
    IG_VPAD_30
    virtual igMetaObject* getInfoMeta(); /* +0x80 */
};

class beModelCtrlKindV {
public:
    IG_VPAD_35
    virtual bool isKind(const char* kind); /* +0x94 */
};

class beModelCtrlInfoV {
public:
    IG_VPAD_20
    virtual igMetaObject* getMeta(); /* +0x58 */
};

/* Accept an info of the manager's class whose kind string the manager handles. */
bool beModelCtrl_appendInfo(beModelCtrl* mgr, beModelCtrlInfo* info) {
    if (((beModelCtrlMetaV*)mgr)->getInfoMeta() == ((beModelCtrlInfoV*)info)->getMeta()) {
        if (((beModelCtrlKindV*)mgr)->isKind(info->_kindString) == true) {
            beBaseInfoManager_appendInfo(mgr, info);
            return true;
        }
    }
    return false;
}

/* Register a model ram as MODELCTRL, plus the hit groups its HIT data asks for. */
void beModelCtrl_entry(beModelCtrl* mgr, beBaseInfoRam* ram) {
    beBaseInfo* info = ram->_parentInfo;
    beModelCtrlInfoDataHit* hit;

    beMessenger_entryRam(mgr->_messenger, ram, D_0050EF18);
    hit = (beModelCtrlInfoDataHit*)func_003022C0(info->_dataList, D_00585BE0, D_0050EF10);
    if (hit != NULL) {
        if (hit->_isHitPlayerBullet) {
            beMessenger_entryRam(mgr->_messenger, ram, D_0050EF30);
        }
        if (hit->_isHitPlayerTalk) {
            beMessenger_entryRam(mgr->_messenger, ram, D_0050EF40);
        }
        if (hit->_isHitEnemyBullet) {
            beMessenger_entryRam(mgr->_messenger, ram, D_0050EF50);
        }
    }
    ((beModelCtrlV*)mgr)->entryRam(ram);
}

void func_00390F70(void) {
}

INCLUDE_ASM("asm/nonmatchings/beMeterCtrl", func_00390F80);

}
