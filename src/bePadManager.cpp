#include "game.h"

extern "C" {
void func_0039AF10(bePadData* pad, s32 buttons);
void func_0039AF90(bePadData* pad, igVec2f* v);
void func_0039B050(bePadData* pad, igVec2f* v);
void func_004542D0(void* dispatcher, void* receiver, void* a2);
void func_00454290(void* dispatcher, void* receiver);
void func_004574F0(igHotKeyEventReceiver* r, s32 key);
void func_00462E90(igViewerSceneInfoManager* m);
igObject* func_0034CE50(void* pool); /* new bePadData */
extern igMetaObject* beSystem_Meta;
extern igMetaObject* igViewerSceneInfoManager_Meta;
extern bool D_00554C14;
extern void* D_00587108;
void func_004542E0(void* dispatcher, void* receiver, void* a2);
void func_004542A0(void* dispatcher, void* receiver);
void func_00236BE0(igInsightCore* insight, void* o);
void func_00236C40(igInsightCore* insight, void* o);
extern igMetaObject* bePadManager_Meta;
}

void bePadManager_registerToInsight(bePadManager* mgr) {
    func_00236BE0(mgr->_insight, mgr);
}

void bePadManager_unRegisterFromInsight(bePadManager* mgr) {
    func_00236C40(mgr->_insight, mgr);
}

IG_REF(bePadData);

static inline void* PoolOf(void* o) {
    void* a = func_00301000(o);

    return D_00554C14 ? a : func_002F88E0(D_00587108);
}

/* Five pads (players 1-4 and a spare), the keyboard receiver, and the viewer's hot keys. */
void bePadManager_begin(bePadManager* mgr) {
    s32 i;
    igInsightCore* insight;
    igEventDispatcher* d;
    igViewerSceneInfoManager* viewer;
    igHotKeyEventReceiver* keys;

    igAssign(mgr->_system, (beSystem*)func_00236F80(mgr->_insight, beSystem_Meta));
    for (i = 0; i < 5; i++) {
        bePadDataRef pad((bePadData*)func_0034CE50(PoolOf(mgr)), IG_ADOPT);

        func_00301E30(mgr->_padList, pad);
    }
    insight = mgr->_insight;
    d = insight->_dispatcher;
    func_004542D0(d, mgr->_kbReceiver, insight->_windowResizeReceiver);
    func_00454290(d, mgr->_kbReceiver);
    viewer = (igViewerSceneInfoManager*)func_00236F80(mgr->_insight, igViewerSceneInfoManager_Meta);
    keys = viewer->_hotKeyReceiver;
    func_004574F0(keys, 15);
    func_004574F0(keys, 3);
    func_004574F0(keys, 0);
    func_004574F0(keys, 10);
    func_004574F0(keys, 11);
    func_004574F0(keys, 1);
    func_004574F0(keys, 2);
    func_004574F0(keys, 12);
    func_00462E90(viewer);
}


void bePadManager_end(bePadManager* mgr) {
    igInsightCore* insight = mgr->_insight;
    igEventDispatcher* d = insight->_dispatcher;

    func_004542E0(d, mgr->_kbReceiver, insight->_windowResizeReceiver);
    func_004542A0(d, mgr->_kbReceiver);
}

extern "C" s32 bePadManager_appendInfo(void* mgr) {
    return 0;
}

extern "C" void bePadManager_removeAllInfos(void* mgr) {
}

/* igController's virtuals. */
class igControllerV {
public:
    IG_VPAD_28
    virtual s32 getButtons(); /* +0x78 */
    virtual void getStick(s32 i, f32* x, f32* y); /* +0x7C */
    virtual u16 getPort(); /* +0x80 */
    virtual void _v31();
    virtual bool isConnected(); /* +0x88 */
};

/* Copies every controller into the pad of its port (port / 2; the first port-1 device counts
   as port 2 when none is there). Skipped on frame-skip frames. */
void bePadManager_update(bePadManager* mgr) {
    igControllerList* list;
    bool found;
    s32 i;
    u16 port;
    igControllerV* c;
    bool on;
    u16 n;
    igVec2f st;

    if (mgr->_system->_isFrameSkip) {
        return;
    }
    found = false;
    list = mgr->_insight->_dispatcher->_controllerManager->_controllers;
    for (i = 0; i < list->_count; i++) {
        c = ((igControllerV**)list->_data)[i];
        port = c->getPort();
        on = c->isConnected();
        if (port == 2) {
            if (!on) {
                continue;
            }
            found = true;
        }
        if (port == 1) {
            if (found) {
                continue;
            }
            found = true;
            port = 2;
        }
        n = (u32)port >> 1;
        if (n < 4) {
            bePadData* pad = ((bePadData**)mgr->_padList->_data)[n];

            pad->_isConnect = on;
            if (on) {
                igVec2f l;
                igVec2f r;

                c->getStick(0, &st.x, &st.y);
                l.x = st.x;
                l.y = st.y;
                func_0039AF90(pad, &l);
                c->getStick(1, &st.x, &st.y);
                r.x = st.x;
                r.y = st.y;
                func_0039B050(pad, &r);
                func_0039AF10(pad, c->getButtons());
            } else {
                igVec2f l;
                igVec2f r;

                l.x = 0.0f;
                l.y = 0.0f;
                func_0039AF90(pad, &l);
                r.x = 0.0f;
                r.y = 0.0f;
                func_0039B050(pad, &r);
                func_0039AF10(pad, 0);
            }
        }
    }
}

/* Pad of player n (0-3); slot 4 when pads are disabled or n is out of range. */
bePadData* bePadManager_getPad(bePadManager* mgr, s32 n) {
    if (mgr->_isPadDisable) {
        return ((bePadData**)mgr->_padList->_data)[4];
    }
    if (n < 4) {
        return ((bePadData**)mgr->_padList->_data)[n];
    }
    return ((bePadData**)mgr->_padList->_data)[4];
}

igMetaObject* bePadManager_getMeta(void) {
    return bePadManager_Meta;
}
