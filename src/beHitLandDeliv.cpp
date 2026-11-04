#include "game.h"

/* beHitLandDeliv: hit / landing delivery manager (MWCC C++). */
extern "C" {

extern igMetaObject* beHitLandDeliv_Meta;
extern igMetaObject* beHitLandDelivInfo_Meta;
extern char D_0050EA40[]; /* "HITLANDDELIVER" */

}

/* The virtual called by func_00386030. */
class HitTargetV {
public:
    IG_VPAD_22
    virtual void call(void* arg); /* +0x60 */
};

struct HitTraversal {
    u8 pad0[0x40];
    HitTargetV* _target; /* 0x40 */
};

extern "C" {

/* -- */

void beHitLandDeliv_prepare(beHitLandDeliv* mgr) {
}

igMetaObject* beHitLandDeliv_getInfoMeta(void) {
    return beHitLandDelivInfo_Meta;
}

void beHitLandDeliv_entry(beHitLandDeliv* mgr, beBaseInfoRam* ram) {
    beMessenger_entryRam(mgr->_messenger, ram, D_0050EA40);
}

void beHitLandDeliv_updateRam(beHitLandDeliv* mgr) {
}

INCLUDE_ASM("asm/nonmatchings/beHitLandDeliv", beHitLandDeliv_unhandled);

igMetaObject* beHitLandDeliv_getMeta(void) {
    return beHitLandDeliv_Meta;
}

s32 func_00386030(HitTraversal* t, void* node, void* arg) {
    t->_target->call(arg);
    return 0;
}

}
