#include "game.h"

extern "C" {

extern igMetaObject* beNDMWMdlPlayer2_Meta;
extern igMetaObject* beNDMWMdlPlayer_Meta;
extern igMetaObject* bePadManager_Meta;
extern igMetaObject* beNDMWGameRam_Meta;
extern igMetaObject* igViewerSceneInfoManager_Meta;
void beBaseInfoManager_begin(beBaseInfoManager* mgr);
extern char D_00516028[]; /* "KIND_PLAYER" */
extern char D_00516038[]; /* "PLAYER" */
extern char D_005165B0[]; /* "KIND_PLAYER2" */
extern char D_005165C0[]; /* "PLAYER2" */

void beNDMWMdlPlayer_begin(beNDMWMdlPlayer* mgr) {
    beBaseInfoManager_begin(mgr);
    mgr->_gameRam = (beNDMWGameRam*)func_00236F80(mgr->_insight, beNDMWGameRam_Meta);
    mgr->_padManager = (bePadManager*)func_00236F80(mgr->_insight, bePadManager_Meta);
    mgr->_viewerSIManager =
        (igViewerSceneInfoManager*)func_00236F80(mgr->_insight, igViewerSceneInfoManager_Meta);
}

s32 func_003D2290(beBaseInfoManager* mgr, const char* kind) {
    return strcmp(kind, D_00516028) == 0;
}

void func_003D22C0(beBaseInfoManager* mgr, beBaseInfoRam* ram) {
    beMessenger_entryRam(mgr->_messenger, ram, D_00516038);
}

extern igMetaObject* D_00585BE0; /* meta used for lookups by name */
extern bool D_00554C14;
extern void* D_00587110;
extern char D_00516110[]; /* "ONE" */
extern char D_00516148[]; /* "NDMWPANEL" */
extern char D_005164B0[]; /* "REMOVE" */
igNamedObject* func_003022C0(igObjectList* list, igMetaObject* meta, const char* name); /* find by name */
beBaseInfoDataList* func_0036AE80(void* pool); /* new beBaseInfoDataList */

}
IG_REF(beBaseInfoDataList);

static inline void* PoolOf2(void* o) {
    void* a = func_00301000(o);

    return D_00554C14 ? a : func_002F88E0(D_00587110);
}

/* The ram's data list, made on first use. */
static inline void MakeDataList(beBaseInfoRam* ram) {
    if (ram->_dataList == NULL) {
        beBaseInfoDataListRef l(func_0036AE80(PoolOf2(ram)), IG_ADOPT);

        igAssign(ram->_dataList, l.p);
    }
}
extern "C" {
extern char D_004E0858[]; /* "" */
extern char D_00516290[]; /* "SQ_WAIT" */
bePadData* bePadManager_getPad(bePadManager* mgr, s32 n);

static inline const char* NameOf(igNamedObject* o) {
    return o->_name ? o->_name : D_004E0858;
}

static inline void SetStrH(char*& dst, const char* s) {
    char* str = func_002EAF60(GetStrPool(), s);

    ReleaseStr(dst);
    dst = str;
}

}

/* A pooled string held for a scope (igStringRef: lives on the stack). */
struct StrRefH {
    char* p;
    StrRefH(const char* s) : p(s == NULL ? NULL : func_002EAF60(GetStrPool(), s)) {}
    ~StrRefH() { ReleaseStr(p); }
};

/* C++ overload: returns the string by value (result slot in a0); mwsplice demangles the name. */
StrRefH func_001C64E0(igStringRefList* list, s32 i);

IG_REF(igIntList);

extern "C" void* memset(void* p, s32 c, u32 n);

template <class L>
static inline void ListReserve(L* list, s32 n, s32 elemSize) {
    if (list->_count <= n) {
        func_002D6680(list, n, elemSize);
    }
}

template <class L>
static inline void ListSetCount(L* list, s32 n, s32 elemSize) {
    if (n >= 0) {
        if (n <= list->_capacity) {
            list->_count = n;
        } else {
            func_002D6790(list, n, elemSize);
        }
    }
}

template <class L, class T>
static inline void ListFill(L* list, const T& v) {
    s32 n = list->_count;
    T* p = (T*)list->_data;
    s32 i;

    for (i = 0; i < n; i++) {
        p[i] = v;
    }
}

template <class L>
static inline void ListZero(L* list) {
    if (list->_count != 0) {
        memset(list->_data, 0, list->_count * 4);
    }
}

static inline void IntListSet(igIntList* list, s32 i, const s32& v) {
    if (list->_count != 0 && i >= 0 && i < list->_count) {
        ((s32*)list->_data)[i] = v;
    }
}

/* beModelCtrl's virtuals. */
class beModelCtrlV {
public:
    IG_VPAD_33
    virtual void setAction(beBaseInfoRam* ram, const char* name); /* +0x8C */
};

extern "C" {

}

IG_REF(beModelCtrlHITBOX);

static inline void SetVec3(igVec3f& v, f32 x, f32 y, f32 z) {
    v.x = x;
    v.y = y;
    v.z = z;
}

extern "C" {
extern char D_00516118[]; /* "SQ_JUMP" */
extern char D_00516120[]; /* "SQ_GUARD" */
extern char D_00516130[]; /* "SQ_FALL" */
extern char D_00516138[]; /* "SQ_ATCK3" */
extern char D_00516158[]; /* "ISCHECK" */
extern char D_00516160[]; /* "SQ_ITEM" */
extern char D_00516168[]; /* "SQ_WAZA" */
extern char D_00516170[]; /* "GENEEMGHEAL" */
extern char D_00516180[]; /* "SOUND" */
extern char D_00516188[]; /* "CURSOR_NG" */
extern char D_00516198[]; /* "PLAY" */
extern char D_005161A0[]; /* "GENERATEITEM" */
extern char D_005161B0[]; /* "GENERATEWAZA" */
extern char D_005161C0[]; /* "GENERATESTAT" */
extern char D_005161D0[]; /* "Set" */
extern char D_005161D8[]; /* "DROPITEM" */
extern char D_005161E8[]; /* "ACTION1" */
extern char D_005161F0[]; /* "ATDELAY" */
extern char D_005161F8[]; /* "SQ_ATCK2" */
extern char D_00516208[]; /* "SQ_ATCK1" */
extern char D_00516218[]; /* "ATTACKDELAY" */
extern char D_00516228[]; /* "MODELCTRL" */
extern char D_00516238[]; /* "i_stpnl05" */
extern char D_00516248[]; /* "ISRUN" */
extern char D_00516250[]; /* "SUBWND" */
extern char D_00516258[]; /* "STATUS" */
extern char D_00516260[]; /* "GENERATE" */
extern s32 D_004CC730[16]; /* stick direction bits -> turn timer slot (-1: none) */
extern const s32 D_004CC770;     /* 30: frames a turn stays counted */
extern char* D_004CC780[];       /* "SQ_DLTM01".."SQ_DLTM04": attack delay meter */
extern igVec3f D_004E1098;       /* (0, 0, 0) */
void* memset(void* p, s32 c, u32 n);
void func_00190380(igMatrix44f* m); /* identity */
void func_00192F50(igVec3f* out, const igVec3f* v, const igMatrix44f* m); /* transform a point */
f32 func_00123640(f32 y, f32 x);    /* atan2f */
beModelCtrlHITBOX* func_00354A60(void* pool); /* new beModelCtrlHITBOX */
void func_0039A500(beModelCtrlSubCom* sub, beModelCtrlHITBOX* box);
void func_00393C80(beNDMWMdlPlayer* mgr, beBaseInfoRam* ram, beNDMWMdlPlayerInfoWork* work, const char* seq, const char* name);
bool func_003A7E90(beBaseInfoRam* ram, const char* name);        /* timer done? */
void func_003A7A20(beBaseInfoRam* ram, const char* name, s64 ns); /* start a timer */
void beMessenger_setRunCheck(beMessenger* m, bool v);
}

/* C++ overload: returns igResult by value (result slot in a0). */
igResult func_00191F30(igMatrix44f* out, const igMatrix44f* m); /* inverse */

extern "C" {

/* Player control (docs/controls.md): turn timers and facing from the left stick (camera
   relative), guard (square), body attack (double tap of a direction while holding square),
   then the first of: technique panel use, jump (circle), heal (triangle), the item / technique
   / status panels (L1 / R1 / R2), attack (X: in the lobby, a "Set" hitbox to talk), menu
   (START alone). */
void func_003D22F0(beNDMWMdlPlayer* mgr, beBaseInfoRam* ram) {
    beNDMWMdlPlayerInfoWork* work;
    bePadData* pad;
    s32 dir;
    f32 sx;
    f32 sy;
    bool allTurns;
    s32 trg;
    s32 btn;
    bool panel;

    MakeDataList(ram);
    work = (beNDMWMdlPlayerInfoWork*)((beModelCtrlInfoRamData*)func_003022C0(ram->_dataList, D_00585BE0,
                                                                             D_00516110))->_work;
    pad = bePadManager_getPad(mgr->_padManager, ram->_ramNo);
    dir = pad->_lStk;
    sx = pad->_LStick.x;
    sy = pad->_LStick.y;
    allTurns = false;
    if (dir != 0) {
        igIntList* timers = work->_turnAtkTimer;
        s32 slot = D_004CC730[dir];
        s32 n;
        s32 i;

        if (slot != -1) {
            IntListSet(timers, slot, D_004CC770);
        }
        n = 0;
        for (i = 0; i < 8; i++) {
            s32 t = ((s32*)timers->_data)[i];

            if (t > 0) {
                IntListSet(timers, i, t - 1);
                n++;
            }
        }
        if (n >= 8) {
            allTurns = true;
        }
        {
            igMatrix44f m;
            igVec3f cam;
            f32 yaw;

            func_00190380(&m);
            func_00191F30(&m, &mgr->_viewerSIManager->_parameters->_viewMatrix);
            func_00192F50(&cam, &D_004E1098, &m);
            yaw = func_00123640(cam.x, cam.z);
            work->_targetDirection.y = yaw + func_00123640(sx, -sy);
        }
        work->_keyInput = true;
    } else {
        work->_keyInput = false;
    }
    trg = pad->_trigger;
    btn = pad->_button;
    if (!work->_isMainLobby) {
        if (strcmp(work->_seqCodeName, D_00516118) != 0 && (trg & 0x8000)) {
            SetStrH(work->_keyEventName, D_00516120);
        }
        if (btn & 0x8000) {
            if (!(work->_bodyAtkTimer <= 0.0f)) {
                f32 t = work->_bodyAtkTimer - mgr->_system->_frameTimeF;

                if (!(t < 0.0f)) {
                    work->_bodyAtkTimer = t;
                } else {
                    work->_bodyAtkTimer = 0.0f;
                }
            }
            switch (work->_bodyAtkSt0) {
            case 0:
                if (!work->_keyInput) {
                    work->_bodyAtkSt0 = 1;
                }
                break;
            case 1:
                if (work->_keyInput == true) {
                    work->_bodyAtkTimer = 15.0f;
                    work->_bodyAtkPad = dir;
                    work->_bodyAtkSt0 = 2;
                }
                break;
            case 2:
                if (work->_bodyAtkTimer == 0.0f) {
                    work->_bodyAtkSt0 = 0;
                } else if (!work->_keyInput) {
                    work->_bodyAtkSt0 = 3;
                }
                break;
            case 3:
                if (work->_bodyAtkTimer == 0.0f) {
                    work->_bodyAtkSt0 = 0;
                } else if (work->_keyInput) {
                    if (dir == work->_bodyAtkPad && strcmp(work->_seqCodeName, D_00516118) != 0 &&
                        strcmp(work->_seqCodeName, D_00516130) != 0) {
                        SetStrH(work->_keyEventName, D_00516138);
                        work->_ctrlSt0 = 0;
                        work->_ctrlSt1 = 0;
                    }
                    work->_bodyAtkSt0 = 0;
                }
                break;
            }
        } else {
            work->_bodyAtkSt0 = 0;
        }
    }
    if (strcmp(work->_seqCodeName, D_00516118) != 0 && strcmp(work->_seqCodeName, D_00516130) != 0) {
        panel = work->_isPanel;
        if (panel) {
            bool used = false;

            beMessenger_call(mgr->_messenger, D_00516148, NULL, ram->_ramNo, D_00516158);
            if (beMessenger_getBoolCount(mgr->_messenger) == 0 || beMessenger_getBool(mgr->_messenger, 0) == false) {
                work->_isPanel = false;
            } else if (beMessenger_getBool(mgr->_messenger, 2)) {
                if (beMessenger_getBool(mgr->_messenger, 1)) {
                    SetStrH(work->_keyEventName, D_00516160);
                } else {
                    func_00393C80(mgr, ram, work, D_00516168, beMessenger_getString(mgr->_messenger, 0));
                    SetStrH(work->_keyEventName, D_00516168);
                }
                used = true;
            }
            if (used == false) {
            if (trg & 0x2000) {
                SetStrH(work->_keyEventName, D_00516118);
            } else if (trg & 0x1000) {
                beMessenger_select(mgr->_messenger, D_00516148, NULL, ram->_ramNo);
                beMessenger_pushObject(mgr->_messenger, work);
                beMessenger_send(mgr->_messenger, D_00516170);
                if (beMessenger_getBoolCount(mgr->_messenger) != 0) {
                    work->_isPanel = true;
                }
            } else if (!work->_keyInput && (btn & ~8) == 0 && (trg & 8)) {
                beMessenger_selectGroup(mgr->_messenger, D_00516180);
                beMessenger_pushString(mgr->_messenger, D_00516188);
                beMessenger_send(mgr->_messenger, D_00516198);
            }
            }
        } else if (trg & 0x2000) {
            SetStrH(work->_keyEventName, D_00516118);
        } else if (trg & 0x1000) {
            beMessenger_select(mgr->_messenger, D_00516148, NULL, ram->_ramNo);
            beMessenger_pushObject(mgr->_messenger, work);
            beMessenger_send(mgr->_messenger, D_00516170);
            if (beMessenger_getBoolCount(mgr->_messenger) != 0) {
                work->_isPanel = true;
            }
        } else if ((trg & 0x400) && panel == false) {
            beMessenger_select(mgr->_messenger, D_00516148, NULL, ram->_ramNo);
            beMessenger_pushObject(mgr->_messenger, work);
            beMessenger_send(mgr->_messenger, D_005161A0);
            if (beMessenger_getBoolCount(mgr->_messenger) != 0) {
                work->_isPanel = true;
            }
        } else if ((trg & 0x800) && panel == false) {
            beMessenger_select(mgr->_messenger, D_00516148, NULL, ram->_ramNo);
            beMessenger_pushObject(mgr->_messenger, work);
            beMessenger_send(mgr->_messenger, D_005161B0);
            if (beMessenger_getBoolCount(mgr->_messenger) != 0) {
                work->_isPanel = true;
            }
        } else if ((trg & 0x200) && panel == false) {
            beMessenger_select(mgr->_messenger, D_00516148, NULL, ram->_ramNo);
            beMessenger_pushObject(mgr->_messenger, work);
            beMessenger_send(mgr->_messenger, D_005161C0);
            if (beMessenger_getBoolCount(mgr->_messenger) != 0) {
                work->_isPanel = true;
            }
        } else if (trg & 0x4000) {
            if (work->_isMainLobby) {
                beModelCtrlHITBOXRef box(func_00354A60(PoolOf2(mgr)), IG_ADOPT);

                func_002FECB0(box, D_005161D0);
                box->_kind = 60000;
                box->_start = 1.0f;
                box->_length = 1.0f;
                SetVec3(box->_offset, 0.0f, 4.0f, 7.0f);
                SetVec3(box->_boxSize, 3.0f, 3.0f, 3.0f);
                func_0039A500(work->_subCommand, box);
            } else {
                beMessenger_selectGroup(mgr->_messenger, D_005161D8);
                beMessenger_pushInt(mgr->_messenger, ram->_ramNo);
                beMessenger_setRunCheck(mgr->_messenger, true);
                beMessenger_send(mgr->_messenger, D_005161E8);
                if (beMessenger_getBoolCount(mgr->_messenger) == 0 && func_003A7E90(ram, D_005161F0)) {
                    s32 delay;

                    if (allTurns) {
                        SetStrH(work->_keyEventName, D_005161F8);
                        ListZero(work->_turnAtkTimer);
                    } else {
                        SetStrH(work->_keyEventName, D_00516208);
                    }
                    delay = beDataObjList_getInt(((beDataObjList**)mgr->_gameRam->_playerDataListList->_data)[ram->_ramNo],
                                                 D_00516218)->_data;
                    func_003A7A20(ram, D_005161F0, (s64)delay * 1000000000 / 60);
                    beMessenger_call(mgr->_messenger, D_00516228, D_00516238, ram->_ramNo, D_004CC780[(delay - 22) / 4]);
                }
            }
        } else if (!work->_keyInput && (btn & ~8) == 0 && (trg & 8)) {
            beMessenger_call(mgr->_messenger, D_00516148, NULL, -1, D_00516248);
            if (beMessenger_getBoolCount(mgr->_messenger) != 0 && beMessenger_getBool(mgr->_messenger, 0) == true) {
                beMessenger_selectGroup(mgr->_messenger, D_00516180);
                beMessenger_pushString(mgr->_messenger, D_00516188);
                beMessenger_send(mgr->_messenger, D_00516198);
            } else {
                beMessenger_select(mgr->_messenger, D_00516250, NULL, 0);
                beMessenger_pushInt(mgr->_messenger, ram->_ramNo);
                beMessenger_pushString(mgr->_messenger, D_00516258);
                beMessenger_send(mgr->_messenger, D_00516260);
            }
        }
    }
}

/* __dt__26beNDMWMdlPlayerInfoWorkRefFv (0x3D3170) is beNDMWMdlPlayerInfoWorkRef's out-of-line destructor (exception
   cleanup of func_003D22F0, which creates the work object via func_0036AE80). While
   func_003D22F0 is asm, this caller (not in the unit, so mwsplice drops it) makes MWCC
   emit it. */
}
IG_REF(beNDMWMdlPlayerInfoWork);
extern "C" {
#ifndef NON_MATCHING
void emit___dt__26beNDMWMdlPlayerInfoWorkRefFv(beNDMWMdlPlayerInfoWork* w) {
    beNDMWMdlPlayerInfoWorkRef work(w, IG_ADOPT);

    func_002FF820(work);
}
#endif

extern char D_00516270[]; /* "Atck01" */
extern char D_00516278[]; /* "ACTION" */
extern char D_00516280[]; /* "ISCOMEND" */
extern char D_00516298[]; /* "Atck02" */
extern char D_005162A0[]; /* "Atck03" */
void func_00395250(beBaseInfoManager* mgr, beBaseInfo* info, beBaseInfoRam* ram, beNDMWMdlPlayerInfoWork* work);

#define PAD_CROSS 0x4000

/* Attack combo: while Atck01/Atck02/Atck03 plays, holding X chains the next attack
   (func_00395250); letting go returns to SQ_WAIT once the action ends (ISCOMEND). */
void func_003D31F0(beNDMWMdlPlayer* mgr, beBaseInfoRam* ram) {
    beBaseInfo* info = ram->_parentInfo;
    beNDMWMdlPlayerInfoWork* work;
    s32 buttons;

    MakeDataList(ram);
    work = (beNDMWMdlPlayerInfoWork*)((beModelCtrlInfoRamData*)func_003022C0(ram->_dataList, D_00585BE0,
                                                                             D_00516110))->_work;
    buttons = 0;
    if (work->_isOnCtrl) {
        buttons = bePadManager_getPad(mgr->_padManager, ram->_ramNo)->_button;
    }
    if (strcmp(work->_wCheckName, D_00516270) == 0) {
        switch (work->_wCheckSt) {
        case 0:
            work->_wCheckMode = 0;
            work->_wCheckSt = 1;
        case 1:
            if (!(buttons & PAD_CROSS)) {
                work->_wCheckMode = 1;
            }
            {
                s32 no = ram->_ramNo;

                beMessenger_call(mgr->_messenger, D_00516278, NameOf(info), no, D_00516280);
            }
            if (beMessenger_getBool(mgr->_messenger, 0)) {
                SetStrH(work->_wCheckName, NULL);
                work->_wCheckSt = 0;
                if (work->_wCheckMode == 0) {
                    func_00395250(mgr, info, ram, work);
                } else {
                    ((beModelCtrlV*)mgr)->setAction(ram, D_00516290);
                }
            }
            break;
        }
    } else if (strcmp(work->_wCheckName, D_00516298) == 0) {
        if (!(buttons & PAD_CROSS)) {
            ((beModelCtrlV*)mgr)->setAction(ram, D_00516290);
        }
        {
            s32 no = ram->_ramNo;

            beMessenger_call(mgr->_messenger, D_00516278, NameOf(info), no, D_00516280);
        }
        if (beMessenger_getBool(mgr->_messenger, 0)) {
            SetStrH(work->_wCheckName, NULL);
            func_00395250(mgr, info, ram, work);
        }
    } else if (strcmp(work->_wCheckName, D_005162A0) == 0 && !(buttons & PAD_CROSS)) {
        SetStrH(work->_wCheckName, NULL);
        func_00395250(mgr, info, ram, work);
    }
}

extern char D_00516038[]; /* "PLAYER" */
extern char D_00516260[]; /* "GENERATE" */
extern char D_00516368[]; /* "PLAYERPOINT" */
extern char D_00516378[]; /* "GETMATRIX" */
extern char D_00516388[]; /* "GAMERAM" */
extern char D_00516390[]; /* "GETPLAYER" */
extern char D_005163A0[]; /* "HP" */
extern char D_005163A8[]; /* "MHP" */
extern char D_005163B0[]; /* "MMP" */
extern char D_005163B8[]; /* "MP" */
extern char D_005163C0[]; /* "UPDATA" */
extern char D_005163C8[]; /* "DOWNDATA" */
extern char D_005163D8[]; /* "ISUSE" */
extern char D_005163E0[]; /* "GAMESTART" */
extern char D_005163F0[]; /* "BASE_ISUSE" */
extern char D_00516400[]; /* "POWERSOCKETNAME" */
extern char D_00516410[]; /* "PLAYERARMS" */
extern char D_00516420[]; /* "RECALCPLAYER" */
extern char D_00516430[]; /* "NUMBERCTRL" */
extern char D_00516440[]; /* "lvnm" */
extern char D_00516448[]; /* "LEVEL" */
extern char D_00516450[]; /* "SETVALUE" */
extern char D_00516460[]; /* "METERCTRL" */
extern char D_00516470[]; /* "hpmt" */
extern char D_00516478[]; /* "SETPERCENT" */
extern char D_00516488[]; /* "hpnm" */
extern char D_00516490[]; /* "mpmt" */
extern char D_00516498[]; /* "mpnm" */
extern char D_005164A0[]; /* "SETMETAL" */
extern const s32 D_004CC7B8; /* 0 */
extern const s32 D_004CC7C0; /* 0 */
extern char* D_004CC7D0[]; /* per-player names (Shift-JIS) */
extern char* D_004CC7E0[]; /* "dng204b1".."dng205b3": metal stages */
extern char* D_004CC800[]; /* "stagML02", "stagML11".."stagML13": lobby stages */
beNDMWMdlPlayerInfoWork* func_003BFC80(void* pool); /* new beNDMWMdlPlayerInfoWork */
igIntList* func_002CAA70(void* pool);                /* new igIntList */
void func_00190380(igMatrix44f* m);                           /* identity */
void func_001908F0(igMatrix44f* dst, const igMatrix44f* src); /* copy */
void func_00396740(beBaseInfoManager* mgr, beBaseInfoRam* ram);
void beMessenger_getMatrix(igMatrix44f* out, beMessenger* m, s32 i);
s32 beMessenger_getMatrixCount(beMessenger* m);
s32 beMessenger_getVec3fCount(beMessenger* m);
void beMessenger_delayCall(beMessenger* m, const char* group, const char* info, s32 ramNo, const char* message, s64 delay);

#ifdef NON_MATCHING
/* 8 instructions off: registers in the MP percentage (mp * 100 / mmp). */
/* Player setup: the work object, the start point (PLAYERPOINT) when none was given, HP/MHP from
   the GAMERAM (full when HP is 0), the level and HP/MP meters, metal stages and lobby flag. */
void func_003D3640(beNDMWMdlPlayer* mgr, beBaseInfoRam* ram) {
    beBaseInfo* info = ram->_parentInfo;
    beModelCtrlInfoRamData* data;
    beNDMWMdlPlayerInfoWork* work;
    beDataObjList* player;
    const char* stage;
    s32 i;
    s32 j;
    s32 k;
    s32 l2;

    MakeDataList(ram);
    data = (beModelCtrlInfoRamData*)func_003022C0(ram->_dataList, D_00585BE0, D_00516110);
    {
        beNDMWMdlPlayerInfoWorkRef w(func_003BFC80(PoolOf2(mgr)), IG_ADOPT);

        igAssign(data->_work, (beModelCtrlInfoWork*)w.p);
        work = w;
        work->_isPlayer = true;
        work->_directionLimiter.x = 0.0f;
        work->_directionLimiter.y = 7.0f;
        work->_directionLimiter.z = 0.0f;
        {
            igIntListRef l(func_002CAA70(PoolOf2(mgr)), IG_ADOPT);

            ListReserve(l.p, 8, 4);
            ListSetCount(l.p, 8, 4);
            ListZero(l.p);
            igAssign(work->_turnAtkTimer, l.p);
        }
    }
    if (beMessenger_getVec3fCount(mgr->_messenger) == 0 && beMessenger_getMatrixCount(mgr->_messenger) == 0) {
        igMatrix44f pos;

        func_00190380(&pos);
        beMessenger_push(mgr->_messenger);
        beMessenger_selectGroup(mgr->_messenger, D_00516368);
        beMessenger_pushInt(mgr->_messenger, 0);
        beMessenger_pushInt(mgr->_messenger, ram->_ramNo);
        beMessenger_send(mgr->_messenger, D_00516378);
        if (beMessenger_getMatrixCount(mgr->_messenger) != 0) {
            igMatrix44f m;

            beMessenger_getMatrix(&m, mgr->_messenger, 0);
            func_001908F0(&pos, &m);
        }
        beMessenger_pop(mgr->_messenger);
        beMessenger_pushMatrix(mgr->_messenger, &pos);
    }
    func_00396740(mgr, ram);
    beMessenger_selectGroup(mgr->_messenger, D_00516388);
    beMessenger_pushInt(mgr->_messenger, ram->_ramNo);
    beMessenger_send(mgr->_messenger, D_00516390);
    player = (beDataObjList*)beMessenger_getObject(mgr->_messenger, 0);
    igAssign(work->_hp, beDataObjList_getInt(player, D_005163A0));
    igAssign(work->_mHp, beDataObjList_getInt(player, D_005163A8));
    if (work->_hp->_data == 0) {
        s32 v;

        v = beDataObjList_getInt(player, D_005163A8)->_data;
        beDataObjList_getInt(player, D_005163A0)->_data = v;
        v = beDataObjList_getInt(player, D_005163B0)->_data;
        beDataObjList_getInt(player, D_005163B8)->_data = v;
    }
    for (i = 0; i < 7; i++) {
        IntListSet(beDataObjList_getIntList(player, D_005163C0)->_data, i, D_004CC7B8);
    }
    for (j = 0; j < 6; j++) {
        IntListSet(beDataObjList_getIntList(player, D_005163C8)->_data, j, D_004CC7C0);
    }
    if (!beDataObjList_getBool(player, D_005163D8)->_data) {
        beMessenger_selectGroup(mgr->_messenger, D_00516388);
        beMessenger_pushInt(mgr->_messenger, ram->_ramNo);
        beMessenger_pushString(mgr->_messenger, NameOf(info));
        beMessenger_pushString(mgr->_messenger, D_004CC7D0[ram->_ramNo]);
        beMessenger_send(mgr->_messenger, D_005163E0);
        ListFill(beDataObjList_getBoolList(player, D_005163F0)->_data, true);
    }
    if (beDataObjList_getStringList(player, D_00516400)->_data->_count != 0) {
        beMessenger_delayCall(mgr->_messenger, D_00516410,
                              func_001C64E0(beDataObjList_getStringList(player, D_00516400)->_data, 0).p,
                              ram->_ramNo, D_00516260, 1);
    }
    beMessenger_selectGroup(mgr->_messenger, D_00516388);
    beMessenger_pushInt(mgr->_messenger, ram->_ramNo);
    beMessenger_send(mgr->_messenger, D_00516420);
    beMessenger_select(mgr->_messenger, D_00516430, D_00516440, ram->_ramNo);
    beMessenger_pushInt(mgr->_messenger, beDataObjList_getInt(player, D_00516448)->_data);
    beMessenger_send(mgr->_messenger, D_00516450);
    beMessenger_select(mgr->_messenger, D_00516460, D_00516470, ram->_ramNo);
    {
        s32 mhp = beDataObjList_getInt(player, D_005163A8)->_data;

        beMessenger_pushFloat(mgr->_messenger, 100.0f * beDataObjList_getInt(player, D_005163A0)->_data / mhp);
    }
    beMessenger_send(mgr->_messenger, D_00516478);
    beMessenger_select(mgr->_messenger, D_00516430, D_00516488, ram->_ramNo);
    beMessenger_pushInt(mgr->_messenger, beDataObjList_getInt(player, D_005163A0)->_data);
    beMessenger_send(mgr->_messenger, D_00516450);
    beMessenger_select(mgr->_messenger, D_00516460, D_00516490, ram->_ramNo);
    {
        s32 mp = beDataObjList_getInt(player, D_005163B8)->_data;

        beMessenger_pushFloat(mgr->_messenger, mp * 100 / beDataObjList_getInt(player, D_005163B0)->_data);
    }
    beMessenger_send(mgr->_messenger, D_00516478);
    beMessenger_select(mgr->_messenger, D_00516430, D_00516498, ram->_ramNo);
    beMessenger_pushInt(mgr->_messenger, beDataObjList_getInt(player, D_005163B8)->_data);
    beMessenger_send(mgr->_messenger, D_00516450);
    stage = mgr->_system->_stageName;
    for (k = 0; k < 5; k++) {
        if (strcmp(D_004CC7E0[k], stage) == 0) {
            beMessenger_call(mgr->_messenger, D_00516038, NULL, ram->_ramNo, D_005164A0);
            break;
        }
    }
    for (l2 = 0; l2 < 4; l2++) {
        if (strcmp(D_004CC800[l2], stage) == 0) {
            work->_isMainLobby = true;
            break;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWMdlPlayer", func_003D3640);
#endif


/* Close the technique panel if this player has it open. */
void func_003D40F0(beBaseInfoManager* mgr, beBaseInfoRam* ram) {
    beNDMWMdlPlayerInfoWork* work;

    MakeDataList(ram);
    work = (beNDMWMdlPlayerInfoWork*)((beModelCtrlInfoRamData*)func_003022C0(ram->_dataList, D_00585BE0,
                                                                             D_00516110))->_work;
    if (work->_isPanel) {
        work->_isPanel = false;
        beMessenger_call(mgr->_messenger, D_00516148, NULL, ram->_ramNo, D_005164B0);
    }
}

/* Same body as func_003D40F0 (close the technique panel). */
void func_003D4240(beBaseInfoManager* mgr, beBaseInfoRam* ram) {
    beNDMWMdlPlayerInfoWork* work;

    MakeDataList(ram);
    work = (beNDMWMdlPlayerInfoWork*)((beModelCtrlInfoRamData*)func_003022C0(ram->_dataList, D_00585BE0,
                                                                             D_00516110))->_work;
    if (work->_isPanel) {
        work->_isPanel = false;
        beMessenger_call(mgr->_messenger, D_00516148, NULL, ram->_ramNo, D_005164B0);
    }
}

}

extern "C" {
extern char D_004E0858[]; /* "" */
extern char D_00516228[]; /* "MODELCTRL" */
extern char D_00516290[]; /* "SQ_WAIT" */
extern char D_005164B8[]; /* "i_pcnumic" */
extern char D_005164C8[]; /* "SQ_OPENP%d" */
extern char D_005164D8[]; /* "SETEFFECT" */
extern char D_00516528[]; /* "g_000bcaps" */
extern char D_00516248[]; /* "ISRUN" */
extern char D_00516260[]; /* "GENERATE" */
extern char D_00516388[]; /* "GAMERAM" */
extern char D_00516538[]; /* "BKCHIPBITEXP" */
extern char* D_004CC810[]; /* "SQ_ENTR1P".."SQ_ENTR4P" */
igMetaEnum* func_003BF830(void); /* the player's sequence names */
igResult func_002FBA30(igObject* table, const char* name, s32* value);
void func_00445200(beBaseInfoManager* mgr, beBaseInfoRam* ram);

/* While waiting (SQ_WAIT): the "player N" icon effect over the model. If the current sequence
   is one of the player's own, the technique panel closes. */
void func_003D4390(beBaseInfoManager* mgr, beBaseInfoRam* ram) {
    beBaseInfo* info = ram->_parentInfo;
    beNDMWMdlPlayerInfoWork* work;
    s32 value;

    MakeDataList(ram);
    work = (beNDMWMdlPlayerInfoWork*)((beModelCtrlInfoRamData*)func_003022C0(ram->_dataList, D_00585BE0,
                                                                             D_00516110))->_work;
    if (strcmp(work->_seqCodeName, D_00516290) == 0) {
        s32 no = ram->_ramNo;
        char name[24];
        igVec3f offset;

        beMessenger_select(mgr->_messenger, D_00516228, NameOf(info), no);
        beMessenger_pushString(mgr->_messenger, D_005164B8);
        sprintf(name, D_005164C8, ram->_ramNo);
        beMessenger_pushString(mgr->_messenger, name);
        beMessenger_pushFloat(mgr->_messenger, 480.0f);
        offset.x = 0.0f;
        offset.y = 10.0f;
        offset.z = 0.0f;
        beMessenger_pushVec3f(mgr->_messenger, &offset);
        beMessenger_pushBool(mgr->_messenger, true);
        beMessenger_pushBool(mgr->_messenger, true);
        beMessenger_send(mgr->_messenger, D_005164D8);
    }
    if (work->_isPanel && func_002FBA30((igObject*)func_003BF830(), work->_seqCodeName, &value) == D_004FB410) {
        work->_isPanel = false;
        beMessenger_call(mgr->_messenger, D_00516148, NULL, ram->_ramNo, D_005164B0);
    }
    func_00445200(mgr, ram);
}

/* On death: the backup chip appears at the player (GENERATE with the player's entry sequence);
   if it is running, the GAMERAM moves BIT and EXP into it (BKCHIPBITEXP). */
void func_003D45F0(beBaseInfoManager* mgr, beBaseInfoRam* ram) {
    beModelCtrlInfoRamData* data;
    beNDMWMdlPlayerInfoWork* work;

    MakeDataList(ram);
    data = (beModelCtrlInfoRamData*)func_003022C0(ram->_dataList, D_00585BE0, D_00516110);
    work = (beNDMWMdlPlayerInfoWork*)data->_work;
    beMessenger_select(mgr->_messenger, D_00516228, D_00516528, ram->_ramNo);
    beMessenger_pushMatrix(mgr->_messenger, &data->_node->_m);
    beMessenger_pushString(mgr->_messenger, D_004CC810[ram->_ramNo]);
    beMessenger_send(mgr->_messenger, D_00516260);
    beMessenger_call(mgr->_messenger, D_00516228, D_00516528, ram->_ramNo, D_00516248);
    if (beMessenger_getBoolCount(mgr->_messenger) != 0 && beMessenger_getBool(mgr->_messenger, 0) == true) {
        beMessenger_selectGroup(mgr->_messenger, D_00516388);
        beMessenger_pushInt(mgr->_messenger, ram->_ramNo);
        beMessenger_send(mgr->_messenger, D_00516538);
    }
    if (work->_isPanel) {
        work->_isPanel = false;
        beMessenger_call(mgr->_messenger, D_00516148, NULL, ram->_ramNo, D_005164B0);
    }
}

/* Same body as func_003D40F0 (close the technique panel). */
void func_003D4820(beBaseInfoManager* mgr, beBaseInfoRam* ram) {
    beNDMWMdlPlayerInfoWork* work;

    MakeDataList(ram);
    work = (beNDMWMdlPlayerInfoWork*)((beModelCtrlInfoRamData*)func_003022C0(ram->_dataList, D_00585BE0,
                                                                             D_00516110))->_work;
    if (work->_isPanel) {
        work->_isPanel = false;
        beMessenger_call(mgr->_messenger, D_00516148, NULL, ram->_ramNo, D_005164B0);
    }
}

igMetaObject* beNDMWMdlPlayer_getMeta(void) {
    return beNDMWMdlPlayer_Meta;
}

void beNDMWMdlPlayer2_begin(beNDMWMdlPlayer2* mgr) {
    beBaseInfoManager_begin(mgr);
    mgr->_padManager = (bePadManager*)func_00236F80(mgr->_insight, bePadManager_Meta);
}

s32 func_003D49C0(beBaseInfoManager* mgr, const char* kind) {
    return strcmp(kind, D_005165B0) == 0;
}

void func_003D49F0(beBaseInfoManager* mgr, beBaseInfoRam* ram) {
    beMessenger_entryRam(mgr->_messenger, ram, D_005165C0);
}

#ifdef NON_MATCHING
/* Register allocation and the BKCHIPPOS stores differ (the original keeps the float's address
   in a register, and reads ramNo in the stage name test's delay slot). */
}

IG_REF(beNDMWMdlPlayer2InfoWork);

/* A pooled string held for a scope (igStringRef: lives on the stack). */
struct StrRef {
    char* p;
    StrRef(const char* s) : p(s == NULL ? NULL : func_002EAF60(GetStrPool(), s)) {}
    ~StrRef() { ReleaseStr(p); }
};

static inline void StrListSet(igStringRefList* list, s32 i, const StrRef& s) {
    if (list->_count != 0 && i >= 0 && i < list->_count) {
        char** p = &((char**)list->_data)[i];

        if (s.p != NULL) {
            ((StrHeader*)s.p - 1)->refs++;
        }
        ReleaseStr(*p);
        *p = s.p;
    }
}

static inline void FloatListSet(igFloatList* list, s32 i, const f32& v) {
    if (list->_count != 0 && i >= 0 && i < list->_count) {
        ((f32*)list->_data)[i] = v;
    }
}

extern "C" {
extern char D_005165C8[]; /* work data name */
extern char D_005165D0[]; /* "GAMERAM" */
extern char D_005165D8[]; /* "GETCOMMON" */
extern char D_005165E8[]; /* "BKCHIPSTAGE" */
extern char D_005165F8[]; /* "BKCHIPPOS" */
beNDMWMdlPlayer2InfoWork* func_003BF560(void* pool); /* new beNDMWMdlPlayer2InfoWork */
void func_00396740(beBaseInfoManager* mgr, beBaseInfoRam* ram);
void func_00190A30(const igMatrix44f* m, f32* pos); /* translation of a matrix */

/* Backup chip creation (docs/death.md): its work object, then BKCHIPSTAGE[owner] = current
   stage and BKCHIPPOS[owner] = the chip's position. */
void func_003D4A20(beNDMWMdlPlayer2* mgr, beBaseInfoRam* ram) {
    beModelCtrlInfoRamData* data;
    beDataObjList* common;
    beDataObjStringList* stages;
    beDataObjFloatList* positions;
    s32 no;
    f32 pos[3];

    MakeDataList(ram);
    data = (beModelCtrlInfoRamData*)func_003022C0(ram->_dataList, D_00585BE0, D_005165C8);
    {
        beNDMWMdlPlayer2InfoWorkRef work(func_003BF560(PoolOf2(mgr)), IG_ADOPT);

        igAssign(data->_work, (beModelCtrlInfoWork*)work.p);
    }
    func_00396740(mgr, ram);
    beMessenger_callGroup(mgr->_messenger, D_005165D0, D_005165D8);
    common = (beDataObjList*)beMessenger_getObject(mgr->_messenger, 0);
    stages = beDataObjList_getStringList(common, D_005165E8);
    positions = beDataObjList_getFloatList(common, D_005165F8);
    {
        StrRef name(mgr->_system->_stageName);

        no = ram->_ramNo;
        StrListSet(stages->_data, no, name);
    }
    func_00190A30(&data->_node->_m, pos);
    FloatListSet(positions->_data, no * 3, pos[0]);
    FloatListSet(positions->_data, no * 3 + 1, pos[1]);
    FloatListSet(positions->_data, no * 3 + 2, pos[2]);
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWMdlPlayer", func_003D4A20);

/* __dt__27beNDMWMdlPlayer2InfoWorkRefFv (0x3D4E20) is beNDMWMdlPlayer2InfoWorkRef's out-of-line
   destructor (exception cleanup of func_003D4A20, which creates the work object via
   func_003BF560). While func_003D4A20 is asm, this caller makes MWCC emit it. */
}
IG_REF(beNDMWMdlPlayer2InfoWork);
extern "C" {
#ifndef NON_MATCHING
void emit___dt__27beNDMWMdlPlayer2InfoWorkRefFv(beNDMWMdlPlayer2InfoWork* w) {
    beNDMWMdlPlayer2InfoWorkRef work(w, IG_ADOPT);

    func_002FF820(work);
}
#endif
#endif

extern char D_005165C8[]; /* work data name */
extern char D_00516608[]; /* target type (player) */
extern char D_00516610[]; /* target mode */
extern char D_00516618[]; /* action: power on */
extern char D_00516620[]; /* action: power off */
extern char D_00516630[]; /* script when no player is around */
bool beTargetObj_search(const igMatrix44f* m, const char* type, const char* mode);
f32 beTargetObj_getTargetDist(void);
void func_00389FF0(beLua* lua, const char* name, const char* dir); /* run a script */

/* Backup chip update (docs/death.md): counts the RAISE delay, and lights up while a player is
   within 30 units. */
void func_003D4EA0(beNDMWMdlPlayer2* mgr, beBaseInfoRam* ram) {
    beModelCtrlInfoRamData* data;
    beNDMWMdlPlayer2InfoWork* work;

    MakeDataList(ram);
    data = (beModelCtrlInfoRamData*)func_003022C0(ram->_dataList, D_00585BE0, D_005165C8);
    work = (beNDMWMdlPlayer2InfoWork*)data->_work;
    if (work->_timerCount < 10) {
        work->_timerCount++;
    }
    bePadManager_getPad(mgr->_padManager, ram->_ramNo);
    if (beTargetObj_search(&data->_node->_m, D_00516608, D_00516610)) {
        f32 d = beTargetObj_getTargetDist();

        if (!work->_isPowerOn) {
            if (d < 900.0f) {
                work->_isPowerOn = true;
                ((beModelCtrlV*)mgr)->setAction(ram, D_00516618);
            }
        } else if (!(d <= 900.0f)) {
            work->_isPowerOn = false;
            ((beModelCtrlV*)mgr)->setAction(ram, D_00516620);
        }
    } else {
        func_00389FF0(mgr->_luaManager, D_00516630, NULL);
    }
}

}

static inline void StrListSetH(igStringRefList* list, s32 i, const StrRefH& s) {
    if (list->_count != 0 && i >= 0 && i < list->_count) {
        char** p = &((char**)list->_data)[i];

        if (s.p != NULL) {
            ((StrHeader*)s.p - 1)->refs++;
        }
        ReleaseStr(*p);
        *p = s.p;
    }
}

static inline const f32& FloatAt(igFloatList* list, s32 i) {
    return ((f32*)list->_data)[i];
}

static inline const s32& IntAt(igIntList* list, s32 i) {
    return ((s32*)list->_data)[i];
}

extern "C" {
extern char D_005165D0[]; /* "GAMERAM" */
extern char D_005165D8[]; /* "GETCOMMON" */
extern char D_005165E8[]; /* "BKCHIPSTAGE" */
extern char D_005165F8[]; /* "BKCHIPPOS" */
extern char D_00516648[]; /* "GENERATE" */
extern char D_00516658[]; /* "REMOVE" */
extern char D_00516660[]; /* "GENESTAGE" */
extern char D_00516670[]; /* "MODELCTRL" */
extern char D_00516680[]; /* "g_000bcaps" */
extern char D_00516690[]; /* "SQ_ENTR%dP" */
extern char D_005166A0[]; /* "RAISE" */
extern char D_005166A8[]; /* "ISRUN" */
extern char D_005166B0[]; /* "GETPLAYER" */
extern char D_005166C0[]; /* "HP" */
extern char D_005166C8[]; /* "DIGIMONNAME" */
extern char D_005166D8[]; /* "p_agumon" */
extern char D_005166E8[]; /* "SQ_CMBK" */
extern char D_005166F0[]; /* "BKCHIPDATA" */
extern char D_00516700[]; /* "BIT" */
extern char D_00516708[]; /* "BKCHIPNAME" */
extern char D_00516718[]; /* "EXP" */
extern char D_00516720[]; /* "SQ_EXIT" */
extern const s32 D_004CC820; /* 0 */
extern const s32 D_004CC828; /* 0 */
void beMessenger_getMatrix(igMatrix44f* out, beMessenger* m, s32 i);
void beMessenger_delayCall(beMessenger* m, const char* group, const char* info, s32 ramNo, const char* message, s64 delay);

/* Backup chip messages (docs/death.md): GENERATE (remove the chip), GENESTAGE (place it at
   BKCHIPPOS if BKCHIPSTAGE is this stage) and RAISE (give BIT and EXP back, revive the owner). */
#ifdef NON_MATCHING
/* 26 instructions off: register choice in GENESTAGE (ramNo vs the temporary's flag) and
   ramNo is read later for BKCHIPNAME. */
bool func_003D50C0(beNDMWMdlPlayer2* mgr, beBaseInfoRam* ram, const char* msg) {
    if (strcmp(msg, D_00516648) == 0) {
        if (ram->_isRun) {
            ((beModelCtrlV*)mgr)->setAction(ram, D_00516658);
        }
    } else if (strcmp(msg, D_00516660) == 0) {
        beDataObjList* common;
        igFloatList* positions;
        s32 no;
        igStringRefList* stages;

        beMessenger_callGroup(mgr->_messenger, D_005165D0, D_005165D8);
        common = (beDataObjList*)beMessenger_getObject(mgr->_messenger, 0);
        stages = beDataObjList_getStringList(common, D_005165E8)->_data;
        positions = beDataObjList_getFloatList(common, D_005165F8)->_data;
        no = ram->_ramNo;
        if (func_001C64E0(stages, no).p != NULL &&
            strcmp(mgr->_system->_stageName, func_001C64E0(stages, no).p) == 0) {
            s32 k = no * 3;
            s32 k1 = k + 1;
            s32 k2 = k + 2;
            f32 x = ((f32*)positions->_data)[k];
            f32 y = ((f32*)positions->_data)[k1];
            f32 z = ((f32*)positions->_data)[k2];
            char name[16];
            igVec3f pos;

            beMessenger_delaySelect(mgr->_messenger, D_00516670, D_00516680, ram->_ramNo);
            pos.x = x;
            pos.y = y;
            pos.z = z;
            beMessenger_pushVec3f(mgr->_messenger, &pos);
            sprintf(name, D_00516690, ram->_ramNo + 1);
            beMessenger_pushString(mgr->_messenger, name);
            beMessenger_delaySend(mgr->_messenger, D_00516648, 1);
        }
        return true;
    } else if (strcmp(msg, D_005166A0) == 0) {
        beModelCtrlInfoRamData* data;
        igMatrix44f mtx;
        s32 rate;
        s32 hp;
        bool alive;
        s32 n;
        s32 i;
        beDataObjList* player;
        beDataObjList* common;
        s32 k;
        s32 v;
        const char* chipName;
        const char* name;

        MakeDataList(ram);
        data = (beModelCtrlInfoRamData*)func_003022C0(ram->_dataList, D_00585BE0, D_005165C8);
        if (((beNDMWMdlPlayer2InfoWork*)data->_work)->_timerCount < 10) {
            return true;
        }
        beMessenger_getMatrix(&mtx, mgr->_messenger, 0);
        rate = beMessenger_getInt(mgr->_messenger, 0);
        hp = beMessenger_getInt(mgr->_messenger, 1);
        alive = false;
        beMessenger_call(mgr->_messenger, D_00516608, NULL, ram->_ramNo, D_005166A8);
        n = beMessenger_getBoolCount(mgr->_messenger);
        for (i = 0; i < n; i++) {
            if (beMessenger_getBool(mgr->_messenger, i) == true) {
                alive = true;
                break;
            }
        }
        beMessenger_selectGroup(mgr->_messenger, D_005165D0);
        beMessenger_pushInt(mgr->_messenger, ram->_ramNo);
        beMessenger_send(mgr->_messenger, D_005166B0);
        player = (beDataObjList*)beMessenger_getObject(mgr->_messenger, 0);
        if (!alive) {
            const char* model;

            beDataObjList_getInt(player, D_005166C0)->_data = hp;
            model = beDataObjList_getString(player, D_005166C8)->_data;
            if (model[0] == '\0') {
                model = D_005166D8;
            }
            beMessenger_select(mgr->_messenger, D_00516670, model, ram->_ramNo);
            beMessenger_pushMatrix(mgr->_messenger, &mtx);
            beMessenger_pushString(mgr->_messenger, D_005166E8);
            beMessenger_send(mgr->_messenger, D_00516648);
        }
        beMessenger_callGroup(mgr->_messenger, D_005165D0, D_005165D8);
        common = (beDataObjList*)beMessenger_getObject(mgr->_messenger, 0);
        k = ram->_ramNo * 2;
        v = IntAt(beDataObjList_getIntList(common, D_005166F0)->_data, k);
        v += beDataObjList_getInt(player, D_00516700)->_data;
        beDataObjList_getInt(player, D_00516700)->_data = v;
        IntListSet(beDataObjList_getIntList(common, D_005166F0)->_data, k, D_004CC820);
        name = beDataObjList_getString(player, D_005166C8)->_data;
        chipName = func_001C64E0(beDataObjList_getStringList(common, D_00516708)->_data, ram->_ramNo).p;
        if (strcmp(name, chipName) == 0) {
            k = ram->_ramNo * 2 + 1;
            v = IntAt(beDataObjList_getIntList(common, D_005166F0)->_data, k);
            v = v * rate / 100;
            v += beDataObjList_getInt(player, D_00516718)->_data;
            beDataObjList_getInt(player, D_00516718)->_data = v;
            IntListSet(beDataObjList_getIntList(common, D_005166F0)->_data, k, D_004CC828);
        }
        beMessenger_delayCall(mgr->_messenger, D_005165C0, NULL, ram->_ramNo, D_00516720, 0);
        beMessenger_callGroup(mgr->_messenger, D_005165D0, D_005165D8);
        common = (beDataObjList*)beMessenger_getObject(mgr->_messenger, 0);
        StrListSetH(beDataObjList_getStringList(common, D_005165E8)->_data, ram->_ramNo, StrRefH(NULL));
        return true;
    }
    return false;
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWMdlPlayer", func_003D50C0);
#endif

igMetaObject* beNDMWMdlPlayer2_getMeta(void) {
    return beNDMWMdlPlayer2_Meta;
}

}
