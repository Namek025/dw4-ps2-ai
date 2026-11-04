#include "game.h"

/* beChangePosTransform: moves a transform to the average position of a
   group's matrices (MWCC C++). */
extern "C" {

extern igMetaObject* beChangePosTransform_Meta;
extern igMetaObject* beChangePosTransformInfo_Meta;
extern void* D_00562640; /* traversal callback table */
void func_001E06C0(void* table, igMetaObject* meta, void (*fn)(void*, void*));
igMetaObject* func_003680B0(void);
igMetaObject* func_00367E50(void);
void func_0037B040(void* a, void* b);
void func_0037B4D0(void* a, void* b);

/* -- */

void func_0037AA70(void) {
}

void func_0037AA80(void) {
}

void func_0037AA90(void) {
}

void func_0037AAA0(void) {
}

void func_0037AAB0(void) {
}

void beChangePosTransform_prepare(beChangePosTransform* mgr) {
}

igMetaObject* beChangePosTransform_getInfoMeta(void) {
    return beChangePosTransformInfo_Meta;
}

INCLUDE_ASM("asm/nonmatchings/beChangePosTransform", beChangePosTransform_updateRam);

igMetaObject* beChangePosTransform_getMeta(void) {
    return beChangePosTransform_Meta;
}

void func_0037B010(void* a, void* b) {
    func_0037B040(b, a);
}

INCLUDE_ASM("asm/nonmatchings/beChangePosTransform", func_0037B040);

void func_0037B460(void) {
    func_001E06C0(D_00562640, func_003680B0(), func_0037B010);
}

void func_0037B4A0(void* a, void* b) {
    func_0037B4D0(b, a);
}

INCLUDE_ASM("asm/nonmatchings/beChangePosTransform", func_0037B4D0);

void func_0037B760(void) {
    func_001E06C0(D_00562640, func_00367E50(), func_0037B4A0);
}

}
