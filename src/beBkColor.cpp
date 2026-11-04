#include "game.h"

/* beBkColor: sets the screen clear color from its info, once (MWCC C++). */
extern "C" {

extern igMetaObject* beBkColorInfo_Meta;
extern igMetaObject* beBkColor_Meta;

}

class igVisualContextV {
public:
    IG_VPAD_175
    virtual void setClearColor(const igVec4f* rgba); /* +0x2C4 */
};

class igWindowV {
public:
    IG_VPAD_36
    virtual igVisualContextV* getVisualContext(); /* +0x98 */
};

extern "C" {

/* -- */

void beBkColor_prepare(beBkColor* mgr) {
    mgr->_isClear = false;
}

igMetaObject* beBkColor_getInfoMeta(void) {
    return beBkColorInfo_Meta;
}

void beBkColor_updateRam(beBkColor* mgr, beBaseInfoRam* ram) {
    if (!mgr->_isClear) {
        beBkColorInfo* info = ((beBkColorInfo**)mgr->_infoList->_data)[0];
        igVisualContextV* vc = ((igWindowV*)mgr->_insight->_window)->getVisualContext();

        vc->setClearColor(&info->_rgba);        mgr->_isClear = true;
    }
}

igMetaObject* beBkColor_getMeta(void) {
    return beBkColor_Meta;
}

}
