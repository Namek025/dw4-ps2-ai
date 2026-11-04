#include "game.h"

/* beNDMWWindow: helpers that drive text / model windows through messages (MWCC C++). */
extern "C" {

extern char D_00518700[]; /* "TEXTCTL" */
extern char D_00518708[]; /* "ALIGN" */
extern char D_00518710[]; /* "TEXTSET" */
extern char D_00518718[]; /* "SET" */
extern char D_00518720[]; /* "SWITCH" */
extern char D_00518728[]; /* "%d" */
extern char D_00518730[]; /* "CLR" */
extern char D_00518738[]; /* "MODELCTRL" */
extern char D_00518748[]; /* "ISRUN" */
extern char D_00518750[]; /* "GENERATE" */
extern igVec3f D_004E1058;          /* zero vector */
extern s32 D_0058A220;
extern igMetaObject* beLua_Meta;
extern igMetaObject* beMessenger_Meta;
extern igMetaObject* bePadManager_Meta;
void beMessenger_select(beMessenger* m, const char* group, const char* info, s32 ramNo);
void beMessenger_send(beMessenger* m, const char* message);
void beMessenger_call(beMessenger* m, const char* group, const char* info, s32 ramNo, const char* message);
void beMessenger_pushString(beMessenger* m, const char* s);
void beMessenger_pushObject(beMessenger* m, igObject* v);
void beMessenger_pushVec3f(beMessenger* m, const igVec3f* v);
s32 beMessenger_getBoolCount(beMessenger* m);
bool beMessenger_getBool(beMessenger* m, s32 i);
void func_001CA620(igStringRefList* list, char** str);            /* append */
void func_001C64E0(char** out, igStringRefList* list, s32 index); /* returns a pooled string by value */
void func_002D6CF0(igDataList* list, s32 index);                  /* remove */
void* func_00236F80(igInsightCore* insight, igMetaObject* meta);  /* manager of that class */
void func_00389FF0(beLua* lua, const char* name, s32);
s32 sprintf(char* buf, const char* fmt, ...);
void beNDMWWindow_clearText(beNDMWWindow* w, const char* type, const char* name);
void beNDMWWindow_setText(beNDMWWindow* w, const char* type, const char* name, const char* text, const char* align);
void beNDMWWindow_playModel(beNDMWWindow* w, const char* info, s32 ramNo, const char* seq, const char* closeSeq, const igVec3f* pos);
void beNDMWWindow_callModel(beNDMWWindow* w, const char* info, s32 ramNo, const char* seq);
void beNDMWWindow_closeAll(beNDMWWindow* w);

/* beNDMWWindow's virtuals: only slot +0x5C is used here. */
class beNDMWWindowV {
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
    virtual void close(); /* vtable +0x5C */
};

IG_DYNAMIC_CAST(beLua)

static inline void CloseSubs(beNDMWWindow* w) {
    s32 n = w->_subWindow->_count;
    s32 i;

    for (i = 0; i < n; i++) {
        beNDMWWindow* sub = ((beNDMWWindow**)w->_subWindow->_data)[i];

        if (sub->_st0 < 2) {
            sub->_st0 = 2;
            sub->_st3 = 0;
            sub->_st2 = 0;
            sub->_st1 = 0;
        }
    }
}

/* -- */

/* Show text: TEXTCTL ALIGN / TEXTSET / SWITCH SET, remembered for closing. */
void beNDMWWindow_setText(beNDMWWindow* w, const char* type, const char* name, const char* text, const char* align) {
    beMessenger_select(w->_messenger, D_00518700, NULL, -1);
    beMessenger_pushString(w->_messenger, type);
    beMessenger_pushString(w->_messenger, name);
    beMessenger_pushString(w->_messenger, align);
    beMessenger_send(w->_messenger, D_00518708);
    beMessenger_select(w->_messenger, D_00518700, NULL, -1);
    beMessenger_pushString(w->_messenger, type);
    beMessenger_pushString(w->_messenger, name);
    beMessenger_pushString(w->_messenger, text);
    beMessenger_send(w->_messenger, D_00518710);
    beMessenger_select(w->_messenger, D_00518700, NULL, -1);
    beMessenger_pushString(w->_messenger, type);
    beMessenger_pushString(w->_messenger, name);
    beMessenger_pushString(w->_messenger, D_00518718);
    beMessenger_send(w->_messenger, D_00518720);
    APPEND_STR(w->_textCloseTypeList, type)
    APPEND_STR(w->_textCloseNameList, name)
}

/* Same with a number. */
void beNDMWWindow_setTextNumber(beNDMWWindow* w, const char* type, const char* name, s32 value, const char* align) {
    char buf[16];

    sprintf(buf, D_00518728, value);
    beNDMWWindow_setText(w, type, name, buf, align);
}

/* Same with a text object. */
void beNDMWWindow_setTextObject(beNDMWWindow* w, const char* type, const char* name, igObject** text, const char* align) {
    beMessenger_select(w->_messenger, D_00518700, NULL, -1);
    beMessenger_pushString(w->_messenger, type);
    beMessenger_pushString(w->_messenger, name);
    beMessenger_pushString(w->_messenger, align);
    beMessenger_send(w->_messenger, D_00518708);
    beMessenger_select(w->_messenger, D_00518700, NULL, -1);
    beMessenger_pushString(w->_messenger, type);
    beMessenger_pushString(w->_messenger, name);
    beMessenger_pushObject(w->_messenger, *text);
    beMessenger_send(w->_messenger, D_00518710);
    beMessenger_select(w->_messenger, D_00518700, NULL, -1);
    beMessenger_pushString(w->_messenger, type);
    beMessenger_pushString(w->_messenger, name);
    beMessenger_pushString(w->_messenger, D_00518718);
    beMessenger_send(w->_messenger, D_00518720);
    APPEND_STR(w->_textCloseTypeList, type)
    APPEND_STR(w->_textCloseNameList, name)
}

/* Run a Lua function of the window's beLua. */
void beNDMWWindow_callLua(beNDMWWindow* w, const char* name, s32 arg) {
    beLua* lua = beLua_cast(func_00236F80(w->_insight, beLua_Meta));

    D_0058A220 = arg;
    func_00389FF0(lua, name, 0);
}

void beNDMWWindow_clearText(beNDMWWindow* w, const char* type, const char* name) {
    beMessenger_select(w->_messenger, D_00518700, NULL, -1);
    beMessenger_pushString(w->_messenger, type);
    beMessenger_pushString(w->_messenger, name);
    beMessenger_pushString(w->_messenger, D_00518730);
    beMessenger_send(w->_messenger, D_00518720);
}

/* Clear every text shown with beNDMWWindow_setText. */
void beNDMWWindow_clearAllText(beNDMWWindow* w) {
    s32 n = w->_textCloseTypeList->_count;
    s32 i;

    for (i = 0; i < n; i++) {
        char* tmp;
        char* tmp2;
        char* type;
        char* name;

        func_001C64E0(&tmp, w->_textCloseTypeList, i);
        type = tmp;
        ReleaseStr(type);
        func_001C64E0(&tmp2, w->_textCloseNameList, i);
        name = tmp2;
        ReleaseStr(name);
        beNDMWWindow_clearText(w, type, name);
    }
    ClearStrList(w->_textCloseTypeList);
    ClearStrList(w->_textCloseNameList);
}

/* Play seq on a model (GENERATE it at pos if it is not running); with
   closeSeq, remember it to play closeSeq when the window closes. */
void beNDMWWindow_playModel(beNDMWWindow* w, const char* info, s32 ramNo, const char* seq, const char* closeSeq, const igVec3f* pos) {
    beMessenger_call(w->_messenger, D_00518738, info, ramNo, D_00518748);
    if (beMessenger_getBool(w->_messenger, 0)) {
        beMessenger_call(w->_messenger, D_00518738, info, ramNo, seq);
    } else {
        igVec3f v;

        beMessenger_select(w->_messenger, D_00518738, info, ramNo);
        beMessenger_pushString(w->_messenger, seq);
        v = *pos;
        beMessenger_pushVec3f(w->_messenger, &v);
        beMessenger_send(w->_messenger, D_00518750);
    }
    if (closeSeq != NULL) {
        APPEND_STR(w->_modelCloseInfoNameList, info)
        {
            s32 k;
            igDataList* list = w->_modelCloseRamNoList;

            k = list->_count;
            if (k < list->_capacity) {
                (*(volatile s32*)&list->_count)++;
            } else {
                func_002D6790(list, k + 1, 4);
            }
            ((s32*)list->_data)[k] = ramNo;
        }
        APPEND_STR(w->_modelCloseSeqNameList, closeSeq)
    }
}

void beNDMWWindow_playModelAtOrigin(beNDMWWindow* w, const char* info, s32 ramNo, const char* seq, const char* closeSeq) {
    igVec3f pos;

    pos.x = D_004E1058.x;
    pos.y = D_004E1058.y;
    pos.z = D_004E1058.z;

    beNDMWWindow_playModel(w, info, ramNo, seq, closeSeq, &pos);
}

void beNDMWWindow_callModel(beNDMWWindow* w, const char* info, s32 ramNo, const char* seq) {
    beMessenger_call(w->_messenger, D_00518738, info, ramNo, seq);
}

#ifdef NON_MATCHING
/* Nonmatching, registers only: the index * 4 induction variable and the two
   strings rotate through s0..s2 differently. */
/* Play the remembered close sequences. */
void beNDMWWindow_playCloseModels(beNDMWWindow* w) {
    s32 n;
    s32 i;
    char* info;
    char* seq;
    s32 ramNo;
    char* tmp2;
    char* tmp;

    n = w->_modelCloseInfoNameList->_count;
    for (i = 0; i < n; i++) {

        func_001C64E0(&tmp, w->_modelCloseInfoNameList, i);
        info = tmp;
        ReleaseStr(info);
        ramNo = ((s32*)w->_modelCloseRamNoList->_data)[i];
        func_001C64E0(&tmp2, w->_modelCloseSeqNameList, i);
        seq = tmp2;
        ReleaseStr(seq);
        beNDMWWindow_callModel(w, info, ramNo, seq);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWWindow", beNDMWWindow_playCloseModels);
#endif

#ifdef NON_MATCHING
/* Nonmatching, registers only: same rotation as beNDMWWindow_playCloseModels. */
/* Whether closing is done: no close sequence still running and no sub window
   left; then forget the close sequences. */
bool beNDMWWindow_isCloseDone(beNDMWWindow* w) {
    s32 n = w->_modelCloseInfoNameList->_count;
    s32 i;

    for (i = 0; i < n; i++) {
        char* tmp;
        char* tmp2;
        char* info;
        s32 ramNo;

        func_001C64E0(&tmp, w->_modelCloseInfoNameList, i);
        info = tmp;
        ReleaseStr(info);
        ramNo = ((s32*)w->_modelCloseRamNoList->_data)[i];
        func_001C64E0(&tmp2, w->_modelCloseSeqNameList, i);
        ReleaseStr(tmp2);
        beMessenger_call(w->_messenger, D_00518738, info, ramNo, D_00518748);
        if (beMessenger_getBoolCount(w->_messenger) > 0 && beMessenger_getBool(w->_messenger, 0)) {
            return false;
        }
    }
    if (w->_subWindow->_count != 0) {
        return false;
    }
    ClearStrList(w->_modelCloseInfoNameList);
    w->_modelCloseRamNoList->_count = 0;
    ClearStrList(w->_modelCloseSeqNameList);
    return true;
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWWindow", beNDMWWindow_isCloseDone);
#endif

void beNDMWWindow_closeSubWindows(beNDMWWindow* w) {
    CloseSubs(w);
}

void beNDMWWindow_setup(beNDMWWindow* w, beNDMWWindow* mother, igInsightCore* insight, s32 playerNo, s32 entCode) {
    igIncRef(mother);
    igUnref(w->_mother);
    w->_mother = mother;
    w->_insight = insight;
    w->_messenger = (beMessenger*)func_00236F80(insight, beMessenger_Meta);
    w->_padManager = (bePadManager*)func_00236F80(insight, bePadManager_Meta);
    w->_playerNo = playerNo;
    w->_entCode = entCode;
}

/* Close this window and its sub windows; drop the closed sub windows. */
void beNDMWWindow_closeAll(beNDMWWindow* w) {
    s32 n;
    s32 i;

    if (w->_isRun) {
        ((beNDMWWindowV*)w)->close();
    }
    if (!w->_isRun || w->_st0 >= 2) {
        CloseSubs(w);
    }
    n = w->_subWindow->_count;
    for (i = 0; i < n; i++) {
        igDataList* list;
        beNDMWWindow* sub;

        beNDMWWindow_closeAll(((beNDMWWindow**)w->_subWindow->_data)[i]);
        list = w->_subWindow;
        sub = ((beNDMWWindow**)list->_data)[i];
        if (!sub->_isRun) {
            igUnref(sub);
            func_002D6CF0(list, i);
            i--;
            n--;
            ((igObject**)list->_data)[list->_count] = NULL;
        }
    }
}

}
