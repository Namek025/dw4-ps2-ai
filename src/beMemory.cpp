#include "game.h"

/* beMemory: memory use display (empty in the release build) (MWCC C++). */
extern "C" {

extern igMetaObject* beMemory_Meta;

/* -- */

void beMemory_registerToInsight(beMemory* m) {
}

void beMemory_unRegisterFromInsight(beMemory* m) {
}

void beMemory_begin(beMemory* m) {
}

void beMemory_end(beMemory* m) {
}

s32 beMemory_appendInfo(beMemory* m) {
    return 0;
}

void beMemory_removeAllInfos(beMemory* m) {
}

void beMemory_update(beMemory* m) {
}

igMetaObject* beMemory_getMeta(void) {
    return beMemory_Meta;
}

}
