#include "game.h"

/* beKeyboardReceiver: keyboard input object, only the igObject hooks (MWCC C++). */
extern "C" {

void igObject_userInstantiate(igObject* o);
void igObject_userRelease(igObject* o);
extern igMetaObject* beKeyboardReceiver_Meta;

/* -- */

void beKeyboardReceiver_userInstantiate(beKeyboardReceiver* o) {
    igObject_userInstantiate(o);
}

void beKeyboardReceiver_userRelease(beKeyboardReceiver* o) {
    igObject_userRelease(o);
}

s32 func_00389060(void) {
    return 0;
}

igMetaObject* beKeyboardReceiver_getMeta(void) {
    return beKeyboardReceiver_Meta;
}

}
