#include "game.h"

/* beHitLandModel: model collision against the land (MWCC C++). */
extern "C" {

extern igMetaObject* beHitLandModel_Meta;
extern igMetaObject* beHitLandModelInfo_Meta;
extern igMetaObject* beHitLandModelOrder_Meta;
extern char D_0050EAE0[]; /* "HITLANDMODEL" */
void beMessenger_entry(beMessenger* m, beBaseInfoRam* ram, const char* group, const char* info, s32 ramNo);

}

/* Reset by func_00388FF0. */
struct HitWork {
    u8 pad0[0x28];
    s32 unk28;
    s32 unk2C;
    u8 pad30[0x4];
    s32 unk34;
};

extern "C" {

/* -- */

void func_00190ED0(igMatrix44f* m, f32 x, f32 y, f32 z);

void beHitLandModel_prepare(beHitLandModel* self) {
    if (self->_order != NULL) {
        igUnref((igObject*)self->_order);
        self->_order = NULL;
    }
    if (self->_hitTraversal != NULL) {
        igUnref((igObject*)self->_hitTraversal);
        self->_hitTraversal = NULL;
    }
    if (self->_probe != NULL) {
        igUnref((igObject*)self->_probe);
        self->_probe = NULL;
    }
    self->_isHitWall = true;
    func_00190ED0(&self->_mat90, 0.0f, 1.5707964f, 0.0f);
}

igMetaObject* beHitLandModel_getInfoMeta(void) {
    return beHitLandModelInfo_Meta;
}

void beHitLandModel_entry(beHitLandModel* mgr, beBaseInfoRam* ram) {
    beMessenger_entry(mgr->_messenger, ram, D_0050EAE0, NULL, -1);
}

void beHitLandModel_updateRam(beHitLandModel* mgr) {
}

INCLUDE_ASM("asm/nonmatchings/beHitLandModel", beHitLandModel_handle);

INCLUDE_ASM("asm/nonmatchings/beHitLandModel", func_00386780);

INCLUDE_ASM("asm/nonmatchings/beHitLandModel", func_003876B0);

INCLUDE_ASM("asm/nonmatchings/beHitLandModel", func_00388010);

/* Clamp *v to lim: mode 0 raises it to lim, mode 1 lowers it. */
void func_00388590(void* self, f32* v, f32* lim, s32 mode) {
    if (*v < *lim) {
        if (mode == 0) {
            *v = *lim;
        }
    } else if (*v > *lim) {
        if (mode == 1) {
            *v = *lim;
        }
    }
}

/* Is a inside the x/z box: a >= d and c <= b on x and z. */
bool func_003885E0(void* self, igVec3f* a, igVec3f* b, igVec3f* c, igVec3f* d) {
    bool r = false;

    if (!(a->x < d->x) && c->x <= b->x && !(a->z < d->z) && c->z <= b->z) {
        r = true;
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/beHitLandModel", func_00388640);

IG_REF(igObject);

struct HitRec;
struct HitElem {
    u8 pad0[0x8];
    char* str;
    s32 unkC;
};

struct HitOwnerList {
    u8 pad0[0x8];
    s32 _count;
    u8 padC[0x4];
    HitElem** _data;
};

struct HitAdder {
    u8 pad0[0x20];
    char* name;
    s32 unk24;
    u8 pad28[0xC];
    s32 idx;
    HitOwnerList* list;
};

struct HitSrc {
    u8 pad0[0xC];
    igMatrix44f* mtx;
    u8 pad10[0x20];
    igVec3f pos;
};

extern bool D_00554C14;
extern void* D_00587110;
void* func_002F88E0(void* p);
igObject* func_0035E400(void* pool);
void func_00192F50(igVec3f* out, igVec3f* v, igMatrix44f* m);
HitRec* func_00388E60(HitRec* d, HitRec* s);

void func_00388C30(HitAdder* self, HitSrc* src) {
    char* name;
    char* s;
    HitElem* e;

    if (self->idx >= self->list->_count) {
        void* p = func_00301000(self);
        igObjectRef o(func_0035E400(D_00554C14 ? p : func_002F88E0(D_00587110)), IG_ADOPT);

        func_00301E30((igObjectList*)self->list, o);
    }
    func_00192F50(&src->pos, &src->pos, src->mtx);
    func_00388E60((HitRec*)((char*)self->list->_data[self->idx] + 0x10), (HitRec*)src);
    e = self->list->_data[self->idx];
    name = self->name;
    s = func_002EAF60(GetStrPool(), name);
    ReleaseStr(e->str);
    e->str = s;
    self->list->_data[self->idx]->unkC = self->unk24;
    self->idx++;
}


struct HitRefHolder {
    igObject* obj;
};

HitRefHolder* func_00388DE0(HitRefHolder* self, s16 flags) {
    if (self != NULL) {
        igUnref(self->obj);
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}

struct HitRec {
    s32 a0, a1, a2, a3, a4;
    f32 f[10];
    s32 b0, b1, b2;
};

HitRec* func_00388E60(HitRec* d, HitRec* s) {
    d->a0 = s->a0;
    d->a1 = s->a1;
    d->a2 = s->a2;
    d->a3 = s->a3;
    d->a4 = s->a4;
    d->f[0] = s->f[0];
    d->f[1] = s->f[1];
    d->f[2] = s->f[2];
    d->f[3] = s->f[3];
    d->f[4] = s->f[4];
    d->f[5] = s->f[5];
    d->f[6] = s->f[6];
    d->f[7] = s->f[7];
    d->f[8] = s->f[8];
    d->f[9] = s->f[9];
    d->b0 = s->b0;
    d->b1 = s->b1;
    d->b2 = s->b2;
    return d;
}

struct HitList {
    u8 pad0[0x8];
    s32 _count;
    u8 pad_c[0x4];
    void** _data;
};

struct HitStepper {
    u8 pad0[0x2C];
    s32 idx;
    HitList* list;
};

extern bool D_00554C14;
extern void* D_00587110;
void* func_002F88E0(void* p);
igObject* func_00188220(void* pool);
void func_0018D2B0(void* o, s32 a, s32 b);

void func_00388F00(HitStepper* self, s32 a, s32 b) {
    if (self->idx >= self->list->_count) {
        void* p = func_00301000(self);
        igObjectRef o(func_00188220(D_00554C14 ? p : func_002F88E0(D_00587110)), IG_ADOPT);

        func_00301E30((igObjectList*)self->list, o);
    }
    func_0018D2B0(self->list->_data[self->idx], a, b);
    self->idx++;
}

void func_00388FF0(HitWork* w) {
    w->unk28 = 0;
    w->unk2C = 0;
    w->unk34 = 0;
}

igMetaObject* beHitLandModelOrder_getMeta(void) {
    return beHitLandModelOrder_Meta;
}

igMetaObject* beHitLandModel_getMeta(void) {
    return beHitLandModel_Meta;
}

}
