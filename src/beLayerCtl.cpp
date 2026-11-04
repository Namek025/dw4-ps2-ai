#include "game.h"

/* beLayerCtl: layer switch control (MWCC C++). */
extern "C" {
void func_001DC9A0(igGroup* group, s32 index); /* remove child */

extern igMetaObject* beLayerCtl_Meta;
extern igMetaObject* beLayerInfo_Meta;
extern char D_0050EBC0[]; /* "LAYERCTL" */
extern void* D_00562640; /* traversal callback table */
void beMessenger_entry(beMessenger* m, beBaseInfoRam* ram, const char* group, const char* info, s32 ramNo);
void func_001E06C0(void* table, igMetaObject* meta, void (*fn)(void*, void*));
igMetaObject* func_0035CD40(void);
void func_00389BC0(void* a, void* b);
igObject* func_0035AA20(igMemoryPool* pool);
extern bool D_00554C14;
extern void* D_00587110;
extern char D_004E0858[];
extern char D_0050EC80[];
extern char D_0050EC88[];

/* -- */

static inline s32 ChildCount(igGroup* g) {
    if (g->_childList != NULL) {
        return g->_childList->_count;
    }
    return 0;
}

void beLayerCtl_prepare(beLayerCtl* mgr) {
    mgr->_isOrdered = false;
    if (mgr->_layerSwt != NULL) {
        while (ChildCount(mgr->_layerSwt) > 0) {
            func_001DC9A0(mgr->_layerSwt, 0);
        }
        igUnref(mgr->_layerSwt);
        mgr->_layerSwt = NULL;
    }
}

void beLayerCtl_entry(beLayerCtl* mgr, beBaseInfoRam* ram) {
    beMessenger_entry(mgr->_messenger, ram, D_0050EBC0, NULL, -1);
}

igMetaObject* beLayerCtl_getInfoMeta(void) {
    return beLayerInfo_Meta;
}

void beLayerCtl_updateRam(beLayerCtl* mgr) {
}

INCLUDE_ASM("asm/nonmatchings/beLayerCtl", beLayerCtl_handle);

igMetaObject* beLayerCtl_getMeta(void) {
    return beLayerCtl_Meta;
}

void func_00389B90(void* a, void* b) {
    func_00389BC0(b, a);
}

INCLUDE_ASM("asm/nonmatchings/beLayerCtl", func_00389BC0);

void func_00389FB0(void) {
    func_001E06C0(D_00562640, func_0035CD40(), func_00389B90);
}

#ifdef NON_MATCHING
/* Nonmatching: the two loads of unk64's data and size arguments are in the other order. */
/* The virtuals of beLuaState, as called from here (+0x60, +0x64). */
class beLuaStateV {
public:
    IG_VPAD_22
    virtual s32 unk60(char* path);                                 /* +0x60 */
    virtual s32 unk64(void* data, s32 size, const char* name);     /* +0x64 */
};

/* A pooled string owned by a local: released when it goes out of scope. */
class PooledStr {
public:
    char* _str;

    PooledStr() {
        _str = NULL;
    }
    ~PooledStr() {
        ReleaseStr(_str);
    }
    void Set(char* s) {
        ReleaseStr(_str);
        _str = s;
    }
};

s32 func_00389FF0(beLua* lua, const char* name, const char* dir) {
    PooledStr pooled;
    char buf1[256];
    char buf0[256];
    s32 result = -1;
    s32 n;
    s32 i;
    s32 j;
    beLuaInfo* info;
    s32 m;
    beLuaData* data;
    char* dname;

    if (dir == NULL) {
        sprintf(buf1, D_0050EC80, lua->_system->_stageName);
        pooled.Set(func_002EAF60(GetStrPool(), buf1));
    } else {
        pooled.Set(func_002EAF60(GetStrPool(), dir));
    }

    n = lua->_infoList->_count;
    for (i = 0; i < n; i++) {
        info = ((beLuaInfo**)lua->_infoList->_data)[i];
        m = info->_dataList->_count;
        for (j = 0; j < m; j++) {
            data = ((beLuaData**)info->_dataList->_data)[j];
            dname = data->_name;
            if ((dname != NULL ? dname : D_004E0858) != NULL &&
                strcmp(dname != NULL ? dname : D_004E0858, name) == 0) {
                result = ((beLuaStateV*)lua->_luaState)
                             ->unk64(data->_dataObject->_data,
                                     data->_dataObject->_dataSize, name);
                break;
            }
        }
    }

    if (result == -1) {
        sprintf(buf0, D_0050EC88, pooled._str, name);
        result = ((beLuaStateV*)lua->_luaState)->unk60(buf0);
    }
    return result;
}
#else
INCLUDE_ASM("asm/nonmatchings/beLayerCtl", func_00389FF0);
#endif

static inline void* GetPool(void* o) {
    void* a = func_00301000(o);
    return D_00554C14 ? a : func_002F88E0(D_00587110);
}

void func_0038A2D0(beLua* lua) {
    void* pool = GetPool(lua);
    igUnref(lua->_luaState);
    lua->_luaState = (beLuaState*)func_0035AA20((igMemoryPool*)pool);
}

}
