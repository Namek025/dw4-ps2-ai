#include "common.h"

extern s32 D_004BF010;

void func_002B4330(s32* arg0) {
    if (arg0 != NULL) {
        *arg0 = 0;
        D_004BF010 -= 1;
    }
}
