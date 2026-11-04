#include "game.h"

/* beCameraMode: a view mode that plays a camera along a graph path
   (MWCC C++). */
extern "C" {

extern igMetaObject* igCamera_Meta;
extern igMetaObject* beCameraMode_Meta;
extern void* D_0058CD38; /* igViewModel field: _viewMatrix */
extern void* D_0058CD48; /* igViewModel field: _hfov */
extern void* D_0058CD50; /* igViewModel field: _nearPlane */
extern void* D_0058CD58; /* igViewModel field: _farPlane */
igNode* func_001C5990(igGraphPath* path);                               /* last node */
void func_001C5B30(igGraphPath* path, igMatrix44f* out, s64 time);     /* transform at a time */
void func_00190380(igMatrix44f* m);                                     /* identity */
void func_00191230(const igMatrix44f* m, f32* x, f32* y, f32* z);      /* euler angles */
void func_00190ED0(igMatrix44f* m, f32 x, f32 y, f32 z);               /* rotation from euler angles */
void func_00190A30(const igMatrix44f* m, igVec3f* pos);                /* translation */
void func_00190A10(igMatrix44f* m, const igVec3f* pos);                /* set translation */
void func_00191E30(s32* ret, igMatrix44f* dst, const igMatrix44f* src); /* invert */
void func_004578C0(igModel* model, void* change);                      /* notify the views */

}

class igCameraV {
public:
    IG_VPAD_33
    virtual void update();    /* +0x8C */
    virtual void activate();  /* +0x90 */
};

IG_DYNAMIC_CAST(igCamera)

/* A model field changed. */
struct ModelChange {
    s32 kind;
    s32 unk4;
    void* field;
};

static inline void Changed(igModel* model, void* field) {
    ModelChange c;

    c.kind = 2;
    c.unk4 = 0;
    c.field = field;
    func_004578C0(model, &c);
}

extern "C" {

/* -- */

void beCameraMode_setPath(beCameraMode* cm, igGraphPath* path) {
    if (path != NULL) {
        igAssign(cm->_cameraPath, path);
        igAssign(cm->_camera, igCamera_cast(func_001C5990(path)));
    }
}

void beCameraMode_activate(beCameraMode* cm) {
    if (cm->_camera != NULL) {
        ((igCameraV*)cm->_camera)->activate();
    }
}

/* Copy the camera's lens to the view model. */
void beCameraMode_applyLens(beCameraMode* cm) {
    if (cm->_camera != NULL) {
        igViewModel* model;
        f32 farPlane;
        f32 nearPlane;

        ((igCameraV*)cm->_camera)->update();
        model = (igViewModel*)cm->_model;
        farPlane = cm->_camera->_farPlane;
        nearPlane = cm->_camera->_nearPlane;
        model->_hfov = cm->_camera->_horizontalFieldOfView;
        Changed(model, D_0058CD48);
        model->_farPlane = farPlane;
        Changed(model, D_0058CD58);
        model->_nearPlane = nearPlane == 0.0f ? farPlane / 1000.0f : nearPlane;
        Changed(model, D_0058CD50);
    }
}

#ifdef NON_MATCHING
/* Nonmatching: stack slots (the change record sits below the matrix in the
   original) and the operand order of the time add. */
/* Advance along the path (at most 50 ms a frame) and set the view matrix. */
void beCameraMode_update(beCameraMode* cm) {
    f32 x;
    f32 y;
    f32 z;
    s32 ret;
    igVec3f pos;
    igMatrix44f m;
    igViewModel* model;

    if (cm->_camera == NULL) {
        return;
    }
    if (cm->_isFreeze) {
        cm->_svWorldTimeL = cm->_insight->_worldTime;
        return;
    }
    if (!cm->_isEnd) {
        s64 now = cm->_insight->_worldTime;
        s64 dt = cm->_svWorldTimeL ? now - cm->_svWorldTimeL : 0;

        if (dt > 50000000) {
            dt = 50000000;
        }
        cm->_nowTimeL = dt + cm->_nowTimeL;
        cm->_svWorldTimeL = now;
        if (cm->_nowTimeL >= cm->_timeLength) {
            cm->_isEnd = true;
            cm->_nowTimeL = cm->_timeLength;
        }
    }
    func_00190380(&m);
    model = (igViewModel*)cm->_model;
    func_001C5B30(cm->_cameraPath, &m, cm->_nowTimeL);
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    func_00191230(&m, &x, &y, &z);
    func_00190A30(&m, &pos);
    func_00190ED0(&m, x, y, z);
    func_00190A10(&m, &pos);
    func_00191E30(&ret, &model->_viewMatrix, &m);
    Changed(model, D_0058CD38);
}
#else
INCLUDE_ASM("asm/nonmatchings/beCameraMode", beCameraMode_update);
#endif

igMetaObject* beCameraMode_getMeta(void) {
    return beCameraMode_Meta;
}

}
