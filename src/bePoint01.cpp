#include "game.h"

/* bePoint01: player points (spawn / warp positions), each info a switch and
   a list of groups holding the point transforms (MWCC C++). */
extern "C" {

extern igMetaObject* bePoint01Info_Meta;
extern igMetaObject* bePoint01_Meta;
extern char D_004E0858[];        /* "" */
extern char D_0050F6E0[];        /* "PLAYERPOINT" */
extern const igMatrix44f D_004E0EF0; /* identity */
igObject* func_0034B980(void);   /* message table */
void func_001B1CF0(igBitMask* mask); /* switch: all children on */
void func_001B1DC0(igBitMask* mask); /* switch: all children off */
void func_001908F0(igMatrix44f* dst, const igMatrix44f* src); /* copy */
void bePoint01_pushPoint(bePoint01* mgr, bePoint01Info* info, s32 group, s32 point);

/* -- */

void bePoint01_prepare(bePoint01* mgr) {
}

igMetaObject* bePoint01_getInfoMeta(void) {
    return bePoint01Info_Meta;
}

void bePoint01_entry(bePoint01* mgr, beBaseInfoRam* ram) {
    beMessenger_entryRam(mgr->_messenger, ram, D_0050F6E0);
}

void bePoint01_updateRam(bePoint01* mgr) {
}

/* Push the matrix of point `point` of group `group` (group 0 if out of
   range; the identity if the group is empty). */
void bePoint01_pushPoint(bePoint01* mgr, bePoint01Info* info, s32 group, s32 point) {
    beGroupKeepList* list = info->_dataList;
    beGroupKeep* g;

    if (list->_count - 1 < group) {
        group = 0;
    }
    g = ((beGroupKeep**)list->_data)[group];
    if (g != NULL) {
        beMessenger_pushMatrix(mgr->_messenger, &((igTransform**)g->_childList->_data)[point]->_m);
    } else {
        igMatrix44f m;

        func_001908F0(&m, &D_004E0EF0);
        beMessenger_pushMatrix(mgr->_messenger, &m);
    }
}

bool bePoint01_handle(bePoint01* mgr, beBaseInfoRam* ram, beMessengerWork* work) {
    bePoint01Info* info = (bePoint01Info*)ram->_parentInfo;

    switch (beMessenger_getMessageIndex(mgr->_messenger, func_0034B980(), work->_message)) {
    case 0:
        func_001B1CF0(info->_switch->_childMask);
        return true;
    case 1:
        func_001B1DC0(info->_switch->_childMask);
        return true;
    case 2: {
        s32 group = mgr->_pointNo;

        bePoint01_pushPoint(mgr, info, group, beMessenger_getInt(mgr->_messenger, 1));
        return true;
    }
    case 3: {
        s32 group = beMessenger_getInt(mgr->_messenger, 0);

        bePoint01_pushPoint(mgr, info, group, beMessenger_getInt(mgr->_messenger, 1));
        return true;
    }
    case 4: {
        beGroupKeepList* list = info->_dataList;
        s32 n = list->_count;
        s32 i;

        for (i = 0; i < n; i++) {
            beGroupKeep* g = ((beGroupKeep**)list->_data)[i];

            beMessenger_pushString(mgr->_messenger, g->_name ? g->_name : D_004E0858);
        }
        return true;
    }
    case 5:
        mgr->_pointNo = beMessenger_getInt(mgr->_messenger, 0);
        return true;
    case 6:
        beMessenger_pushInt(mgr->_messenger, mgr->_pointNo);
        return true;
    }
    return false;
}

igMetaObject* bePoint01_getMeta(void) {
    return bePoint01_Meta;
}

}
