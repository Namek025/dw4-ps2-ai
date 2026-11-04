#include "game.h"

extern "C" {
extern igMetaObject* beWeaponInfo_Meta;
extern igMetaObject* beWeapon_Meta;
}

void beWeapon_prepare(beWeapon* mgr) {
    igUnref(mgr->_attachDataList);
    mgr->_attachDataList = NULL;
}

igMetaObject* beWeapon_getInfoMeta(void) {
    return beWeaponInfo_Meta;
}

extern "C" {
extern bool D_00554C14;
extern void* D_00587110;
extern char D_0050FBF0[]; /* "PLAYERARMS" */
void* func_00301000(void* o);
void* func_002F88E0(void* p);
beWeaponAttachDataList* func_0033F8B0(void* pool); /* new attach data list */
beWeaponAttachData* func_0033FA80(void* pool);     /* new attach data */
}

IG_REF(beWeaponAttachData);
IG_REF(beWeaponAttachDataList);

static inline void* WeaponPool(void* o) {
    void* a = func_00301000(o);

    return D_00554C14 ? a : func_002F88E0(D_00587110);
}

void beWeapon_entry(beWeapon* mgr, beBaseInfoRam* ram) {
    if (mgr->_attachDataList == NULL) {
        beWeaponAttachDataListRef list(func_0033F8B0(WeaponPool(mgr)), IG_ADOPT);
        beWeaponAttachDataList* n = list.p;
        s32 i;

        igIncRef(n);
        igUnref(mgr->_attachDataList);
        mgr->_attachDataList = n;
        for (i = 0; i < 4; i++) {
            beWeaponAttachDataRef item(func_0033FA80(WeaponPool(mgr)), IG_ADOPT);
            func_00301E30(mgr->_attachDataList, item.p);
        }
    }
    beMessenger_entryRam(mgr->_messenger, ram, D_0050FBF0);
}

extern "C" {
void __dl__FPv(void* p);
}

/* Deleting out-of-line destructor of the smart pointer (exception cleanup of beWeapon_entry). */
extern "C" beWeaponAttachDataRef* func_003A19D0(beWeaponAttachDataRef* self, s16 flags) {
    if (self != NULL) {
        beWeaponAttachData* o = self->p;
        igUnref(o);
        if (flags > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

extern "C" void beWeapon_updateRam(void) {
}

INCLUDE_ASM("asm/nonmatchings/beWeapon", beWeapon_unhandled);

igMetaObject* beWeapon_getMeta(void) {
    return beWeapon_Meta;
}

INCLUDE_ASM("asm/nonmatchings/beWeapon", func_003A2130);

extern "C" {
void func_00236B10(void* a, void* b);
}

static inline void ReleaseField(void* owner, void* p) {
    if (p != NULL) {
        func_00236B10(owner, p);
    }
}

void func_003A3470(void* arg0, void* arg1) {
    void* owner = arg1;

    ReleaseField(owner, ((void**)arg0)[0x80 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x94 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x84 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x88 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x90 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x8C / 4]);
    ReleaseField(owner, ((void**)arg0)[0x74 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x38 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x44 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x48 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x14 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x24 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x20 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x1C / 4]);
    ReleaseField(owner, ((void**)arg0)[0x28 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x2C / 4]);
    ReleaseField(owner, ((void**)arg0)[0x3C / 4]);
    ReleaseField(owner, ((void**)arg0)[0x40 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x30 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x34 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x4C / 4]);
    ReleaseField(owner, ((void**)arg0)[0x50 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x18 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x10 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x54 / 4]);
    ReleaseField(owner, ((void**)arg0)[0xC / 4]);
    ReleaseField(owner, ((void**)arg0)[0x8 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x58 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x5C / 4]);
    ReleaseField(owner, ((void**)arg0)[0x98 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x60 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x64 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x68 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x6C / 4]);
    ReleaseField(owner, ((void**)arg0)[0x70 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x78 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x7C / 4]);
    ReleaseField(owner, ((void**)arg0)[0xA0 / 4]);
    ReleaseField(owner, ((void**)arg0)[0x9C / 4]);
}
