#include "game.h"

/* beFont: text display (TEXTCTL) and beUnicodeObj (MWCC C++). */
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

extern "C" {

void beBaseInfoManager_end(beBaseInfoManager* mgr);
extern igMetaObject* beFont_Meta;
extern igMetaObject* beFontInfo_Meta;
extern igMetaObject* beFontImage_Meta;
extern igMetaObject* beUnicodeObj_Meta;
extern char D_0050E728[]; /* "TEXTCTL" */
void beBaseInfoManager_begin(beBaseInfoManager* mgr);
void beMessenger_entry(beMessenger* m, beBaseInfoRam* ram, const char* group, const char* info, s32 ramNo);
void func_002ED4C0(void* buffer, void* pool); /* free */
void igObject_userRelease(igObject* o);
extern char D_004E0858[];
void* memset(void* p, s32 c, u32 n);
u32 func_002ED4E0(void* buffer);
void* func_002ED2B0(u32 size, s32 align, void* pool);

/* -- */

void beFont_begin(beFont* mgr) {
    beBaseInfoManager_begin(mgr);
}

void beFont_end(beFont* mgr) {
    ObjListClear(mgr->_fonts);
    beBaseInfoManager_end(mgr);
}

void beFont_entry(beFont* mgr, beBaseInfoRam* ram) {
    beMessenger_entry(mgr->_messenger, ram, D_0050E728, NULL, -1);
}

void func_0030B420(igStringObj* s);

void beFont_prepare(beFont* mgr) {
    igUnref(mgr->_root);
    mgr->_root = NULL;
    beFontGeomAttrPairList* list = mgr->_gaPairList;
    if (*(volatile s32*)&list->_count > 0) {
        ObjListClear(list);
        if ((mgr->_gaPairList->_count > 0) == false) {
            func_002D6680(mgr->_gaPairList, 0, 4);
        }
    }
    func_0030B420(mgr->_convertData);
}

igMetaObject* beFont_getInfoMeta(void) {
    return beFontInfo_Meta;
}

void beFont_updateRam(beFont* mgr) {
}

INCLUDE_ASM("asm/nonmatchings/beFont", beFont_handle);

}
struct beFontRefHolder {
    igObject* p;
    ~beFontRefHolder() { igUnref(p); }
};
class igObjectV {
public:
    IG_VPAD_20
    virtual igMetaObject* getMeta(); /* +0x58 */
};
extern "C" {

beFontRefHolder* func_00382D70(beFontRefHolder* h, s16 flag) {
    if (h != NULL) {
        igUnref(h->p);
        if (flag > 0) {
            operator delete(h);
        }
    }
    return h;
}

static inline s32 DataCount(beFontInfo* info) {
    return info->_dataList->_count;
}

static inline beBaseInfoData* DataAt(beFontInfo* info, s32 i) {
    return ((beBaseInfoData**)(*(beBaseInfoDataList* volatile*)&info->_dataList)->_data)[i];
}

void func_00382DF0(beFont* mgr, beFontInfo* info) {
    s32 i;
    igMetaObject* meta;
    igObject* o;

    for (i = 0; i < DataCount(info); i++) {
        meta = beFontImage_Meta;
        o = DataAt(info, i);
        if (meta == ((igObjectV*)o)->getMeta()) {
            func_00301E30(mgr->_fonts, o);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/beFont", func_00382EB0);

}
struct beFontRefHolder2 {
    igObject* p;
    ~beFontRefHolder2() { igUnref(p); }
};
extern "C" {

beFontRefHolder2* func_003832D0(beFontRefHolder2* h, s16 flag) {
    if (h != NULL) {
        igUnref(h->p);
        if (flag > 0) {
            operator delete(h);
        }
    }
    return h;
}

s32 func_00383350(beFont* mgr, const char* name) {
    s32 i;
    const char* s;

    if (name == NULL) {
        return 0;
    }
    for (i = 0; i < mgr->_fonts->_count; i++) {
        s = ((igNamedObject**)mgr->_fonts->_data)[i]->_name;
        if (strcmp(name, s != NULL ? s : D_004E0858) == 0) {
            return i;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/beFont", func_00383420);

}
struct beFontCnt : igObject {
    s32 pad8;
    u32 size; /* 0xC */
};
struct beFontCntRef {
    beFontCnt* p;
    beFontCntRef(beFontCnt* o) : p(o) { igIncRef(p); }
    ~beFontCntRef() { igUnref(p); }
};
class beFontMsg {
public:
    IG_VPAD_37
    virtual void setup(s32 a, s32 b, s32 c);
    char pad4[8];
    beFontCnt* buf; /* 0xC */
};
class beFontVA {
public:
    IG_VPAD_22
    virtual void init(u32* flags, u32 size, s32 a, s32 b);
};
struct beFontVARef {
    beFontVA* p;
    beFontVARef(beFontVA* o) : p(o) { igIncRef((igObject*)p); }
    ~beFontVARef() { igUnref((igObject*)p); }
};
extern bool D_00554C14;
extern void* D_00587110;
static inline void* GetPool(void* o) {
    void* a = func_00301000(o);

    return D_00554C14 ? a : func_002F88E0(D_00587110);
}
extern "C" {
void* func_0014B940(void* pool);

void func_00383910(beFont* mgr, beFontMsg* m, s32 idx, u32 size) {
    void* pool;
    beFontCntRef r(m->buf);
    u32 flags;
    beFontCnt* n;

    pool = GetPool(mgr);
    igUnref(r.p);
    r.p = (beFontCnt*)func_0014B940(pool);
    flags = ((igBitmapFont**)mgr->_fonts->_data)[idx]->_vertexFlags;
    flags &= ~4;
    ((beFontVA*)r.p)->init(&flags, size, 0, 0);
    n = r.p;
    igIncRef(n);
    igUnref(m->buf);
    m->buf = n;
}

}
extern "C" {
s32 func_004553E0(void* a, s32 b);
s32 func_00455370(void* a, s32 b);

void func_00383AB0(beFont* mgr, beFontMsg* m, s32 arg2, s32 arg3) {
    s32 n;
    u32 size;

    if (arg3 != 0) {
        n = func_004553E0(((void**)mgr->_fonts->_data)[mgr->_fontIdx], arg2);
        size = (n + 2) * 3;
    } else {
        n = func_00455370(((void**)mgr->_fonts->_data)[mgr->_fontIdx], arg2);
        size = n * 3;
    }
    m->setup(3, n, 0);
    {
        beFontCntRef r(m->buf);
        if (r.p == NULL || r.p->size < size) {
            func_00383910(mgr, m, mgr->_fontIdx, size);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/beFont", func_00383BF0);

struct beFontU32Arr {
    s32 pad0[2];
    s32 count;
    s32 padC;
    u32* data;
};
struct beFontF32Arr {
    s32 pad0[4];
    f32* data;
};
struct beFontTxA {
    char pad0[0x30];
    beFontU32Arr* counts;
    beFontF32Arr* widths;
};
struct beFontTxB {
    char pad0[0x1C];
    f32 width;
    s32 pad20;
    s32 mode;
};
struct beFontTxC {
    s32 pad0[4];
    char* data;
    s32 stride;
};

#ifdef NON_MATCHING
/* Nonmatching: counts-pointer/end register is t5 instead of t3 (distance 16) */
#pragma opt_unroll_loops off
void func_00383ED0(beFontTxA* a, void* unused, beFontTxB* b, beFontTxC* c) {
    f32 w = b->width;
    s32 mode = b->mode;
    f32 off = 0.0f;
    f32 zero = 0.0f;
    char* base = c->data;
    f32* p;
    u32 v = 0;
    u32 cnt;
    u32 j;
    s32 i = 0;
    s32 o = 0;
    u32 end;

    for (; i < a->counts->count; i++, o += 4) {
        cnt = *(u32*)((char*)a->counts->data + o);
        switch (mode) {
        case 1:
            off = w - *(f32*)((char*)a->widths->data + o);
            break;
        case 2:
            off = (w - *(f32*)((char*)a->widths->data + o)) / 2.0f;
            break;
        }
        while (v < cnt) {
            end = v + 6;
            j = v;
            if (j < end) {
                for (; j < end; j++) {
                    p = (f32*)(base + c->stride * j);
                    p[0] = p[0] + off;
                    p[1] = p[1] + zero;
                    p[2] = p[2] + zero;
                }
            }
            v += 6;
        }
    }
}
#pragma opt_unroll_loops reset
#else
INCLUDE_ASM("asm/nonmatchings/beFont", func_00383ED0);
#endif

}
struct beFontQuad {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
};
class beFontTarget {
public:
    IG_VPAD_25
    virtual void apply(beFontQuad* q, s32 flag); /* +0x6C */
    s32 pad[2];
    s32 unkC;
};
extern "C" {

void func_00384010(void* unused, beFontTarget* t, beFontQuad* q0, beFontQuad* q1) {
    s32 v;

    q0->b = 0;
    q0->a = 0;
    q0->c = 0;
    q1->b = 3;
    q1->a = 0;
    q1->c = 0;
    v = t->unkC;
    q1->d = v;
    q0->d = v;
    t->apply(q0, 1);
    t->apply(q1, 1);
}

s32 func_003841D0(beFont* mgr, s32 arg);

void func_003840A0(beFont* mgr, s32 arg, const char* c0, const char* c1) {
    char* str = func_002EAF60(GetStrPool(), c0);
    char* str2;

    ReleaseStr(mgr->_condition00);
    mgr->_condition00 = str;
    str2 = func_002EAF60(GetStrPool(), c1);
    ReleaseStr(mgr->_condition01);
    mgr->_condition01 = str2;
    mgr->_history = 0;
    func_003841D0(mgr, arg);
}

struct beFontNm {
    s32 pad0[2];
    char* name;
    s32 pad0C;
    beFontNm* sub;
};
struct beFontNmList {
    s32 pad0[2];
    s32 count;
    s32 pad0C;
    beFontNm** data;
};
struct beFontNmMid {
    s32 pad0[5];
    beFontNmList* list;
};
struct beFontNmArg {
    s32 pad0[3];
    beFontNmMid* mid;
};

static inline bool NmEq(const char* s, const char* t) {
    return strcmp(s != NULL ? s : D_004E0858, t) == 0;
}

s32 func_003841D0(beFont* mgr, s32 arg) {
    s32 found = -1;
    s32 i;
    s32 j;
    beFontNmList* list;
    beFontNm* e;
    beFontNm* sub;
    const char* s;

    list = ((beFontNmArg*)arg)->mid->list;

    if (strcmp(mgr->_condition00, "ALL") == 0) {
        for (i = mgr->_history; i < list->count; i++) {
            s = list->data[i]->sub->name;
            if (NmEq(s, mgr->_condition01)) {
                found = i;
                break;
            }
        }
    } else {
        for (j = mgr->_history; j < list->count; j++) {
            e = list->data[j];
            sub = e->sub;
            s = e->name;
            if (NmEq(s, mgr->_condition00)) {
                s = sub->name;
                if (NmEq(s, mgr->_condition01)) {
                    found = j;
                    break;
                }
            }
        }
    }
    if (found != -1) {
        mgr->_history = found + 1;
    }
    return found;
}

void beUnicodeObj_userRelease(beUnicodeObj* o) {
    if (o->_buffer != NULL) {
        func_002ED4C0(o->_buffer, func_00301000(o));
    }
    igObject_userRelease(o);
}

void func_003843C0(beUnicodeObj* o, s32 n) {
    u32 size;

    if (o->_buffer != NULL) {
        size = (n + 1) * 2;
        if (func_002ED4E0(o->_buffer) < size) {
            func_002ED4C0(o->_buffer, func_00301000(o));
            o->_buffer = func_002ED2B0(size, 0x10, func_00301000(o));
        }
    } else {
        o->_buffer = func_002ED2B0((n + 1) * 2, 0x10, func_00301000(o));
    }
    memset(o->_buffer, 0, func_002ED4E0(o->_buffer));
}

igMetaObject* func_00384480(void) {
    return beUnicodeObj_Meta;
}

igMetaObject* beFont_getMeta(void) {
    return beFont_Meta;
}

}
