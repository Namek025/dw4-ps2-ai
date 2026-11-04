#include "common.h"

/* Built with MWCC as C++ (see configure.py). Symbols are stripped in the
   original, so names stay unmangled to match config/symbol_addrs.txt. */
extern "C" {

/* Named game parameters (guess): func_00371C10(n) returns a parameter
   container, the lookups find an entry by name and the value sits at +0xC. */
typedef struct {
    u8 pad0[0xC];
    s32 value;
} ParamInt;

typedef struct {
    u8 pad0[0xC];
    bool value;
} ParamBool;

typedef struct {
    u8 pad0[0xC];
    char* value;
} ParamStr;

/* Array parameters: element count at +0x8, data at +0x10. */
typedef struct {
    u8 pad0[0x8];
    s32 count;
    u8 padC[0x4];
    s32* data;
} IntArray;

typedef struct {
    u8 pad0[0x8];
    s32 count;
    u8 padC[0x4];
    bool* data;
} BoolArray;

typedef struct {
    u8 pad0[0x8];
    s32 count;
    u8 padC[0x4];
    char** data;
} StrArray;

typedef struct {
    u8 pad0[0xC];
    IntArray* value;
} ParamIntArray;

typedef struct {
    u8 pad0[0xC];
    BoolArray* value;
} ParamBoolArray;

/* Shared string pool (guess): strings carry a header with an owner at -8 and
   a reference count at -4. Created on first use. */
typedef struct StrPool StrPool;

typedef struct {
    void* owner;
    s32 refs;
} StrHeader;

typedef struct {
    f32 x, y, z;
} Vec3;

/* Object at D_0058A1B8->unk48 used to turn "F<...>" text into a float. */
typedef struct UnkStruct_Conv {
    u8 pad0[0xC];
    s32 unkC;
} UnkStruct_Conv;

/* Random generator object. Only Next() is known: vtable +0x60, after the
   8-byte MWCC vtable header, so 22 virtuals come before it. */
class Random {
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
    virtual u32 Next();
};

typedef struct {
    u8 pad0[0x10];
    u8 unk10;
    u8 pad11[0x1F];
    Random* random;
    StrArray* unk34;
    char* unk38;
    char* unk3C;
    u8 unk40;
    u8 pad41[0x7];
    struct UnkStruct_Conv* unk48;
} UnkStruct_0058A1B8;

typedef struct {
    u8 pad0[0xC];
    void* unkC;
    StrArray* unk10;
} UnkStruct_00371A30;

typedef struct {
    u8 pad0[0xC];
    StrArray* value;
} ParamStrArray;

typedef unsigned int size_t;

extern void* D_0056DE30;                         /* heap */
void* func_002ED240(size_t size, void* heap);   /* allocate */
void func_002ED4C0(void* p, void* heap);        /* free */

/* Declarations (data in the data block, functions in other units).
 *
 * Most bindings drive the engine object at D_0058A1C0 (guess: script/object
 * system): select a class (beMessenger_selectGroup, or beMessenger_call/beMessenger_callGroup with
 * the method too), push arguments (beMessenger_pushInt int, beMessenger_pushString string,
 * beMessenger_pushObject object), call a method by name (beMessenger_send), then read
 * results (beMessenger_getBoolCount count, beMessenger_getBool bool, beMessenger_getInt int,
 * beMessenger_getObject object). D_0058A1C8 / D_0058A1D0 are managers that thin
 * wrappers forward to. */
extern void* D_0058A1C0;
extern char D_0050D940[];      /* "GAMERAM" */
extern char D_0050D8F0[];      /* "GETCOMMON" */
extern char* D_004C8720[];     /* "GETCOMMON", "GETPLAYER1", "GETPLAYER2", ... */
void beMessenger_call(void* obj, const char* cls, const char* arg0, s32 arg1, const char* method);
void beMessenger_callGroup(void* obj, const char* cls, const char* method);
void beMessenger_select(void* obj, const char* cls, const char* arg0, s32 arg1);
void* beMessenger_getObject(void* obj, s32 index);
void beMessenger_selectGroup(void* obj, const char* cls);
void beMessenger_send(void* obj, const char* method);
void beMessenger_pushInt(void* obj, s32 value);
void beMessenger_pushString(void* obj, const char* value);
bool beMessenger_getBool(void* obj, s32 index);
s32 beMessenger_getInt(void* obj, s32 index);
s32 beMessenger_getBoolCount(void* obj); /* result count */
void beMessenger_pushObject(void* obj, void* value); /* push an object */
extern char D_0050DAF0[];     /* "ISRUN" */
ParamIntArray* beDataObjList_getIntList(void* params, const char* name);
ParamBoolArray* beDataObjList_getBoolList(void* params, const char* name);
extern StrPool* D_00586F20;
void* func_002EB010(u32 size);                    /* operator new */
StrPool* func_002EADF0(StrPool* pool);            /* StrPool constructor */
char* func_002EAF60(StrPool* pool, const char* s); /* get / add a string */
void func_0030C940(void* owner, StrHeader* h); /* free a pooled string */
void func_002D6680(StrArray* array, s32, s32); /* resize? */
ParamStrArray* beDataObjList_getStringList(void* params, const char* name);
void func_001C64E0(char** out, StrArray* array, s32 index); /* returns a pooled string by value */
void* func_00371C10(s32 index);
void func_0037E110(void* mgr, char*, char*, s32, char*, char*);
void func_0037E3A0(void* mgr, s32, char*, char*, s32, char*, char*, s64);
extern s32 D_0058A1E4;
extern char* D_0058A1F8;
extern char D_0050DB10[]; /* "MODELCTRL" */
extern char D_0050DB00[]; /* "V<%f,%f,%f>" */
extern void* D_0058A1E8;
void func_00190A30(void* src, f32* pos); /* position of a model / matrix */
void func_00190380(f32* mtx);                       /* identity */
void func_00190A50(f32* mtx, f32* v);
void func_001908F0(f32* mtx, void* model);
s32 sprintf(char* buf, const char* fmt, ...);
s32 strcmp(const char* a, const char* b);
extern char D_0050D9C8[]; /* "" */
extern char D_0050D9F0[]; /* "FLAG" */
extern char D_0050DEE0[]; /* "GETPATKZOKUSEI" */
extern char D_0050D960[]; /* "POWERSOCKETNAME" */
extern char D_0050D9D8[]; /* "CTRLITEM" */
void sd_0(char* cls, char* arg0, s32 arg1, char* method, char* arg3);
ParamInt* beDataObjList_getInt(void* params, const char* name);
ParamBool* beDataObjList_getBool(void* params, const char* name);
ParamStr* beDataObjList_getString(void* params, const char* name);
extern UnkStruct_0058A1B8* D_0058A1B8;
extern char D_0050D948[]; /* "ISUSE" */
extern char D_0050D950[]; /* "DIGIMONNAME" */
extern char D_0050D970[]; /* "NINZU" */
extern char D_0050D978[]; /* "DIFFICULTY" */
extern char D_0050DA00[]; /* "LEVEL" */
extern char D_0050DA08[]; /* "DIGIMONNO" */
extern void* D_0058A1C8;
extern void* D_0058A1D0;
extern char D_0050CEB8[]; /* "be" */
extern char D_0050D7F0[]; /* "igInfoManager" */
extern u8 D_00539F00[];
void func_00241930(void* self, const char* name, const char* base, s32, s32, s32, s32, s32);
extern char D_0050C908[]; /* "Bec" */
extern u8 D_00539EF0[];
s32 func_00370780(void* L);
void func_0023F4F0(void* self, s32, s32 (*dispatch)(void*), char** names, s32 count, u32* descs, s32);
extern char* D_004C8560[];
extern u32 D_0050CFC0[];
extern UnkStruct_00371A30* D_0058A1D8;
extern void* beSystem_Meta;
extern void* beMessenger_Meta;
extern void* beMatCtrl_Meta;
extern void* beDemoManager_Meta;
void* func_00236F80(void* owner, void* type);
extern char D_0050D988[]; /* "CAMERAMODE" */
extern char D_0050D998[]; /* "LIGHTMODE" */
extern char D_0050D9A8[]; /* "GATESTAGENO" */
extern char D_0050D9B8[]; /* "GATESTAGENAME" */
extern char D_0050D9D0[]; /* "DEVICE" */
extern char D_0050D9E8[]; /* "DISK" */
extern char D_0050D9F8[]; /* "BIT" */
extern char D_0050DA18[]; /* "BASE_ISUSE" */
extern char D_0050DA98[]; /* "CTRLPLAYER" */
void func_00372CF0(char* args);
void beMessenger_delaySelect(void* obj, const char* cls, const char* arg0, s32 arg1);
void beMessenger_delaySend(void* obj, const char* method, s64 arg);
void beMessenger_delayCall(void* obj, char*, char*, s32, char*, s64);
bool func_0038ACA0(void*);
bool func_0038ACD0(void*);
s32 func_0038AD40(void*);
bool func_0038ADD0(void*, s32);
bool func_0038AE30(void*, s32);
s32 func_0038AEA0(void*, s32);
void func_0038AE90(void*, bool);
extern char D_0050DAC8[]; /* "!=0" */
extern char D_0050DAD0[]; /* "==1" */
extern char D_0050DAD8[]; /* "==0" */
extern char D_0050DAE0[]; /* "!=1" */
void func_0037E6E0(void*, char*);
void func_0037E890(void*, char*, s32);
void func_0037E180(void*);
void func_0037E2D0(void*, s32);
void func_0037EA50(void*);
extern char D_0050DAF8[]; /* "COUNTER" */
extern char D_0058A230[];
extern char D_0058A270[];
extern f32 D_004E1098[];
void func_00192F50(f32* out, f32* v, f32* mtx);
f32 func_00123640(f32 y, f32 x); /* atan2f */
extern s32 D_0058A1EC;
extern s32 D_0058A1F0;
extern s32 D_0058A1FC;
extern char* D_004C873C[]; /* "PCWA", "PCCA", ... indexed by kind / 10000 */
extern s32 D_0058A200;
extern u8 D_0058A210[]; /* first byte of a larger object (absolute, not small data) */
extern f32* D_0058A218;
void func_001E6D30(const char* s, f32* out, const char* fmt);
extern s32 D_0058A1F4;
extern char D_0050DD48[]; /* "CHECKSTATUS" */
extern char D_0050DD58[]; /* "PLAYERPOINT" */
extern char D_0050DD68[]; /* "GETGENERATEPOS" */
extern char D_0058A2F0[];
extern char D_0050DDA8[]; /* "PLAYER" */
extern char D_0050DDB0[]; /* "GETMATRIX" */
s32 beMessenger_getMatrixCount(void* obj);
void beMessenger_getMatrix(f32* out, void* obj, s32 index); /* returns a matrix by value */
extern s32 D_0058A220;
extern s32 D_0058A208;
extern char D_0050DDE8[]; /* "FIGHTCALC" */
extern char D_0050DDF8[]; /* "P2EISCRITICAL" */
extern s32 D_0058A330;
extern s32 D_0058A338;
extern char* D_004C8800[]; /* "common", "fire", "ice", "thunder", "dark" */
extern char D_0050DE40[]; /* "P2ESTUNTIME" */
extern char D_0050DE50[]; /* "P2EPOISONTIME" */
extern char D_0050DE60[]; /* "P2ESLEEPTIME" */
extern char D_0050DE70[]; /* "P2EBINDTIME" */
extern char D_0050DE80[]; /* "P2EATKDWNTIME" */
extern char D_0050DE90[]; /* "P2EPOWDWNTIME" */
extern char D_0050DEA0[]; /* "P2ESKILLUP" */
extern char D_0050DEB0[]; /* "P2EEXPUP" */
extern char D_0050DEC0[]; /* "E2PISHITCHECK" */
extern s32 D_0058A20C;
extern char D_0050DEF0[]; /* "E2PDAMAGERANK" */
extern void* D_0058A1E0;
extern s32 D_0058A340;
extern char* D_0058A204;
extern char D_0050DDC0[]; /* "ENEMY" */
extern char D_0050DDC8[]; /* "GETPEBASEWORK" */
extern char D_0050DF00[]; /* "E2PSTUNTIME" */
s32 beMessenger_getObjectCount(void* obj);
extern char D_0050DF10[]; /* "E2PPOISONTIME" */
extern char D_0050DF20[]; /* "E2PSLEEPTIME" */
extern char D_0050DF30[]; /* "E2PBINDTIME" */
extern char D_0050DF40[]; /* "E2PATKDWNTIME" */
extern char D_0050DF50[]; /* "E2PPOWDWNTIME" */
extern char D_0050DF60[]; /* "P2PKOUKAMAGIC" */
extern char D_0050DF70[]; /* "LOADINTF2" */
extern char D_0050DF80[]; /* "SETDIFFICULTMAX" */

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

/* Store a pooled copy of s in *dst, dropping the reference to the old string. */
static inline void AssignStr(char** dst, const char* s) {
    char* str = func_002EAF60(GetStrPool(), s);

    if (*dst != NULL) {
        StrHeader* h = (StrHeader*)*dst - 1;
        if (--h->refs == 0) {
            func_0030C940(h->owner, h);
        }
    }
    *dst = str;
}

/* Same as AssignStr, for a parameter. Kept separate: MWCC stops inlining one
   level deeper and SYSsetGateStage no longer matches. */
static inline void ReleaseStr(char* s) {
    if (s != NULL) {
        StrHeader* h = (StrHeader*)s - 1;
        if (--h->refs == 0) {
            func_0030C940(h->owner, h);
        }
    }
}

/* Bounds-checked store into a bool array. The element address is computed as
   an integer (index first), like a generic list would. */
static inline void SetBool(BoolArray* a, s32 i, const bool& v) {
    if (a->count != 0 && i >= 0 && i < a->count) {
        *(u8*)(i + (u32)a->data) = v;
    }
}

static inline void SetStr(ParamStr* param, const char* s) {
    char* str = func_002EAF60(GetStrPool(), s);

    if (param->value != NULL) {
        StrHeader* h = (StrHeader*)param->value - 1;
        if (--h->refs == 0) {
            func_0030C940(h->owner, h);
        }
    }
    param->value = str;
}

/* Script command prototypes, typed from the descriptor table (D_0050CFC0). */
s32 BLPcalcKoukaMagic(void);
s32 BLPgetAttackDownTime(char*);
char* BLPgetAttackZokusei(void);
s32 BLPgetBindTime(void);
s32 BLPgetDamageRank(s32, s32);
s32 BLPgetDamageValue(void);
s32 BLPgetPoisonTime(char*);
s32 BLPgetPowerDownTime(char*);
s32 BLPgetSleepTime(void);
s32 BLPgetStunTime(s32, s32);
bool BLPisHitCheck(void);
s32 BLTcalcEXPUp(void);
void BLTcalcSkillUp(bool, bool);
s32 BLTgetAttackDownTime(char*);
char* BLTgetAttributeStr(void);
s32 BLTgetBindTime(void);
s32 BLTgetDamageRank(s32, bool);
s32 BLTgetDamageValue(bool);
s32 BLTgetPoisonTime(char*);
s32 BLTgetPowerDownTime(char*);
s32 BLTgetShootRamNo(void);
s32 BLTgetSleepTime(void);
s32 BLTgetStunTime(s32, bool);
bool BLTisCritical(void);
bool BLTisHitCheck(void);
bool BLTisPlayerBullet(void);
void CFLclear(void);
char* CFLgetFolderName(void);
void CFLsetName(char*);
s32 COUNTERget(s32);
void COUNTERset(s32, s32);
void DMcase(s32);
void DMchkSD_0(char*, char*, s32, char*, char*, char*);
void DMchkSD_1(char*, char*, s32, char*, char*);
s32 DMgetCount(void);
void DMmsgBox(char*, s32);
void DMrun(void);
void DMsetLua(char*);
void DMsetSD_0(char*, char*, s32, char*, char*, s64);
void DMsetSD_1(char*, char*, s32, char*, char*);
void DMsetSD_2(char*, char*, s32, char*, s64);
void DMsetSD_3(char*, char*, s32, char*);
void DMswtEnd(void);
void DMswtSD_0(char*, char*, s32, char*, char*);
void DMswtSD_1(char*, char*, s32, char*);
void FLGflip(s32);
bool FLGisOn(s32);
void FLGset(s32, bool);
void LISsetDifficultMax(s32);
s32 MATgetPlayerMatStat(s32);
s32 MATgetPlayerNo(void);
bool MATisAllPlayer(void);
bool MATisPlayer(void);
bool MATisRideOffPlayer(s32);
bool MATisRidePlayer(s32);
void MATsetRun(bool);
void MDLaddHp(s32);
void MDLdspDamage(s32);
void MDLdspDamageP(s32);
void MDLdspExp(s32);
s32 MDLgetBulletBoxNo(void);
s32 MDLgetBulletKind(void);
char* MDLgetBulletName(void);
s32 MDLgetHp(void);
char* MDLgetInfoName(void);
f32 MDLgetKyoriSquare(f32, f32, f32);
s32 MDLgetMHp(void);
char* MDLgetPosStr(f32, f32, f32);
s32 MDLgetRamNo(void);
s32 MDLgetRecNo(void);
char* MDLgetRotStr(f32, f32, f32);
bool MDLisCallName(char*);
bool MDLisLive(char*, s32);
bool MDLisSeqName(char*);
bool MDLisStatus(char*, s32, char*);
void MDLsetDMGTurn(s32);
void MDLsetSD(char*, char*);
void MDLsetSeq(char*);
void MDLsetWorldPosTarget_0(f32, f32, f32);
void MDLsetWorldPosTarget_1(char*);
void MsgBox(char*, s32);
char* PLAYERgetDataStr(s32, s32, f32);
s32 PLAYERgetPos(void);
char* PLAYERgetPosStr(s32, f32, f32, f32);
s32 SYSctlBit(char*, s32, s32);
s32 SYSctlCardFolder(char*, s32);
bool SYSctlDeviceFolder(char*, s32, char*, s32, s32, s32);
bool SYSctlDiskFolder(char*, s32, s32, s32);
bool SYSctlFlagFolder(char*, s32);
s32 SYSctlPlayer(char*, char*, s32, s32);
s32 SYSctlShinka(char*, s32, s32);
s32 SYSgetArea(void);
s32 SYSgetCameraMode(void);
s32 SYSgetCountry(void);
s32 SYSgetDifficulty(void);
char* SYSgetGateStageName(void);
s32 SYSgetGateStageNo(void);
s32 SYSgetIsPackOnly(void);
s32 SYSgetLightMode(void);
s32 SYSgetPlayerDigimon(s32);
s32 SYSgetPlayerLevel(s32);
char* SYSgetPlayerName(s32);
s32 SYSgetPlayerNinzu(void);
s32 SYSgetRandom(s32);
s32 SYSgetTarget(void);
char* SYSgetWeaponName(s32, s32);
bool SYSisGetShinkaDigimon(s32, s32);
bool SYSisJoinPlayer(s32);
void SYSsetDifficulty(s32);
void SYSsetGateStage(char*, s32);
void ShutDown(void);
s32 TXTgetCallNo(void);
void sd_0(char*, char*, s32, char*, char*);
void sd_1(char*, char*, s32, char*);
void sd_2(char*, char*, s32, char*, char*, s64);
void sd_3(char*, char*, s32, char*, s64);

/* Script arguments: 0x18 bytes each, value at +0x10. */
typedef struct {
    u8 pad0[0x10];
    union {
        s32 i;
        bool b;
        f32 f;
        char* s;
        s64 l;
    } v;
} ScriptArg;

typedef struct {
    u8 pad0[0xC];
    char* name;
} ScriptClass;

/* Registered command (built by func_0023F4F0). */
typedef struct {
    u8 pad0[0xC];
    char* name;
    u8 unk10[0xC];
    ScriptClass* cls;
    u8 pad20[0x8];
    s16 id;
    u16 flags;
} ScriptCmd;

extern char D_0050D600[]; /* "Can't find method %s::%s" */
ScriptCmd* lua_touserdata(void* L, s32 index);
void lua_pushnumber(void* L, double n);
void func_0023D070(void* str, void* L, const char* s); /* push string */
void igLuaState_reportError(const char* fmt, ...);

/* Script command dispatcher: switch on the command id, call the
   implementation with its arguments and push the result. */
s32 func_00370780(void* L) {
    ScriptCmd* cmd = lua_touserdata(L, -1);
    ScriptArg* args = (cmd->flags & 0xFFF) ? (ScriptArg*)lua_touserdata(L, -2) : NULL;

    switch (cmd->id) {
    case 0:
        lua_pushnumber(L, (double)(u32)SYSisJoinPlayer(args[0].v.i));
        return 1;
    case 1:
        func_0023D070(&cmd->unk10, L, SYSgetPlayerName(args[0].v.i));
        return 1;
    case 2:
        lua_pushnumber(L, (double)SYSgetPlayerNinzu());
        return 1;
    case 3:
        func_0023D070(&cmd->unk10, L, SYSgetWeaponName(args[0].v.i, args[1].v.i));
        return 1;
    case 4:
        lua_pushnumber(L, (double)SYSgetRandom(args[0].v.i));
        return 1;
    case 5:
        lua_pushnumber(L, (double)SYSgetDifficulty());
        return 1;
    case 6:
        SYSsetDifficulty(args[0].v.i);
        return 0;
    case 7:
        lua_pushnumber(L, (double)SYSgetCameraMode());
        return 1;
    case 8:
        lua_pushnumber(L, (double)SYSgetLightMode());
        return 1;
    case 9:
        lua_pushnumber(L, (double)SYSgetArea());
        return 1;
    case 10:
        lua_pushnumber(L, (double)SYSgetCountry());
        return 1;
    case 11:
        lua_pushnumber(L, (double)SYSgetTarget());
        return 1;
    case 12:
        lua_pushnumber(L, (double)SYSgetIsPackOnly());
        return 1;
    case 13:
        SYSsetGateStage(args[0].v.s, args[1].v.i);
        return 0;
    case 14:
        func_0023D070(&cmd->unk10, L, SYSgetGateStageName());
        return 1;
    case 15:
        lua_pushnumber(L, (double)SYSgetGateStageNo());
        return 1;
    case 16:
        lua_pushnumber(L, (double)(u32)SYSctlDeviceFolder(args[0].v.s, args[1].v.i, args[2].v.s, args[3].v.i, args[4].v.i, args[5].v.i));
        return 1;
    case 17:
        lua_pushnumber(L, (double)(u32)SYSctlDiskFolder(args[0].v.s, args[1].v.i, args[2].v.i, args[3].v.i));
        return 1;
    case 18:
        lua_pushnumber(L, (double)(u32)SYSctlFlagFolder(args[0].v.s, args[1].v.i));
        return 1;
    case 19:
        lua_pushnumber(L, (double)SYSgetPlayerLevel(args[0].v.i));
        return 1;
    case 20:
        lua_pushnumber(L, (double)SYSgetPlayerDigimon(args[0].v.i));
        return 1;
    case 21:
        lua_pushnumber(L, (double)(u32)SYSisGetShinkaDigimon(args[0].v.i, args[1].v.i));
        return 1;
    case 22:
        lua_pushnumber(L, (double)SYSctlCardFolder(args[0].v.s, args[1].v.i));
        return 1;
    case 23:
        lua_pushnumber(L, (double)SYSctlBit(args[0].v.s, args[1].v.i, args[2].v.i));
        return 1;
    case 24:
        lua_pushnumber(L, (double)SYSctlShinka(args[0].v.s, args[1].v.i, args[2].v.i));
        return 1;
    case 25:
        lua_pushnumber(L, (double)SYSctlPlayer(args[0].v.s, args[1].v.s, args[2].v.i, args[3].v.i));
        return 1;
    case 26:
        CFLclear();
        return 0;
    case 27:
        CFLsetName(args[0].v.s);
        return 0;
    case 28:
        func_0023D070(&cmd->unk10, L, CFLgetFolderName());
        return 1;
    case 29:
        sd_0(args[0].v.s, args[1].v.s, args[2].v.i, args[3].v.s, args[4].v.s);
        return 0;
    case 30:
        sd_1(args[0].v.s, args[1].v.s, args[2].v.i, args[3].v.s);
        return 0;
    case 31:
        sd_2(args[0].v.s, args[1].v.s, args[2].v.i, args[3].v.s, args[4].v.s, args[5].v.l);
        return 0;
    case 32:
        sd_3(args[0].v.s, args[1].v.s, args[2].v.i, args[3].v.s, args[4].v.l);
        return 0;
    case 33:
        lua_pushnumber(L, (double)(u32)MATisPlayer());
        return 1;
    case 34:
        lua_pushnumber(L, (double)(u32)MATisAllPlayer());
        return 1;
    case 35:
        lua_pushnumber(L, (double)MATgetPlayerNo());
        return 1;
    case 36:
        lua_pushnumber(L, (double)(s32)MATisRidePlayer(args[0].v.i));
        return 1;
    case 37:
        lua_pushnumber(L, (double)(s32)MATisRideOffPlayer(args[0].v.i));
        return 1;
    case 38:
        lua_pushnumber(L, (double)MATgetPlayerMatStat(args[0].v.i));
        return 1;
    case 39:
        MATsetRun(args[0].v.b);
        return 0;
    case 40:
        DMsetSD_0(args[0].v.s, args[1].v.s, args[2].v.i, args[3].v.s, args[4].v.s, args[5].v.l);
        return 0;
    case 41:
        DMsetSD_1(args[0].v.s, args[1].v.s, args[2].v.i, args[3].v.s, args[4].v.s);
        return 0;
    case 42:
        DMsetSD_2(args[0].v.s, args[1].v.s, args[2].v.i, args[3].v.s, args[4].v.l);
        return 0;
    case 43:
        DMsetSD_3(args[0].v.s, args[1].v.s, args[2].v.i, args[3].v.s);
        return 0;
    case 44:
        DMchkSD_0(args[0].v.s, args[1].v.s, args[2].v.i, args[3].v.s, args[4].v.s, args[5].v.s);
        return 0;
    case 45:
        DMchkSD_1(args[0].v.s, args[1].v.s, args[2].v.i, args[3].v.s, args[4].v.s);
        return 0;
    case 46:
        DMswtSD_0(args[0].v.s, args[1].v.s, args[2].v.i, args[3].v.s, args[4].v.s);
        return 0;
    case 47:
        DMswtSD_1(args[0].v.s, args[1].v.s, args[2].v.i, args[3].v.s);
        return 0;
    case 48:
        DMswtEnd();
        return 0;
    case 49:
        DMcase(args[0].v.i);
        return 0;
    case 50:
        DMsetLua(args[0].v.s);
        return 0;
    case 51:
        DMmsgBox(args[0].v.s, args[1].v.i);
        return 0;
    case 52:
        DMrun();
        return 0;
    case 53:
        lua_pushnumber(L, (double)DMgetCount());
        return 1;
    case 54:
        lua_pushnumber(L, (double)(u32)FLGisOn(args[0].v.i));
        return 1;
    case 55:
        FLGset(args[0].v.i, args[1].v.b);
        return 0;
    case 56:
        FLGflip(args[0].v.i);
        return 0;
    case 57:
        COUNTERset(args[0].v.i, args[1].v.i);
        return 0;
    case 58:
        lua_pushnumber(L, (double)COUNTERget(args[0].v.i));
        return 1;
    case 59:
        func_0023D070(&cmd->unk10, L, MDLgetInfoName());
        return 1;
    case 60:
        lua_pushnumber(L, (double)MDLgetRamNo());
        return 1;
    case 61:
        lua_pushnumber(L, (double)MDLgetRecNo());
        return 1;
    case 62:
        lua_pushnumber(L, (double)MDLgetMHp());
        return 1;
    case 63:
        lua_pushnumber(L, (double)MDLgetHp());
        return 1;
    case 64:
        func_0023D070(&cmd->unk10, L, MDLgetPosStr(args[0].v.f, args[1].v.f, args[2].v.f));
        return 1;
    case 65:
        func_0023D070(&cmd->unk10, L, MDLgetRotStr(args[0].v.f, args[1].v.f, args[2].v.f));
        return 1;
    case 66:
        MDLaddHp(args[0].v.i);
        return 0;
    case 67:
        MDLsetSeq(args[0].v.s);
        return 0;
    case 68:
        MDLsetSD(args[0].v.s, args[1].v.s);
        return 0;
    case 69:
        lua_pushnumber(L, (double)(u32)MDLisLive(args[0].v.s, args[1].v.i));
        return 1;
    case 70:
        lua_pushnumber(L, (double)MDLgetBulletKind());
        return 1;
    case 71:
        func_0023D070(&cmd->unk10, L, MDLgetBulletName());
        return 1;
    case 72:
        lua_pushnumber(L, (double)MDLgetBulletBoxNo());
        return 1;
    case 73:
        MDLdspDamage(args[0].v.i);
        return 0;
    case 74:
        MDLdspDamageP(args[0].v.i);
        return 0;
    case 75:
        MDLdspExp(args[0].v.i);
        return 0;
    case 76:
        MDLsetDMGTurn(args[0].v.i);
        return 0;
    case 77:
        lua_pushnumber(L, (double)(u32)MDLisCallName(args[0].v.s));
        return 1;
    case 78:
        MDLsetWorldPosTarget_0(args[0].v.f, args[1].v.f, args[2].v.f);
        return 0;
    case 79:
        MDLsetWorldPosTarget_1(args[0].v.s);
        return 0;
    case 80:
        lua_pushnumber(L, (double)MDLgetKyoriSquare(args[0].v.f, args[1].v.f, args[2].v.f));
        return 1;
    case 81:
        lua_pushnumber(L, (double)(u32)MDLisSeqName(args[0].v.s));
        return 1;
    case 82:
        lua_pushnumber(L, (double)(u32)MDLisStatus(args[0].v.s, args[1].v.i, args[2].v.s));
        return 1;
    case 83:
        lua_pushnumber(L, (double)(u32)BLTisPlayerBullet());
        return 1;
    case 84:
        lua_pushnumber(L, (double)BLTgetShootRamNo());
        return 1;
    case 85:
        lua_pushnumber(L, (double)(u32)BLTisHitCheck());
        return 1;
    case 86:
        lua_pushnumber(L, (double)BLTgetDamageValue(args[0].v.b));
        return 1;
    case 87:
        lua_pushnumber(L, (double)BLTgetDamageRank(args[0].v.i, args[1].v.b));
        return 1;
    case 88:
        lua_pushnumber(L, (double)(u32)BLTisCritical());
        return 1;
    case 89:
        lua_pushnumber(L, (double)BLTgetStunTime(args[0].v.i, args[1].v.b));
        return 1;
    case 90:
        lua_pushnumber(L, (double)BLTgetPoisonTime(args[0].v.s));
        return 1;
    case 91:
        lua_pushnumber(L, (double)BLTgetSleepTime());
        return 1;
    case 92:
        lua_pushnumber(L, (double)BLTgetBindTime());
        return 1;
    case 93:
        lua_pushnumber(L, (double)BLTgetAttackDownTime(args[0].v.s));
        return 1;
    case 94:
        lua_pushnumber(L, (double)BLTgetPowerDownTime(args[0].v.s));
        return 1;
    case 95:
        BLTcalcSkillUp(args[0].v.b, args[1].v.b);
        return 0;
    case 96:
        lua_pushnumber(L, (double)BLTcalcEXPUp());
        return 1;
    case 97:
        func_0023D070(&cmd->unk10, L, BLTgetAttributeStr());
        return 1;
    case 98:
        lua_pushnumber(L, (double)(u32)BLPisHitCheck());
        return 1;
    case 99:
        lua_pushnumber(L, (double)BLPgetDamageValue());
        return 1;
    case 100:
        func_0023D070(&cmd->unk10, L, BLPgetAttackZokusei());
        return 1;
    case 101:
        lua_pushnumber(L, (double)BLPgetDamageRank(args[0].v.i, args[1].v.i));
        return 1;
    case 102:
        lua_pushnumber(L, (double)BLPgetStunTime(args[0].v.i, args[1].v.i));
        return 1;
    case 103:
        lua_pushnumber(L, (double)BLPgetPoisonTime(args[0].v.s));
        return 1;
    case 104:
        lua_pushnumber(L, (double)BLPgetSleepTime());
        return 1;
    case 105:
        lua_pushnumber(L, (double)BLPgetBindTime());
        return 1;
    case 106:
        lua_pushnumber(L, (double)BLPgetAttackDownTime(args[0].v.s));
        return 1;
    case 107:
        lua_pushnumber(L, (double)BLPgetPowerDownTime(args[0].v.s));
        return 1;
    case 108:
        lua_pushnumber(L, (double)BLPcalcKoukaMagic());
        return 1;
    case 109:
        lua_pushnumber(L, (double)TXTgetCallNo());
        return 1;
    case 110:
        lua_pushnumber(L, (double)PLAYERgetPos());
        return 1;
    case 111:
        func_0023D070(&cmd->unk10, L, PLAYERgetDataStr(args[0].v.i, args[1].v.i, args[2].v.f));
        return 1;
    case 112:
        func_0023D070(&cmd->unk10, L, PLAYERgetPosStr(args[0].v.i, args[1].v.f, args[2].v.f, args[3].v.f));
        return 1;
    case 113:
        LISsetDifficultMax(args[0].v.i);
        return 0;
    case 114:
        MsgBox(args[0].v.s, args[1].v.i);
        return 0;
    case 115:
        ShutDown();
        return 0;
    default:
        igLuaState_reportError(D_0050D600, cmd->cls->name, cmd->name);
        return 0;
    }
}

/* Two script object classes, registered with the engine under the names
 * "be" (parent "igInfoManager") and "Bec". 0x5C bytes, allocated from the heap
 * at D_0056DE30 through class operators; vtable pointer at +0x58. */
class be {
public:
    be();
    static void* operator new(size_t size) { return func_002ED240(size, D_0056DE30); }
    static void operator delete(void* p, size_t) { func_002ED4C0(p, D_0056DE30); }
    u8 pad0[0x5C];
};

class Bec {
public:
    Bec();
    static void* operator new(size_t size) { return func_002ED240(size, D_0056DE30); }
    static void operator delete(void* p, size_t) { func_002ED4C0(p, D_0056DE30); }
    u8 pad0[0x5C];
};

be::be() {
    func_00241930(this, D_0050CEB8, D_0050D7F0, 0, 0, 0, 0, 0);
    *(void**)((u8*)this + 0x58) = D_00539F00;
}

void func_0036E160(void);

be* func_00371840(void) {
    func_0036E160();
    return new be();
}

Bec::Bec() {
    func_00241930(this, D_0050C908, NULL, 0, 0, 0, 0, 0);
    *(void**)((u8*)this + 0x58) = D_00539EF0;
}

Bec* func_00371930(void) {
    return new Bec();
}

void func_003719C0(void) {
}

/* Register the 116 script commands (see docs/script_bindings.md). */
void func_003719D0(void* self) {
    func_0023F4F0(self, 0, func_00370780, D_004C8560, 0x74, D_0050CFC0, 0);
}

void be_registerToInsight(void) {
}

void be_unRegisterFromInsight(void) {
}

/* Look up the objects the bindings use (types at beSystem_Meta etc.). */
void be_begin(UnkStruct_00371A30* self) {
    D_0058A1B8 = (UnkStruct_0058A1B8*)func_00236F80(self->unkC, beSystem_Meta);
    D_0058A1C0 = func_00236F80(self->unkC, beMessenger_Meta);
    D_0058A1C8 = func_00236F80(self->unkC, beMatCtrl_Meta);
    D_0058A1D0 = func_00236F80(self->unkC, beDemoManager_Meta);
    D_0058A1D8 = self;
}

void be_end(void) {
}

s32 be_appendInfo(void) {
    return 0;
}

/* Release every string in D_0058A1D8->unk10 and empty it. */
void be_removeAllInfos(void) {
    StrArray* list = D_0058A1D8->unk10;
    s32 count = list->count;
    s32 i;

    for (i = 0; i < count; i++) {
        AssignStr(&list->data[i], NULL);
    }
    list->count = 0;
    if ((D_0058A1D8->unk10->count > 0) == false) {
        func_002D6680(D_0058A1D8->unk10, 0, 4);
    }
}

void be_update(void) {
}

/* GAMERAM::GETCOMMON (index 0) or GAMERAM::GETPLAYERn (index n). */
void* func_00371C10(s32 index) {
    beMessenger_call(D_0058A1C0, D_0050D940, 0, 0, D_004C8720[index]);
    return beMessenger_getObject(D_0058A1C0, 0);
}

bool SYSisJoinPlayer(s32 player) {
    return beDataObjList_getBool(func_00371C10(player + 1), D_0050D948)->value;
}

char* SYSgetPlayerName(s32 player) {
    return beDataObjList_getString(func_00371C10(player + 1), D_0050D950)->value;
}


/* Name of the weapon in a player's power socket slot (NULL past the end). The
   pooled string comes back by value; its temporary reference is dropped. */
char* SYSgetWeaponName(s32 player, s32 slot) {
    StrArray* names = beDataObjList_getStringList(func_00371C10(player + 1), D_0050D960)->value;
    char* tmp;
    char* r;

    if (names->count <= slot) {
        return NULL;
    }
    func_001C64E0(&tmp, names, slot);
    r = tmp;
    ReleaseStr(r);
    return r;
}

s32 SYSgetPlayerNinzu(void) {
    return beDataObjList_getInt(func_00371C10(0), D_0050D970)->value;
}

s32 SYSgetDifficulty(void) {
    return beDataObjList_getInt(func_00371C10(0), D_0050D978)->value;
}

void SYSsetDifficulty(s32 difficulty) {
    beDataObjList_getInt(func_00371C10(0), D_0050D978)->value = difficulty;
}

s32 SYSgetCameraMode(void) {
    beMessenger_callGroup(D_0058A1C0, D_0050D940, D_0050D8F0);
    return beDataObjList_getInt(beMessenger_getObject(D_0058A1C0, 0), D_0050D988)->value;
}

s32 SYSgetLightMode(void) {
    beMessenger_callGroup(D_0058A1C0, D_0050D940, D_0050D8F0);
    return beDataObjList_getInt(beMessenger_getObject(D_0058A1C0, 0), D_0050D998)->value;
}

s32 SYSgetArea(void) {
    return 1;
}

s32 SYSgetCountry(void) {
    return 1;
}

s32 SYSgetTarget(void) {
    return 1;
}

s32 SYSgetIsPackOnly(void) {
    return 1;
}

s32 SYSgetRandom(s32 n) {
    return D_0058A1B8->random->Next() % (u32)n;
}

void SYSsetGateStage(char* name, s32 no) {
    void* params;

    beMessenger_callGroup(D_0058A1C0, D_0050D940, D_0050D8F0);
    params = beMessenger_getObject(D_0058A1C0, 0);
    beDataObjList_getInt(params, D_0050D9A8)->value = no;
    if (name == NULL) {
        SetStr(beDataObjList_getString(params, D_0050D9B8), D_0050D9C8);
    } else {
        SetStr(beDataObjList_getString(params, D_0050D9B8), name);
    }
}

char* SYSgetGateStageName(void) {
    beMessenger_callGroup(D_0058A1C0, D_0050D940, D_0050D8F0);
    return beDataObjList_getString(beMessenger_getObject(D_0058A1C0, 0), D_0050D9B8)->value;
}

s32 SYSgetGateStageNo(void) {
    beMessenger_callGroup(D_0058A1C0, D_0050D940, D_0050D8F0);
    return beDataObjList_getInt(beMessenger_getObject(D_0058A1C0, 0), D_0050D9A8)->value;
}

/* GAMERAM::CTRLITEM("DEVICE", name, arg2, arg1, arg3, arg4, arg5) */
bool SYSctlDeviceFolder(char* name, s32 arg1, char* arg2, s32 arg3, s32 arg4, s32 arg5) {
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050D9D0);
    beMessenger_pushString(D_0058A1C0, name);
    beMessenger_pushString(D_0058A1C0, arg2);
    beMessenger_pushInt(D_0058A1C0, arg1);
    beMessenger_pushInt(D_0058A1C0, arg3);
    beMessenger_pushInt(D_0058A1C0, arg4);
    beMessenger_pushInt(D_0058A1C0, arg5);
    beMessenger_send(D_0058A1C0, D_0050D9D8);
    return beMessenger_getBool(D_0058A1C0, 0);
}

/* GAMERAM::CTRLITEM("DISK", name, "", arg1, arg2, 0, arg3) */
bool SYSctlDiskFolder(char* name, s32 arg1, s32 arg2, s32 arg3) {
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050D9E8);
    beMessenger_pushString(D_0058A1C0, name);
    beMessenger_pushString(D_0058A1C0, D_0050D9C8);
    beMessenger_pushInt(D_0058A1C0, arg1);
    beMessenger_pushInt(D_0058A1C0, arg2);
    beMessenger_pushInt(D_0058A1C0, 0);
    beMessenger_pushInt(D_0058A1C0, arg3);
    beMessenger_send(D_0058A1C0, D_0050D9D8);
    return beMessenger_getBool(D_0058A1C0, 0);
}

/* GAMERAM::CTRLITEM("FLAG", name, "", 0, value, 0, 0) */
bool SYSctlFlagFolder(char* name, s32 value) {
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050D9F0);
    beMessenger_pushString(D_0058A1C0, name);
    beMessenger_pushString(D_0058A1C0, D_0050D9C8);
    beMessenger_pushInt(D_0058A1C0, 0);
    beMessenger_pushInt(D_0058A1C0, value);
    beMessenger_pushInt(D_0058A1C0, 0);
    beMessenger_pushInt(D_0058A1C0, 0);
    beMessenger_send(D_0058A1C0, D_0050D9D8);
    return beMessenger_getBool(D_0058A1C0, 0);
}

/* GAMERAM::CTRLITEM("BIT", name, "", arg1, arg2, 0, 0) */
s32 SYSctlBit(char* name, s32 arg1, s32 arg2) {
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050D9F8);
    beMessenger_pushString(D_0058A1C0, name);
    beMessenger_pushString(D_0058A1C0, D_0050D9C8);
    beMessenger_pushInt(D_0058A1C0, arg1);
    beMessenger_pushInt(D_0058A1C0, arg2);
    beMessenger_pushInt(D_0058A1C0, 0);
    beMessenger_pushInt(D_0058A1C0, 0);
    beMessenger_send(D_0058A1C0, D_0050D9D8);
    return beMessenger_getInt(D_0058A1C0, 0);
}

s32 SYSgetPlayerLevel(s32 player) {
    return beDataObjList_getInt(func_00371C10(player + 1), D_0050DA00)->value;
}

s32 SYSgetPlayerDigimon(s32 player) {
    return beDataObjList_getInt(func_00371C10(player + 1), D_0050DA08)->value;
}

bool SYSisGetShinkaDigimon(s32 player, s32 digimon) {
    BoolArray* used;

    used = beDataObjList_getBoolList(func_00371C10(player + 1), D_0050DA18)->value;
    return used->data[digimon];
}

extern char D_0050DA28[]; /* "CARDLIST" */
extern char D_0050DA38[]; /* "KAZU" */
extern char D_0050DA40[]; /* "SET" */
extern char D_0050DA48[]; /* "CLR" */

/* Card folder: "KAZU" counts owned cards, "SET"/"CLR" change one card and
   return 1 if it changed. */
s32 SYSctlCardFolder(char* cmd, s32 card) {
    BoolArray* cards = beDataObjList_getBoolList(func_00371C10(1), D_0050DA28)->value;

    if (strcmp(cmd, D_0050DA38) == 0) {
        s32 n = 0;
        s32 count = cards->count;
        s32 i;

        for (i = 0; i < count; i++) {
            if (cards->data[i] == 1) {
                n++;
            }
        }
        return n;
    }
    if (strcmp(cmd, D_0050DA40) == 0) {
        if (cards->data[card]) {
            return 0;
        }
        SetBool(cards, card, true);
        return 1;
    }
    if (strcmp(cmd, D_0050DA48) == 0) {
        if (!cards->data[card]) {
            return 0;
        }
        SetBool(cards, card, false);
        return 1;
    }
    return 0;
}

extern char D_0050DA50[]; /* "GETEXP" */
extern char D_0050DA58[]; /* "BASE_EXP" */
extern char D_0050DA68[]; /* "GETLEVEL" */
extern char D_0050DA78[]; /* "BASE_LEVEL" */
extern char D_0050DA88[]; /* "SETSHINKA" */

/* Nonmatching: the -1 returns sit in their own blocks in the original;
 * MWCC hoists `li v0, -1` into the beqz delay slot. Tried ternary, inverted
 * ifs, == false, else, an inline getter. */
#ifdef NON_MATCHING
/* Evolution data of a player's digimon: "GETEXP" / "GETLEVEL" (-1 if not
   unlocked), "SETSHINKA" unlocks it (1 if it changed). */
s32 SYSctlShinka(char* cmd, s32 player, s32 digimon) {
    u8 used = beDataObjList_getBoolList(func_00371C10(player + 1), D_0050DA18)->value->data[digimon];

    if (strcmp(cmd, D_0050DA50) == 0) {
        if (used) {
            return beDataObjList_getIntList(func_00371C10(player + 1), D_0050DA58)->value->data[digimon];
        }
        return -1;
    }
    if (strcmp(cmd, D_0050DA68) == 0) {
        if (used) {
            return beDataObjList_getIntList(func_00371C10(player + 1), D_0050DA78)->value->data[digimon];
        }
        return -1;
    }
    if (strcmp(cmd, D_0050DA88) == 0) {
        if (used) {
            return 0;
        }
        SetBool(beDataObjList_getBoolList(func_00371C10(player + 1), D_0050DA18)->value, digimon, true);
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/script_bind", SYSctlShinka);
#endif

/* GAMERAM::CTRLPLAYER(arg0, arg1, arg2, arg3) */
s32 SYSctlPlayer(char* arg0, char* arg1, s32 arg2, s32 arg3) {
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, arg0);
    beMessenger_pushString(D_0058A1C0, arg1);
    beMessenger_pushInt(D_0058A1C0, arg2);
    beMessenger_pushInt(D_0058A1C0, arg3);
    beMessenger_send(D_0058A1C0, D_0050DA98);
    return beMessenger_getInt(D_0058A1C0, 0);
}

void CFLclear(void) {
    StrArray* list = D_0058A1B8->unk34;
    s32 count = list->count;
    s32 i;

    for (i = 0; i < count; i++) {
        AssignStr(&list->data[i], NULL);
    }
    list->count = 0;
    if ((D_0058A1B8->unk34->count > 0) == false) {
        func_002D6680(D_0058A1B8->unk34, 0, 4);
    }
}

void func_001CA620(StrArray* array, char** str); /* append */

/* Add name to the card folder name list unless it is already there. */
void CFLsetName(char* name) {
    StrArray* list = D_0058A1B8->unk34;
    s32 count = list->count;
    bool found = false;
    s32 i;
    char* tmp;
    char* str;

    for (i = 0; i < count; i++) {
        func_001C64E0(&tmp, list, i);
        if (strcmp(tmp, name) == 0) {
            found = true;
            ReleaseStr(tmp);
            break;
        }
        ReleaseStr(tmp);
    }
    if (found == false) {
        str = name == NULL ? NULL : func_002EAF60(GetStrPool(), name);
        func_001CA620(list, &str);
        ReleaseStr(str);
    }
}

char* CFLgetFolderName(void) {
    return D_0058A1B8->unk38;
}

extern char D_0050DAB0[]; /* " < %f , %f , %f > " */
u32 strlen(const char* s); /* strlen */
void beMessenger_pushFloat(void* obj, f32 value);
void beMessenger_pushLong(void* obj, s64 value);
void beMessenger_pushVec3f(void* obj, Vec3* value);
void func_0030B3C0(void* conv, const char* s);
void func_0030BBF0(void* conv, s32, s32);
void func_0030B630(void* conv, f32* out);

/* Nonmatching by one instruction: the original puts `move s0, a0` in the
 * delay slot of `jal strlen`, ours fills the `beqz` slot with it. Tried an
 * early return vs wrapping the body, a strlen temporary, a local copy of args. */
#ifdef NON_MATCHING
/* Push script arguments given as "a/b/c": "I<n>" int, "L<n>" 64-bit,
   "V<x,y,z>" vector, "F<x>" float, anything else a string. */
void func_00372CF0(char* args) {
    f32 f;
    Vec3 parsed;
    Vec3 vec;
    char* tokens[12];
    char buf[0x400];
    char* p;
    s32 count;
    s32 i;

    if (*args == 0) {
        return;
    }
    if (strlen(args) > 0x400) {
        args[0x3FF] = 0;
    }
    p = buf;
    count = 0;
    tokens[count++] = p;
    for (;;) {
        if (*args == 0) {
            *p = 0;
            break;
        }
        if (*args == '/') {
            *p = 0;
            args++;
            p++;
            tokens[count++] = p;
        } else {
            *p = *args;
            args++;
            p++;
        }
    }
    for (i = 0; i < count; i++) {
        char* tok = tokens[i];

        if (tok[1] != '<') {
            beMessenger_pushString(D_0058A1C0, tok);
            continue;
        }
        switch (tok[0]) {
        case 'I': {
            s32 n = 0;
            bool neg = false;
            s32 j = 2;

            if (tok[2] == '-') {
                neg = true;
                j++;
            }
            while (tok[j] != '>') {
                n *= 10;
                n += tok[j++] - '0';
            }
            if (neg) {
                n = -n;
            }
            beMessenger_pushInt(D_0058A1C0, n);
            break;
        }
        case 'L': {
            s64 n = 0;
            s32 j = 2;

            while (tok[j] != '>') {
                n *= 10;
                n += tok[j++] - '0';
            }
            beMessenger_pushLong(D_0058A1C0, n);
            break;
        }
        case 'V':
            func_001E6D30(tok + 1, (f32*)&parsed, D_0050DAB0);
            vec = parsed;
            beMessenger_pushVec3f(D_0058A1C0, &vec);
            break;
        case 'F': {
            UnkStruct_Conv* conv = D_0058A1B8->unk48;

            func_0030B3C0(conv, tok + 2);
            func_0030BBF0(conv, conv->unkC - 1, 1);
            func_0030B630(conv, &f);
            beMessenger_pushFloat(D_0058A1C0, f);
            break;
        }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/script_bind", func_00372CF0);
#endif

/* cls::method(args...), args parsed from a string by func_00372CF0. */
void sd_0(char* cls, char* arg0, s32 arg1, char* method, char* args) {
    beMessenger_select(D_0058A1C0, cls, arg0, arg1);
    if (args != NULL) {
        func_00372CF0(args);
    }
    beMessenger_send(D_0058A1C0, method);
}

void sd_1(char* cls, char* arg0, s32 arg1, char* method) {
    beMessenger_call(D_0058A1C0, cls, arg0, arg1, method);
}

void sd_2(char* cls, char* arg0, s32 arg1, char* method, char* args, s64 arg5) {
    beMessenger_delaySelect(D_0058A1C0, cls, arg0, arg1);
    if (args != NULL) {
        func_00372CF0(args);
    }
    beMessenger_delaySend(D_0058A1C0, method, arg5);
}

void sd_3(char* arg0, char* arg1, s32 arg2, char* arg3, s64 arg4) {
    beMessenger_delayCall(D_0058A1C0, arg0, arg1, arg2, arg3, arg4);
}

bool MATisPlayer(void) {
    return func_0038ACA0(D_0058A1C8);
}

bool MATisAllPlayer(void) {
    return func_0038ACD0(D_0058A1C8);
}

s32 MATgetPlayerNo(void) {
    return func_0038AD40(D_0058A1C8);
}

bool MATisRidePlayer(s32 arg0) {
    return func_0038ADD0(D_0058A1C8, arg0);
}

bool MATisRideOffPlayer(s32 arg0) {
    return func_0038AE30(D_0058A1C8, arg0);
}

s32 MATgetPlayerMatStat(s32 arg0) {
    return func_0038AEA0(D_0058A1C8, arg0);
}

void MATsetRun(bool arg0) {
    func_0038AE90(D_0058A1C8, arg0);
}

void DMsetSD_0(char* arg0, char* arg1, s32 arg2, char* arg3, char* arg4, s64 arg5) {
    func_0037E3A0(D_0058A1D0, 0, arg0, arg1, arg2, arg3, arg4, arg5);
}

void DMsetSD_1(char* arg0, char* arg1, s32 arg2, char* arg3, char* arg4) {
    func_0037E3A0(D_0058A1D0, 0, arg0, arg1, arg2, arg3, arg4, 0);
}

void DMsetSD_2(char* arg0, char* arg1, s32 arg2, char* arg3, s64 arg4) {
    func_0037E3A0(D_0058A1D0, 0, arg0, arg1, arg2, arg3, NULL, arg4);
}

void DMsetSD_3(char* arg0, char* arg1, s32 arg2, char* arg3) {
    func_0037E3A0(D_0058A1D0, 0, arg0, arg1, arg2, arg3, NULL, 0);
}

void DMchkSD_0(char* arg0, char* arg1, s32 arg2, char* arg3, char* arg4, char* op) {
    if (strcmp(D_0050DAC8, op) == 0 || strcmp(D_0050DAD0, op) == 0) {
        func_0037E3A0(D_0058A1D0, 2, arg0, arg1, arg2, arg3, arg4, 0);
    } else if (strcmp(D_0050DAD8, op) == 0 || strcmp(D_0050DAE0, op) == 0) {
        func_0037E3A0(D_0058A1D0, 1, arg0, arg1, arg2, arg3, arg4, 0);
    }
}

void DMchkSD_1(char* arg0, char* arg1, s32 arg2, char* arg3, char* op) {
    if (strcmp(D_0050DAC8, op) == 0 || strcmp(D_0050DAD0, op) == 0) {
        func_0037E3A0(D_0058A1D0, 2, arg0, arg1, arg2, arg3, NULL, 0);
    } else if (strcmp(D_0050DAD8, op) == 0 || strcmp(D_0050DAE0, op) == 0) {
        func_0037E3A0(D_0058A1D0, 1, arg0, arg1, arg2, arg3, NULL, 0);
    }
}

void DMsetLua(char* arg0) {
    func_0037E6E0(D_0058A1D0, arg0);
}

void DMmsgBox(char* arg0, s32 arg1) {
    func_0037E890(D_0058A1D0, arg0, arg1);
}

void DMswtSD_0(char* arg0, char* arg1, s32 arg2, char* arg3, char* arg4) {
    func_0037E110(D_0058A1D0, arg0, arg1, arg2, arg3, arg4);
}

void DMswtSD_1(char* arg0, char* arg1, s32 arg2, char* arg3) {
    func_0037E110(D_0058A1D0, arg0, arg1, arg2, arg3, NULL);
}

void DMswtEnd(void) {
    func_0037E180(D_0058A1D0);
}

void DMcase(s32 arg0) {
    func_0037E2D0(D_0058A1D0, arg0);
}

void DMrun(void) {
    func_0037EA50(D_0058A1D0);
}

extern char D_0050DAE8[]; /* "SUBWND" */
extern char D_0050DAF0[]; /* "ISRUN" */

typedef struct {
    u8 pad0[0x8];
    s32 count;
} UnkCount;

/* Number of entries in the demo manager's list; when empty, -1 if a sub
   window is running (SUBWND::ISRUN), else 0. */
s32 DMgetCount(void) {
    s32 n = (*(UnkCount**)((u8*)D_0058A1D0 + 0x1C))->count;
    s32 r;

    if (n != 0) {
        r = n;
    } else {
        beMessenger_call(D_0058A1C0, D_0050DAE8, NULL, 0, D_0050DAF0);
        if (beMessenger_getBoolCount(D_0058A1C0) == 0) {
            r = 0;
        } else {
            r = -(beMessenger_getBool(D_0058A1C0, 0) != 0);
        }
    }
    return r;
}

bool FLGisOn(s32 flag) {
    void* params;
    BoolArray* flags;

    beMessenger_call(D_0058A1C0, D_0050D940, 0, 0, D_0050D8F0);
    params = beMessenger_getObject(D_0058A1C0, 0);
    if (flag == 1000) {
        return SYSgetPlayerNinzu() != 0;
    }
    flags = beDataObjList_getBoolList(params, D_0050D9F0)->value;
    return flags->data[flag];
}

void FLGset(s32 flag, bool value) {
    beMessenger_call(D_0058A1C0, D_0050D940, 0, 0, D_0050D8F0);
    SetBool(beDataObjList_getBoolList(beMessenger_getObject(D_0058A1C0, 0), D_0050D9F0)->value, flag, value);
}

void FLGflip(s32 flag) {
    void* params;

    beMessenger_call(D_0058A1C0, D_0050D940, 0, 0, D_0050D8F0);
    params = beMessenger_getObject(D_0058A1C0, 0);
    if (beDataObjList_getBoolList(params, D_0050D9F0)->value->data[flag]) {
        SetBool(beDataObjList_getBoolList(params, D_0050D9F0)->value, flag, false);
    } else {
        SetBool(beDataObjList_getBoolList(params, D_0050D9F0)->value, flag, true);
    }
}

void COUNTERset(s32 index, s32 value) {
    IntArray* counters;

    beMessenger_call(D_0058A1C0, D_0050D940, 0, 0, D_0050D8F0);
    counters = beDataObjList_getIntList(beMessenger_getObject(D_0058A1C0, 0), D_0050DAF8)->value;
    if (counters->count != 0 && index >= 0 && index < counters->count) {
        counters->data[index] = value;
    }
}

s32 COUNTERget(s32 index) {
    beMessenger_call(D_0058A1C0, D_0050D940, 0, 0, D_0050D8F0);
    return beDataObjList_getIntList(beMessenger_getObject(D_0058A1C0, 0), D_0050DAF8)->value->data[index];
}

s32 MDLgetRamNo(void) {
    return D_0058A1E4;
}

char* MDLgetInfoName(void) {
    return D_0058A1F8;
}

char* MDLgetPosStr(f32 x, f32 y, f32 z) {
    f32 mtx[16];
    f32 v[3];
    f32 pos[3];

    func_00190380(mtx);
    func_001908F0(mtx, D_0058A1E8);
    v[0] = x;
    v[1] = y;
    v[2] = z;
    func_00190A50(mtx, v);
    func_00190A30(mtx, pos);
    sprintf(D_0058A230, D_0050DB00, (double)pos[0], (double)pos[1], (double)pos[2]);
    return D_0058A230;
}

/* "V<x, y + model heading in degrees, z>" */
char* MDLgetRotStr(f32 x, f32 y, f32 z) {
    f32 mtx[16];
    f32 dir[3];
    f32 angle;

    func_00190380(mtx);
    func_001908F0(mtx, D_0058A1E8);
    func_00192F50(dir, D_004E1098, mtx);
    angle = func_00123640(dir[0], dir[2]);
    sprintf(D_0058A270, D_0050DB00, (double)x, (double)(57.29578f * angle + y), (double)z);
    return D_0058A270;
}

s32 MDLgetMHp(void) {
    return D_0058A1EC;
}

s32 MDLgetHp(void) {
    return D_0058A1F0;
}

void MDLaddHp(s32 arg0) {
    D_0058A1F0 += arg0;
    if (D_0058A1F0 < 0) {
        D_0058A1F0 = 0;
    }
}

void MDLsetSeq(char* method) {
    beMessenger_call(D_0058A1C0, D_0050DB10, D_0058A1F8, D_0058A1E4, method);
}

void MDLsetSD(char* arg0, char* arg1) {
    sd_0(D_0050DB10, D_0058A1F8, D_0058A1E4, arg0, arg1);
}

/* MODELCTRL::ISRUN: true if any result is true. */
}

/* getBool seen with an int result here (no andi): a C++-linkage overload, demangled back
   to beMessenger_getBool by mwsplice. */
struct MsgObj;
s32 beMessenger_getBool(MsgObj* m, s32 index);

static inline bool IsFalse(MsgObj* m, s32 index) {
    return !(beMessenger_getBool(m, index) != 0);
}

extern "C" {

bool MDLisLive(char* name, s32 ramNo) {
    s32 n;
    bool r;
    s32 i;

    beMessenger_call(D_0058A1C0, D_0050DB10, name, ramNo, D_0050DAF0);
    n = beMessenger_getBoolCount(D_0058A1C0);
    r = false;
    for (i = 0; i < n; i++) {
        if (IsFalse((MsgObj*)D_0058A1C0, i)) {
            continue;
        }
        r = true;
        break;
    }
    return r;
}

s32 MDLgetBulletKind(void) {
    return D_0058A1FC;
}

char* MDLgetBulletName(void) {
    return D_004C873C[D_0058A1FC / 10000];
}

s32 MDLgetBulletBoxNo(void) {
    return D_0058A200;
}

/* Floating number popups (damage, exp, ...): model, number controller and
   one animation sequence per digit count. */
typedef struct {
    char* model;
    char* counter;
    char* seq[5];
} NumPopup;

extern NumPopup D_004C8770[];
extern char D_0050DD00[]; /* "FIND_SPACE" */
extern char D_0050DD10[]; /* "GENERATE" */
extern char D_0050DD20[]; /* "NUMBERCTRL" */
extern char D_0050DD30[]; /* "SETVALUE" */

typedef struct {
    u8 pad0[0x8];
    s32 unk8;
} UnkStruct_Space;

void func_00373EF0(s32 value, s32 kind) {
    char* model = D_004C8770[kind].model;
    char* counter = D_004C8770[kind].counter;
    UnkStruct_Space* space;
    s32 digits;
    char* seq;
    f32 pos[3];
    Vec3 vec;

    beMessenger_call(D_0058A1C0, D_0050DB10, model, -1, D_0050DD00);
    if (beMessenger_getObjectCount(D_0058A1C0) != 0) {
        space = (UnkStruct_Space*)beMessenger_getObject(D_0058A1C0, 0);
        if (value < 10) {
            digits = 0;
        } else if (value < 100) {
            digits = 1;
        } else if (value < 1000) {
            digits = 2;
        } else if (value < 10000) {
            digits = 3;
        } else if (value < 100000) {
            digits = 4;
        } else {
            digits = 4;
            value = 99999;
        }
        seq = D_004C8770[kind].seq[digits];
        beMessenger_select(D_0058A1C0, D_0050DB10, model, space->unk8);
        beMessenger_pushString(D_0058A1C0, seq);
        func_00190A30(D_0058A1E8, pos);
        vec = *(Vec3*)pos;
        beMessenger_pushVec3f(D_0058A1C0, &vec);
        beMessenger_send(D_0058A1C0, D_0050DD10);
        beMessenger_select(D_0058A1C0, D_0050DD20, counter, space->unk8);
        beMessenger_pushInt(D_0058A1C0, value);
        beMessenger_send(D_0058A1C0, D_0050DD30);
    }
}

void MDLdspDamage(s32 arg0) {
    func_00373EF0(arg0, 0);
}

void MDLdspDamageP(s32 arg0) {
    func_00373EF0(arg0, 3);
}

void MDLdspExp(s32 arg0) {
    func_00373EF0(arg0, 1);
}

void MDLsetDMGTurn(s32 arg0) {
    D_0058A210[0] = arg0 == 1;
}

/* Nonmatching: the logic is
 *     if (D_0058A214 == NULL) return name == NULL;
 *     return strcmp(D_0058A214, name) == 0;   (strcmp)
 * but the original has a dead nop after the `b` that aligns the else block
 * to 8 bytes. Tried if/else, ternary, bool, inline helper. */
extern char* D_0058A214;

#ifdef NON_MATCHING
/* Nonmatching (distance 2): only the alignment nop after the `b` is missing
 * (the strcmp block starts 8-aligned in the original). Also tried: result
 * variable, != NULL first, ternaries, !strcmp, inline strcmp helper. */
bool MDLisCallName(char* name) {
    if (D_0058A214 == NULL) {
        return name == NULL;
    }
    return strcmp(D_0058A214, name) == 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/script_bind", MDLisCallName);
#endif

void MDLsetWorldPosTarget_0(f32 x, f32 y, f32 z) {
    f32* pos = D_0058A218;

    if (pos != NULL) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
    }
}

static inline void SetWorldPosTarget(f32 x, f32 y, f32 z) {
    f32* pos = D_0058A218;

    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}

void MDLsetWorldPosTarget_1(char* str) {
    f32 v[3];

    if (D_0058A218 != NULL) {
        func_001E6D30(str, v, D_0050DB00);
        SetWorldPosTarget(v[0], v[1], v[2]);
    }
}

f32 MDLgetKyoriSquare(f32 x, f32 y, f32 z) {
    f32 pos[3];

    func_00190A30(D_0058A1E8, pos);
    return (pos[0] - x) * (pos[0] - x) + (pos[1] - y) * (pos[1] - y) + (pos[2] - z) * (pos[2] - z);
}

s32 MDLgetRecNo(void) {
    return D_0058A1F4;
}

/* Nonmatching, registers only (model / new pointer swap s0 and s1). Lazily
 * creates the model's sequence object, then compares the "ONE" entry name. */
#ifdef NON_MATCHING
/* Alchemy object: reference count in the low 23 bits of +4. */
struct igObject {
    void* vtbl;
    u32 refCount;
};

void func_002FF820(igObject* o); /* release */

static inline void igRef(igObject* o) {
    if (o != NULL) {
        o->refCount++;
    }
}

static inline void igUnref(igObject* o) {
    if (o != NULL) {
        o->refCount--;
        if ((o->refCount & 0x7FFFFF) == 0) {
            func_002FF820(o);
        }
    }
}

struct igTempRef {
    igObject* p;
    igTempRef(igObject* o) : p(o) {}
    ~igTempRef() { igUnref(p); }
};

struct UnkStruct_SeqName {
    u8 pad0[0x14];
    char* name;
};

struct UnkStruct_Seq {
    u8 pad0[0x14];
    UnkStruct_SeqName* unk14;
};

struct UnkStruct_SeqModel {
    u8 pad0[0x28];
    igObject* unk28;
};

extern bool D_00554C14;
extern char D_0050DD40[]; /* "ONE" */
extern void* D_00587110;
extern void* D_00585BE0;
void* func_00301000(void* model);
void* func_002F88E0(void* p);
igObject* func_0036AE80(void* p);
UnkStruct_Seq* func_003022C0(igObject* o, void* type, const char* name);

static inline void Assign(igObject** dst, igObject* src) {
    igRef(src);
    igUnref(*dst);
    *dst = src;
}

static inline igObject* GetSeq(UnkStruct_SeqModel* model) {
    if (model->unk28 == NULL) {
        void* a = func_00301000(model);
        igTempRef tmp(func_0036AE80(D_00554C14 ? a : func_002F88E0(D_00587110)));

        Assign(&model->unk28, tmp.p);
    }
    return model->unk28;
}

bool MDLisSeqName(char* name) {
    return strcmp(func_003022C0(GetSeq((UnkStruct_SeqModel*)D_0058A1E0), D_00585BE0, D_0050DD40)->unk14->name, name) == 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/script_bind", MDLisSeqName);
#endif

/* MODELCTRL::CHECKSTATUS(status) */
bool MDLisStatus(char* arg0, s32 arg1, char* status) {
    beMessenger_select(D_0058A1C0, D_0050DB10, arg0, arg1);
    beMessenger_pushString(D_0058A1C0, status);
    beMessenger_send(D_0058A1C0, D_0050DD48);
    if (beMessenger_getBoolCount(D_0058A1C0) == 1) {
        return beMessenger_getBool(D_0058A1C0, 0);
    }
    return 0;
}

s32 PLAYERgetPos(void) {
    beMessenger_callGroup(D_0058A1C0, D_0050DD58, D_0050DD68);
    return beMessenger_getInt(D_0058A1C0, 0);
}

extern char D_0058A2B0[];
extern char D_0050DD80[]; /* "GETMATRIXDIRECT" */
extern char D_0050DD90[]; /* "V<%f,%f,%f>/V<0,%f,0>" */

/* Position and heading (degrees, plus rot) of PLAYERPOINT::GETMATRIXDIRECT. */
char* PLAYERgetDataStr(s32 arg0, s32 arg1, f32 rot) {
    f32 mtx[16];
    f32 dir[3];
    f32 pos[3];
    f32 angle;

    beMessenger_selectGroup(D_0058A1C0, D_0050DD58);
    beMessenger_pushInt(D_0058A1C0, arg0);
    beMessenger_pushInt(D_0058A1C0, arg1);
    beMessenger_send(D_0058A1C0, D_0050DD80);
    beMessenger_getMatrix(mtx, D_0058A1C0, 0);
    func_00190A30(mtx, pos);
    func_00192F50(dir, D_004E1098, mtx);
    angle = func_00123640(dir[0], dir[2]);
    sprintf(D_0058A2B0, D_0050DD90, (double)pos[0], (double)pos[1], (double)pos[2],
                  (double)(57.29578f * angle + rot));
    return D_0058A2B0;
}

/* Position (x, y, z) in player space, as "V<x,y,z>"; NULL if no player. */
char* PLAYERgetPosStr(s32 player, f32 x, f32 y, f32 z) {
    f32 mtx[16];
    f32 v[3];
    f32 pos[3];

    beMessenger_call(D_0058A1C0, D_0050DDA8, NULL, player, D_0050DDB0);
    if (beMessenger_getMatrixCount(D_0058A1C0) != 0) {
        beMessenger_getMatrix(mtx, D_0058A1C0, 0);
        v[0] = x;
        v[1] = y;
        v[2] = z;
        func_00190A50(mtx, v);
        func_00190A30(mtx, pos);
        sprintf(D_0058A2F0, D_0050DB00, (double)pos[0], (double)pos[1], (double)pos[2]);
        return D_0058A2F0;
    }
    return NULL;
}

void MsgBox(char* arg0, s32 arg1) {
}

void ShutDown(void) {
    UnkStruct_0058A1B8* sys = D_0058A1B8;

    sys->unk10 = 1;
    sys->unk40 = 0;
    AssignStr(&sys->unk3C, NULL);
}

s32 TXTgetCallNo(void) {
    return D_0058A220;
}

bool BLTisPlayerBullet(void) {
    s32 kind = D_0058A1FC;

    if (kind < 10000) {
        if (kind < 1000) {
            return 1;
        }
    } else if (kind < 60000) {
        return 1;
    } else if (kind >= 80000 && kind < 90000) {
        return 1;
    }
    return 0;
}

s32 BLTgetShootRamNo(void) {
    return D_0058A208;
}

extern char D_0050DDD8[]; /* "P2EISHITCHECK" */

bool BLTisHitCheck(void) {
    void* work = NULL;

    if (D_0058A1F8 != NULL) {
        beMessenger_call(D_0058A1C0, D_0050DDC0, D_0058A1F8, D_0058A1E4, D_0050DDC8);
        if (beMessenger_getObjectCount(D_0058A1C0) != 0) {
            work = beMessenger_getObject(D_0058A1C0, 1);
        }
    }
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050DDD8);
    beMessenger_pushInt(D_0058A1C0, D_0058A208);
    beMessenger_pushInt(D_0058A1C0, D_0058A1F4);
    beMessenger_pushInt(D_0058A1C0, D_0058A1FC);
    beMessenger_pushObject(D_0058A1C0, work);
    beMessenger_send(D_0058A1C0, D_0050DDE8);
    return beMessenger_getBool(D_0058A1C0, 0);
}

/* GAMERAM::FIGHTCALC("P2EISCRITICAL", ...) */
bool BLTisCritical(void) {
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050DDF8);
    beMessenger_pushInt(D_0058A1C0, D_0058A208);
    beMessenger_pushInt(D_0058A1C0, D_0058A1F4);
    beMessenger_pushInt(D_0058A1C0, D_0058A1FC);
    beMessenger_send(D_0058A1C0, D_0050DDE8);
    return beMessenger_getBool(D_0058A1C0, 0);
}

extern char D_0050DE08[]; /* "P2EDAMAGE" */

/* Player -> enemy damage. Also stores the attribute (D_0058A338, see
   BLTgetAttributeStr) and a flag (D_0058A340, see BLTgetDamageRank). */
s32 BLTgetDamageValue(bool arg0) {
    void* work = NULL;

    if (D_0058A1F8 != NULL) {
        beMessenger_call(D_0058A1C0, D_0050DDC0, D_0058A1F8, D_0058A1E4, D_0050DDC8);
        if (beMessenger_getObjectCount(D_0058A1C0) != 0) {
            work = beMessenger_getObject(D_0058A1C0, 1);
        }
    }
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050DE08);
    beMessenger_pushInt(D_0058A1C0, D_0058A208);
    beMessenger_pushInt(D_0058A1C0, D_0058A1F4);
    beMessenger_pushInt(D_0058A1C0, D_0058A1FC);
    beMessenger_pushInt(D_0058A1C0, arg0);
    beMessenger_pushObject(D_0058A1C0, work);
    beMessenger_send(D_0058A1C0, D_0050DDE8);
    D_0058A330 = beMessenger_getInt(D_0058A1C0, 0);
    D_0058A338 = beMessenger_getInt(D_0058A1C0, 1);
    D_0058A340 = beMessenger_getInt(D_0058A1C0, 2);
    return D_0058A330;
}

s32 BLTgetDamageRank(s32 damage, bool flag) {
    s32 rank;
    f32 ratio;

    if (damage <= 0) {
        return 3;
    }
    rank = 0;
    ratio = (f32)D_0058A330 / (f32)D_0058A1EC;
    if (ratio > 0.5f) {
        rank = 2;
    } else if (ratio > 0.25f) {
        rank = 1;
    }
    if (rank == 0 && D_0058A340 == 1) {
        return rank + 1;
    }
    if (rank == 0 || rank == 1) {
        if (flag) {
            return rank + 1;
        }
        if (D_0058A1FC / 10000 == 2 || D_0058A1FC / 10000 == 3) {
            rank++;
        }
    }
    return rank;
}

char* BLTgetAttributeStr(void) {
    if (D_0058A338 >= 4) {
        return D_004C8800[0];
    }
    return D_004C8800[D_0058A338 + 1];
}

s32 BLTgetStunTime(s32 arg0, bool arg1) {
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050DE40);
    beMessenger_pushInt(D_0058A1C0, D_0058A208);
    beMessenger_pushInt(D_0058A1C0, D_0058A1F4);
    beMessenger_pushInt(D_0058A1C0, D_0058A1EC);
    beMessenger_pushInt(D_0058A1C0, arg0);
    beMessenger_pushInt(D_0058A1C0, arg1);
    beMessenger_send(D_0058A1C0, D_0050DDE8);
    return beMessenger_getInt(D_0058A1C0, 0);
}

/* Only for bullet kind 4xxxx with xxxx == 20. */
s32 BLTgetPoisonTime(char* arg0) {
    if (D_0058A1FC / 10000 != 4) {
        return 0;
    }
    if (D_0058A1FC % 10000 != 20) {
        return 0;
    }
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050DE50);
    beMessenger_pushString(D_0058A1C0, arg0);
    beMessenger_pushInt(D_0058A1C0, D_0058A208);
    beMessenger_pushInt(D_0058A1C0, D_0058A1F4);
    beMessenger_pushInt(D_0058A1C0, D_0058A1FC);
    beMessenger_send(D_0058A1C0, D_0050DDE8);
    return beMessenger_getInt(D_0058A1C0, 0);
}

/* Only for bullet kind 4xxxx with xxxx == 90. */
s32 BLTgetSleepTime(void) {
    if (D_0058A1FC / 10000 != 4) {
        return 0;
    }
    if (D_0058A1FC % 10000 != 90) {
        return 0;
    }
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050DE60);
    beMessenger_pushInt(D_0058A1C0, D_0058A208);
    beMessenger_pushInt(D_0058A1C0, D_0058A1F4);
    beMessenger_pushInt(D_0058A1C0, D_0058A1FC);
    beMessenger_send(D_0058A1C0, D_0050DDE8);
    return beMessenger_getInt(D_0058A1C0, 0);
}

/* Only for bullet kind 4xxxx with xxxx == 95. */
s32 BLTgetBindTime(void) {
    if (D_0058A1FC / 10000 != 4) {
        return 0;
    }
    if (D_0058A1FC % 10000 != 95) {
        return 0;
    }
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050DE70);
    beMessenger_pushInt(D_0058A1C0, D_0058A208);
    beMessenger_pushInt(D_0058A1C0, D_0058A1F4);
    beMessenger_pushInt(D_0058A1C0, D_0058A1FC);
    beMessenger_send(D_0058A1C0, D_0050DDE8);
    return beMessenger_getInt(D_0058A1C0, 0);
}

/* Only for bullet kind 4xxxx with xxxx == 70. */
s32 BLTgetAttackDownTime(char* arg0) {
    if (D_0058A1FC / 10000 != 4) {
        return 0;
    }
    if (D_0058A1FC % 10000 != 70) {
        return 0;
    }
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050DE80);
    beMessenger_pushString(D_0058A1C0, arg0);
    beMessenger_pushInt(D_0058A1C0, D_0058A208);
    beMessenger_pushInt(D_0058A1C0, D_0058A1F4);
    beMessenger_pushInt(D_0058A1C0, D_0058A1FC);
    beMessenger_send(D_0058A1C0, D_0050DDE8);
    return beMessenger_getInt(D_0058A1C0, 0);
}

/* Only for bullet kind 4xxxx with xxxx == 85. */
s32 BLTgetPowerDownTime(char* arg0) {
    if (D_0058A1FC / 10000 != 4) {
        return 0;
    }
    if (D_0058A1FC % 10000 != 85) {
        return 0;
    }
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050DE90);
    beMessenger_pushString(D_0058A1C0, arg0);
    beMessenger_pushInt(D_0058A1C0, D_0058A208);
    beMessenger_pushInt(D_0058A1C0, D_0058A1F4);
    beMessenger_pushInt(D_0058A1C0, D_0058A1FC);
    beMessenger_send(D_0058A1C0, D_0050DDE8);
    return beMessenger_getInt(D_0058A1C0, 0);
}

void BLTcalcSkillUp(bool arg0, bool arg1) {
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050DEA0);
    beMessenger_pushInt(D_0058A1C0, D_0058A208);
    beMessenger_pushInt(D_0058A1C0, D_0058A1FC);
    beMessenger_pushInt(D_0058A1C0, arg0);
    beMessenger_pushInt(D_0058A1C0, arg1);
    beMessenger_send(D_0058A1C0, D_0050DDE8);
}

s32 BLTcalcEXPUp(void) {
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050DEB0);
    beMessenger_pushInt(D_0058A1C0, D_0058A208);
    beMessenger_pushInt(D_0058A1C0, D_0058A1F4);
    beMessenger_send(D_0058A1C0, D_0050DDE8);
    return beMessenger_getInt(D_0058A1C0, 0);
}

bool BLPisHitCheck(void) {
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050DEC0);
    beMessenger_pushInt(D_0058A1C0, D_0058A1E4);
    beMessenger_pushInt(D_0058A1C0, D_0058A20C);
    beMessenger_pushInt(D_0058A1C0, D_0058A1FC);
    beMessenger_send(D_0058A1C0, D_0050DDE8);
    return beMessenger_getBool(D_0058A1C0, 0);
}

extern char D_0050DED0[]; /* "E2PDAMAGE" */

/* Enemy -> player damage, same results as BLTgetDamageValue. */
s32 BLPgetDamageValue(void) {
    void* work = NULL;

    if (D_0058A204 != NULL) {
        beMessenger_call(D_0058A1C0, D_0050DDC0, D_0058A204, D_0058A208, D_0050DDC8);
        if (beMessenger_getObjectCount(D_0058A1C0) != 0) {
            work = beMessenger_getObject(D_0058A1C0, 1);
        }
    }
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050DED0);
    beMessenger_pushInt(D_0058A1C0, D_0058A1E4);
    beMessenger_pushInt(D_0058A1C0, D_0058A20C);
    beMessenger_pushInt(D_0058A1C0, D_0058A1FC);
    beMessenger_pushObject(D_0058A1C0, work);
    beMessenger_send(D_0058A1C0, D_0050DDE8);
    D_0058A330 = beMessenger_getInt(D_0058A1C0, 0);
    D_0058A338 = beMessenger_getInt(D_0058A1C0, 1);
    D_0058A340 = beMessenger_getInt(D_0058A1C0, 2);
    return D_0058A330;
}


char* BLPgetAttackZokusei(void) {
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050DEE0);
    beMessenger_pushInt(D_0058A1C0, D_0058A208);
    beMessenger_pushInt(D_0058A1C0, D_0058A1FC);
    beMessenger_send(D_0058A1C0, D_0050DDE8);
    D_0058A338 = beMessenger_getInt(D_0058A1C0, 0);
    return BLTgetAttributeStr();
}

s32 BLPgetDamageRank(s32 arg0, s32 arg1) {
    s32 rank;

    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050DEF0);
    beMessenger_pushInt(D_0058A1C0, D_0058A1E4);
    beMessenger_pushInt(D_0058A1C0, D_0058A1FC);
    beMessenger_pushInt(D_0058A1C0, arg0);
    beMessenger_pushInt(D_0058A1C0, arg1);
    beMessenger_pushObject(D_0058A1C0, D_0058A1E0);
    beMessenger_send(D_0058A1C0, D_0050DDE8);
    rank = beMessenger_getInt(D_0058A1C0, 0);
    if (rank == 0 && D_0058A340 == 1) {
        rank++;
    }
    return rank;
}

s32 BLPgetStunTime(s32 arg0, s32 arg1) {
    void* work = NULL;

    if (D_0058A204 != NULL) {
        beMessenger_call(D_0058A1C0, D_0050DDC0, D_0058A204, D_0058A208, D_0050DDC8);
        if (beMessenger_getObjectCount(D_0058A1C0) != 0) {
            work = beMessenger_getObject(D_0058A1C0, 1);
        }
    }
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050DF00);
    beMessenger_pushInt(D_0058A1C0, D_0058A1E4);
    beMessenger_pushInt(D_0058A1C0, D_0058A20C);
    beMessenger_pushInt(D_0058A1C0, D_0058A1FC);
    beMessenger_pushInt(D_0058A1C0, arg0);
    beMessenger_pushInt(D_0058A1C0, arg1);
    beMessenger_pushObject(D_0058A1C0, work);
    beMessenger_send(D_0058A1C0, D_0050DDE8);
    return beMessenger_getInt(D_0058A1C0, 0);
}

s32 BLPgetPoisonTime(char* arg0) {
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050DF10);
    beMessenger_pushString(D_0058A1C0, arg0);
    beMessenger_pushInt(D_0058A1C0, D_0058A1E4);
    beMessenger_pushInt(D_0058A1C0, D_0058A1FC);
    beMessenger_send(D_0058A1C0, D_0050DDE8);
    return beMessenger_getInt(D_0058A1C0, 0);
}

s32 BLPgetSleepTime(void) {
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050DF20);
    beMessenger_pushInt(D_0058A1C0, D_0058A1E4);
    beMessenger_pushInt(D_0058A1C0, D_0058A20C);
    beMessenger_pushInt(D_0058A1C0, D_0058A1FC);
    beMessenger_send(D_0058A1C0, D_0050DDE8);
    return beMessenger_getInt(D_0058A1C0, 0);
}

s32 BLPgetBindTime(void) {
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050DF30);
    beMessenger_pushInt(D_0058A1C0, D_0058A1E4);
    beMessenger_pushInt(D_0058A1C0, D_0058A20C);
    beMessenger_pushInt(D_0058A1C0, D_0058A1FC);
    beMessenger_send(D_0058A1C0, D_0050DDE8);
    return beMessenger_getInt(D_0058A1C0, 0);
}

s32 BLPgetAttackDownTime(char* arg0) {
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050DF40);
    beMessenger_pushString(D_0058A1C0, arg0);
    beMessenger_pushInt(D_0058A1C0, D_0058A1E4);
    beMessenger_pushInt(D_0058A1C0, D_0058A20C);
    beMessenger_pushInt(D_0058A1C0, D_0058A1FC);
    beMessenger_send(D_0058A1C0, D_0050DDE8);
    return beMessenger_getInt(D_0058A1C0, 0);
}

s32 BLPgetPowerDownTime(char* arg0) {
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050DF50);
    beMessenger_pushString(D_0058A1C0, arg0);
    beMessenger_pushInt(D_0058A1C0, D_0058A1E4);
    beMessenger_pushInt(D_0058A1C0, D_0058A20C);
    beMessenger_pushInt(D_0058A1C0, D_0058A1FC);
    beMessenger_send(D_0058A1C0, D_0050DDE8);
    return beMessenger_getInt(D_0058A1C0, 0);
}

s32 BLPcalcKoukaMagic(void) {
    beMessenger_selectGroup(D_0058A1C0, D_0050D940);
    beMessenger_pushString(D_0058A1C0, D_0050DF60);
    beMessenger_pushInt(D_0058A1C0, D_0058A1E4);
    beMessenger_pushInt(D_0058A1C0, D_0058A208);
    beMessenger_pushInt(D_0058A1C0, D_0058A1FC);
    beMessenger_send(D_0058A1C0, D_0050DDE8);
    return beMessenger_getInt(D_0058A1C0, 0);
}

/* LOADINTF2::SETDIFFICULTMAX(max) */
void LISsetDifficultMax(s32 max) {
    beMessenger_select(D_0058A1C0, D_0050DF70, 0, 0);
    beMessenger_pushInt(D_0058A1C0, max);
    beMessenger_send(D_0058A1C0, D_0050DF80);
}

} /* extern "C" */
