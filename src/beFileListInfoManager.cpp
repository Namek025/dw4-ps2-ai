#include "game.h"

/* beFileListInfoManager: loads the files a stage asks for (CFLsetName) and runs
   the stage's boot scripts, one step per frame (MWCC C++). */
extern "C" {

extern igMetaObject* beSystem_Meta;
extern igMetaObject* beMessenger_Meta;
extern igMetaObject* igViewerSceneInfoManager_Meta;
extern igMetaObject* beFileListInfo_Meta;
extern igMetaObject* igNamedObjectInfo_Meta;
extern igMetaObject* beFont_Meta;
extern igMetaObject* beFileListInfoManager_Meta;
extern igMetaObject* beLuaInfo_Meta;
extern igMetaObject* igSceneInfo_Meta;
extern char D_0050E558[]; /* "rb" */
extern char D_0050E6C8[]; /* "ROOTATTR" */
extern char D_0050E560[]; /* "notDefined" */
extern char D_0050E570[]; /* "ld" */
extern char D_0050E578[]; /* ".igb" */
extern char D_0050E580[]; /* "loadingUSA/" */
extern char D_0050E590[]; /* "BecText.igb" */
extern char D_0050E5A0[]; /* "_common/sbiallse.igb" */
extern char D_0050E5B8[]; /* "SOUND" */
extern char D_0050E5C0[]; /* "SEARCH" */
extern char D_0050E5C8[]; /* "MAKEBINLIST" */
extern char D_0050E5D8[]; /* "ISDONETRANS" */
extern char D_0050E5E8[]; /* "%s/lua" */
extern char D_0050E5F0[]; /* "func.lua" */
extern char D_0050E600[]; /* "boot.lua" */
extern char D_0050E610[]; /* "LAYERCTL" */
extern char D_0050E620[]; /* "ORDER" */
extern char D_0050E628[]; /* "LOADING" */
extern char D_0050E630[]; /* "SET" */
extern char D_0050E638[]; /* "SWITCH" */
extern char D_0050E640[]; /* "GETLAYERNODE" */
extern char D_0050E650[]; /* "TEXTCTL" */
extern char D_0050E658[]; /* "INITIALIZE" */
extern char D_0050E668[]; /* "start.lua" */
extern char D_0050E678[]; /* "first.lua" */
extern char D_0050E688[]; /* "stagIS08" */
extern char D_0050E698[]; /* "stagIS09" */
extern char D_0050E6A8[]; /* "stagIS10" */
extern char D_0050E6B8[]; /* "stagIS12" */
extern char D_004E0858[]; /* "" */
extern igResource* D_00586DD0; /* the resource manager */
extern bool D_00554C14;
extern void* D_00587108;
extern void* D_00587110;
igDirectory* func_003092F0(igResource* res, const char* file); /* start loading */
bool func_00309740(igResource* res, igDirectory* dir);         /* loaded */
void func_00309750(igResource* res, const char* name);         /* release */
void func_00236BE0(igInsightCore* insight, void* o);
void func_00236C40(igInsightCore* insight, void* o);
void func_00237030(igInsightCore* insight, igInfo* info); /* add an info */
void func_00382DF0(void* font, igInfo* info); /* beFont: take a font info */
extern igMetaObject* beLua_Meta;
extern igMetaObject* beLoadingNode_Meta;
extern char* D_004C22F0; /* igStringObj's empty buffer */
u32 strlen(const char* s);
bool beFileListInfoManager_readStep(beFileListInfoManager* mgr, igStringRefList* fnames);
s32 beFileListInfoManager_addReadInfos(beFileListInfoManager* mgr);
void func_00308C80(igResource* res);
void func_00308E00(igResource* res);
void func_0030B3C0(igStringObj* so, const char* s);                    /* set */
void func_0030BBF0(igStringObj* so, s32 at, s32 n);                    /* remove */
void func_0030B460(igStringObj* so, const char* s, s32 at, u32 n);     /* insert */
void func_00389FF0(beLua* lua, const char* name, const char* dir);     /* run a script */
void func_0038A2D0(beLua* lua);
bool func_0039AE00(beLoadingNode* node); /* played its minimum time */
void func_0039ADF0(beLoadingNode* node); /* jump to the end */
void func_001C5D10(igGroup* group, igNode* child); /* append a child */

}

/* igObject's virtuals. */
class igObjectV {
public:
    IG_VPAD_20
    virtual igMetaObject* getMeta(); /* +0x58 */
};

/* igFile's virtuals. */
class igFileV {
public:
    IG_VPAD_23
    virtual s32 open(const char* name, const char* mode); /* +0x64 */
    virtual void close();                                 /* +0x68 */
};

/* Memory pools (igMemoryPool) used for new objects. */
static inline igMemoryPool* ObjectPool(void* o) {
    void* a = func_00301000(o);

    return (igMemoryPool*)(D_00554C14 ? a : func_002F88E0(D_00587108));
}

static inline igMemoryPool* StagePool(void* o) {
    void* a = func_00301000(o);

    return (igMemoryPool*)(D_00554C14 ? a : func_002F88E0(D_00587110));
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

/* Checked cast that rereads the meta (the original does not reuse it from the
   type test just before). */
static inline igSceneInfo* igSceneInfo_cast(void* o) {
    igSceneInfo* r;

    if (o != NULL && func_00300CD0(o, *(igMetaObject* volatile*)&igSceneInfo_Meta)) {
        r = (igSceneInfo*)o;
    } else {
        r = NULL;
    }
    return r;
}

/* A pooled string held for a scope (igStringRef: lives on the stack). */
struct StrRef {
    char* p;
    StrRef(const char* s) : p(s == NULL ? NULL : func_002EAF60(GetStrPool(), s)) {}
    StrRef(const StrRef& r) : p(r.p) {
        if (p != NULL) {
            ((StrHeader*)p - 1)->refs++;
        }
    }
    ~StrRef() { ReleaseStr(p); }
};

/* Append a pooled string to a string list (by value: a copy of the ref). */
static inline void StrListAppend(igStringRefList* list, StrRef s) {
    func_001CA620(list, &s.p);
}

static inline void SetStr(char*& dst, const char* s) {
    char* str = func_002EAF60(GetStrPool(), s);

    ReleaseStr(dst);
    dst = str;
}

static inline void SetStageName(beSystem* o, const char* s) {
    char* str = func_002EAF60(GetStrPool(), s);

    ReleaseStr(o->_stageName);
    o->_stageName = str;
}

static inline const char* NameOf(igNamedObject* o) {
    return o->_name != NULL ? o->_name : D_004E0858;
}

static inline const char* Buffer(igStringObj* so) {
    const char* b = (const char*)so->_buffer;

    if (b == NULL) {
        b = D_004C22F0;
    }
    return b;
}

static inline const char* BufferV(igStringObj* so) {
    const char* b = *(const char* volatile*)&so->_buffer;

    if (b == NULL) {
        b = D_004C22F0;
    }
    return b;
}

IG_DYNAMIC_CAST(beLoadingNode)

static inline bool IsLoaded(igDirectory* dir) {
    return func_00309740(D_00586DD0, dir);
}

template <class L>
static inline void ObjListRemove(L* list, s32 i) {
    igUnref(((igObject**)list->_data)[i]);
    func_002D6CF0((igDataList*)list, i);
    ((igObject**)list->_data)[list->_count] = NULL;
}

struct FileHolder {
    u8 pad0[0x8];
    igFileV* file; /* 0x8 */
};

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

/* Same, with the count reread (the original does not reuse it from before). */
template <class L>
static inline void ObjListClearV(L* list) {
    s32 n = *(volatile s32*)&list->_count;
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

/* Free an empty list's storage. */
template <class L>
static inline void ListFreeIfEmpty(L* list) {
    if ((list->_count > 0) == false) {
        func_002D6680((igDataList*)list, 0, 4);
    }
}

static inline void ObjListInsertN(igDataList* list, s32 i, s32 n, igObject** objs) {
    s32 k;

    for (k = 0; k < n; k++) {
        igIncRef(objs[k]);
    }
    func_002D6B80(list, i, n, objs);
}

static inline void ObjListInsert(igDataList* list, s32 i, igObject* o) {
    ObjListInsertN(list, i, 1, &o);
}

extern "C" {

/* -- */

/* Whether a file can be opened. */
s32 func_0037F1B0(FileHolder* h, const char* name) {
    if (h->file->open(name, D_0050E558)) {
        h->file->close();
        return 1;
    }
    return 0;
}

void beFileListInfoManager_registerToInsight(beFileListInfoManager* mgr) {
    func_00236BE0(mgr->_insight, mgr);
}

void beFileListInfoManager_unRegisterFromInsight(beFileListInfoManager* mgr) {
    func_00236C40(mgr->_insight, mgr);
}

void beFileListInfoManager_begin(beFileListInfoManager* mgr) {
    mgr->_system = (beSystem*)func_00236F80(mgr->_insight, beSystem_Meta);
    mgr->_messenger = (beMessenger*)func_00236F80(mgr->_insight, beMessenger_Meta);
    mgr->_sceneInfoManager = (igViewerSceneInfoManager*)func_00236F80(mgr->_insight, igViewerSceneInfoManager_Meta);
}

void beFileListInfoManager_end(beFileListInfoManager* mgr) {
}

/* Take a file list (the newest goes first and restarts the loading) or a font. */
s32 beFileListInfoManager_appendInfo(beFileListInfoManager* mgr, igInfo* info) {
    if (beFileListInfo_Meta == ((igObjectV*)info)->getMeta()) {
        igInfoList* list = mgr->_infoList;

        if (list->_count > 0) {
            ObjListInsert((igDataList*)list, 0, info);
        } else {
            func_00301E30((igObjectList*)list, info);
        }
        mgr->_proc = 0;
        mgr->_system->_isSResetEnable = false;
        return 1;
    }
    if (igNamedObjectInfo_Meta == ((igObjectV*)info)->getMeta()) {
        void* font = func_00236F80(mgr->_insight, beFont_Meta);

        if (font == NULL) {
            return 0;
        }
        func_00382DF0(font, info);
        return 1;
    }
    return 0;
}

}

extern "C" {

/* Forget every file list and loaded file. */
void beFileListInfoManager_removeAllInfos(beFileListInfoManager* mgr) {
    ObjListClear(mgr->_infoList);
    ObjListClear(mgr->_dirList);
    ClearStrList(mgr->_fnames);
    ObjListClear(mgr->_importantInfoList);
    ObjListClear(mgr->_readInfoList);
    ListFreeIfEmpty(mgr->_infoList);
    ListFreeIfEmpty(mgr->_dirList);
    ListFreeIfEmpty(mgr->_importantInfoList);
    ListFreeIfEmpty(mgr->_readInfoList);
    mgr->_loaded = 0;
}

}

IG_REF(igStringObj);

extern "C" {

/* Every frame, one step of the stage loading (_proc):
   0     load the stage's loading screen (loadingUSA/ld<stage>.igb)
   1     show it; queue the one-time files (first stage only) and the names
         whose file part starts with 's'
   100   read those, then look for the stage's sounds (101: wait for them)
   102   queue the other names; 2: read them
   3     func.lua, boot.lua
   4     read the system's extra files; wait for the loading animation's
         minimum time, then end it
   20-21 two frames
   5     hand the remaining infos over; 6: layer order and scene graphs
   7     texts, start.lua; 8: system active; 9: first.lua, reset enable */
#ifdef NON_MATCHING
/* Nonmatching (~94.5% exact, 99% structure): mostly s-register assignment in
   states 1 and 102, the slash search loop (the original rereads so->_buffer
   and so->_length every iteration, with the empty buffer hoisted) and the
   bool shapes in state 4. Case order matters: MWCC tests cases in reverse
   source order and places the default block right after the tests. */
void beFileListInfoManager_update(beFileListInfoManager* mgr) {
    beSystem* sys = mgr->_system;

    if (sys->_execDvdErr > 0) {
        return;
    }
    switch (mgr->_proc) {
    default:
        mgr->_proc = -1;
        return;
    case -1:
        break;
    case 0: {
        if (mgr->_infoList->_count <= 0) {
            mgr->_proc = -1;
            return;
        }
        sys->_isActive = false;
        {
            beFileListInfo* info = ((beFileListInfo**)mgr->_infoList->_data)[0];

            if (NameOf(info) != NULL) {
                SetStageName(mgr->_system, NameOf(info));
            } else {
                SetStageName(mgr->_system, D_0050E560);
            }
        }
        func_00308C80(D_00586DD0);
        {
            igStringObjRef so(mgr->_system->_globalStrObj);

            func_0030B3C0(so, mgr->_system->_stageName);
            func_0030BBF0(so, 0, 2);
            func_0030B460(so, D_0050E570, 0, strlen(D_0050E570));
            {
                u32 len = so->_length;

                func_0030B460(so, D_0050E578, len, strlen(D_0050E578));
            }
            func_0030B460(so, D_0050E580, 0, strlen(D_0050E580));
            func_00301E30((igObjectList*)mgr->_dirList, func_003092F0(D_00586DD0, Buffer(so)));
        }
        mgr->_proc = 1;
        return;
    }
    case 1: {
        igDirectory* dir = ((igDirectory**)mgr->_dirList->_data)[0];

        if (func_00309740(D_00586DD0, dir) != false) {
            igInfoList* infos = dir->_infoList;
            igNamedObjectList* names;
            bool any;
            s32 i;

            for (i = 0; i < infos->_count; i++) {
                func_00237030(mgr->_insight, ((igInfo**)infos->_data)[i]);
            }
            func_00309750(D_00586DD0, dir->_name);
            ObjListClear(mgr->_dirList);
            names = ((beFileListInfo**)mgr->_infoList->_data)[0]->_fnames;
            if (!mgr->_isReadOnetime) {
                StrRef text(D_0050E590);

                StrListAppend(mgr->_fnames, text);
                {
                    StrRef se(D_0050E5A0);

                    StrListAppend(mgr->_fnames, se);
                    mgr->_isReadOnetime = true;
                    mgr->_onetimeCount = 2;
                }
            }
            {
                igStringObjRef so(mgr->_system->_globalStrObj);

                any = false;
                for (i = 0; i < names->_count; i++) {
                    StrRef name(NameOf(((igNamedObject**)names->_data)[i]));
                    u32 slash = (u32)-1;
                    u32 k;

                    func_0030B3C0(so, name.p);
                    for (k = 0; k < so->_length; k++) {
                        if (BufferV(so)[k] == '/') {
                            slash = k;
                            break;
                        }
                    }
                    if (slash == (u32)-1) {
                        slash = 0;
                    }
                    if (name.p[slash + 1] == 's') {
                        StrListAppend(mgr->_fnames, name);
                        any = true;
                        ObjListRemove(names, i);
                        i--;
                    }
                }
                if (any) {
                    mgr->_proc = 100;
                } else {
                    mgr->_proc = 102;
                }
            }
        }
        break;
    }
    case 100:
        if (beFileListInfoManager_readStep(mgr, mgr->_fnames)) {
            beFileListInfoManager_addReadInfos(mgr);
            ObjListClear(mgr->_readInfoList);
            mgr->_loaded = 0;
            ClearStrList(mgr->_fnames);
            beMessenger_callGroup(mgr->_messenger, D_0050E5B8, D_0050E5C0);
            if (beMessenger_getBool(mgr->_messenger, 0)) {
                mgr->_proc = 101;
                beMessenger_callGroup(mgr->_messenger, D_0050E5B8, D_0050E5C8);
                return;
            }
            mgr->_proc = 102;
            return;
        }
        break;
    case 101:
        beMessenger_callGroup(mgr->_messenger, D_0050E5B8, D_0050E5D8);
        if (beMessenger_getBool(mgr->_messenger, 0) == true) {
            mgr->_proc = 102;
            return;
        }
        break;
    case 102: {
        igNamedObjectList* names = ((beFileListInfo**)mgr->_infoList->_data)[0]->_fnames;
        s32 i;

        for (i = 0; i < names->_count; i++) {
            StrRef name(NameOf(((igNamedObject**)names->_data)[i]));

            StrListAppend(mgr->_fnames, name);
        }
        mgr->_proc = 2;
        return;
    }
    case 2:
        if (beFileListInfoManager_readStep(mgr, mgr->_fnames)) {
            mgr->_loaded = 0;
            mgr->_proc = 3;
            return;
        }
        break;
    case 3: {
        beLua* lua = (beLua*)func_00236F80(mgr->_insight, beLua_Meta);

        if (lua != NULL) {
            char dir[32];

            sprintf(dir, D_0050E5E8, mgr->_system->_stageName);
            func_0038A2D0(lua);
            func_00389FF0(lua, D_0050E5F0, dir);
            func_00389FF0(lua, D_0050E600, dir);
        }
        mgr->_proc = 4;
        return;
    }
    case 4: {
        igStringRefList* extra = sys->_readFileList;
        igNodeList* children = mgr->_sceneInfoManager->_parameters->_scene->_childList;
        beLoadingNode* node = NULL;
        bool shown;
        bool read;

        if ((children != NULL ? children->_count : 0) > 0) {
            node = beLoadingNode_cast(((igNode**)children->_data)[0]);
            if (node != NULL) {
                shown = func_0039AE00(node);
            } else {
                shown = true;
            }
        } else {
            shown = true;
        }
        read = beFileListInfoManager_readStep(mgr, extra);
        if (shown && read) {
            mgr->_loaded = 0;
            mgr->_proc = 20;
            if (node != NULL) {
                func_0039ADF0(node);
            }
            while (mgr->_importantInfoList->_count > 0) {
                func_00301E30((igObjectList*)mgr->_readInfoList, ((igInfo**)mgr->_importantInfoList->_data)[0]);
                ObjListRemove(mgr->_importantInfoList, 0);
            }
            func_00308E00(D_00586DD0);
            return;
        }
        break;
    }
    case 20:
        mgr->_proc = 21;
        return;
    case 21:
        mgr->_proc = 5;
        return;
    case 5:
        if (beFileListInfoManager_addReadInfos(mgr)) {
            ObjListClear(mgr->_readInfoList);
            mgr->_proc = 6;
            return;
        }
        break;
    case 6:
        beMessenger_callGroup(mgr->_messenger, D_0050E610, D_0050E620);
        if (beMessenger_getBoolCount(mgr->_messenger) > 0) {
            beMessenger_selectGroup(mgr->_messenger, D_0050E610);
            beMessenger_pushString(mgr->_messenger, D_0050E628);
            beMessenger_pushString(mgr->_messenger, D_0050E630);
            beMessenger_send(mgr->_messenger, D_0050E638);
            if (mgr->_infoList->_count >= 2) {
                s32 i;

                for (i = 1; i < mgr->_infoList->_count; i++) {
                    igInfo* info = ((igInfo**)mgr->_infoList->_data)[i];

                    if (igSceneInfo_Meta == ((igObjectV*)info)->getMeta()) {
                        igNode* graph = ((igSceneInfo*)info)->_sceneGraph;

                        beMessenger_selectGroup(mgr->_messenger, D_0050E610);
                        beMessenger_pushString(mgr->_messenger, NameOf(graph));
                        beMessenger_send(mgr->_messenger, D_0050E640);
                        if (beMessenger_getObjectCount(mgr->_messenger) > 0) {
                            func_001C5D10((igGroup*)beMessenger_getObject(mgr->_messenger, 0), graph);
                        }
                    }
                }
            }
        }
        mgr->_proc = 7;
        return;
    case 7: {
        beLua* lua;

        beMessenger_callGroup(mgr->_messenger, D_0050E650, D_0050E658);
        lua = (beLua*)func_00236F80(mgr->_insight, beLua_Meta);
        if (lua != NULL) {
            char dir[32];

            sprintf(dir, D_0050E5E8, mgr->_system->_stageName);
            func_00389FF0(lua, D_0050E668, dir);
        }
        mgr->_proc = 8;
        return;
    }
    case 8:
        sys->_isActive = true;
        mgr->_proc = 9;
        return;
    case 9: {
        beLua* lua = (beLua*)func_00236F80(mgr->_insight, beLua_Meta);

        if (lua != NULL) {
            char dir[32];

            sprintf(dir, D_0050E5E8, mgr->_system->_stageName);
            func_00389FF0(lua, D_0050E678, dir);
        }
        mgr->_proc = -1;
        {
            StrRef s08(D_0050E688);
            StrRef s09(D_0050E698);
            StrRef s10(D_0050E6A8);
            StrRef s12(D_0050E6B8);
            StrRef stage(mgr->_system->_stageName);

            if (stage.p != s08.p && stage.p != s09.p && stage.p != s10.p && stage.p != s12.p) {
                mgr->_system->_isSResetEnable = true;
            }
        }
        break;
    }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/beFileListInfoManager", beFileListInfoManager_update);
#endif

}

extern "C" {

/* One loading step: when the file being read is in, sort its infos (Lua runs
   now, scenes go to _infoList or, with cameras or a ROOTATTR scene graph, to
   _importantInfoList, the rest to _readInfoList); else start reading the next
   name. 1 when every name is done. */
bool beFileListInfoManager_readStep(beFileListInfoManager* mgr, igStringRefList* fnames) {
    igDirectory* dir;
    igInfoList* infos;
    s32 i;
    s32 at;
    s32 k;
    igInfo* info;
    igSceneInfo* scene;

    if (mgr->_loaded >= fnames->_count) {
        return 1;
    }
    if (mgr->_dirList->_count != 0) {
        dir = ((igDirectory**)mgr->_dirList->_data)[0];

        if (func_00309740(D_00586DD0, dir)) {
            StrRef root(D_0050E6C8);
            infos = dir->_infoList;

            for (i = 0; i < infos->_count; i++) {
                info = ((igInfo**)infos->_data)[i];

                if (beLuaInfo_Meta == ((igObjectV*)info)->getMeta()) {
                    func_00237030(mgr->_insight, info);
                } else if (igSceneInfo_Meta == ((igObjectV*)info)->getMeta()) {
                    scene = igSceneInfo_cast(info);
                    igGraphPathList* cams = scene->_cameras;

                    if (cams != NULL && cams->_count > 0) {
                        func_00301E30((igObjectList*)mgr->_importantInfoList, info);
                    } else {
                        char* name = scene->_sceneGraph->_name;

                        if ((name != NULL ? name : D_004E0858) == root.p) {
                            func_00301E30((igObjectList*)mgr->_importantInfoList, info);
                        } else {
                            func_00301E30((igObjectList*)mgr->_infoList, info);
                        }
                    }
                } else {
                    at = -1;

                    for (k = 0; k < mgr->_readInfoList->_count; k++) {
                        if (igSceneInfo_Meta == ((igObjectV*)info)->getMeta()) {
                            at = k;
                            break;
                        }
                    }
                    if (at == -1) {
                        func_00301E30((igObjectList*)mgr->_readInfoList, info);
                    } else {
                        ObjListInsert((igDataList*)mgr->_readInfoList, at, info);
                    }
                }
            }
            func_00309750(D_00586DD0, dir->_name);
            ObjListClear(mgr->_dirList);
            mgr->_loaded++;
        }
    } else {
        if (mgr->_onetimeCount > -1) {
            if (mgr->_onetimeCount == 2) {
                IG_SET(D_00586DD0, _objectMemoryPool, ObjectPool(mgr))
                IG_SET(D_00586DD0, _dataMemoryPool, ObjectPool(mgr))
            } else if (mgr->_onetimeCount == 0) {
                IG_SET(D_00586DD0, _objectMemoryPool, StagePool(mgr))
                IG_SET(D_00586DD0, _dataMemoryPool, StagePool(mgr))
            }
            mgr->_onetimeCount--;
        }
        {
            char* name;

            func_001C64E0(&name, fnames, mgr->_loaded);
            func_00301E30((igObjectList*)mgr->_dirList, func_003092F0(D_00586DD0, name));
            ReleaseStr(name);
        }
    }
    return 0;
}

}


extern "C" {

/* Hand the infos read from the loaded files to Alchemy, then drop them. */
s32 beFileListInfoManager_addReadInfos(beFileListInfoManager* mgr) {
    s32 i;

    for (i = 0; i < mgr->_readInfoList->_count; i++) {
        igInfo* info = ((igInfo**)mgr->_readInfoList->_data)[i];

        if (info != NULL) {
            func_00237030(mgr->_insight, info);
        }
    }
    ObjListClearV(mgr->_readInfoList);
    return 1;
}

}

extern "C" {

igMetaObject* beFileListInfoManager_getMeta(void) {
    return beFileListInfoManager_Meta;
}

}
