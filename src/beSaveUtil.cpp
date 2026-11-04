#include "game.h"

extern "C" {
extern char D_00510738[]; /* "SAVEUTILITY" */
extern igMetaObject* beSaveUtilInfo_Meta;
extern igMetaObject* beSaveUtil_Meta;
void __dl__FPv(void* p);
}

void beSaveUtil_prepare(beSaveUtil* mgr) {
    if (mgr->_api != NULL) {
        igUnref(mgr->_api);
        mgr->_api = NULL;
    }
    if (mgr->_connectCheck != NULL) {
        igUnref(mgr->_connectCheck);
        mgr->_connectCheck = NULL;
    }
}

INCLUDE_ASM("asm/nonmatchings/beSaveUtil", beSaveUtil_updateRam);

extern "C" {

void** func_003AF010(void** arg0, s16 arg1) {
    igObject* o;

    if (arg0 != NULL) {
        o = (igObject*)*arg0;
        igUnref(o);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

}

extern "C" {

void** func_003AF090(void** arg0, s16 arg1) {
    igObject* o;

    if (arg0 != NULL) {
        o = (igObject*)*arg0;
        igUnref(o);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

}

igMetaObject* beSaveUtil_getInfoMeta(void) {
    return beSaveUtilInfo_Meta;
}

void beSaveUtil_entry(beSaveUtil* mgr, beBaseInfoRam* ram) {
    beMessenger_entryRam(mgr->_messenger, ram, D_00510738);
}

INCLUDE_ASM("asm/nonmatchings/beSaveUtil", beSaveUtil_handle);

extern "C" {

void** func_003AFAF0(void** arg0, s16 arg1) {
    igObject* o;

    if (arg0 != NULL) {
        o = (igObject*)*arg0;
        igUnref(o);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

}

extern "C" {

void** func_003AFB70(void** arg0, s16 arg1) {
    igObject* o;

    if (arg0 != NULL) {
        o = (igObject*)*arg0;
        igUnref(o);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

}

extern "C" {

void** func_003AFBF0(void** arg0, s16 arg1) {
    igObject* o;

    if (arg0 != NULL) {
        o = (igObject*)*arg0;
        igUnref(o);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

}

extern "C" {

void** func_003AFC70(void** arg0, s16 arg1) {
    igObject* o;

    if (arg0 != NULL) {
        o = (igObject*)*arg0;
        igUnref(o);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

}

extern "C" {

void** func_003AFCF0(void** arg0, s16 arg1) {
    igObject* o;

    if (arg0 != NULL) {
        o = (igObject*)*arg0;
        igUnref(o);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

}

extern "C" {

void** func_003AFD70(void** arg0, s16 arg1) {
    igObject* o;

    if (arg0 != NULL) {
        o = (igObject*)*arg0;
        igUnref(o);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

}

extern "C" {

void** func_003AFDF0(void** arg0, s16 arg1) {
    igObject* o;

    if (arg0 != NULL) {
        o = (igObject*)*arg0;
        igUnref(o);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

}

igMetaObject* beSaveUtil_getMeta(void) {
    return beSaveUtil_Meta;
}
