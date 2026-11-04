#include "game.h"

/* beNDMWMdlObject: model controller for objects (MWCC C++). */
extern "C" {

extern igMetaObject* beNDMWMdlObject_Meta;
extern igMetaObject* D_00585BE0; /* meta used for lookups by name */
extern char D_00515F40[];        /* "KIND_OBJECT" */
extern char D_00515F50[];        /* "ONE" */
extern bool D_00554C14;
extern void* D_00587110;
igNamedObject* func_003022C0(igObjectList* list, igMetaObject* meta, const char* name); /* find by name */
beBaseInfoDataList* func_0036AE80(void* pool);  /* new beBaseInfoDataList */
beModelCtrlInfoWork* func_00350020(void* pool); /* new beModelCtrlInfoWork */
void func_00396740(beModelCtrl* mgr, beBaseInfoRam* ram); /* beModelCtrl: set up a ram (with arguments) */

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

static inline void SetHitParentName(beModelCtrlInfoWork* w, const char* name) {
    char* str = func_002EAF60(GetStrPool(), name);

    ReleaseStr(w->_hitParentName);
    w->_hitParentName = str;
}

extern "C" {

/* -- */

bool beNDMWMdlObject_isKind(beNDMWMdlObject* mgr, const char* kind) {
    return strcmp(kind, D_00515F40) == 0;
}

void func_003D1D40(beNDMWMdlObject* mgr) {
}

/* Give the ram a fresh work object; the optional arguments name the parent
   it hangs from (name, ram number). */
void beNDMWMdlObject_setupRam(beNDMWMdlObject* mgr, beBaseInfoRam* ram) {
    beModelCtrlInfoRamData* data;

    MakeDataList(ram);
    data = (beModelCtrlInfoRamData*)func_003022C0(ram->_dataList, D_00585BE0, D_00515F50);
    {
        beModelCtrlInfoWorkRef work(func_00350020(PoolOf2(mgr)), IG_ADOPT);

        igAssign(data->_work, work.p);
        if (beMessenger_getIntCount(mgr->_messenger)) {
            const char* name = beMessenger_getString(mgr->_messenger, 0);
            s32 ramNo;

            SetHitParentName(work, name);
            ramNo = beMessenger_getInt(mgr->_messenger, 0);
            work->_hitParentRamNo = ramNo;
        }
    }
    func_00396740(mgr, ram);
}

igMetaObject* beNDMWMdlObject_getMeta(void) {
    return beNDMWMdlObject_Meta;
}

}
