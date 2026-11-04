#include "game.h"

/* beCri: CRI ADX sound streams (BGM, environment, sound effects) (MWCC C++). */
extern "C" {

extern igMetaObject* beCri_Meta;
extern igMetaObject* beCriFxInfo_Meta;
extern igMetaObject* igAdxFile_Meta;
extern igMetaObject* igFile_Meta;
extern void* D_0058A350; /* igFile's abstract proxy before beCri replaced it */
extern s32 D_005875E8;
void func_00236C40(igInsightCore* insight, void* o);
void func_0037CC50(beCri* cri, s32 n);
void func_003A6B50(beCriAudio* a);
void func_0037D4A0(beCri* cri);
void func_00332D20(s32 a);
void func_00333230(s32 a);
void func_003332B0(s32 a, s32 b, s32 c);
void func_00332E90(s32 a, s32 b);
void func_002825E0(s32 a, s32 b, s32 c);
void func_0037E3A0(void* o, s32 kind, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern igResource* D_00586DD0; /* the resource manager */
extern igMetaObject* beCriSfpData_Meta;
extern igMetaObject* igMemoryRefMetaField_Meta;
void func_00308860(igResource* res, const char* name, s32 align);
void func_002FB280(igMemoryRefMetaField* f, const char* name);
extern char D_004E0858[]; /* "" */
extern char D_0050E480[];
void func_002829F0(u32 a, s32 b);
igMetaField* func_002FDF20(igMetaObject* meta, const char* name); /* field by name */
s32 func_0037D680(beCri* cri, beCriSfpData* sfp);
extern s32 D_0058A460[];

}

IG_DYNAMIC_CAST(igMemoryRefMetaField)

/* igObject's virtuals. */
class igObjectV {
public:
    IG_VPAD_20
    virtual igMetaObject* getMeta(); /* +0x58 */
};

/* Owner of func_0037E110's request flag. */
struct CriReq {
    u8 pad0[0x20];
    bool _busy; /* 0x20 */
};

/* Owner of func_0037EA50's pending object. */
struct CriPend {
    u8 pad0[0x18];
    igObject* _obj;       /* 0x18 */
    igObjectList* _list;  /* 0x1C */
    bool _keep;           /* 0x20 */
};

IG_REF(igStringObj);

struct CriArkCore {
    u8 pad0[0x38];
    void* _cfg; /* 0x38 */
};

extern "C" {

/* -- */

void func_00236BE0(igInsightCore* insight, void* o);
s32 func_00306360(void* a, const char* name, s32 z);
s32 func_00306770(void* a, s32 idx, const char* name, void* out, void* def, s32 z);
igStringObj* func_002B9500(void* pool);
void func_003327C0(char* s);
void func_00332810(s32 a);
void func_00332C50(s32 a);
s32 strcmp(const char* a, const char* b);
void func_0030B3C0(igStringObj* so, const char* s);
igMetaObject* func_0037B950(void);
extern bool D_00554C14;
extern bool D_00554DE4;
extern void* D_00587110;
extern char* D_004C22F0;
extern s32 D_005873C8;
extern char D_0050E400[];
extern char D_0050E410[];
extern char D_0050E420[];
extern char D_0050E428[];
extern char D_0050E438[];
extern char D_0050E440[];
extern char D_0050E460[];
extern CriArkCore* D_00586DC8;

void beCri_registerToInsight(beCri* cri) {
    s32 idx;
    void* cfg;

    func_00236BE0(cri->_insight, cri);
    cfg = D_00586DC8->_cfg;
    if (cfg != NULL) {
        idx = func_00306360(cfg, D_0050E400, 0);
        if (idx != -1) {
            void* a = func_00301000(cri);
            igStringObjRef so(func_002B9500(D_00554C14 ? a : func_002F88E0(D_00587110)), IG_ADOPT);

            if (func_00306770(cfg, idx, D_0050E410, &so.p, D_0050E420, 0) == 0) {
                if (strcmp(D_0050E428, D_0050E438) == 0) {
                    func_0030B3C0(so.p, D_0050E440);
                } else {
                    func_0030B3C0(so.p, D_0050E460);
                }
            }
            func_003327C0(so.p->_buffer == NULL ? D_004C22F0 : (char*)so.p->_buffer);
            D_00554DE4 = true;
            func_00332810(0);
            D_005873C8 = 2;
            func_00332C50(1);
            D_0058A350 = igFile_Meta->_abstractProxy;
            igFile_Meta->_abstractProxy = (void*)func_0037B950;
        }
    }
}

igMetaObject* func_0037B950(void) {
    return igAdxFile_Meta;
}

void beCri_unRegisterFromInsight(beCri* cri) {
    igFile_Meta->_abstractProxy = D_0058A350;
    func_00236C40(cri->_insight, cri);
}

extern void* D_00587108;
extern s32 D_0058A360[];
igObject* func_00366910(void* pool);
void func_003A6950(beCriAudio* a, s32 v);
void func_0037D310(beCri* cri);
extern igMetaObject* beSystem_Meta;

void beCri_begin(beCri* cri) {
    s32 i;
    void* a;
    void* pool;

    cri->_system = (beSystem*)func_00236F80(cri->_insight, beSystem_Meta);
    a = func_00301000(cri);
    pool = D_00554C14 ? a : func_002F88E0(D_00587108);
    igUnref((igObject*)cri->_hdBGM);
    cri->_hdBGM = (beCriHandle*)func_00366910(pool);
    func_003A6950(cri->_hdBGM->_audio, 2);
    func_002FECB0(cri->_hdBGM, 0);
    a = func_00301000(cri);
    pool = D_00554C14 ? a : func_002F88E0(D_00587108);
    igUnref((igObject*)cri->_hdENV);
    cri->_hdENV = (beCriHandle*)func_00366910(pool);
    func_003A6950(cri->_hdENV->_audio, 2);
    func_002FECB0(cri->_hdENV, 0);
    func_0037D310(cri);
    cri->_pauseOnetime = false;
    for (i = 0; i < 32; i++) {
        D_0058A360[i] = 0;
        D_0058A360[i + 32] = 0;
        D_0058A460[i] = 0;
    }
}

void beCri_end(beCri* cri) {
    func_0037CC50(cri, 5);
    func_003A6B50(cri->_hdBGM->_audio);
    func_003A6B50(cri->_hdENV->_audio);
    func_0037D4A0(cri);
}

bool beCri_appendInfo(beCri* cri, igObject* info) {
    igMetaObject* meta = beCriFxInfo_Meta;

    if (meta == ((igObjectV*)info)->getMeta()) {
        func_00301E30((igObjectList*)cri->_infoList, info);
        return true;
    }
    return false;
}

void func_0037D870(beCri* cri, s32 a);
void func_0037DC60(beCri* cri, const char* name);

void beCri_removeAllInfos(beCri* cri) {
    igInfoList* list;
    s32 n;
    s32 i;
    s32 m;
    s32 j;

    func_0037DC60(cri, D_0050E480);
    list = cri->_infoList;
    n = list->_count;
    for (i = 0; i < n; i++) {
        igUnref(((igObject**)list->_data)[i]);
    }
    m = list->_count;
    for (j = 0; j < m; j++) {
        ((igObject**)list->_data)[j] = NULL;
    }
    list->_count = 0;
    if ((cri->_infoList->_count > 0) == false) {
        func_002D6680(cri->_infoList, 0, 4);
    }
    func_0037D870(cri, 0);
}

INCLUDE_ASM("asm/nonmatchings/beCri", beCri_update);

void func_0037CAF0(beCri* cri, beCriHandle* hd);

#ifdef NON_MATCHING
/* Nonmatching: second loop guard compiles to blez instead of slt+beqz; loop index/pointer regs differ */
s32 func_0037C130(beCri* cri, bool kind, s16 prio) {
    beCriHandleList* list = cri->_hdSEs;
    s32 i;
    s32 n;
    s32 best;
    s64 bestID;
    s16 bestPrio;
    beCriHandle* hd;
    beCriAudio* au;

    n = list->_count;
    i = 0;
    if (n > 0) {
        do {
            hd = ((beCriHandle**)list->_data)[i];
            au = hd->_audio;
            if (kind == true && au->_adxt == 0) goto next1;
            if (kind == false && au->_sndf == 0) goto next1;
            if (hd->_playID == 0) return i;
next1:
            i++;
        } while (i < *(volatile s32*)&list->_count);
    }
    bestID = cri->_request;
    bestPrio = 0x7FFF;
    i = 0;
    if (0 < n) {
        do {
            hd = ((beCriHandle**)list->_data)[i];
            au = hd->_audio;
            if (kind == true && au->_adxt == 0) goto next2;
            if (kind == false && au->_sndf == 0) goto next2;
            if (prio >= hd->_priority && hd->_playID < bestID && bestPrio >= hd->_priority) {
                bestID = hd->_playID;
                bestPrio = hd->_priority;
                best = i;
            }
next2:
            i++;
        } while (i < *(volatile s32*)&list->_count);
    }
    if (best != -1) {
        func_0037CAF0(cri, ((beCriHandle**)list->_data)[best]);
    }
    return best;
}
#else
INCLUDE_ASM("asm/nonmatchings/beCri", func_0037C130);
#endif

struct CriPlayReq {
    u8 pad0[8];
    const char* _name; /* 0x8 */
    u8 padC[4];
    s16 _kind;         /* 0x10 */
    s16 _slot;         /* 0x12 */
    s32 _flag;         /* 0x14 */
    u8 pad18[2];
    s16 _volume;       /* 0x1A */
};

s32 func_003A70C0(beCriAudio* a);
s32 func_003A6C00(beCriAudio* a, const char* name);
void func_003A6F70(beCriAudio* a, s32 v);
void func_003A7260(beCriAudio* a, s32 v);
void func_003A7590(beCriVolume* v, s32 vol, f32 x, f32 y, f32 z);

s64 func_0037C2C0(beCri* cri, beCriHandleList* list, CriPlayReq* req) {
    beCriHandle* hd;
    beCriAudio* au;
    beCriVolume* vol;
    const char* nm;
    const char* nm2;
    s64 id;

    if (req->_flag != -1) {
        return 0;
    }
    hd = NULL;
    au = NULL;
    vol = NULL;
    switch (req->_kind) {
    case 4:
    case 3:
        return 0;
    case 1:
        hd = cri->_hdBGM;
        au = hd->_audio;
        vol = hd->_volume;
        break;
    case 2:
        hd = cri->_hdENV;
        au = hd->_audio;
        vol = hd->_volume;
        break;
    }
    if (func_003A70C0(au) == 1) {
        if (vol->_isExec == 0) {
            func_003A7590(vol, cri->_baseVolume, 1.0f, 0.0f, 10.0f);
        }
        return 0;
    }
    vol->_max = req->_volume;
    nm = ((igNamedObject**)list->_data)[req->_slot]->_name;
    nm = nm ? nm : D_004E0858;
    func_003A7260(au, 1);
    func_003A6F70(au, vol->_max + cri->_baseVolume);
    if (func_003A6C00(au, nm) & 0xFF) {
        if (cri->_pauseOnetime) {
            cri->_pauseOnetime = false;
        } else {
            func_003A7260(au, 0);
        }
        id = cri->_request + 1;
        cri->_request = id;
        hd->_playID = id;
        hd->_type = req->_kind;
        nm2 = req->_name;
        nm2 = nm2 ? nm2 : D_004E0858;
        func_002FECB0(hd, nm2);
        return id;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/beCri", func_0037C4C0);

void func_003A6F70(beCriAudio* a, s32 v);
void func_003A6E00(beCriAudio* a);

void func_0037CAF0(beCri* cri, beCriHandle* hd) {
    beCriAudio* audio = hd->_audio;
    beCriVolume* vol = hd->_volume;

    hd->_playID = 0;
    hd->_type = 0;
    vol->_isExec = 0;
    vol->_max = 0;
    vol->_current = 0.0f;
    vol->_sta = 0.0f;
    vol->_end = 0.0f;
    if (audio->_audioType == 3) {
        func_003A6F70(audio, 0);
    } else {
        func_003A6F70(audio, -999);
    }
    func_003A6E00(audio);
    func_002FECB0(hd, 0);
}

void func_0037CB80(beCri* cri, s64 id) {
    s32 i;

    if (id == cri->_hdBGM->_playID) {
        func_0037CAF0(cri, cri->_hdBGM);
        return;
    }
    if (id == cri->_hdENV->_playID) {
        func_0037CAF0(cri, cri->_hdENV);
        return;
    }
    for (i = 0; i < cri->_hdSEs->_count; i++) {
        beCriHandle* hd = ((beCriHandle**)cri->_hdSEs->_data)[i];
        if (id == hd->_playID) {
            func_0037CAF0(cri, hd);
        }
    }
}

void func_0037CC50(beCri* cri, s32 n) {
    s32 i;

    switch (n) {
    case 1:
        func_0037CAF0(cri, cri->_hdBGM);
        break;
    case 2:
        func_0037CAF0(cri, cri->_hdENV);
        break;
    case 3:
    case 4:
        for (i = 0; i < cri->_hdSEs->_count; i++) {
            func_0037CAF0(cri, ((beCriHandle**)cri->_hdSEs->_data)[i]);
        }
        break;
    case 5:
    default:
        func_0037CAF0(cri, cri->_hdBGM);
        func_0037CAF0(cri, cri->_hdENV);
        for (i = 0; i < cri->_hdSEs->_count; i++) {
            func_0037CAF0(cri, ((beCriHandle**)cri->_hdSEs->_data)[i]);
        }
        break;
    }
}

void func_003A7260(beCriAudio* a, s32 v);
s32 strlen(const char* s);

#define NM(h) ((h)->_name ? (h)->_name : D_004E0858)

void func_0037CD90(beCri* cri, const char* name) {
    s32 i;
    beCriHandle* hd;

    if (strcmp(NM(cri->_hdBGM), name) == 0) {
        func_003A7260(cri->_hdBGM->_audio, 0);
        return;
    }
    if (strcmp(NM(cri->_hdENV), name) == 0) {
        func_003A7260(cri->_hdENV->_audio, 0);
        return;
    }
    for (i = 0; i < cri->_hdSEs->_count; i++) {
        hd = ((beCriHandle**)cri->_hdSEs->_data)[i];
        if (NM(hd) != NULL && strlen(NM(hd)) != 0 && strcmp(NM(hd), name) == 0) {
            func_003A7260(hd->_audio, 0);
        }
    }
}

void func_0037CF30(beCri* cri, const char* name, f32 x, f32 y, f32 z) {
    s32 i;
    beCriHandle* hd;

    if (NM(cri->_hdBGM) != NULL && strcmp(NM(cri->_hdBGM), name) == 0) {
        func_003A7590(cri->_hdBGM->_volume, cri->_baseVolume, x, y, z);
        return;
    }
    if (NM(cri->_hdENV) != NULL && strcmp(NM(cri->_hdENV), name) == 0) {
        func_003A7590(cri->_hdENV->_volume, cri->_baseVolume, x, y, z);
        return;
    }
    for (i = 0; i < cri->_hdSEs->_count; i++) {
        hd = ((beCriHandle**)cri->_hdSEs->_data)[i];
        if (NM(hd) != NULL && strlen(NM(hd)) != 0 && strcmp(name, NM(hd)) == 0) {
            func_003A7590(hd->_volume, cri->_baseVolume, x, y, z);
        }
    }
}

void func_003A7590(beCriVolume* v, s32 vol, f32 x, f32 y, f32 z);

void func_0037D160(beCri* cri, s64 id, f32 x, f32 y, f32 z) {
    s32 i;

    if (id == cri->_hdBGM->_playID) {
        func_003A7590(cri->_hdBGM->_volume, cri->_baseVolume, x, y, z);
        return;
    }
    if (id == cri->_hdENV->_playID) {
        func_003A7590(cri->_hdENV->_volume, cri->_baseVolume, x, y, z);
        return;
    }
    for (i = 0; i < cri->_hdSEs->_count; i++) {
        beCriHandle* hd = ((beCriHandle**)cri->_hdSEs->_data)[i];
        if (id == hd->_playID) {
            func_003A7590(hd->_volume, cri->_baseVolume, x, y, z);
        }
    }
}

void func_0037D270(beCri* cri, s32 a, s32 b) {
    func_00332D20(a);
    func_00333230(b);
}

void func_0037D2A0(beCri* cri, s32 n, s32 v) {
    if (n < 0 || n >= D_005875E8) {
        return;
    }
    func_003332B0(n, v, 0);
}

void func_0037D2F0(beCri* cri) {
    func_00332E90(-1, 0);
}

extern bool D_00554C14;
extern void* D_00587108;
igObject* func_00366910(void* pool); /* beCriHandle instantiate */
void func_003A6950(beCriAudio* a, s32 v);

}

struct CriHandleRef {
    beCriHandle* p;
    CriHandleRef(igObject* o) : p((beCriHandle*)o) {}
    ~CriHandleRef() { igUnref((igObject*)p); }
};

extern "C" {

/* Recreate the sound-effect handles: 4 with type 1, then 48 with -1. */
void func_0037D310(beCri* cri) {
    s32 i;

    if (cri->_hdSEs->_count > 0) {
        func_0037D4A0(cri);
    }
    for (i = 0; i < 4; i++) {
        void* a = func_00301000(cri);
        CriHandleRef hd(func_00366910(D_00554C14 ? a : func_002F88E0(D_00587108)));

        func_003A6950(hd.p->_audio, 1);
        func_002FECB0(hd.p, 0);
        func_00301E30((igObjectList*)cri->_hdSEs, (igObject*)hd.p);
    }
    for (i = 0; i < 48; i++) {
        void* a = func_00301000(cri);
        CriHandleRef hd(func_00366910(D_00554C14 ? a : func_002F88E0(D_00587108)));

        func_003A6950(hd.p->_audio, -1);
        func_002FECB0(hd.p, 0);
        func_00301E30((igObjectList*)cri->_hdSEs, (igObject*)hd.p);
    }
}

/* Stop and release every sound-effect handle. */
void func_0037D4A0(beCri* cri) {
    func_0037CC50(cri, 3);
    while (cri->_hdSEs->_count > 0) {
        beCriHandleList* list;

        func_003A6B50(((beCriHandle**)cri->_hdSEs->_data)[0]->_audio);
        list = cri->_hdSEs;
        igUnref(((igObject**)list->_data)[0]);
        func_002D6CF0(list, 0);
        ((igObject**)list->_data)[list->_count] = NULL;
    }
}

s32 func_003A70C0(beCriAudio* a);

/* Pause (on) or resume every playing sound-effect handle of type 4. */
void func_0037D570(beCri* cri, s32 on) {
    s32 i;

    for (i = 0; i < cri->_hdSEs->_count; i++) {
        beCriHandle* hd = ((beCriHandle**)cri->_hdSEs->_data)[i];
        if (hd->_playID != 0 && hd->_type == 4) {
            beCriAudio* audio = hd->_audio;
            if (func_003A70C0(audio) == 1) {
                if (on) {
                    func_003A6F70(audio, audio->_audioType == 3 ? 0x7F : 0);
                } else {
                    func_003A6F70(audio, audio->_audioType == 3 ? 0 : -999);
                }
            }
        }
    }
}

extern s32 D_0058A360[]; /* table size per port */
extern s32 D_0058A3E0[]; /* pcm size per port */
s32 func_00280F60(s32* tbl);
s32 func_00281078(s32 port, void* data);
void func_002813C8(s32 port);

/* Transfer one sfp data to its port. */
s32 func_0037D680(beCri* cri, beCriSfpData* sfp) {
    s32 port;

    if (sfp->_isTransfer) {
        return 1;
    }
    port = sfp->_portNo;
    if (D_0058A360[port] != 0 || D_0058A3E0[port] != 0) {
        func_002813C8(port);
    }
    D_0058A360[port] = sfp->_tblSize;
    D_0058A3E0[port] = sfp->_pcmSize;
    if (func_00280F60(D_0058A360) < 0) {
        return 0;
    }
    if (func_00281078(port, sfp->_dataBuffer) < 0) {
        return 0;
    }
    D_0058A460[port] = 1;
    return 1;
}

/* Assign port numbers to the sfp data; transfer the first one not yet transferred. */
bool func_0037D780(beCri* cri) {
    bool ok;
    igObjectList* list;
    bool first;
    s32 i;

    ok = true;
    list = cri->_lsBinObjs;
    if (cri->_dataType == 1) {
        first = true;
        for (i = 0; i < list->_count; i++) {
            beCriSfpData* sfp = ((beCriSfpData**)list->_data)[i];
            if (sfp->_isTransfer == false) {
                sfp->_portNo = i;
                if (first) {
                    if (func_0037D680(cri, sfp) == 0) {
                        ok = false;
                    }
                    first = false;
                } else {
                    D_0058A460[i] = 1;
                }
            } else {
                D_0058A460[i] = 2;
            }
        }
    }
    return ok;
}

INCLUDE_ASM("asm/nonmatchings/beCri", func_0037D870);

void func_002ED460(void* p);
s32 func_00281CF8(s32 port);

/* Poll the transfer state of every port; free finished buffers and start the next one. */
bool func_0037DAF0(beCri* cri) {
    bool ok;
    s32 i;

    ok = true;
    for (i = 0; i < cri->_lsBinObjs->_count; i++) {
        if (D_0058A460[i] != 0 && D_0058A460[i] != 2) {
            D_0058A460[i] = func_00281CF8(i);
            switch (D_0058A460[i]) {
            case 1:
                return false;
            case 2: {
                beCriSfpData* sfp = ((beCriSfpData**)cri->_lsBinObjs->_data)[i];
                igObjectList* list;

                func_002ED460(sfp->_dataBuffer);
                sfp->_dataBuffer = NULL;
                sfp->_isTransfer = true;
                list = cri->_lsBinObjs;
                if (i < list->_count - 1) {
                    beCriSfpData* next = ((beCriSfpData**)list->_data)[i + 1];
                    if (next->_isTransfer == false) {
                        func_0037D680(cri, next);
                    }
                }
                break;
            }
            case 3:
                ok = false;
                break;
            default:
                ok = false;
                break;
            }
        }
    }
    return ok;
}

/* Set the reverb of the first fx info if its name matches (NULL = "NONE"), else 127. */
void func_0037DC60(beCri* cri, const char* name) {
    igInfoList* list = cri->_infoList;
    beCriFxInfo* fx;

    if (list->_count > 0) {
        fx = ((beCriFxInfo**)list->_data)[0];
        if (name == NULL) {
            name = D_0050E480;
        }
        if (strcmp(name, fx->_name ? fx->_name : D_004E0858) == 0) {
            func_002829F0(0xA4000000, fx->_reverb);
        } else {
            func_002829F0(0xA4000000, 0x7F);
        }
    } else {
        func_002829F0(0xA4000000, 0x7F);
    }
}

INCLUDE_ASM("asm/nonmatchings/beCri", func_0037DD10);

/* Set the sound-file volume once (1..127, anything else is 127). */
void func_0037E020(beCri* cri, s32 vol) {
    if (cri->_sndfVolume == 0) {
        if (vol < 1 || vol > 127) {
            vol = 0x7F;
        }
        cri->_sndfVolume = vol;
        func_002825E0(0, 0xA0010000, vol);
    }
}

/* Register the "sndfBuffer" alignment and set it on beCriSfpData::_dataBuffer. */
void func_0037E070(void) {
    igMemoryRefMetaField* f;

    func_00308860(D_00586DD0, "sndfBuffer", 0x20);
    f = igMemoryRefMetaField_cast(func_002FDF20(beCriSfpData_Meta, "_dataBuffer"));
    func_002FB280(f, "sndfBuffer");
}

igMetaObject* beCri_getMeta(void) {
    return beCri_Meta;
}

void func_0037E110(CriReq* o, s32 a, s32 b, s32 c, s32 d, s32 e) {
    if (o->_busy == false) {
        o->_busy = true;
    } else {
        return;
    }
    func_0037E3A0(o, 3, a, b, c, d, e, 0);
}

/* Out-of-line destructor of an igObject smart pointer (deleting form). */
igObject** func_0037E250(igObject** r, s16 flags) {
    if (r != NULL) {
        igUnref(*r);
        if (flags > 0) {
            operator delete(r);
        }
    }
    return r;
}

extern bool D_00554C14;
extern void* D_00587110;
igObject* func_00363C20(void* pool);

}

struct CriTempRef {
    igObject* p;
    CriTempRef(igObject* o) : p(o) {}
    ~CriTempRef() { igUnref(p); }
};

struct CriMsg {
    u8 pad0[0x8];
    s32 unk8;
    u8 padC[0x14];
    s32 unk20;
    u8 pad24[0x4];
    char* unk28;
};

struct CriQueue {
    u8 pad0[0xC];
    igObjectList* unkC;
};

struct CriSender {
    u8 pad0[0x18];
    CriQueue* unk18;
    u8 pad1C[0x4];
    bool unk20;
};

extern "C" {

void func_0037E180(CriSender* o) {
    CriQueue* q;

    if (o->unk18 != NULL) {
        if (o->unk20 == true) {
            o->unk20 = false;
        } else {
            return;
        }
        q = o->unk18;
        {
            void* a = func_00301000(o);
            CriTempRef msg(func_00363C20(D_00554C14 ? a : func_002F88E0(D_00587110)));

            ((CriMsg*)msg.p)->unk8 = 5;
            func_00301E30(q->unkC, msg.p);
        }
    }
}

void func_0037E2D0(CriSender* o, s32 v) {
    CriQueue* q = o->unk18;

    if (q != NULL && o->unk20) {
        void* a = func_00301000(o);
        CriTempRef msg(func_00363C20(D_00554C14 ? a : func_002F88E0(D_00587110)));

        ((CriMsg*)msg.p)->unk8 = 4;
        ((CriMsg*)msg.p)->unk20 = v;
        func_00301E30(q->unkC, msg.p);
    }
}

INCLUDE_ASM("asm/nonmatchings/beCri", func_0037E3A0);

CriQueue* func_003637C0(void* pool);

void func_0037E6E0(CriSender* o, const char* str) {
    CriQueue* q;
    void* a;

    if (o->unk18 == NULL) {
        void* b = func_00301000(o);
        void* pool = D_00554C14 ? b : func_002F88E0(D_00587110);

        igUnref((igObject*)o->unk18);
        o->unk18 = func_003637C0(pool);
        o->unk20 = false;
    }
    q = o->unk18;
    a = func_00301000(o);
    {
        CriTempRef msg(func_00363C20(D_00554C14 ? a : func_002F88E0(D_00587110)));
        CriMsg* m;
        char* ns;

        ((CriMsg*)msg.p)->unk8 = 7;
        m = (CriMsg*)msg.p;
        ns = func_002EAF60(GetStrPool(), str);
        ReleaseStr(m->unk28);
        m->unk28 = ns;
        func_00301E30(q->unkC, msg.p);
    }
}

void func_0037E890(CriSender* o, const char* str, s32 v) {
    void* a;
    char* ns;
    CriQueue* q;
    CriMsg* m;

    if (o->unk18 == NULL) {
        void* b = func_00301000(o);
        void* pool = D_00554C14 ? b : func_002F88E0(D_00587110);

        igUnref((igObject*)o->unk18);
        o->unk18 = func_003637C0(pool);
        o->unk20 = false;
    }
    q = o->unk18;
    a = func_00301000(o);
    {
        CriTempRef msg(func_00363C20(D_00554C14 ? a : func_002F88E0(D_00587110)));

        ((CriMsg*)msg.p)->unk8 = 6;
        m = (CriMsg*)msg.p;
        ns = func_002EAF60(GetStrPool(), str);
        ReleaseStr(m->unk28);
        m->unk28 = ns;
        ((CriMsg*)msg.p)->unk20 = v;
        func_00301E30(q->unkC, msg.p);
    }
}

void func_0037EA50(CriPend* o) {
    if (o->_obj != NULL && o->_keep != true) {
        func_00301E30(o->_list, o->_obj);
        igUnref(o->_obj);
        o->_obj = NULL;
        o->_keep = false;
    }
}

}
