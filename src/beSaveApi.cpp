#include "game.h"

/* Save system API objects (beSaveApi and its operations): construction and
   destruction hooks (MWCC C++). */
extern "C" {

void igObject_userInstantiate(igObject* o); /* igObject: after construction */
void igObject_userRelease(igObject* o); /* igObject: before destruction */
void func_002FDD20(igMetaObject* meta, s32);
extern igMetaObject* beSaveApi_Meta;
extern igMetaObject* beSvEndApi_Meta;
extern igMetaObject* beSvStartApi_Meta;
extern igMetaObject* beSvGetFreeSizeApi_Meta;
extern igMetaObject* beSvWriteMediaApi_Meta;
extern igMetaObject* beSvReadMediaApi_Meta;
extern igMetaObject* beSvUseCheckApi_Meta;
extern igMetaObject* beSvFormatApi_Meta;
extern igMetaObject* beSvFileFindApi_Meta;
extern igMetaObject* beSvFileMakeApi_Meta;

}

/* igObject's virtuals. */
class igObjectV {
public:
    IG_VPAD_20
    virtual igMetaObject* getMeta(); /* +0x58 */
};

#define META(o) (((igObjectV*)(o))->getMeta())

template <class L>
static inline void ObjListClear(L* list) {
    s32 n = list->_count;
    s32 i;

    for (i = 0; i < n; i++) {
        igUnref(((igObject**)list->_data)[i]);
    }
    {
        s32 c = list->_count;
        s32 j;

        for (j = 0; j < c; j++) {
            ((igObject**)list->_data)[j] = NULL;
        }
    }
    list->_count = 0;
}

extern "C" {

/* -- */

void beSaveApi_userInstantiate(beSaveApi* o) {
    igObject_userInstantiate(o);
    func_002FDD20(META(o), 1);
    func_002FDD20(META(o), 1);
}

igMetaObject* beSaveApi_getMeta(void) {
    return beSaveApi_Meta;
}

void beSaveApi_userRelease(beSaveApi* o) {
    igObject_userRelease(o);
}

igMetaObject* beSvEndApi_getMeta(void) {
    return beSvEndApi_Meta;
}

void beSvEndApi_userInstantiate(beSvEndApi* o) {
    igObject_userInstantiate(o);
    func_002FDD20(META(o), 1);
    func_002FDD20(META(o), 1);
}

void beSvEndApi_userRelease(beSvEndApi* o) {
    igObject_userRelease(o);
}

igMetaObject* beSvStartApi_getMeta(void) {
    return beSvStartApi_Meta;
}

void beSvStartApi_userInstantiate(beSvStartApi* o) {
    igObject_userInstantiate(o);
    func_002FDD20(META(o), 1);
    func_002FDD20(META(o), 1);
}

void beSvStartApi_userRelease(beSvStartApi* o) {
    igObject_userRelease(o);
}

igMetaObject* beSvGetFreeSizeApi_getMeta(void) {
    return beSvGetFreeSizeApi_Meta;
}

void beSvGetFreeSizeApi_userInstantiate(beSvGetFreeSizeApi* o) {
    igObject_userInstantiate(o);
    func_002FDD20(META(o), 1);
    func_002FDD20(META(o), 1);
}

void beSvGetFreeSizeApi_userRelease(beSvGetFreeSizeApi* o) {
    igObject_userRelease(o);
}

igMetaObject* beSvWriteMediaApi_getMeta(void) {
    return beSvWriteMediaApi_Meta;
}

void beSvWriteMediaApi_userInstantiate(beSvWriteMediaApi* o) {
    igObject_userInstantiate(o);
    func_002FDD20(META(o), 1);
    func_002FDD20(META(o), 1);
}

void beSvWriteMediaApi_userRelease(beSvWriteMediaApi* o) {
    if (o->_saveWork != NULL) {
        igAssign(o->_saveWork, (beSaveMemoryObj*)NULL);
    }
    igObject_userRelease(o);
}

igMetaObject* beSvReadMediaApi_getMeta(void) {
    return beSvReadMediaApi_Meta;
}

void beSvReadMediaApi_userInstantiate(beSvReadMediaApi* o) {
    igObject_userInstantiate(o);
    func_002FDD20(META(o), 1);
    func_002FDD20(META(o), 1);
}

void beSvReadMediaApi_userRelease(beSvReadMediaApi* o) {
    igObjectList* list = o->_loadWorkList;

    /* The count is read again for the clear (volatile). */
    if (*(volatile s32*)&list->_count != 0) {
        ObjListClear(list);
    }
    igObject_userRelease(o);
}

igMetaObject* beSvUseCheckApi_getMeta(void) {
    return beSvUseCheckApi_Meta;
}

void beSvUseCheckApi_userRelease(beSvUseCheckApi* o) {
    igObject_userRelease(o);
}

void beSvUseCheckApi_userInstantiate(beSvUseCheckApi* o) {
    igObject_userInstantiate(o);
    func_002FDD20(META(o), 1);
}

igMetaObject* beSvFormatApi_getMeta(void) {
    return beSvFormatApi_Meta;
}

void beSvFormatApi_userRelease(beSvFormatApi* o) {
    igObject_userRelease(o);
}

igMetaObject* beSvFileFindApi_getMeta(void) {
    return beSvFileFindApi_Meta;
}

void beSvFileFindApi_userRelease(beSvFileFindApi* o) {
    igObject_userRelease(o);
}

igMetaObject* beSvFileMakeApi_getMeta(void) {
    return beSvFileMakeApi_Meta;
}

void beSvFileMakeApi_userInstantiate(beSvFileMakeApi* o) {
    igObject_userInstantiate(o);
    func_002FDD20(META(o), 1);
    func_002FDD20(META(o), 1);
}

void beSvFileMakeApi_userRelease(beSvFileMakeApi* o) {
    if (o->_saveWork != NULL) {
        igAssign(o->_saveWork, (beSaveMemoryObj*)NULL);
    }
    igObject_userRelease(o);
}

void func_003AE620(igObject* o) {
    igObject_userRelease(o);
}

}
