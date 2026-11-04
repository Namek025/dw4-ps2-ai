#include "game.h"

/* beSystem: frame timing and stage loading (MWCC C++). */
extern "C" {

extern igMetaObject* beSystem_Meta;
extern char D_0050FB10[]; /* "ndmw.igb" */
extern igResource* D_00586DD0; /* the resource manager */
extern s32 D_004C22E8;
extern bool D_00554C14;
extern void* D_00587060;
extern void* D_00587108;
extern void* D_00587110;
void func_00236BE0(igInsightCore* insight, void* o);
void func_00236C40(igInsightCore* insight, void* o);
void func_00237030(igInsightCore* insight, igInfo* info);
f32 func_0022FD70(s64 v); /* (f32)v */
void func_00308C80(igResource* res);
igDirectory* func_003092F0(igResource* res, const char* file); /* load */
bool func_00309740(igResource* res, igDirectory* dir);         /* loaded */
void func_00309750(igResource* res, const char* name);
void func_00308E00(igResource* res);

}

/* Memory pools (igMemoryPool) used for new objects. */
static inline igMemoryPool* ObjectPool(void* o) {
    void* a = func_00301000(o);

    return (igMemoryPool*)(D_00554C14 ? a : func_002F88E0(D_00587108));
}

static inline igMemoryPool* StagePool(void* o) {
    void* a = func_00301000(o);

    return (igMemoryPool*)(D_00554C14 ? a : func_002F88E0(D_00587110));
}

static inline igMemoryPool* TemporaryPool(void* o) {
    void* a = func_00301000(o);

    return (igMemoryPool*)(D_00554C14 ? a : func_002F88E0(D_00587060));
}

static inline void SetStr(char*& dst, const char* s) {
    char* str = func_002EAF60(GetStrPool(), s);

    ReleaseStr(dst);
    dst = str;
}

/* Smart pointer member assignment (incref new, unref old). */
#define IG_SET(obj, field, v)                                                  \
    {                                                                          \
        igMemoryPool* v_ = (v);                                                \
        igResource* o_ = (obj);                                                \
                                                                               \
        igIncRef(v_);                                                          \
        igUnref(o_->field);                                                    \
        o_->field = v_;                                                        \
    }

extern "C" {

/* -- */

void beSystem_registerToInsight(beSystem* sys) {
    func_00236BE0(sys->_insight, sys);
}

void beSystem_unRegisterFromInsight(beSystem* sys) {
    func_00236C40(sys->_insight, sys);
}

/* Boot: load the main archive next. */
void beSystem_begin(beSystem* sys) {
    sys->_isFirstTime = false;
    sys->_isActive = true;
    SetStr(sys->_nextStage, D_0050FB10);
    igAssign(sys->_bootDirectory, (igDirectory*)NULL);
    D_004C22E8 = 100000;
    D_00586DD0->_useMemoryPoolAssignmentsState = false;
    IG_SET(D_00586DD0, _objectMemoryPool, ObjectPool(sys))
    IG_SET(D_00586DD0, _dataMemoryPool, ObjectPool(sys))
    IG_SET(D_00586DD0, _temporaryMemoryPool, TemporaryPool(sys))
}

void beSystem_end(beSystem* sys) {
}

s32 beSystem_appendInfo(beSystem* sys) {
    return 0;
}

void beSystem_removeAllInfos(beSystem* sys) {
    sys->_isFirstTime = true;
}

/* Every frame: frame time, then the pending stage load. */
void beSystem_update(beSystem* sys) {
    s64 now = sys->_insight->_worldTime;

    sys->_frameTimeL = now - sys->_svWorldTimeL;
    sys->_frameTimeF = func_0022FD70(sys->_frameTimeL) / 16666667.0f;
    sys->_svWorldTimeL = now;
    if (sys->_nextStage != NULL || sys->_bootDirectory != NULL) {
        if (sys->_nextStage != NULL) {
            if (strcmp(sys->_nextStage, D_0050FB10) == 0) {
                IG_SET(D_00586DD0, _objectMemoryPool, ObjectPool(sys))
                IG_SET(D_00586DD0, _dataMemoryPool, ObjectPool(sys))
            } else {
                IG_SET(D_00586DD0, _objectMemoryPool, StagePool(sys))
                IG_SET(D_00586DD0, _dataMemoryPool, StagePool(sys))
            }
            func_00308C80(D_00586DD0);
            igAssign(sys->_bootDirectory, func_003092F0(D_00586DD0, sys->_nextStage));
            SetStr(sys->_nextStage, NULL);
        } else if (sys->_bootDirectory != NULL) {
            if (func_00309740(D_00586DD0, sys->_bootDirectory)) {
                igInfoList* infos = sys->_bootDirectory->_infoList;
                s32 i;

                for (i = 0; i < infos->_count; i++) {
                    func_00237030(sys->_insight, ((igInfo**)infos->_data)[i]);
                }
                func_00309750(D_00586DD0, sys->_bootDirectory->_name);
                igAssign(sys->_bootDirectory, (igDirectory*)NULL);
                func_00308E00(D_00586DD0);
            }
        } else {
            SetStr(sys->_stageName, NULL);
            igAssign(sys->_bootDirectory, (igDirectory*)NULL);
        }
    }
    if (sys->_isActive && sys->_isFirstTime) {
        sys->_isFirstTime = false;
    }
}

/* Running: active and past the first frame. */
bool beSystem_isRunning(beSystem* sys) {
    return sys->_isActive & !(sys->_isFirstTime != false);
}

igMetaObject* beSystem_getMeta(void) {
    return beSystem_Meta;
}

}
