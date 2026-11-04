#include "game.h"

/* beCameraCtrl: camera manager (MWCC C++). */
extern "C" {
void func_00236BE0(igInsightCore* insight, void* o);
void func_00236C40(igInsightCore* insight, void* o);
void func_00378ED0(beCameraCtrl* mgr, beBaseInfoRam* ram);
void func_00379A90(beCameraCtrl* mgr, beBaseInfoRam* ram);

extern igMetaObject* beCameraCtrl_Meta;
extern igMetaObject* beCameraCtrlInfo_Meta;
extern igMetaObject* beCameraBoxData_Meta;
extern igMetaObject* beCameraDemoData_Meta;
extern char D_0050E1F0[]; /* "CAMERA" */
void func_00462DC0(igViewerSceneInfoManager* m);

}

/* igObject's virtuals. */
class igObjectV {
public:
    IG_VPAD_20
    virtual igMetaObject* getMeta(); /* +0x58 */
};

/* A camera info data entry: its object at +0x1C. */
struct CameraData {
    u8 pad0[0x1C];
    igObjectV* _obj; /* 0x1C */
};

extern "C" {

/* -- */

igMetaObject* beCameraCtrl_getInfoMeta(void) {
    return beCameraCtrlInfo_Meta;
}

void beCameraCtrl_entry(beCameraCtrl* mgr, beBaseInfoRam* ram) {
    beMessenger_entryRam(mgr->_messenger, ram, D_0050E1F0);
}

void beCameraCtrl_prepare(beCameraCtrl* mgr) {
    mgr->_cameraNow = 0;
    mgr->_cameraOld = 0;
    if (mgr->_viewerMang != NULL) {
        func_00462DC0(mgr->_viewerMang);
    }
    igUnref(mgr->_cameraMode);
    mgr->_cameraMode = NULL;
}

/* Is this data a camera box? */
bool func_00378E40(void* unused, CameraData* d) {
    igMetaObject* meta = beCameraBoxData_Meta;

    return meta == d->_obj->getMeta();
}

/* Is this data a demo camera? */
bool func_00378E80(void* unused, CameraData* d) {
    igMetaObject* meta = beCameraDemoData_Meta;

    return meta == d->_obj->getMeta();
}

void func_00378EC0(void) {
}

INCLUDE_ASM("asm/nonmatchings/beCameraCtrl", func_00378ED0);

INCLUDE_ASM("asm/nonmatchings/beCameraCtrl", func_00379A90);

void beCameraCtrl_updateRam(beCameraCtrl* mgr, beBaseInfoRam* ram) {
    if (mgr->_isFirst) {
        /* first update: unregister and register again with the insight (update order?) */
        func_00236C40(mgr->_insight, mgr);
        func_00236BE0(mgr->_insight, mgr);
        mgr->_isFirst = false;
    }
    if (ram->_isRun) {
        if (func_00378E40(mgr, (CameraData*)ram->_parentInfo)) {
            func_00378ED0(mgr, ram);
        } else {
            func_00379A90(mgr, ram);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/beCameraCtrl", beCameraCtrl_unhandled);

/* Out-of-line destructor of an igObject smart pointer (deleting form). */
igObject** func_0037A600(igObject** r, s16 flags) {
    if (r != NULL) {
        igUnref(*r);
        if (flags > 0) {
            operator delete(r);
        }
    }
    return r;
}

igMetaObject* beCameraCtrl_getMeta(void) {
    return beCameraCtrl_Meta;
}

}
