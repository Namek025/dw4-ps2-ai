#include "common.h"

struct igMetaObject;
struct igObject;
struct beMessenger;
struct beBaseInfoRam;
void beMessenger_call(beMessenger* m, const char* group, const char* info, s32 ramNo, const char* message);
void beMessenger_entry(beMessenger* m, beBaseInfoRam* ram, const char* group, const char* info, s32 ramNo);
void beMessenger_entryRam(beMessenger* m, beBaseInfoRam* ram, const char* group);
void beMessenger_select(beMessenger* m, const char* group, const char* info, s32 ramNo);
void beMessenger_pushString(beMessenger* m, const char* s);
void beMessenger_send(beMessenger* m, const char* message);
bool beMessenger_getBool(beMessenger* m, s32 i);
void beMessenger_selectGroup(beMessenger* m, const char* group);
void beMessenger_pushInt(beMessenger* m, s32 v);
void beMessenger_pushObject(beMessenger* m, igObject* v);
extern s32 D_004CEB30[][20];

struct igInsightCore;
extern igMetaObject* bePadManager_Meta;
void* func_00236F80(igInsightCore* insight, igMetaObject* meta);
extern "C" void func_002FF820(void*);

struct LoadIntf2PrepRef {
    int unk0;
    s32 unk4;
};

struct LoadIntf2Prep {
    char pad0[0xC];
    igInsightCore* unkC;
    char pad10[0x14];
    LoadIntf2PrepRef* unk24;
};

void beNDMWLoadIntf2_prepare(LoadIntf2Prep* o) {
    if (o->unk24 == NULL) {
        LoadIntf2PrepRef* r = (LoadIntf2PrepRef*)func_00236F80(o->unkC, bePadManager_Meta);
        if (r != NULL) {
            r->unk4 = r->unk4 + 1;
        }
        LoadIntf2PrepRef* old = o->unk24;
        if (old != NULL) {
            old->unk4 = old->unk4 - 1;
            if ((old->unk4 & 0x7FFFFF) == 0) {
                func_002FF820(old);
            }
        }
        o->unk24 = r;
    }
}

INCLUDE_ASM("asm/nonmatchings/beNDMWLoadIntf2", beNDMWLoadIntf2_updateRam);

struct LoadIntf2Ref {
    int unk0;
    s32 unk4;
};

struct LoadIntf2RefHolder {
    LoadIntf2Ref* p;
};

extern "C" void func_002FF820(void*);

LoadIntf2RefHolder* func_003E9010(LoadIntf2RefHolder* h, s16 flags) {
    if (h != NULL) {
        LoadIntf2Ref* r = h->p;
        if (r != NULL) {
            r->unk4 = r->unk4 - 1;
            if ((r->unk4 & 0x7FFFFF) == 0) {
                func_002FF820(r);
            }
        }
        if (flags > 0) {
            operator delete(h);
        }
    }
    return h;
}

struct LoadIntf2Obj {
    char pad0[0xC];
    u8 unkC;
    u8 unkD;
    u8 padE;
    s8 unkF;
    s32 unk10;
};

s32 func_003E9090(LoadIntf2Obj* o) {
    return o->unk10;
}

void func_003E90A0(LoadIntf2Obj* o, s8 v) {
    o->unkF = v;
}

u8 func_003E90B0(LoadIntf2Obj* o) {
    return o->unkD;
}

u8 func_003E90C0(LoadIntf2Obj* o) {
    return o->unkC;
}

void func_003E90D0(LoadIntf2Ref* o) {
    if (o != NULL) {
        o->unk4 = o->unk4 + 1;
    }
}

struct LoadIntf2Arr {
    char pad0[0x10];
    s32* unk10;
};

s32 func_003E90F0(LoadIntf2Arr* o, s32 i) {
    return o->unk10[i];
}

void func_003E9110(LoadIntf2Ref* o) {
    if (o != NULL) {
        o->unk4 = o->unk4 - 1;
        if ((o->unk4 & 0x7FFFFF) == 0) {
            func_002FF820(o);
        }
    }
}

struct LoadIntf2Arr2 {
    char pad0[8];
    s32 unk8;
    char padC[4];
    LoadIntf2Ref** unk10;
};

void func_003E9160(LoadIntf2Arr2* o, s32 i, LoadIntf2Ref* r) {
    LoadIntf2Ref* old;
    if (r != NULL) {
        r->unk4 = r->unk4 + 1;
    }
    old = o->unk10[i];
    if (old != NULL) {
        old->unk4 = old->unk4 - 1;
        if ((old->unk4 & 0x7FFFFF) == 0) {
            func_002FF820(old);
        }
    }
    if (o->unk8 != 0 && i >= 0 && i < o->unk8) {
        o->unk10[i] = r;
    }
}

struct LoadIntf2V3 {
    f32 x, y, z;
};

void func_003E9220(LoadIntf2V3* d, LoadIntf2V3* s) {
    d->x = s->x;
    d->y = s->y;
    d->z = s->z;
}

s32 func_003E9240(LoadIntf2Arr* o, s32 i) {
    return o->unk10[i];
}

s32 func_003E9260(LoadIntf2Arr* o, s32 i) {
    return o->unk10[i];
}

extern void func_003022C0(void);

void func_003E9280(void) {
    func_003022C0();
}

s32 func_003E92A0(s32* p) {
    return *p;
}

LoadIntf2RefHolder* func_003E92B0(LoadIntf2RefHolder* d, LoadIntf2RefHolder* s) {
    LoadIntf2Ref* n = s->p;
    if (n != NULL) {
        n->unk4 = n->unk4 + 1;
    }
    LoadIntf2Ref* r = d->p;
    if (r != NULL) {
        r->unk4 = r->unk4 - 1;
        if ((r->unk4 & 0x7FFFFF) == 0) {
            func_002FF820(r);
        }
    }
    d->p = n;
    return d;
}

extern s32 func_0036AE80(s32);

s32* func_003E9330(s32* d, s32* s) {
    *d = func_0036AE80(*s);
    return d;
}

void func_003E9360(s32* p, s32 v) {
    *p = v;
}

extern bool D_00554C14;
extern s32 func_002F88E0(s32);

#ifdef NON_MATCHING
/* Nonmatching: branch layout, beqz/bnez inverted and epilogue not duplicated */
s32 func_003E9370(s32* p, s32 v) {
    s32 r = v;
    if (D_00554C14 != false) {
    } else {
        r = func_002F88E0(*p);
    }
    return r;
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWLoadIntf2", func_003E9370);
#endif

s32 func_003E93B0(s32* p) {
    return *p;
}

extern igMetaObject* beNDMWLoadIntf2Info_Meta;

igMetaObject* beNDMWLoadIntf2_getInfoMeta(void) {
    return beNDMWLoadIntf2Info_Meta;
}

struct LoadIntf2Ent {
    char pad0[8];
    s32 unk8;
};

struct LoadIntf2Self14 {
    char pad0[0x14];
    beMessenger* unk14;
};

void beNDMWLoadIntf2_entry(LoadIntf2Self14* self, beBaseInfoRam* ram) {
    beMessenger_entryRam(self->unk14, ram, "LOADINTF2_ALL");
    beMessenger_entry(self->unk14, ram, "LOADINTF2", 0, ((LoadIntf2Ent*)ram)->unk8);
}

INCLUDE_ASM("asm/nonmatchings/beNDMWLoadIntf2", beNDMWLoadIntf2_handle);

LoadIntf2RefHolder* func_003EB480(LoadIntf2RefHolder* h, s16 flags) {
    if (h != NULL) {
        LoadIntf2Ref* r = h->p;
        if (r != NULL) {
            r->unk4 = r->unk4 - 1;
            if ((r->unk4 & 0x7FFFFF) == 0) {
                func_002FF820(r);
            }
        }
        if (flags > 0) {
            operator delete(h);
        }
    }
    return h;
}

LoadIntf2RefHolder* func_003EB500(LoadIntf2RefHolder* h, s16 flags) {
    if (h != NULL) {
        LoadIntf2Ref* r = h->p;
        if (r != NULL) {
            r->unk4 = r->unk4 - 1;
            if ((r->unk4 & 0x7FFFFF) == 0) {
                func_002FF820(r);
            }
        }
        if (flags > 0) {
            operator delete(h);
        }
    }
    return h;
}

LoadIntf2RefHolder* func_003EB580(LoadIntf2RefHolder* h, s16 flags) {
    if (h != NULL) {
        LoadIntf2Ref* r = h->p;
        if (r != NULL) {
            r->unk4 = r->unk4 - 1;
            if ((r->unk4 & 0x7FFFFF) == 0) {
                func_002FF820(r);
            }
        }
        if (flags > 0) {
            operator delete(h);
        }
    }
    return h;
}

LoadIntf2RefHolder* func_003EB600(LoadIntf2RefHolder* h, s16 flags) {
    if (h != NULL) {
        LoadIntf2Ref* r = h->p;
        if (r != NULL) {
            r->unk4 = r->unk4 - 1;
            if ((r->unk4 & 0x7FFFFF) == 0) {
                func_002FF820(r);
            }
        }
        if (flags > 0) {
            operator delete(h);
        }
    }
    return h;
}

LoadIntf2RefHolder* func_003EB680(LoadIntf2RefHolder* h, s16 flags) {
    if (h != NULL) {
        LoadIntf2Ref* r = h->p;
        if (r != NULL) {
            r->unk4 = r->unk4 - 1;
            if ((r->unk4 & 0x7FFFFF) == 0) {
                func_002FF820(r);
            }
        }
        if (flags > 0) {
            operator delete(h);
        }
    }
    return h;
}

LoadIntf2RefHolder* func_003EB700(LoadIntf2RefHolder* h, s16 flags) {
    if (h != NULL) {
        LoadIntf2Ref* r = h->p;
        if (r != NULL) {
            r->unk4 = r->unk4 - 1;
            if ((r->unk4 & 0x7FFFFF) == 0) {
                func_002FF820(r);
            }
        }
        if (flags > 0) {
            operator delete(h);
        }
    }
    return h;
}

LoadIntf2RefHolder* func_003EB780(LoadIntf2RefHolder* h, s16 flags) {
    if (h != NULL) {
        LoadIntf2Ref* r = h->p;
        if (r != NULL) {
            r->unk4 = r->unk4 - 1;
            if ((r->unk4 & 0x7FFFFF) == 0) {
                func_002FF820(r);
            }
        }
        if (flags > 0) {
            operator delete(h);
        }
    }
    return h;
}

LoadIntf2RefHolder* func_003EB800(LoadIntf2RefHolder* h, s16 flags) {
    if (h != NULL) {
        LoadIntf2Ref* r = h->p;
        if (r != NULL) {
            r->unk4 = r->unk4 - 1;
            if ((r->unk4 & 0x7FFFFF) == 0) {
                func_002FF820(r);
            }
        }
        if (flags > 0) {
            operator delete(h);
        }
    }
    return h;
}

LoadIntf2RefHolder* func_003EB880(LoadIntf2RefHolder* h, s16 flags) {
    if (h != NULL) {
        LoadIntf2Ref* r = h->p;
        if (r != NULL) {
            r->unk4 = r->unk4 - 1;
            if ((r->unk4 & 0x7FFFFF) == 0) {
                func_002FF820(r);
            }
        }
        if (flags > 0) {
            operator delete(h);
        }
    }
    return h;
}

LoadIntf2RefHolder* func_003EB900(LoadIntf2RefHolder* h, s16 flags) {
    if (h != NULL) {
        LoadIntf2Ref* r = h->p;
        if (r != NULL) {
            r->unk4 = r->unk4 - 1;
            if ((r->unk4 & 0x7FFFFF) == 0) {
                func_002FF820(r);
            }
        }
        if (flags > 0) {
            operator delete(h);
        }
    }
    return h;
}

LoadIntf2RefHolder* func_003EB980(LoadIntf2RefHolder* h, s16 flags) {
    if (h != NULL) {
        LoadIntf2Ref* r = h->p;
        if (r != NULL) {
            r->unk4 = r->unk4 - 1;
            if ((r->unk4 & 0x7FFFFF) == 0) {
                func_002FF820(r);
            }
        }
        if (flags > 0) {
            operator delete(h);
        }
    }
    return h;
}

extern igMetaObject* beNDMWLoadIntf2_Meta;

igMetaObject* beNDMWLoadIntf2_getMeta(void) {
    return beNDMWLoadIntf2_Meta;
}

INCLUDE_ASM("asm/nonmatchings/beNDMWLoadIntf2", func_003EBA10);

struct LoadIntf2Txt {
    char pad0[0x10];
    beMessenger* unk10;
};

void func_003EBCE0(LoadIntf2Txt* o, const char* a, const char* b) {
    beMessenger_select(o->unk10, "TEXTCTL", 0, -1);
    beMessenger_pushString(o->unk10, a);
    beMessenger_pushString(o->unk10, b);
    beMessenger_pushString(o->unk10, "CLR");
    beMessenger_send(o->unk10, "SWITCH");
}

extern void func_003EBA10(s32, s32, s32, s32, s32, s32);

void func_003EBD70(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
    func_003EBA10(a, b, D_004CEB30[c][d], e, f, g);
}

void func_003EBDC0(s32 a, s32 b, s32 c, s32 d) {
    func_003EBCE0((LoadIntf2Txt*)a, (const char*)b, (const char*)D_004CEB30[c][d]);
}

INCLUDE_ASM("asm/nonmatchings/beNDMWLoadIntf2", func_003EBE00);

struct LoadIntf2TabA {
    u32 key;
    s32 unk4;
    LoadIntf2TabA* list;
};

struct LoadIntf2TabB {
    s32 a, b, c;
};

extern LoadIntf2TabA D_004CDA40[];
extern s32 D_004CDA48[][3];

void func_003EC050(LoadIntf2Txt* o, s32 key) {
    s32 i = 0;
    LoadIntf2TabA* t = D_004CDA40;
    LoadIntf2TabB* e;
    while (1) {
        if ((t->key & 0xFFFFF) == key) {
            break;
        }
        t = (LoadIntf2TabA*)((char*)t + 0xC);
        i++;
    }
    if (i != -1) {
        e = (LoadIntf2TabB*)D_004CDA48[i][0];
        while (1) {
            if (e->a == 0) {
                break;
            }
            func_003EBCE0(o, (const char*)e->a, (const char*)D_004CEB30[e->b][e->c]);
            e++;
        }
    }
}

void func_003EC120(LoadIntf2Txt* o, const char* a, s32 b, s32 c, s32 d, s32 e, s32 f) {
    const char* temp;

    beMessenger_selectGroup(o->unk10, "TEXTCTL");
    beMessenger_pushObject(o->unk10, (igObject*)d);
    beMessenger_pushInt(o->unk10, 0x3F);
    beMessenger_send(o->unk10, "REPLACEUNKNOWN");
    temp = (const char*)D_004CEB30[b][c];
    switch (e) {
    case 0:
        beMessenger_select(o->unk10, "TEXTCTL", 0, -1);
        beMessenger_pushString(o->unk10, a);
        beMessenger_pushString(o->unk10, temp);
        beMessenger_pushString(o->unk10, "LEFT");
        beMessenger_send(o->unk10, "ALIGN");
        break;
    case 1:
        beMessenger_select(o->unk10, "TEXTCTL", 0, -1);
        beMessenger_pushString(o->unk10, a);
        beMessenger_pushString(o->unk10, temp);
        beMessenger_pushString(o->unk10, "CENTER");
        beMessenger_send(o->unk10, "ALIGN");
        break;
    case 2:
        beMessenger_select(o->unk10, "TEXTCTL", 0, -1);
        beMessenger_pushString(o->unk10, a);
        beMessenger_pushString(o->unk10, temp);
        beMessenger_pushString(o->unk10, "RIGHT");
        beMessenger_send(o->unk10, "ALIGN");
        break;
    }
    beMessenger_selectGroup(o->unk10, "TEXTCTL");
    beMessenger_pushString(o->unk10, a);
    beMessenger_pushString(o->unk10, temp);
    beMessenger_pushObject(o->unk10, (igObject*)d);
    beMessenger_send(o->unk10, "TEXTSET");
    beMessenger_select(o->unk10, "TEXTCTL", 0, -1);
    beMessenger_pushString(o->unk10, a);
    beMessenger_pushString(o->unk10, temp);
    beMessenger_pushString(o->unk10, "SET");
    beMessenger_send(o->unk10, "SWITCH");
    switch (f) {
    case 0:
        beMessenger_select(o->unk10, "TEXTCTL", 0, -1);
        beMessenger_pushString(o->unk10, a);
        beMessenger_pushString(o->unk10, temp);
        beMessenger_send(o->unk10, "DEFAULTCOLOR");
        break;
    case 1:
        beMessenger_select(o->unk10, "TEXTCTL", 0, -1);
        beMessenger_pushString(o->unk10, a);
        beMessenger_pushString(o->unk10, temp);
        beMessenger_send(o->unk10, "CANCELCOLOR");
        /* fallthrough */
    case 2:
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/beNDMWLoadIntf2", func_003EC450);

struct LoadIntf2Sel {
    char pad0[0x10];
    beMessenger* unk10;
};

bool func_003ECA00(LoadIntf2Sel* o, s32 ramNo, const char* info, const char* str) {
    beMessenger_select(o->unk10, "MODELCTRL", info, ramNo);
    beMessenger_pushString(o->unk10, str);
    beMessenger_send(o->unk10, "ISSEQNAME");
    return beMessenger_getBool(o->unk10, 0);
}

struct LoadIntf2Obj20;

struct LoadIntf2Flags {
    char pad0[0x1C];
    u8 unk1C, unk1D, unk1E, unk1F, unk20, unk21, unk22;
};

void func_003ECA70(LoadIntf2Flags* o) {
    o->unk1C = 1;
    o->unk1D = 0;
    o->unk1E = 0;
    o->unk22 = 0;
    o->unk1F = 0;
    o->unk20 = 0;
    o->unk21 = 0;
}

void func_003ECB90(LoadIntf2Obj20* o);

void func_003ECAA0(LoadIntf2Flags* o) {
    func_003ECB90((LoadIntf2Obj20*)o);
    o->unk1D = 1;
}

struct LoadIntf2Obj34 {
    char pad0[0x21];
    u8 unk21;
    char pad22[0x12];
    s32 unk34;
};

void func_003ECAD0(LoadIntf2Obj34* o, s32 v) {
    o->unk21 = 1;
    o->unk34 = v;
}

struct LoadIntf2ObjAE0 {
    char pad0[0x21];
    u8 unk21, unk22;
    char pad23;
    s32 unk24, unk28;
    char pad2C[4];
    s32 unk30, unk34;
};

void func_003ECAE0(LoadIntf2ObjAE0* o, s32 a, s32 b) {
    o->unk21 = 1;
    o->unk22 = 1;
    o->unk34 = a;
    o->unk30 = b;
    o->unk28 = 0;
    o->unk24 = 0;
}

struct LoadIntf2ObjB00 {
    char pad0[0x21];
    u8 unk21;
    u8 unk22;
    char pad23;
    s32 unk24;
    s32 unk28;
    char pad2C[8];
    s32 unk34;
};

void func_003ECB00(LoadIntf2ObjB00* o, s32 v) {
    o->unk21 = 1;
    o->unk22 = 1;
    o->unk34 = v;
    o->unk28 = 0;
    o->unk24 = 1;
}

struct LoadIntf2Wait {
    char pad0[0x10];
    beMessenger* unk10;
    char pad14[0xE];
    u8 unk22;
    char pad23[9];
    s32 unk2C;
};

void func_003ECB20(LoadIntf2Wait* o) {
    o->unk22 = 0;
    beMessenger_call(o->unk10, "MODELCTRL", "j_chrselmesswin", o->unk2C, "SQ_WAIT");
    func_003ECB90((LoadIntf2Obj20*)o);
}

struct LoadIntf2Obj22 {
    char pad0[0x22];
    u8 unk22;
};

u8 func_003ECB70(LoadIntf2Obj22* o) {
    return o->unk22;
}

struct LoadIntf2Obj30 {
    char pad0[0x30];
    s32 unk30;
};

s32 func_003ECB80(LoadIntf2Obj30* o) {
    return o->unk30;
}

struct LoadIntf2Obj20 {
    char pad0[0x20];
    u8 unk20;
    u8 unk21;
};

void func_003ECB90(LoadIntf2Obj20* o) {
    o->unk20 = 1;
    o->unk21 = 1;
}

struct LoadIntf2ObjBA {
    char pad0[0x1F];
    u8 unk1F;
    char pad20;
    u8 unk21;
    char pad22[0x16];
    s32 unk38;
    s32 unk3C;
};

void func_003ECBA0(LoadIntf2ObjBA* o, s32 v) {
    o->unk21 = 1;
    o->unk1F = 1;
    o->unk38 = v;
    o->unk3C = 0;
}

struct LoadIntf2ObjBC {
    char pad0[0x1F];
    u8 unk1F;
    char pad20[2];
    u8 unk22, unk23;
    s32 unk24;
    char pad28[8];
    s32 unk30, unk34;
};

void func_003ECBC0(LoadIntf2ObjBC* o) {
    o->unk1F = 0;
    if (o->unk23 == 1) {
        if (o->unk22 == 1) {
            switch (o->unk24) {
            case 0:
                func_003ECAE0((LoadIntf2ObjAE0*)o, o->unk34, o->unk30);
                break;
            case 1:
                func_003ECB00((LoadIntf2ObjB00*)o, o->unk34);
                break;
            }
        } else {
            func_003ECAD0((LoadIntf2Obj34*)o, o->unk34);
        }
    } else {
        func_003ECB90((LoadIntf2Obj20*)o);
    }
}

#ifdef NON_MATCHING
/* Nonmatching: MWCC hoists o->unk34->unk2C from the first loop test into the body (a1); the original reloads it */
struct CC60Idx { char pad0[8]; s32 count; char padC[4]; s32* data; };
struct CC60Ent { char pad0[8]; s32 id; char padC[4]; CC60Idx* idx; };
struct CC60EntList { char pad0[8]; s32 count; char padC[4]; CC60Ent** data; };
struct CC60Flag { char pad0[8]; u8 unk8; char pad9[7]; u8 unk10; };
struct CC60FlagList { char pad0[8]; s32 count; char padC[4]; CC60Flag** data; };
struct CC60Mid { char pad0[0x2C]; CC60FlagList* unk2C; };
struct CC60Obj {
    char pad0[0x20];
    CC60EntList* unk20;
    char pad24[4];
    CC60FlagList* unk28;
    char pad2C[8];
    CC60Mid* unk34;
};
extern void func_003F1490(void*);
static inline CC60Flag* CC60Get(CC60Obj* o, s32 i) { return o->unk34->unk2C->data[i]; }
static inline s32 CC60Count(CC60Obj* o) { return o->unk34->unk2C->count; }

void func_003ECC60(CC60Obj* o) {
    CC60Idx* x;
    s32 i, j, k;
    func_003F1490(o->unk34);
    o->unk28->data[0]->unk10 = 0;
    o->unk28->data[1]->unk10 = 0;
    o->unk28->data[2]->unk10 = 0;
    o->unk28->data[3]->unk10 = 0;
    for (i = 0; i < CC60Count(o); i++) {
        if (CC60Get(o, i)->unk8 == 0) {
            for (j = 0; j < o->unk20->count; j++) {
                CC60Ent* e = o->unk20->data[j];
                if (i == e->id) {
                    x = e->idx;
                    for (k = 0; k < x->count; k++) {
                        o->unk28->data[x->data[k]]->unk10 = 1;
                    }
                }
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWLoadIntf2", func_003ECC60);
#endif

struct LoadIntf2Slot {
    char pad0[8];
    s32 unk8;
    char padC[3];
    s8 unkF;
    char pad10[0x14];
    s32 unk24, unk28, unk2C, unk30;
};

struct LoadIntf2SlotArr {
    char pad0[0x10];
    LoadIntf2Slot** unk10;
};

struct LoadIntf2D050Mid {
    char pad0[0x20];
    s32 unk20;
    u8 unk24;
};

struct LoadIntf2D050Owner {
    char pad0[0x28];
    LoadIntf2SlotArr* unk28;
    char pad2C[8];
    LoadIntf2D050Mid* unk34;
};

struct LoadIntf2Init;
void func_003F1460(LoadIntf2Init* o, s32 a, s32 b);

s32 func_003ECDC0(LoadIntf2D050Owner* o, s32 sel, void* a2, s32 v, s32 a4, s32 a5, s32 mode) {
    LoadIntf2Slot* p;
    s32 var17 = 0;
    s32 var16 = 0;

    switch (mode) {
    case 0:
        var16 = o->unk28->unk10[sel]->unk24;
        break;
    case 1:
        var16 = o->unk28->unk10[sel]->unk28;
        break;
    case 2:
        var16 = o->unk28->unk10[sel]->unk2C;
        break;
    case 3:
        var16 = o->unk28->unk10[sel]->unk30;
        break;
    }

    switch (var16) {
    case 0:
        func_003ECB00((LoadIntf2ObjB00*)a2, v);
        var16 += 1;
        break;
    case 1:
        if (!(func_003ECB70((LoadIntf2Obj22*)a2) & 0xFF)) {
            var16 += 1;
        }
        break;
    case 2:
        func_003ECAE0((LoadIntf2ObjAE0*)a2, a4, a5);
        var16 += 1;
        break;
    case 3:
        if (!(func_003ECB70((LoadIntf2Obj22*)a2) & 0xFF)) {
            s32 t = func_003ECB80((LoadIntf2Obj30*)a2);
            switch (t) {
            case 0:
                var17 = 1;
                break;
            case 1:
                var17 = 2;
                break;
            }
        }
        break;
    }

    switch (mode) {
    case 0:
        p = o->unk28->unk10[sel];
        p->unk24 = var16;
        p->unk30 = 0;
        p->unk2C = 0;
        p->unk28 = 0;
        break;
    case 1:
        p = o->unk28->unk10[sel];
        p->unk28 = var16;
        p->unk30 = 0;
        p->unk2C = 0;
        break;
    case 2:
        p = o->unk28->unk10[sel];
        p->unk2C = var16;
        p->unk30 = 0;
        break;
    case 3:
        o->unk28->unk10[sel]->unk30 = var16;
        break;
    }
    return var17;
}

#ifdef NON_MATCHING
/* Nonmatching: registers and the switch bodies match; the case 1 and case 3 inner ifs are laid out with the out-of-line branch the original uses differently */
s32 func_003ED050(LoadIntf2D050Owner* o, s32 sel, s32 unused, s32 v, s32 mode) {
    LoadIntf2Slot* p;
    s32 var17 = 0;
    s32 var16 = 0;

    switch (mode) {
    case 0:
        var16 = o->unk28->unk10[sel]->unk24;
        break;
    case 1:
        var16 = o->unk28->unk10[sel]->unk28;
        break;
    case 2:
        var16 = o->unk28->unk10[sel]->unk2C;
        break;
    case 3:
        var16 = o->unk28->unk10[sel]->unk30;
        break;
    }

    switch (var16) {
    case 0:
        func_003F1460((LoadIntf2Init*)o->unk34, 3, v);
        var16 += 1;
        break;
    case 1:
        if (o->unk34->unk24 == 1) {
            if (o->unk34->unk20 != 2) {
                var17 = 2;
            } else {
                var16 += 1;
            }
        }
        break;
    case 2:
        func_003F1460((LoadIntf2Init*)o->unk34, 1, v);
        var16 += 1;
        break;
    case 3:
        if (o->unk34->unk24 == 1) {
            var17 = (o->unk34->unk20 == 2) ? 1 : 2;
        }
        break;
    }

    switch (mode) {
    case 0:
        p = o->unk28->unk10[sel];
        p->unk24 = var16;
        p->unk30 = 0;
        p->unk2C = 0;
        p->unk28 = 0;
        break;
    case 1:
        p = o->unk28->unk10[sel];
        p->unk28 = var16;
        p->unk30 = 0;
        p->unk2C = 0;
        break;
    case 2:
        p = o->unk28->unk10[sel];
        p->unk2C = var16;
        p->unk30 = 0;
        break;
    case 3:
        o->unk28->unk10[sel]->unk30 = var16;
        break;
    }
    return var17;
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWLoadIntf2", func_003ED050);
#endif

INCLUDE_ASM("asm/nonmatchings/beNDMWLoadIntf2", func_003ED2E0);

INCLUDE_ASM("asm/nonmatchings/beNDMWLoadIntf2", func_003ED650);

LoadIntf2RefHolder* func_003EDBA0(LoadIntf2RefHolder* h, s16 flags) {
    if (h != NULL) {
        LoadIntf2Ref* r = h->p;
        if (r != NULL) {
            r->unk4 = r->unk4 - 1;
            if ((r->unk4 & 0x7FFFFF) == 0) {
                func_002FF820(r);
            }
        }
        if (flags > 0) {
            operator delete(h);
        }
    }
    return h;
}

INCLUDE_ASM("asm/nonmatchings/beNDMWLoadIntf2", func_003EDC20);

INCLUDE_ASM("asm/nonmatchings/beNDMWLoadIntf2", func_003EDF60);

struct LoadIntf2SlotOwner {
    char pad0[0x28];
    LoadIntf2SlotArr* unk28;
    char pad2C[0x18];
    s32 unk44;
};

#define SLOT_AT_OFF (*(LoadIntf2Slot**)((char*)o->unk28->unk10 + off))

#ifdef NON_MATCHING
/* Nonmatching: the 60 constant and the slot temp swap registers (a2/a3) in the loop */
void func_003EE190(LoadIntf2SlotOwner* o, s32 sel) {
    LoadIntf2Slot* p;
    LoadIntf2Slot* p2;
    s32 i = 0;
    s32 off = 0;
    do {
        i++;
        p = SLOT_AT_OFF;
        p->unk24 = 1;
        p->unk30 = 0;
        p->unk2C = 0;
        p->unk28 = 0;
        SLOT_AT_OFF->unkF = 0;
        p = SLOT_AT_OFF;
        off += 4;
        p->unk8 = 0x3C;
    } while (i < 4);
    p2 = o->unk28->unk10[sel];
    p2->unk24 = 0;
    p2->unk30 = 0;
    p2->unk2C = 0;
    p2->unk28 = 0;
    o->unk28->unk10[sel]->unkF = 1;
    o->unk28->unk10[sel]->unk8 = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWLoadIntf2", func_003EE190);
#endif

void func_003EE250(LoadIntf2SlotOwner* o, s32 sel, s32 v) {
    s32 i;
    s32 off;
    LoadIntf2Slot* p;
    s32* q;
    for (i = 0, off = 0; i < 4; i++, off += 4) {
        p = SLOT_AT_OFF;
        p->unk24 = 1;
        p->unk30 = 0;
        p->unk2C = 0;
        p->unk28 = 0;
        SLOT_AT_OFF->unkF = 0;
        q = &SLOT_AT_OFF->unk8;
        if (*q <= 0) {
            *q = 10;
        }
    }
    p = o->unk28->unk10[sel];
    p->unk24 = v;
    p->unk30 = 0;
    p->unk2C = 0;
    p->unk28 = 0;
    o->unk28->unk10[sel]->unkF = 1;
    o->unk28->unk10[sel]->unk8 = 0;
    o->unk44 = sel;
}

struct LdLst {
    char pad0[8];
    s32 count;
    char padC[4];
    struct LdObj** data;
};

struct LdLst2 {
    char pad0[8];
    s32 count;
    char padC[4];
    s32* data;
};

struct LdObj {
    char pad0[4];
    u32 ref;
    char pad8[8];
    LdLst2* list;
};

struct LdOwner {
    char pad0[0x20];
    LdLst* unk20;
};

extern "C" void func_002D6CF0(void*, s32);

#ifdef NON_MATCHING
/* Nonmatching: inner loop strength-reduced to a pointer increment (orig keeps base+offset), found in t0 not t1 */
void func_003EE320(LdOwner* o, s32 key) {
    LdLst* l = o->unk20;
    s32 j;
    s32 i;
    s32 off;
    s32 off2;
    LdObj* e;
    u8 found = 0;
    for (i = l->count - 1, off = i * 4; i >= 0; i--, off -= 4) {
        LdLst2* sub = (*(LdObj**)((char*)l->data + off))->list;
        s32 n = sub->count;
        for (j = 0, off2 = 0; j < n; j++, off2 += 4) {
            char* base = (char*)sub->data;
            if (key == *(s32*)(base + off2)) {
                func_002D6CF0(sub, j);
                found = 1;
                break;
            }
        }
        if (found == 1) {
            break;
        }
    }
    if (found == 1) {
        for (i = l->count - 1, off = i * 4; i >= 0; i--, off -= 4) {
            e = *(LdObj**)((char*)l->data + off);
            if (e->list->count == 0) {
                if (e != NULL) {
                    e->ref--;
                    if ((e->ref & 0x7FFFFF) == 0) {
                        func_002FF820(e);
                    }
                }
                func_002D6CF0(l, i);
                l->data[l->count] = 0;
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWLoadIntf2", func_003EE320);
#endif

extern "C" void* memcpy(void*, const void*, unsigned);

struct LoadIntf2Buf {
    char pad0[0x1C];
    void* unk1C;
    char pad20[4];
    u32 unk24;
};

extern void func_003EE4D0(LoadIntf2Buf*);

#ifdef NON_MATCHING
/* Nonmatching: lw a2 is scheduled before move s0,a0 in the original (same as func_003EE690) */
void func_003EE490(LoadIntf2Buf* o, LoadIntf2Buf* src) {
    memcpy(o->unk1C, src->unk1C, o->unk24);
    func_003EE4D0(o);
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWLoadIntf2", func_003EE490);
#endif

struct LoadIntf2ObjE4 {
    char pad0[8];
    u16 unk8[9];
    char pad1A[2];
    char* unk1C;
    u8 unk20_pad[0x8];
    u8 unk28;
    char pad29[3];
    u32 unk2C;
    char* unk30;
    char* unk34;
    s32 unk38, unk3C, unk40, unk44, unk48;
};

struct L4StrPool;
struct L4StrHeader {
    void* owner;
    s32 refs;
};
extern "C" {
extern L4StrPool* D_00586F20;
void* func_002EB010(u32 size);
L4StrPool* func_002EADF0(L4StrPool* pool);
char* func_002EAF60(L4StrPool* pool, const char* s);
void func_0030C940(void* owner, L4StrHeader* h);
}

static inline L4StrPool* L4GetPool(void) {
    if (D_00586F20 == NULL) {
        L4StrPool* pool = (L4StrPool*)func_002EB010(0x10);
        if (pool != NULL) {
            pool = func_002EADF0(pool);
        }
        D_00586F20 = pool;
    }
    return D_00586F20;
}

static inline void L4Release(char* s) {
    if (s != NULL) {
        L4StrHeader* h = (L4StrHeader*)s - 1;
        if (--h->refs == 0) {
            func_0030C940(h->owner, h);
        }
    }
}

void func_003EE4D0(LoadIntf2Buf* self) {
    LoadIntf2ObjE4* o = (LoadIntf2ObjE4*)self;
    char* b = o->unk1C;
    char* s;
    s32 i;
    u16* src;
    o->unk28 = *(u8*)b;
    o->unk38 = *(s32*)(b + 0x48);
    o->unk3C = *(s32*)(b + 0x4C);
    o->unk40 = *(s32*)(b + 0x50);
    o->unk44 = *(s32*)(b + 0x54);
    o->unk48 = *(s32*)(b + 0x58);
    s = func_002EAF60(L4GetPool(), b + 8);
    L4Release(o->unk30);
    o->unk30 = s;
    s = func_002EAF60(L4GetPool(), b + 0x28);
    L4Release(o->unk34);
    o->unk34 = s;
    o->unk2C = *(u32*)(b + 4);
    src = (u16*)(b + 0x28);
    for (i = 0; i < 9; i++) {
        if (src[i] == 0) {
            break;
        }
        o->unk8[i] = src[i];
    }
    for (; i < 9; i++) {
        o->unk8[i] = 0;
    }
}

#ifdef NON_MATCHING
/* Nonmatching: lw a2 is scheduled before move s0,a0 in the original */
void func_003EE690(LoadIntf2Buf* o, const void* src) {
    memcpy(o->unk1C, src, o->unk24);
    func_003EE4D0(o);
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWLoadIntf2", func_003EE690);
#endif

INCLUDE_ASM("asm/nonmatchings/beNDMWLoadIntf2", func_003EE6D0);

INCLUDE_ASM("asm/nonmatchings/beNDMWLoadIntf2", func_003EF1E0);

INCLUDE_ASM("asm/nonmatchings/beNDMWLoadIntf2", func_003EF5D0);

struct LoadIntf2CpItem {
    char pad0[0x1C];
    void* unk1C;
    char pad20[4];
    s32 unk24;
};

struct LoadIntf2CpList {
    char pad0[8];
    s32 unk8;
    char padC[4];
    LoadIntf2CpItem** unk10;
};

struct LoadIntf2CpSet {
    char pad0[0xC];
    LoadIntf2CpList* unkC;
};

void func_003F0D60(LoadIntf2CpSet* o, char* dst0, s32 sel, LoadIntf2CpItem* alt) {
    char* dst = dst0;
    LoadIntf2CpList* l;
    LoadIntf2CpItem* it;
    void* src;
    s32 i;
    s32 n;
    s32 off;

    l = o->unkC;
    i = 0;
    if (0 < l->unk8) {
        off = 0;
        do {
            if (i == sel) {
                src = alt->unk1C;
                n = alt->unk24;
            } else {
                it = *(LoadIntf2CpItem**)((char*)l->unk10 + off);
                src = it->unk1C;
                n = it->unk24;
            }
            memcpy(dst, src, n);
            l = o->unkC;
            i++;
            dst += n;
            off += 4;
        } while (i < l->unk8);
    }
}

struct LoadIntf2Item {
    char pad0[0x24];
    s32 unk24;
};

struct LoadIntf2List {
    char pad0[8];
    s32 unk8;
    char padC[4];
    LoadIntf2Item** unk10;
};

struct LoadIntf2Set {
    char pad0[0xC];
    LoadIntf2List* unkC;
};

void func_003EE690(LoadIntf2Buf* o, const void* src);

void func_003F0E30(LoadIntf2Set* o, char* dst) {
    s32 i;
    for (i = 0; i < o->unkC->unk8; i++) {
        func_003EE690((LoadIntf2Buf*)o->unkC->unk10[i], dst);
        dst += o->unkC->unk10[i]->unk24;
    }
}

struct LoadIntf2Load {
    char pad0[8];
    s32 unk8;
    char padC;
    u8 unkD;
    u8 unkE;
    char padF[0x24 - 0xF];
    u8 unk24;
    char pad25[0x2C - 0x25];
    void* unk2C;
};

struct igMemoryPool;
extern void* D_00587110;
extern "C" void* func_00301000(void* o);
extern "C" void* func_002F88E0(void* p);
extern "C" void func_00301E30(void* list, void* o);
LoadIntf2Ref* func_003C5EE0(igMemoryPool* pool);
LoadIntf2Ref* func_003C5CF0(igMemoryPool* pool);
LoadIntf2Ref* func_003C5AA0(igMemoryPool* pool);

static inline igMemoryPool* LoadIntf2GetPool(void* a) {
    return (igMemoryPool*)(D_00554C14 ? a : func_002F88E0(D_00587110));
}

struct LoadIntf2ObjRef {
    LoadIntf2Ref* p;

    LoadIntf2ObjRef(LoadIntf2Ref* o) : p(o) {}
    ~LoadIntf2ObjRef() {
        LoadIntf2Ref* r = p;
        if (r != NULL) {
            r->unk4 = r->unk4 - 1;
            if ((r->unk4 & 0x7FFFFF) == 0) {
                func_002FF820(r);
            }
        }
    }
    operator LoadIntf2Ref*() const { return p; }
};

void func_003F0ED0(LoadIntf2Load* o, s32 arg1, s32 arg2) {
    s32 i;

    o->unk8 = arg1;
    o->unkE = 0;
    o->unk24 = 0;
    o->unkD = 0;
    switch (arg2) {
    case 0:
        for (i = 0; i < 5; i++) {
            LoadIntf2ObjRef r(func_003C5EE0(LoadIntf2GetPool(func_00301000(o))));
            func_00301E30(o->unk2C, r);
        }
        break;
    case 1:
        for (i = 0; i < 2; i++) {
            LoadIntf2ObjRef r(func_003C5CF0(LoadIntf2GetPool(func_00301000(o))));
            func_00301E30(o->unk2C, r);
        }
        break;
    case 2:
        for (i = 0; i < 9; i++) {
            LoadIntf2ObjRef r(func_003C5AA0(LoadIntf2GetPool(func_00301000(o))));
            func_00301E30(o->unk2C, r);
        }
        break;
    }
}

LoadIntf2RefHolder* func_003F10F0(LoadIntf2RefHolder* h, s16 flags) {
    if (h != NULL) {
        LoadIntf2Ref* r = h->p;
        if (r != NULL) {
            r->unk4 = r->unk4 - 1;
            if ((r->unk4 & 0x7FFFFF) == 0) {
                func_002FF820(r);
            }
        }
        if (flags > 0) {
            operator delete(h);
        }
    }
    return h;
}

LoadIntf2RefHolder* func_003F1170(LoadIntf2RefHolder* h, s16 flags) {
    if (h != NULL) {
        LoadIntf2Ref* r = h->p;
        if (r != NULL) {
            r->unk4 = r->unk4 - 1;
            if ((r->unk4 & 0x7FFFFF) == 0) {
                func_002FF820(r);
            }
        }
        if (flags > 0) {
            operator delete(h);
        }
    }
    return h;
}

LoadIntf2RefHolder* func_003F11F0(LoadIntf2RefHolder* h, s16 flags) {
    if (h != NULL) {
        LoadIntf2Ref* r = h->p;
        if (r != NULL) {
            r->unk4 = r->unk4 - 1;
            if ((r->unk4 & 0x7FFFFF) == 0) {
                func_002FF820(r);
            }
        }
        if (flags > 0) {
            operator delete(h);
        }
    }
    return h;
}

void beMessenger_pushInt(beMessenger* m, s32 v);
s32 beMessenger_getBoolCount(beMessenger* m);

struct LoadIntf2Msg {
    char pad0[8];
    beMessenger* unk8;
    u8 unkC;
    u8 unkD;
};

void func_003F1270(LoadIntf2Msg* o) {
    beMessenger_select(o->unk8, "SAVEUTILITY", 0, -1);
    beMessenger_pushInt(o->unk8, 4);
    beMessenger_send(o->unk8, "GENERATE");
    if (beMessenger_getBoolCount(o->unk8) > 0) {
        o->unkC = 1;
    } else {
        o->unkC = 0;
    }
}

u8 func_003F12F0(LoadIntf2Msg* o) {
    u8 r = 0;
    if (o->unkC == 1) {
        beMessenger_call(o->unk8, "SAVEUTILITY", 0, -1, "ISRDY");
        if (beMessenger_getBool(o->unk8, 0) == true) {
            o->unkD = 1;
            r = 1;
        }
    } else {
        o->unkD = 1;
        r = 1;
    }
    return r;
}

struct beMessenger;
void beMessenger_call(beMessenger* m, const char* group, const char* info, s32 ramNo, const char* message);

void func_003F1380(LoadIntf2Msg* o) {
    if (o->unkC == 1) {
        beMessenger_call(o->unk8, "SAVEUTILITY", 0, -1, "REMOVE");
    }
}

u8 func_003F13D0(LoadIntf2Msg* o) {
    u8 r = 0;
    if (o->unkC == 1) {
        beMessenger_call(o->unk8, "SAVEUTILITY", 0, -1, "ISRDY");
        if (beMessenger_getBool(o->unk8, 0) == false) {
            r = 1;
        }
    } else {
        r = 1;
    }
    return r;
}

struct LoadIntf2Init {
    char pad0[0xE];
    u8 unkE;
    char padF;
    u8 unk10;
    char pad11[3];
    s32 unk14;
    char pad18[4];
    s32 unk1C;
    s32 unk20;
    u8 unk24;
    char pad25[11];
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
};

void func_003F1460(LoadIntf2Init* o, s32 a, s32 b) {
    o->unk14 = b;
    o->unk1C = a;
    o->unk20 = 0;
    o->unk24 = 0;
    o->unkE = 1;
    o->unk10 = 0;
    o->unk30 = 0;
    o->unk3C = 0;
    o->unk38 = 0;
    o->unk34 = 0;
}

INCLUDE_ASM("asm/nonmatchings/beNDMWLoadIntf2", func_003F1490);
