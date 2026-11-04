#include "game.h"

/* beNDMWMdlEffect: model controller for effect models (MWCC C++). */
extern "C" {

extern igMetaObject* beNDMWMdlEffect_Meta;
extern igMetaObject* D_00585BE0; /* meta used for lookups by name */
extern char D_00515DA8[];        /* "KIND_EFFECT" */
extern char D_00515DB8[];        /* "ONE" */
extern bool D_00554C14;
extern void* D_00587110;
igNamedObject* func_003022C0(igObjectList* list, igMetaObject* meta, const char* name); /* find by name */
beBaseInfoDataList* func_0036AE80(void* pool);  /* new beBaseInfoDataList */
beModelCtrlInfoWork* func_00350020(void* pool); /* new beModelCtrlInfoWork */
void func_00396340(beModelCtrl* mgr, beBaseInfoRam* ram); /* beModelCtrl: set up a ram */

}

IG_REF(beBaseInfoDataList);
IG_REF(beModelCtrlInfoWork);

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

bool beNDMWMdlEffect_isKind(beNDMWMdlEffect* mgr, const char* kind) {
    return strcmp(kind, D_00515DA8) == 0;
}

void func_003D16F0(beNDMWMdlEffect* mgr) {
}

/* Give the ram a fresh work object. */
void beNDMWMdlEffect_setupRam(beNDMWMdlEffect* mgr, beBaseInfoRam* ram) {
    beModelCtrlInfoRamData* data;

    MakeDataList(ram);
    data = (beModelCtrlInfoRamData*)func_003022C0(ram->_dataList, D_00585BE0, D_00515DB8);
    {
        beModelCtrlInfoWorkRef work(func_00350020(PoolOf2(mgr)), IG_ADOPT);

        igAssign(data->_work, work.p);
        func_00396340(mgr, ram);
    }
}

igMetaObject* beNDMWMdlEffect_getMeta(void) {
    return beNDMWMdlEffect_Meta;
}

}
