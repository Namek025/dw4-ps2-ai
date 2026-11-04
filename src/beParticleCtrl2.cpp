#include "game.h"

extern "C" {
extern char D_004E0858[]; /* "" */
extern char D_0050F638[]; /* "MODELCTRL" */
void beMessenger_entry(beMessenger* m, beBaseInfoRam* ram, const char* group, const char* info, s32 ramNo);
extern igMetaObject* beParticleCtrl2Info_Meta;
extern igMetaObject* beParticleCtrl2_Meta;
void func_001DC8D0(igSwitch* sw); /* all children off */
void func_001DC820(igSwitch* sw, s32 n);
void func_001DC880(igSwitch* sw, s32 n);
void func_00190380(igMatrix44f* m);                                     /* identity */
void func_001908F0(igMatrix44f* dst, const igMatrix44f* src);           /* copy */
void func_00190A50(igMatrix44f* m, const igVec3f* v);                   /* translate */
}

#ifdef NON_MATCHING
/* igTransform's virtual used by updateRam. */
class igTransformMatV {
public:
    IG_VPAD_35
    virtual void setMatrix(igMatrix44f* m); /* +0x94 */
};
#endif

void beParticleCtrl2_prepare(beParticleCtrl2* mgr) {
    s32 n;
    s32 i;

    n = mgr->_infoList->_count;
    for (i = 0; i < n; i++) {
        beParticleCtrl2Info* info = ((beParticleCtrl2Info**)mgr->_infoList->_data)[i];
        s32 c;
        s32 j;

        func_001DC8D0(info->_mainSwitch);
        func_001DC8D0(info->_subSwitch);
        c = info->_ramList->_count;
        for (j = 0; j < c; j++) {
            beParticleCtrl2InfoRam* ram = ((beParticleCtrl2InfoRam**)info->_ramList->_data)[j];

            ram->_isRun = false;
            igUnref(ram->_work);
            ram->_work = NULL;
            ram->_particleTimer->_timeRunning = false;
        }
    }
}

#ifdef NON_MATCHING
/* Nonmatching: 30 off; the stray nop before case 2, the switch scrutinee in a2 (asm a0) and the loop's bool test (sltu) differ. */
void beParticleCtrl2_updateRam(beParticleCtrl2* mgr, beParticleCtrl2InfoRam* ram) {
    beParticleCtrl2Info* info = (beParticleCtrl2Info*)ram->_parentInfo;
    igMatrix44f m;
    beParticleCtrl2InfoWork* work;
    bool found;
    s32 count;
    u32 i;

    if (ram->_isRun) {
        work = ram->_work;
        if (work != NULL) {
            func_00190380(&m);
            func_001908F0(&m, &work->_positionLockNode->_m);
            func_00190A50(&m, &work->_positionLockOffset);
            ((igTransformMatV*)ram->_transform)->setMatrix(&m);
        }
        switch (ram->_st0) {
        case 0:
            func_001DC820(info->_mainSwitch, 0);
            func_001DC820(info->_subSwitch, ram->_ramNo);
            ram->_particleTimer->_time = -1;
            ram->_particleTimer->_timeRunning = true;
            ((igAlphaStateAttr**)ram->_particleTimer->_attributes->_data)[0]->_enabled = true;
            ram->_st0++;
            ram->_st3 = 0;
            ram->_st2 = 0;
            ram->_st1 = 0;
            break;
        case 1:
            if (mgr->_insight->_worldTime - ram->_particleTimer->_timeBias < info->_endTimeLong) {
                break;
            }
            ram->_st0++;
            ram->_st3 = 0;
            ram->_st2 = 0;
            ram->_st1 = 0;
            /* fall through */
        case 2:
            ram->_particleTimer->_timeRunning = false;
            func_001DC880(info->_subSwitch, ram->_ramNo);
            ram->_isRun = false;
            igUnref(ram->_work);
            ram->_work = NULL;
            count = 0;
            if (info->_subSwitch->_childList != NULL) {
                count = info->_subSwitch->_childList->_count;
            }
            found = false;
            for (i = 0; (s32)i < count; i++) {
                if ((((u32*)info->_subSwitch->_childMask->_data)[i >> 5] & (1 << (i & 31))) != 0) {
                    found = true;
                    break;
                }
            }
            if (found == false) {
                func_001DC880(info->_mainSwitch, 0);
            }
            break;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/beParticleCtrl2", beParticleCtrl2_updateRam);
#endif

igMetaObject* beParticleCtrl2_getInfoMeta(void) {
    return beParticleCtrl2Info_Meta;
}

static inline const char* NameOf(igNamedObject* o) {
    return o->_name ? o->_name : D_004E0858;
}

void beParticleCtrl2_entry(beParticleCtrl2* mgr, beParticleCtrl2InfoRam* ram) {
    s32 no = ram->_ramNo;
    beMessenger_entry(mgr->_messenger, ram, D_0050F638, NameOf(ram->_parentInfo), no);
    ram->_isRun = false;
    igUnref(ram->_work);
    ram->_work = NULL;
}

INCLUDE_ASM("asm/nonmatchings/beParticleCtrl2", beParticleCtrl2_handle);

igMetaObject* beParticleCtrl2_getMeta(void) {
    return beParticleCtrl2_Meta;
}
