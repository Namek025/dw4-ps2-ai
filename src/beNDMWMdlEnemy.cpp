#include "game.h"

/* beNDMWMdlEnemy: model controller for enemies (MWCC C++). */
extern "C" {

extern igMetaObject* beNDMWMdlEnemy_Meta;
extern igMetaObject* D_00585BE0; /* meta used for lookups by name */
extern char D_00515E18[];        /* "KIND_ENEMY" */
extern char D_00515E28[];        /* "ENEMY" */
extern char D_00515E30[];        /* "ONE" */
extern char D_00515E38[];        /* "DATABASE" */
extern char D_00515E48[];        /* "enemy" */
extern char D_00515E50[];        /* "GETDATABASE" */
extern char D_00515E60[];        /* "RARITY" */
extern char D_00515E68[];        /* "SETRAREICON" */
extern char D_004E0858[];        /* "" */
extern bool D_00554C14;
extern void* D_00587110;
igNamedObject* func_003022C0(igObjectList* list, igMetaObject* meta, const char* name); /* find by name */
beBaseInfoDataList* func_0036AE80(void* pool);  /* new beBaseInfoDataList */
beNDMWMdlEnemyInfoWork* func_003C1910(void* pool); /* new beNDMWMdlEnemyInfoWork */
void func_00396740(beModelCtrl* mgr, beBaseInfoRam* ram); /* beModelCtrl: set up a ram (with arguments) */
void func_00445200(beNDMWMdlEnemy* mgr, beBaseInfoRam* ram);

}

IG_REF(beBaseInfoDataList);
IG_REF(beNDMWMdlEnemyInfoWork);

static inline void* PoolOf2(void* o) {
    void* a = func_00301000(o);

    return D_00554C14 ? a : func_002F88E0(D_00587110);
}

/* The ram's data list, made on first use. */
static inline void MakeDataList(beBaseInfoRam* ram) {
    if (ram->_dataList == NULL) {
        beBaseInfoDataListRef l(func_0036AE80(PoolOf2(ram)), IG_ADOPT);

        igAssign(ram->_dataList, l.p);
    }
}

extern "C" {

/* -- */

bool beNDMWMdlEnemy_isKind(beNDMWMdlEnemy* mgr, const char* kind) {
    return strcmp(kind, D_00515E18) == 0;
}

void beNDMWMdlEnemy_entry(beNDMWMdlEnemy* mgr, beBaseInfoRam* ram) {
    beMessenger_entryRam(mgr->_messenger, ram, D_00515E28);
}

/* Give the ram a fresh work object. The optional arguments are the enemy's
   database record and HP; a rare enemy gets its icon. */
void beNDMWMdlEnemy_setupRam(beNDMWMdlEnemy* mgr, beBaseInfoRam* ram) {
    beBaseInfo* info = ram->_parentInfo;
    beModelCtrlInfoRamData* data;

    MakeDataList(ram);
    data = (beModelCtrlInfoRamData*)func_003022C0(ram->_dataList, D_00585BE0, D_00515E30);
    {
        beNDMWMdlEnemyInfoWorkRef work(func_003C1910(PoolOf2(mgr)), IG_ADOPT);

        igAssign(data->_work, (beModelCtrlInfoWork*)work.p);
        if (beMessenger_getIntCount(mgr->_messenger)) {
            s32 v;
            s32 rarity;

            v = beMessenger_getInt(mgr->_messenger, 0);
            work->_recNo = v;
            v = beMessenger_getInt(mgr->_messenger, 1);
            work->_mHp->_data = v;
            v = beMessenger_getInt(mgr->_messenger, 1);
            work->_hp->_data = v;
            beMessenger_push(mgr->_messenger);
            beMessenger_call(mgr->_messenger, D_00515E38, D_00515E48, 0, D_00515E50);
            rarity = beDataBase_getInt((beDataBase*)beMessenger_getObject(mgr->_messenger, 0), work->_recNo,
                                       D_00515E60);
            if (rarity) {
                s32 ramNo = ram->_ramNo;

                beMessenger_delaySelect(mgr->_messenger, D_00515E28, info->_name ? info->_name : D_004E0858,
                                        ramNo);
                beMessenger_pushInt(mgr->_messenger, rarity);
                beMessenger_delaySend(mgr->_messenger, D_00515E68, 60);
            }
            beMessenger_pop(mgr->_messenger);
        }
    }
    func_00396740(mgr, ram);
}

void func_003D1CE0(beNDMWMdlEnemy* mgr, beBaseInfoRam* ram) {
    func_00445200(mgr, ram);
}

igMetaObject* beNDMWMdlEnemy_getMeta(void) {
    return beNDMWMdlEnemy_Meta;
}

}
