#include "alchemy.h"

/* beMessenger: the game's message system (MWCC C++, same flags as script_bind).
 *
 * Objects (beBaseInfoRam, one per instance of an "info") register under a
 * group name ("GAMERAM", "MODELCTRL", ...). A call selects a group (optionally
 * an info name and a ram number) into a work entry on _workStack, fills the
 * argument lists in _argData, then sends a message name: every matching ram's
 * manager gets it through a virtual call, and results come back in _argData.
 * Delayed calls wait in _delayStack, sorted by send time.
 *
 * Method names are descriptive (the originals are not in the binary). */
extern "C" {

void func_001908F0(igMatrix44f* dst, const igMatrix44f* src); /* copy */
void func_00301E30(igObjectList* list, igObject* o);       /* append, with a reference */
void func_001CA620(igStringRefList* list, char** str);     /* append */
void func_001C64E0(char** out, igStringRefList* list, s32 index); /* returns a pooled string by value */
void func_002D6CF0(igDataList* list, s32 index);                    /* remove */
void func_002D6B80(igDataList* list, s32 index, s32 n, void* src);  /* insert */
void func_002D6680(igDataList* list, s32 capacity, s32 elemSize);   /* set capacity */
s32 strcmp(const char* a, const char* b);

extern igMetaObject* beMessenger_Meta;
extern bool D_00554C14;
extern void* D_00587110;
extern igMetaObject* D_00585BE0;
extern char D_004E0858[]; /* "" */
extern char D_0050EE10[]; /* "SEARCH" */
extern char D_0050EE18[]; /* "FIND_SPACE" */
void* func_00301000(void* o); /* memory pool of an object */
void* func_002F88E0(void* p);
beMessengerGroup* func_003022C0(igObjectList* list, igMetaObject* meta, const char* name); /* find by name */
beMessengerGroup* func_003584E0(void* pool);  /* new beMessengerGroup */
beMessengerDelay* func_00358950(void* pool);  /* new beMessengerDelay */
void func_002FECB0(igNamedObject* o, const char* name); /* set name */
void beMessenger_allocate(beMessenger* m);

extern igObject* D_0058A4E0; /* last table looked up */
extern char D_0058A4F0[];    /* last name looked up */
extern s32 D_0058A530;       /* its index */
igResult func_002FBA30(igObject* table, const char* name, s32* value);
char* strcpy(char* dst, const char* src);
void func_00236BE0(igInsightCore* insight, void* o);
void func_00236C40(igInsightCore* insight, void* o);
void func_0038C490(beMessenger* m);

void beMessenger_clearArgs(beMessengerArgData* args); /* clear */
void beMessenger_clearDelay(beMessengerDelay* d);

void beMessenger_send(beMessenger* m, const char* message);
void beMessenger_select(beMessenger* m, const char* group, const char* info, s32 ramNo);
void beMessenger_delaySelect(beMessenger* m, const char* group, const char* info, s32 ramNo);
void beMessenger_delaySend(beMessenger* m, const char* message, s64 delay);

/* The manager a ram belongs to handles its messages (virtual, two slots known):
   handle() switches on the message name, unhandled() gets the rest. */
class beInfoManagerV {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void unhandled(beBaseInfoRam* ram, const char* message);  /* vtable +0x8C, default: nothing */
    virtual bool handle(beBaseInfoRam* ram, beMessengerWork* work);   /* vtable +0x90, true if handled */
};

IG_REF(beMessengerGroup);
IG_REF(beMessengerWork);
IG_REF(beMessengerDelay);
IG_REF(beMessengerArgData);
IG_REF(beBaseInfoRam);

}

static inline void* GetPool(void* o) {
    void* a = func_00301000(o);

    return D_00554C14 ? a : func_002F88E0(D_00587110);
}

/* String fields are smart pointers in the original (assignment pools the new
   string, then drops the old one). Written per field: through a char*& the
   compiler forms the field address first. */
#define DEFINE_SET_STR(T, field)                                  \
    static inline void Set##field(T* o, const char* s) {          \
        char* str = func_002EAF60(GetStrPool(), s);               \
                                                                  \
        ReleaseStr(o->field);                                     \
        o->field = str;                                           \
    }

DEFINE_SET_STR(beMessengerWork, _groupName)
DEFINE_SET_STR(beMessengerWork, _infoName)
DEFINE_SET_STR(beMessengerWork, _message)

static inline void SetDelayGroup(beMessengerDelay* o, const char* s) {
    char* str = func_002EAF60(GetStrPool(), s);

    ReleaseStr(o->_groupName);
    o->_groupName = str;
}

static inline void SetDelayInfo(beMessengerDelay* o, const char* s) {
    char* str = func_002EAF60(GetStrPool(), s);

    ReleaseStr(o->_infoName);
    o->_infoName = str;
}

static inline void SetDelayMessage(beMessengerDelay* o, const char* s) {
    char* str = func_002EAF60(GetStrPool(), s);

    ReleaseStr(o->_message);
    o->_message = str;
}

/* _argData is a smart pointer member in the original; assign through its Ref class. */
#define ARGDATA(m) (*(beMessengerArgDataRef*)&(m)->_argData)

template <class L>
static inline void ObjListRemove(L* list, s32 i) {
    igUnref(((igObject**)list->_data)[i]);
    func_002D6CF0(list, i);
    ((igObject**)list->_data)[list->_count] = NULL;
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

static inline void ListShrinkN(igDataList* list, s32 elemSize) {
    if (list->_capacity > 4 && list->_count < 5) {
        func_002D6680(list, 4, elemSize);
    }
}

static inline void SetArgDebug(beMessengerArgData* o, const char* s) {
    char* str = func_002EAF60(GetStrPool(), s);

    ReleaseStr(o->_debugString);
    o->_debugString = str;
}

static inline void ListShrink(igDataList* list) {
    if (list->_capacity > 4 && list->_count < 5) {
        func_002D6680(list, 4, 4);
    }
}

/* Append to an int list. The count is reread for the increment (volatile). */
static inline void IntListAppend(igDataList* list, s32 v) {
    s32 n;

    n = list->_count;
    if (n < list->_capacity) {
        (*(volatile s32*)&list->_count)++;
    } else {
        func_002D6790(list, n + 1, 4);
    }
    ((s32*)list->_data)[n] = v;
}

static inline void PushBool(beMessengerArgData* args, const bool& v) {
    s32 n;
    igDataList* list = args->_boolList;

    n = list->_count;
    if (n < list->_capacity) {
        list->_count = n + 1;
    } else {
        func_002D6790(list, n + 1, 1);
    }
    ((bool*)list->_data)[n] = v;
}

static inline void PushInt(beMessengerArgData* args, const s32& v) {
    s32 n;
    igDataList* list = args->_intList;

    n = list->_count;
    if (n < list->_capacity) {
        (*(volatile s32*)&list->_count)++;
    } else {
        func_002D6790(list, n + 1, 4);
    }
    ((s32*)list->_data)[n] = v;
}

static inline void ResetArgs(beMessenger* m) {
    if (!m->_isMemoryAssign) {
        beMessenger_allocate(m);
    } else {
        beMessenger_clearArgs(m->_argData);
    }
}

extern "C" {

INCLUDE_ASM("asm/nonmatchings/beMessenger", beMessenger_allocate);

/* Out-of-line destructor of a beMessenger smart pointer (deleting form). The Ref type is
   not known; the code is the same for every IG_REF type. */
beMessengerDelayRef* func_0038C390(beMessengerDelayRef* r, s16 flags) {
    if (r != NULL) {
        igUnref(r->p);
        if (flags > 0) {
            operator delete(r);
        }
    }
    return r;
}

/* Out-of-line destructor of a second beMessenger smart pointer (deleting form). */
beMessengerWorkRef* func_0038C410(beMessengerWorkRef* r, s16 flags) {
    if (r != NULL) {
        igUnref(r->p);
        if (flags > 0) {
            operator delete(r);
        }
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/beMessenger", func_0038C490);

/* Clear all argument lists (keeping at most 4 slots of capacity each). */
void beMessenger_clearArgs(beMessengerArgData* a) {
    a->_isRunCheck = false;
    SetArgDebug(a, NULL);
    a->_boolList->_count = 0;
    ListShrinkN(a->_boolList, 1);
    a->_intList->_count = 0;
    ListShrinkN(a->_intList, 4);
    a->_floatList->_count = 0;
    ListShrinkN(a->_floatList, 4);
    a->_longList->_count = 0;
    ListShrinkN(a->_longList, 8);
    a->_vec3fList->_count = 0;
    ListShrinkN(a->_vec3fList, 0xC);
    a->_matrix44fList->_count = 0;
    ListShrinkN(a->_matrix44fList, 0x40);
    {
        igDataList* list = a->_stringList;
        s32 n = list->_count;
        s32 i;

        for (i = 0; i < n; i++) {
            char** p = &((char**)list->_data)[i];
            char* str = func_002EAF60(GetStrPool(), NULL);

            ReleaseStr(*p);
            *p = str;
        }
        list->_count = 0;
    }
    ListShrinkN(a->_stringList, 4);
    {
        igDataList* list = a->_objectList;
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
    ListShrinkN(a->_objectList, 4);
}

/* Clear a delayed call entry. */
void beMessenger_clearDelay(beMessengerDelay* d) {
    d->_isUse = false;
    d->_sendTime = 0;
    SetDelayGroup(d, NULL);
    SetDelayInfo(d, NULL);
    d->_ramNo = 0;
    SetDelayMessage(d, NULL);
    beMessenger_clearArgs(d->_argData);
}

void beMessenger_registerToInsight(beMessenger* m) {
    func_00236BE0(m->_insight, m);
}

void beMessenger_unRegisterFromInsight(beMessenger* m) {
    func_00236C40(m->_insight, m);
}

void beMessenger_begin(void) {
}

void beMessenger_end(void) {
}

s32 beMessenger_appendInfo(beMessenger* m) {
    if (!m->_isMemoryAssign) {
        beMessenger_allocate(m);
    }
    return 0;
}

void beMessenger_removeAllInfos(beMessenger* m) {
    func_0038C490(m);
}

/* Per-frame update: deliver the delayed calls whose time has come (in time
   order); while frozen, push their send times forward instead. */
#ifdef NON_MATCHING
/* Nonmatching, registers only: in the argument swap the original keeps the
   saved lists in s5 and the entry in s4. */
void beMessenger_update(beMessenger* m) {
    if (m->_isMemoryAssign) {
        s32 n = m->_delayStack->_count;

        if (n != 0) {
            if (m->_freezeTime == 0) {
                s64 now;
                s32 i;
                beMessengerDelay* dp;
                beMessengerArgData* o;

                now = ((igInsightCore*)m->_insight)->_worldTime;
                for (i = 0; i < n; i++) {
                    beMessengerDelayRef d = ((beMessengerDelay**)m->_delayStack->_data)[0];

                    if (!d->_isUse) {
                        break;
                    }
                    if (now < d->_sendTime) {
                        break;
                    }
                    beMessenger_select(m, d->_groupName, d->_infoName, d->_ramNo);
                    {
                        beMessengerArgDataRef tmp;

                        tmp = m->_argData;
                        ARGDATA(m) = d->_argData;
                        {
                            dp = d;
                            o = tmp.p;

                            igIncRef(o);
                            igUnref(dp->_argData);
                            dp->_argData = o;
                        }
                    }
                    beMessenger_send(m, d->_message);
                    beMessenger_clearDelay(d);
                    ObjListRemove(m->_delayStack, 0);
                    func_00301E30(m->_delayStack, d);
                }
            } else {
                s64 freeze;
                s64 delta;
                s32 i;
                s64 now;

                freeze = m->_freezeTime;
                now = ((igInsightCore*)m->_insight)->_worldTime;
                delta = now - freeze;

                for (i = 0; i < n; i++) {
                    beMessengerDelay* d = ((beMessengerDelay**)m->_delayStack->_data)[i];

                    if (!d->_isUse) {
                        break;
                    }
                    d->_sendTime = (u64)delta + d->_sendTime;
                }
                m->_freezeTime = now;
            }
            beMessenger_clearArgs(m->_argData);
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/beMessenger", beMessenger_update);
#endif

/* Freeze (remember the world time) or unfreeze delayed calls. */
void beMessenger_setFreeze(beMessenger* m, bool freeze) {
    if (freeze) {
        m->_freezeTime = ((igInsightCore*)m->_insight)->_worldTime;
    } else {
        m->_freezeTime = 0;
    }
}

/* Index of a message name in a name table, with a one-entry cache. */
s32 beMessenger_getMessageIndex(beMessenger* m, igObject* table, const char* name) {
    s32 value;

    if (D_0058A4E0 == table && strcmp(D_0058A4F0, name) == 0) {
        return D_0058A530;
    }
    if (func_002FBA30(table, name, &value) == D_004FB410) {
        D_0058A4E0 = table;
        strcpy(D_0058A4F0, name);
        D_0058A530 = value;
        return value;
    }
    return -1;
}

/* Register ram under group, with its info name and ram number. */
void beMessenger_entry(beMessenger* m, beBaseInfoRam* ram, const char* group, const char* info, s32 ramNo) {
    beMessengerGroupRef g = func_003022C0(m->_groupList, D_00585BE0, group);

    if (g == NULL) {
        void* a = func_00301000(m);
        void* pool = D_00554C14 ? a : func_002F88E0(D_00587110);

        igUnref(g.p);
        g.p = func_003584E0(pool);
        func_002FECB0(g, group);
        func_00301E30(m->_groupList, g);
    }
    func_00301E30(g->_ramList, ram);
    {
        char* s = PoolStr(info);

        func_001CA620(g->_infoNameList, &s);
        ReleaseStr(s);
    }
    IntListAppend(g->_ramNoList, ramNo);
}

/* __dt__19beMessengerGroupRefFv is beMessengerGroupRef's out-of-line destructor (exception
   cleanup of beMessenger_entry, named in config/symbol_addrs.txt). */

void beMessenger_entryRam(beMessenger* m, beBaseInfoRam* ram, const char* group) {
    s32 ramNo = ram->_ramNo;

    beMessenger_entry(m, ram, group, ram->_parentInfo->_name ? ram->_parentInfo->_name : D_004E0858, ramNo);
}

/* Select group / info / ramNo for the next send: reuse the work entry that
   already has them (moved to the top of the stack) or recycle the oldest. */
void beMessenger_select(beMessenger* m, const char* group, const char* info, s32 ramNo) {
    s32 j;
    s32 k;
    beMessengerWork* top;
    beMessengerGroup* g;
    bool found;
    s32 n;
    s32 i;

    ResetArgs(m);
    found = false;
    n = m->_workStack->_count;
    for (i = 0; i < n; i++) {
        beMessengerWorkRef w = ((beMessengerWork**)m->_workStack->_data)[i];

        if (w->_groupName == NULL) {
            continue;
        }
        if (strcmp(w->_groupName, group) != 0) {
            continue;
        }
        if (w->_infoName == NULL || info == NULL) {
            if (w.p->_infoName != info) {
                continue;
            }
        } else if (strcmp(w->_infoName, info) != 0) {
            continue;
        }
        w->_ramNo = ramNo;
        if (i != 0) {
            ObjListRemove(m->_workStack, i);
            ObjListInsert(m->_workStack, 0, w);
        }
        found = true;
        break;
    }
    if (found == false) {
        s32 last = m->_workStack->_count - 1;
        beMessengerWorkRef w = ((beMessengerWork**)m->_workStack->_data)[last];

        Set_groupName(w, group);
        Set_infoName(w, info);
        w->_ramNo = ramNo;
        w->_findRamNoList->_count = 0;
        ListShrink(w->_findRamNoList);
        {
            igDataList* list = w->_findRamList;
            s32 c = list->_count;
            s32 a;

            for (a = 0; a < c; a++) {
                igUnref(((igObject**)list->_data)[a]);
            }
            {
                s32 c2 = list->_count;
                s32 b;

                for (b = 0; b < c2; b++) {
                    ((igObject**)list->_data)[b] = NULL;
                }
            }
            list->_count = 0;
        }
        ListShrink(w->_findRamList);
        ObjListRemove(m->_workStack, m->_workStack->_count - 1);
        ObjListInsert(m->_workStack, 0, w);
    }
    top = ((beMessengerWork**)m->_workStack->_data)[0];
    if (top->_findRamList->_count == 0) {
        g = func_003022C0(m->_groupList, D_00585BE0, top->_groupName);
        if (g != NULL) {
            s32 cnt = g->_ramList->_count;

            for (j = 0; j < cnt; j++) {
                char* s;
                char* tmp;

                func_001C64E0(&tmp, g->_infoNameList, j);
                s = tmp;
                ReleaseStr(s);
                if (top->_infoName == NULL || s == NULL || strcmp(top->_infoName, s) == 0) {
                    s32* id = &((s32*)g->_ramNoList->_data)[j];
                    igDataList* list = top->_findRamNoList;

                    k = list->_count;
                    if (k < list->_capacity) {
                        (*(volatile s32*)&list->_count)++;
                    } else {
                        func_002D6790(list, k + 1, 4);
                    }
                    ((s32*)list->_data)[k] = *id;
                    func_00301E30(top->_findRamList, ((igObject**)g->_ramList->_data)[j]);
                }
            }
        }
    }
}

/* Send message to the selected rams. "SEARCH" returns whether (and how many)
   rams match, "FIND_SPACE" the first one that is not running. */
#ifdef NON_MATCHING
/* Nonmatching: the "SEARCH" loop's guard is placed at the end of the function
   in the original and _data is reloaded every iteration; registers in the
   last loop differ too. */
void beMessenger_send(beMessenger* m, const char* message) {
    s32 n;
    s32 count;
    s32 i;
    beMessengerWork* top = ((beMessengerWork**)m->_workStack->_data)[0];

    Set_message(top, message);
    n = top->_findRamList->_count;
    count = 0;
    if (strcmp(message, D_0050EE10) == 0) {
        for (i = 0; i < n; i++) {
            s32 id = ((s32*)top->_findRamNoList->_data)[i];

            if (top->_ramNo == -1 || id == -1 || id == top->_ramNo) {
                count++;
            }
        }
        if (count != 0) {
            PushBool(m->_argData, true);
            PushInt(m->_argData, count);
        } else {
            PushBool(m->_argData, false);
            PushInt(m->_argData, 0);
        }
    } else if (strcmp(message, D_0050EE18) == 0) {
        beBaseInfoRamRef best = NULL;

        for (i = 0; i < n; i++) {
            beBaseInfoRam* r = ((beBaseInfoRam**)top->_findRamList->_data)[i];

            if (!r->_isRun) {
                igIncRef(r);
                igUnref(best.p);
                best.p = r;
                break;
            }
        }
        if (best != NULL) {
            func_00301E30(m->_argData->_objectList, best);
        }
    } else {
        for (i = 0; i < n; i++) {
            beBaseInfoRam* r = ((beBaseInfoRam**)top->_findRamList->_data)[i];

            if (top->_ramNo == -2) {
                if (r->_isRun) {
                    continue;
                }
            } else {
                s32 id = ((s32*)top->_findRamNoList->_data)[i];

                if (m->_argData->_isRunCheck == 1 && !r->_isRun) {
                    continue;
                }
                if (!(top->_ramNo == -1 || id == -1 || id == top->_ramNo)) {
                    continue;
                }
            }
            {
                beInfoManagerV* mgr = (beInfoManagerV*)r->_parentManager;

                if (mgr->handle(r, top) == false) {
                    mgr->unhandled(r, message);
                }
            }
            if (top->_ramNo == -2) {
                break;
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/beMessenger", beMessenger_send);
#endif

/* __dt__16beBaseInfoRamRefFv is beBaseInfoRamRef's out-of-line destructor (exception
   cleanup of beMessenger_send's `best`). While send is asm, this caller (not in the
   unit, so mwsplice drops it) makes MWCC emit it. */
#ifndef NON_MATCHING
void emit___dt__16beBaseInfoRamRefFv(beMessenger* m, beBaseInfoRam* r) {
    beBaseInfoRamRef best = r;

    func_00301E30(m->_argData->_objectList, best);
}
#endif

void beMessenger_selectRamNo(beMessenger* m, const char* group, s32 ramNo) {
    beMessenger_select(m, group, NULL, ramNo);
}

void beMessenger_selectGroup(beMessenger* m, const char* group) {
    beMessenger_select(m, group, NULL, -1);
}

void beMessenger_call(beMessenger* m, const char* group, const char* info, s32 ramNo, const char* message) {
    beMessenger_select(m, group, info, ramNo);
    beMessenger_send(m, message);
}

void beMessenger_callRamNo(beMessenger* m, const char* group, s32 ramNo, const char* message) {
    beMessenger_selectRamNo(m, group, ramNo);
    beMessenger_send(m, message);
}

void beMessenger_callGroup(beMessenger* m, const char* group, const char* message) {
    beMessenger_selectGroup(m, group);
    beMessenger_send(m, message);
}

/* Start a delayed call: take the last (free) entry, or a new one if all are in use. */
#ifdef NON_MATCHING
/* Nonmatching, registers only: the original keeps group / info / ramNo in
   s3 / s1 / s0 and the pool / entry in s2. */
void beMessenger_delaySelect(beMessenger* m, const char* group, const char* info, s32 ramNo) {
    ResetArgs(m);
    {
        s32 last = m->_delayStack->_count - 1;
        beMessengerDelayRef d = ((beMessengerDelay**)m->_delayStack->_data)[last];

        if (d->_isUse) {
            void* pool = GetPool(m);

            igUnref(d.p);
            d.p = func_00358950(pool);
            func_00301E30(m->_delayStack, d);
        }
        SetDelayGroup(d, group);
        SetDelayInfo(d, info);
        d->_ramNo = ramNo;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/beMessenger", beMessenger_delaySelect);
#endif

/* Finish a delayed call: message, send time (delay in frames: 1/60 s in ns)
   and the current arguments; then insert it in time order. */
#ifdef NON_MATCHING
/* Nonmatching: register allocation, the s64 multiply's operand order and the
   insertion search loop (offset vs pointer induction). */
void beMessenger_delaySend(beMessenger* m, const char* message, s64 delay) {
    s32 top = m->_delayStack->_count - 1;
    beMessengerDelayRef d = ((beMessengerDelay**)m->_delayStack->_data)[top];

    d->_sendTime = ((igInsightCore*)m->_insight)->_worldTime + delay * 16666666;
    {
        beMessengerArgDataRef tmp;

        tmp = m->_argData;
        ARGDATA(m) = d->_argData;
        ARGDATA(d) = tmp;
    }
    SetDelayMessage(d, message);
    d->_isUse = true;
    {
        s32 last = m->_delayStack->_count - 1;
        s32 n;
        s32 i;

        ObjListRemove(m->_delayStack, last);
        n = m->_delayStack->_count;
        for (i = 0; i < n; i++) {
            beMessengerDelay* o = ((beMessengerDelay**)m->_delayStack->_data)[i];

            if (!o->_isUse || d->_sendTime < o->_sendTime) {
                break;
            }
        }
        ObjListInsert(m->_delayStack, i, d);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/beMessenger", beMessenger_delaySend);
#endif

void beMessenger_delayCall(beMessenger* m, const char* group, const char* info, s32 ramNo, const char* message, s64 delay) {
    beMessenger_delaySelect(m, group, info, ramNo);
    beMessenger_delaySend(m, message, delay);
}

void beMessenger_delayCallGroup(beMessenger* m, const char* group, const char* message, s64 delay) {
    beMessenger_delaySelect(m, group, NULL, -1);
    beMessenger_delaySend(m, message, delay);
}

void beMessenger_delaySelectGroup(beMessenger* m, const char* group) {
    beMessenger_delaySelect(m, group, NULL, -1);
}

/* Save the current arguments, switching to a fresh set. */
void beMessenger_push(beMessenger* m) {
    {
        beMessengerArgDataRef tmp;
        s32 last;

        tmp = m->_argData;
        last = m->_pushPopStack->_count - 1;
        ARGDATA(m) = ((beMessengerArgData**)m->_pushPopStack->_data)[last];
        ObjListRemove(m->_pushPopStack, m->_pushPopStack->_count - 1);
        ObjListInsert(m->_pushPopStack, 0, tmp);
    }
    m->_pushPopCount++;
}

/* Go back to the saved arguments. */
void beMessenger_pop(beMessenger* m) {
    beMessenger_clearArgs(m->_argData);
    {
        beMessengerArgDataRef tmp;

        tmp = m->_argData;
        ARGDATA(m) = ((beMessengerArgData**)m->_pushPopStack->_data)[0];
        ObjListRemove(m->_pushPopStack, 0);
        func_00301E30(m->_pushPopStack, tmp);
    }
    m->_pushPopCount--;
}

/* Argument lists. */

void beMessenger_setRunCheck(beMessenger* m, bool v) {
    m->_argData->_isRunCheck = v;
}

s32 beMessenger_getBoolCount(beMessenger* m) {
    return m->_argData->_boolList->_count;
}

void beMessenger_setBoolCount(beMessenger* m, s32 n) {
    igDataList* list = m->_argData->_boolList;

    if (n >= 0) {
        if (n <= list->_capacity) {
            list->_count = n;
        } else {
            func_002D6790(list, n, 1);
        }
    }
}

void beMessenger_pushBool(beMessenger* m, bool v) {
    s32 n;
    igDataList* list = m->_argData->_boolList;

    n = list->_count;
    if (n < list->_capacity) {
        list->_count = n + 1;
    } else {
        func_002D6790(list, n + 1, 1);
    }
    ((bool*)list->_data)[n] = v;
}

void beMessenger_clearBools(beMessenger* m) {
    m->_argData->_boolList->_count = 0;
}

void beMessenger_setBool(beMessenger* m, s32 i, bool v) {
    igDataList* list = m->_argData->_boolList;

    if (list->_count != 0 && i >= 0 && i < list->_count) {
        ((bool*)list->_data)[i] = v;
    }
}

bool beMessenger_getBool(beMessenger* m, s32 i) {
    bool* p = (bool*)m->_argData->_boolList->_data;

    return p[i];
}

s32 beMessenger_getIntCount(beMessenger* m) {
    return m->_argData->_intList->_count;
}

void beMessenger_pushInt(beMessenger* m, s32 v) {
    s32 n;
    igDataList* list = m->_argData->_intList;

    n = list->_count;
    if (n < list->_capacity) {
        (*(volatile s32*)&list->_count)++;
    } else {
        func_002D6790(list, n + 1, 4);
    }
    ((s32*)list->_data)[n] = v;
}

void beMessenger_clearInts(beMessenger* m) {
    m->_argData->_intList->_count = 0;
}

void beMessenger_setInt(beMessenger* m, s32 i, s32 v) {
    igDataList* list = m->_argData->_intList;

    if (list->_count != 0 && i >= 0 && i < list->_count) {
        ((s32*)list->_data)[i] = v;
    }
}

s32 beMessenger_getInt(beMessenger* m, s32 i) {
    return ((s32*)m->_argData->_intList->_data)[i];
}

s32 beMessenger_getFloatCount(beMessenger* m) {
    return m->_argData->_floatList->_count;
}

void beMessenger_pushFloat(beMessenger* m, f32 v) {
    s32 n;
    igDataList* list = m->_argData->_floatList;

    n = list->_count;
    if (n < list->_capacity) {
        (*(volatile s32*)&list->_count)++;
    } else {
        func_002D6790(list, n + 1, 4);
    }
    ((f32*)list->_data)[n] = v;
}

f32 beMessenger_getFloat(beMessenger* m, s32 i) {
    return ((f32*)m->_argData->_floatList->_data)[i];
}

s32 beMessenger_getLongCount(beMessenger* m) {
    return m->_argData->_longList->_count;
}

void beMessenger_pushLong(beMessenger* m, s64 v) {
    s32 n;
    igDataList* list = m->_argData->_longList;

    n = list->_count;
    if (n < list->_capacity) {
        list->_count = n + 1;
    } else {
        func_002D6790(list, n + 1, 8);
    }
    ((s64*)list->_data)[n] = v;
}

s64 beMessenger_getLong(beMessenger* m, s32 i) {
    return ((s64*)m->_argData->_longList->_data)[i];
}

s32 beMessenger_getVec3fCount(beMessenger* m) {
    return m->_argData->_vec3fList->_count;
}

void beMessenger_pushVec3f(beMessenger* m, const igVec3f* v) {
    s32 n;
    igDataList* list = m->_argData->_vec3fList;

    n = list->_count;
    if (n < list->_capacity) {
        list->_count = n + 1;
    } else {
        func_002D6790(list, n + 1, 0xC);
    }
    ((igVec3f*)list->_data)[n] = *v;
}

void beMessenger_clearVec3fs(beMessenger* m) {
    m->_argData->_vec3fList->_count = 0;
}

/* Returns an igVec3f by value (result pointer in a0). */
void beMessenger_getVec3f(igVec3f* out, beMessenger* m, s32 i) {
    igVec3f* v = &((igVec3f*)m->_argData->_vec3fList->_data)[i];

    out->x = v->x;
    out->y = v->y;
    out->z = v->z;
}

s32 beMessenger_getMatrixCount(beMessenger* m) {
    return m->_argData->_matrix44fList->_count;
}

void beMessenger_pushMatrix(beMessenger* m, const igMatrix44f* v) {
    s32 n;
    igDataList* list = m->_argData->_matrix44fList;

    n = list->_count;
    if (n < list->_capacity) {
        list->_count = n + 1;
    } else {
        func_002D6790(list, n + 1, 0x40);
    }
    func_001908F0(&((igMatrix44f*)list->_data)[n], v);
}

/* Returns an igMatrix44f by value (result pointer in a0). */
void beMessenger_getMatrix(igMatrix44f* out, beMessenger* m, s32 i) {
    func_001908F0(out, &((igMatrix44f*)m->_argData->_matrix44fList->_data)[i]);
}

s32 beMessenger_getObjectCount(beMessenger* m) {
    return m->_argData->_objectList->_count;
}

void beMessenger_pushObject(beMessenger* m, igObject* v) {
    func_00301E30(m->_argData->_objectList, v);
}

void beMessenger_clearObjects(beMessenger* m) {
    igDataList* list = m->_argData->_objectList;
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

igObject* beMessenger_getObject(beMessenger* m, s32 i) {
    return ((igObject**)m->_argData->_objectList->_data)[i];
}

s32 beMessenger_getStringCount(beMessenger* m) {
    return m->_argData->_stringList->_count;
}

void beMessenger_pushString(beMessenger* m, const char* s) {
    char* str = PoolStr(s);

    func_001CA620(m->_argData->_stringList, &str);
    ReleaseStr(str);
}

/* Returns the pooled string after dropping the temporary's reference. */
char* beMessenger_getString(beMessenger* m, s32 i) {
    char* str;
    char* r;

    func_001C64E0(&str, m->_argData->_stringList, i);
    r = str;
    ReleaseStr(r);
    return r;
}

igMetaObject* beMessenger_getMeta(void) {
    return beMessenger_Meta;
}

void beMeterCtrl_prepare(void) {
}

}
