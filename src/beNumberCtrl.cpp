#include "game.h"

extern "C" {
extern char D_004E0858[]; /* "" */
extern char D_0050FEB0[]; /* "NUMBERCTRL" */
void beMessenger_entry(beMessenger* m, beBaseInfoRam* ram, const char* group, const char* info, s32 ramNo);
extern igMetaObject* beNumberCtrlInfo_Meta;
extern igMetaObject* beNumberCtrl_Meta;
extern bool D_00554C14;
extern void* D_00587110;
void* func_00301000(void* o);
void* func_002F88E0(void* p);
igObject* func_0034E300(void);
beNumberCtrlInfoWork* func_0034D2B0(void* pool); /* new beNumberCtrlInfoWork */
void func_003A4B20(beNumberCtrl* mgr, beNumberCtrlInfoRam* ram, s32 v, s32 flag);
}

IG_REF(beNumberCtrlInfoWork);

static inline void* PoolOf2(void* o) {
    void* a = func_00301000(o);

    return D_00554C14 ? a : func_002F88E0(D_00587110);
}

extern "C" void beNumberCtrl_prepare(void) {
}

igMetaObject* beNumberCtrl_getInfoMeta(void) {
    return beNumberCtrlInfo_Meta;
}

#ifdef NON_MATCHING
/* Nonmatching: 16 off; mgr/ram save order and s-reg use of the switch value differ */
void beNumberCtrl_updateRam(beNumberCtrl* mgr, beNumberCtrlInfoRam* ram) {
    s32 disp;
    s32 real;
    s32 change;
    s32 v;
    beNumberCtrlInfoWork* work;

    switch (ram->_st0) {
    default:
        if (ram->_work == NULL) {
            beNumberCtrlInfoWorkRef w(func_0034D2B0(PoolOf2(mgr)));

            work = w.p;
            igUnref(ram->_work);
            ram->_work = work;
        }
        ram->_st0++;
        ram->_st3 = 0;
        ram->_st2 = 0;
        ram->_st1 = 0;
        /* fallthrough */
    case 1:
        work = ram->_work;
        real = work->_realNum;
        disp = work->_dispNum;
        if (disp != real) {
            change = work->_changeNum;
            v = disp + change;
            if (change > 0) {
                if (real < v) {
                    v = real;
                }
            } else if (change < 0) {
                if (v < real) {
                    v = real;
                }
            } else {
                v = real;
            }
            func_003A4B20(mgr, ram, v, 0);
            work->_dispNum = v;
        }
        break;
    case 2:
    case 3:
        break;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/beNumberCtrl", beNumberCtrl_updateRam);
#endif

INCLUDE_ASM("asm/nonmatchings/beNumberCtrl", func_003A4B20);

/* v / 128 plus one step away from zero, capped at +-99, skipping multiples of 10. */
s32 func_003A4E40(beNumberCtrl* mgr, s32 v) {
    s32 r = 0;

    if (v > 0) {
        r = v / 128 + 1;
        if (r >= 100) {
            r = 99;
        }
        if (r % 10 == 0) {
            r++;
        }
    } else if (v < 0) {
        r = v / 128 - 1;
        if (r < -99) {
            r = -99;
        }
        if (r % 10 == 0) {
            r--;
        }
    }
    return r;
}

static inline const char* NameOf(igNamedObject* o) {
    return o->_name ? o->_name : D_004E0858;
}

void beNumberCtrl_entry(beNumberCtrl* mgr, beBaseInfoRam* ram) {
    s32 no = ram->_ramNo;
    beMessenger_entry(mgr->_messenger, ram, D_0050FEB0, NameOf(ram->_parentInfo), no);
}

s32 beNumberCtrl_handle(beNumberCtrl* mgr, beNumberCtrlInfoRam* ram, beMessengerWork* msg) {
    beNumberCtrlInfoWork* work;
    s32 v;
    s32 cur;
    const char* name = msg->_message;

    beMessenger_getMessageIndex(mgr->_messenger, func_0034E300(), name);
    work = ram->_work;
    switch (beMessenger_getMessageIndex(mgr->_messenger, func_0034E300(), name)) {
    case 0:
        if (work != NULL) {
            beMessenger_pushInt(mgr->_messenger, work->_realNum);
        }
        return 1;
    case 1:
        if (work == NULL) {
            beNumberCtrlInfoWorkRef w(func_0034D2B0(PoolOf2(mgr)), IG_ADOPT);

            igAssign(ram->_work, w.p);
            work = w.p;
        }
        if (beMessenger_getIntCount(mgr->_messenger) == 0) {
            return 1;
        }
        v = beMessenger_getInt(mgr->_messenger, 0);
        if (v < 0) {
            v = 0;
        }
        if (ram->_maxValue < v) {
            v = ram->_maxValue;
        }
        func_003A4B20(mgr, ram, v, 0);
        work->_dispNum = v;
        work->_realNum = v;
        work->_changeNum = 0;
        return 1;
    case 3:
        if (work == NULL) {
            beNumberCtrlInfoWorkRef w(func_0034D2B0(PoolOf2(mgr)), IG_ADOPT);

            igAssign(ram->_work, w.p);
            work = w.p;
        }
        if (beMessenger_getIntCount(mgr->_messenger) == 0) {
            return 1;
        }
        cur = work->_realNum;
        work->_realNum = cur + beMessenger_getInt(mgr->_messenger, 0);
        if (work->_realNum < 0) {
            work->_realNum = 0;
        }
        if (ram->_maxValue < work->_realNum) {
            work->_realNum = ram->_maxValue;
        }
        work->_changeNum = func_003A4E40(mgr, work->_realNum - work->_dispNum);
        return 1;
    default:
        return 0;
    }
}

igMetaObject* beNumberCtrl_getMeta(void) {
    return beNumberCtrl_Meta;
}
