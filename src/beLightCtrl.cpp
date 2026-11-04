#include "game.h"

/* beLightCtrl: sets light colors on request (MWCC C++). */
extern "C" {

extern igMetaObject* beLightCtrlInfo_Meta;
extern igMetaObject* beLightCtrl_Meta;
extern igMetaObject* D_00585BE0; /* meta used for lookups by name */
extern char D_00510838[];        /* "LIGHTCTRL" */
igObject* func_0035BFB0(void);   /* message table */
igNamedObject* func_003022C0(igObjectList* list, igMetaObject* meta, const char* name); /* find by name */
void beMessenger_getVec3f(igVec3f* out, beMessenger* m, s32 i);

}

class igLightV {
public:
    IG_VPAD_30
    virtual void setDiffuse(const igVec4f* c);  /* +0x80 */
    virtual void setAmbient(const igVec4f* c);  /* +0x84 */
    virtual void setSpecular(const igVec4f* c); /* +0x88 */
};

static inline void SetRGB(igVec4f& d, const igVec3f& s) {
    d.x = s.x;
    d.y = s.y;
    d.z = s.z;
}

extern "C" {

/* -- */

void beLightCtrl_prepare(beLightCtrl* mgr) {
}

igMetaObject* beLightCtrl_getInfoMeta(void) {
    return beLightCtrlInfo_Meta;
}

void beLightCtrl_entry(beLightCtrl* mgr, beBaseInfoRam* ram) {
    beMessenger_entryRam(mgr->_messenger, ram, D_00510838);
}

void beLightCtrl_updateRam(beLightCtrl* mgr, beBaseInfoRam* ram) {
}

/* Set the ambient, diffuse and specular colors of a light set's first light. */
bool beLightCtrl_handle(beLightCtrl* mgr, beBaseInfoRam* ram, beMessengerWork* work) {
    beLightCtrlInfo* info = (beLightCtrlInfo*)ram->_parentInfo;

    switch (beMessenger_getMessageIndex(mgr->_messenger, func_0035BFB0(), (const char*)work)) {
    case 0: {
        const char* name = beMessenger_getString(mgr->_messenger, 0);
        igVec3f ambient;
        igVec3f diffuse;
        igVec3f specular;
        igLightSet* set;

        beMessenger_getVec3f(&ambient, mgr->_messenger, 0);
        beMessenger_getVec3f(&diffuse, mgr->_messenger, 1);
        beMessenger_getVec3f(&specular, mgr->_messenger, 2);
        set = (igLightSet*)func_003022C0(info->_lightSetList, D_00585BE0, name);
        if (set != NULL) {
            igLightV* light = ((igLightV**)set->_lights->_data)[0];
            igVec4f c;

            c.w = 1.0f;
            SetRGB(c, ambient);
            light->setAmbient(&c);
            SetRGB(c, diffuse);
            light->setDiffuse(&c);
            SetRGB(c, specular);
            light->setSpecular(&c);
        }
        return true;
    }
    }
    return false;
}

igMetaObject* beLightCtrl_getMeta(void) {
    return beLightCtrl_Meta;
}

}
