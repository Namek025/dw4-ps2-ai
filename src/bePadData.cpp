#include "game.h"

/* bePadData: pad (controller) data object (MWCC C++). */
extern "C" {
f32 func_00123640(f32 y, f32 x); /* atan2f */
void igObject_userInstantiate(igObject* o);
float sqrtf(float x); /* MWCC emits sqrt.s inline */
extern igMetaObject* bePadData_Meta;

void igObject_userRelease(igObject* o);

static inline f32 Len2(f32 x, f32 y) {
    return sqrtf(x * x + y * y);
}

void bePadData_userInstantiate(bePadData* o) {
    igObject_userInstantiate(o);
    o->_enaLen = 0.4f;
    o->_enaLen = Len2(o->_enaLen, 0.0f);
    o->_adAngle = 0.7853982f; /* pi / 4 */
    o->_1sttime = 10;
    o->_2ndtime = 6;
    o->_lTrg = o->_lRpt = o->_lStk = o->_lLast = o->_lTime = 0;
    o->_rTrg = o->_rRpt = o->_rStk = o->_rLast = o->_rTime = 0;
    o->_button = o->_trigger = o->_repeat = o->_last = o->_rptime = 0;
}

void bePadData_userRelease(bePadData* o) {
    igObject_userRelease(o);
}

/* Pressed-this-frame bits and auto-repeat: repeat fires on a change, then after _1sttime
   frames held, then every _2ndtime frames. */
static inline void PadRepeat(bePadData* pad, s32* now, s32* last, s32* trg, s32* rpt, s32* time) {
    *trg = *now & (*last ^ *now);
    if (*last ^ *now) {
        *rpt = *trg;
        *time = pad->_1sttime;
    } else if (--*time != 0) {
        *rpt = 0;
    } else {
        *rpt = *now;
        *time = pad->_2ndtime;
    }
}

/* Buttons for this frame. */
void func_0039AF10(bePadData* pad, s32 buttons) {
    pad->_last = pad->_button;
    pad->_button = buttons;
    PadRepeat(pad, &pad->_button, &pad->_last, &pad->_trigger, &pad->_repeat, &pad->_rptime);
}

/* Alchemy's igVec2f is a class with a copy constructor: a by-value argument is
   copied by the caller into a temporary. */
struct Vec2 : igVec2f {
    Vec2(const igVec2f& v) {
        x = v.x;
        y = v.y;
    }
    Vec2(const Vec2& v) {
        x = v.x;
        y = v.y;
    }
};

void func_0039B110(bePadData* pad, s32* bits, Vec2 v);

/* Left stick for this frame (also as direction bits with repeat). */
void func_0039AF90(bePadData* pad, igVec2f* v) {
    pad->_LStick.x = v->x;
    pad->_LStick.y = v->y;
    pad->_lLast = pad->_lStk;
    func_0039B110(pad, &pad->_lStk, pad->_LStick);
    PadRepeat(pad, &pad->_lStk, &pad->_lLast, &pad->_lTrg, &pad->_lRpt, &pad->_lTime);
}

/* Right stick for this frame. */
void func_0039B050(bePadData* pad, igVec2f* v) {
    pad->_RStick.x = v->x;
    pad->_RStick.y = v->y;
    pad->_rLast = pad->_rStk;
    func_0039B110(pad, &pad->_rStk, pad->_RStick);
    PadRepeat(pad, &pad->_rStk, &pad->_rLast, &pad->_rTrg, &pad->_rRpt, &pad->_rTime);
}

/* Stick direction bits: 8, 4, 2, 1 for the four 90-degree sectors around atan2 = +-pi, -pi/2,
   0, pi/2, each widened by _adAngle / 2 so diagonals set two bits. Nothing below _enaLen. */
static inline f32 VecLen(const igVec2f& v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}

void func_0039B110(bePadData* pad, s32* bits, Vec2 v) {
    f32 len = VecLen(v);
    f32 a = func_00123640(v.y, v.x);
    f32 h = 0.5f * pad->_adAngle;

    *bits = 0;
    if (len < pad->_enaLen) {
        return;
    }
    if (a < -2.3561945f + h) {
        *bits |= 8;
    }
    if (a >= -2.3561945f - h && a <= -0.7853982f + h) {
        *bits |= 4;
    }
    if (a > -0.7853982f - h && a < 0.7853982f + h) {
        *bits |= 2;
    }
    if (a >= 0.7853982f - h && a <= 2.3561945f + h) {
        *bits |= 1;
    }
    if (a > 2.3561945f - h) {
        *bits |= 8;
    }
}

igMetaObject* func_0039B2D0(void) {
    return bePadData_Meta;
}

}
