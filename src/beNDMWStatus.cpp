#include "common.h"
#include "meta.h"

extern igMetaObject* beNDMWStatusInfo_Meta;
extern igMetaObject* beNDMWStatus_Meta;

/* A vector passed by value (the caller copies it, member by member, and passes
   the copy's address). */
struct beNDMWStatusVec3 : igVec3f {
    beNDMWStatusVec3(f32 a, f32 b, f32 c) {
        x = a;
        y = b;
        z = c;
    }
    beNDMWStatusVec3(const beNDMWStatusVec3& o) {
        x = o.x;
        y = o.y;
        z = o.z;
    }
};

extern "C++" void beMessenger_pushVec3f(void* m, beNDMWStatusVec3 v);

struct beNDMWStatusSel {
    char pad[0x4F];
    s8 unk4F;
    s8 unk50;
};

struct beNDMWStatusCntInner {
    char pad[8];
    s32 unk8;
};

struct beNDMWStatusCnt {
    char pad[0x14];
    beNDMWStatusCntInner* unk14;
};

struct beNDMWStatusCnt2 {
    char pad[0x18];
    beNDMWStatusCntInner* unk18;
};

struct beNDMWStatusState {
    char pad[0x20];
    s8 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 pad30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
};

struct beNDMWStatusRef {
    s32 pad0;
    s32 _refCount;
};

struct beNDMWStatusP {
    char pad[0x20];
    beNDMWStatusRef* unk20;
};

extern "C" void func_002FF820(void*);

void beNDMWStatus_prepare(beNDMWStatusP* self) {
    beNDMWStatusRef* r = self->unk20;
    if (r != NULL) {
        r->_refCount--;
        if ((r->_refCount & 0x7FFFFF) == 0) {
            func_002FF820(r);
        }
    }
    self->unk20 = NULL;
}

igMetaObject* beNDMWStatus_getInfoMeta(void) {
    return beNDMWStatusInfo_Meta;
}

struct beNDMWStatusE {
    char pad[0x14];
    void* unk14;
    beNDMWStatusCntInner* unk18;
};

struct beNDMWStatusE2 {
    char pad[0x14];
    s8 unk14;
};

extern char D_00516788[];
void beMessenger_entryRam(void* m, void* ram, const char* group);

void beNDMWStatus_entry(beNDMWStatusE* self, beNDMWStatusE2* ram) {
    if (self->unk18->unk8 == 1) {
        beMessenger_entryRam(self->unk14, ram, D_00516788);
        ram->unk14 = 0;
    }
}



void* func_003D9BD0(beNDMWStatusRef** self, s16 flag) {
    if (self != NULL) {
        beNDMWStatusRef* r = *self;
        if (r != NULL) {
            r->_refCount--;
            if ((r->_refCount & 0x7FFFFF) == 0) {
                func_002FF820(r);
            }
        }
        if (flag > 0) {
            delete (char*)self;
        }
    }
    return self;
}

void* func_003D9C50(beNDMWStatusRef** self, s16 flag) {
    if (self != NULL) {
        beNDMWStatusRef* r = *self;
        if (r != NULL) {
            r->_refCount--;
            if ((r->_refCount & 0x7FFFFF) == 0) {
                func_002FF820(r);
            }
        }
        if (flag > 0) {
            delete (char*)self;
        }
    }
    return self;
}

void* func_003D9CD0(beNDMWStatusRef** self, s16 flag) {
    if (self != NULL) {
        beNDMWStatusRef* r = *self;
        if (r != NULL) {
            r->_refCount--;
            if ((r->_refCount & 0x7FFFFF) == 0) {
                func_002FF820(r);
            }
        }
        if (flag > 0) {
            delete (char*)self;
        }
    }
    return self;
}

void* func_003D9D50(beNDMWStatusRef** self, s16 flag) {
    if (self != NULL) {
        beNDMWStatusRef* r = *self;
        if (r != NULL) {
            r->_refCount--;
            if ((r->_refCount & 0x7FFFFF) == 0) {
                func_002FF820(r);
            }
        }
        if (flag > 0) {
            delete (char*)self;
        }
    }
    return self;
}

igMetaObject* beNDMWStatus_getMeta(void) {
    return beNDMWStatus_Meta;
}

struct beNDMWStatusA {
    char pad8[8];
    s32 unk8, unkC, unk10, unk14, unk18, unk1C;
    s8 unk20;
    char pad21[0x13];
    s32 unk34, unk38, unk3C, unk40;
};

void func_003D9DE0(beNDMWStatusA* self, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6) {
    self->unkC = a4;
    self->unk14 = a2;
    self->unk10 = a3;
    self->unk18 = a1;
    self->unk8 = a5;
    self->unk1C = a6;
    self->unk20 = 0;
    self->unk34 = 0;
    self->unk40 = 0;
    self->unk3C = 0;
    self->unk38 = 0;
}

void func_003D9E10(beNDMWStatusState* self, s32 a, s32 b) {
    self->unk28 = a;
    self->unk2C = b;
    self->unk20 = 0;
    self->unk34 = 1;
    self->unk40 = 0;
    self->unk3C = 0;
    self->unk38 = 0;
}

void func_003D9E40(beNDMWStatusState* self, s32 a, s32 b) {
    self->unk28 = a;
    self->unk2C = b;
    self->unk20 = 0;
    self->unk34 = 3;
    self->unk40 = 0;
    self->unk3C = 0;
    self->unk38 = 0;
}

void func_003D9E70(beNDMWStatusState* self, s32 a) {
    self->unk20 = 0;
    self->unk24 = a;
    self->unk34 = 2;
    self->unk40 = 0;
    self->unk3C = 0;
    self->unk38 = 0;
}

void func_003D9E90(beNDMWStatusState* self) {
    self->unk20 = 0;
    self->unk24 = 1;
    self->unk34 = 2;
    self->unk40 = 0;
    self->unk3C = 0;
    self->unk38 = 0;
}


struct beNDMWStatusG {
    char pad[0x10];
    s32 unk10;
    s32 pad14;
    s32 unk18;
    s32 unk1C;
    char pad20[0x10];
    u8 unk30;
};

extern "C" void func_003EBDC0(s32, const char*, s32, s32);
extern "C" void func_003EC050(s32, s32);

void func_003DA5F0(beNDMWStatusG* self) {
    if (self->unk1C == 0) {
        func_003EBDC0(self->unk10, "CHRSELFONT03", self->unk18, 0);
        func_003EBDC0(self->unk10, "CHRSELFONT04", self->unk18, 0);
        func_003EBDC0(self->unk10, "CHRSELFONT04", self->unk18, 0xA);
        func_003EBDC0(self->unk10, "CHRSELFONT05", self->unk18, 0);
        func_003EBDC0(self->unk10, "CHRSELFONT05", self->unk18, 0xA);
        func_003EBDC0(self->unk10, "CHRSELFONT05", self->unk18, 1);
        func_003EBDC0(self->unk10, "CHRSELFONT05", self->unk18, 0xB);
        func_003EC050(self->unk10, (self->unk18 << 16) | 0x10);
        return;
    }
    if (self->unk30 == 1) {
        func_003EBDC0(self->unk10, "CHRSELFONT03", self->unk18, 0xA);
        func_003EBDC0(self->unk10, "CHRSELFONT04", self->unk18, 0xB);
        func_003EBDC0(self->unk10, "CHRSELFONT04", self->unk18, 1);
        func_003EBDC0(self->unk10, "CHRSELFONT05", self->unk18, 2);
        func_003EBDC0(self->unk10, "CHRSELFONT05", self->unk18, 0xC);
        func_003EBDC0(self->unk10, "CHRSELFONT05", self->unk18, 3);
        func_003EBDC0(self->unk10, "CHRSELFONT05", self->unk18, 0xD);
        return;
    }
    func_003EC050(self->unk10, (self->unk18 << 16) | 0x110);
}


struct beNDMWStatusB {
    char pad[0x14];
    s32 unk14;
    char pad18[0x18];
    s8 unk30, unk31, unk32, unk33, unk34;
    char pad35[3];
    s32 unk38, unk3C;
    char pad40[8];
    s8 unk48;

    s32 unk4C, unk50, unk54, unk58;
};

void func_003DAFB0(beNDMWStatusB* self, s32 a, s32 b) {
    self->unk3C = a;
    self->unk48 = 0;
    self->unk38 = b;
    self->unk14 = 0;
    self->unk30 = 0;
    self->unk31 = 0;
    self->unk32 = 0;
    self->unk34 = 0;
    self->unk4C = 0;
    self->unk58 = 0;
    self->unk54 = 0;
    self->unk50 = 0;
}

void func_003DAFF0(beNDMWStatusB* self, s32 n) {
    switch (n) {
    default:
        self->unk4C = 0;
        self->unk58 = 0;
        self->unk54 = 0;
        self->unk50 = 0;
        break;
    case 1:
        self->unk50 = 0;
        self->unk58 = 0;
        self->unk54 = 0;
        break;
    case 2:
        self->unk54 = 0;
        self->unk58 = 0;
        break;
    case 3:
        self->unk58 = 0;
        break;
    }
}



struct beNDMWStatusB2Pair {
    beNDMWStatusState* unk0;
    beNDMWStatusState* unk4;
};

struct beNDMWStatusB2Mid {
    char pad[0x10];
    beNDMWStatusB2Pair* unk10;
};

struct beNDMWStatusB2In {
    char pad[0x10];
    beNDMWStatusB2Mid* unk10;
};

struct beNDMWStatusB2 {
    char pad[0x2C];
    beNDMWStatusB2In* unk2C;
    char pad30;
    s8 unk31, unk32;
    char pad33[0x19];
    s32 unk4C, unk50, unk54, unk58;
};

u8 func_003DA7C0(beNDMWStatusState*);

bool func_003DCAB0(beNDMWStatusB2* self, s32 n) {
    beNDMWStatusB2In* in = self->unk2C;
    bool done = false;
    s32 state;
    switch (n) {
    default:
        state = self->unk4C;
        break;
    case 1:
        state = self->unk50;
        break;
    case 2:
        state = self->unk54;
        break;
    case 3:
        state = self->unk58;
        break;
    }
    switch (state) {
    case 0:
        func_003D9E70(in->unk10->unk10->unk0, 0);
        func_003D9E70(in->unk10->unk10->unk4, 0);
        self->unk32 = 0;
        state++;
        self->unk31 = 0;
        break;
    case 1:
        if (func_003DA7C0(in->unk10->unk10->unk0) == 1 && func_003DA7C0(in->unk10->unk10->unk4) == 1) {
            state++;
        }
        break;
    case 2:
        done = true;
        break;
    }
    switch (n) {
    default:
        self->unk4C = state;
        self->unk58 = 0;
        self->unk54 = 0;
        self->unk50 = 0;
        break;
    case 1:
        self->unk50 = state;
        self->unk58 = 0;
        self->unk54 = 0;
        break;
    case 2:
        self->unk54 = state;
        self->unk58 = 0;
        break;
    case 3:
        self->unk58 = state;
        break;
    }
    return done;
}

struct beNDMWStatusMdl2;
void func_003E42A0(beNDMWStatusMdl2* m, s32 ramNo, const char* info, const char* msg, s32 flag);
bool func_003E44D0(beNDMWStatusCnt2* self); /* no models left */

extern char D_00516D30[]; /* "SQ_CLOSE" */
extern char D_00516D70[]; /* "j_saveareawin01" */
extern char D_00516D80[]; /* "j_saveareawin02" */
extern char D_00516D90[]; /* "j_saveareawin03" */
extern char D_00516DA0[]; /* "j_saveareawin04" */
extern char D_00516DB0[]; /* "j_saveareacursorr" */
extern char D_00516DC8[]; /* "CHRSELFONT08" */

/* The save area windows: a step per nesting level (n), like func_003DCAB0. */
struct beNDMWStatusSaveArea {
    char pad[0x14];
    s32 unk14;               /* area (A..D) */
    struct bePadManager* unk18;
    beNDMWStatusMdl2* unk1C; /* models */
    s32 unk20;               /* font owner */
    struct beNDMWStatusSlots* unk24; /* save slots */
    s32 unk28;               /* message window */
    beNDMWStatusB2In* unk2C; /* list window */
    u8 unk30;                /* windows open */
    u8 unk31;                /* first list open */
    u8 unk32;                /* second list open */
    char pad33;
    u8 unk34;                /* cursor open */
    char pad35[3];
    s32 unk38;
    s32 unk3C;               /* ram number */
    s32 unk40;               /* already in use */
    char pad44[5];
    u8 unk49;                /* cancelled */
    char pad4A[2];
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
};

struct beNDMWStatusMdlPos {
    char pad[0x1C];
    beNDMWStatusVec3 pos; /* 0x1C */
};

struct beNDMWStatusMdl;
void func_003E3E00(beNDMWStatusMdl* m, s32 ramNo, const char* info, const char* msg, beNDMWStatusVec3 pos, s32 flag);
bool func_003E4280(beNDMWStatusCnt* self); /* model ready */
extern "C" void func_003ECAD0(s32 o, s32 v);

/* Open the cursor of the chosen save area (A..D), wait for it, then show the
   message (0x513 / 0x503 for ram unk3C). */
bool func_003DD160(beNDMWStatusSaveArea* self, s32 n) {
    beNDMWStatusMdl2* mdl = self->unk1C;
    s32 msgWin = self->unk28;
    bool done = false;
    s32 state;

    switch (n) {
    default:
        state = self->unk4C;
        break;
    case 1:
        state = self->unk50;
        break;
    case 2:
        state = self->unk54;
        break;
    case 3:
        state = self->unk58;
        break;
    }
    switch (state) {
    case 0:
        if (self->unk34 == 0) {
            switch (self->unk14) {
            case 0:
                func_003E3E00((beNDMWStatusMdl*)mdl, self->unk3C, D_00516DB0, "SQ_AAREAOPEN", ((beNDMWStatusMdlPos*)mdl)->pos, 1);
                break;
            case 1:
                func_003E3E00((beNDMWStatusMdl*)mdl, self->unk3C, D_00516DB0, "SQ_BAREAOPEN", ((beNDMWStatusMdlPos*)mdl)->pos, 1);
                break;
            case 2:
                func_003E3E00((beNDMWStatusMdl*)mdl, self->unk3C, D_00516DB0, "SQ_CAREAOPEN", ((beNDMWStatusMdlPos*)mdl)->pos, 1);
                break;
            case 3:
                func_003E3E00((beNDMWStatusMdl*)mdl, self->unk3C, D_00516DB0, "SQ_DAREAOPEN", ((beNDMWStatusMdlPos*)mdl)->pos, 1);
                break;
            }
            self->unk34 = 1;
        }
        state++;
        break;
    case 1:
        if (func_003E4280((beNDMWStatusCnt*)mdl) == true) {
            state++;
            if (self->unk38 == 0) {
                func_003ECAD0(msgWin, (self->unk3C << 16) | 0x513);
            } else {
                func_003ECAD0(msgWin, (self->unk3C << 16) | 0x503);
            }
        }
        break;
    case 2:
        done = true;
        break;
    }
    switch (n) {
    default:
        self->unk4C = state;
        self->unk58 = 0;
        self->unk54 = 0;
        self->unk50 = 0;
        break;
    case 1:
        self->unk50 = state;
        self->unk58 = 0;
        self->unk54 = 0;
        break;
    case 2:
        self->unk54 = state;
        self->unk58 = 0;
        break;
    case 3:
        self->unk58 = state;
        break;
    }
    return done;
}

/* Close everything open in the save area (windows, both lists, cursor), then
   wait for each to finish. */
bool func_003DBF90(beNDMWStatusSaveArea* self, s32 n) {
    beNDMWStatusMdl2* mdl = self->unk1C;
    s32 font = self->unk20;
    beNDMWStatusB2In* list = self->unk2C;
    bool done = false;
    s32 state;

    switch (n) {
    default:
        state = self->unk4C;
        break;
    case 1:
        state = self->unk50;
        break;
    case 2:
        state = self->unk54;
        break;
    case 3:
        state = self->unk58;
        break;
    }
    switch (state) {
    case 0:
        if (self->unk30 == 1) {
            func_003E42A0(mdl, self->unk3C, D_00516D70, D_00516D30, 1);
            func_003E42A0(mdl, self->unk3C, D_00516D80, D_00516D30, 1);
            func_003E42A0(mdl, self->unk3C, D_00516D90, D_00516D30, 1);
            func_003E42A0(mdl, self->unk3C, D_00516DA0, D_00516D30, 1);
            func_003EBDC0(font, D_00516DC8, self->unk3C, 0);
            func_003EBDC0(font, D_00516DC8, self->unk3C, 1);
            func_003EBDC0(font, D_00516DC8, self->unk3C, 2);
            func_003EBDC0(font, D_00516DC8, self->unk3C, 3);
            self->unk30 = 0;
        }
        if (self->unk31 == 1) {
            func_003D9E70(list->unk10->unk10->unk0, 0);
        }
        if (self->unk32 == 1) {
            func_003D9E70(list->unk10->unk10->unk4, 0);
        }
        if (self->unk34 == 1) {
            func_003E42A0(mdl, self->unk3C, D_00516DB0, D_00516D30, 1);
        }
        state++;
        break;
    case 1:
        if (self->unk31 == 1) {
            if (func_003DA7C0(list->unk10->unk10->unk0) == 1) {
                self->unk31 = 0;
            } else {
                break;
            }
        }
        if (self->unk32 == 1) {
            if (func_003DA7C0(list->unk10->unk10->unk4) == 1) {
                self->unk32 = 0;
            } else {
                break;
            }
        }
        if (func_003E44D0((beNDMWStatusCnt2*)mdl) == true) {
            done = true;
            state++;
        }
        break;
    case 2:
        done = true;
        break;
    }
    switch (n) {
    default:
        self->unk4C = state;
        self->unk58 = 0;
        self->unk54 = 0;
        self->unk50 = 0;
        break;
    case 1:
        self->unk50 = state;
        self->unk58 = 0;
        self->unk54 = 0;
        break;
    case 2:
        self->unk54 = state;
        self->unk58 = 0;
        break;
    case 3:
        self->unk58 = state;
        break;
    }
    return done;
}

/* Close the save area windows, wait for the models, then open the list window
   (func_003D9E10 at line unk14) and wait for it. */
bool func_003DC2E0(beNDMWStatusSaveArea* self, s32 n) {
    beNDMWStatusMdl2* mdl = self->unk1C;
    s32 font = self->unk20;
    beNDMWStatusB2In* list = self->unk2C;
    bool done = false;
    s32 state;

    switch (n) {
    default:
        state = self->unk4C;
        break;
    case 1:
        state = self->unk50;
        break;
    case 2:
        state = self->unk54;
        break;
    case 3:
        state = self->unk58;
        break;
    }
    switch (state) {
    case 0:
        if (self->unk30 == 1) {
            func_003E42A0(mdl, self->unk3C, D_00516D70, D_00516D30, 1);
            func_003E42A0(mdl, self->unk3C, D_00516D80, D_00516D30, 1);
            func_003E42A0(mdl, self->unk3C, D_00516D90, D_00516D30, 1);
            func_003E42A0(mdl, self->unk3C, D_00516DA0, D_00516D30, 1);
            func_003EBDC0(font, D_00516DC8, self->unk3C, 0);
            func_003EBDC0(font, D_00516DC8, self->unk3C, 1);
            func_003EBDC0(font, D_00516DC8, self->unk3C, 2);
            func_003EBDC0(font, D_00516DC8, self->unk3C, 3);
            self->unk30 = 0;
        }
        state++;
        break;
    case 1:
        if (func_003E44D0((beNDMWStatusCnt2*)mdl) == true) {
            state++;
            func_003D9E10(list->unk10->unk10->unk4, 0, self->unk14);
            self->unk32 = 1;
        }
        break;
    case 2:
        if (func_003DA7C0(list->unk10->unk10->unk4) == 1) {
            done = true;
            state++;
        }
        break;
    case 3:
        break;
    }
    switch (n) {
    default:
        self->unk4C = state;
        self->unk58 = 0;
        self->unk54 = 0;
        self->unk50 = 0;
        break;
    case 1:
        self->unk50 = state;
        self->unk58 = 0;
        self->unk54 = 0;
        break;
    case 2:
        self->unk54 = state;
        self->unk58 = 0;
        break;
    case 3:
        self->unk58 = state;
        break;
    }
    return done;
}

/* Close the save area windows and cursor, then wait for the models to go. */
bool func_003DD440(beNDMWStatusSaveArea* self, s32 n) {
    beNDMWStatusMdl2* mdl = self->unk1C;
    s32 font = self->unk20;
    bool done = false;
    s32 state;

    switch (n) {
    default:
        state = self->unk4C;
        break;
    case 1:
        state = self->unk50;
        break;
    case 2:
        state = self->unk54;
        break;
    case 3:
        state = self->unk58;
        break;
    }
    switch (state) {
    case 0:
        if (self->unk30 == 1) {
            func_003E42A0(mdl, self->unk3C, D_00516D70, D_00516D30, 1);
            func_003E42A0(mdl, self->unk3C, D_00516D80, D_00516D30, 1);
            func_003E42A0(mdl, self->unk3C, D_00516D90, D_00516D30, 1);
            func_003E42A0(mdl, self->unk3C, D_00516DA0, D_00516D30, 1);
            func_003EBDC0(font, D_00516DC8, self->unk3C, 0);
            func_003EBDC0(font, D_00516DC8, self->unk3C, 1);
            func_003EBDC0(font, D_00516DC8, self->unk3C, 2);
            func_003EBDC0(font, D_00516DC8, self->unk3C, 3);
            self->unk30 = 0;
        }
        if (self->unk34 == 1) {
            func_003E42A0(mdl, self->unk3C, D_00516DB0, D_00516D30, 1);
        }
        state++;
        break;
    case 1:
        if (func_003E44D0((beNDMWStatusCnt2*)mdl) == true) {
            done = true;
            state++;
        }
        break;
    case 2:
        done = true;
        break;
    }
    switch (n) {
    default:
        self->unk4C = state;
        self->unk58 = 0;
        self->unk54 = 0;
        self->unk50 = 0;
        break;
    case 1:
        self->unk50 = state;
        self->unk58 = 0;
        self->unk54 = 0;
        break;
    case 2:
        self->unk54 = state;
        self->unk58 = 0;
        break;
    case 3:
        self->unk58 = state;
        break;
    }
    return done;
}

/* A line choice window (two or three lines, picked with the pad). */
struct beNDMWStatusF {
    char pad[0x10];
    s32 unk10;     /* cursor */
    s32 unk14;     /* lines */
    s32 unk18;     /* ram number */
    s32 unk1C;     /* chosen line */
    s32 unk20;     /* layout: 0 two lines, 1 three */
    u8 unk24;      /* done */
    u8 unk25;      /* cancelled */
    char pad26[2];
    struct bePadManager* unk28;
    struct beNDMWStatusMdlPos* unk2C; /* models */
    s32 unk30;     /* window texts */
    s32 pad34;
    s32 unk38;     /* message window */
    s32 unk3C, unk40, unk44, unk48;
};

void func_003DDB10(beNDMWStatusF* self, s32 a, s32 b) {
    self->unk10 = 0;
    self->unk1C = 0;
    self->unk18 = b;
    self->unk1C = 0;
    self->unk20 = a;
    self->unk24 = 0;
    self->unk25 = 0;
    self->unk3C = 0;
    self->unk48 = 0;
    self->unk44 = 0;
    self->unk40 = 0;
    switch (self->unk20) {
    case 0:
        self->unk14 = 2;
        break;
    case 1:
        self->unk14 = 3;
        break;
    }
}


/* A menu cursor (also used by the load and save screens): mode, the cursor
   position per entry and per-entry flags. */
struct beNDMWStatusCur {
    char pad[0x14];
    struct bePadManager* unk14;
    struct beNDMWStatusMdlPos* unk18; /* models */
    s32 unk1C;     /* font owner */
    s32 unk20;     /* ram number */
    s32 unk24;     /* face */
    s32 mode;      /* 0x28 */
    s32 unk2C;
    s32 unk30;
    s32 unk34;     /* current entry's position */
    s32 unk38;
    s32 unk3C;
    s32 pad40;
    s32 unk44;
    s32 cur;       /* 0x48 */
    u8 unk4C;      /* done */
    u8 unk4D;      /* confirmed */
    u8 unk4E;      /* asking */
    u8 unk4F;      /* answered */
    u8 unk50;      /* answer */
    u8 unk51;      /* side arrows shown */
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 pad64;
    s32 pos[8][3]; /* 0x68 */
    s32 flag[9];   /* 0xC8 */
    u16 val[9];    /* 0xEC: the typed characters */
    s8 unkFE;
    s8 unkFF;
    s8 unk100;
    s8 unk101;
    s8 unk102;
    s8 unk103;
};

void func_003DE260(beNDMWStatusCur* self, u8 reset) {
    s32 i;
    s32 j;

    self->mode = 1;
    switch (self->mode) {
    case 0:
        self->unk44 = 6;
        self->unk2C = 24;
        self->unk30 = 0;
        break;
    case 1:
        self->unk44 = 8;
        self->unk2C = 4;
        self->unk30 = 0;
        break;
    default:
        self->unk44 = 8;
        self->unk2C = 30;
        self->unk30 = 24;
        break;
    }
    if (reset == 1) {
        self->unkFE = 0;
        self->unkFF = 0;
        self->unk100 = 0;
        self->unk101 = 0;
        self->unk102 = 0;
        self->unk103 = 0;
        self->cur = -1;
        for (i = 0; i < 9; i++) {
            self->flag[i] = 0;
        }
        for (j = 0; j < 9; j++) {
            self->val[j] = 0;
        }
    }
    switch (self->mode) {
    case 0:
        if (self->cur == -1) {
            self->pos[0][0] = 4;
            self->pos[0][1] = 0;
            self->pos[0][2] = 0;
            self->cur = 0;
        }
        break;
    case 1:
        if (self->cur == -1) {
            self->pos[0][0] = 0;
            self->pos[0][1] = 0;
            self->pos[0][2] = 0;
            self->cur = 0;
        }
        break;
    case 2:
    default:
        if (self->cur == -1) {
            self->pos[0][0] = 28;
            self->pos[0][1] = 0;
            self->pos[0][2] = 0;
            self->cur = 0;
        }
        break;
    }
    self->unk34 = self->pos[self->cur][0];
    self->unk38 = self->pos[self->cur][1];
    self->unk3C = self->pos[self->cur][2];
    self->flag[self->cur] = 1;
    self->unk4C = 0;
    self->unk4D = 0;
    self->unk4E = 0;
    self->unk4F = 0;
    self->unk50 = 0;
    self->unk51 = 0;
    self->unk54 = 0;
    self->unk60 = 0;
    self->unk5C = 0;
    self->unk58 = 0;
}

struct beNDMWStatusH {
    char pad[0x1C];
    s32 unk1C, unk20;
    char pad24[0x10];
    s32 unk34, unk38;
};

struct beNDMWStatusTbl {
    u32 count;
    u16 chars[32];
    char pad[0x44 - 4 - 64];
};
extern beNDMWStatusTbl D_004CC860[];

extern "C" void func_003EC120(s32, const char*, s32, s32, void*, s32, s32);
extern "C" void func_003843C0(beUnicodeObj*, s32);
extern bool D_00554C14;
extern void* D_00587110;
void* func_00301000(void* o);
void* func_002F88E0(void* p);
igObject* func_003614F0(void* pool);

struct beNDMWStatusUni {
    beUnicodeObj* p;
    beNDMWStatusUni(beUnicodeObj* o) : p(o) {}
    ~beNDMWStatusUni() {
        beUnicodeObj* o = p;
        if (o != NULL) {
            o->_refCount--;
            if ((o->_refCount & 0x7FFFFF) == 0) {
                func_002FF820(o);
            }
        }
    }
};

/* A wheel of three characters (previous, current, next; wrapping in
   [unk30, unk2C)) from the D_004CC860 tables. */
struct beNDMWStatusWheel {
    char pad[0x1C];
    s32 unk1C; /* font owner */
    s32 unk20; /* ram number */
    char pad24[8];
    s32 unk2C; /* end */
    s32 unk30; /* first */
    s32 unk34; /* current */
};

void func_003DE480(beNDMWStatusWheel* self, s32 mode) {
    switch (mode) {
    case 1:
        func_003EBDC0(self->unk1C, "CHRSELFONT10", self->unk20, 0);
        func_003EBDC0(self->unk1C, "CHRSELFONT10", self->unk20, 1);
        func_003EBDC0(self->unk1C, "CHRSELFONT10", self->unk20, 2);
        break;
    case 0: {
        s32 prev = self->unk34 - 1;

        if (prev < self->unk30) {
            prev = self->unk2C - 1;
        }
        {
            void* a = func_00301000(self);
            beNDMWStatusUni tmp((beUnicodeObj*)func_003614F0(D_00554C14 ? a : func_002F88E0(D_00587110)));
            func_003843C0(tmp.p, 1);
            *(u16*)tmp.p->_buffer = D_004CC860[prev].chars[0];
            func_003EC120(self->unk1C, "CHRSELFONT10", self->unk20, 0, tmp.p, 1, 0);
        }
        {
            s32 cur = self->unk34;
            void* a = func_00301000(self);
            beNDMWStatusUni tmp((beUnicodeObj*)func_003614F0(D_00554C14 ? a : func_002F88E0(D_00587110)));
            func_003843C0(tmp.p, 1);
            *(u16*)tmp.p->_buffer = D_004CC860[cur].chars[0];
            func_003EC120(self->unk1C, "CHRSELFONT10", self->unk20, 1, tmp.p, 1, 0);
        }
        {
            s32 next = self->unk34 + 1 >= self->unk2C ? self->unk30 : self->unk34 + 1;

            void* a = func_00301000(self);
            beNDMWStatusUni tmp((beUnicodeObj*)func_003614F0(D_00554C14 ? a : func_002F88E0(D_00587110)));
            func_003843C0(tmp.p, 1);
            *(u16*)tmp.p->_buffer = D_004CC860[next].chars[0];
            func_003EC120(self->unk1C, "CHRSELFONT10", self->unk20, 2, tmp.p, 1, 0);
        }
        break;
    }
    }
}

void func_003DE7A0(beNDMWStatusH* self, s32 mode) {
    s32 base;
    s32 i;
    s32 idx;

    switch (mode) {
    case 1:
        func_003EBDC0(self->unk1C, "CHRSELFONT11", self->unk20, 0);
        func_003EBDC0(self->unk1C, "CHRSELFONT11", self->unk20, 1);
        func_003EBDC0(self->unk1C, "CHRSELFONT11", self->unk20, 2);
        func_003EBDC0(self->unk1C, "CHRSELFONT11", self->unk20, 3);
        func_003EBDC0(self->unk1C, "CHRSELFONT11", self->unk20, 4);
        break;
    case 0:
        base = self->unk38;
        for (i = 0; i < 5; i++) {
            idx = (base + i) % (s32)D_004CC860[self->unk34].count;
            void* a = func_00301000(self);
            beNDMWStatusUni tmp((beUnicodeObj*)func_003614F0(D_00554C14 ? a : func_002F88E0(D_00587110)));
            func_003843C0(tmp.p, 1);
            *(u16*)tmp.p->_buffer = D_004CC860[self->unk34].chars[idx];
            func_003EC120(self->unk1C, "CHRSELFONT11", self->unk20, i, tmp.p, 1, 0);
        }
        break;
    }
}

/* Save slots: per player, a list of slot numbers; per slot, four entries. */
/* A unicode name: length, then the characters. */
struct beNDMWStatusName {
    u16 len;
    u16 chars[8];
};

struct beNDMWStatusSlotItem {
    char pad[0x8];
    beNDMWStatusName name; /* 0x8 */
    char pad1A[0xE];
    u8 unk28;             /* used */
    char pad29[3];
    s32 unk2C;            /* id */
    const char* unk30;    /* kind */
    const char* unk34;    /* file name */
    s32 unk38;            /* the five numbers shown with it */
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
};

struct beNDMWStatusSlotInfo {
    char pad[0xC];
    igObjectList* unkC;   /* the four entries */
    igIntList* unk10;     /* slot numbers */
};

struct beNDMWStatusSlotOwner {
    char pad[0xF];
    u8 unkF;              /* memory card in */
    char pad10[0x10];
    beNDMWStatusSlotInfo* unk20;
};

struct beNDMWStatusSlots {
    char pad[0x24];
    igObjectList* unk24;  /* single entries */
    igObjectList* unk28;  /* per player / slot */
};

struct beNDMWStatusKind {
    const char* name;
    const char* gen;      /* generate sequence */
    const char* open;     /* open sequence */
};

extern beNDMWStatusKind D_004CEC70[16];
extern char* D_004CC830[4];   /* the four entry models */
extern char D_00516CF8[];     /* "SQ_NOOPEN" */
void func_003E4030(s32, s32, char*, const char*, s32);
extern "C" void func_003EBE00(s32 o, s32 v, s32 a);

extern "C" int strcmp(const char*, const char*);

/* Fill the save area: for each of the player's slots, open the four entries
   (by kind) with their names, or SQ_NOOPEN and no text when empty. */
bool func_003DD6D0(beNDMWStatusSaveArea* self, s32 n) {
    beNDMWStatusSlots* slots = self->unk24;
    beNDMWStatusMdl2* mdl = self->unk1C;
    s32 font = self->unk20;
    s32 state;
    s32 i;
    igIntList* list;

    switch (n) {
    default:
        state = self->unk4C;
        break;
    case 1:
        state = self->unk50;
        break;
    case 2:
        state = self->unk54;
        break;
    case 3:
        state = self->unk58;
        break;
    }
    list = ((beNDMWStatusSlotOwner**)slots->unk28->_data)[self->unk3C]->unk20->unk10;
    for (i = 0; i < list->_count; i++) {
        s32 slot = ((s32*)list->_data)[i];
        s32 j;

        for (j = 0; j < 4; j++) {
            beNDMWStatusSlotItem* item =
                ((beNDMWStatusSlotItem**)((beNDMWStatusSlotOwner**)slots->unk28->_data)[slot]->unk20->unkC->_data)[j];

            if (item->unk28 == 1) {
                s32 kind = 0;
                s32 k;

                for (k = 0; k < 16; k++) {
                    if (strcmp(D_004CEC70[k].name, item->unk30) == 0) {
                        kind = k;
                        break;
                    }
                }
                func_003E4030((s32)mdl, slot, D_004CC830[j], D_004CEC70[kind].open, 0);
                {
                    void* a = func_00301000(self);
                    beNDMWStatusUni tmp((beUnicodeObj*)func_003614F0(D_00554C14 ? a : func_002F88E0(D_00587110)));
                    u16* dst;
                    beNDMWStatusName* nm;
                    s32 c;

                    func_003843C0(tmp.p, 8);
                    dst = (u16*)tmp.p->_buffer;
                    nm = &item->name;
                    for (c = 0; c < 8; c++) {
                        dst[c] = nm->chars[c];
                        if (dst[c] == 0) {
                            break;
                        }
                    }
                    for (; c < 8; c++) {
                        dst[c] = 0;
                    }
                    func_003EC120(font, D_00516DC8, slot, j, tmp.p, 1, 0);
                }
            } else {
                func_003E4030((s32)mdl, slot, D_004CC830[j], D_00516CF8, 0);
                func_003EBE00(font, ((slot << 16) | 0x300) | j, 0);
            }
        }
    }
    switch (n) {
    default:
        self->unk4C = state;
        self->unk58 = 0;
        self->unk54 = 0;
        self->unk50 = 0;
        break;
    case 1:
        self->unk50 = state;
        self->unk58 = 0;
        self->unk54 = 0;
        break;
    case 2:
        self->unk54 = state;
        self->unk58 = 0;
        break;
    case 3:
        self->unk58 = state;
        break;
    }
    return true;
}

/* Close the second list, then generate the player's own entries and write
   their names once the models are ready. */
bool func_003DC590(beNDMWStatusSaveArea* self, s32 n) {
    beNDMWStatusSlots* slots = self->unk24;
    beNDMWStatusMdlPos* mdl = (beNDMWStatusMdlPos*)self->unk1C;
    bool done = false;
    s32 font = self->unk20;
    beNDMWStatusB2In* list = self->unk2C;
    s32 state;

    switch (n) {
    default:
        state = self->unk4C;
        break;
    case 1:
        state = self->unk50;
        break;
    case 2:
        state = self->unk54;
        break;
    case 3:
        state = self->unk58;
        break;
    }
    switch (state) {
    case 0:
        func_003D9E70(list->unk10->unk10->unk4, 0);
        self->unk32 = 0;
        state++;
        break;
    case 1:
        if (func_003DA7C0(list->unk10->unk10->unk4) == 1) {
            state++;
        }
        break;
    case 2: {
        s32 j;

        for (j = 0; j < 4; j++) {
            beNDMWStatusSlotItem* item =
                ((beNDMWStatusSlotItem**)((beNDMWStatusSlotOwner**)slots->unk28->_data)[self->unk3C]->unk20->unkC->_data)[j];

            if (item->unk28 == 1) {
                s32 kind = 0;
                s32 k;

                for (k = 0; k < 16; k++) {
                    if (strcmp(D_004CEC70[k].name, item->unk30) == 0) {
                        kind = k;
                        break;
                    }
                }
                func_003E3E00((beNDMWStatusMdl*)mdl, self->unk3C, D_004CC830[j], D_004CEC70[kind].gen, mdl->pos, 1);
            } else {
                func_003E3E00((beNDMWStatusMdl*)mdl, self->unk3C, D_004CC830[j], D_00516CF8, mdl->pos, 1);
            }
        }
        self->unk30 = 1;
        state++;
        break;
    }
    case 3:
        if (func_003E4280((beNDMWStatusCnt*)mdl) == true) {
            s32 j;

            for (j = 0; j < 4; j++) {
                s32 ram = self->unk3C;
                beNDMWStatusSlotItem* item =
                    ((beNDMWStatusSlotItem**)((beNDMWStatusSlotOwner**)slots->unk28->_data)[ram]->unk20->unkC->_data)[j];

                if (item->unk28 == 1) {
                    void* a = func_00301000(self);
                    beNDMWStatusUni tmp((beUnicodeObj*)func_003614F0(D_00554C14 ? a : func_002F88E0(D_00587110)));
                    u16* dst;
                    beNDMWStatusName* nm;
                    s32 c;

                    func_003843C0(tmp.p, 8);
                    dst = (u16*)tmp.p->_buffer;
                    nm = &item->name;
                    for (c = 0; c < 8; c++) {
                        dst[c] = nm->chars[c];
                        if (dst[c] == 0) {
                            break;
                        }
                    }
                    for (; c < 8; c++) {
                        dst[c] = 0;
                    }
                    func_003EC120(font, D_00516DC8, self->unk3C, j, tmp.p, 1, 0);
                } else {
                    func_003EBE00(font, ((ram << 16) | 0x300) | j, 0);
                }
            }
            state++;
        }
        break;
    case 4:
        done = true;
        break;
    }
    switch (n) {
    default:
        self->unk4C = state;
        self->unk58 = 0;
        self->unk54 = 0;
        self->unk50 = 0;
        break;
    case 1:
        self->unk50 = state;
        self->unk58 = 0;
        self->unk54 = 0;
        break;
    case 2:
        self->unk54 = state;
        self->unk58 = 0;
        break;
    case 3:
        self->unk58 = state;
        break;
    }
    return done;
}

/* Generate the player's own four entries (by kind), open the first list, then
   once both are ready write the entry names. */
bool func_003DCC60(beNDMWStatusSaveArea* self, s32 n) {
    beNDMWStatusSlots* slots = self->unk24;
    beNDMWStatusMdlPos* mdl = (beNDMWStatusMdlPos*)self->unk1C;
    beNDMWStatusB2In* list;
    bool done = false;
    s32 font = self->unk20;
    s32 state;

    list = self->unk2C;
    switch (n) {
    default:
        state = self->unk4C;
        break;
    case 1:
        state = self->unk50;
        break;
    case 2:
        state = self->unk54;
        break;
    case 3:
        state = self->unk58;
        break;
    }
    switch (state) {
    case 0: {
        s32 j;

        for (j = 0; j < 4; j++) {
            beNDMWStatusSlotItem* item =
                ((beNDMWStatusSlotItem**)((beNDMWStatusSlotOwner**)slots->unk28->_data)[self->unk3C]->unk20->unkC->_data)[j];

            if (item->unk28 == 1) {
                s32 kind = 0;
                s32 k;

                for (k = 0; k < 16; k++) {
                    if (strcmp(D_004CEC70[k].name, item->unk30) == 0) {
                        kind = k;
                        break;
                    }
                }
                func_003E3E00((beNDMWStatusMdl*)mdl, self->unk3C, D_004CC830[j], D_004CEC70[kind].gen,
                              mdl->pos, 1);
            } else {
                func_003E3E00((beNDMWStatusMdl*)mdl, self->unk3C, D_004CC830[j], D_00516CF8,
                              mdl->pos, 1);
            }
        }
        self->unk30 = 1;
        func_003D9E10(list->unk10->unk10->unk0, 1, self->unk3C);
        self->unk31 = 1;
        state++;
        break;
    }
    case 1:
        if (func_003E4280((beNDMWStatusCnt*)mdl) == true && func_003DA7C0(list->unk10->unk10->unk0) == 1) {
            s32 j;

            state++;
            for (j = 0; j < 4; j++) {
                s32 ram = self->unk3C;
                beNDMWStatusSlotItem* item =
                    ((beNDMWStatusSlotItem**)((beNDMWStatusSlotOwner**)slots->unk28->_data)[ram]->unk20->unkC->_data)[j];

                if (item->unk28 == 1) {
                    void* a = func_00301000(self);
                    beNDMWStatusUni tmp((beUnicodeObj*)func_003614F0(D_00554C14 ? a : func_002F88E0(D_00587110)));
                    u16* dst;
                    beNDMWStatusName* nm;
                    s32 c;

                    func_003843C0(tmp.p, 8);
                    dst = (u16*)tmp.p->_buffer;
                    nm = &item->name;
                    for (c = 0; c < 8; c++) {
                        dst[c] = nm->chars[c];
                        if (dst[c] == 0) {
                            break;
                        }
                    }
                    for (; c < 8; c++) {
                        dst[c] = 0;
                    }
                    func_003EC120(font, D_00516DC8, self->unk3C, j, tmp.p, 1, 0);
                } else {
                    func_003EBE00(font, ((ram << 16) | 0x300) | j, 0);
                }
            }
        }
        break;
    case 2:
        done = true;
        break;
    }
    switch (n) {
    default:
        self->unk4C = state;
        self->unk58 = 0;
        self->unk54 = 0;
        self->unk50 = 0;
        break;
    case 1:
        self->unk50 = state;
        self->unk58 = 0;
        self->unk54 = 0;
        break;
    case 2:
        self->unk54 = state;
        self->unk58 = 0;
        break;
    case 3:
        self->unk58 = state;
        break;
    }
    return done;
}

/* Draw the text being typed (one character per box, highlighted or plain by its
   flag) and clear the boxes after it: six in mode 0, eight otherwise. */
void func_003DE990(beNDMWStatusCur* self, s32 n) {
    switch (self->mode) {
    case 0:
        switch (n) {
        case 1:
            func_003EBDC0(self->unk1C, "CHRSELFONT12", self->unk20, 0);
            func_003EBDC0(self->unk1C, "CHRSELFONT12", self->unk20, 1);
            func_003EBDC0(self->unk1C, "CHRSELFONT12", self->unk20, 2);
            func_003EBDC0(self->unk1C, "CHRSELFONT12", self->unk20, 3);
            func_003EBDC0(self->unk1C, "CHRSELFONT12", self->unk20, 4);
            func_003EBDC0(self->unk1C, "CHRSELFONT12", self->unk20, 5);
            break;
        case 0: {
            s32 i;

            for (i = 0; i < 6; i++) {
                if (self->val[i] == 0) {
                    break;
                }
                switch (self->flag[i]) {
                case 0:
                    break;
                case 1: {
                    void* a = func_00301000(self);
                    beNDMWStatusUni tmp((beUnicodeObj*)func_003614F0(D_00554C14 ? a : func_002F88E0(D_00587110)));
                    func_003843C0(tmp.p, 1);
                    *(u16*)tmp.p->_buffer = self->val[i];
                    func_003EC120(self->unk1C, "CHRSELFONT12", self->unk20, i, tmp.p, 1, 1);
                    break;
                }
                case 2: {
                    void* a = func_00301000(self);
                    beNDMWStatusUni tmp((beUnicodeObj*)func_003614F0(D_00554C14 ? a : func_002F88E0(D_00587110)));
                    func_003843C0(tmp.p, 1);
                    *(u16*)tmp.p->_buffer = self->val[i];
                    func_003EC120(self->unk1C, "CHRSELFONT12", self->unk20, i, tmp.p, 1, 0);
                    break;
                }
                }
            }
            for (; i < 6; i++) {
                func_003EBDC0(self->unk1C, "CHRSELFONT12", self->unk20, i);
            }
            break;
        }
        }
        break;
    case 1:
    default:
        switch (n) {
        case 1:
            func_003EBDC0(self->unk1C, "CHRSELFONT13", self->unk20, 0);
            func_003EBDC0(self->unk1C, "CHRSELFONT13", self->unk20, 1);
            func_003EBDC0(self->unk1C, "CHRSELFONT13", self->unk20, 2);
            func_003EBDC0(self->unk1C, "CHRSELFONT13", self->unk20, 3);
            func_003EBDC0(self->unk1C, "CHRSELFONT13", self->unk20, 4);
            func_003EBDC0(self->unk1C, "CHRSELFONT13", self->unk20, 5);
            func_003EBDC0(self->unk1C, "CHRSELFONT13", self->unk20, 6);
            func_003EBDC0(self->unk1C, "CHRSELFONT13", self->unk20, 7);
            break;
        case 0: {
            s32 i;

            for (i = 0; i < 8; i++) {
                if (self->val[i] == 0) {
                    break;
                }
                switch (self->flag[i]) {
                case 0:
                    break;
                case 1: {
                    void* a = func_00301000(self);
                    beNDMWStatusUni tmp((beUnicodeObj*)func_003614F0(D_00554C14 ? a : func_002F88E0(D_00587110)));
                    func_003843C0(tmp.p, 1);
                    *(u16*)tmp.p->_buffer = self->val[i];
                    func_003EC120(self->unk1C, "CHRSELFONT13", self->unk20, i, tmp.p, 1, 1);
                    break;
                }
                case 2: {
                    void* a = func_00301000(self);
                    beNDMWStatusUni tmp((beUnicodeObj*)func_003614F0(D_00554C14 ? a : func_002F88E0(D_00587110)));
                    func_003843C0(tmp.p, 1);
                    *(u16*)tmp.p->_buffer = self->val[i];
                    func_003EC120(self->unk1C, "CHRSELFONT13", self->unk20, i, tmp.p, 1, 0);
                    break;
                }
                }
            }
            for (; i < 8; i++) {
                func_003EBDC0(self->unk1C, "CHRSELFONT13", self->unk20, i);
            }
            break;
        }
        }
        break;
    }
}

void func_003DEF70(beNDMWStatusSel* self, s8 v) {
    self->unk50 = v;
    self->unk4F = 1;
}

bePadData* bePadManager_getPad(bePadManager* mgr, s32 n);

/* Run the line choice: open the window, move the cursor with the pad (up/down,
   0x4000 picks, 0x2000 cancels), then close it with the chosen line's sequence. */
u8 func_003DDB80(beNDMWStatusF* self, u8 input) {
    beNDMWStatusMdlPos* mdl;
    s32 win;
    s32 msg;
    s32 trig;
    bool up;
    s32 rep;
    bool down;

    if (self->unk24 == 1) {
        return 1;
    }
    mdl = self->unk2C;
    win = self->unk30;
    msg = self->unk38;
    trig = 0;
    up = false;
    down = false;
    if (input == 1) {
        bePadData* pad = bePadManager_getPad(self->unk28, self->unk18);
        s32 stick;

        trig = pad->_trigger;
        rep = pad->_repeat;
        stick = pad->_lTrg | pad->_lRpt;
        {
            bool u = true;

            if ((stick & 1) == 0 && ((trig | rep) & 0x10) == 0) {
                u = false;
            }
            up = u;
        }
        down = (stick & 4) || ((trig | rep) & 0x40);
    }
    switch (self->unk3C) {
    case 0:
        switch (self->unk40) {
        case 0:
            switch (self->unk20) {
            case 0:
                func_003E3E00((beNDMWStatusMdl*)mdl, self->unk18, "j_linewin02", "SQ_OPEN", mdl->pos, 1);
                break;
            case 1:
                func_003E3E00((beNDMWStatusMdl*)mdl, self->unk18, "j_linewin03", "SQ_OPEN", mdl->pos, 1);
                break;
            }
            self->unk40++;
            self->unk48 = 0;
            self->unk44 = 0;
            break;
        case 1:
            if (func_003E4280((beNDMWStatusCnt*)mdl) == true) {
                switch (self->unk20) {
                case 0:
                    func_003EBE00(win, (self->unk18 << 16) | 0x402, 0);
                    break;
                case 1:
                    func_003EBE00(win, (self->unk18 << 16) | 0x403, 0);
                    break;
                }
                func_003ECAD0(msg, (self->unk18 << 16) | 0x404);
                self->unk3C++;
                self->unk48 = 0;
                self->unk44 = 0;
                self->unk40 = 0;
            }
            break;
        }
        break;
    case 1:
        switch (self->unk40) {
        case 0:
            if (up) {
                self->unk10--;
                if (self->unk10 < 0) {
                    self->unk10 = 0;
                } else {
                    switch (self->unk20) {
                    case 0:
                        func_003E4030((s32)mdl, self->unk18, "j_linewin02", "SQ_LINEUP00", 1);
                        break;
                    case 1:
                        if (self->unk10 == 0) {
                            func_003E4030((s32)mdl, self->unk18, "j_linewin03", "SQ_LINEUP00", 1);
                        } else {
                            func_003E4030((s32)mdl, self->unk18, "j_linewin03", "SQ_LINEUP01", 1);
                        }
                        break;
                    }
                    self->unk40++;
                    self->unk48 = 0;
                    self->unk44 = 0;
                }
            } else if (down) {
                self->unk10++;
                if (self->unk14 - 1 < self->unk10) {
                    self->unk10 = self->unk14 - 1;
                } else {
                    switch (self->unk20) {
                    case 0:
                        func_003E4030((s32)mdl, self->unk18, "j_linewin02", "SQ_LINEDW01", 1);
                        break;
                    case 1:
                        if (self->unk10 == 1) {
                            func_003E4030((s32)mdl, self->unk18, "j_linewin03", "SQ_LINEDW01", 1);
                        } else {
                            func_003E4030((s32)mdl, self->unk18, "j_linewin03", "SQ_LINEDW02", 1);
                        }
                        break;
                    }
                    self->unk40++;
                    self->unk48 = 0;
                    self->unk44 = 0;
                }
            } else if (trig & 0x4000) {
                self->unk1C = self->unk10;
                self->unk3C++;
                self->unk48 = 0;
                self->unk44 = 0;
                self->unk40 = 0;
            } else if (trig & 0x2000) {
                self->unk25 = 1;
                self->unk3C++;
                self->unk48 = 0;
                self->unk44 = 0;
                self->unk40 = 0;
            }
            break;
        case 1:
            if (func_003E4280((beNDMWStatusCnt*)mdl) == true) {
                self->unk40 = 0;
                self->unk48 = 0;
                self->unk44 = 0;
            }
            break;
        }
        break;
    case 2:
        switch (self->unk40) {
        case 0:
            switch (self->unk20) {
            case 0:
                func_003EC050(win, (self->unk18 << 16) | 0x402);
                if (self->unk25 == 0) {
                    if (self->unk10 == 0) {
                        func_003E42A0((beNDMWStatusMdl2*)mdl, self->unk18, "j_linewin02", "SQ_LINEON00", 1);
                    } else {
                        func_003E42A0((beNDMWStatusMdl2*)mdl, self->unk18, "j_linewin02", "SQ_LINEON01", 1);
                    }
                } else {
                    func_003E42A0((beNDMWStatusMdl2*)mdl, self->unk18, "j_linewin02", "SQ_CANCEL", 1);
                }
                break;
            case 1:
                func_003EC050(win, (self->unk18 << 16) | 0x403);
                if (self->unk25 == 0) {
                    switch (self->unk10) {
                    case 0:
                        func_003E42A0((beNDMWStatusMdl2*)mdl, self->unk18, "j_linewin03", "SQ_LINEON00", 1);
                        break;
                    case 1:
                        func_003E42A0((beNDMWStatusMdl2*)mdl, self->unk18, "j_linewin03", "SQ_LINEON01", 1);
                        break;
                    case 2:
                        func_003E42A0((beNDMWStatusMdl2*)mdl, self->unk18, "j_linewin03", "SQ_LINEON02", 1);
                        break;
                    }
                } else {
                    func_003E42A0((beNDMWStatusMdl2*)mdl, self->unk18, "j_linewin03", "SQ_CANCEL", 1);
                }
                break;
            }
            self->unk40++;
            self->unk48 = 0;
            self->unk44 = 0;
            break;
        case 1:
            if (func_003E44D0((beNDMWStatusCnt2*)mdl) == true) {
                self->unk24 = 1;
            }
            break;
        }
        break;
    }
    return self->unk24;
}

/* An entry's details (name, kind name from the shinka database and its five
   numbers) on the left (unk1C 0) or right panel; cleared when the entry is empty. */
struct beNDMWStatusDetail {
    char pad[0x8];
    void* unk8;                 /* messenger */
    beNDMWStatusMdlPos* unkC;   /* models */
    s32 unk10;                  /* font owner */
    beNDMWStatusSlots* unk14;
    s32 unk18;                  /* ram number */
    s32 unk1C;                  /* panel */
    u8 unk20;                   /* done */
    char pad21[3];
    s32 unk24;                  /* how to close: 0, line on, cancel */
    s32 unk28;                  /* source: 0 the player's slot entries */
    s32 unk2C;                  /* entry */
    u8 unk30;                   /* entry used */
    char pad31[3];
    s32 unk34;                  /* 1 open, 2 close, 3 refresh */
    s32 unk38;                  /* step */
    s32 unk3C;
    s32 unk40;
};

extern char D_00516C50[]; /* "DATABASE" */
extern char D_00516C60[]; /* "shinka" */
extern "C" void func_003EBD70(s32 o, const char* font, s32 ram, s32 box, const char* s, s32 a, s32 b);
extern "C" int sprintf(char* buf, const char* fmt, ...);
igObject* beMessenger_getObject(void* m, s32 i);
void beMessenger_call(void* m, const char* group, const char* info, s32 ram, const char* message);
const char* beDataBase_getString(beDataBase* db, s32 record, const char* field);

void func_003D9EC0(beNDMWStatusDetail* self) {
    beNDMWStatusSlots* slots = self->unk14;

    if (self->unk1C == 0) {
        beNDMWStatusSlotItem* item;

        if (self->unk28 == 0) {
            item = ((beNDMWStatusSlotItem**)((beNDMWStatusSlotOwner**)slots->unk28->_data)[self->unk18]->unk20->unkC->_data)[self->unk2C];
        } else {
            item = ((beNDMWStatusSlotItem**)slots->unk24->_data)[self->unk18];
        }
        if (item->unk28 == 1) {
            s32 kind;
            beDataBase* db;
            s32 k;
            char buf[32];

            {
                void* a = func_00301000(self);
                beNDMWStatusUni tmp((beUnicodeObj*)func_003614F0(D_00554C14 ? a : func_002F88E0(D_00587110)));
                u16* dst;
                beNDMWStatusName* nm;
                s32 c;

                func_003843C0(tmp.p, 8);
                dst = (u16*)tmp.p->_buffer;
                nm = &item->name;
                for (c = 0; c < 8; c++) {
                    dst[c] = nm->chars[c];
                    if (dst[c] == 0) {
                        break;
                    }
                }
                for (; c < 8; c++) {
                    dst[c] = 0;
                }
                func_003EC120(self->unk10, "CHRSELFONT03", self->unk18, 0, tmp.p, 0, 0);
            }
            beMessenger_call(self->unk8, D_00516C50, D_00516C60, 0, "GETDATABASE");
            db = (beDataBase*)beMessenger_getObject(self->unk8, 0);
            for (k = 0; k < 16; k++) {
                if (strcmp(D_004CEC70[k].name, item->unk30) == 0) {
                    kind = k;
                    break;
                }
            }
            func_003EBD70(self->unk10, "CHRSELFONT04", self->unk18, 10, beDataBase_getString(db, kind, "NAME"), 0, 0);
            sprintf(buf, "%d", item->unk38);
            func_003EBD70(self->unk10, "CHRSELFONT04", self->unk18, 0, buf, 1, 0);
            sprintf(buf, "%d", item->unk3C);
            func_003EBD70(self->unk10, "CHRSELFONT05", self->unk18, 0, buf, 2, 0);
            sprintf(buf, "%d", item->unk40);
            func_003EBD70(self->unk10, "CHRSELFONT05", self->unk18, 10, buf, 2, 0);
            sprintf(buf, "%d", item->unk44);
            func_003EBD70(self->unk10, "CHRSELFONT05", self->unk18, 1, buf, 2, 0);
            sprintf(buf, "%d", item->unk48);
            func_003EBD70(self->unk10, "CHRSELFONT05", self->unk18, 11, buf, 2, 0);
        } else {
            func_003EBE00(self->unk10, (self->unk18 << 16) | 0x10, 0);
        }
    } else {
        beNDMWStatusSlotItem* item;

        if (self->unk28 == 0) {
            item = ((beNDMWStatusSlotItem**)((beNDMWStatusSlotOwner**)slots->unk28->_data)[self->unk18]->unk20->unkC->_data)[self->unk2C];
        } else {
            item = ((beNDMWStatusSlotItem**)slots->unk24->_data)[self->unk18];
        }
        if (item->unk28 == 1) {
            s32 kind;
            beDataBase* db;
            s32 k;
            char buf[32];

            {
                void* a = func_00301000(self);
                beNDMWStatusUni tmp((beUnicodeObj*)func_003614F0(D_00554C14 ? a : func_002F88E0(D_00587110)));
                u16* dst;
                beNDMWStatusName* nm;
                s32 c;

                func_003843C0(tmp.p, 8);
                dst = (u16*)tmp.p->_buffer;
                nm = &item->name;
                for (c = 0; c < 8; c++) {
                    dst[c] = nm->chars[c];
                    if (dst[c] == 0) {
                        break;
                    }
                }
                for (; c < 8; c++) {
                    dst[c] = 0;
                }
                func_003EC120(self->unk10, "CHRSELFONT03", self->unk18, 10, tmp.p, 0, 0);
            }
            beMessenger_call(self->unk8, D_00516C50, D_00516C60, 0, "GETDATABASE");
            db = (beDataBase*)beMessenger_getObject(self->unk8, 0);
            for (k = 0; k < 16; k++) {
                if (strcmp(D_004CEC70[k].name, item->unk30) == 0) {
                    kind = k;
                    break;
                }
            }
            func_003EBD70(self->unk10, "CHRSELFONT04", self->unk18, 11, beDataBase_getString(db, kind, "NAME"), 0, 0);
            sprintf(buf, "%d", item->unk38);
            func_003EBD70(self->unk10, "CHRSELFONT04", self->unk18, 1, buf, 1, 0);
            sprintf(buf, "%d", item->unk3C);
            func_003EBD70(self->unk10, "CHRSELFONT05", self->unk18, 2, buf, 2, 0);
            sprintf(buf, "%d", item->unk40);
            func_003EBD70(self->unk10, "CHRSELFONT05", self->unk18, 12, buf, 2, 0);
            sprintf(buf, "%d", item->unk44);
            func_003EBD70(self->unk10, "CHRSELFONT05", self->unk18, 3, buf, 2, 0);
            sprintf(buf, "%d", item->unk48);
            func_003EBD70(self->unk10, "CHRSELFONT05", self->unk18, 13, buf, 2, 0);
        } else {
            func_003EBE00(self->unk10, (self->unk18 << 16) | 0x110, 0);
        }
    }
}

void func_003DA5F0(beNDMWStatusG* self);
void func_003D9EC0(beNDMWStatusDetail* self);

#ifdef NON_MATCHING
/* Nonmatching: only in mode 3 (18 off): the open sequence lookup D_004CEC70[kind].open is scheduled differently (the original loads the table address before the window name). */
/* Open (1), close (2) or refresh without animation (3) an entry's windows (face
   and status, or the save check window on the right panel), then draw its details. */
u8 func_003DA7C0(beNDMWStatusDetail* self) {
    beNDMWStatusSlots* slots;

    if (self->unk20 == 1) {
        return 1;
    }
    slots = self->unk14;
    switch (self->unk34) {
    case 0:
        break;
    case 1:
        switch (self->unk38) {
        case 0: {
            beNDMWStatusSlotItem* item;

        if (self->unk28 == 0) {
            item = ((beNDMWStatusSlotItem**)((beNDMWStatusSlotOwner**)slots->unk28->_data)[self->unk18]->unk20->unkC->_data)[self->unk2C];
        } else {
            item = ((beNDMWStatusSlotItem**)slots->unk24->_data)[self->unk18];
        }
        if (item->unk28 == 1) {
            self->unk30 = 1;
        } else {
            self->unk30 = 0;
        }
            if (self->unk30 == 1) {
            s32 kind = 0;
            s32 k;

            for (k = 0; k < 16; k++) {
                if (strcmp(D_004CEC70[k].name, item->unk30) == 0) {
                    kind = k;
                    break;
                }
            }
                if (self->unk1C == 0) {
                    func_003E3E00((beNDMWStatusMdl*)self->unkC, self->unk18, "j_digifacewin", D_004CEC70[kind].gen, self->unkC->pos, 1);
                    func_003E3E00((beNDMWStatusMdl*)self->unkC, self->unk18, "j_mainstatuswin", "SQ_OPEN", self->unkC->pos, 1);
                } else {
                    func_003E3E00((beNDMWStatusMdl*)self->unkC, self->unk18, "j_savecheckwin", D_004CEC70[kind].gen, self->unkC->pos, 1);
                }
            } else if (self->unk1C == 0) {
                func_003E3E00((beNDMWStatusMdl*)self->unkC, self->unk18, "j_digifacewin", "SQ_nofaceOPEN", self->unkC->pos, 1);
                func_003E3E00((beNDMWStatusMdl*)self->unkC, self->unk18, "j_mainstatuswin", "SQ_NOOPEN", self->unkC->pos, 1);
            } else {
                func_003E3E00((beNDMWStatusMdl*)self->unkC, self->unk18, "j_savecheckwin", "SQ_nofaceOPEN", self->unkC->pos, 1);
            }
            self->unk38++;
            self->unk40 = 0;
            self->unk3C = 0;
            break;
        }
        case 1:
            if (func_003E4280((beNDMWStatusCnt*)self->unkC) == true) {
                func_003D9EC0(self);
                self->unk20 = 1;
                self->unk34 = 0;
                self->unk40 = 0;
                self->unk3C = 0;
                self->unk38 = 0;
            }
            break;
        }
        break;
    case 3: {
        beNDMWStatusSlotItem* item;

        if (self->unk28 == 0) {
            item = ((beNDMWStatusSlotItem**)((beNDMWStatusSlotOwner**)slots->unk28->_data)[self->unk18]->unk20->unkC->_data)[self->unk2C];
        } else {
            item = ((beNDMWStatusSlotItem**)slots->unk24->_data)[self->unk18];
        }
        if (item->unk28 == 1) {
            self->unk30 = 1;
        } else {
            self->unk30 = 0;
        }
        if (self->unk30 == 1) {
            s32 kind = 0;
            s32 k;

            for (k = 0; k < 16; k++) {
                if (strcmp(D_004CEC70[k].name, item->unk30) == 0) {
                    kind = k;
                    break;
                }
            }
            if (self->unk1C == 0) {
                func_003E4030((s32)self->unkC, self->unk18, "j_digifacewin", D_004CEC70[kind].open, 0);
                func_003E4030((s32)self->unkC, self->unk18, "j_mainstatuswin", "SQ_WAIT", 0);
            } else {
                func_003E4030((s32)self->unkC, self->unk18, "j_savecheckwin", D_004CEC70[kind].open, 0);
            }
        } else if (self->unk1C == 0) {
            func_003E4030((s32)self->unkC, self->unk18, "j_digifacewin", "SQ_noface", 0);
            func_003E4030((s32)self->unkC, self->unk18, "j_mainstatuswin", "SQ_NOWAIT", 0);
        } else {
            func_003E4030((s32)self->unkC, self->unk18, "j_savecheckwin", "SQ_noface", 0);
        }
        func_003DA5F0((beNDMWStatusG*)self);
        func_003D9EC0(self);
        self->unk20 = 1;
        self->unk34 = 0;
        self->unk40 = 0;
        self->unk3C = 0;
        self->unk38 = 0;
        break;
    }
    case 2: {
        beNDMWStatusSlotItem* item;

        if (self->unk28 == 0) {
            item = ((beNDMWStatusSlotItem**)((beNDMWStatusSlotOwner**)slots->unk28->_data)[self->unk18]->unk20->unkC->_data)[self->unk2C];
        } else {
            item = ((beNDMWStatusSlotItem**)slots->unk24->_data)[self->unk18];
        }
        if (item->unk28 == 1) {
            self->unk30 = 1;
        } else {
            self->unk30 = 0;
        }
        switch (self->unk38) {
        case 0:
            if (self->unk1C == 0) {
                func_003E42A0((beNDMWStatusMdl2*)self->unkC, self->unk18, "j_digifacewin", "SQ_CLOSE", 1);
                if (self->unk30 == 1) {
                    switch (self->unk24) {
                    case 0:
                        func_003E42A0((beNDMWStatusMdl2*)self->unkC, self->unk18, "j_mainstatuswin", "SQ_CLOSE", 1);
                        break;
                    case 1:
                        func_003E42A0((beNDMWStatusMdl2*)self->unkC, self->unk18, "j_mainstatuswin", "SQ_LINEON00", 1);
                        break;
                    case 2:
                        func_003E42A0((beNDMWStatusMdl2*)self->unkC, self->unk18, "j_mainstatuswin", "SQ_CANCEL", 1);
                        break;
                    }
                } else {
                    func_003E42A0((beNDMWStatusMdl2*)self->unkC, self->unk18, "j_mainstatuswin", "SQ_NOCLOSE", 1);
                }
            } else {
                func_003E42A0((beNDMWStatusMdl2*)self->unkC, self->unk18, "j_savecheckwin", "SQ_CLOSE", 1);
            }
            func_003DA5F0((beNDMWStatusG*)self);
            self->unk38++;
            self->unk40 = 0;
            self->unk3C = 0;
            break;
        case 1:
            if (func_003E44D0((beNDMWStatusCnt2*)self->unkC) == true) {
                self->unk20 = 1;
                self->unk34 = 0;
                self->unk40 = 0;
                self->unk3C = 0;
                self->unk38 = 0;
            }
            break;
        }
        break;
    }
    }
    return self->unk20;
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWStatus", func_003DA7C0);
#endif

/* Open the player's own save area: generate the four entries and the cursor, say
   whether the chosen entry is already in another player's slot (0x515) or not
   (0x513), open the first list, then write the names once all is ready. */
bool func_003DB060(beNDMWStatusSaveArea* self, s32 n) {
    beNDMWStatusSlots* slots;
    beNDMWStatusMdlPos* mdl;
    s32 msg;
    beNDMWStatusB2In* list;
    bool done;
    s32 font;
    s32 state;

    slots = self->unk24;
    mdl = (beNDMWStatusMdlPos*)self->unk1C;
    done = false;
    font = self->unk20;
    msg = self->unk28;
    list = self->unk2C;
    switch (n) {
    default:
        state = self->unk4C;
        break;
    case 1:
        state = self->unk50;
        break;
    case 2:
        state = self->unk54;
        break;
    case 3:
        state = self->unk58;
        break;
    }
    switch (state) {
    case 0: {
        s32 j;

        for (j = 0; j < 4; j++) {
            s32 kind;
            s32 k;
            beNDMWStatusSlotItem* item =
                ((beNDMWStatusSlotItem**)((beNDMWStatusSlotOwner**)slots->unk28->_data)[self->unk3C]->unk20->unkC->_data)[j];

            if (item->unk28 == 1) {
                if (*item->unk30 != 0) {
                    kind = 0;
                    for (k = 0; k < 16; k++) {
                        if (strcmp(D_004CEC70[k].name, item->unk30) == 0) {
                            kind = k;
                            break;
                        }
                    }
                    func_003E3E00((beNDMWStatusMdl*)mdl, self->unk3C, D_004CC830[j], D_004CEC70[kind].gen, mdl->pos, 1);
                } else {
                    item->unk28 = 0;
                    func_003E3E00((beNDMWStatusMdl*)mdl, self->unk3C, D_004CC830[j], D_00516CF8, mdl->pos, 1);
                }
            } else {
                item->unk28 = 0;
                func_003E3E00((beNDMWStatusMdl*)mdl, self->unk3C, D_004CC830[j], D_00516CF8, mdl->pos, 1);
            }
        }
        self->unk30 = 1;
        func_003E3E00((beNDMWStatusMdl*)mdl, self->unk3C, D_00516DB0, "SQ_OPEN", mdl->pos, 1);
        self->unk34 = 1;
        if (self->unk38 == 0) {
            bool free;
            s32 i;
            beNDMWStatusSlotItem* cur =
                ((beNDMWStatusSlotItem**)((beNDMWStatusSlotOwner**)slots->unk28->_data)[self->unk3C]->unk20->unkC->_data)[self->unk14];

            free = true;
            if (cur->unk28 == 1) {
                for (i = 0; i < 4; i++) {
                    if (i != self->unk3C) {
                        beNDMWStatusSlotItem* o = ((beNDMWStatusSlotItem**)slots->unk24->_data)[i];

                        if (o->unk28 == 1 && o->unk2C == cur->unk2C) {
                            const char* a = o->unk34;
                            const char* b = cur->unk34;

                            if (a != NULL && b != NULL && strcmp(a, b) == 0) {
                                free = false;
                                break;
                            }
                        }
                    }
                }
            }
            if (free == true) {
                self->unk40 = 0;
                func_003ECAD0(msg, (self->unk3C << 16) | 0x513);
            } else {
                self->unk40 = 1;
                func_003ECAD0(msg, (self->unk3C << 16) | 0x515);
            }
            func_003D9E10(list->unk10->unk10->unk0, 0, self->unk14);
        } else {
            self->unk40 = 0;
            func_003ECAD0(msg, (self->unk3C << 16) | 0x503);
            func_003D9E10(list->unk10->unk10->unk0, 1, self->unk3C);
        }
        self->unk31 = 1;
        state++;
        break;
    }
    case 1:
        if (func_003DA7C0(list->unk10->unk10->unk0) == 1 && func_003E4280((beNDMWStatusCnt*)mdl) == true) {
            s32 j;

            for (j = 0; j < 4; j++) {
                beNDMWStatusSlotItem* item =
                    ((beNDMWStatusSlotItem**)((beNDMWStatusSlotOwner**)slots->unk28->_data)[self->unk3C]->unk20->unkC->_data)[j];

                if (item->unk28 == 1) {
                    void* a = func_00301000(self);
                    beNDMWStatusUni tmp((beUnicodeObj*)func_003614F0(D_00554C14 ? a : func_002F88E0(D_00587110)));
                    u16* dst;
                    beNDMWStatusName* nm;
                    s32 c;

                    func_003843C0(tmp.p, 8);
                    dst = (u16*)tmp.p->_buffer;
                    nm = &item->name;
                    for (c = 0; c < 8; c++) {
                        dst[c] = nm->chars[c];
                        if (dst[c] == 0) {
                            break;
                        }
                    }
                    for (; c < 8; c++) {
                        dst[c] = 0;
                    }
                    func_003EC120(font, D_00516DC8, self->unk3C, j, tmp.p, 1, 0);
                } else {
                    item->unk28 = 0;
                    func_003EBE00(font, ((self->unk3C << 16) | 0x300) | j, 0);
                }
            }
            done = true;
            state++;
        }
        break;
    case 2:
        done = true;
        break;
    }
    switch (n) {
    default:
        self->unk4C = state;
        self->unk58 = 0;
        self->unk54 = 0;
        self->unk50 = 0;
        break;
    case 1:
        self->unk50 = state;
        self->unk58 = 0;
        self->unk54 = 0;
        break;
    case 2:
        self->unk54 = state;
        self->unk58 = 0;
        break;
    case 3:
        self->unk58 = state;
        break;
    }
    return done;
}


struct beNDMWStatusC {
    char pad[0x34];
    s8 unk34, unk35, unk36, unk37, unk38;
    char pad39[0x13];
    s32 unk4C, unk50, unk54, unk58;
};

void func_003E0B80(beNDMWStatusC* self) {
    self->unk34 = 0;
    self->unk36 = 0;
    self->unk38 = 0;
    self->unk35 = 0;
    self->unk38 = 0;
    self->unk4C = 0;
    self->unk58 = 0;
    self->unk54 = 0;
    self->unk50 = 0;
}

struct beNDMWStatusCSrc {
    char pad[0x38];
    s32 unk38;
};

struct beNDMWStatusC2 {
    char pad[0x20];
    beNDMWStatusCSrc* unk20;
    s32 unk24;
    s32 pad28;
    s32 unk2C;
    char pad30[0xC];
    s32 unk3C;
};

void func_003E0BB0(beNDMWStatusC2* self) {
    beNDMWStatusCSrc* src = self->unk20;
    func_003E0B80((beNDMWStatusC*)self);
    self->unk3C = 0;
    switch (src->unk38) {
    case 0:
        self->unk2C = 5;
        self->unk24 = 0;
        break;
    case 1:
        self->unk2C = 2;
        self->unk24 = 1;
        break;
    case 2:
        self->unk2C = 9;
        self->unk24 = 2;
        break;
    }
}


/* Memory card state: per card / slot, whether it is there (unk8). */
struct beNDMWStatusCard {
    char pad[8];
    u8 unk8;
};

struct beNDMWStatusCardInfo {
    char pad[0xF];
    u8 unkF;              /* layout: 0 two marks, else five */
    char pad10[0x1C];
    igObjectList* unk2C;
};

struct beNDMWStatusCards {
    char pad[0x34];
    beNDMWStatusCardInfo* unk34;
};

struct beNDMWStatusD {
    char pad[0x14];
    struct bePadManager* unk14;
    struct beNDMWStatusMdlPos* unk18; /* models */
    s32 unk1C;     /* font owner */
    struct beNDMWStatusCards* unk20;  /* memory card state */
    s32 unk24;     /* mode */
    char pad28[4];
    s32 unk2C;     /* number of marks */
    s32 unk30;     /* ram number */
    s8 unk34;
    u8 unk35;      /* done */
    u8 unk36;      /* cancelled */
    u8 unk37;
    u8 unk38;      /* closed */
    char pad39[3];
    s32 unk3C;     /* cursor */
    s32 unk40;     /* chosen card */
    s32 unk44;     /* layout: 0 two marks, else five */
    s32 unk48;     /* text shown */

    s32 unk4C, unk50, unk54, unk58;
};

extern s32 D_004CD060[];
extern s32 D_004CD090[];
extern char D_00517160[];
void func_003E4030(s32, s32, char*, const char*, s32);

extern char D_00517118[]; /* "j_slotbase" */
extern char D_00517150[]; /* "j_memoryiconwin" */
extern char D_00517170[]; /* "j_slotmark02" */
extern char D_005171A0[]; /* "j_slotmark01" */
struct beNDMWStatusM;
bool func_003E4480(beNDMWStatusM* self, s32 ramNo, const char* info); /* model exists */

#ifdef NON_MATCHING
/* Nonmatching: the switch on unk50 reuses the 1 left in v0 by the unk38 test (move t0,v0) where the original loads it into t0 again; 99% structure. */
/* Close the memory slot window (base, cursor, icon window, texts and the slot
   marks: two, or five in the other layout), then wait for the models. */
u8 func_003E2B10(beNDMWStatusD* self) {
    if (self->unk4C != 2) {
        return 0;
    }
    if (self->unk38 == 1) {
        return 1;
    }
    switch (self->unk24) {
    case 1:
    case 2:
        break;
    default:
        switch (self->unk50) {
        case 0:
            if (self->unk44 == 0) {
                s32 font;
                s32 i;

                func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30, D_00517118, D_00516D30, 1);
                func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30, D_00517160, D_00516D30, 1);
                func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30, D_00517150, D_00516D30, 1);
                font = self->unk1C;
                func_003EC050(font, (self->unk30 << 16) | 0x100C);
                func_003EC050(font, (self->unk30 << 16) | 0x1100);
                func_003EC050(font, (self->unk30 << 16) | 0x102E);
                for (i = 0; i < 2; i++) {
                    switch (i) {
                    case 0:
                        if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 2, D_00517170) == 1) {
                            func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30 * 2, D_00517170, D_00516D30, 0);
                        }
                        break;
                    case 1:
                        if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 2 + 1, D_00517170) == 1) {
                            func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30 * 2 + 1, D_00517170, D_00516D30, 0);
                        }
                        break;
                    }
                }
            } else {
                s32 font;
                s32 i;

                func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30, D_00517118, D_00516D30, 1);
                func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30, D_00517160, D_00516D30, 1);
                func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30, D_00517150, D_00516D30, 1);
                font = self->unk1C;
                func_003EC050(font, (self->unk30 << 16) | 0x100C);
                func_003EC050(font, (self->unk30 << 16) | 0x1100);
                func_003EC050(font, (self->unk30 << 16) | 0x102E);
                for (i = 0; i < 5; i++) {
                    switch (i) {
                    case 0:
                        if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 5, D_005171A0) == 1) {
                            func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30 * 5, D_005171A0, D_00516D30, 0);
                        }
                        break;
                    case 1:
                        if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 5 + 1, D_005171A0) == 1) {
                            func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30 * 5 + 1, D_005171A0, D_00516D30, 0);
                        }
                        break;
                    case 2:
                        if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 5 + 2, D_005171A0) == 1) {
                            func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30 * 5 + 2, D_005171A0, D_00516D30, 0);
                        }
                        break;
                    case 3:
                        if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 5 + 3, D_005171A0) == 1) {
                            func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30 * 5 + 3, D_005171A0, D_00516D30, 0);
                        }
                        break;
                    case 4:
                        if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 2, D_00517170) == 1) {
                            func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30 * 2, D_00517170, D_00516D30, 0);
                        }
                        break;
                    }
                }
            }
            self->unk50++;
            self->unk58 = 0;
            self->unk54 = 0;
            break;
        case 1:
            if (func_003E44D0((beNDMWStatusCnt2*)self->unk18) == true) {
                self->unk38 = 1;
                self->unk50++;
                self->unk58 = 0;
                self->unk54 = 0;
            }
            break;
        }
        break;
    }
    return self->unk38;
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWStatus", func_003E2B10);
#endif

void func_003E2A80(beNDMWStatusD* self, s32 a1, s32 a2, s32* tbl) {
    self->unk4C = 1;
    self->unk58 = 0;
    self->unk54 = 0;
    self->unk50 = 0;
    self->unk35 = 0;
    switch (self->unk24) {
    case 1:
    case 2:
        break;
    default:
        if (self->unk44 == 0) {
            tbl = D_004CD090;
        } else {
            tbl = D_004CD060;
        }
        break;
    }
    func_003E4030((s32)self->unk18, self->unk30, D_00517160, (const char*)tbl[self->unk3C * 2], 0);
}

#ifdef NON_MATCHING
/* Nonmatching: the two-mark layout matches; in the five-mark one the original spills tbl and keeps one more temporary address in a register (frame 304 vs 288); 98.9% structure. */
/* Show the slot marks of the memory slot window for the cards present (except
   slot skip), and the text for the chosen one. */
void func_003E3070(beNDMWStatusD* self, s32 skip) {
    beNDMWStatusCards* cards = self->unk20;

    switch (self->unk24) {
    case 1:
    case 2:
        break;
    default:
        if (self->unk44 == 0) {
            s32* tbl = D_004CD090;
            s32 i;
            s32 font;

            for (i = 0; i < 2; i++) {
                u8 has = ((beNDMWStatusCard**)cards->unk34->unk2C->_data)[tbl[i * 2 + 1]]->unk8;

                if (skip == i) {
                    has = 0;
                }
                if (has == 1) {
                    switch (i) {
                        case 0:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 2, D_00517170) == false) {
                                func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30 * 2, D_00517170, "SQ_OPEN01", self->unk18->pos, 0);
                            } else {
                                func_003E4030((s32)self->unk18, self->unk30 * 2, D_00517170, "SQ_OPEN01", 0);
                            }
                            break;
                        case 1:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 2 + 1, D_00517170) == false) {
                                func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30 * 2 + 1, D_00517170, "SQ_OPEN02", self->unk18->pos, 0);
                            } else {
                                func_003E4030((s32)self->unk18, self->unk30 * 2 + 1, D_00517170, "SQ_OPEN02", 0);
                            }
                            break;
                    }
                } else {
                    switch (i) {
                        case 0:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 2, D_00517170) == 1) {
                                func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30 * 2, D_00517170, D_00516D30, 0);
                            }
                            break;
                        case 1:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 2 + 1, D_00517170) == 1) {
                                func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30 * 2 + 1, D_00517170, D_00516D30, 0);
                            }
                            break;
                    }
                }
            }
            font = self->unk1C;
            if (self->unk48 == 0) {
                if (((beNDMWStatusCard**)cards->unk34->unk2C->_data)[tbl[self->unk3C * 2 + 1]]->unk8 == 1) {
                    self->unk48 = 1;
                    func_003EC050(font, (self->unk30 << 16) | 0x102E);
                }
            } else {
                if (((beNDMWStatusCard**)cards->unk34->unk2C->_data)[tbl[self->unk3C * 2 + 1]]->unk8 != 1) {
                    self->unk48 = 0;
                    func_003EBE00(font, (self->unk30 << 16) | 0x102E, 0);
                }
            }
        } else {
            s32 i;
            s32 font;
            s32* tbl = D_004CD060;

            for (i = 0; i < 5; i++) {
                u8 has = ((beNDMWStatusCard**)cards->unk34->unk2C->_data)[tbl[i * 2 + 1]]->unk8;

                if (skip == i) {
                    has = 0;
                }
                if (has == 1) {
                    switch (i) {
                        case 0:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 5, D_005171A0) == false) {
                                func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30 * 5, D_005171A0, "SQ_OPEN1A", self->unk18->pos, 0);
                            } else {
                                func_003E4030((s32)self->unk18, self->unk30 * 5, D_005171A0, "SQ_OPEN1A", 0);
                            }
                            break;
                        case 1:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 5 + 1, D_005171A0) == false) {
                                func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30 * 5 + 1, D_005171A0, "SQ_OPEN1B", self->unk18->pos, 0);
                            } else {
                                func_003E4030((s32)self->unk18, self->unk30 * 5 + 1, D_005171A0, "SQ_OPEN1B", 0);
                            }
                            break;
                        case 2:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 5 + 2, D_005171A0) == false) {
                                func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30 * 5 + 2, D_005171A0, "SQ_OPEN1C", self->unk18->pos, 0);
                            } else {
                                func_003E4030((s32)self->unk18, self->unk30 * 5 + 2, D_005171A0, "SQ_OPEN1C", 0);
                            }
                            break;
                        case 3:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 5 + 3, D_005171A0) == false) {
                                func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30 * 5 + 3, D_005171A0, "SQ_OPEN1D", self->unk18->pos, 0);
                            } else {
                                func_003E4030((s32)self->unk18, self->unk30 * 5 + 3, D_005171A0, "SQ_OPEN1D", 0);
                            }
                            break;
                        case 4:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 2, D_00517170) == false) {
                                func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30 * 2, D_00517170, "SQ_OPEN02", self->unk18->pos, 0);
                            } else {
                                func_003E4030((s32)self->unk18, self->unk30 * 2, D_00517170, "SQ_OPEN02", 0);
                            }
                            break;
                    }
                } else {
                    switch (i) {
                        case 0:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 5, D_005171A0) == 1) {
                                func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30 * 5, D_005171A0, D_00516D30, 0);
                            }
                            break;
                        case 1:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 5 + 1, D_005171A0) == 1) {
                                func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30 * 5 + 1, D_005171A0, D_00516D30, 0);
                            }
                            break;
                        case 2:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 5 + 2, D_005171A0) == 1) {
                                func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30 * 5 + 2, D_005171A0, D_00516D30, 0);
                            }
                            break;
                        case 3:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 5 + 3, D_005171A0) == 1) {
                                func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30 * 5 + 3, D_005171A0, D_00516D30, 0);
                            }
                            break;
                        case 4:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 2, D_00517170) == 1) {
                                func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30 * 2, D_00517170, D_00516D30, 0);
                            }
                            break;
                    }
                }
            }
            font = self->unk1C;
            if (self->unk48 == 0) {
                if (((beNDMWStatusCard**)cards->unk34->unk2C->_data)[tbl[self->unk3C * 2 + 1]]->unk8 == 1) {
                    self->unk48 = 1;
                    func_003EC050(font, (self->unk30 << 16) | 0x102E);
                }
            } else {
                if (((beNDMWStatusCard**)cards->unk34->unk2C->_data)[tbl[self->unk3C * 2 + 1]]->unk8 != 1) {
                    self->unk48 = 0;
                    func_003EBE00(font, (self->unk30 << 16) | 0x102E, 0);
                }
            }
        }
        break;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWStatus", func_003E3070);
#endif

#ifdef NON_MATCHING
/* Nonmatching: only the 2 and 1 of the unk24 test are reused for the switch on unk50 (the original loads them again, as in func_003E2B10); 99.7% structure. */
/* Open the memory slot window: base (two or five marks by the card layout), icon
   window, then the cursor, the marks of the cards present and the texts. */
u8 func_003E0C40(beNDMWStatusD* self, s32 cur) {
    beNDMWStatusCards* cards;

    if (self->unk4C != 0) {
        return 0;
    }
    cards = self->unk20;
    switch (self->unk24) {
    case 1:
    case 2:
        break;
    default:
        switch (self->unk50) {
        case 0:
            if (cur != -1) {
                self->unk3C = cur;
            }
            if (cards->unk34->unkF == 0) {
                func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30, D_00517118, "SQ_OPENPS01", self->unk18->pos, 1);
                self->unk44 = 0;
            } else {
                func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30, D_00517118, "SQ_OPENPS02", self->unk18->pos, 1);
                self->unk44 = 1;
            }
            func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30, D_00517150, "SQ_OPEN", self->unk18->pos, 1);
            self->unk37 = 0;
            self->unk50++;
            self->unk58 = 0;
            self->unk54 = 0;
            break;
        case 1:
            if (func_003E4280((beNDMWStatusCnt*)self->unk18) == true) {
                if (self->unk44 == 0) {
                    s32* tbl = D_004CD090;
                    s32 i;
                    s32 font;

                    func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30, D_00517160, (const char*)tbl[self->unk3C * 2], self->unk18->pos, 1);
                    for (i = 0; i < 2; i++) {
                        if (((beNDMWStatusCard**)cards->unk34->unk2C->_data)[tbl[i * 2 + 1]]->unk8 == 1) {
                            switch (i) {
                                case 0:
                                    if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 2, D_00517170) == false) {
                                        func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30 * 2, D_00517170, "SQ_OPEN01", self->unk18->pos, 0);
                                    } else {
                                        func_003E4030((s32)self->unk18, self->unk30 * 2, D_00517170, "SQ_OPEN01", 0);
                                    }
                                    break;
                                case 1:
                                    if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 2 + 1, D_00517170) == false) {
                                        func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30 * 2 + 1, D_00517170, "SQ_OPEN02", self->unk18->pos, 0);
                                    } else {
                                        func_003E4030((s32)self->unk18, self->unk30 * 2 + 1, D_00517170, "SQ_OPEN02", 0);
                                    }
                                    break;
                            }
                        }
                    }
                    font = self->unk1C;
                    func_003EBE00(font, (self->unk30 << 16) | 0x100C, 0);
                    switch (self->unk3C) {
                        case 0:
                            func_003EBE00(font, (self->unk30 << 16) | 0x1100, 0);
                            break;
                        case 1:
                            func_003EBE00(font, (self->unk30 << 16) | 0x1101, 0);
                            break;
                    }
                    if (((beNDMWStatusCard**)cards->unk34->unk2C->_data)[tbl[self->unk3C * 2 + 1]]->unk8 != 1) {
                        self->unk48 = 0;
                        func_003EBE00(font, (self->unk30 << 16) | 0x102E, 0);
                    } else {
                        self->unk48 = 1;
                        func_003EC050(font, (self->unk30 << 16) | 0x102E);
                    }
                    self->unk50++;
                    self->unk58 = 0;
                    self->unk54 = 0;
                } else {
                    s32* tbl = D_004CD060;
                    s32 i;
                    s32 font;

                    func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30, D_00517160, (const char*)tbl[self->unk3C * 2], self->unk18->pos, 1);
                    for (i = 0; i < 5; i++) {
                        if (((beNDMWStatusCard**)cards->unk34->unk2C->_data)[tbl[i * 2 + 1]]->unk8 == 1) {
                            switch (i) {
                                case 0:
                                    if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 5, D_005171A0) == false) {
                                        func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30 * 5, D_005171A0, "SQ_OPEN1A", self->unk18->pos, 0);
                                    } else {
                                        func_003E4030((s32)self->unk18, self->unk30 * 5, D_005171A0, "SQ_OPEN1A", 0);
                                    }
                                    break;
                                case 1:
                                    if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 5 + 1, D_005171A0) == false) {
                                        func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30 * 5 + 1, D_005171A0, "SQ_OPEN1B", self->unk18->pos, 0);
                                    } else {
                                        func_003E4030((s32)self->unk18, self->unk30 * 5 + 1, D_005171A0, "SQ_OPEN1B", 0);
                                    }
                                    break;
                                case 2:
                                    if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 5 + 2, D_005171A0) == false) {
                                        func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30 * 5 + 2, D_005171A0, "SQ_OPEN1C", self->unk18->pos, 0);
                                    } else {
                                        func_003E4030((s32)self->unk18, self->unk30 * 5 + 2, D_005171A0, "SQ_OPEN1C", 0);
                                    }
                                    break;
                                case 3:
                                    if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 5 + 3, D_005171A0) == false) {
                                        func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30 * 5 + 3, D_005171A0, "SQ_OPEN1D", self->unk18->pos, 0);
                                    } else {
                                        func_003E4030((s32)self->unk18, self->unk30 * 5 + 3, D_005171A0, "SQ_OPEN1D", 0);
                                    }
                                    break;
                                case 4:
                                    if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 2, D_00517170) == false) {
                                        func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30 * 2, D_00517170, "SQ_OPEN02", self->unk18->pos, 0);
                                    } else {
                                        func_003E4030((s32)self->unk18, self->unk30 * 2, D_00517170, "SQ_OPEN02", 0);
                                    }
                                    break;
                            }
                        }
                    }
                    font = self->unk1C;
                    func_003EBE00(font, (self->unk30 << 16) | 0x100C, 0);
                    switch (self->unk3C) {
                        case 0:
                            func_003EBE00(font, (self->unk30 << 16) | 0x1102, 0);
                            break;
                        case 1:
                            func_003EBE00(font, (self->unk30 << 16) | 0x1103, 0);
                            break;
                        case 2:
                            func_003EBE00(font, (self->unk30 << 16) | 0x1104, 0);
                            break;
                        case 3:
                            func_003EBE00(font, (self->unk30 << 16) | 0x1105, 0);
                            break;
                        case 4:
                            func_003EBE00(font, (self->unk30 << 16) | 0x1101, 0);
                            break;
                    }
                    if (((beNDMWStatusCard**)cards->unk34->unk2C->_data)[tbl[self->unk3C * 2 + 1]]->unk8 != 1) {
                        self->unk48 = 0;
                        func_003EBE00(font, (self->unk30 << 16) | 0x102E, 0);
                    } else {
                        self->unk48 = 1;
                        func_003EC050(font, (self->unk30 << 16) | 0x102E);
                    }
                    self->unk50++;
                    self->unk58 = 0;
                    self->unk54 = 0;
                }
            }
            break;
        case 2:
            if (func_003E4280((beNDMWStatusCnt*)self->unk18) == true) {
                self->unk34 = 1;
                self->unk4C++;
                self->unk58 = 0;
                self->unk54 = 0;
                self->unk50 = 0;
            }
            break;
        }
        break;
    }
    return self->unk34;
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWStatus", func_003E0C40);
#endif



extern char D_005171F8[];
extern char D_00517208[];
extern char D_00517218[];
extern char D_00517228[];
void beMessenger_select(void* m, const char* group, s32 info, s32 ram);
void beMessenger_send(void* m, const char* message);
void beMessenger_call(void* m, const char* group, const char* info, s32 ram, const char* message);
void beMessenger_pushString(void* m, const char* s);
bool beMessenger_getBool(void* m, s32 i);

struct beNDMWStatusStrPool;
extern beNDMWStatusStrPool* D_00586F20;
extern bool D_00554C14;
extern void* D_00587110;
void* func_002EB010(u32 size);
beNDMWStatusStrPool* func_002EADF0(beNDMWStatusStrPool* pool);
char* func_002EAF60(beNDMWStatusStrPool* pool, const char* s);
void func_0030C940(void* owner, void* h);
void* func_00301000(void* o);
void* func_002F88E0(void* p);
igObject* func_003C7450(void* pool);
void func_00301E30(igObjectList* list, igObject* o);

struct beNDMWStatusStrHdr {
    void* owner;
    s32 refs;
};

static inline beNDMWStatusStrPool* beNDMWStatus_GetPool(void) {
    if (D_00586F20 == NULL) {
        beNDMWStatusStrPool* pool = (beNDMWStatusStrPool*)func_002EB010(0x10);
        if (pool != NULL) {
            pool = func_002EADF0(pool);
        }
        D_00586F20 = pool;
    }
    return D_00586F20;
}

static inline void beNDMWStatus_SetInfo(beLoadIntf2ComMdlData* o, const char* s) {
    char* str = func_002EAF60(beNDMWStatus_GetPool(), s);

    if (o->_infoName != NULL) {
        beNDMWStatusStrHdr* h = (beNDMWStatusStrHdr*)o->_infoName - 1;
        if (--h->refs == 0) {
            func_0030C940(h->owner, h);
        }
    }
    o->_infoName = str;
}

static inline void beNDMWStatus_SetOpen(beLoadIntf2ComMdlData* o, const char* s) {
    char* str = func_002EAF60(beNDMWStatus_GetPool(), s);

    if (o->_openSQ != NULL) {
        beNDMWStatusStrHdr* h = (beNDMWStatusStrHdr*)o->_openSQ - 1;
        if (--h->refs == 0) {
            func_0030C940(h->owner, h);
        }
    }
    o->_openSQ = str;
}

struct beNDMWStatusMdl {
    char pad[0x10];
    void* unk10;
    igObjectList* unk14;
};

struct beNDMWStatusTmp {
    igObject* p;
    beNDMWStatusTmp(igObject* o) : p(o) {}
    ~beNDMWStatusTmp() {
        igObject* o = p;
        if (o != NULL) {
            o->_refCount--;
            if ((o->_refCount & 0x7FFFFF) == 0) {
                func_002FF820(o);
            }
        }
    }
};

#ifdef NON_MATCHING
/* Nonmatching: s-registers of ramNo/info swapped (distance 14) */
void func_003E3B80(beNDMWStatusMdl* m, s32 ramNo, s32 info, s32 msg, s32 flag) {
    beMessenger_call(m->unk10, D_005171F8, (const char*)info, ramNo, D_00517208);
    beMessenger_call(m->unk10, D_005171F8, (const char*)info, ramNo, (const char*)msg);
    if ((u8)flag == 1) {
        void* a = func_00301000(m);
        beNDMWStatusTmp tmp(func_003C7450(D_00554C14 ? a : func_002F88E0(D_00587110)));
        ((beLoadIntf2ComMdlData*)tmp.p)->_ramNo = ramNo;
        beNDMWStatus_SetInfo((beLoadIntf2ComMdlData*)tmp.p, (const char*)info);
        beNDMWStatus_SetOpen((beLoadIntf2ComMdlData*)tmp.p, (const char*)msg);
        func_00301E30(m->unk14, tmp.p);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWStatus", func_003E3B80);
#endif

void* func_003E3D80(beNDMWStatusRef** self, s16 flag) {
    if (self != NULL) {
        beNDMWStatusRef* r = *self;
        if (r != NULL) {
            r->_refCount--;
            if ((r->_refCount & 0x7FFFFF) == 0) {
                func_002FF820(r);
            }
        }
        if (flag > 0) {
            delete (char*)self;
        }
    }
    return self;
}

#ifdef NON_MATCHING
/* Nonmatching: the new record reloads into s3 instead of s0 (4 off), as in func_003E3B80. */
/* Generate a model at pos (MODELCTRL GENERATE, then the open sequence msg); with
   flag, keep a record of it (ramNo, info and msg names) in m's list. */
void func_003E3E00(beNDMWStatusMdl* m, s32 ramNo, const char* info, const char* msg, beNDMWStatusVec3 pos, s32 flag) {
    beMessenger_select(m->unk10, D_005171F8, (s32)info, ramNo);
    beMessenger_pushVec3f(m->unk10, pos);
    beMessenger_send(m->unk10, D_00517208);
    beMessenger_call(m->unk10, D_005171F8, (const char*)info, ramNo, msg);
    if ((u8)flag == 1) {
        void* a = func_00301000(m);
        beNDMWStatusTmp tmp(func_003C7450(D_00554C14 ? a : func_002F88E0(D_00587110)));
        ((beLoadIntf2ComMdlData*)tmp.p)->_ramNo = ramNo;
        beNDMWStatus_SetInfo((beLoadIntf2ComMdlData*)tmp.p, info);
        beNDMWStatus_SetOpen((beLoadIntf2ComMdlData*)tmp.p, msg);
        func_00301E30(m->unk14, tmp.p);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWStatus", func_003E3E00);
#endif

struct beNDMWStatusM {
    char pad[0x10];
    void* unk10;
};

#ifdef NON_MATCHING
/* Nonmatching: s-registers of ramNo/info swapped (distance 10) */
void func_003E4030(beNDMWStatusMdl* m, s32 ramNo, s32 info, s32 msg, s32 flag) {

    beMessenger_call(m->unk10, D_005171F8, (const char*)info, ramNo, (const char*)msg);
    if ((u8)flag == 1) {
        void* a = func_00301000(m);
        beNDMWStatusTmp tmp(func_003C7450(D_00554C14 ? a : func_002F88E0(D_00587110)));
        ((beLoadIntf2ComMdlData*)tmp.p)->_ramNo = ramNo;
        beNDMWStatus_SetInfo((beLoadIntf2ComMdlData*)tmp.p, (const char*)info);
        beNDMWStatus_SetOpen((beLoadIntf2ComMdlData*)tmp.p, (const char*)msg);
        func_00301E30(m->unk14, tmp.p);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWStatus", func_003E4030);
#endif

bool func_003E4210(beNDMWStatusM* self, s32 a1, s32 a2, s32 a3) {
    beMessenger_select(self->unk10, D_005171F8, a2, a1);
    beMessenger_pushString(self->unk10, (const char*)a3);
    beMessenger_send(self->unk10, D_00517218);
    return beMessenger_getBool(self->unk10, 0);
}

bool func_003E4280(beNDMWStatusCnt* self) {
    return self->unk14->unk8 <= 0;
}

#ifdef NON_MATCHING
/* Nonmatching: s-registers of ramNo/info swapped (distance 10) */
struct beNDMWStatusMdl2 {
    char pad[0x10];
    void* unk10;
    s32 pad14;
    igObjectList* unk18;
};

void func_003E42A0(beNDMWStatusMdl2* m, s32 ramNo, s32 info, s32 msg, s32 flag) {
    beMessenger_call(m->unk10, D_005171F8, (const char*)info, ramNo, (const char*)msg);
    if ((u8)flag == 1) {
        void* a = func_00301000(m);
        beNDMWStatusTmp tmp(func_003C7450(D_00554C14 ? a : func_002F88E0(D_00587110)));
        ((beLoadIntf2ComMdlData*)tmp.p)->_ramNo = ramNo;
        beNDMWStatus_SetInfo((beLoadIntf2ComMdlData*)tmp.p, (const char*)info);
        beNDMWStatus_SetOpen((beLoadIntf2ComMdlData*)tmp.p, (const char*)msg);
        func_00301E30(m->unk18, tmp.p);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWStatus", func_003E42A0);
#endif

bool func_003E4480(beNDMWStatusM* self, s32 a1, const char* a2) {
    beMessenger_call(self->unk10, D_005171F8, a2, a1, D_00517228);
    return beMessenger_getBool(self->unk10, 0);
}

bool func_003E44D0(beNDMWStatusCnt2* self) {
    return self->unk18->unk8 <= 0;
}

struct beNDMWStatusItem {
    s32 pad0;
    s32 _refCount;
    s32 unk8;
    s32 unkC;
    s32 unk10;
};

struct beNDMWStatusLst {
    char pad[8];
    s32 _count;
    s32 pad0C;
    beNDMWStatusItem** _data;
};

struct beNDMWStatusW {
    char pad[0x10];
    void* unk10;
    beNDMWStatusLst* unk14;
    beNDMWStatusLst* unk18;
};

void func_002D6CF0(void* list, s32 index);

#ifdef NON_MATCHING
/* Nonmatching: s-registers permuted (i/offset/list get s0/s1/s2 in reverse order) */
inline void beNDMWStatus_removeAt(s32 i, beNDMWStatusLst* l) {
    beNDMWStatusItem* r = l->_data[i];
    if (r != NULL) {
        r->_refCount--;
        if ((r->_refCount & 0x7FFFFF) == 0) {
            func_002FF820(r);
        }
    }
    func_002D6CF0(l, i);
    l->_data[l->_count] = NULL;
}

void func_003E44F0(beNDMWStatusW* self) {
    beNDMWStatusItem* e;
    s32 i;
    s32 n = self->unk14->_count;
    if (n > 0) {
        for (i = n - 1; i >= 0; i--) {
            e = self->unk14->_data[i];
            if (func_003E4210((beNDMWStatusM*)self, e->unk8, e->unk10, e->unkC) == 1) {
                break;
            }
            beNDMWStatus_removeAt(i, self->unk14);
        }
    }
    n = self->unk18->_count;
    if (n > 0) {
        for (i = n - 1; i >= 0; i--) {
            e = self->unk18->_data[i];
            if (func_003E4480((beNDMWStatusM*)self, e->unk8, (const char*)e->unk10) == 1) {
                break;
            }
            beNDMWStatus_removeAt(i, self->unk18);
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWStatus", func_003E44F0);
#endif

static inline void beNDMWStatus_Unref(igObject* o) {
    if (o != NULL) {
        o->_refCount--;
        if ((o->_refCount & 0x7FFFFF) == 0) {
            func_002FF820(o);
        }
    }
}

/* A window held for a scope. */
struct beNDMWStatusWinRef {
    beNDMWWindow* p;

    beNDMWStatusWinRef() : p(NULL) {}
    ~beNDMWStatusWinRef() { beNDMWStatus_Unref(p); }
};

void func_003D8740(beNDMWStatus* self, beNDMWStatusInfoRam* ram);
void* func_00301000(void* o);
void* func_002F88E0(void* p);
beNDMWWindow* func_003BB1D0(void* pool);
beNDMWWindow* func_003B8D90(void* pool);
beNDMWWindow* func_003B7750(void* pool);
beNDMWWindow* func_003B7EB0(void* pool);
void beNDMWWindow_setup(beNDMWWindow* w, beNDMWWindow* mother, igInsightCore* insight, s32 playerNo, s32 entCode);
void beNDMWWindow_closeAll(beNDMWWindow* w);
void beMessenger_selectGroup(void* m, const char* group);
void beMessenger_callGroup(void* m, const char* group, const char* message);
void beMessenger_pushString(void* m, const char* s);
void beMessenger_pushInt(void* m, s32 v);
void beMessenger_send(void* m, const char* message);
void beMessenger_setFreeze(void* m, bool freeze);
bool beMessenger_getBool(void* m, s32 i);
igObject* beMessenger_getObject(void* m, s32 i);
beDataObjBool* beDataObjList_getBool(beDataObjList* list, const char* name);
void* func_00236F80(igInsightCore* insight, igMetaObject* meta);
void func_00389FF0(beLua* lua, const char* name, const char* dir);
extern igMetaObject* beLua_Meta;
extern bool D_00554C14;
extern void* D_00587110;

/* Sales lists of the shops, by the first player's LEVEL (and, for the weapons,
   by digimon, or 150 with several players); the junk shop by the COUNTER total. */
extern void* D_00587108;
extern char D_00516980[]; /* "DATABASE" */
extern char D_00516998[]; /* "GETDATABASE" */
beNDMWStatusInfoWork* func_003B5EC0(void* pool);
beNDMWItemBase* func_003C8C90(void* pool); /* new weapon */
beNDMWItemBase* func_003C8B00(void* pool); /* new armor */
beNDMWItemBase* func_003C8970(void* pool); /* new chip */
beNDMWItemBase* func_003C87F0(void* pool);
void func_004482E0(igObject* o, s32 id, beNDMWStatus* st, s32 level);
void func_00448430(igObject* o, s32 id, beNDMWStatus* st, s32 level);
void func_00448580(igObject* o, s32 id, beNDMWStatus* st, s32 level);
void func_00448660(igObject* o, s32 id);
void func_002D6680(void* list, s32 n, s32 size);
void func_00301C90(igObjectList* list, igObject* o);
beDataObjInt* beDataObjList_getInt(beDataObjList* list, const char* name);
beDataObjIntList* beDataObjList_getIntList(beDataObjList* list, const char* name);
s32 beDataBase_getInt(beDataBase* db, s32 record, const char* field);
extern "C" char* strcat(char* d, const char* s);

void func_003D8740(beNDMWStatus* self, beNDMWStatusInfoRam* ram) {
    beNDMWStatusInfoWork* work = ram->_work;
    s32 players;
    s32 i;
    beDataObjList* pl;
    s32 level;

    if (work == NULL) {
        void* a = func_00301000(self);
        beNDMWStatusTmp w((igObject*)func_003B5EC0(D_00554C14 ? a : func_002F88E0(D_00587110)));
        igObject* n = w.p;

        if (n != NULL) {
            n->_refCount++;
        }
        beNDMWStatus_Unref((igObject*)ram->_work);
        ram->_work = (beNDMWStatusInfoWork*)n;
        work = ram->_work;
    }
    players = 0;
    for (i = 0; i < 4; i++) {
        beMessenger_selectGroup(self->_messenger, "GAMERAM");
        beMessenger_pushInt(self->_messenger, i);
        beMessenger_send(self->_messenger, "GETPLAYER");
        pl = (beDataObjList*)beMessenger_getObject(self->_messenger, 0);
        if (beDataObjList_getBool(pl, "ISUSE")->_data) {
            players++;
            if (beDataObjList_getInt(pl, "BIT")->_data > 9999999) {
                beDataObjInt* o = beDataObjList_getInt(pl, "BIT");

                o->_data = 9999999;
            }
            if (beDataObjList_getInt(pl, "XDATA")->_data > 9999) {
                beDataObjInt* o = beDataObjList_getInt(pl, "XDATA");

                o->_data = 9999;
            }
        }
    }
    beMessenger_selectGroup(self->_messenger, "GAMERAM");
    beMessenger_pushInt(self->_messenger, 0);
    beMessenger_send(self->_messenger, "GETPLAYER");
    pl = (beDataObjList*)beMessenger_getObject(self->_messenger, 0);
    level = beDataObjList_getInt(pl, "LEVEL")->_data;
    switch (self->_subWndNo) {
    case 6:
        if (work->_weaponSalesList->_count <= 0) {
            s32 base;
            s32 rank;
            beDataBase* db;
            s32 j;
            char col[8];

            if (((volatile igDataList*)work->_weaponSalesList)->_count < 37) {
                func_002D6680(work->_weaponSalesList, 36, 4);
            }
            func_00301C90(work->_weaponSalesList, NULL);
            if (players > 1) {
                base = 150;
            } else {
                switch (beDataObjList_getInt(pl, "DIGIMONNO")->_data) {
                default:
                case 15:
                    base = 0;
                    break;
                case 1:
                case 4:
                case 12:
                    base = 25;
                    break;
                case 2:
                case 7:
                case 14:
                    base = 50;
                    break;
                case 3:
                case 9:
                    base = 75;
                    break;
                case 8:
                case 11:
                case 13:
                    base = 125;
                    break;
                case 6:
                case 10:
                    base = 100;
                    break;
                }
            }
            if (level < 10) {
                rank = 0;
            } else if (level < 17) {
                rank = 1;
            } else if (level < 22) {
                rank = 2;
            } else if (level < 27) {
                rank = 3;
            } else if (level < 32) {
                rank = 4;
            } else if (level < 37) {
                rank = 5;
            } else if (level < 42) {
                rank = 6;
            } else if (level < 47) {
                rank = 7;
            } else if (level < 52) {
                rank = 8;
            } else if (level < 61) {
                rank = 9;
            } else if (level < 68) {
                rank = 10;
            } else if (level < 73) {
                rank = 11;
            } else if (level < 78) {
                rank = 12;
            } else if (level < 83) {
                rank = 13;
            } else if (level < 88) {
                rank = 14;
            } else if (level < 93) {
                rank = 15;
            } else if (level < 98) {
                rank = 16;
            } else if (level < 103) {
                rank = 17;
            } else if (level < 108) {
                rank = 18;
            } else if (level < 113) {
                rank = 19;
            } else if (level < 118) {
                rank = 20;
            } else if (level < 128) {
                rank = 21;
            } else if (level < 138) {
                rank = 22;
            } else if (level < 148) {
                rank = 23;
            } else {
                rank = 24;
            }
            beMessenger_call(self->_messenger, D_00516980, "shopWeapon", 0, D_00516998);
            db = (beDataBase*)beMessenger_getObject(self->_messenger, 0);
            for (j = 0; j < 36; j++) {
                s32 id;

                col[0] = j / 10 + '0';
                col[1] = j % 10 + '0';
                col[2] = 0;
                id = beDataBase_getInt(db, base + rank, col);
                if (id != -1) {
                    {
                        void* a = func_00301000(self);
                        beNDMWStatusTmp it(func_003C8C90(D_00554C14 ? a : func_002F88E0(D_00587108)));

                        func_004482E0(it.p, id, self, level);
                        func_00301E30(work->_weaponSalesList, it.p);
                    }
                } else {
                    func_00301E30(work->_weaponSalesList, NULL);
                }
            }
        }
        break;
    case 7:
        if (work->_armorSalesList->_count <= 0) {
            s32 rank;
            beDataBase* db;
            s32 j;
            s32 k;
            char col[8];

            if (((volatile igDataList*)work->_armorSalesList)->_count < 37) {
                func_002D6680(work->_armorSalesList, 36, 4);
            }
            func_00301C90(work->_armorSalesList, NULL);
            if (level < 10) {
                rank = 0;
            } else if (level < 17) {
                rank = 1;
            } else if (level < 22) {
                rank = 2;
            } else if (level < 27) {
                rank = 3;
            } else if (level < 32) {
                rank = 4;
            } else if (level < 37) {
                rank = 5;
            } else if (level < 42) {
                rank = 6;
            } else if (level < 47) {
                rank = 7;
            } else if (level < 52) {
                rank = 8;
            } else if (level < 61) {
                rank = 9;
            } else if (level < 68) {
                rank = 10;
            } else if (level < 73) {
                rank = 11;
            } else if (level < 78) {
                rank = 12;
            } else if (level < 83) {
                rank = 13;
            } else if (level < 88) {
                rank = 14;
            } else if (level < 93) {
                rank = 15;
            } else if (level < 98) {
                rank = 16;
            } else if (level < 103) {
                rank = 17;
            } else if (level < 108) {
                rank = 18;
            } else if (level < 113) {
                rank = 19;
            } else if (level < 118) {
                rank = 20;
            } else if (level < 128) {
                rank = 21;
            } else if (level < 138) {
                rank = 22;
            } else if (level < 148) {
                rank = 23;
            } else if (level < 158) {
                rank = 24;
            } else if (level < 168) {
                rank = 25;
            } else if (level < 178) {
                rank = 26;
            } else if (level < 188) {
                rank = 27;
            } else if (level < 198) {
                rank = 28;
            } else {
                rank = 29;
            }
            beMessenger_call(self->_messenger, D_00516980, "shopArmor", 0, D_00516998);
            db = (beDataBase*)beMessenger_getObject(self->_messenger, 0);
            for (j = 0; j < 12; j++) {
                s32 id;

                col[0] = j / 10 + '0';
                col[1] = j % 10 + '0';
                col[2] = 0;
                id = beDataBase_getInt(db, rank, col);
                {
                    void* a = func_00301000(self);
                    beNDMWStatusTmp it(func_003C8B00(D_00554C14 ? a : func_002F88E0(D_00587108)));

                    func_00448430(it.p, id, self, level);
                    func_00301E30(work->_armorSalesList, it.p);
                }
            }
            for (k = 12; k < 36; k++) {
                s32 id;

                col[0] = k / 10 + '0';
                col[1] = k % 10 + '0';
                col[2] = 0;
                id = beDataBase_getInt(db, rank, col);
                if (id != -1) {
                    {
                        void* a = func_00301000(self);
                        beNDMWStatusTmp it(func_003C8970(D_00554C14 ? a : func_002F88E0(D_00587108)));

                        func_00448580(it.p, id, self, level);
                        func_00301E30(work->_armorSalesList, it.p);
                    }
                } else {
                    func_00301E30(work->_armorSalesList, NULL);
                }
            }
        }
        break;
    case 8:
        if (work->_chipSalesList->_count <= 0) {
            s32 rank;
            beDataBase* db;
            s32 j;
            char col[8];

            if (((volatile igDataList*)work->_chipSalesList)->_count < 37) {
                func_002D6680(work->_chipSalesList, 36, 4);
            }
            func_00301C90(work->_chipSalesList, NULL);
            if (level < 10) {
                rank = 0;
            } else if (level < 17) {
                rank = 1;
            } else if (level < 22) {
                rank = 2;
            } else if (level < 27) {
                rank = 3;
            } else if (level < 32) {
                rank = 4;
            } else if (level < 37) {
                rank = 5;
            } else if (level < 42) {
                rank = 6;
            } else if (level < 47) {
                rank = 7;
            } else if (level < 52) {
                rank = 8;
            } else if (level < 61) {
                rank = 9;
            } else if (level < 68) {
                rank = 10;
            } else if (level < 73) {
                rank = 11;
            } else if (level < 78) {
                rank = 12;
            } else if (level < 83) {
                rank = 13;
            } else if (level < 88) {
                rank = 14;
            } else if (level < 93) {
                rank = 15;
            } else if (level < 98) {
                rank = 16;
            } else if (level < 103) {
                rank = 17;
            } else if (level < 108) {
                rank = 18;
            } else if (level < 113) {
                rank = 19;
            } else if (level < 118) {
                rank = 20;
            } else if (level < 128) {
                rank = 21;
            } else if (level < 138) {
                rank = 22;
            } else if (level < 148) {
                rank = 23;
            } else if (level < 158) {
                rank = 24;
            } else if (level < 168) {
                rank = 25;
            } else if (level < 178) {
                rank = 26;
            } else if (level < 188) {
                rank = 27;
            } else if (level < 198) {
                rank = 28;
            } else {
                rank = 29;
            }
            beMessenger_call(self->_messenger, D_00516980, "shopChip", 0, D_00516998);
            db = (beDataBase*)beMessenger_getObject(self->_messenger, 0);
            for (j = 0; j < 36; j++) {
                s32 id;

                col[0] = j / 10 + '0';
                col[1] = j % 10 + '0';
                col[2] = 0;
                id = beDataBase_getInt(db, rank, col);
                if (id != -1) {
                    {
                        void* a = func_00301000(self);
                        beNDMWStatusTmp it(func_003C87F0(D_00554C14 ? a : func_002F88E0(D_00587108)));

                        func_00448660(it.p, id);
                        func_00301E30(work->_chipSalesList, it.p);
                    }
                } else {
                    func_00301E30(work->_chipSalesList, NULL);
                }
            }
        }
        break;
    case 9: {
        s32 total;
        s32 rank;

        beMessenger_callGroup(self->_messenger, "GAMERAM", "GETCOMMON");
        total = ((s32*)beDataObjList_getIntList((beDataObjList*)beMessenger_getObject(self->_messenger, 0), "COUNTER")->_data->_data)[1];
        if (total >= 3956000) {
            rank = 8;
        } else if (total >= 1956000) {
            rank = 7;
        } else if (total >= 956000) {
            rank = 6;
        } else if (total >= 456000) {
            rank = 5;
        } else if (total >= 206000) {
            rank = 4;
        } else if (total >= 86000) {
            rank = 3;
        } else if (total >= 26000) {
            rank = 2;
        } else if (total >= 6000) {
            rank = 1;
        } else if (total >= 1000) {
            rank = 0;
        } else {
            rank = -1;
        }
        if (work->_junkSalesList->_count <= 0) {
            work->_junkGift = total;
            if (((volatile igDataList*)work->_junkSalesList)->_count < 37) {
                func_002D6680(work->_junkSalesList, 36, 4);
            }
            func_00301C90(work->_junkSalesList, NULL);
            if (rank != -1) {
                beDataBase* db;
                s32 j;
                char col[8];

                beMessenger_call(self->_messenger, D_00516980, "shopJunk", 0, D_00516998);
                db = (beDataBase*)beMessenger_getObject(self->_messenger, 0);
                for (j = 0; j < 36; j++) {
                    s32 id;

                    col[0] = j / 10 + '0';
                    col[1] = j % 10 + '0';
                    col[2] = 0;
                    id = beDataBase_getInt(db, rank, col);
                    if (id != -1) {
                        strcat(col, "kind");
                        switch (beDataBase_getInt(db, rank, col)) {
                        case 0:
                            {
                                void* a = func_00301000(self);
                                beNDMWStatusTmp it(func_003C8C90(D_00554C14 ? a : func_002F88E0(D_00587108)));

                                func_004482E0(it.p, id, self, level);
                                func_00301E30(work->_junkSalesList, it.p);
                            }
                            break;
                        case 1:
                            {
                                void* a = func_00301000(self);
                                beNDMWStatusTmp it(func_003C8B00(D_00554C14 ? a : func_002F88E0(D_00587108)));

                                func_00448430(it.p, id, self, level);
                                func_00301E30(work->_junkSalesList, it.p);
                            }
                            break;
                        case 2:
                            {
                                void* a = func_00301000(self);
                                beNDMWStatusTmp it(func_003C8970(D_00554C14 ? a : func_002F88E0(D_00587108)));

                                func_00448580(it.p, id, self, level);
                                func_00301E30(work->_junkSalesList, it.p);
                            }
                            break;
                        }
                    }
                }
            }
        }
        break;
    }
    }
}

#define STATUS_NEW_WINDOW(w, create)                                           \
    {                                                                          \
        void* a_ = func_00301000(self);                                        \
        void* pool_ = D_00554C14 ? a_ : func_002F88E0(D_00587110);             \
                                                                               \
        beNDMWStatus_Unref(w.p);                                               \
        w.p = create(pool_);                                                   \
    }

/* Open the status window for _subWndNo (with the shop music for the shops),
   wait for it to close, then give the game back: unfreeze, refill the players'
   HP / MP and leave (shopexit.lua) or go on (stagML12 after window 10). */
void beNDMWStatus_updateRam(beNDMWStatus* self, beBaseInfoRam* ram) {
    s32 i;

    if (ram->_isRun == false) {
        return;
    }
    switch (ram->_st0) {
    case 0:
        func_003D8740(self, (beNDMWStatusInfoRam*)ram);
        beMessenger_selectGroup(self->_messenger, "STAGECTL");
        beMessenger_pushString(self->_messenger, "ENABLE");
        beMessenger_send(self->_messenger, "PADINPUT");
        {
            beNDMWStatusWinRef w;

            switch (self->_subWndNo) {
            case 0:
                beMessenger_selectGroup(self->_messenger, "LAYERCTL");
                beMessenger_pushString(self->_messenger, "WN01");
                beMessenger_pushString(self->_messenger, "DISPOFF");
                beMessenger_send(self->_messenger, "SWITCH");
                beMessenger_selectGroup(self->_messenger, "LAYERCTL");
                beMessenger_pushString(self->_messenger, "WN00");
                beMessenger_pushString(self->_messenger, "DISPOFF");
                beMessenger_send(self->_messenger, "SWITCH");
                STATUS_NEW_WINDOW(w, func_003BB1D0)
                break;
            case 1:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                STATUS_NEW_WINDOW(w, func_003B8D90)
                break;
            case 2:
                STATUS_NEW_WINDOW(w, func_003B7750)
                break;
            case 10:
                STATUS_NEW_WINDOW(w, func_003B7EB0)
                break;
            }
            beNDMWWindow_setup(w.p, NULL, self->_insight, self->_whoGenerate, 0);
            w.p->_entCode = self->_subWndNo;
            {
                beNDMWWindow* n = w.p;

                if (n != NULL) {
                    n->_refCount++;
                }
                beNDMWStatus_Unref(self->_windowPro);
                self->_windowPro = n;
            }
            if (self->_subWndNo != 0) {
                beMessenger_callGroup(self->_messenger, "SOUND", "BGMSTOP");
                beMessenger_callGroup(self->_messenger, "SOUND", "ENVSTOP");
                beMessenger_selectGroup(self->_messenger, "SOUND");
                beMessenger_pushString(self->_messenger, "RM_NSHOP");
                beMessenger_send(self->_messenger, "PLAY");
            }
        }
        ram->_st0++;
        ram->_st3 = 0;
        ram->_st2 = 0;
        ram->_st1 = 0;
    case 1:
        if (self->_windowPro != NULL) {
            beNDMWWindow_closeAll(self->_windowPro);
        }
        if (self->_windowPro->_isRun != false) {
            break;
        }
        ram->_st0++;
        ram->_st3 = 0;
        ram->_st2 = 0;
        ram->_st1 = 0;
    default:
        beNDMWStatus_Unref(self->_windowPro);
        self->_windowPro = NULL;
        beMessenger_callGroup(self->_messenger, "SOUND", "OFFREEZE");
        beMessenger_callGroup(self->_messenger, "GENERATER", "OFFREEZE");
        beMessenger_callGroup(self->_messenger, "MODELCTRL", "OFFREEZE");
        beMessenger_setFreeze(self->_messenger, false);
        ram->_isRun = false;
        for (i = 0; i < 4; i++) {
            beMessenger_selectGroup(self->_messenger, "GAMERAM");
            beMessenger_pushInt(self->_messenger, i);
            beMessenger_send(self->_messenger, "GETPLAYER");
            if (beDataObjList_getBool((beDataObjList*)beMessenger_getObject(self->_messenger, 0), "ISUSE")->_data) {
                beMessenger_selectGroup(self->_messenger, "GAMERAM");
                beMessenger_pushInt(self->_messenger, i);
                beMessenger_pushInt(self->_messenger, 0);
                beMessenger_send(self->_messenger, "ADDHP");
                beMessenger_selectGroup(self->_messenger, "GAMERAM");
                beMessenger_pushInt(self->_messenger, i);
                beMessenger_pushInt(self->_messenger, 0);
                beMessenger_send(self->_messenger, "ADDMP");
            }
        }
        if (self->_subWndNo == 10) {
            beMessenger_callGroup(self->_messenger, "STAGECTL", "ISRESETSTART");
            if (beMessenger_getBool(self->_messenger, 0) == false) {
                beMessenger_selectGroup(self->_messenger, "PLAYERPOINT");
                beMessenger_pushInt(self->_messenger, 4);
                beMessenger_send(self->_messenger, "SETGENERATEPOS");
                beMessenger_selectGroup(self->_messenger, "STAGECTL");
                beMessenger_pushString(self->_messenger, "stagML12");
                beMessenger_send(self->_messenger, "NEXTSTAGE");
            }
        } else {
            beMessenger_selectGroup(self->_messenger, "LAYERCTL");
            beMessenger_pushString(self->_messenger, "WN01");
            beMessenger_pushString(self->_messenger, "DISPON");
            beMessenger_send(self->_messenger, "SWITCH");
            beMessenger_selectGroup(self->_messenger, "LAYERCTL");
            beMessenger_pushString(self->_messenger, "WN00");
            beMessenger_pushString(self->_messenger, "DISPON");
            beMessenger_send(self->_messenger, "SWITCH");
            if (self->_subWndNo != 0) {
                func_00389FF0((beLua*)func_00236F80(self->_insight, beLua_Meta), "shopexit.lua", NULL);
            }
        }
        break;
    }
}

extern char D_00516E48[]; /* "SQ_MOVE" */
extern char D_00516D50[]; /* "SQ_CANCEL" */
extern char* D_004CC840[4]; /* cursor wait sequences */
extern char* D_004CC850[4]; /* cursor choose sequences */
void func_003D9E40(beNDMWStatusState* self, s32 a, s32 b);
void func_003EE190(beNDMWStatusSlots* slots, s32 ram);

#ifdef NON_MATCHING
/* Nonmatching: only the in-use flag of the pick (the original sets it from the 1 of the compare with andi; ours daddiu) and two loads around it; 99% structure. */
/* Move the save area cursor (2x2) with the pad, pick an entry (copied to the
   player's slot unless it is in use) or cancel; keep the in-use message right. */
bool func_003DB780(beNDMWStatusSaveArea* self, s32 n) {
    beNDMWStatusSlots* slots;
    beNDMWStatusMdlPos* mdl;
    s32 msg;
    beNDMWStatusB2In* list;
    bool done;
    s32 state;
    s32 trig;
    bool up;
    bool down;
    bool right;
    bool left;

    slots = self->unk24;
    mdl = (beNDMWStatusMdlPos*)self->unk1C;
    done = false;
    msg = self->unk28;
    list = self->unk2C;
    switch (n) {
    default:
        state = self->unk4C;
        break;
    case 1:
        state = self->unk50;
        break;
    case 2:
        state = self->unk54;
        break;
    case 3:
        state = self->unk58;
        break;
    }
    trig = 0;
    up = false;
    down = false;
    right = false;
    left = false;
    if (((beNDMWStatusSlotOwner**)slots->unk28->_data)[self->unk3C]->unkF == 1) {
        bePadData* pad = bePadManager_getPad(self->unk18, self->unk3C);
        s32 rep;
        s32 stick;

        trig = pad->_trigger;
        rep = pad->_repeat;
        stick = pad->_lTrg | pad->_lRpt;
        {
            bool u = true;

            if ((stick & 1) == 0 && ((trig | rep) & 0x10) == 0) {
                u = false;
            }
            up = u;
        }
        down = (stick & 4) || ((trig | rep) & 0x40);
        right = (stick & 8) || ((trig | rep) & 0x80);
        left = (stick & 2) || ((trig | rep) & 0x20);
    }
    switch (state) {
    case 0: {
        s32 c = self->unk14;
        s32 nc = c;

        if (up) {
            if (c == 2) {
                nc = 0;
            } else if (c == 3) {
                nc = 1;
            }
        } else if (down) {
            if (c == 0) {
                nc = 2;
            } else if (c == 1) {
                nc = 3;
            }
        } else if (left) {
            if (c == 0) {
                nc = 1;
            } else if (c == 2) {
                nc = 3;
            }
        } else if (right) {
            if (c == 1) {
                nc = 0;
            } else if (c == 3) {
                nc = 2;
            }
        }
        if (nc != c) {
            self->unk14 = nc;
            if (self->unk38 == 0) {
                func_003D9E40(list->unk10->unk10->unk0, 0, self->unk14);
            }
            func_003E4030((s32)mdl, self->unk3C, D_00516DB0, D_00516E48, 0);
            state = 1;
        } else if (trig & 0x4000) {
            if (self->unk38 == 0) {
                bool free;
                s32 i;
                beNDMWStatusSlotItem* cur =
                    ((beNDMWStatusSlotItem**)((beNDMWStatusSlotOwner**)slots->unk28->_data)[self->unk3C]->unk20->unkC->_data)[c];

                free = true;
                if (cur->unk28 == 1) {
                    for (i = 0; i < 4; i++) {
                        if (i != self->unk3C) {
                            beNDMWStatusSlotItem* o = ((beNDMWStatusSlotItem**)slots->unk24->_data)[i];

                            if (o->unk28 == 1 && o->unk2C == cur->unk2C) {
                                const char* a = o->unk34;
                                const char* b = cur->unk34;

                                if (a != NULL && b != NULL && strcmp(a, b) == 0) {
                                    free = false;
                                    break;
                                }
                            }
                        }
                    }
                    if (free == true) {
                        beNDMWStatusSlotItem* mine;
                        const char* name;
                        char* str;

                        ((beNDMWStatusSlotItem**)slots->unk24->_data)[self->unk3C]->unk28 = 1;
                        ((beNDMWStatusSlotItem**)slots->unk24->_data)[self->unk3C]->unk2C = cur->unk2C;
                        mine = ((beNDMWStatusSlotItem**)slots->unk24->_data)[self->unk3C];
                        name = cur->unk34;
                        str = func_002EAF60(beNDMWStatus_GetPool(), name);
                        if (mine->unk34 != NULL) {
                            beNDMWStatusStrHdr* h = (beNDMWStatusStrHdr*)mine->unk34 - 1;

                            if (--h->refs == 0) {
                                func_0030C940(h->owner, h);
                            }
                        }
                        mine->unk34 = str;
                        self->unk49 = 0;
                        state = 2;
                    }
                }
            } else {
                func_003EE190(slots, self->unk3C);
                self->unk49 = 0;
                state = 2;
            }
        } else if (trig & 0x2000) {
            state = 4;
            self->unk49 = 1;
        }
        if (self->unk38 == 0) {
            bool free;
            s32 i;
            beNDMWStatusSlotItem* cur;

            func_003D9E40(list->unk10->unk10->unk0, 0, self->unk14);
            cur = ((beNDMWStatusSlotItem**)((beNDMWStatusSlotOwner**)slots->unk28->_data)[self->unk3C]->unk20->unkC->_data)[self->unk14];
            if (cur->unk28 == 1) {
                free = true;
                for (i = 0; i < 4; i++) {
                    if (i != self->unk3C) {
                        beNDMWStatusSlotItem* o = ((beNDMWStatusSlotItem**)slots->unk24->_data)[i];

                        if (o->unk28 == 1 && o->unk2C == cur->unk2C) {
                            const char* a = o->unk34;
                            const char* b = cur->unk34;

                            if (a != NULL && b != NULL && strcmp(a, b) == 0) {
                                free = false;
                                break;
                            }
                        }
                    }
                }
                if (free == true) {
                    if (self->unk40 != 0) {
                        self->unk40 = 0;
                        func_003ECAD0(msg, (self->unk3C << 16) | 0x513);
                    }
                } else if (self->unk40 != 1) {
                    self->unk40 = 1;
                    func_003ECAD0(msg, (self->unk3C << 16) | 0x515);
                }
            } else if (self->unk40 != 0) {
                self->unk40 = 0;
                func_003ECAD0(msg, (self->unk3C << 16) | 0x513);
            }
        }
        break;
    }
    case 1:
        if (self->unk38 != 0 || func_003DA7C0(list->unk10->unk10->unk0) == 1) {
            if (func_003E4280((beNDMWStatusCnt*)mdl) == true) {
                func_003E4030((s32)mdl, self->unk3C, D_00516DB0, D_004CC840[self->unk14], 0);
                state = 0;
            }
        }
        break;
    case 2:
        func_003E42A0((beNDMWStatusMdl2*)mdl, self->unk3C, D_00516DB0, D_004CC850[self->unk14], 1);
        self->unk34 = 0;
        state = 3;
        break;
    case 3:
        if (func_003E44D0((beNDMWStatusCnt2*)mdl) == true) {
            state = 6;
        }
        break;
    case 4:
        func_003E42A0((beNDMWStatusMdl2*)mdl, self->unk3C, D_00516DB0, D_00516D50, 1);
        self->unk34 = 0;
        state = 5;
        break;
    case 5:
        if (func_003E44D0((beNDMWStatusCnt2*)mdl) == true) {
            state = 6;
        }
        break;
    case 6:
        done = true;
        break;
    }
    switch (n) {
    default:
        self->unk4C = state;
        self->unk58 = 0;
        self->unk54 = 0;
        self->unk50 = 0;
        break;
    case 1:
        self->unk50 = state;
        self->unk58 = 0;
        self->unk54 = 0;
        break;
    case 2:
        self->unk54 = state;
        self->unk58 = 0;
        break;
    case 3:
        self->unk58 = state;
        break;
    }
    return done;
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWStatus", func_003DB780);
#endif

extern char D_005171F0[]; /* "SQ_ON" */

/* Memory slot window: keep the slot marks and the text in step with the cards
   present, move the cursor with the pad, choose a card or cancel. Returns done. */
u8 func_003E1760(beNDMWStatusD* self, bool input, bool wait) {
    beNDMWStatusCards* cards;
    s32 trig;
    bool up;
    bool down;
    bool right;
    bool left;
    s32* tbl;
    s32 i;
    s32 j;
    s32 font;

    if (wait == true) {
        if (self->unk4C != 1) {
            return 0;
        }
        if (self->unk35 == 1) {
            return 1;
        }
    }
    cards = self->unk20;
    trig = 0;
    up = false;
    down = false;
    right = false;
    left = false;
    if (input == true) {
        bePadData* pad = bePadManager_getPad(self->unk14, self->unk30);
        s32 rep;
        s32 stick;

        trig = pad->_trigger;
        rep = pad->_repeat;
        stick = pad->_lTrg | pad->_lRpt;
        {
            bool u = true;

            if ((stick & 1) == 0 && ((trig | rep) & 0x10) == 0) {
                u = false;
            }
            up = u;
        }
        down = (stick & 4) || ((trig | rep) & 0x40);
        right = (stick & 8) || ((trig | rep) & 0x80);
        left = (stick & 2) || ((trig | rep) & 0x20);
    }
    switch (self->unk24) {
    case 1:
    case 2:
        break;
    default:
        if (self->unk44 == 0) {
            tbl = D_004CD090;
            for (i = 0; i < 2; i++) {
                if (((beNDMWStatusCard**)cards->unk34->unk2C->_data)[tbl[i * 2 + 1]]->unk8 == 1) {
                    switch (i) {
                        case 0:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 2, D_00517170) == false) {
                                func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30 * 2, D_00517170, "SQ_OPEN01", self->unk18->pos, 0);
                            } else {
                                func_003E4030((s32)self->unk18, self->unk30 * 2, D_00517170, "SQ_OPEN01", 0);
                            }
                            break;
                        case 1:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 2 + 1, D_00517170) == false) {
                                func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30 * 2 + 1, D_00517170, "SQ_OPEN02", self->unk18->pos, 0);
                            } else {
                                func_003E4030((s32)self->unk18, self->unk30 * 2 + 1, D_00517170, "SQ_OPEN02", 0);
                            }
                            break;
                    }
                } else {
                    switch (i) {
                        case 0:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 2, D_00517170) == 1) {
                                func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30 * 2, D_00517170, D_00516D30, 0);
                            }
                            break;
                        case 1:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 2 + 1, D_00517170) == 1) {
                                func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30 * 2 + 1, D_00517170, D_00516D30, 0);
                            }
                            break;
                    }
                }
            }
            font = self->unk1C;
            if (self->unk48 == 0) {
                if (((beNDMWStatusCard**)cards->unk34->unk2C->_data)[tbl[self->unk3C * 2 + 1]]->unk8 == 1) {
                    self->unk48 = 1;
                    func_003EC050(font, (self->unk30 << 16) | 0x102E);
                }
            } else {
                if (((beNDMWStatusCard**)cards->unk34->unk2C->_data)[tbl[self->unk3C * 2 + 1]]->unk8 != 1) {
                    self->unk48 = 0;
                    func_003EBE00(font, (self->unk30 << 16) | 0x102E, 0);
                }
            }
            if (cards->unk34->unkF == 1) {
                self->unk37 = 1;
                self->unk35 = 1;
                self->unk4C++;
                self->unk58 = 0;
                self->unk54 = 0;
                self->unk50 = 0;
                break;
            }
        } else {
            tbl = D_004CD060;
            for (j = 0; j < 5; j++) {
                if (((beNDMWStatusCard**)cards->unk34->unk2C->_data)[tbl[j * 2 + 1]]->unk8 == 1) {
                    switch (j) {
                        case 0:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 5, D_005171A0) == false) {
                                func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30 * 5, D_005171A0, "SQ_OPEN1A", self->unk18->pos, 0);
                            } else {
                                func_003E4030((s32)self->unk18, self->unk30 * 5, D_005171A0, "SQ_OPEN1A", 0);
                            }
                            break;
                        case 1:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 5 + 1, D_005171A0) == false) {
                                func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30 * 5 + 1, D_005171A0, "SQ_OPEN1B", self->unk18->pos, 0);
                            } else {
                                func_003E4030((s32)self->unk18, self->unk30 * 5 + 1, D_005171A0, "SQ_OPEN1B", 0);
                            }
                            break;
                        case 2:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 5 + 2, D_005171A0) == false) {
                                func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30 * 5 + 2, D_005171A0, "SQ_OPEN1C", self->unk18->pos, 0);
                            } else {
                                func_003E4030((s32)self->unk18, self->unk30 * 5 + 2, D_005171A0, "SQ_OPEN1C", 0);
                            }
                            break;
                        case 3:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 5 + 3, D_005171A0) == false) {
                                func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30 * 5 + 3, D_005171A0, "SQ_OPEN1D", self->unk18->pos, 0);
                            } else {
                                func_003E4030((s32)self->unk18, self->unk30 * 5 + 3, D_005171A0, "SQ_OPEN1D", 0);
                            }
                            break;
                        case 4:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 2, D_00517170) == false) {
                                func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk30 * 2, D_00517170, "SQ_OPEN02", self->unk18->pos, 0);
                            } else {
                                func_003E4030((s32)self->unk18, self->unk30 * 2, D_00517170, "SQ_OPEN02", 0);
                            }
                            break;
                    }
                } else {
                    switch (j) {
                        case 0:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 5, D_005171A0) == 1) {
                                func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30 * 5, D_005171A0, D_00516D30, 0);
                            }
                            break;
                        case 1:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 5 + 1, D_005171A0) == 1) {
                                func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30 * 5 + 1, D_005171A0, D_00516D30, 0);
                            }
                            break;
                        case 2:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 5 + 2, D_005171A0) == 1) {
                                func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30 * 5 + 2, D_005171A0, D_00516D30, 0);
                            }
                            break;
                        case 3:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 5 + 3, D_005171A0) == 1) {
                                func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30 * 5 + 3, D_005171A0, D_00516D30, 0);
                            }
                            break;
                        case 4:
                            if (func_003E4480((beNDMWStatusM*)self->unk18, self->unk30 * 2, D_00517170) == 1) {
                                func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk30 * 2, D_00517170, D_00516D30, 0);
                            }
                            break;
                    }
                }
            }
            if (cards->unk34->unkF == 0) {
                self->unk37 = 1;
                self->unk35 = 1;
                self->unk4C++;
                self->unk58 = 0;
                self->unk54 = 0;
                self->unk50 = 0;
                break;
            }
            font = self->unk1C;
            if (self->unk48 == 0) {
                if (((beNDMWStatusCard**)cards->unk34->unk2C->_data)[tbl[self->unk3C * 2 + 1]]->unk8 == 1) {
                    self->unk48 = 1;
                    func_003EC050(font, (self->unk30 << 16) | 0x102E);
                }
            } else {
                if (((beNDMWStatusCard**)cards->unk34->unk2C->_data)[tbl[self->unk3C * 2 + 1]]->unk8 != 1) {
                    self->unk48 = 0;
                    func_003EBE00(font, (self->unk30 << 16) | 0x102E, 0);
                }
            }
        }
        switch (self->unk50) {
        case 0:
            if (self->unk44 == 0) {
                if (up) {
                    if (self->unk3C != 0) {
                        self->unk3C = 0;
                        func_003E4030((s32)self->unk18, self->unk30, D_00517160, D_00516E48, 1);
                        self->unk50++;
                        self->unk58 = 0;
                        self->unk54 = 0;
                    }
                } else if (down) {
                    if (self->unk3C != 1) {
                        self->unk3C = 1;
                        func_003E4030((s32)self->unk18, self->unk30, D_00517160, D_00516E48, 1);
                        self->unk50++;
                        self->unk58 = 0;
                        self->unk54 = 0;
                    }
                } else if (trig & 0x4000) {
                    if (((beNDMWStatusCard**)cards->unk34->unk2C->_data)[tbl[self->unk3C * 2 + 1]]->unk8 == 1) {
                        func_003EE190((beNDMWStatusSlots*)cards, self->unk30);
                        self->unk40 = tbl[self->unk3C * 2 + 1];
                        func_003E4030((s32)self->unk18, self->unk30, D_00517160, D_005171F0, 1);
                        self->unk50 = 2;
                        self->unk58 = 0;
                        self->unk54 = 0;
                    }
                } else if (trig & 0x2000) {
                    self->unk36 = 1;
                    func_003E4030((s32)self->unk18, self->unk30, D_00517160, D_00516D50, 1);
                    self->unk50 = 2;
                    self->unk58 = 0;
                    self->unk54 = 0;
                }
            } else if (trig & 0x4000) {
                if (((beNDMWStatusCard**)cards->unk34->unk2C->_data)[tbl[self->unk3C * 2 + 1]]->unk8 == 1) {
                    func_003EE190((beNDMWStatusSlots*)cards, self->unk30);
                    self->unk40 = tbl[self->unk3C * 2 + 1];
                    func_003E4030((s32)self->unk18, self->unk30, D_00517160, D_005171F0, 1);
                    self->unk50 = 2;
                    self->unk58 = 0;
                    self->unk54 = 0;
                }
            } else if (trig & 0x2000) {
                self->unk36 = 1;
                func_003E4030((s32)self->unk18, self->unk30, D_00517160, D_00516D50, 1);
                self->unk50 = 2;
                self->unk58 = 0;
                self->unk54 = 0;
            } else if (up) {
                if (self->unk3C != 0) {
                    self->unk3C = 0;
                    func_003E4030((s32)self->unk18, self->unk30, D_00517160, D_00516E48, 1);
                    self->unk50++;
                    self->unk58 = 0;
                    self->unk54 = 0;
                }
            } else if (down) {
                if (self->unk3C != self->unk2C - 1) {
                    self->unk3C = self->unk2C - 1;
                    func_003E4030((s32)self->unk18, self->unk30, D_00517160, D_00516E48, 1);
                    self->unk50++;
                    self->unk58 = 0;
                    self->unk54 = 0;
                }
            } else if (self->unk3C != self->unk2C - 1) {
                if (left) {
                    self->unk3C++;
                    if (self->unk3C >= self->unk2C - 1) {
                        self->unk3C = self->unk2C - 2;
                    } else {
                        func_003E4030((s32)self->unk18, self->unk30, D_00517160, D_00516E48, 1);
                        self->unk50++;
                        self->unk58 = 0;
                        self->unk54 = 0;
                    }
                } else if (right) {
                    self->unk3C--;
                    if (self->unk3C < 0) {
                        self->unk3C = 0;
                    } else {
                        func_003E4030((s32)self->unk18, self->unk30, D_00517160, D_00516E48, 1);
                        self->unk50++;
                        self->unk58 = 0;
                        self->unk54 = 0;
                    }
                }
            }
            break;
        case 1:
            if (func_003E4280((beNDMWStatusCnt*)self->unk18) == true) {
                func_003E4030((s32)self->unk18, self->unk30, D_00517160, (const char*)tbl[self->unk3C * 2], 0);
                font = self->unk1C;
                if (self->unk44 == 0) {
                    switch (self->unk3C) {
                        case 0:
                            func_003EBE00(font, (self->unk30 << 16) | 0x1100, 0);
                            break;
                        case 1:
                            func_003EBE00(font, (self->unk30 << 16) | 0x1101, 0);
                            break;
                    }
                } else {
                    switch (self->unk3C) {
                        case 0:
                            func_003EBE00(font, (self->unk30 << 16) | 0x1102, 0);
                            break;
                        case 1:
                            func_003EBE00(font, (self->unk30 << 16) | 0x1103, 0);
                            break;
                        case 2:
                            func_003EBE00(font, (self->unk30 << 16) | 0x1104, 0);
                            break;
                        case 3:
                            func_003EBE00(font, (self->unk30 << 16) | 0x1105, 0);
                            break;
                        case 4:
                            func_003EBE00(font, (self->unk30 << 16) | 0x1101, 0);
                            break;
                    }
                }
                font = self->unk1C;
                if (self->unk48 == 0) {
                    if (((beNDMWStatusCard**)cards->unk34->unk2C->_data)[tbl[self->unk3C * 2 + 1]]->unk8 == 1) {
                        self->unk48 = 1;
                        func_003EC050(font, (self->unk30 << 16) | 0x102E);
                    }
                } else {
                    if (((beNDMWStatusCard**)cards->unk34->unk2C->_data)[tbl[self->unk3C * 2 + 1]]->unk8 != 1) {
                        self->unk48 = 0;
                        func_003EBE00(font, (self->unk30 << 16) | 0x102E, 0);
                    }
                }
                self->unk50 = 0;
                self->unk58 = 0;
                self->unk54 = 0;
            }
            break;
        case 2:
            if (func_003E4280((beNDMWStatusCnt*)self->unk18) == true) {
                func_003E4030((s32)self->unk18, self->unk30, D_00517160, (const char*)tbl[self->unk3C * 2], 0);
                self->unk35 = 1;
                self->unk4C++;
                self->unk58 = 0;
                self->unk54 = 0;
                self->unk50 = 0;
            }
            break;
        }
        break;
    }
    return self->unk35;
}

extern char* D_004CEC74[][3]; /* face sequences */
extern char D_00516CD0[]; /* "SQ_OPEN" */
extern char D_00516FF8[]; /* "j_namecursorjp" */
extern char D_00517008[]; /* "j_namecursorus" */

/* The character under the cursor of the current wheel goes into the typed text. */
inline void NameSetChar(beNDMWStatusCur* self) {
    self->val[self->cur] = D_004CC860[self->unk34].chars[(self->unk38 + self->unk3C) % (s32)D_004CC860[self->unk34].count];
}

/* Play a sequence on the text cursor (Japanese or English layout). */
inline void NameCursorSeq(beNDMWStatusCur* self, const char* seq, s32 flag) {
    switch (self->mode) {
    case 0:
        func_003E4030((s32)self->unk18, self->unk20, D_00516FF8, seq, flag);
        break;
    default:
    case 1:
        func_003E4030((s32)self->unk18, self->unk20, D_00517008, seq, flag);
        break;
    }
}

inline void NameCursorClose(beNDMWStatusCur* self, const char* seq) {
    switch (self->mode) {
    case 0:
        func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk20, D_00516FF8, seq, 1);
        break;
    default:
    case 1:
        func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk20, D_00517008, seq, 1);
        break;
    }
}

/* Name entry: open the windows, move over the character wheels with the pad,
   type (circle), delete (cross / select), ask for confirmation and close.
   Returns done. */
u8 func_003DEF80(beNDMWStatusCur* self, bool input) {
    s32 trig;
    bool up;
    bool down;
    bool right;
    bool left;

    if (self->unk4C == 1) {
        return 1;
    }
    trig = 0;
    up = false;
    down = false;
    right = false;
    left = false;
    if (input == true) {
        bePadData* pad = bePadManager_getPad(self->unk14, self->unk20);
        s32 rep;
        s32 stick;

        trig = pad->_trigger;
        rep = pad->_repeat;
        stick = pad->_lTrg | pad->_lRpt;
        {
            bool u = true;

            if ((stick & 1) == 0 && ((trig | rep) & 0x10) == 0) {
                u = false;
            }
            up = u;
        }
        down = (stick & 4) || ((trig | rep) & 0x40);
        right = (stick & 8) || ((trig | rep) & 0x80);
        left = (stick & 2) || ((trig | rep) & 0x20);
    }
    switch (self->unk54) {
    case 0:
        switch (self->unk58) {
        case 0: {
            char buf[80];

            func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk20, "j_digifacewin", D_004CEC74[self->unk24][0], self->unk18->pos, 1);
            func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk20, "j_namecursor01", D_00516CD0, self->unk18->pos, 1);
            func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk20, "j_namecursor02", D_00516CD0, self->unk18->pos, 1);
            func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk20, "j_namewin01", D_00516CD0, self->unk18->pos, 1);
            func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk20, "j_namecursordown", D_00516CD0, self->unk18->pos, 1);
            func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk20, "j_namecursorup", D_00516CD0, self->unk18->pos, 1);
            func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk20, "j_namecursorleft", D_00516CD0, self->unk18->pos, 1);
            func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk20, "j_namecursorright", D_00516CD0, self->unk18->pos, 1);
            self->unk51 = 1;
            NameSetChar(self);
            sprintf(buf, "SQ_NAMESL0%d", self->cur);
            switch (self->mode) {
            case 0:
                func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk20, "j_namecursorjp", buf, self->unk18->pos, 0);
                break;
            default:
            case 1:
                func_003E3E00((beNDMWStatusMdl*)self->unk18, self->unk20, "j_namecursorus", buf, self->unk18->pos, 0);
                break;
            }
            func_003DE480((beNDMWStatusWheel*)self, 0);
            func_003DE7A0((beNDMWStatusH*)self, 0);
            func_003DE990(self, 0);
            self->unk58++;
            self->unk60 = 0;
            self->unk5C = 0;
            break;
        }
        case 1:
            if (func_003E4280((beNDMWStatusCnt*)self->unk18) == true) {
                self->unk58++;
                self->unk60 = 0;
                self->unk5C = 0;
            }
            break;
        case 2: {
            char buf[80];

            if (self->cur >= self->unk44) {
                self->cur = self->unk44 - 1;
            }
            sprintf(buf, "SQ_SELECT0%d", self->unk3C);
            func_003E4030((s32)self->unk18, self->unk20, "j_namecursor02", buf, 1);
            self->unk58++;
            self->unk60 = 0;
            self->unk5C = 0;
            break;
        }
        case 3:
            if (func_003E4280((beNDMWStatusCnt*)self->unk18) == true) {
                self->unk54 = 1;
                self->unk60 = 0;
                self->unk5C = 0;
                self->unk58 = 0;
            }
            break;
        }
        break;
    case 1:
        switch (self->unk58) {
        case 0:
            if (left) {
                self->unk3C++;
                if (self->unk3C >= 5) {
                    self->unk3C = 4;
                    if ((s32)D_004CC860[self->unk34].count > 5) {
                        self->unk38++;
                        if (self->unk38 >= (s32)D_004CC860[self->unk34].count) {
                            self->unk38 = 0;
                        }
                        if (self->unk51 == 1) {
                            char buf[80];

                            func_003E4030((s32)self->unk18, self->unk20, "j_namecursorright", "SQ_ON00", 1);
                            sprintf(buf, "SQ_NAMECG0%d", self->cur);
                            NameCursorSeq(self, buf, 1);
                        }
                        func_003DE7A0((beNDMWStatusH*)self, 0);
                    }
                } else {
                    char buf[80];
                    char buf2[80];

                    sprintf(buf, "SQ_SELECT0%d", self->unk3C);
                    func_003E4030((s32)self->unk18, self->unk20, "j_namecursor02", buf, 1);
                    sprintf(buf2, "SQ_NAMECG0%d", self->cur);
                    NameCursorSeq(self, buf2, 1);
                }
                NameSetChar(self);
                func_003DE990(self, 0);
                self->unk58++;
                self->unk60 = 0;
                self->unk5C = 0;
            } else if (right) {
                self->unk3C--;
                if (self->unk3C < 0) {
                    self->unk3C = 0;
                    if ((s32)D_004CC860[self->unk34].count > 5) {
                        self->unk38--;
                        if (0 > self->unk38) {
                            self->unk38 = D_004CC860[self->unk34].count - 1;
                        }
                        func_003DE7A0((beNDMWStatusH*)self, 0);
                        if (self->unk51 == 1) {
                            char buf[80];

                            func_003E4030((s32)self->unk18, self->unk20, "j_namecursorleft", "SQ_ON00", 1);
                            sprintf(buf, "SQ_NAMECG0%d", self->cur);
                            NameCursorSeq(self, buf, 1);
                        }
                    }
                } else {
                    char buf[80];
                    char buf2[80];

                    sprintf(buf, "SQ_SELECT0%d", self->unk3C);
                    func_003E4030((s32)self->unk18, self->unk20, "j_namecursor02", buf, 1);
                    sprintf(buf2, "SQ_NAMECG0%d", self->cur);
                    NameCursorSeq(self, buf2, 1);
                }
                NameSetChar(self);
                func_003DE990(self, 0);
                self->unk58++;
                self->unk60 = 0;
                self->unk5C = 0;
            } else if (up) {
                char buf[80];

                func_003E4030((s32)self->unk18, self->unk20, "j_namecursorup", "SQ_ON00", 1);
                sprintf(buf, "SQ_NAMECG0%d", self->cur);
                NameCursorSeq(self, buf, 1);
                self->unk34--;
                if (self->unk34 < self->unk30) {
                    self->unk34 = self->unk2C - 1;
                }
                if ((s32)D_004CC860[self->unk34].count > 5) {
                    func_003E4030((s32)self->unk18, self->unk20, "j_namecursorleft", "SQ_SELECT00", 0);
                    func_003E4030((s32)self->unk18, self->unk20, "j_namecursorright", "SQ_SELECT00", 0);
                    self->unk51 = 1;
                } else {
                    func_003E4030((s32)self->unk18, self->unk20, "j_namecursorleft", "SQ_NOSELECT", 0);
                    func_003E4030((s32)self->unk18, self->unk20, "j_namecursorright", "SQ_NOSELECT", 0);
                    self->unk51 = 0;
                }
                self->unk38 = 0;
                NameSetChar(self);
                self->unk58++;
                self->unk60 = 0;
                self->unk5C = 0;
                func_003DE990(self, 0);
                func_003DE480((beNDMWStatusWheel*)self, 0);
                func_003DE7A0((beNDMWStatusH*)self, 0);
            } else if (down) {
                char buf[80];

                func_003E4030((s32)self->unk18, self->unk20, "j_namecursordown", "SQ_ON00", 1);
                sprintf(buf, "SQ_NAMECG0%d", self->cur);
                NameCursorSeq(self, buf, 1);
                self->unk34++;
                if (self->unk34 >= self->unk2C) {
                    self->unk34 = self->unk30;
                }
                if ((s32)D_004CC860[self->unk34].count > 5) {
                    func_003E4030((s32)self->unk18, self->unk20, "j_namecursorleft", "SQ_SELECT00", 0);
                    func_003E4030((s32)self->unk18, self->unk20, "j_namecursorright", "SQ_SELECT00", 0);
                    self->unk51 = 1;
                } else {
                    func_003E4030((s32)self->unk18, self->unk20, "j_namecursorleft", "SQ_NOSELECT", 0);
                    func_003E4030((s32)self->unk18, self->unk20, "j_namecursorright", "SQ_NOSELECT", 0);
                    self->unk51 = 0;
                }
                self->unk38 = 0;
                NameSetChar(self);
                self->unk58++;
                self->unk60 = 0;
                self->unk5C = 0;
                func_003DE990(self, 0);
                func_003DE7A0((beNDMWStatusH*)self, 0);
                func_003DE480((beNDMWStatusWheel*)self, 0);
            } else if (trig & 0x4000) {
                if (self->cur < self->unk44) {
                    char buf[80];

                    self->pos[self->cur][0] = self->unk34;
                    self->pos[self->cur][1] = self->unk38;
                    self->pos[self->cur][2] = self->unk3C;
                    self->flag[self->cur] = 2;
                    NameSetChar(self);
                    sprintf(buf, "SQ_NAMEON0%d", self->cur);
                    NameCursorSeq(self, buf, 1);
                    self->cur++;
                    self->val[self->cur + 1] = 0;
                    self->flag[self->cur] = 1;
                    self->unk58 = 3;
                    self->unk60 = 0;
                    self->unk5C = 0;
                }
                func_003DE990(self, 0);
            } else if (trig & 0x2000) {
                self->val[self->cur] = 0;
                self->cur--;
                NameCursorSeq(self, "SQ_NAMECANCEL", 1);
                if (self->cur < 0) {
                    self->cur = -1;
                    self->unk54 = 4;
                    self->unk60 = 0;
                    self->unk5C = 0;
                    self->unk58 = 0;
                } else {
                    char buf[80];

                    self->unk34 = self->pos[self->cur][0];
                    self->unk38 = self->pos[self->cur][1];
                    self->unk3C = self->pos[self->cur][2];
                    NameSetChar(self);
                    sprintf(buf, "SQ_SELECT0%d", self->unk3C);
                    func_003E4030((s32)self->unk18, self->unk20, "j_namecursor02", buf, 1);
                    func_003DE7A0((beNDMWStatusH*)self, 0);
                    func_003DE480((beNDMWStatusWheel*)self, 0);
                    self->unk58 = 2;
                    self->unk60 = 0;
                    self->unk5C = 0;
                }
                func_003DE990(self, 0);
            } else if (trig & 8) {
                if (self->cur > 0) {
                    char buf[80];

                    sprintf(buf, "SQ_NAMEON0%d", self->cur);
                    NameCursorSeq(self, buf, 1);
                    self->val[self->cur] = 0;
                    self->cur--;
                    func_003DE990(self, 0);
                    self->unk58 = 4;
                    self->unk60 = 0;
                    self->unk5C = 0;
                }
            }
            break;
        case 1:
            if (func_003E4280((beNDMWStatusCnt*)self->unk18) != false) {
                self->unk58 = 0;
                self->unk60 = 0;
                self->unk5C = 0;
            }
            break;
        case 2:
            if (func_003E4280((beNDMWStatusCnt*)self->unk18) != false) {
                char buf[80];

                sprintf(buf, "SQ_NAMESL0%d", self->cur);
                NameCursorSeq(self, buf, 0);
                self->flag[self->cur] = 1;
                func_003DE990(self, 0);
                self->unk58 = 0;
                self->unk60 = 0;
                self->unk5C = 0;
            }
            break;
        case 3:
            if (func_003E4280((beNDMWStatusCnt*)self->unk18) != false) {
                s32 i;

                if (self->cur >= self->unk44) {
                    self->cur = self->unk44 - 1;
                    self->unk54 = 2;
                    self->unk60 = 0;
                    self->unk5C = 0;
                    self->unk58 = 0;
                } else {
                    char buf[80];

                    sprintf(buf, "SQ_NAMESL0%d", self->cur);
                    NameCursorSeq(self, buf, 0);
                    NameSetChar(self);
                    func_003DE990(self, 0);
                }
                for (i = self->cur + 1; i < self->unk44; i++) {
                    self->val[i] = 0;
                }
                self->unk58 = 0;
                self->unk60 = 0;
                self->unk5C = 0;
            }
            break;
        case 4:
            if (func_003E4280((beNDMWStatusCnt*)self->unk18) != false) {
                self->unk54 = 2;
                self->unk60 = 0;
                self->unk5C = 0;
                self->unk58 = 0;
            }
            break;
        }
        break;
    case 2:
        switch (self->unk58) {
        case 0:
            NameCursorSeq(self, "SQ_LINESL00", 1);
            self->unk58++;
            self->unk60 = 0;
            self->unk5C = 0;
            break;
        case 1:
            if (func_003E44D0((beNDMWStatusCnt2*)self->unk18) == true) {
                self->unk54 = 3;
                self->unk60 = 0;
                self->unk5C = 0;
                self->unk58 = 0;
            }
            break;
        }
        break;
    case 3:
        switch (self->unk58) {
        case 0:
            self->unk4E = 1;
            self->unk4F = 0;
            self->unk58++;
            self->unk60 = 0;
            self->unk5C = 0;
            break;
        case 1:
            if (self->unk4F == 1) {
                self->unk4E = 0;
                if (self->unk50 == 1) {
                    self->unk58++;
                    self->unk60 = 0;
                    self->unk5C = 0;
                    self->unk4D = 1;
                } else {
                    self->unk4D = 0;
                    self->unk58 = 4;
                    self->unk60 = 0;
                    self->unk5C = 0;
                }
            }
            break;
        case 2:
            NameCursorClose(self, "SQ_LINEON00");
            self->unk58++;
            self->unk60 = 0;
            self->unk5C = 0;
            break;
        case 3:
            if (func_003E44D0((beNDMWStatusCnt2*)self->unk18) == true) {
                self->unk54 = 5;
                self->unk60 = 0;
                self->unk5C = 0;
                self->unk58 = 0;
            }
            break;
        case 4: {
            char buf[80];

            self->unk34 = self->pos[self->cur][0];
            self->unk38 = self->pos[self->cur][1];
            self->unk3C = self->pos[self->cur][2];
            self->flag[self->cur] = 1;
            sprintf(buf, "SQ_NAMESL0%d", self->cur);
            NameCursorSeq(self, buf, 0);
            sprintf(buf, "SQ_SELECT0%d", self->unk3C);
            func_003E4030((s32)self->unk18, self->unk20, "j_namecursor02", buf, 1);
            func_003DE7A0((beNDMWStatusH*)self, 0);
            func_003DE480((beNDMWStatusWheel*)self, 0);
            func_003DE990(self, 0);
            self->unk58++;
            self->unk60 = 0;
            self->unk5C = 0;
            break;
        }
        case 5:
            if (func_003E44D0((beNDMWStatusCnt2*)self->unk18) == true) {
                self->unk54 = 1;
                self->unk60 = 0;
                self->unk5C = 0;
                self->unk58 = 0;
            }
            break;
        }
        break;
    case 4:
        switch (self->unk58) {
        case 0:
            NameCursorClose(self, D_00516D30);
            self->unk58++;
            self->unk60 = 0;
            self->unk5C = 0;
            break;
        case 1:
            if (func_003E44D0((beNDMWStatusCnt2*)self->unk18) == true) {
                self->unk54 = 5;
                self->unk60 = 0;
                self->unk5C = 0;
                self->unk58 = 0;
            }
            break;
        }
        break;
    case 5:
        switch (self->unk58) {
        case 0:
            func_003DE480((beNDMWStatusWheel*)self, 1);
            func_003DE7A0((beNDMWStatusH*)self, 1);
            func_003DE990(self, 1);
            func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk20, "j_digifacewin", D_00516D30, 1);
            func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk20, "j_namewin01", D_00516D30, 1);
            func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk20, "j_namecursor01", D_00516D30, 1);
            func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk20, "j_namecursor02", D_00516D30, 1);
            func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk20, "j_namecursordown", D_00516D30, 1);
            func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk20, "j_namecursorup", D_00516D30, 1);
            func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk20, "j_namecursorleft", D_00516D30, 1);
            func_003E42A0((beNDMWStatusMdl2*)self->unk18, self->unk20, "j_namecursorright", D_00516D30, 1);
            self->unk58++;
            self->unk60 = 0;
            self->unk5C = 0;
            break;
        case 1:
            if (func_003E44D0((beNDMWStatusCnt2*)self->unk18) == true) {
                self->unk4C = 1;
            }
            break;
        }
        break;
    }
    return self->unk4C;
}

s32 beMessenger_getInt(void* m, s32 i);
const char* beMessenger_getString(void* m, s32 i);
s32 beMessenger_getMessageIndex(void* m, igObject* table, const char* name);
void beMessenger_delayCallGroup(void* m, const char* group, const char* message, s32 delay);
void beMessenger_pushBool(void* m, bool v);
void beMessenger_pushObject(void* m, void* o);
beDataObjObjectList* beDataObjList_getObjectList(beDataObjList* list, const char* name);
extern "C" void func_002D6790(void* list, s32 n, s32 size);
/* Short fixed strings copied in place (the original expands these inline). */
struct beNDMWStatusStr5 {
    f32 a;
    u8 b;
};

struct beNDMWStatusStr2 {
    s16 a;
};

inline void StatusCopy5(char* d, const char* s) {
    char* d_ = d;
    const char* s_ = s;

    *(beNDMWStatusStr5*)d_ = *(const beNDMWStatusStr5*)s_;
}

inline void StatusCopy2(char* d, const char* s) {
    char* d_ = d;
    const char* s_ = s;

    *(beNDMWStatusStr2*)d_ = *(const beNDMWStatusStr2*)s_;
}
extern "C" igObject* func_003BB730(void); /* handle's message table */
extern "C" igObject* func_003B1D70(void); /* window names */

/* An object held for a scope (counted reference). */
struct beNDMWStatusHold {
    igObject* p;
    beNDMWStatusHold(igObject* o) : p(o) {
        if (p != NULL) {
            p->_refCount++;
        }
    }
    ~beNDMWStatusHold() {
        igObject* o = p;
        if (o != NULL) {
            o->_refCount--;
            if ((o->_refCount & 0x7FFFFF) == 0) {
                func_002FF820(o);
            }
        }
    }
};

#define STATUS_TEXTSET(self, win, font, text)                                  \
    beMessenger_selectGroup((self)->_messenger, "TEXTCTL");                    \
    beMessenger_pushString((self)->_messenger, win);                           \
    beMessenger_pushString((self)->_messenger, font);                          \
    beMessenger_pushString((self)->_messenger, text);                          \
    beMessenger_send((self)->_messenger, "TEXTSET");

#define STATUS_PARAM_ADD(work, v)                                              \
    {                                                                          \
        igIntList* list_ = (work)->_parameter;                                 \
        s32 k_ = list_->_count;                                                \
                                                                               \
        if (k_ < list_->_capacity) {                                           \
            (*(volatile s32*)&list_->_count)++;                                \
        } else {                                                               \
            func_002D6790(list_, k_ + 1, 4);                                   \
        }                                                                      \
        ((s32*)list_->_data)[k_] = v;                                                  \
    }

/* Gray when unchanged, red when lower, blue when higher than before. */
#define STATUS_COMPARE(v, old)                                                 \
    if ((v) == (old)) {                                                        \
        r = 0.9f;                                                              \
        g = 0.9f;                                                              \
        b = 0.9f;                                                              \
    } else if ((v) < (old)) {                                                  \
        r = 0.94f;                                                             \
        g = 0.4f;                                                              \
        b = 0.25f;                                                             \
    } else {                                                                   \
        r = 0.0f;                                                              \
        g = 0.96f;                                                             \
        b = 1.0f;                                                              \
    }

#define STATUS_COLOR(self, win, font)                                          \
    beMessenger_selectGroup((self)->_messenger, "TEXTCTL");                    \
    beMessenger_pushString((self)->_messenger, win);                           \
    beMessenger_pushString((self)->_messenger, font);                          \
    beMessenger_pushVec3f((self)->_messenger, beNDMWStatusVec3(r, g, b));      \
    beMessenger_send((self)->_messenger, "COLOR");

#ifdef NON_MATCHING
/* Nonmatching: the "----" / "0" copies; the original copies them inline with both addresses in registers (lwc1 + lbu, lh), likely an expanded strcpy that this compiler setup does not reproduce; 4 instructions off. */
/* Status window messages: open one (frozen game), close it, fill the player's
   texts (keeping the values to compare), color them against the kept values,
   restore them, and hand out the shops' sales lists. */
extern "C" bool beNDMWStatus_handle(beNDMWStatus* self, beNDMWStatusInfoRam* ram, beMessengerWork* work_) {
    switch (beMessenger_getMessageIndex(self->_messenger, func_003BB730(), work_->_message)) {
    case 6:
        beMessenger_pushBool(self->_messenger, ram->_isRun);
        return true;
    case 0:
        if (ram->_isRun == false) {
            s32 who = beMessenger_getInt(self->_messenger, 0);
            const char* name = beMessenger_getString(self->_messenger, 0);
            s32 no = beMessenger_getMessageIndex(self->_messenger, func_003B1D70(), name);

            ram->_isRun = true;
            ram->_st0 = 0;
            ram->_st3 = 0;
            ram->_st2 = 0;
            ram->_st1 = 0;
            self->_subWndNo = no;
            self->_whoGenerate = who;
            beMessenger_callGroup(self->_messenger, "MODELCTRL", "ONFREEZE");
            beMessenger_callGroup(self->_messenger, "GENERATER", "ONFREEZE");
            beMessenger_setFreeze(self->_messenger, true);
            beMessenger_callGroup(self->_messenger, "SOUND", "ONFREEZE");
        }
        return true;
    case 1:
        if (ram->_isRun == true) {
            beNDMWWindow* w = self->_windowPro;

            if (w != NULL) {
                w->_st0 = 2;
                w->_st3 = 0;
                w->_st2 = 0;
                w->_st1 = 0;
            } else {
                beMessenger_setFreeze(self->_messenger, false);
                beMessenger_delayCallGroup(self->_messenger, D_00516788, "REMOVE", 1);
            }
        }
        return true;
    case 2: {
        beNDMWStatusInfoWork* work = ram->_work;
        s32 who = beMessenger_getInt(self->_messenger, 0);
        beDataObjList* pl;
        s32 level;
        s32 hp;
        s32 mp;
        s32 v;
        s32 exp;
        s32 rest;
        char buf[16];

        if (work == NULL) {
            func_003D8740(self, ram);
            work = ram->_work;
        }
        work->_parameter->_count = 0;
        beMessenger_selectGroup(self->_messenger, "GAMERAM");
        beMessenger_pushInt(self->_messenger, who);
        beMessenger_send(self->_messenger, "GETPLAYER");
        pl = (beDataObjList*)beMessenger_getObject(self->_messenger, 0);
        {
            beNDMWStatusHold name(((igObject**)beDataObjList_getObjectList(pl, "PLAYERNAME")->_data->_data)[0]);

            beMessenger_selectGroup(self->_messenger, "TEXTCTL");
            beMessenger_pushString(self->_messenger, "STWINFONT02");
            beMessenger_pushString(self->_messenger, "font000");
            beMessenger_pushObject(self->_messenger, name.p);
            beMessenger_send(self->_messenger, "TEXTSET");
            level = beDataObjList_getInt(pl, "LEVEL")->_data;
            if (level > 999) {
                level = 999;
            }
            sprintf(buf, "%d", level);
            STATUS_TEXTSET(self, "STWINFONT04", "font000", buf);
            beMessenger_call(self->_messenger, D_00516980, "shinka", 0, D_00516998);
            {
                beDataBase* db = (beDataBase*)beMessenger_getObject(self->_messenger, 0);
                const char* dname = beDataBase_getString(db, beDataObjList_getInt(pl, "DIGIMONNO")->_data, "NAME");

                STATUS_TEXTSET(self, "STWINFONT04", "font001", dname);
            }
            hp = beDataObjList_getInt(pl, "HP")->_data;
            sprintf(buf, "%d", hp);
            STATUS_TEXTSET(self, "STWINFONT06", "font000", buf);
            mp = beDataObjList_getInt(pl, "MP")->_data;
            sprintf(buf, "%d", mp);
            STATUS_TEXTSET(self, "STWINFONT06", "font001", buf);
            v = beDataObjList_getInt(pl, "MHP")->_data;
            sprintf(buf, "%d", v);
            STATUS_TEXTSET(self, "STWINFONT06", "font010", buf);
            STATUS_PARAM_ADD(work, v);
            v = beDataObjList_getInt(pl, "MMP")->_data;
            sprintf(buf, "%d", v);
            STATUS_TEXTSET(self, "STWINFONT06", "font011", buf);
            STATUS_PARAM_ADD(work, v);
            v = beDataObjList_getInt(pl, "TSUYOSA")->_data;
            sprintf(buf, "%d", v);
            STATUS_TEXTSET(self, "STWINFONT06", "font040", buf);
            STATUS_PARAM_ADD(work, v);
            v = beDataObjList_getInt(pl, "MAMORI")->_data;
            sprintf(buf, "%d", v);
            STATUS_TEXTSET(self, "STWINFONT06", "font041", buf);
            STATUS_PARAM_ADD(work, v);
            v = beDataObjList_getInt(pl, "KASHIKOSA")->_data;
            sprintf(buf, "%d", v);
            STATUS_TEXTSET(self, "STWINFONT06", "font042", buf);
            STATUS_PARAM_ADD(work, v);
            v = beDataObjList_getInt(pl, "SEISHIN")->_data;
            sprintf(buf, "%d", v);
            STATUS_TEXTSET(self, "STWINFONT06", "font043", buf);
            STATUS_PARAM_ADD(work, v);
            v = beDataObjList_getInt(pl, "SUBAYASA")->_data;
            sprintf(buf, "%d", v);
            STATUS_TEXTSET(self, "STWINFONT06", "font044", buf);
            STATUS_PARAM_ADD(work, v);
            v = beDataObjList_getInt(pl, "HONOOTEIKO")->_data;
            sprintf(buf, "%d", v);
            STATUS_TEXTSET(self, "STWINFONT06", "font045", buf);
            STATUS_PARAM_ADD(work, v);
            v = beDataObjList_getInt(pl, "KOORITEIKO")->_data;
            sprintf(buf, "%d", v);
            STATUS_TEXTSET(self, "STWINFONT06", "font046", buf);
            STATUS_PARAM_ADD(work, v);
            v = beDataObjList_getInt(pl, "KAMINARITEIKO")->_data;
            sprintf(buf, "%d", v);
            STATUS_TEXTSET(self, "STWINFONT06", "font047", buf);
            STATUS_PARAM_ADD(work, v);
            v = beDataObjList_getInt(pl, "YAMITEIKO")->_data;
            sprintf(buf, "%d", v);
            STATUS_TEXTSET(self, "STWINFONT06", "font048", buf);
            STATUS_PARAM_ADD(work, v);
            sprintf(buf, "%d", beDataObjList_getInt(pl, "XDATA")->_data);
            STATUS_TEXTSET(self, "STWINFONT06", "font049", buf);
            sprintf(buf, "%d", beDataObjList_getInt(pl, "BIT")->_data);
            STATUS_TEXTSET(self, "STWINFONT06", "font070", buf);
            exp = beDataObjList_getInt(pl, "EXP")->_data;
            rest = beDataObjList_getInt(pl, "NEXTEXP")->_data - exp;
            if (level >= 999) {
                StatusCopy5(buf, "----");
            } else if (rest < 0) {
                StatusCopy2(buf, "0");
            } else {
                sprintf(buf, "%d", rest);
            }
            STATUS_TEXTSET(self, "STWINFONT06", "font071", buf);
            STATUS_PARAM_ADD(work, hp);
            STATUS_PARAM_ADD(work, mp);
        }
        return true;
    }
    case 4: {
        beNDMWStatusInfoWork* work = ram->_work;
        s32 who = beMessenger_getInt(self->_messenger, 0);
        beDataObjList* pl;
        s32 v;
        f32 r;
        f32 g;
        f32 b;
        char buf[16];

        beMessenger_selectGroup(self->_messenger, "GAMERAM");
        beMessenger_pushInt(self->_messenger, who);
        beMessenger_send(self->_messenger, "GETPLAYER");
        pl = (beDataObjList*)beMessenger_getObject(self->_messenger, 0);
        v = beDataObjList_getInt(pl, "MHP")->_data;
        STATUS_COMPARE(v, ((s32*)work->_parameter->_data)[0]);
        sprintf(buf, "%d", v);
        STATUS_TEXTSET(self, "STWINFONT06", "font010", buf);
        STATUS_COLOR(self, "STWINFONT06", "font010");
        v = beDataObjList_getInt(pl, "MMP")->_data;
        STATUS_COMPARE(v, ((s32*)work->_parameter->_data)[1]);
        sprintf(buf, "%d", v);
        STATUS_TEXTSET(self, "STWINFONT06", "font011", buf);
        STATUS_COLOR(self, "STWINFONT06", "font011");
        v = beDataObjList_getInt(pl, "TSUYOSA")->_data;
        STATUS_COMPARE(v, ((s32*)work->_parameter->_data)[2]);
        sprintf(buf, "%d", v);
        STATUS_TEXTSET(self, "STWINFONT06", "font040", buf);
        STATUS_COLOR(self, "STWINFONT06", "font040");
        v = beDataObjList_getInt(pl, "MAMORI")->_data;
        STATUS_COMPARE(v, ((s32*)work->_parameter->_data)[3]);
        sprintf(buf, "%d", v);
        STATUS_TEXTSET(self, "STWINFONT06", "font041", buf);
        STATUS_COLOR(self, "STWINFONT06", "font041");
        v = beDataObjList_getInt(pl, "KASHIKOSA")->_data;
        STATUS_COMPARE(v, ((s32*)work->_parameter->_data)[4]);
        sprintf(buf, "%d", v);
        STATUS_TEXTSET(self, "STWINFONT06", "font042", buf);
        STATUS_COLOR(self, "STWINFONT06", "font042");
        v = beDataObjList_getInt(pl, "SEISHIN")->_data;
        STATUS_COMPARE(v, ((s32*)work->_parameter->_data)[5]);
        sprintf(buf, "%d", v);
        STATUS_TEXTSET(self, "STWINFONT06", "font043", buf);
        STATUS_COLOR(self, "STWINFONT06", "font043");
        v = beDataObjList_getInt(pl, "SUBAYASA")->_data;
        STATUS_COMPARE(v, ((s32*)work->_parameter->_data)[6]);
        sprintf(buf, "%d", v);
        STATUS_TEXTSET(self, "STWINFONT06", "font044", buf);
        STATUS_COLOR(self, "STWINFONT06", "font044");
        v = beDataObjList_getInt(pl, "HONOOTEIKO")->_data;
        STATUS_COMPARE(v, ((s32*)work->_parameter->_data)[7]);
        sprintf(buf, "%d", v);
        STATUS_TEXTSET(self, "STWINFONT06", "font045", buf);
        STATUS_COLOR(self, "STWINFONT06", "font045");
        v = beDataObjList_getInt(pl, "KOORITEIKO")->_data;
        STATUS_COMPARE(v, ((s32*)work->_parameter->_data)[8]);
        sprintf(buf, "%d", v);
        STATUS_TEXTSET(self, "STWINFONT06", "font046", buf);
        STATUS_COLOR(self, "STWINFONT06", "font046");
        v = beDataObjList_getInt(pl, "KAMINARITEIKO")->_data;
        STATUS_COMPARE(v, ((s32*)work->_parameter->_data)[9]);
        sprintf(buf, "%d", v);
        STATUS_TEXTSET(self, "STWINFONT06", "font047", buf);
        STATUS_COLOR(self, "STWINFONT06", "font047");
        v = beDataObjList_getInt(pl, "YAMITEIKO")->_data;
        STATUS_COMPARE(v, ((s32*)work->_parameter->_data)[10]);
        sprintf(buf, "%d", v);
        STATUS_TEXTSET(self, "STWINFONT06", "font048", buf);
        STATUS_COLOR(self, "STWINFONT06", "font048");
        return true;
    }
    case 5: {
        beNDMWStatusInfoWork* work = ram->_work;
        s32 who = beMessenger_getInt(self->_messenger, 0);
        beDataObjList* pl;
        s32 hp;
        s32 mp;
        char buf[16];

        sprintf(buf, "%d", ((s32*)work->_parameter->_data)[0]);
        STATUS_TEXTSET(self, "STWINFONT06", "font010", buf);
        beMessenger_selectGroup(self->_messenger, "TEXTCTL");
        beMessenger_pushString(self->_messenger, "STWINFONT06");
        beMessenger_pushString(self->_messenger, "font010");
        beMessenger_send(self->_messenger, "DEFAULTCOLOR");
        sprintf(buf, "%d", ((s32*)work->_parameter->_data)[1]);
        STATUS_TEXTSET(self, "STWINFONT06", "font011", buf);
        beMessenger_selectGroup(self->_messenger, "TEXTCTL");
        beMessenger_pushString(self->_messenger, "STWINFONT06");
        beMessenger_pushString(self->_messenger, "font011");
        beMessenger_send(self->_messenger, "DEFAULTCOLOR");
        sprintf(buf, "%d", ((s32*)work->_parameter->_data)[2]);
        STATUS_TEXTSET(self, "STWINFONT06", "font040", buf);
        beMessenger_selectGroup(self->_messenger, "TEXTCTL");
        beMessenger_pushString(self->_messenger, "STWINFONT06");
        beMessenger_pushString(self->_messenger, "font040");
        beMessenger_send(self->_messenger, "DEFAULTCOLOR");
        sprintf(buf, "%d", ((s32*)work->_parameter->_data)[3]);
        STATUS_TEXTSET(self, "STWINFONT06", "font041", buf);
        beMessenger_selectGroup(self->_messenger, "TEXTCTL");
        beMessenger_pushString(self->_messenger, "STWINFONT06");
        beMessenger_pushString(self->_messenger, "font041");
        beMessenger_send(self->_messenger, "DEFAULTCOLOR");
        sprintf(buf, "%d", ((s32*)work->_parameter->_data)[4]);
        STATUS_TEXTSET(self, "STWINFONT06", "font042", buf);
        beMessenger_selectGroup(self->_messenger, "TEXTCTL");
        beMessenger_pushString(self->_messenger, "STWINFONT06");
        beMessenger_pushString(self->_messenger, "font042");
        beMessenger_send(self->_messenger, "DEFAULTCOLOR");
        sprintf(buf, "%d", ((s32*)work->_parameter->_data)[5]);
        STATUS_TEXTSET(self, "STWINFONT06", "font043", buf);
        beMessenger_selectGroup(self->_messenger, "TEXTCTL");
        beMessenger_pushString(self->_messenger, "STWINFONT06");
        beMessenger_pushString(self->_messenger, "font043");
        beMessenger_send(self->_messenger, "DEFAULTCOLOR");
        sprintf(buf, "%d", ((s32*)work->_parameter->_data)[6]);
        STATUS_TEXTSET(self, "STWINFONT06", "font044", buf);
        beMessenger_selectGroup(self->_messenger, "TEXTCTL");
        beMessenger_pushString(self->_messenger, "STWINFONT06");
        beMessenger_pushString(self->_messenger, "font044");
        beMessenger_send(self->_messenger, "DEFAULTCOLOR");
        sprintf(buf, "%d", ((s32*)work->_parameter->_data)[7]);
        STATUS_TEXTSET(self, "STWINFONT06", "font045", buf);
        beMessenger_selectGroup(self->_messenger, "TEXTCTL");
        beMessenger_pushString(self->_messenger, "STWINFONT06");
        beMessenger_pushString(self->_messenger, "font045");
        beMessenger_send(self->_messenger, "DEFAULTCOLOR");
        sprintf(buf, "%d", ((s32*)work->_parameter->_data)[8]);
        STATUS_TEXTSET(self, "STWINFONT06", "font046", buf);
        beMessenger_selectGroup(self->_messenger, "TEXTCTL");
        beMessenger_pushString(self->_messenger, "STWINFONT06");
        beMessenger_pushString(self->_messenger, "font046");
        beMessenger_send(self->_messenger, "DEFAULTCOLOR");
        sprintf(buf, "%d", ((s32*)work->_parameter->_data)[9]);
        STATUS_TEXTSET(self, "STWINFONT06", "font047", buf);
        beMessenger_selectGroup(self->_messenger, "TEXTCTL");
        beMessenger_pushString(self->_messenger, "STWINFONT06");
        beMessenger_pushString(self->_messenger, "font047");
        beMessenger_send(self->_messenger, "DEFAULTCOLOR");
        sprintf(buf, "%d", ((s32*)work->_parameter->_data)[10]);
        STATUS_TEXTSET(self, "STWINFONT06", "font048", buf);
        beMessenger_selectGroup(self->_messenger, "TEXTCTL");
        beMessenger_pushString(self->_messenger, "STWINFONT06");
        beMessenger_pushString(self->_messenger, "font048");
        beMessenger_send(self->_messenger, "DEFAULTCOLOR");
        hp = ((s32*)work->_parameter->_data)[11];
        mp = ((s32*)work->_parameter->_data)[12];
        beMessenger_selectGroup(self->_messenger, "GAMERAM");
        beMessenger_pushInt(self->_messenger, who);
        beMessenger_send(self->_messenger, "GETPLAYER");
        pl = (beDataObjList*)beMessenger_getObject(self->_messenger, 0);
        beDataObjList_getInt(pl, "HP")->_data = hp;
        beDataObjList_getInt(pl, "MP")->_data = mp;
        return true;
    }
    case 3: {
        beNDMWStatusInfoWork* work = ram->_work;

        switch (beMessenger_getInt(self->_messenger, 0)) {
        case 6:
            beMessenger_pushObject(self->_messenger, work->_weaponSalesList);
            break;
        case 7:
            beMessenger_pushObject(self->_messenger, work->_armorSalesList);
            break;
        case 8:
            beMessenger_pushObject(self->_messenger, work->_chipSalesList);
            break;
        case 9:
            beMessenger_pushObject(self->_messenger, work->_junkSalesList);
            beMessenger_pushInt(self->_messenger, work->_junkGift);
            break;
        }
        return true;
    }
    default:
        return false;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/beNDMWStatus", beNDMWStatus_handle);
#endif
