#include "game.h"

extern "C" {
void func_002255B0(s32 n);
void func_003A9990(beSaveMemoryObj* o);
void func_003A98D0(void* o, s32 size, s32 flag);
igObject* func_0034AC50(void* pool);
extern bool D_00554C14;
extern void* D_00587110;
void func_002FDD20(igMetaObject* meta, s32);
void igObject_userRelease(igObject* o);
extern bool D_00554E60;
extern void* D_0058A600;
extern igMetaObject* beSaveMemoryObj_Meta;
extern igMetaObject* beSvConnectCheck_Meta;
s32 func_0011A500(s32, s32);
extern s32 D_004C91B0[];
extern s32 D_004C91B4[];
extern s32 D_0058A870;
extern s32 D_0058A8D0;
extern bool D_00554E64;
}

/* beSaveApi's last word (0x3C), not a registered field. */
#define API_UNK3C(api) (*(s32*)(api)->unk3C)
#define API_UNK3C_V(api) (*(volatile s32*)(api)->unk3C)

/* igObject's virtuals. */
class igObjectV {
public:
    IG_VPAD_20
    virtual igMetaObject* getMeta(); /* +0x58 */
};

#define META(o) (((igObjectV*)(o))->getMeta())

void beSaveMemoryObj_userRelease(beSaveMemoryObj* o) {
    func_003A9990(o);
    func_002FDD20(META(o), 1);
    igObject_userRelease(o);
}

igMetaObject* beSaveMemoryObj_getMeta(void) {
    return beSaveMemoryObj_Meta;
}

INCLUDE_ASM("asm/nonmatchings/beSaveMemoryObj", func_003A9A50);

INCLUDE_ASM("asm/nonmatchings/beSaveMemoryObj", func_003A9B10);

INCLUDE_ASM("asm/nonmatchings/beSaveMemoryObj", func_003A9C40);

void func_003AB550(beSaveApi* api, beSvDeliver* dlv) {
    dlv->_result = api->_result;
    dlv->_isEndOperat = true;
}

void func_003AB570(beSaveApi* api, beSvDeliver* dlv) {
    if (dlv->_targetConnect == -1) {
        api->_connect = 0;
        api->_isConAllChk = true;
    } else {
        api->_connect = dlv->_targetConnect;
        api->_isConAllChk = false;
    }
    api->_isEnd = false;
    api->_result = 1;
    D_0058A600 = dlv->_platformData;
    api->_st0 = 0;
    api->_st3 = 0;
    api->_st2 = 0;
    api->_st1 = 0;
    D_00554E60 = true;
    API_UNK3C(api) = 0;
}

INCLUDE_ASM("asm/nonmatchings/beSaveMemoryObj", func_003AB5D0);

void func_003ABB30(beSaveApi* api, beSvDeliver* dlv) {
    dlv->_result = api->_result;
    dlv->_isEndOperat = true;
}

void func_003ABB50(beSaveApi* api, beSvDeliver* dlv) {
    api->_isEnd = false;
    api->_connect = dlv->_targetConnect;
    api->_result = 1;
    api->_st0 = 0;
    api->_st3 = 0;
    api->_st2 = 0;
    api->_st1 = 0;
    D_00554E60 = true;
    API_UNK3C(api) = 0;
}

#if 1
/* Nonmatching: not yet built. */
bool func_003ABB80(beSaveApi* api) {
    s32 idx;
    s32 d;

    if (api->_isEnd) {
        return true;
    }
    if (D_00554E64 == true) {
        return false;
    }
    switch (api->_st0) {
    case 0:
        API_UNK3C(api) = 0;
        api->_st0++;
        api->_st3 = 0;
        api->_st2 = 0;
        api->_st1 = 0;
        break;
    case 1:
        idx = api->_connect * 2;
        if (func_0011A500(D_004C91B0[idx], D_004C91B4[idx]) >= 0) {
            D_0058A870 = 1;
            api->_st0++;
            api->_st3 = 0;
            api->_st2 = 0;
            api->_st1 = 0;
        }
        break;
    case 2:
        if (D_0058A870 == 1) {
            return false;
        }
        d = D_0058A8D0;
        if (d >= 0) {
            api->_isEnd = true;
            api->_result = 2;
        } else {
            API_UNK3C_V(api)++;
            if ((API_UNK3C_V(api) < 3) == true) {
                api->_st0 = 1;
                api->_st3 = 0;
                api->_st2 = 0;
                api->_st1 = 0;
            } else {
                switch (d) {
                case -5:
                    api->_result = 12;
                    api->_isEnd = true;
                    break;
                default:
                    api->_result = 12;
                    api->_isEnd = true;
                    break;
                }
            }
        }
        break;
    }
    if (api->_isEnd) {
        D_00554E60 = false;
    }
    return api->_isEnd;
}
#else
INCLUDE_ASM("asm/nonmatchings/beSaveMemoryObj", func_003ABB80);
#endif

void func_003ABD30(beSaveApi* api, beSvDeliver* dlv) {
    dlv->_result = api->_result;
    dlv->_isEndOperat = true;
}

void func_003ABD50(beSaveApi* api, beSvDeliver* dlv) {
    api->_connect = dlv->_targetConnect;
    api->_isEnd = false;
    api->_result = 1;
    api->_st0 = 0;
    api->_st3 = 0;
    api->_st2 = 0;
    api->_st1 = 0;
    D_00554E60 = true;
    API_UNK3C(api) = 0;
}

INCLUDE_ASM("asm/nonmatchings/beSaveMemoryObj", func_003ABD80);

void func_003ABFA0(beSaveApi* api, beSvDeliver* dlv) {
    dlv->_result = api->_result;
    dlv->_isEndOperat = true;
}

IG_REF(igObject);

void func_003ABFC0(beSvReadMediaApi* api, beSvDeliver* dlv) {
    beSvPlatBaseData* platform;
    beSaveDataDeliver* gameData;
    s32 i;

    platform = dlv->_platformData;
    api->_connect = dlv->_targetConnect;
    gameData = dlv->_gameData;
    igIncRef(gameData);
    igUnref(api->_lodeData);
    api->_lodeData = gameData;
    api->_gameData = dlv->_gameData;
    api->_isEnd = false;
    api->_result = 1;
    D_0058A600 = dlv->_platformData;
    i = 0;
    do {
        {
            void* a = func_00301000(api);
            void* pool = D_00554C14 ? a : func_002F88E0(D_00587110);
            igObjectRef sp(func_0034AC50(pool), IG_ADOPT);
            func_003A98D0(sp, platform->_dataSize, 0);
            func_00301E30(api->_loadWorkList, sp);
        }
        i++;
    } while (i < 2);
    api->_st0 = 0;
    api->_st3 = 0;
    api->_st2 = 0;
    api->_st1 = 0;
    D_00554E60 = true;
    API_UNK3C(api) = 0;
}

INCLUDE_ASM("asm/nonmatchings/beSaveMemoryObj", func_003AC130);

void func_003AC910(beSaveApi* api, beSvDeliver* dlv) {
    dlv->_result = api->_result;
    dlv->_isEndOperat = true;
}

INCLUDE_ASM("asm/nonmatchings/beSaveMemoryObj", func_003AC930);

INCLUDE_ASM("asm/nonmatchings/beSaveMemoryObj", func_003ACA70);

void func_003AD220(beSaveApi* api, beSvDeliver* dlv) {
    dlv->_result = api->_result;
    dlv->_isEndOperat = true;
}

void func_003AD240(beSaveApi* api, beSvDeliver* dlv) {
    api->_connect = dlv->_targetConnect;
    api->_isEnd = false;
    api->_result = 1;
    D_0058A600 = dlv->_platformData;
    api->_st0 = 0;
    api->_st3 = 0;
    api->_st2 = 0;
    api->_st1 = 0;
    D_00554E60 = true;
    API_UNK3C(api) = 0;
}

INCLUDE_ASM("asm/nonmatchings/beSaveMemoryObj", func_003AD280);

INCLUDE_ASM("asm/nonmatchings/beSaveMemoryObj", func_003AD530);

INCLUDE_ASM("asm/nonmatchings/beSaveMemoryObj", func_003AD5B0);

bool func_003AD6B0(beSaveApi* api) {
    if (api->_isEnd) {
        return true;
    }
    api->_isEnd = true;
    return true;
}

void func_003AD6E0(beSaveApi* api, beSvDeliver* dlv) {
    dlv->_result = api->_result;
    dlv->_isEndOperat = true;
}

void func_003AD700(beSaveApi* api) {
    api->_isEnd = false;
    api->_result = 1;
    func_002255B0(2);
    api->_st0 = 0;
    api->_st3 = 0;
    api->_st2 = 0;
    api->_st1 = 0;
}

bool func_003AD750(beSaveApi* api) {
    if (api->_isEnd) {
        return true;
    }
    api->_isEnd = true;
    api->_result = 2;
    return true;
}

void func_003AD780(beSaveApi* api, beSvDeliver* dlv) {
    dlv->_result = api->_result;
    dlv->_isEndOperat = true;
}

INCLUDE_ASM("asm/nonmatchings/beSaveMemoryObj", func_003AD7A0);

INCLUDE_ASM("asm/nonmatchings/beSaveMemoryObj", func_003AD850);

INCLUDE_ASM("asm/nonmatchings/beSaveMemoryObj", func_003AD950);

INCLUDE_ASM("asm/nonmatchings/beSaveMemoryObj", func_003ADA60);

INCLUDE_ASM("asm/nonmatchings/beSaveMemoryObj", func_003ADD40);

void func_003ADF20(void) {
}

s32 func_003ADF30(void) {
    return 0;
}

void func_003ADF40(void) {
}

igMetaObject* beSvConnectCheck_getMeta(void) {
    return beSvConnectCheck_Meta;
}
