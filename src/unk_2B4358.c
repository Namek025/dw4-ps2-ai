#include "common.h"

typedef struct {
    u8 pad0[0x4];
    s32 unk4;  /* bits per entry: 0x10, 0x18 or 0x20 */
    u8 pad8[0x34];
    f32 unk3C;
    f32 unk40;
    void (*unk44)(u32* arg0, u16* arg1, f32 arg2, f32 arg3);
} UnkStruct_2B4B40;

typedef struct {
    u8 pad0[0x4];
    s32 unk4;
    u8 pad8[0x20];
    UnkStruct_2B4B40* unk28;
} UnkStruct_002B50C8;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} UnkStruct_2B4358_8;

typedef struct {
    s32 unk0;
    s32 unk4;
    UnkStruct_2B4358_8 unk8;
    s32 unk14;
    UnkStruct_2B4358_8 unk18;
    s32 unk24;
    UnkStruct_2B4358_8 unk28;
} UnkStruct_2B4358;

extern s32 D_004BF280;
extern u8 D_004F59C0[];
extern void* volatile D_005856A8;
extern s32 D_004BF014;

void* func_002B5138(void);

void func_002B4380(UnkStruct_2B4358* arg0);
void func_002B4648(s32 arg0, s32 arg1, s32 arg2, s32* arg3);

extern char D_004F5A40[];
extern char D_004F5A58[];
extern char D_004F5990[];
extern char D_004F58F0[];
s32 func_001236E8(const char* str);
void func_002B3B38(s32 arg0, s32 arg1, const char* msg, ...);
void func_002B4498(s32 arg0, s32 arg1, s32* arg2, s32* arg3);
void func_002B47D8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_002B4C98(UnkStruct_002B50C8* arg0, u32* arg1, s32 arg2, s32 arg3);
unsigned long func_002B5758(const char* arg0, char** arg1, s32 arg2);
s32 func_002B52D8(const char* arg0, s32 arg1);
void func_002B50C8(UnkStruct_002B50C8 *arg0, s32 arg1);
s32 func_002B4B10(f32 arg0, f32 arg1);

extern char D_004F5A50[];
extern char D_004F5A20[];
extern volatile s32 D_004BF290;
extern s32 D_004BF294;
extern s32 D_004BF298;
extern s32* D_004F5A70[];

extern char D_004F59F8[];
extern char D_004F5A08[];
extern char D_004F5A10[];
extern char D_004F5A18[];
extern char D_004F58B0[];
char* strncpy(char* dst, const char* src, u32 n);
char* strcat(char* dst, const char* src);
s32 sprintf(char* buf, const char* fmt, ...);
void func_0027D6B0(s32* in, void* arg1, void* arg2, s32* out);
void func_002B51F8(char* arg0, s32* arg1);

typedef struct {
    const char* s[2];
} StrPair;
extern StrPair D_004F5A30;
extern StrPair D_004F5A48;
extern StrPair D_004F5A60;
extern char D_004F5A68[];
extern s32 D_004F5870[];
extern s32 D_004F5878[];
extern s32 D_004F5880[];
s32 memcmp(const void* a, const void* b, u32 n);
s32 func_002B5838(s32 arg0);

extern char D_004F5888[];
extern char D_004F5890[];
extern char D_004F5898[];
extern char D_004F58A0[];
void* memset(void* dst, s32 c, u32 n);
s32 sscanf(const char* str, const char* fmt, ...);
s64 func_0022E440(s64 arg0, s32 arg1);
void func_002B45B0(s32 arg0, void* arg1, void* arg2, s32* arg3, s32* arg4);
void func_002B4960(UnkStruct_2B4B40* arg0, u32* arg1, u16* arg2);
void func_002B48D0(s32 arg0, u32* arg1, u16* arg2);

void func_002B4358(UnkStruct_2B4358* arg0, s32 arg1, s32 arg2) {
    UnkStruct_2B4358_8* p = &arg0->unk8;

    p->unk0 = 1;
    p->unk4 = arg1;
    p->unk8 = arg2;
    func_002B4380(arg0);
}

INCLUDE_ASM("asm/nonmatchings/unk_2B4358", func_002B4380);

void func_002B4468(UnkStruct_2B4358* arg0, s32* arg1, s32* arg2) {
    UnkStruct_2B4358_8* p = &arg0->unk8;

    if (p->unk0 != 1) {
        *arg1 = 0;
        *arg2 = 0;
    } else {
        s32 a = p->unk4;
        s32 b = p->unk8;

        *arg2 = b;
        *arg1 = a;
    }
}

void func_002B4498(s32 arg0, s32 arg1, s32* arg2, s32* arg3) {
    s32 ptr;
    s32 len;
    s64 size;
    s64 unused;
    s64 a;
    s64 b;
    char* str;

    func_002B45B0(arg0, D_004F5888, D_004F5878, &ptr, &len);
    if (ptr == 0) {
        *arg2 = 0;
        *arg3 = 0x7FFFFFFF;
        return;
    }
    sscanf((char*)ptr, D_004F5890, &size);
    func_002B45B0(arg0, D_004F5898, D_004F5878, &ptr, &len);
    str = (char*)ptr;
    if (str == NULL) {
        *arg2 = 0x7FFFFFFF;
        return;
    }
    sscanf(str + (s32)func_0022E440(size, arg1), D_004F58A0, &unused, &a, &b);
    *arg2 = a;
    *arg3 = b;
}

INCLUDE_ASM("asm/nonmatchings/unk_2B4358", func_002B45B0);

void func_002B4630(s32 arg0, s32 arg1, s32 arg2, s32* arg3) {
    func_002B4648(arg0, arg1, arg2, arg3);
}

INCLUDE_ASM("asm/nonmatchings/unk_2B4358", func_002B4648);

void func_002B4788(s32 arg0, s32 arg1, s32 arg2) {
    s32 a;
    s32 b;

    func_002B4498(arg0, arg1, &a, &b);
    func_002B47D8(arg0, a, b, arg2);
}

void func_002B47D8(s32 arg0_, s32 arg1, s32 arg2, s32 arg3_) {
    UnkStruct_2B4B40* arg0 = (UnkStruct_2B4B40*)arg0_;
    u16* arg3 = (u16*)arg3_;
    u32* tbl = (u32*)((u8*)arg3 + 0x400);

    memset(arg3, 0, 0x400);
    func_002B4630((s32)arg0, arg1, arg2, (s32*)tbl);
    if (arg0->unk44 == NULL) {
        if (arg0->unk4 == 0x10) {
            func_002B48D0((s32)arg0, tbl, arg3);
        } else {
            func_002B4960(arg0, tbl, arg3);
        }
    } else {
        arg0->unk44(tbl, arg3, arg0->unk3C, arg0->unk40);
    }
}

void func_002B48D0(s32 arg0, u32* arg1, u16* arg2) {
    s32 i;

    if (func_002B4B98() == 1) {
        for (i = 0; i < 0x100; i++) {
            arg2[i] = arg1[i] >> 15;
        }
    } else {
        func_002B3B38(0, 0, D_004F58B0);
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_2B4358", func_002B4960);

void func_002B4AA8(UnkStruct_2B4B40* arg0, f32 arg1, f32 arg2) {
    if (func_002B4B10(arg1, arg2) != 1) {
        func_002B3B38(0, 0, D_004F58F0);
    }
    arg0->unk3C = arg1;
    arg0->unk40 = arg2;
}

s32 func_002B4B10(f32 arg0, f32 arg1) {
    s32 ret = 1;

    if (arg1 <= arg0) {
        ret = 0;
    }
    if (arg1 - arg0 < -14.0f) {
        ret = 0;
    }
    return ret;
}

void func_002B4B40(UnkStruct_2B4B40* arg0, f32* arg1, f32* arg2) {
    *arg1 = arg0->unk3C;
    *arg2 = arg0->unk40;
}

s32 func_002B4B58(UnkStruct_2B4B40* arg0) {
    f32 x = arg0->unk3C;
    f32 y = arg0->unk40;
    s32 ret = 1;

    if (x == 0.0f && y == 0.0f) {
        ret = 0;
    }
    return ret;
}

void func_002B4B88(s32 arg0) {
    D_004BF014 = arg0;
}

s32 func_002B4B98(void) {
    return D_004BF014;
}

void func_002B4BA8(UnkStruct_002B50C8* arg0, u32* arg1, s32 arg2) {
    func_002B50C8(arg0, 0x10);
    func_002B4C98(arg0, arg1, 0x10, arg2);
}

void func_002B4BF8(UnkStruct_002B50C8* arg0, u32* arg1, s32 arg2) {
    func_002B50C8(arg0, 0x18);
    func_002B4C98(arg0, arg1, 0x18, arg2);
}

void func_002B4C48(UnkStruct_002B50C8* arg0, u32* arg1, s32 arg2) {
    func_002B50C8(arg0, 0x20);
    func_002B4C98(arg0, arg1, 0x20, arg2);
}

INCLUDE_ASM("asm/nonmatchings/unk_2B4358", func_002B4C98);

INCLUDE_ASM("asm/nonmatchings/unk_2B4358", func_002B4DC8);

INCLUDE_ASM("asm/nonmatchings/unk_2B4358", func_002B4F58);

s32 func_002B5068(u32 arg0) {
    s32 ret;

    switch (arg0) {
    case 0x10: ret = 1; break;
    case 0x18: ret = 2; break;
    case 0x20: ret = 3; break;
    case 0x8:
    default:
        func_002B3B38(0, 0, D_004F5990);
        ret = 0;
    }
    return ret;
}

void func_002B50C8(UnkStruct_002B50C8 *arg0, s32 arg1) {
arg0->unk28->unk4 = arg1;
}

s32 func_002B50D8(UnkStruct_002B50C8 *arg0) {
return arg0->unk28->unk4;
}

void func_002B50E8(void) {
    if (D_004BF280 <= 0) {
        D_005856A8 = func_002B5138();
        D_004BF280++;
    }
}

void* func_002B5138(void) {
    return D_004F59C0;
}

void func_002B5148(void) {
    if (D_004BF280 > 0) {
        D_004BF280--;
    }
}

void func_002B5168(char* arg0, s32 arg1, s32* arg2) {
    char buf[16];

    strncpy(arg0, D_004F59F8, 8);
    sprintf(buf, D_004F5A08, 0x13);
    strcat(arg0, buf);
    arg0 += 0x10;
    strncpy(arg0, D_004F5A10, 2);
    func_002B51F8(arg0, arg2);
    strcat(arg0, D_004F5A18);
}

void func_002B51F8(char* arg0, s32* arg1) {
    StrPair t0 = D_004F5A30;
    StrPair t1 = D_004F5A48;
    StrPair t2 = D_004F5A60;

    strncpy(arg0 + 2, t0.s[arg1[0]], 1);
    strncpy(arg0 + 3, t1.s[arg1[1]], 1);
    strncpy(arg0 + 4, t2.s[arg1[2]], 1);
}

typedef struct { s8 c[2]; } Chars2;
s32 func_002B52D8(const char* arg0, s32 arg1) {
    Chars2 buf;

    if (arg0 == NULL || arg1 < 0) {
        return 0;
    }
    buf = *(Chars2*)(arg0 + 0x10);
    return func_001236E8((char*)&buf);
}

extern char D_004F5A28[];

s32 strncmp(const char *arg0, const char *arg1, s32 arg2);

s32 func_002B5320(const char *arg0, s32 arg1) {
    if (arg0 == NULL || arg1 < 0) {
        return 0;
    }
    return strncmp(arg0 + 0x12, D_004F5A28, 1) == 0;
}

s32 func_002B5360(const char *arg0, s32 arg1) {
    if (arg0 == NULL || arg1 < 0) {
        return 0;
    }
    return strncmp(arg0 + 0x13, D_004F5A40, 1) == 0;
}

s32 func_002B53A0(const char* arg0, s32 arg1) {
    if (arg0 == NULL || arg1 < 0) {
        return 0;
    }
    if (func_002B52D8(arg0, arg1) == 0) {
        return 0;
    }
    return strncmp(arg0 + 0x14, D_004F5A58, 1) == 0;
}

s32 func_002B5400(const char* arg0, s32 arg1) {
    const char* p;
    s32 ret;

    if (arg0 == NULL || arg1 < 0) {
        return 0;
    }
    if ((u32)func_002B52D8(arg0, arg1) < 2) {
        return 0;
    }
    p = arg0 + 0x15;
    if (strncmp(p, D_004F5A50, 1) == 0) {
        ret = 1;
    } else if (strncmp(p, D_004F5A58, 1) == 0) {
        ret = 2;
    } else {
        ret = 0;
    }
    return ret;
}

s32 func_002B5490(const char* arg0, s32 arg1) {
    if (arg0 == NULL || arg1 < 0) {
        return 0;
    }
    if ((u32)func_002B52D8(arg0, arg1) < 2) {
        return 0;
    }
    return strncmp(arg0 + 0x16, D_004F5A20, 1) != 0;
}

s32 func_002B54F8(const char* arg0, s32 arg1) {
    if (arg0 == NULL || arg1 < 0) {
        return 0;
    }
    if ((u32)func_002B52D8(arg0, arg1) < 2) {
        return 0;
    }
    return func_002B56F8(arg0, arg1, 0x17);
}

s32 func_002B5578(const char* arg0, s32 arg1) {
    if (arg0 == NULL || arg1 < 0) {
        return 0;
    }
    if ((u32)func_002B52D8(arg0, arg1) < 2) {
        return 0;
    }
    return func_002B56F8(arg0, arg1, 0x1A);
}

s32 func_002B55F8(const char* arg0, s32 arg1) {
    if (arg0 == NULL || arg1 < 0) {
        return 0;
    }
    if ((u32)func_002B52D8(arg0, arg1) < 2) {
        return 0;
    }
    return func_002B56F8(arg0, arg1, 0x1D);
}

s32 func_002B5678(const char* arg0, s32 arg1) {
    if (arg0 == NULL || arg1 < 0) {
        return 0;
    }
    if ((u32)func_002B52D8(arg0, arg1) < 2) {
        return 0;
    }
    return func_002B56F8(arg0, arg1, 0x20);
}

typedef struct { s8 c[3]; } Chars3;
s32 func_002B56F8(const char* arg0, s32 arg1, s32 arg2) {
    char buf[16];
    char* end;

    if (arg0 == NULL || arg1 < 0) {
        return 0;
    }
    *(Chars3*)buf = *(Chars3*)(arg0 + arg2);
    return func_002B5758(buf, &end, 16);
}

unsigned long strtoul(const char *arg0, char **arg1, s32 arg2);

unsigned long func_002B5758(const char *arg0, char **arg1, s32 arg2) {
    return strtoul(arg0, arg1, arg2);
}

INCLUDE_ASM("asm/nonmatchings/unk_2B4358", func_002B5770);

s32 func_002B5838(s32 arg0) {
    if (arg0 == 0) {
        return 0;
    }
    return 0x23;
}

INCLUDE_ASM("asm/nonmatchings/unk_2B4358", func_002B5848);

void func_002B59A0(void) {
    D_004BF290--;
    if (D_004BF290 == 0 && D_004BF294 != -1 && (D_004BF298 & 0xC7) != 0x82) {
        *D_004F5A70[D_004BF294] = D_004BF298 ^ 0xC00;
    }
    if (D_004BF290 < 0) {
        D_004BF290 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_2B4358", func_002B5A10);

u64 func_002B5A10(void);
extern s64 D_004BF288;

s32 func_002B5B38(void) {
    func_002B5A10();
    return D_004BF288 == 1;
}
