#include "game.h"

/* igIGBFile: Alchemy's IGB reader (readFile and its phases), MWCC C++.
 * Range 0x2E5F70-0x2EA040. */
extern "C" {

void func_002D8F20(igIGBFile* file);
void func_002E8A10(igIGBFile* file);
void func_002E8A40(igIGBFile* file);

struct D_00586DC8Context {
    u8 unk0[0x38];
    void* unk38;
};

extern D_00586DC8Context* D_00586DC8;
extern char D_004F9AC8[4];
extern bool D_00554C04; /* generate unknown metaobjects from their parent */
s32 func_00306E50(void* arg0, s32 arg1, const void* arg2, void* arg3, s32 arg4, s32 arg5);
void func_002E3CB0(igIGBFile* file);
igObject* func_002D7E90(igObjectList* list, s32 i); /* element i */
void func_0030A260(igResource* res, char* name);
igResult func_002E9F30(igIGBFile* file, s32 size);
s32 strlen(const char* s);
igResult func_002E9560(igIGBFile* file);
void* memcpy(void* dst, const void* src, s32 n);
s32 func_002E3A10(igIGBFile* file, void* place); /* size of the object at place */
igResult func_002E7030(igIGBFile* file);
igPointerList* func_002C9130(igMemoryPool* pool); /* new igPointerList */
void func_002D6790(igDataList* list, s32 count, s32 elemSize); /* resize */
void igObject_userDeallocateFields(igObject* o);
extern igMetaField* D_00586280;
extern igMetaObject* igObjectDirEntry_Meta;
extern igMetaObject* igIGBFile_Meta;
struct igMetaFieldRegistry {
    u8 unk0[0x8];
    igObject** _data; /* 0x8 */
    s32 _count;       /* 0xC */
};
igMetaFieldRegistry* func_002FC250(void); /* registered metafield types */
igMetaFieldList* func_002C8AF0(igMemoryPool* pool); /* new igMetaFieldList */
void func_002D6680(igDataList* list, s32 count, s32 elemSize); /* resize */
s32 strcmp(const char* a, const char* b);
void func_00301EE0(igDirectory* dir, s32 count); /* set the entry count */
igObject* func_002D7D50(igDirectory* list, s32 index); /* entry by index */
igIntList* func_002CAA70(igMemoryPool* pool); /* new igIntList */
s32 func_002F89E0(const char* name); /* memory pool handle by name */
extern s32 D_004C2140; /* default pool handle */
extern igMetaObject* igDirEntry_Meta;
igMetaField* func_002C3090(void); /* the int metafield */
igMetaField* func_002BA2E0(void); /* the short metafield */
igDirectory* func_002C72C0(igMemoryPool* pool); /* new entry list */
igObjectList* func_002BD680(igMemoryPool* pool); /* new entry pool list */
extern bool D_00554C14; /* ignore the file's memory pool assignments */
extern igResource* D_00586DD0; /* the resource manager */
extern char* D_004C22F0;       /* igStringObj's empty buffer */
igResource* func_00309A50(void); /* the default resource manager */
igMetaObjectList* func_002C8E10(igMemoryPool* pool);   /* new igMetaObjectList */
igShortListList* func_002C9BD0(igMemoryPool* pool);    /* new igShortListList */
igShortList* func_002C9D80(igMemoryPool* pool);        /* new igShortList */
igMetaObject* func_002BE4F0(igMemoryPool* pool);       /* new igMetaObject */
igMetaField* func_002CBF70(igMemoryPool* pool);        /* new igMetaField */
igMetaField* func_002FDA40(igMetaObject* type, igMemoryPool* pool); /* new field of a type */
igMetaObject* func_002FE460(const char* name);         /* find a registered meta object */
s32 func_002FE470(igMetaObject* meta);                 /* field count */
s32 func_002FE730(igMetaObject* meta);                 /* first field slot of its own */
void func_002FDDB0(igMetaObject* meta, const char* name); /* set name */
void func_002FDC50(igMetaObject* meta, igMetaObject* parent); /* set parent */
void func_002FDE60(igMetaObject* meta, igMetaField* field, s32 alignMask); /* append field */
void func_002FC2C0(igMetaField* field, const char* name, s32 a, s32 b, igMetaObject* owner);
void func_002D3060(D_00586DC8Context* reg, igMetaObject* meta); /* register meta */
void func_002D3150(D_00586DC8Context* reg, igMetaObject* meta);
extern char D_004F9AA8[]; /* "fakeField" */
extern char D_004F9AB8[]; /* "Generated" */
igResult func_002E63E0(igIGBFile* file); /* read the metaobjects */
igResult func_002E7390(igIGBFile* file); /* read shared entries */
igResult func_002E79A0(igIGBFile* file); /* read entries */
s32 func_00308320(const char* fmt, ...); /* report a warning; 2 = don't show again */
s32 func_00308240(const char* fmt, ...); /* report info; 2 = don't show again */
extern char D_00586E20[];       /* "readFile(%s)" diagnostic buffer */
extern char D_004F9AD8[];       /* "readFile(%s)" */
extern char D_004F9AF0[];       /* old igb format with external references warning */
extern char D_004F9B90[];       /* "Total Time to load %s - %f seconds" */
extern bool D_00554C08;         /* old-format warning silenced */
extern bool D_00554C0C;         /* load time message silenced */
extern struct igTimerV* D_005871D8; /* system timer */
igDirectory* func_00308FB0(igResource* res, const char* name); /* load / find a directory */

/* IGB file header (12 words). */
struct igIGBHeader {
    s32 entryBufferSize;
    s32 entryCount;
    s32 metaObjectBufferSize;
    s32 metaObjectCount;
    s32 objectBufferSize;
    s32 objectCount;
    s32 memoryRefBufferSize;
    s32 memoryRefCount;
    s32 metaFieldBufferSize;
    s32 metaFieldCount;
    s32 magicCookie;
    u32 version; /* low 16 bits: version; high bits: flags */
};
igObject* func_002FDB40(igMetaObject* meta, igMemoryPool* pool); /* new instance */
void func_002FF3A0(igObject* o, s32 arg);
s32 func_002FFF90(igObject* entry, void* data, igIGBFile* file, igMetaObjectList* metas, bool swap);
s32 func_00300250(igObject* entry, void* data, igIGBFile* file, igMetaObjectList* metas, bool swap);
igMemoryPool* func_002F00D0(void); /* default memory pool */
igResult func_002E3A60(igIGBFile* file, char* name, s32 mode); /* open */
void func_002E3D30(igIGBFile* file);
void func_002E31B0(igFile* f, s32 mode);

}

/* Virtuals reached through placeholder classes (slot = (offset - 8) / 4). */
class igMemoryPoolV {
public:
    IG_VPAD_52
    virtual void* alloc(s32 size);                   /* +0xD8 */
    virtual void* allocAligned(s32 size, u16 align); /* +0xDC */
    virtual void _p54();
    virtual void _p55();
    virtual void _p56();
    virtual void _p57();
    virtual char* strdup(const char* s); /* +0xF0 */
    virtual void _p59();
    virtual void _p60();
    virtual void _p61();
    virtual void _p62();
    virtual void _p63();
    virtual void free(void* p);          /* +0x108 */
};

class igFileV {
public:
    IG_VPAD_25
    virtual s32 read(void* buf, s32 size, s32 count); /* +0x6C */
};

class igObjectReadV {
public:
    IG_VPAD_26
    virtual void releaseRef(void* ref);       /* +0x70 */
    virtual void _p27();
    virtual void _p28();
    virtual void readRef(igIGBFile* file);    /* +0x7C */
    virtual void readPass0(igIGBFile* file);  /* +0x80 */
    virtual void readPass1(igIGBFile* file);  /* +0x84 */
    virtual void readPass2(igIGBFile* file);  /* +0x88 */
    virtual void _p33();
    virtual void setConcrete(bool on);       /* +0x90 */
    virtual void readPostProcess(igIGBFile* file); /* +0x94 */
    virtual void _p36();
    virtual void _p37();
    virtual void _p38();
    virtual void readBuffers(igIGBFile* file); /* +0xA4 */
};

class igMetaFieldV {
public:
    IG_VPAD_50
    virtual void endianSwap(void* p, s32 count); /* +0xD0 */
};

#define POOL(p) ((igMemoryPoolV*)(p))
#define ENTRY(f, i) ((igObjectReadV*)func_002D7E90((f), (i)))

IG_REF(igObject);
IG_REF(igIGBFile);
IG_DYNAMIC_CAST(igIGBFile)
IG_REF(igDirEntry);
IG_REF(igStringObjList);
IG_REF(igIntList);
IG_REF(igShortList);
IG_DYNAMIC_CAST(igDirEntry)

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

/* Assign a reference field from a smart pointer (taken by reference, like a smart pointer
   member's operator=). */
template <class T, class R>
static inline void igAssignRef(T*& dst, const R& r) {
    T* o = r.p;

    igIncRef(o);
    igUnref(dst);
    dst = o;
}

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

/* list[i] = v, bounds-checked, no references. */
template <class L>
static inline void SetAtRaw(L* list, s32 i, void* v) {
    if (list->_count != 0 && i >= 0 && i < list->_count) {
        ((void**)list->_data)[i] = v;
    }
}

/* Pooled string field assignment. */
static inline void SetPoolStr(char*& dst, const char* s) {
    char* n = func_002EAF60(GetStrPool(), s);

    ReleaseStr(dst);
    dst = n;
}

static inline void SetRelativePath(igResource* res, const char* s) {
    char* n = func_002EAF60(GetStrPool(), s);

    ReleaseStr(res->_relativeFilePath);
    res->_relativeFilePath = n;
}

static inline igPointerList* GetRefList(igIGBFile* file) {
    return file->_refList;
}

/* igStringObj's text, or the shared empty buffer. */
static inline char* GetStr(igStringObj* o) {
    if (o->_buffer == NULL) {
        return D_004C22F0;
    }
    return (char*)o->_buffer;
}

/* list[i] = o, with references and a bounds check. */
template <class L, class T>
static inline void SetAt(L* list, s32 i, T* o) {
    igIncRef(o);
    igUnref(((igObject**)list->_data)[i]);
    if (list->_count != 0 && i >= 0 && i < list->_count) {
        ((igObject**)list->_data)[i] = o;
    }
}

/* Append to an igShortList. An inline (not a macro) so that n gets its stack slot after the
   caller's locals; arguments are evaluated right to left, so the value is computed first. */
static inline void AppendShort(igShortList* list, s16 v) {
    s32 n = list->_count;

    if (n < list->_capacity) {
        (*(volatile s32*)&list->_count)++;
    } else {
        func_002D6790(list, n + 1, 2);
    }
    ((s16*)list->_data)[n] = v;
}

class igMetaFieldAlignV {
public:
    IG_VPAD_25
    virtual s32 getRequiredAlignment(); /* +0x6C */
};

/* Place a generated field at the end of meta, aligned. */
static inline void PlaceField(igMetaObject* meta, igMetaField* f) {
    s32 mask = ((igMetaFieldAlignV*)f)->getRequiredAlignment() - 1;
    s32 align = meta->_requiredAlignment;

    if (align < mask + 1) {
        align = mask + 1;
    }
    meta->_requiredAlignment = align;
    f->_offset = ~mask & (mask + meta->_sizeofSize);
    func_002FDE60(meta, f, mask);
}

class igObjectMetaV {
public:
    IG_VPAD_20
    virtual igMetaObject* getMeta(); /* +0x58 */
};

class igDirEntryV {
public:
    IG_VPAD_21
    virtual void setResolved(s32 on);              /* +0x5C */
    virtual void _p22();
    virtual void _p23();
    virtual igObject* makeShared(igIGBFile* file); /* +0x68 */
    virtual void setRef(void* ref);                /* +0x6C */
    virtual void _p26();
    virtual void setMemoryPool(s32 handle);        /* +0x74 */
    virtual s32 getMemoryPoolIndex();              /* +0x78 */
    virtual void readFields(igIGBFile* file);      /* +0x7C */
};

class igDirectoryV {
public:
    IG_VPAD_28
    virtual igResult resolveExternal();     /* +0x78 */
    virtual igResult resolve();             /* +0x7C */
    virtual void _p30();
    virtual igResult readFilePostProcess(); /* +0x84 */
};

struct igTimerV {
    IG_VPAD_23
    virtual double getTime(); /* +0x64 */
};

/* isOfType seen with an int result here (no andi): a C++-linkage overload of the same
   function, demangled back to func_00300CD0 by mwsplice. */
s32 func_00300CD0(igObject* o, igMetaObject* meta);

static inline bool IsNotEntry(igObject* o) {
    return !(func_00300CD0(o, igObjectDirEntry_Meta) != 0);
}

extern "C" {

igResult func_002E5F70(igIGBFile* file, char* name) {
    if (file->_file == NULL && name == NULL) {
        return D_004FB418;
    }
    if (file->_file == NULL) {
        if (func_002E3A60(file, name, 4) == D_004FB418) {
            if (name != NULL) {
                igUnref((igObject*)file->_file);
                file->_file = NULL;
            }
            return D_004FB418;
        }
        file->_autoOpened = 1;
    }
    if (file->_file == NULL) {
        return D_004FB418;
    }
    func_002E3D30(file);
    func_002E31B0(file->_file, 4);
    return D_004FB410;
}

igResult func_002E6090(igIGBFile* file) {
    igIGBHeader* h;

    if (func_002E9F30(file, 0xC0) == D_004FB418) {
        return D_004FB418;
    }
    h = (igIGBHeader*)file->_chunk;
    ((igFileV*)file->_file)->read(h, 4, 12);
    igAssign(file->imf, func_002C3090());
    igAssign(file->smf, func_002BA2E0());
    if (h->magicCookie != file->_magicCookie) {
        ((igMetaFieldV*)file->imf)->endianSwap(h, 12);
        if (h->magicCookie == file->_magicCookie) {
            file->_endianSwap = true;
        }
    } else {
        file->_endianSwap = false;
    }
    if (h->magicCookie != file->_magicCookie) {
        return D_004FB418;
    }
    file->_magicVersion = (u16)h->version;
    if (file->_magicVersion > 7) {
        return D_004FB418;
    }
    file->_hasInfo = (h->version & 0x80000000) != 0;
    file->_hasExternal = (s32)(h->version & 0x40000000) > 0;
    file->_sharedEntries = (s32)(h->version & 0x20000000) > 0;
    file->_hasVersion1Cruft = h->version == 1;
    file->_hasMemoryPoolNames = (s32)(h->version & 0x10000000) > 0;
    if (file->_hasVersion1Cruft) {
        file->_version1CruftMultiply = 1;
    } else {
        file->_version1CruftMultiply = 0;
    }
    file->_entryBufferSize = h->entryBufferSize;
    file->_entryCount = h->entryCount;
    file->_metaObjectBufferSize = h->metaObjectBufferSize;
    file->_metaObjectCount = h->metaObjectCount;
    file->_objectBufferSize = h->objectBufferSize;
    file->_objectCount = h->objectCount;
    file->_memoryRefBufferSize = h->memoryRefBufferSize;
    file->_memoryRefCount = h->memoryRefCount;
    file->_metaFieldBufferSize = h->metaFieldBufferSize;
    file->_metaFieldCount = h->metaFieldCount;
    return D_004FB410;
}

igResult func_002E6330(igIGBFile* file) {
    bool ok;

    if (func_002E9F30(file, file->_metaObjectBufferSize) == D_004FB418) {
        return D_004FB418;
    }
    file->_metaObjectBuffer = file->_chunk;
    ok = true;
    if (file->_metaObjectBufferSize != 0) {
        ok = ((igFileV*)file->_file)->read(file->_metaObjectBuffer, file->_metaObjectBufferSize, 1) == 1;
    }
    igResult ret = D_004FB410;

    if (!ok) {
        ret = D_004FB418;
    }
    return ret;
}

/* Converts the file's metaobject table: each class is looked up by name and its
   version checked; unknown classes (version >= 4) are generated from the parent,
   with "fakeField" padding and "Generated" fields; per-object slot maps are built
   in _metaFieldPerObjectIndices.
   Matching notes: every return has its own copy of the baseSlots cleanup (no shared
   exit); the list refs are temporaries (their stack slots sit below the named refs);
   spilled locals get stack slots in declaration order, so i/descShorts/hasFields/recs
   come first and the *2/*4 strides are left to the compiler (desc += descShorts);
   the slot-map block reuses j and desc; _sizeofSize is an unsigned sum; the parent
   slot is read through a char* offset (data + 2*idx operand order). */
igResult func_002E63E0(igIGBFile* file) {
    s32 recWords;
    s32 i;
    s32 descShorts;
    bool hasFields;
    s32* recs;
    s32* rec;
    char* name;

    igAssignRef(file->_metaObjectListInFile,
                igTmpRef<igMetaObjectList>(func_002C8E10(file->_temporaryMemoryPool), IG_ADOPT));
    if (file->_metaObjectCount >= file->_metaObjectListInFile->_count) {
        func_002D6680(file->_metaObjectListInFile, file->_metaObjectCount, 4);
    }
    igShortListRef baseSlots;
    if (file->_magicVersion < 4) {
        recWords = 3;
        descShorts = 0;
        hasFields = false;
    } else {
        descShorts = 3;
        hasFields = true;
        recWords = 6;
        igAssignRef(file->_metaFieldPerObjectIndices,
                    igTmpRef<igShortListList>(func_002C9BD0(file->_temporaryMemoryPool), IG_ADOPT));
        if (file->_metaObjectCount >= file->_metaFieldPerObjectIndices->_count) {
            func_002D6680(file->_metaFieldPerObjectIndices, file->_metaObjectCount, 4);
        }
        {
            igMemoryPool* pool = file->_temporaryMemoryPool;

            igUnref(baseSlots.p);
            baseSlots.p = func_002C9D80(pool);
        }
        if (file->_metaObjectCount >= baseSlots->_count) {
            func_002D6680(baseSlots.p, file->_metaObjectCount, 2);
        }
    }
    if (file->_endianSwap) {
        ((igMetaFieldV*)file->imf)->endianSwap(file->_metaObjectBuffer, recWords * file->_metaObjectCount);
    }
    recs = (s32*)file->_metaObjectBuffer;
    i = 0;
    rec = recs;
    name = (char*)rec + recWords * file->_metaObjectCount * 4;
    for (; i < file->_metaObjectCount; i++) {
        igMetaObject* meta = func_002FE460(name);

        if (meta != NULL) {
            s32 major = rec[1];

            if (major == 0) {
                major = 1;
            }
            if (major == meta->_majorVersion && rec[2] == meta->_minorVersion) {
                func_00301E30((igObjectList*)file->_metaObjectListInFile, meta);
                if (hasFields) {
                    igShortListRef slots(func_002C9D80(file->_temporaryMemoryPool), IG_ADOPT);
                    s16* desc;
                    s32 j;

                    if (rec[3] >= slots->_count) {
                        func_002D6680(slots.p, rec[3], 2);
                    }
                    func_00301E30((igObjectList*)file->_metaFieldPerObjectIndices, slots.p);
                    desc = (s16*)(name + rec[0]);
                    j = 0;
                    if (file->_endianSwap) {
                        ((igMetaFieldV*)func_002BA2E0())->endianSwap(desc, descShorts * rec[3]);
                    }
                    for (; j < rec[3]; j++) {
                        s32 base = rec[5];
                        s32 k = i;
                        s16 slot = desc[1];
                        s32* r = rec;

                        while (slot < base) {
                            k = r[4];
                            r = recs + k * recWords;
                            base = r[5];
                        }
                        slot += ((s16*)baseSlots->_data)[k];
                        AppendShort(slots.p, slot);
                        desc += descShorts;
                    }
                    AppendShort(baseSlots.p, func_002FE470(meta) - rec[5]);
                }
            } else {
                return D_004FB418;
            }
        } else if (hasFields && D_00554C04) {
            s32 parentIdx = rec[4];
            s32* parentRec = recs + parentIdx * recWords;
            igMetaObject* parent = ((igMetaObject**)file->_metaObjectListInFile->_data)[parentIdx];
            s32 shift;
            s32 slot;
            s16* desc;
            igMetaObject* gen;
            s32 j;

            if (parent->_abstractState) {
                return D_004FB418;
            }
            slot = func_002FE730(parent);
            shift = func_002FE470(parent) - slot;
            gen = func_002BE4F0(func_002F00D0());
            func_002FDDB0(gen, name);
            func_002FDC50(gen, parent);
            func_00301E30((igObjectList*)file->_metaObjectListInFile, gen);
            desc = (s16*)(name + rec[0]);
            if (file->_endianSwap) {
                ((igMetaFieldV*)func_002BA2E0())->endianSwap(desc, descShorts * rec[3]);
            }
            for (j = 0; j < rec[3]; j++) {
                if (desc[1] < parentRec[5]) {
                    desc += descShorts;
                } else {
                    s32 target = desc[1] + *(s16*)((char*)baseSlots->_data + parentIdx * 2) - shift;
                    igMetaField* f;

                    while (slot < target) {
                        f = func_002CBF70(file->_temporaryMemoryPool);
                        f->_persistent = false;
                        func_002FC2C0(f, D_004F9AA8, 0, -1, gen);
                        f->_internalIndex = slot;
                        f->_size = 1;
                        PlaceField(gen, f);
                        slot++;
                        gen->_sizeofSize = (u32)f->_offset + f->_size;
                    }
                    f = func_002FDA40(((igObjectMetaV*)((igObject**)file->_metaFieldListInFile->_data)[desc[0]])->getMeta(),
                                      func_002F00D0());
                    func_002FC2C0(f, D_004F9AB8, 0, -1, gen);
                    f->_internalIndex = target;
                    f->_size = desc[2];
                    PlaceField(gen, f);
                    slot++;
                    desc += descShorts;
                    gen->_sizeofSize = (u32)f->_offset + f->_size;
                }
            }
            {
                u16 a = gen->_requiredAlignment;

                gen->_sizeofSize = ~(a - 1) & (gen->_sizeofSize + a - 1);
            }
            gen->_validity |= 4;
            func_002D3060(D_00586DC8, gen);
            func_002D3150(D_00586DC8, gen);
            desc = (s16*)(name + rec[0]);
            {
                igShortListRef slots(func_002C9D80(file->_temporaryMemoryPool), IG_ADOPT);

                func_00301E30((igObjectList*)file->_metaFieldPerObjectIndices, slots.p);
                for (j = 0; j < rec[3]; j++) {
                    s32 base = rec[5];
                    s32 k = i;
                    s16 fslot = desc[1];
                    s32* r = rec;

                    while (fslot < base) {
                        k = r[4];
                        r = recs + k * recWords;
                        base = r[5];
                    }
                    fslot += ((s16*)baseSlots->_data)[k];
                    AppendShort(slots.p, fslot);
                    desc += descShorts;
                }
                AppendShort(baseSlots.p, func_002FE470(gen) - rec[5]);
            }
        } else {
            if (hasFields) {
                return D_004FB418;
            }
            return D_004FB418;
        }
        name += rec[0];
        if (hasFields) {
            name += descShorts * (rec[3] * 2);
        }
        rec += recWords;
    }
    file->_metaObjectBuffer = NULL;
    return D_004FB410;
}

igResult func_002E6F50(igIGBFile* file) {
    s32 i;

    file->_basePath = POOL(file->_temporaryMemoryPool)->strdup(file->_file->_fileName);
    i = strlen((char*)file->_basePath);
    while (--i >= 0) {
        if (((char*)file->_basePath)[i] == '/' || ((char*)file->_basePath)[i] == '\\') {
            ((char*)file->_basePath)[i] = 0;
            break;
        }
    }
    if (i < 0) {
        *(char*)file->_basePath = 0;
    }
    return D_004FB410;
}

#pragma exceptions off
s32 func_002E7000(void) {
    return func_00306E50(D_00586DC8->unk38, 2, D_004F9AC8, &D_00554C04, 0, 0);
}
#pragma exceptions on

igResult func_002E7030(igIGBFile* file) {
    s32 remain;
    s32 size;

    remain = (file->_chunkSize - file->_chunkPlace) % file->_chunkSize;
    if (remain > 0) {
        memcpy(file->_chunk, (char*)file->_chunk + file->_chunkPlace, remain);
    }
    size = file->_chunkSize - remain;
    if (file->_bufferSize < file->_bufferPlace + size + remain) {
        size = file->_bufferSize - file->_bufferPlace - remain;
    }
    if (((igFileV*)file->_file)->read((char*)file->_chunk + remain, size, 1) != 1) {
        return D_004FB418;
    } else {
        file->_chunkPlace = 0;
        return D_004FB410;
    }
}

igDirEntryRef func_002E7100(igIGBFile* file) {
    char* place = (char*)file->_chunk + file->_chunkPlace;

    if (file->_chunkPlace > file->_chunkSize - 9
        || file->_chunkPlace + func_002E3A10(file, place) + file->_version1CruftMultiply * 4 > file->_chunkSize) {
        if (func_002E7030(file) == D_004FB418) {
            return (igDirEntry*)NULL;
        }
    }
    {
        s32* data = (s32*)((char*)file->_chunk + file->_chunkPlace);
        s32 metaIndex = *data;
        s32 size;

        if (file->_endianSwap) {
            ((igMetaFieldV*)func_002C3090())->endianSwap(&metaIndex, 1);
        }
        igDirEntryRef entry(igDirEntry_cast(func_002FDB40(
            ((igMetaObject**)file->_metaObjectListInFile->_data)[metaIndex], file->_temporaryMemoryPool)));

        {
            igDirEntry* o = entry.p;

            o->_refCount--;
            if ((o->_refCount & 0x7FFFFF) == 0) {
                func_002FF820(o);
            }
        }
        func_002FF3A0(entry.p, 0);
        if (file->_magicVersion < 4) {
            size = func_002FFF90(entry.p, data, file, file->_metaObjectListInFile, file->_endianSwap);
        } else {
            size = func_00300250(entry.p, data, file, file->_metaObjectListInFile, file->_endianSwap);
        }
        file->_chunkPlace += size;
        file->_bufferPlace += size;
        file->_chunkPlace += file->_version1CruftMultiply * 4;
        file->_bufferPlace += file->_version1CruftMultiply * 4;
        return entry.p;
    }
}

/* The entry goes through a local before SetAt: passing entry.p straight in swaps the
   s-registers of the entry and the list. */
igResult func_002E7390(igIGBFile* file) {
    s32 i;

    file->_chunkPlace = file->_chunkSize;
    file->_bufferPlace = 0;
    file->_bufferSize = file->_entryBufferSize;
    {
        igMemoryPool* pool = file->_temporaryMemoryPool;

        igUnref((igObject*)file->_uniqueEntryList);
        file->_uniqueEntryList = func_002C72C0(pool);
    }
    func_00301EE0(file->_uniqueEntryList, file->_entryCount);
    if (func_002E7030(file) == D_004FB418) {
        return D_004FB418;
    }
    for (i = 0; i < file->_entryCount; i++) {
        igDirEntryRef entry = func_002E7100(file);

        if (entry == NULL) {
            return D_004FB418;
        }
        if (file->_useMemoryPoolAssignmentsState && file->_memoryPoolHandles != NULL && !D_00554C14) {
            s32 k = ((igDirEntryV*)entry.p)->getMemoryPoolIndex();

            if (k > 0) {
                ((igDirEntryV*)entry.p)->setMemoryPool(((s32*)file->_memoryPoolHandles->_data)[k]);
            }
        } else {
            ((igDirEntryV*)entry.p)->setMemoryPool(-1);
        }
        {
            igObject* o = entry.p;

            SetAt(file->_uniqueEntryList, i, o);
        }
    }
    if (file->_hasCriticalFailure) {
        return D_004FB418;
    }
    return D_004FB410;
}

igResult func_002E7640(igIGBFile* file) {
    s32 count;
    s32 pos;
    s32 size;
    s32 n;
    u32 base;
    s32 k;
    igObject* entry;
    s32 idx;

    count = 0;
    ((igFileV*)file->_file)->read(&file->_indexBufferSize, 4, 1);
    ((igFileV*)file->_file)->read(&count, 4, 1);
    if (file->_endianSwap) {
        ((igMetaFieldV*)file->imf)->endianSwap(&file->_indexBufferSize, 1);
        ((igMetaFieldV*)file->imf)->endianSwap(&count, 1);
    }
    if (func_002E9F30(file, 0x1000) == D_004FB418) {
        return D_004FB418;
    }
    pos = 8;
    func_00301EE0(file, count);
    for (; pos < file->_indexBufferSize; pos += size) {
        size = file->_chunkSize & ~1;
        if (file->_indexBufferSize - pos < size) {
            size = file->_indexBufferSize - pos;
        }
        n = size >> 1;
        ((igFileV*)file->_file)->read(file->_chunk, size, 1);
        if (file->_endianSwap) {
            ((igMetaFieldV*)file->smf)->endianSwap(file->_chunk, n);
        }
        base = (u32)(pos - 8) >> 1;
        for (k = 0; k < n; k++) {
            entry = func_002D7D50(file->_uniqueEntryList, ((s16*)file->_chunk)[k]);
            igIncRef(entry);
            idx = k + base;
            igUnref(((igObject**)file->_data)[idx]);
            if (file->_count != 0 && idx >= 0 && idx < file->_count) {
                ((igObject**)file->_data)[idx] = entry;
            }
        }
    }
    return D_004FB410;
}

igResult func_002E78B0(igIGBFile* file) {
    s32 i;
    s32 n;
    igMemoryPool* pool;
    igPointerList* list;

    n = file->_count;
    pool = file->_temporaryMemoryPool;
    igUnref((igObject*)file->_refList);
    file->_refList = func_002C9130(pool);
    list = file->_refList;
    if (n >= 0) {
        if (n <= list->_capacity) {
            list->_count = n;
        } else {
            func_002D6790(list, n, 4);
        }
    }
    for (i = 0; i < n; i++) {
        ((igObjectReadV*)func_002D7E90(file, i))->readRef(file);
    }
    return D_004FB410;
}

igResult func_002E79A0(igIGBFile* file) {
    s32 i; /*P*/
    igObject* o; /*P*/

    file->_chunkPlace = file->_chunkSize;
    file->_bufferPlace = 0;
    file->_bufferSize = file->_entryBufferSize;
    if (file->_sharingState) {
        s32 n;
        igPointerList* list;

        {
            igMemoryPool* pool = file->_temporaryMemoryPool;

            igUnref((igObject*)file->_refList);
            file->_refList = func_002C9130(pool);
        }
        n = file->_entryCount;
        list = file->_refList;
        if (n >= 0) {
            if (n <= list->_capacity) {
                list->_count = n;
            } else {
                func_002D6790(list, n, 4);
            }
        }
        {
            igMemoryPool* pool = file->_temporaryMemoryPool;

            igUnref((igObject*)file->_entryPool);
            file->_entryPool = func_002BD680(pool);
        }
    }
    if (func_002E7030(file) == D_004FB418) {
        return D_004FB418;
    }
    func_00301EE0(file, file->_entryCount);
    for (i = 0; i < file->_entryCount; i++) {
        igDirEntryRef entry = func_002E7100(file);

        if (entry == NULL) {
            return D_004FB418;
        }
        entry->_index = i;
        o = entry.p;
        igIncRef(o);
        igUnref(((igObject**)file->_data)[i]);
        if (file->_count != 0 && i >= 0 && i < file->_count) {
            ((igObject**)file->_data)[i] = o;
        }
        if (file->_useMemoryPoolAssignmentsState && file->_memoryPoolHandles != NULL && !D_00554C14) {
            s32 k = ((igDirEntryV*)entry.p)->getMemoryPoolIndex();

            if (k > 0) {
                ((igDirEntryV*)entry.p)->setMemoryPool(((s32*)file->_memoryPoolHandles->_data)[k]);
            }
        } else {
            ((igDirEntryV*)entry.p)->setMemoryPool(-1);
        }
        ((igDirEntryV*)entry.p)->readFields(file);
        if (file->_sharingState) {
            SetAtRaw(GetRefList(file), i, entry->_ref);
            ((igDirEntryV*)entry.p)->setRef(entry->_ref);
            ((igDirEntryV*)entry.p)->setResolved(0);
            entry->_index = i;
            SetAt(file, i, ((igDirEntryV*)entry.p)->makeShared(file));
        }
    }
    if (file->_hasCriticalFailure) {
        return D_004FB418;
    }
    return D_004FB410;
}

s32 func_002E7DD0(igIGBFile* file) {
    if (file->_file->_openMode == 4) {
        return (s32)file->_chunk + file->_chunkPlace;
    }
    return (s32)file->_objectBufferPlace;
}

void func_002E7E00(igIGBFile* file, void* place) {
    if (file->_file->_openMode == 4) {
        s32 old = (s32)file->_chunk + file->_chunkPlace;

        file->_chunkPlace = (s32)place - (s32)file->_chunk;
        file->_bufferPlace += (s32)place - old;
        if (file->_bufferPlace != file->_bufferSize) {
            if (file->_chunkPlace > file->_chunkSize - 9) {
                func_002E7030(file);
            } else if (file->_chunkPlace + (file->_version1CruftMultiply * 4 + func_002E3A10(file, place)) > file->_chunkSize) {
                func_002E7030(file);
            }
        }
    } else {
        file->_objectBufferPlace = place;
    }
}

igResult func_002E7ED0(igIGBFile* file) {
    POOL(file->_temporaryMemoryPool)->free(file->_basePath);
    file->_basePath = NULL;
    return D_004FB410;
}

igResult func_002E7F20(igIGBFile* file) {
    igMetaFieldRegistry* reg;
    s32 regCount;
    s32* records;
    char* name;
    igMetaFieldList* list;
    s32 i;
    igObject* mf;
    s32 j;
    bool found;

    if (func_002E9F30(file, file->_metaFieldBufferSize) == D_004FB418) {
        return D_004FB418;
    }
    file->_mfBuf = file->_chunk;
    ((igFileV*)file->_file)->read(file->_mfBuf, file->_metaFieldBufferSize, 1);
    reg = func_002FC250();
    regCount = reg->_count;
    records = (s32*)file->_mfBuf;
    name = (char*)records + file->_metaFieldCount * 12;
    if (file->_endianSwap) {
        ((igMetaFieldV*)file->imf)->endianSwap(records, file->_metaFieldCount * 3);
    }
    list = func_002C8AF0(file->_temporaryMemoryPool);
    igIncRef((igObject*)list);
    igUnref((igObject*)file->_metaFieldListInFile);
    file->_metaFieldListInFile = list;
    if (file->_metaFieldCount >= file->_metaFieldListInFile->_count) {
        func_002D6680(file->_metaFieldListInFile, file->_metaFieldCount, 4);
    }
    for (i = 0; i < file->_metaFieldCount; i++) {
        found = false;
        mf = NULL;
        for (j = 0; j < regCount; j++) {
            mf = reg->_data[j];
            if (strcmp(((igObjectMetaV*)mf)->getMeta()->_name, name) == 0) {
                found = true;
                break;
            }
        }
        if (!found) {
            mf = NULL;
        } else {
            ((igObjectMetaV*)mf)->getMeta();
        }
        func_00301E30((igObjectList*)file->_metaFieldListInFile, mf);
        name += records[i * 3];
    }
    file->_mfBuf = NULL;
    return D_004FB410;
}

void igIGBFile_userResetFields(igIGBFile* file) {
    igMemoryPool* pool;

    pool = func_002F00D0();
    igIncRef((igObject*)pool);
    igUnref((igObject*)file->_temporaryMemoryPool);
    file->_temporaryMemoryPool = pool;
    pool = func_002F00D0();
    igIncRef((igObject*)pool);
    igUnref((igObject*)file->_dataMemoryPool);
    file->_dataMemoryPool = pool;
    pool = func_002F00D0();
    igIncRef((igObject*)pool);
    igUnref((igObject*)file->_objectMemoryPool);
    file->_objectMemoryPool = pool;
}

igResult func_002E82A0(igIGBFile* file) {
    s32 header[3];
    s32* lens;
    char* name;
    s32 align;
    s32 i;
    s32 j;
    s32 n;
    bool found;

    if (((igFileV*)file->_file)->read(header, 12, 1) != 1) {
        return D_004FB418;
    }
    if (file->_endianSwap) {
        ((igMetaFieldV*)file->imf)->endianSwap(header, 3);
    }
    file->_alignmentBufferSize = header[0] - 12;
    file->_alignmentCount = header[2];
    if (file->_alignmentCount <= 0) {
        return D_004FB410;
    }
    file->_alignmentBuffer = file->_chunk;
    if (((igFileV*)file->_file)->read(file->_alignmentBuffer, file->_alignmentBufferSize, 1) != 1) {
        return D_004FB418;
    }
    lens = (s32*)file->_alignmentBuffer;
    if (file->_endianSwap) {
        ((igMetaFieldV*)file->imf)->endianSwap(lens, file->_alignmentCount);
    }
    name = (char*)lens + file->_alignmentCount * 4;
    {
        igStringObjListRef names(D_00586DD0->_alignmentStrings);
        igIntListRef values(D_00586DD0->_alignmentValues);

        n = 0;
        if (names != NULL) {
            n = names->_count;
        }
        {
            igTmpRef<igIntList> list(func_002CAA70(file->_temporaryMemoryPool), IG_ADOPT);

            igAssignRef(file->_alignmentListInFile, list);
        }
        if (file->_alignmentCount >= file->_alignmentListInFile->_count) {
            func_002D6680(file->_alignmentListInFile, file->_alignmentCount, 4);
        }
        for (i = 0; i < file->_alignmentCount; i++) {
            found = false;
            align = 0x10;
            for (j = 0; j < n; j++) {
                if (strcmp(GetStr(((igStringObj**)names->_data)[j]), name) == 0) {
                    found = true;
                    break;
                }
            }
            if (found) {
                align = ((s32*)values->_data)[j];
            }
            APPEND_INT(file->_alignmentListInFile, align);
            name += lens[i];
        }
        file->_alignmentBuffer = NULL;
        return D_004FB410;
    }
}

igResult func_002E8670(igIGBFile* file) {
    s32 i;
    s32 n;

    file->_objectBufferPlace = file->_objBuf;
    file->_chunkPlace = file->_chunkSize;
    file->_bufferPlace = 0;
    file->_bufferSize = file->_objectBufferSize;
    if (func_002E7030(file) == D_004FB418) {
        return D_004FB418;
    }
    n = file->_count;
    for (i = 0; i < n; i++) {
        if (func_00300CD0(((igObject**)file->_data)[i], igObjectDirEntry_Meta)) {
            ENTRY(file, i)->readPass0(file);
        }
    }
    if (file->_magicVersion < 4) {
        for (i = 0; i < n; i++) {
            if (func_00300CD0(((igObject**)file->_data)[i], igObjectDirEntry_Meta)) {
                ENTRY(file, i)->readPass1(file);
            }
        }
    } else {
        for (i = 0; i < n; i++) {
            if (func_00300CD0(((igObject**)file->_data)[i], igObjectDirEntry_Meta)) {
                igObjectRef entry(func_002D7E90(file, i));

                ((igObjectReadV*)entry.p)->readPass2(file);
            }
        }
    }
    return D_004FB410;
}

igResult func_002E8880(igIGBFile* file) {
    s32 i;

    for (i = 0; i < file->_count; i++) {
        if (IsNotEntry(((igObject**)file->_data)[i])) {
            ENTRY(file, i)->readPass0(file);
        }
    }
    if (file->_magicVersion < 4) {
        for (i = 0; i < file->_count; i++) {
            if (IsNotEntry(((igObject**)file->_data)[i])) {
                ENTRY(file, i)->readPass1(file);
            }
        }
    } else {
        for (i = 0; i < file->_count; i++) {
            if (IsNotEntry(((igObject**)file->_data)[i])) {
                ENTRY(file, i)->readPass2(file);
            }
        }
    }
    return D_004FB410;
}

void func_002E8A10(igIGBFile* file) {
    if (file->_autoOpened > 0) {
        func_002E3CB0(file);
    }
}

void func_002E8A40(igIGBFile* file) {
    igUnref((igObject*)file->_metaFieldListInFile);
    igUnref((igObject*)file->_metaFieldListInFile);
    file->_metaFieldListInFile = NULL;
    igUnref((igObject*)file->_metaObjectListInFile);
    file->_metaObjectListInFile = NULL;
    igUnref((igObject*)file->_metaFieldPerObjectIndices);
    file->_metaFieldPerObjectIndices = NULL;
    file->_metaObjectBuffer = NULL;
    file->_dirBuf = NULL;
    file->_mfBuf = NULL;
    file->_objBuf = NULL;
    igUnref((igObject*)file->_entryPool);
    file->_entryPool = NULL;
    file->_indexBuffer = NULL;
    igUnref((igObject*)file->_uniqueEntryList);
    file->_uniqueEntryList = NULL;
    POOL(file->_temporaryMemoryPool)->free(file->_chunk);
    file->_chunk = NULL;
    file->_memoryPoolNameBufferSize = 0;
    if (file->_memoryPoolNameBuffer != NULL) {
        POOL(file->_temporaryMemoryPool)->free(file->_memoryPoolNameBuffer);
        file->_memoryPoolNameBuffer = NULL;
    }
    igUnref((igObject*)file->_memoryPoolHandles);
    file->_memoryPoolHandles = NULL;
}

void igIGBFile_userDeallocateFields(igIGBFile* file) {
    s32 i;
    s32 n;

    if (file->_refList != NULL) {
        n = file->_refList->_count;
        for (i = 0; i < n; i++) {
            ((igObjectReadV*)((igObject**)file->_data)[i])->releaseRef(((void**)file->_refList->_data)[i]);
        }
        igUnref((igObject*)file->_refList);
        file->_refList = NULL;
    }
    igUnref((igObject*)file->_refList);
    file->_refList = NULL;
    igObject_userDeallocateFields(file);
}

igResult func_002E8D40(igIGBFile* file) {
    igInfoList* infos;

    if (file->_hasInfo) {
        if (((igFileV*)file->_file)->read((char*)file + D_00586280->_offset, 4, 1) != 1) {
            return D_004FB418;
        }
        if (file->_endianSwap) {
            ((igMetaFieldV*)file->imf)->endianSwap((char*)file + D_00586280->_offset, 1);
        }
        infos = *(igInfoList**)((char*)func_002D7E90(file, file->_infoListPlace) + 0x18);
        igIncRef((igObject*)infos);
        igUnref((igObject*)file->_infoList);
        file->_infoList = infos;
    }
    return D_004FB410;
}

igResult func_002E8E50(igIGBFile* file) {
    s32 header[3];
    s32 count;
    s32* lens;
    char* name;
    s32 i;

    if (file->_hasExternal) {
        if (((igFileV*)file->_file)->read(header, 12, 1) != 1) {
            return D_004FB418;
        }
        if (file->_endianSwap) {
            ((igMetaFieldV*)file->imf)->endianSwap(header, 3);
        }
        file->_externalDirectoryBufferSize = header[0] - 12;
        count = header[2];
        file->_externalDirectoryBuffer =
            POOL(file->_temporaryMemoryPool)->allocAligned(file->_externalDirectoryBufferSize, 16);
        if (((igFileV*)file->_file)->read(file->_externalDirectoryBuffer, file->_externalDirectoryBufferSize, 1) != 1) {
            return D_004FB418;
        }
        lens = (s32*)file->_externalDirectoryBuffer;
        if (file->_endianSwap) {
            ((igMetaFieldV*)file->imf)->endianSwap(lens, count);
        }
        name = (char*)lens + count * 4;
        if (file->_managingResource == NULL) {
            file->_managingResource = func_00309A50();
        }
        if (file->_managingResource != NULL) {
            SetPoolStr(file->_previousResourceFilePath, file->_managingResource->_relativeFilePath);
            SetRelativePath(file->_managingResource, (char*)file->_basePath);
        }
        for (i = 0; i < count; i++) {
            func_00301E30((igObjectList*)file->_externalDirectoryList, func_00308FB0(file->_managingResource, name));
            name += lens[i];
        }
        POOL(file->_temporaryMemoryPool)->free(file->_externalDirectoryBuffer);
        file->_externalDirectoryBuffer = NULL;
        if (file->_managingResource != NULL) {
            SetRelativePath(file->_managingResource, file->_previousResourceFilePath);
            SetPoolStr(file->_previousResourceFilePath, NULL);
        }
    }
    return D_004FB410;
}

igResult func_002E91E0(igIGBFile* file) {
    s32 header[2];
    char* buf;
    char* name;
    s32 i;

    if (((igFileV*)file->_file)->read(header, 8, 1) != 1) {
        return D_004FB418;
    }
    if (file->_endianSwap) {
        ((igMetaFieldV*)file->imf)->endianSwap(header, 2);
    }
    buf = (char*)POOL(file->_temporaryMemoryPool)->alloc(header[0]);
    if (buf == NULL) {
        return D_004FB418;
    }
    if (((igFileV*)file->_file)->read(buf, header[0] - 8, 1) != 1) {
        POOL(file->_temporaryMemoryPool)->free(buf);
        return D_004FB418;
    }
    if (file->_memoryPoolHandles != NULL) {
        file->_memoryPoolHandles->_count = 0;
    } else {
        igMemoryPool* pool = file->_temporaryMemoryPool;

        igUnref((igObject*)file->_memoryPoolHandles);
        file->_memoryPoolHandles = func_002CAA70(pool);
    }
    name = buf;
    for (i = 0; header[1]-- > 0; i++) {
        s32 handle = func_002F89E0(name);

        APPEND_INT(file->_memoryPoolHandles, handle);
        name += strlen(name) + 1;
    }
    for (; i < 0x2E; i++) {
        APPEND_INT(file->_memoryPoolHandles, D_004C2140);
    }
    POOL(file->_temporaryMemoryPool)->free(buf);
    return D_004FB410;
}

int func_002E9470(igIGBFile* file) {
    func_002E8A40(file);
    file->_loadPendingState = false;
    file->_validState = false;
    func_002E8A10(file);
    return -1;
}

igResult func_002E94B0(igIGBFile* file) {
    s32 i;
    s32 n;
    bool concrete;

    n = file->_count;
    concrete = !(file->_concreteState != false);
    for (i = 0; i < n; i++) {
        ((igObjectReadV*)func_002D7E90(file, i))->setConcrete(concrete);
    }
    if (concrete) {
        file->_concreteState = true;
    }
    return D_004FB410;
}

igResult func_002E9560(igIGBFile* file) {
    s32 i;

    for (i = 0; i < file->_count; i++) {
        ((igObjectReadV*)func_002D7E90(file, i))->readPostProcess(file);
    }
    return D_004FB410;
}

s32 igIGBFile_readFile(igIGBFile* file, char* name) {
    double start;

    sprintf(D_00586E20, D_004F9AD8, name);
    start = D_005871D8->getTime();
    file->_validState = false;
    file->_loadPendingState = true;
    file->_concreteState = false;
    if (func_002E5F70(file, name) == D_004FB418) {
        return func_002E9470(file);
    }
    if (func_002E6090(file) == D_004FB418) {
        return func_002E9470(file);
    }
    if (file->_magicVersion < 5 && file->_hasExternal) {
        if (!D_00554C08) {
            s32 r = func_00308320(D_004F9AF0, file->_file->_fileName, file->_file->_fileName);

            if (r != 1 && r == 2) {
                D_00554C08 = true;
            }
        }
        return func_002E9470(file);
    }
    if (file->_magicVersion >= 3) {
        if (func_002E7F20(file) == D_004FB418) {
            return func_002E9470(file);
        }
        if (func_002E82A0(file) == D_004FB418) {
            return func_002E9470(file);
        }
    }
    if (func_002E6330(file) == D_004FB418) {
        return func_002E9470(file);
    }
    if (func_002E63E0(file) == D_004FB418) {
        return func_002E9470(file);
    }
    if (func_002E6F50(file) == D_004FB418) {
        return func_002E9470(file);
    }
    if (file->_magicVersion >= 5) {
        if (func_002E8E50(file) == D_004FB418) {
            return func_002E9470(file);
        }
    }
    if (file->_hasMemoryPoolNames) {
        if (func_002E91E0(file) == D_004FB418) {
            return func_002E9470(file);
        }
    }
    if (file->_sharedEntries) {
        if (func_002E7390(file) == D_004FB418) {
            return func_002E9470(file);
        }
        if (func_002E7640(file) == D_004FB418) {
            return func_002E9470(file);
        }
        if (func_002E78B0(file) == D_004FB418) {
            return func_002E9470(file);
        }
    } else {
        if (func_002E79A0(file) == D_004FB418) {
            return func_002E9470(file);
        }
    }
    if (file->_magicVersion >= 5) {
        if (func_002E8D40(file) == D_004FB418) {
            return func_002E9470(file);
        }
    }
    if (func_002E8670(file) == D_004FB418) {
        return func_002E9470(file);
    }
    if (((igDirectoryV*)file)->readFilePostProcess() == D_004FB418) {
        return func_002E9470(file);
    }
    if (file->_magicVersion < 3) {
        if (func_002E7F20(file) == D_004FB418) {
            return func_002E9470(file);
        }
    }
    if (file->_magicVersion < 5) {
        if (func_002E8D40(file) == D_004FB418) {
            return func_002E9470(file);
        }
    }
    if (file->_magicVersion < 5) {
        if (func_002E8E50(file) == D_004FB418) {
            return func_002E9470(file);
        }
    }
    if (func_002E8880(file) == D_004FB418) {
        return func_002E9470(file);
    }
    if (func_002E7ED0(file) == D_004FB418) {
        return func_002E9470(file);
    }
    func_002E94B0(file);
    if (file->_autoReadFilePostProcessState == true) {
        if (((igDirectoryV*)file)->resolveExternal() == D_004FB418) {
            return func_002E9470(file);
        }
    }
    func_002E8A10(file);
    func_002E8A40(file);
    file->_validState = true;
    file->_loadPendingState = false;
    if (!D_00554C0C) {
        s32 r = func_00308240(D_004F9B90, name, D_005871D8->getTime() - start);

            if (r != 1 && r == 2) {
                D_00554C0C = true;
            }
    }
    return file->_count;
}

/* Returns an igResult by value; written with the result slot explicit because a returned
   igResult is copied through $f0, unlike the original. */
igResult func_002E9C30(igIGBFile* file) {
    if (file->_concreteState == false) {
        func_002D8F20(file);
    }
    return D_004FB410;
}

igResult func_002E9C70(igIGBFile* file) {
    if (file->_readFilePostProcessedState == false) {
        igResult r = func_002E9560(file);

        if (file->_managingResource != NULL) {
            func_0030A260(file->_managingResource, file->_name);
        }
        file->_readFilePostProcessedState = true;
        return r;
    }
    return D_004FB410;
}

igResult func_002E9CE0(igIGBFile* file) {
    igResult res = D_004FB410;
    s32 i;

    for (i = 0; i < file->_externalDirectoryList->_count; i++) {
        igIGBFileRef ext(igIGBFile_cast(((igObject**)file->_externalDirectoryList->_data)[i]));

        if (ext != NULL) {
            if (((igDirectoryV*)ext.p)->resolve() == D_004FB418) {
                res = D_004FB418;
            }
        }
    }
    if (((igDirectoryV*)file)->resolveExternal() == D_004FB418) {
        res = D_004FB418;
    }
    return res;
}

s32 func_002E9E50(igIGBFile* file) {
    s32 i;

    file->_chunkSize = file->_forceChunkSize;
    if (file->_chunkSize < 0) {
        file->_chunkSize = file->_file->_optimalWriteChunkSize;
    }
    if (file->_memoryRefBufferSize < file->_chunkSize) {
        file->_chunkSize = file->_memoryRefBufferSize;
    }
    file->_chunk = POOL(file->_temporaryMemoryPool)->allocAligned(file->_chunkSize, file->_file->_pageSize);
    file->_memoryBufferPlace = 0;
    file->_memoryBytesCopied = 0;
    file->_bytesRemainingInChunk = file->_chunkSize;
    for (i = 0; i < file->_count; i++) {
        ((igObjectReadV*)func_002D7E90(file, i))->readBuffers(file);
    }
    POOL(file->_temporaryMemoryPool)->free(file->_chunk);
    file->_chunk = NULL;
    return 1;
}

igResult func_002E9F30(igIGBFile* file, s32 size) {
    if (file->_chunk != NULL && file->_chunkSize >= size) {
        return D_004FB410;
    }
    if (file->_chunk != NULL) {
        POOL(file->_temporaryMemoryPool)->free(file->_chunk);
    }
    file->_chunkSize = file->_forceChunkSize;
    if (file->_chunkSize < 0) {
        file->_chunkSize = file->_file->_optimalReadChunkSize;
    }
    if (file->_chunkSize < size) {
        file->_chunkSize = size;
    }
    file->_chunkSize = (file->_chunkSize + 3) & ~3;
    file->_chunk = POOL(file->_temporaryMemoryPool)->allocAligned(file->_chunkSize, file->_file->_pageSize);
    if (file->_chunk == NULL) {
        return D_004FB418;
    }
    return D_004FB410;
}

}
