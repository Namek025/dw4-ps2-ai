#include "game.h"

extern "C" {
extern char D_004E0858[]; /* "" */
extern igMetaObject* beSoundInfo_Meta;
extern igMetaObject* beSound_Meta;
extern igMetaObject* beCri_Meta;
extern char D_0050F9A0[]; /* "SOUND" */
extern char D_0050F9A8[]; /* "acx" */
void beMessenger_entry(beMessenger* m, beBaseInfoRam* ram, const char* group, const char* info, s32 ramNo);
void func_0037E020(beCri* cri, s32 vol);
}

void beSound_prepare(beSound* mgr) {
    beCri* cri;
    bePlaySEList* list;
    s32 count;
    s32 i;
    s32 limit;
    s32 off;

    if (mgr->_cri == NULL) {
        cri = (beCri*)func_00236F80(mgr->_insight, beCri_Meta);
        igIncRef(cri);
        igUnref(mgr->_cri);
        mgr->_cri = cri;
    }
    list = mgr->_playSEs;
    count = list->_count;
    for (i = 0; i < count; i++) {
        igUnref(((igObject**)list->_data)[i]);
    }
    count = list->_count;
    i = 0;
    if (0 < count) {
        if (8 < count) {
            limit = count - 8;
            off = 0;
            do {
                i += 8;
                *(s32*)((char*)list->_data + off + 0x0) = 0;
                *(s32*)((char*)list->_data + off + 0x4) = 0;
                *(s32*)((char*)list->_data + off + 0x8) = 0;
                *(s32*)((char*)list->_data + off + 0xC) = 0;
                *(s32*)((char*)list->_data + off + 0x10) = 0;
                *(s32*)((char*)list->_data + off + 0x14) = 0;
                *(s32*)((char*)list->_data + off + 0x18) = 0;
                *(s32*)((char*)list->_data + off + 0x1C) = 0;
                off += 0x20;
            } while (i < limit);
        }
        off = i * 4;
        if (i < count) {
            do {
                i++;
                *(s32*)((char*)list->_data + off) = 0;
                off += 4;
            } while (i < count);
        }
    }
    list->_count = 0;
    if ((mgr->_playSEs->_count > 0) == false) {
        func_002D6680(mgr->_playSEs, 0, 4);
    }
}

void beSound_entry(beSound* mgr, beBaseInfoRam* ram) {
    beSoundInfo* info;
    char* str;

    beMessenger_entry(mgr->_messenger, ram, D_0050F9A0, NULL, -1);
    info = (beSoundInfo*)ram->_parentInfo;
    mgr->_cri->_baseVolume = info->_baseVolume;
    func_0037E020(mgr->_cri, info->_sndfVolume);
    if (info->_dataType == NULL) {
        str = func_002EAF60(GetStrPool(), D_0050F9A8);
        ReleaseStr(info->_dataType);
        info->_dataType = str;
    }
    if (strcmp(info->_dataType, D_0050F9A8) == 0) {
        mgr->_cri->_dataType = 0;
        return;
    }
    mgr->_cri->_dataType = 1;
}

igMetaObject* beSound_getInfoMeta(void) {
    return beSoundInfo_Meta;
}

INCLUDE_ASM("asm/nonmatchings/beSound", beSound_updateRam);

INCLUDE_ASM("asm/nonmatchings/beSound", beSound_handle);

/* Out-of-line destructor of a single-pointer Ref smart pointer (deleting form). */
void** func_0039FAE0(void** r, s16 flags) {
    if (r != NULL) {
        igUnref((igObject*)*r);
        if (flags > 0) {
            operator delete(r);
        }
    }
    return r;
}

static inline const char* NameOf(igNamedObject* o) {
    return o->_name ? o->_name : D_004E0858;
}

/* Index of the info's data named `name`, or -1. */
s32 func_0039FB60(beSound* mgr, beBaseInfoRam* ram, const char* name) {
    s32 i;
    beBaseInfoDataList* list;
    s32 n;

    list = ram->_parentInfo->_dataList;
    n = list->_count;
    for (i = 0; i < n; i++) {
        if (strcmp(name, NameOf(((igNamedObject**)list->_data)[i])) == 0) {
            return i;
        }
    }
    return -1;
}

igMetaObject* beSound_getMeta(void) {
    return beSound_Meta;
}
