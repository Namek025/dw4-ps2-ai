#include "game.h"

extern "C" {
extern igMetaObject* beCri_Meta;
void func_0037D2F0(beCri* cri);
void func_00236C40(igInsightCore* insight, void* o);
extern igMetaObject* beNDMWAfsSetup_Meta;
extern igMetaObject* beNDMWAfsStageLoad_Meta;
}

/* Unref every element, then empty the list (same as beBaseInfoManager's). */
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

extern "C" void beNDMWAfsSetup_registerToInsight(void* self) {
}

extern "C" void beNDMWAfsSetup_unRegisterFromInsight(void* self) {
}

extern "C" void beNDMWAfsSetup_begin(void* self) {
}

void beNDMWAfsSetup_end(beNDMWAfsSetup* mgr) {
    func_0037D2F0((beCri*)func_00236F80(mgr->_insight, beCri_Meta));
}

INCLUDE_ASM("asm/nonmatchings/beNDMWAfsSetup", beNDMWAfsSetup_appendInfo);

extern "C" void beNDMWAfsSetup_removeAllInfos(beNDMWAfsSetup* mgr) {
    ObjListClear(mgr->_infoList);
    if ((mgr->_infoList->_count > 0) == false) {
        func_002D6680(mgr->_infoList, 0, 4);
    }
    func_00236C40(mgr->_insight, mgr);
}

extern "C" void beNDMWAfsSetup_update(void* self) {
}

void func_003CDEC0(void) {
}

igMetaObject* beNDMWAfsStageLoad_getMeta(void) {
    return beNDMWAfsStageLoad_Meta;
}

igMetaObject* beNDMWAfsSetup_getMeta(void) {
    return beNDMWAfsSetup_Meta;
}

INCLUDE_ASM("asm/nonmatchings/beNDMWAfsSetup", func_003CDEF0);

/* Panel position on screen from its player number (x = 150 * player - 260, y = -160). */
void func_003CE4C0(beNDMWPanelObject* o, igVec3f* pos) {
    pos->x = 150.0f * o->_playerNo - 260.0f;
    pos->y = -160.0f;
    pos->z = 0.0f;
}

extern "C" void func_003CDEF0(beNDMWPanelObject* o, s32 kind, s32 idx, s32 a3, s32 a4);

void func_003CE500(beNDMWPanelObject* o, const char* info, const char* message, igVec3f* pos, const char* str) {
    igVec3f v;

    beMessenger_select(o->_messenger, "MODELCTRL", info, o->_playerNo);
    if (pos != NULL) {
        v.x = pos->x;
        v.y = pos->y;
        v.z = pos->z;
        beMessenger_pushVec3f(o->_messenger, &v);
    }
    if (str != NULL) {
        beMessenger_pushString(o->_messenger, str);
    }
    beMessenger_send(o->_messenger, message);
}

extern char* D_004CC690[][4];

void func_003CE5B0(beNDMWPanelObject* o, s32 isGen, bool flag) {
    igVec3f pos;
    s32 kazu;
    s32 idx;

    kazu = o->_listKazu;
    switch (kazu) {
    case 0:
    case 1:
    case 2:
        idx = 2;
        break;
    case 3:
        idx = 1;
        break;
    default:
        idx = 0;
        break;
    }
    o->_lineMode = idx;
    if (isGen != 0) {
        func_003CE4C0(o, &pos);
        func_003CE500(o, "i_wzslbase00", "GENERATE", &pos, D_004CC690[o->_page][idx]);
    } else {
        func_003CE500(o, "i_wzslbase00", D_004CC690[o->_page][idx], NULL, NULL);
    }
    if (flag == false) {
        beMessenger_selectGroup(o->_messenger, "SOUND");
        beMessenger_pushString(o->_messenger, "WINDOW_OPEN");
        beMessenger_send(o->_messenger, "PLAY");
    }
}

extern char* D_004CC69C[][4];

#ifdef NON_MATCHING
/* Nonmatching: the move a0,s0 before the bgez sits after the idx < 0 block instead of in its delay slot (distance 2). */
void func_003CE700(beNDMWPanelObject* o) {
    s32 i;
    s32 idx;
    s32 v;

    func_003CE500(o, "i_wzslbase00", D_004CC69C[o->_page][0], NULL, NULL);
    i = 0;
    do {
        idx = i + o->_playerNo * 4;
        func_003CDEF0(o, 0, idx, 0, 0);
        func_003CDEF0(o, 2, idx, 0, 0);
        v = idx >> 2;
        if (i == 0) {
            if (idx < 0) {
                v = (idx + 3) >> 2;
            }
            func_003CDEF0(o, 1, v, 0, 0);
        }
        i += 1;
    } while (i < 4);
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWAfsSetup", func_003CE700);
#endif

void func_003CE7E0(beNDMWPanelObject* o) {
    s32 cursor;
    s32 offset;

    cursor = o->_cursor;
    if (cursor != 0) {
        o->_cursor = cursor - 1;
        beMessenger_selectGroup(o->_messenger, "SOUND");
        beMessenger_pushString(o->_messenger, "CURSOR_MOVE");
        beMessenger_send(o->_messenger, "PLAY");
        return;
    }
    offset = o->_offset;
    if (offset != 0) {
        o->_offset = offset - 1;
        beMessenger_selectGroup(o->_messenger, "SOUND");
        beMessenger_pushString(o->_messenger, "CURSOR_MOVE");
        beMessenger_send(o->_messenger, "PLAY");
    }
}

void func_003CE890(beNDMWPanelObject* o) {
    s32 cursor;
    s32 count;
    s32 offset;

    count = o->_listKazu;
    if (count != 0 && (cursor = o->_cursor, cursor != count - 1)) {
        if (cursor != 3) {
            o->_cursor = cursor + 1;
            beMessenger_selectGroup(o->_messenger, "SOUND");
            beMessenger_pushString(o->_messenger, "CURSOR_MOVE");
            beMessenger_send(o->_messenger, "PLAY");
            return;
        }
        offset = o->_offset;
        if (offset + 4 < count) {
            o->_offset = offset + 1;
            beMessenger_selectGroup(o->_messenger, "SOUND");
            beMessenger_pushString(o->_messenger, "CURSOR_MOVE");
            beMessenger_send(o->_messenger, "PLAY");
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/beNDMWAfsSetup", func_003CE960);

extern "C" s64 __divdi3(s64 a, s64 b);
extern "C" s32 func_00447C10(s32 item, void* mgr, const char* name);
extern "C" s32 func_00447CB0(s32 item, void* mgr, const char* name);

void func_003CEED0(beNDMWPanelObjectWaza* o, s32* label, s64* delay) {
    s32 off;
    s32 slot;
    beDataObjInt* mp;
    s32 cost;
    s32 mmp;

    *delay = 0;
    *label = 0;
    off = (o->_cursor + o->_offset) * 4;
    if (*(s32*)((char*)o->_listColor->_data + off) == 0) {
        slot = *(s32*)((char*)o->_listFolder->_data + off);
        mp = beDataObjList_getInt(o->_playerDataList, "MP");
        cost = func_00447C10(slot, o->_manager, "MP");
        if (mp->_data >= cost) {
            *delay = (s64)(s32)func_00447C10(slot, o->_manager, "DELAY");
            *delay = __divdi3(*delay * 1000000000, 60);
            *label = func_00447CB0(slot, o->_manager, "LABEL");
            mp->_data = (s32)(mp->_data - cost);
            mmp = beDataObjList_getInt(o->_playerDataList, "MMP")->_data;
            beMessenger_delaySelect(o->_messenger, "METERCTRL", "mpmt", o->_playerNo);
            beMessenger_pushFloat(o->_messenger, (f32)((s32)(mp->_data * 100) / mmp));
            beMessenger_delaySend(o->_messenger, "SETPERCENT", 1);
            beMessenger_delaySelect(o->_messenger, "NUMBERCTRL", "mpnm", o->_playerNo);
            beMessenger_pushInt(o->_messenger, mp->_data);
            beMessenger_delaySend(o->_messenger, "SETVALUE", 1);
            return;
        }
        beMessenger_selectGroup(o->_messenger, "SOUND");
        beMessenger_pushString(o->_messenger, "CURSOR_NG");
        beMessenger_send(o->_messenger, "PLAY");
        return;
    }
    beMessenger_selectGroup(o->_messenger, "SOUND");
    beMessenger_pushString(o->_messenger, "CURSOR_NG");
    beMessenger_send(o->_messenger, "PLAY");
}

INCLUDE_ASM("asm/nonmatchings/beNDMWAfsSetup", func_003CF150);

extern f32 D_004E1058;
extern f32 D_004E105C;
extern f32 D_004E1060;

/* List item of a waza panel: vtable slot 22 (+0x60) checks whether the item can be used. */
class beNDMWPanelWazaEntryV {
public:
    IG_VPAD_22
    virtual bool canUse(beBaseInfoManager* mgr, s32 playerNo); /* +0x60 */
    s32 unk4;
    s32 unk8;
    s32 _kind;  /* 0xC */
    s32 unk10;
    s32 _count;  /* 0x14 */
};

void func_003CF6A0(beNDMWPanelObjectWaza* o, s32* label, s64* delay) {
    beNDMWPanelWazaEntryV* item;
    s32 count;
    s32 off;

    *delay = 0;
    *label = 0;
    off = (o->_cursor + o->_offset) * 4;
    if (*(s32*)((char*)o->_listColor->_data + off) == 0) {
        item = *(beNDMWPanelWazaEntryV**)((char*)o->_listFolder->_data + off);
        count = item->_count;
        if (count != 0) {
            if (item->canUse(o->_manager, o->_playerNo) == false) {
                *delay = 0x3B9ACA00;
            } else {
                *delay = -1;
            }
            item->_count = count - 1;
            switch (item->_kind) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 7:
            case 9:
                {
                    igVec3f v;

                    beMessenger_delaySelect(o->_messenger, "PLAYER", NULL, o->_playerNo);
                    beMessenger_pushString(o->_messenger, "x_250healhit00");
                    beMessenger_pushString(o->_messenger, "SQ_HEALEFONLY");
                    beMessenger_pushFloat(o->_messenger, 1.0f);
                    v.x = D_004E1058;
                    v.y = D_004E105C;
                    v.z = D_004E1060;
                    beMessenger_pushVec3f(o->_messenger, &v);
                    beMessenger_pushBool(o->_messenger, false);
                    beMessenger_pushBool(o->_messenger, true);
                    beMessenger_delaySend(o->_messenger, "SETEFFECT", 1);
                }
                return;
            case 6:
                {
                    igVec3f v;

                    beMessenger_delaySelect(o->_messenger, "PLAYER", NULL, o->_playerNo);
                    beMessenger_pushString(o->_messenger, "x_250curehit00");
                    beMessenger_pushString(o->_messenger, "SQ_CUREEFONLY");
                    beMessenger_pushFloat(o->_messenger, 1.0f);
                    v.x = D_004E1058;
                    v.y = D_004E105C;
                    v.z = D_004E1060;
                    beMessenger_pushVec3f(o->_messenger, &v);
                    beMessenger_pushBool(o->_messenger, false);
                    beMessenger_pushBool(o->_messenger, true);
                    beMessenger_delaySend(o->_messenger, "SETEFFECT", 1);
                }
                return;
            case 8:
            case 10:
            case 11:
                break;
            }
        } else {
            beMessenger_selectGroup(o->_messenger, "SOUND");
            beMessenger_pushString(o->_messenger, "CURSOR_NG");
            beMessenger_send(o->_messenger, "PLAY");
        }
    } else {
        beMessenger_selectGroup(o->_messenger, "SOUND");
        beMessenger_pushString(o->_messenger, "CURSOR_NG");
        beMessenger_send(o->_messenger, "PLAY");
    }
}

INCLUDE_ASM("asm/nonmatchings/beNDMWAfsSetup", func_003CF960);
