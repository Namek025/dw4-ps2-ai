#include "game.h"

/* beTargetObj: finds a target (player or enemy) for a model, the nearest one
   or the one closest to its facing direction (MWCC C++). The search keeps its
   state in globals. */
extern "C" {

extern igMetaObject* beTargetObj_Meta;
extern igMetaObject* beMessenger_Meta;
extern char D_0050FFD0[]; /* "PLAYER" */
extern char D_0050FFD8[]; /* "GETMATRIX" */
extern char D_0050FFE8[]; /* "ENEMY" */
extern char D_0050FFF0[]; /* "GETMATRIX_ACTIVE" */
extern char D_00510008[]; /* "NEAR" */
extern const igVec3f D_004E1058; /* (0, 0, 0) */
extern const igVec3f D_004E1098; /* facing axis */

extern beTargetObj* beTargetObj_sInstance;
extern beMessenger* beTargetObj_sMessenger;
extern igVec3f beTargetObj_sSelfPos;
extern f32 beTargetObj_sSelfAngle; /* -100: not computed yet */
extern igMatrix44f beTargetObj_sSelfMatrix;
extern igVec3f beTargetObj_sTargetPos;
extern f32 beTargetObj_sTargetDist; /* squared */
extern s32 beTargetObj_sTargetRamNo;

void func_00236BE0(igInsightCore* insight, void* o);
void func_00236C40(igInsightCore* insight, void* o);
s32 beMessenger_getMatrixCount(beMessenger* m);
void beMessenger_getMatrix(igMatrix44f* out, beMessenger* m, s32 i);
void func_001908F0(igMatrix44f* dst, const igMatrix44f* src);    /* copy */
void func_00190A30(const igMatrix44f* m, igVec3f* pos);         /* translation */
void func_00192F50(igVec3f* out, const igVec3f* v, const igMatrix44f* m); /* rotate */
f32 func_00123640(f32 x, f32 z); /* atan2 */
f32 beTargetObj_getSelfAngle(void);

}

/* Alchemy's igVec3f is a class with a copy constructor: a by-value argument is
   copied by the caller into a temporary. */
struct Vec3 : igVec3f {
    Vec3(const igVec3f& v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }
    Vec3(const Vec3& v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }
};

template <class A, class B>
static inline f32 Dist2(const A& a, const B& b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
}

extern "C" {

/* -- */

void beTargetObj_registerToInsight(beTargetObj* t) {
    func_00236BE0(t->_insight, t);
}

void beTargetObj_unRegisterFromInsight(beTargetObj* t) {
    func_00236C40(t->_insight, t);
}

void beTargetObj_begin(beTargetObj* t) {
    beTargetObj_sInstance = (beTargetObj*)func_00236F80(t->_insight, beTargetObj_Meta);
    beTargetObj_sMessenger = (beMessenger*)func_00236F80(t->_insight, beMessenger_Meta);
}

void beTargetObj_end(beTargetObj* t) {
}

s32 beTargetObj_appendInfo(beTargetObj* t) {
    return 0;
}

void beTargetObj_removeAllInfos(beTargetObj* t) {
    t->_playerPosList->_count = 0;
    if ((t->_playerPosList->_count > 0) == false) {
        func_002D6680(t->_playerPosList, 0, 12);
    }
    t->_playerRamNoList->_count = 0;
    if ((t->_playerRamNoList->_count > 0) == false) {
        func_002D6680(t->_playerRamNoList, 0, 4);
    }
    t->_enemyPosList->_count = 0;
    if ((t->_enemyPosList->_count > 0) == false) {
        func_002D6680(t->_enemyPosList, 0, 12);
    }
}

/* Every frame: forget the positions (collected again on the next search). */
void beTargetObj_update(beTargetObj* t) {
    t->_playerPosList->_count = 0;
    t->_playerRamNoList->_count = 0;
    t->_enemyPosList->_count = 0;
}

/* Send message to the rams of group and collect their positions (and the
   ram numbers they answer with, if ramNos is given). */
void beTargetObj_collect(beTargetObj* t, igVec3fList* posList, igIntList* ramNos, const char* group,
                         const char* message) {
    s32 n;
    s32 i;

    beMessenger_callGroup(beTargetObj_sMessenger, group, message);
    n = beMessenger_getMatrixCount(beTargetObj_sMessenger);
    for (i = 0; i < n; i++) {
        igMatrix44f m;
        igVec3f pos;
        s32 k;

        beMessenger_getMatrix(&m, beTargetObj_sMessenger, i);
        func_00190A30(&m, &pos);
        k = posList->_count;
        if (k < posList->_capacity) {
            posList->_count = k + 1;
        } else {
            func_002D6790(posList, k + 1, 12);
        }
        ((igVec3f*)posList->_data)[k] = pos;
        if (ramNos != NULL) {
            s32 ramNo = beMessenger_getInt(beTargetObj_sMessenger, i);

            k = ramNos->_count;
            if (k < ramNos->_capacity) {
                (*(volatile s32*)&ramNos->_count)++;
            } else {
                func_002D6790(ramNos, k + 1, 4);
            }
            ((s32*)ramNos->_data)[k] = ramNo;
        }
    }
}

/* Nearest position to from. */
void beTargetObj_findNearest(beTargetObj* t, igVec3fList* list, igIntList* ramNos, Vec3 from, igVec3f* outPos,
                             f32* outDist, s32* outRamNo) {
    s32 ramNo = 0;
    s32 n = list->_count;
    s32 i;
    f32 best;
    igVec3f bestPos;

    for (i = 0; i < n; i++) {
        igVec3f* p = &((igVec3f*)list->_data)[i];
        f32 x = p->x;
        f32 y = p->y;
        f32 z = p->z;
        f32 d = (from.x - x) * (from.x - x) + (from.y - y) * (from.y - y) + (from.z - z) * (from.z - z);

        if (i == 0 || !(best <= d)) {
            best = d;
            bestPos.x = x;
            bestPos.y = y;
            bestPos.z = z;
            if (ramNos != NULL) {
                ramNo = ((s32*)ramNos->_data)[i];
            }
        }
    }
    *outPos = bestPos;
    *outDist = best;
    *outRamNo = ramNo;
}

/* Position closest to the self's facing direction, seen from from. */
void beTargetObj_findFacing(beTargetObj* t, igVec3fList* list, igIntList* ramNos, Vec3 from, igVec3f* outPos,
                            f32* outDist, s32* outRamNo) {
    f32 self = beTargetObj_getSelfAngle();
    s32 n = list->_count;
    s32 ramNo;
    s32 i;
    f32 best;
    igVec3f bestPos;

    for (i = 0; i < n; i++) {
        igVec3f* p = &((igVec3f*)list->_data)[i];
        f32 x = p->x;
        f32 y = p->y;
        f32 z = p->z;
        f32 a = func_00123640(x - from.x, z - from.z) - self;

        if (a < 0.0f) {
            a = -a;
        }
        while (a > 6.2831855f) {
            a -= 6.2831855f;
        }
        if (i == 0 || !(best <= a)) {
            best = a;
            bestPos.x = x;
            bestPos.y = y;
            bestPos.z = z;
            ramNo = ((s32*)ramNos->_data)[i];
        }
    }
    *outPos = bestPos;
    *outDist = Dist2(from, bestPos);
    *outRamNo = ramNo;
}

/* Average position of the players. */
bool beTargetObj_getPlayerCenter(igVec3f* out) {
    s32 n;
    s32 i;

    if (beTargetObj_sInstance->_playerPosList->_count == 0) {
        beTargetObj_collect(beTargetObj_sInstance, beTargetObj_sInstance->_playerPosList,
                            beTargetObj_sInstance->_playerRamNoList, D_0050FFD0, D_0050FFD8);
        if (beTargetObj_sInstance->_playerPosList->_count == 0) {
            return false;
        }
    }
    *out = D_004E1058;
    n = beTargetObj_sInstance->_playerPosList->_count;
    for (i = 0; i < n; i++) {
        igVec3f* p = &((igVec3f*)beTargetObj_sInstance->_playerPosList->_data)[i];

        out->x = out->x + p->x;
        out->y = out->y + p->y;
        out->z = out->z + p->z;
    }
    switch (n) {
    case 0:
    case 1:
        break;
    case 2:
        out->x /= 2.0f;
        out->y /= 2.0f;
        out->z /= 2.0f;
        break;
    case 3:
        out->x /= 3.0f;
        out->y /= 3.0f;
        out->z /= 3.0f;
        break;
    case 4:
        out->x /= 4.0f;
        out->y /= 4.0f;
        out->z /= 4.0f;
        break;
    }
    return true;
}

bool beTargetObj_findPlayer(beTargetObj* t, Vec3 from, bool nearest) {
    if (t->_playerPosList->_count == 0) {
        beTargetObj_collect(t, t->_playerPosList, t->_playerRamNoList, D_0050FFD0, D_0050FFD8);
        if (t->_playerPosList->_count == 0) {
            return false;
        }
    }
    if (nearest) {
        beTargetObj_findNearest(t, t->_playerPosList, t->_playerRamNoList, from, &beTargetObj_sTargetPos,
                                &beTargetObj_sTargetDist, &beTargetObj_sTargetRamNo);
    } else {
        beTargetObj_findFacing(t, t->_playerPosList, t->_playerRamNoList, from, &beTargetObj_sTargetPos,
                               &beTargetObj_sTargetDist, &beTargetObj_sTargetRamNo);
    }
    return true;
}

bool beTargetObj_findEnemy(beTargetObj* t, Vec3 from, bool nearest) {
    if (t->_enemyPosList->_count == 0) {
        beTargetObj_collect(t, t->_enemyPosList, NULL, D_0050FFE8, D_0050FFF0);
        if (t->_enemyPosList->_count == 0) {
            return false;
        }
    }
    if (nearest) {
        beTargetObj_findNearest(t, t->_enemyPosList, NULL, from, &beTargetObj_sTargetPos, &beTargetObj_sTargetDist,
                                &beTargetObj_sTargetRamNo);
    } else {
        beTargetObj_findFacing(t, t->_enemyPosList, NULL, from, &beTargetObj_sTargetPos, &beTargetObj_sTargetDist,
                               &beTargetObj_sTargetRamNo);
    }
    return true;
}

/* Search a target for the model at m: type "PLAYER" or "ENEMY", mode "NEAR"
   (nearest) or anything else (closest to the facing direction). */
bool beTargetObj_search(const igMatrix44f* m, const char* type, const char* mode) {
    igVec3f pos;
    bool nearest;

    func_001908F0(&beTargetObj_sSelfMatrix, m);
    func_00190A30(m, &pos);
    beTargetObj_sSelfPos = pos;
    beTargetObj_sSelfAngle = -100.0f;
    nearest = strcmp(mode, D_00510008) == 0;
    if (strcmp(type, D_0050FFD0) == 0) {
        return beTargetObj_findPlayer(beTargetObj_sInstance, pos, nearest);
    } else if (strcmp(type, D_0050FFE8) == 0) {
        return beTargetObj_findEnemy(beTargetObj_sInstance, pos, nearest);
    }
    return false;
}

/* Use a given position as the target of the model at m. */
bool beTargetObj_setTarget(const igMatrix44f* m, const igVec3f* target) {
    igVec3f pos;

    func_00190A30(m, &pos);
    beTargetObj_sSelfPos = pos;
    func_001908F0(&beTargetObj_sSelfMatrix, m);
    beTargetObj_sSelfAngle = -100.0f;
    beTargetObj_sTargetPos = *target;
    beTargetObj_sTargetDist = Dist2(pos, *target);
    beTargetObj_sTargetRamNo = 0;
    return true;
}

igVec3f* beTargetObj_getSelfPos(void) {
    return &beTargetObj_sSelfPos;
}

/* Facing angle of the self (computed once per search). */
f32 beTargetObj_getSelfAngle(void) {
    if (beTargetObj_sSelfAngle == -100.0f) {
        igVec3f dir;

        func_00192F50(&dir, &D_004E1098, &beTargetObj_sSelfMatrix);
        beTargetObj_sSelfAngle = func_00123640(dir.x, dir.z);
    }
    return beTargetObj_sSelfAngle;
}

igVec3f* beTargetObj_getTargetPos(void) {
    return &beTargetObj_sTargetPos;
}

f32 beTargetObj_getTargetDist(void) {
    return beTargetObj_sTargetDist;
}

/* Angle from the self to the target, plus offset. */
f32 beTargetObj_getTargetAngle(f32 offset) {
    return offset + func_00123640(beTargetObj_sTargetPos.x - beTargetObj_sSelfPos.x,
                                  beTargetObj_sTargetPos.z - beTargetObj_sSelfPos.z);
}

s32 beTargetObj_getTargetRamNo(void) {
    return beTargetObj_sTargetRamNo;
}

igMetaObject* beTargetObj_getMeta(void) {
    return beTargetObj_Meta;
}

}
