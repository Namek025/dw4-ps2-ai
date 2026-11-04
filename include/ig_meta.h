#ifndef IG_META_H
#define IG_META_H

/* Alchemy meta registration boilerplate. Every registered class has a run of small
   functions next to its _register / _registerFields (see CLAUDE.md "Alchemy reflection
   data"); these macros write one each. tools/meta_src.py recognizes them in the asm and
   emits the macros (with the function names, so they can stay func_XXXXXXXX). The
   prototypes they reference are declared by the generator, in its block of the unit. */

#include "game.h"

struct igMetaArkCore {
    u8 unk0[0x394];
    s32 _vtableOffset; /* 0x394: where an instance keeps its vtable pointer */
};

extern "C" {
extern igMetaArkCore* D_00586DC8;
extern igMemoryPool* D_00587040;
igMetaObject* func_002BE4F0(void* pool);                        /* new igMetaObject */
igObject* func_002FDA40(igMetaObject* meta, igMemoryPool* pool); /* instantiate */
igMetaEnum* func_002FBC70(const char* name, void* names, void* values, s32 count); /* new enum */
void func_002FE940(void (*reg)(void));                          /* register a class once */
void func_002FEF20(void* obj);                                  /* igObject constructor */
s32 func_002FE470(igMetaObject* meta);                          /* field count */
igMetaField* func_002FDF00(igMetaObject* meta, s32 index);      /* field by index */
void metaSetFieldTypes(igMetaObject* meta, void* types, s32 count);
void metaSetFieldNames(igMetaObject* meta, void* names, void* a2, void* offsets, s32 base);
igMetaField* func_002FDF20(igMetaObject* meta, const char* name);  /* field by name */
s32 func_002EA820(void* list, igMetaField* f);                     /* index in a list */
void func_002FDEA0(igMetaObject* meta, s32 index, igMetaField* f); /* replace a field */
void func_0030AA10(igMetaField* f, const char* s);
void func_002EA4B0(igMetaField* f, s32 v);
void func_002D3890(igMetaField* f, s32 v);
void func_002EBC00(igMetaField* f, s32 v);
}

class igMetaFieldV {
public:
    IG_VPAD_19
    virtual igMetaField* createCopy(bool deep); /* +0x54 */
};

extern "C" {
void metaRegister(s32 isAbstract, igMetaObject** meta, void (*parentRegister)(void),
                  igMetaObject* (*parentGetMeta)(void), igMetaObject* (*getMeta)(void),
                  const char* name, s32 size, void* (*retrieveVTable)(void),
                  void (*registerFields)(void), void* (*a9)(void), void* a10);
}

static inline bool igMetaIsRegistered(igMetaObject* meta) {
    return (meta->_validity & 4) != 0;
}

/* Creates the meta object the first time. */
#define IG_META_CREATE(fn, meta)                                                  \
    extern "C" igMetaObject* fn(void) {                                           \
        if (meta == NULL) {                                                       \
            meta = func_002BE4F0(func_002F88E0(D_00587040));                      \
        }                                                                         \
        return meta;                                                              \
    }

/* getClassMeta: registers the class the first time. */
#define IG_META_GET_CLASS_META(fn, meta, arkRegister)                             \
    extern "C" igMetaObject* fn(void) {                                           \
        if (meta == NULL || igMetaIsRegistered(meta) == false) {                  \
            arkRegister();                                                        \
        }                                                                         \
        return meta;                                                              \
    }

#define IG_META_ARK_REGISTER(fn, reg)                                             \
    extern "C" void fn(void) {                                                    \
        func_002FE940(reg);                                                       \
    }

/* The meta object's _getMetaObjectFunction. */
#define IG_META_THUNK(fn, target)                                                 \
    extern "C" igMetaObject* fn(void) {                                           \
        return target();                                                          \
    }

/* A retrieveVTable (or metaRegister's 9th argument) that forwards to a function next to
   the class's methods. */
#define IG_META_VTABLE_THUNK(fn, target)                                          \
    extern "C" void* fn(void) {                                                   \
        return target();                                                          \
    }

/* Vtable of a class whose constructor only sets it (no fields to initialize). */
#define IG_META_VTABLE(fn, T, vtable)                                             \
    extern "C" void* fn(void) {                                                   \
        T obj;                                                                    \
        func_002FEF20(&obj);                                                      \
        *(void**)&obj = vtable;                                                   \
        return *(void**)((char*)&obj + D_00586DC8->_vtableOffset);                \
    }

/* isAbstract classes pass no retrieveVTable. */
#define IG_META_REGISTER(fn, pre, isAbstract, meta, parentRegister, parentGetMeta,    \
                         getMeta, name, size, retrieveVTable, registerFields, a9, a10) \
    extern "C" void fn(void) {                                                    \
        pre();                                                                    \
        metaRegister(isAbstract, &meta, parentRegister, parentGetMeta, getMeta, name, \
                     size, retrieveVTable, registerFields, a9, a10);              \
    }

#define IG_META_INSTANTIATE(fn, meta, arkRegister)                                \
    extern "C" igObject* fn(igMemoryPool* pool) {                                 \
        arkRegister();                                                            \
        return func_002FDA40(meta, pool);                                         \
    }

#define IG_META_GETTER(fn, meta)                                                  \
    extern "C" igMetaObject* fn(void) {                                           \
        return meta;                                                              \
    }

/* An enum's meta object, created the first time. */
#define IG_META_ENUM(fn, storage, name, names, values, count)                     \
    extern "C" igMetaEnum* fn(void) {                                             \
        if (storage == NULL) {                                                    \
            storage = func_002FBC70(name, names, values, count);                  \
        }                                                                         \
        return storage;                                                           \
    }

/* X_registerFields: field types, then per-field fixes (IG_FIELD_*), then names.
   IG_META_FIELDS(fn, meta, types, count)
   IG_FIELD_REF(k, T_getMeta)   object ref field base+k points to T
   IG_FIELDS_END(names, a2, offsets) */
#define IG_META_FIELDS(fn, meta, types, count)                                    \
    extern "C" void fn(void) {                                                    \
        igMetaObject* meta_ = meta;                                               \
        s32 base_ = func_002FE470(meta_);                                         \
        metaSetFieldTypes(meta_, types, count);

#define IG_FIELD_REF(k, getMeta)                                                  \
        {                                                                         \
            igMetaField* f_ = func_002FDF00(meta_, base_ + k);                    \
            *(igMetaObject**)((char*)f_ + 0x38) = getMeta();                      \
        }

/* Any other field fix: IG_FIELD(k) / IG_FIELD_OVERRIDE(name) open a field, IG_SET and
   IG_CALL change it, IG_FIELD_END / IG_FIELD_OVERRIDE_END close it. An override copies
   the field inherited under `name`, changes the copy and puts it in its place. */
#define IG_FIELD(k)                                                               \
        {                                                                         \
            igMetaField* f_ = func_002FDF00(meta_, base_ + k);

#define IG_FIELD_OVERRIDE(name)                                                   \
        {                                                                         \
            igMetaField* f_ = func_002FDF20(meta_, name);                         \
            s32 i_ = func_002EA820(meta_->_metaFields, f_);                       \
            f_ = ((igMetaFieldV*)f_)->createCopy(true);

#define IG_SET(T, off, v)                                                         \
            *(T*)((char*)f_ + off) = v;

#define IG_CALL(fn, v)                                                            \
            fn(f_, v);

#define IG_FIELD_END                                                              \
        }

#define IG_FIELD_OVERRIDE_END                                                     \
            func_002FDEA0(meta_, i_, f_);                                         \
            f_->_refCount--;                                                      \
            if ((f_->_refCount & 0x7FFFFF) == 0) {                                \
                func_002FF820(f_);                                                \
            }                                                                     \
        }

#define IG_FIELDS_END(names, a2, offsets)                                         \
        metaSetFieldNames(meta_, names, a2, offsets, base_);                      \
    }

/* X_registerFields of a class that adds no fields, only overrides inherited ones. */
#define IG_META_OVERRIDES(fn, meta)                                               \
    extern "C" void fn(void) {                                                    \
        igMetaObject* meta_ = meta;

#define IG_OVERRIDES_END                                                          \
    }

/* Alchemy: after the names, hand a function to the meta system. */
#define IG_FIELDS_END_CALL(names, a2, offsets, fn, arg)                           \
        metaSetFieldNames(meta_, names, a2, offsets, base_);                      \
        fn((void*)arg);                                                           \
    }

/* Alchemy: metaRegister's 9th argument. */
#define IG_META_SETUP(fn, first, second, arg)                                     \
    extern "C" void* fn(void) {                                                   \
        first();                                                                  \
        return second((void*)arg);                                                \
    }

#endif
