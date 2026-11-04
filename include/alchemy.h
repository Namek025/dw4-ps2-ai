#ifndef ALCHEMY_H
#define ALCHEMY_H

/* Intrinsic Alchemy engine helpers (MWCC C++). The class layouts come from the
   game's reflection data, see meta.h. */

#include "meta.h"
#include "vpad.h"

extern "C" {

void func_002FF820(igObject* o); /* release */
void func_002D6790(igDataList* list, s32 count, s32 elemSize); /* grow to count */

/* Pooled strings: a header with the owner at -8 and a reference count at -4. */
typedef struct StrPool StrPool;

typedef struct {
    void* owner;
    s32 refs;
} StrHeader;

extern StrPool* D_00586F20;
void* func_002EB010(u32 size);                    /* operator new */
StrPool* func_002EADF0(StrPool* pool);            /* StrPool constructor */
char* func_002EAF60(StrPool* pool, const char* s); /* get / add a string */
void func_0030C940(void* owner, StrHeader* h); /* free a pooled string */
bool func_00300CD0(void* o, igMetaObject* meta);  /* isOfType */

}

static inline StrPool* GetStrPool(void) {
    if (D_00586F20 == NULL) {
        StrPool* pool = (StrPool*)func_002EB010(0x10);
        if (pool != NULL) {
            pool = func_002EADF0(pool);
        }
        D_00586F20 = pool;
    }
    return D_00586F20;
}

static inline char* PoolStr(const char* s) {
    return s == NULL ? NULL : func_002EAF60(GetStrPool(), s);
}

static inline void ReleaseStr(char* s) {
    if (s != NULL) {
        StrHeader* h = (StrHeader*)s - 1;
        if (--h->refs == 0) {
            func_0030C940(h->owner, h);
        }
    }
}

/* Reference count: low 23 bits of igObject::_refCount. */
static inline void igIncRef(igObject* o) {
    if (o != NULL) {
        o->_refCount++;
    }
}

static inline void igUnref(igObject* o) {
    if (o != NULL) {
        o->_refCount--;
        if ((o->_refCount & 0x7FFFFF) == 0) {
            func_002FF820(o);
        }
    }
}

/* Tag for taking over a new object's reference (what _instantiateFromPool hands back). */
enum igAdopt { IG_ADOPT };

/* Smart pointer class per type (Alchemy-style igXxxRef, not a template: the
   out-of-line destructors get plain mangled names like __dt__12CallEntryRefFv).
   With C++ exceptions on it lives on the stack (it has a destructor), so
   every use reloads it. */
#define IG_REF(T)                                     \
    struct T##Ref {                                   \
        T* p;                                         \
        T##Ref() : p(NULL) {}                         \
        T##Ref(T* o) : p(o) { igIncRef(p); }          \
        T##Ref(T* o, igAdopt) : p(o) {}               \
        T##Ref& operator=(T* o) {                     \
            igIncRef(o);                              \
            igUnref(p);                               \
            p = o;                                    \
            return *this;                             \
        }                                             \
        T##Ref& operator=(const T##Ref& r) {          \
            T* o = r.p;                               \
            igIncRef(o);                              \
            igUnref(p);                               \
            p = o;                                    \
            return *this;                             \
        }                                             \
        ~T##Ref() { igUnref(p); }                     \
        operator T*() const { return p; }             \
        T* operator->() const { return p; }           \
    }

/* Assign an object reference field (what a smart pointer member's operator= does). */
template <class T>
static inline void igAssign(T*& dst, T* src) {
    igIncRef(src);
    igUnref(dst);
    dst = src;
}

/* Alchemy-style checked cast (igDynamicCast<T>): the object if it is of class T,
   else NULL. Per class, so the meta is read inside (as the original does). */
#define IG_DYNAMIC_CAST(T)                                     \
    static inline T* T##_cast(void* o) {                       \
        T* r;                                                  \
                                                               \
        if (o != NULL && func_00300CD0(o, T##_Meta)) {         \
            r = (T*)o;                                         \
        } else {                                               \
            r = NULL;                                          \
        }                                                      \
        return r;                                              \
    }

/* Alchemy result code, returned by value (result slot in a0) and compared with ==
   against the success/failure globals. */
struct igResult {
    s32 code;

    igResult() {}
    igResult(const igResult& o) : code(o.code) {}
    bool operator==(const igResult& o) const { return code == o.code; }
    /* Copied as an int (a plain struct copy goes through $f0). */
    void operator=(const igResult& o) { code = o.code; }
};


extern "C" {
extern igResult D_004FB410; /* success */
extern igResult D_004FB418; /* failure */
}

#endif /* ALCHEMY_H */
