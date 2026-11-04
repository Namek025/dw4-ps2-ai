#include "game.h"

/* igResource: Alchemy's resource manager (finds, loads and tracks IGB directories), MWCC C++.
 * Range 0x3087A0-0x30A300. */
extern "C" {

/* The Alchemy core singleton (Gap::Core::igArkCore?). */
struct igArkCoreData {
    u8 unk0[0x40];
    igResource* unk40; /* the default resource manager */
};
extern igArkCoreData* D_00586DC8;
extern igMetaObject* D_00586670; /* igDirectory_Meta */
extern igMetaObject* igIGBFile_Meta;
extern char D_004FB390[]; /* "igImageDirectory" */
extern char D_004FB3B8[]; /* "igSceneInfo" */
extern char D_004FB400[]; /* "igSceneWrapper" */
extern char D_004F9A78[]; /* "%s/%s" */
s32 strcmp(const char* a, const char* b);
s32 strlen(const char* s);
igNamedObject* func_003022C0(igObjectList* list, igMetaObject* meta, const char* name); /* find by name */
igDirectory* func_00308A90(igResource* res); /* the image directory */
s32 func_002D91C0(igDirectory* dir); /* release a load; remaining count */
void func_00309C90(igDirectory* dir, const char* wrapper, const char* info);
void func_00309FE0(igDirectory* dir);
void func_00305000(igQueue* q);
void func_002FF150(igObject* o); /* igObject_userDeallocateFields */
igDirectory* func_00309640(igResource* res, igDirectory* dir);
void func_00308850(igResource* res, igObject* o);
void func_00309750(igResource* res, const char* name);
void func_00308E00(igResource* res);
igDirectory* func_002C72C0(igMemoryPool* pool); /* new igDirectory */
igQueue* func_002BC0D0(igMemoryPool* pool);     /* new igQueue */
igThread* func_00304F00(igQueue* q, void* entry, void* arg); /* new loader thread */
s32 func_00124E10(void* f); /* fflush */
struct ReentLite {
    s32 _errno;
    void* _stdin;
    void* _stdout;
};
extern ReentLite* _impure_ptr;
extern void* D_004C22E8;
extern void* D_00587020;
igResult func_00308C00(igDirectory* dir);
void func_002D9130(igDirectory* dir); /* add a load */
igIGBFile* func_002C37E0(igMemoryPool* pool); /* new igIGBFile */
void func_002D9330(igDirectory* dir); /* mark for asynchronous post-processing */
igStringObjList* func_002B9350(void* pool); /* new igStringObjList */
igIntList* func_002CAA70(void* pool);       /* new igIntList */
igStringObj* func_002B9500(void* pool);     /* new igStringObj */
void func_0030B3C0(igStringObj* str, const char* s); /* set */
igMetaObject* func_002FE460(const char* name); /* find a registered meta object */
u32 func_002D9120(igDirectory* dir);        /* entry count */
igObject* func_002D8370(igDirectory* dir, u32 i); /* object i */
extern char D_004F9A50[]; /* "root" */
extern char D_004FB3B0[]; /* "igNode" */
extern char D_004FB3C8[]; /* "_sceneGraph" */
extern char D_004FB3D8[]; /* "_cameras" */
extern char D_004FB3F0[]; /* "_sceneGraphList" */
extern igMetaObject* igObjectRefMetaField_Meta;
extern igMetaObject* igObjectList_Meta;
extern igMetaObject* igInfo_Meta;
extern igMetaObject* igObjectDirEntry_Meta;
extern igMetaObject* igObject_Meta;
igObject* func_002D8EB0(igDirectory* dir, const char* name); /* object by name */
igObject* func_002FDA10(igMetaObject* meta);       /* new instance */
igMetaField* func_002FDF20(igMetaObject* meta, const char* name); /* field by name */
void func_00303390(igMetaField* field, igObject* o, igObject* value); /* set an object field */
void func_00302240(igObjectList* list, igObject* o); /* append */
void func_002D8310(igDirectory* dir, igObject* info); /* add an info */
igObject* func_002D7E90(igObjectList* list, s32 i); /* element i */
igMetaField* func_002FDF00(igMetaObject* meta, s32 i); /* field i */
s32 func_002FE470(igMetaObject* meta); /* field count */
void igMetaField_dispatchShallowCopy(igMetaField* field, void* dst, igObject* src);

}

IG_REF(igDirectory);

/* Smart pointer whose destructor works on the member itself (reloaded after the decrement). */
template <class T>
struct igTmpRef {
    T* p;
    igTmpRef(T* o, igAdopt) : p(o) {}
    ~igTmpRef() {
        if (p != NULL) {
            p->_refCount--;
            if ((p->_refCount & 0x7FFFFF) == 0) {
                func_002FF820(p);
            }
        }
    }
};

template <class T, class R>
static inline void igAssignRef(T*& dst, const R& r) {
    T* o = r.p;

    igIncRef(o);
    igUnref(dst);
    dst = o;
}

static inline void SetName(igDirectory* o, const char* s) {
    char* n = func_002EAF60(GetStrPool(), s);

    ReleaseStr(o->_name);
    o->_name = n;
}

IG_REF(igIGBFile);
IG_REF(igMetaObject);
IG_REF(igStringObj);
IG_REF(igObject);
IG_DYNAMIC_CAST(igObjectRefMetaField)
IG_DYNAMIC_CAST(igObjectList)
IG_DYNAMIC_CAST(igObjectDirEntry)

/* isOfType seen with an int result here (no andi): a C++-linkage overload of the same
   function, demangled back to func_00300CD0 by mwsplice. */
s32 func_00300CD0(igObject* o, igMetaObject* meta);

static inline bool IsNotOfType(igObject* o, igMetaObject* meta) {
    return !(func_00300CD0(o, meta) != 0);
}

/* A reference passed by value (copied with a new reference). */
struct igObjectArg {
    igObject* p;
    igObjectArg(igObject* o) : p(o) { igIncRef(p); }
    igObjectArg(const igObjectArg& r) : p(r.p) { igIncRef(p); }
    ~igObjectArg() { igUnref(p); }
};
void func_002D7F10(igDirectory* dir, igObjectArg from, igObjectArg to); /* replace an object */

class igObjectMetaV {
public:
    IG_VPAD_20
    virtual igMetaObject* getMeta(); /* +0x58 */
};

/* Append to an igIntList; a macro so that the value is read after the resize. */
#define APPEND_INT(l, v)                                \
    do {                                                \
        s32 n_;                                         \
        igIntList* list_ = (l);                         \
                                                        \
        n_ = list_->_count;                             \
        if (n_ < list_->_capacity) {                    \
            (*(volatile s32*)&list_->_count)++;         \
        } else {                                        \
            func_002D6790(list_, n_ + 1, 4);            \
        }                                               \
        ((s32*)list_->_data)[n_] = (v);                 \
    } while (0)

/* Reference field assignment through the owning object. */
#define SET_REF_FIELD(obj, field, val)                  \
    do {                                                \
        igMemoryPool* v_ = (val);                       \
        igIGBFile* o_ = (obj);                          \
                                                        \
        igIncRef(v_);                                   \
        igUnref(o_->field);                             \
        o_->field = v_;                                 \
    } while (0)

static inline igMemoryPool* PoolOr(igMemoryPool* pool, igResource* res) {
    return pool != NULL ? pool : (igMemoryPool*)func_00301000(res);
}

class igQueueInitV {
public:
    IG_VPAD_21
    virtual void init(); /* +0x5C */
};

class igThreadV {
public:
    IG_VPAD_48
    virtual igResult start(s32 priority); /* +0xC8 */
};

class igSysV2 {
public:
    IG_VPAD_32
    virtual igResult enable(s32 on); /* +0x88 */
};

class igDirectoryRV {
public:
    IG_VPAD_27
    virtual s32 openFile(const char* path); /* +0x74 */
};

class igQueueV {
public:
    IG_VPAD_24
    virtual igResult push(void* item, s32 wait); /* +0x68 */
};

class igThreadManagerV {
public:
    IG_VPAD_22
    virtual igResult join(igThread* t); /* +0x60 */
};

class igSystemV {
public:
    IG_VPAD_21
    virtual igThreadManagerV* getThreadManager(); /* +0x5C */
};
extern igSystemV* D_005871D0;

extern "C" {

igDirectoryRef func_003087A0(igResource* res, const char* name) {
    if (strcmp(name, D_004FB390) == 0) {
        return func_00308A90(res);
    }
    return (igDirectory*)func_003022C0((igObjectList*)res->_directoryList, D_00586670, name);
}

#pragma exceptions off
void func_00308850(igResource* res, igObject* o) {
    func_00301E30((igObjectList*)res->_directoryList, o);
}
#pragma exceptions on

void func_00308860(igResource* res, const char* name, s32 align) {
    if (res->_alignmentStrings == NULL) {
        igAssignRef(res->_alignmentStrings, igTmpRef<igStringObjList>(func_002B9350(func_00301000(res)), IG_ADOPT));
        igAssignRef(res->_alignmentValues, igTmpRef<igIntList>(func_002CAA70(func_00301000(res)), IG_ADOPT));
    }
    igStringObjRef str(func_002B9500(func_00301000(res)), IG_ADOPT);

    func_0030B3C0(str, name);
    func_00301E30((igObjectList*)res->_alignmentStrings, str);
    APPEND_INT(res->_alignmentValues, align);
}

igDirectory* func_00308A90(igResource* res) {
    if (res->_imageDirectory == NULL) {
        {
            igTmpRef<igDirectory> dir(func_002C72C0((igMemoryPool*)func_00301000(res)), IG_ADOPT);

            igAssignRef(res->_imageDirectory, dir);
        }
        {
            char* n;
            igDirectory* dir = res->_imageDirectory;

            n = func_002EAF60(GetStrPool(), D_004FB390);
            ReleaseStr(dir->_name);
            dir->_name = n;
        }
        res->_imageDirectory->_managingResource = res;
        func_00308850(res, res->_imageDirectory);
    }
    func_00124E10(_impure_ptr->_stdout);
    return res->_imageDirectory;
}

igResult func_00308C00(igDirectory* dir) {
    igResource* res;

    if (dir == NULL) {
        return D_004FB418;
    }
    res = dir->_managingResource;
    if (dir->_loadedRefCount > 1) {
        func_00309640(res, dir);
        func_00309750(res, dir->_name);
    }
    return D_004FB410;
}

#ifdef NON_MATCHING
/* Nonmatching (2 instructions): the original loads D_004C22E8 (a2) before res->_loadQueue (a0)
   for the thread creation. */
void func_00308C80(igResource* res) {
    {
        igTmpRef<igQueue> q(func_002BC0D0((igMemoryPool*)func_00301000(res)), IG_ADOPT);

        igAssignRef(res->_loadQueue, q);
    }
    ((igQueueInitV*)res->_loadQueue)->init();
    igAssign(res->_loadThread, func_00304F00(res->_loadQueue, (void*)func_00308C00, D_004C22E8));
    ((igThreadV*)res->_loadThread)->start(0xFF);
    res->_asyncLoadState = true;
    ((igSysV2*)func_002F88E0(D_00587020))->enable(1);
}
#else
INCLUDE_ASM("asm/nonmatchings/alchemy/igResource", func_00308C80);
#endif

void func_00308E00(igResource* res) {
    if (res->_asyncLoadState) {
        igThreadManagerV* tm;
        igThread* thread;

        res->_asyncLoadState = false;
        ((igQueueV*)res->_loadQueue)->push(NULL, 1);
        thread = res->_loadThread;
        tm = D_005871D0->getThreadManager();
        tm->join(thread);
        func_00305000(res->_loadQueue);
        igUnref((igObject*)res->_loadQueue);
        res->_loadQueue = NULL;
        igUnref((igObject*)res->_loadThread);
        res->_loadThread = NULL;
    }
}

void func_00308F00(igResource* res) {
    if (res->_asyncLoadState == true) {
        func_00308E00(res);
    }
    igUnref((igObject*)res->_alignmentStrings);
    res->_alignmentStrings = NULL;
    igUnref((igObject*)res->_alignmentValues);
    res->_alignmentValues = NULL;
    func_002FF150(res);
}

#ifdef NON_MATCHING
/* Nonmatching (8 instructions): in the first pool assignment the original has the file in s2
   and the pool in s3; the other two match (func_003092F0 matches with the same shape). */
igDirectory* func_00308FB0(igResource* res, const char* name) {
    igIGBFile* f;
    igMemoryPool* pool;
    igDirectory* found = func_003087A0(res, name);

    if (found != NULL) {
        func_002D9130(found);
        return found;
    }
    {
        igIGBFileRef file(func_002C37E0(PoolOr(res->_temporaryMemoryPool, res)), IG_ADOPT);

        file->_sharingState = res->_IGBSharingState;
        file->_forceChunkSize = res->_IGBChunkSize;
        file->_useMemoryPoolAssignmentsState = res->_useMemoryPoolAssignmentsState;
        pool = PoolOr(res->_temporaryMemoryPool, res);
        f = file.p;
        igIncRef(pool);
        igUnref(f->_temporaryMemoryPool);
        f->_temporaryMemoryPool = pool;
        pool = PoolOr(res->_dataMemoryPool, res);
        f = file.p;
        igIncRef(pool);
        igUnref(f->_dataMemoryPool);
        f->_dataMemoryPool = pool;
        pool = PoolOr(res->_objectMemoryPool, res);
        f = file.p;
        igIncRef(pool);
        igUnref(f->_objectMemoryPool);
        f->_objectMemoryPool = pool;
        if (res->_asyncLoadState == true) {
            file->_autoReadFilePostProcessState = res->_postProcessAsyncronousFilesState;
        }
        file->_validState = false;
        file->_loadPendingState = true;
        SetName(file, name);
        file->_managingResource = res;
        func_002D9130(file);
        func_00308850(res, file);
        return func_00309640(res, file);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/alchemy/igResource", func_00308FB0);
#endif

igDirectory* func_003092F0(igResource* res, const char* name) {
    igMemoryPool* pool0;
    igIGBFile* f0;
    igMemoryPool* pool;
    igIGBFile* f;
    igDirectory* found = func_003087A0(res, name);

    if (found != NULL) {
        func_002D9130(found);
        return found;
    }
    {
        igIGBFileRef file(func_002C37E0(res->_temporaryMemoryPool), IG_ADOPT);

        file->_sharingState = res->_IGBSharingState;
        file->_forceChunkSize = res->_IGBChunkSize;
        file->_useMemoryPoolAssignmentsState = res->_useMemoryPoolAssignmentsState;
        pool0 = res->_temporaryMemoryPool;
        if (pool0 != NULL) {
            f0 = file.p;
            igIncRef(pool0);
            igUnref(f0->_temporaryMemoryPool);
            f0->_temporaryMemoryPool = pool0;
        }
        pool = res->_dataMemoryPool;
        if (pool != NULL) {
            f = file.p;
            igIncRef(pool);
            igUnref(f->_dataMemoryPool);
            f->_dataMemoryPool = pool;
        }
        pool = res->_objectMemoryPool;
        if (pool != NULL) {
            f = file.p;
            igIncRef(pool);
            igUnref(f->_objectMemoryPool);
            f->_objectMemoryPool = pool;
        }
        if (res->_asyncLoadState == true) {
            file->_autoReadFilePostProcessState = res->_postProcessAsyncronousFilesState;
            func_002D9330(file);
        }
        file->_validState = false;
        file->_loadPendingState = true;
        SetName(file, name);
        file->_managingResource = res;
        func_002D9130(file);
        func_00308850(res, file);
        if (res->_asyncLoadState == false) {
            return func_00309640(res, file);
        }
        func_002D9130(file);
        ((igQueueV*)res->_loadQueue)->push(file, 1);
        return file;
    }
}

igDirectory* func_00309640(igResource* res, igDirectory* dir) {
    char path[0x800];
    s32 ok;

    if (dir == NULL) {
        return NULL;
    }
    ok = false;
    if (res->_relativeFilePath != NULL && strlen(res->_relativeFilePath) != 0) {
        sprintf(path, D_004F9A78, res->_relativeFilePath, dir->_name);
        ok = ((igDirectoryRV*)dir)->openFile(path) > 0;
    }
    if (!ok) {
        if (res->_absoluteFilePath != NULL && strlen(res->_absoluteFilePath) != 0) {
            sprintf(path, D_004F9A78, res->_absoluteFilePath, dir->_name);
            ok = ((igDirectoryRV*)dir)->openFile(path) > 0;
        }
    }
    if (!ok) {
        ((igDirectoryRV*)dir)->openFile(dir->_name);
    }
    return dir;
}

bool func_00309740(void* unused, igDirectory* dir) {
    return !(dir->_loadPendingState != false);
}

void func_00309750(igResource* res, const char* name) {
    igDirectoryRef dir = func_003087A0(res, name);
    bool isImage;
    s32 n;

    if (dir == NULL) {
        return;
    }
    isImage = false;
    if (dir == res->_imageDirectory) {
        isImage = true;
    }
    if (dir->_managingResource == NULL) {
        dir.p->_managingResource = res;
    }
    n = func_002D91C0(dir);
    if (isImage && n <= 0) {
        igUnref((igObject*)res->_imageDirectory);
        res->_imageDirectory = NULL;
    }
}

void func_00309870(igResource* res) {
    igDirectoryList* list = res->_directoryList;
    s32 n = list->_count;
    s32 i;

    for (i = 0; i < n; i++) {
        igUnref(((igObject**)list->_data)[i]);
    }
    n = list->_count;
    for (i = 0; i < n; i++) {
        ((igObject**)list->_data)[i] = NULL;
    }
    list->_count = 0;
    igUnref((igObject*)res->_imageDirectory);
    res->_imageDirectory = NULL;
}

void func_00309970(igResource* res) {
    s32 i;

    for (i = res->_directoryList->_count - 1; i >= 0; i--) {
        if (func_00300CD0(((igObject**)res->_directoryList->_data)[i], igIGBFile_Meta)) {
            igDirectoryList* list = res->_directoryList;

            igUnref(((igObject**)list->_data)[i]);
            func_002D6CF0(list, i);
            ((igObject**)list->_data)[list->_count] = NULL;
        }
    }
}

igResource* func_00309A50(void) {
    return D_00586DC8->unk40;
}

igObject* func_00309A60(igResource* res, const char* dirName, const char* type, s32 index) {
    igDirectoryRef dir = func_003087A0(res, dirName);
    igMetaObjectRef meta(func_002FE460(type));
    s32 count;
    u32 i;

    if (dir == NULL || meta == NULL) {
        return NULL;
    }
    count = 0;
    for (i = 0; i < func_002D9120(dir); i++) {
        if (func_00300CD0(func_002D8370(dir, i), meta)) {
            if (count++ == index) {
                return func_002D8370(dir, i);
            }
        }
    }
    return NULL;
}

void func_00309C90(igDirectory* dir, const char* fromName, const char* toName) {
    if (dir != NULL) {
        igMetaObject* from = func_002FE460(fromName);
        igMetaObject* to = func_002FE460(toName);

        if (from != NULL) {
            s32 n = dir->_count;
            s32 i;

            for (i = 0; i < n; i++) {
                igObjectDirEntry* entry = igObjectDirEntry_cast(func_002D7E90(dir, i));

                if (entry != NULL && func_00300CD0(entry->_ref, from)) {
                    if (to == NULL) {
                        return;
                    }
                    {
                        igObjectArg old((igObject*)entry->_ref);
                        igObjectArg neu(func_002FDA10(to));
                        s32 count;
                        s32 k;

                        {
                            igObject* o = neu.p;

                            o->_refCount--;
                            if ((o->_refCount & 0x7FFFFF) == 0) {
                                func_002FF820(o);
                            }
                        }
                        count = func_002FE470(from);
                        for (k = func_002FE470(igObject_Meta); k < count; k++) {
                            igMetaField* f = func_002FDF00(from, k);
                            igMetaField* g = func_002FDF20(to, f->_fieldName);

                            if (g != NULL && !IsNotOfType(g, ((igObjectMetaV*)f)->getMeta())) {
                                igMetaField_dispatchShallowCopy(f, (char*)neu.p + g->_offset - f->_offset, old.p);
                            }
                        }
                        func_002D7F10(dir, old, neu);
                        if (func_00300CD0(neu.p, igInfo_Meta)) {
                            func_002D8310(dir, neu.p);
                        }
                    }
                }
            }
        }
    }
}

void func_00309FE0(igDirectory* dir) {
    if (dir != NULL) {
        igMetaObject* nodeMeta = func_002FE460(D_004FB3B0);

        if (nodeMeta != NULL) {
            igObject* root = func_002D8EB0(dir, D_004F9A50);

            if (root != NULL && func_00300CD0(root, nodeMeta)) {
                igMetaObject* infoMeta = func_002FE460(D_004FB3B8);

                if (infoMeta != NULL) {
                    igObjectRef info(func_002FDA10(infoMeta));
                    igMetaField* sceneGraph;
                    igMetaField* cameras;
                    igMetaField* sceneGraphList;
                    igObjectList* list;

                    {
                        igObject* o = info.p;

                        o->_refCount--;
                        if ((o->_refCount & 0x7FFFFF) == 0) {
                            func_002FF820(o);
                        }
                    }
                    sceneGraph = igObjectRefMetaField_cast(func_002FDF20(((igObjectMetaV*)info.p)->getMeta(), D_004FB3C8));
                    func_00303390(sceneGraph, info, root);
                    cameras = igObjectRefMetaField_cast(func_002FDF20(((igObjectMetaV*)info.p)->getMeta(), D_004FB3D8));
                    sceneGraphList = igObjectRefMetaField_cast(func_002FDF20(((igObjectMetaV*)info.p)->getMeta(), D_004FB3F0));
                    list = igObjectList_cast(*(igObject**)((char*)info.p + sceneGraphList->_offset));
                    if (list != NULL) {
                        func_00302240(list, root);
                    }
                    func_00303390(cameras, info, NULL);
                    func_002D8310(dir, info);
                }
            }
        }
    }
}

void func_0030A260(igResource* res, const char* name) {
    if (res->_autoCompatibility) {
        igDirectory* dir = func_003087A0(res, name);

        if (dir != NULL) {
            func_00309C90(dir, D_004FB400, D_004FB3B8);
            func_00309FE0(dir);
            dir->_compatibleState = true;
        }
    }
}

}
