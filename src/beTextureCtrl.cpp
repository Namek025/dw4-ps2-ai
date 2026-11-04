#include "game.h"

/* beTextureCtrl: swaps textures in attribute sets by name, once after the
   stage is loaded (MWCC C++). */
extern "C" {

extern igMetaObject* beTextureCtrlInfo_Meta;
extern igMetaObject* beTextureCtrl_Meta;
extern igMetaObject* igTextureBindAttr_Meta;
extern igMetaObject* D_00585BE0; /* meta used for lookups by name */
extern char D_004E0858[];        /* "" */
extern bool D_00554C14;
extern void* D_00587110;
igNamedObject* func_003022C0(igObjectList* list, igMetaObject* meta, const char* name); /* find by name */
beTextureCtrlWork* func_00342630(void* pool);  /* new beTextureCtrlWork */
igTextureBindAttr* func_00132810(void* pool);  /* new igTextureBindAttr */
void func_00302000(s32* out, igObjectList* list, igObject* o, s32); /* remove */

}

class igTextureBindAttrV {
public:
    IG_VPAD_31
    virtual void setTexture(igTextureAttr* tex); /* +0x84 */
};

/* igObject's virtuals. */
class igObjectV {
public:
    IG_VPAD_20
    virtual igMetaObject* getMeta(); /* +0x58 */
};

IG_REF(igTextureBindAttr);

static inline bool IsA(igObject* o, igMetaObject* meta) {
    return meta == ((igObjectV*)o)->getMeta();
}

static inline void* PoolOf2(void* o) {
    void* a = func_00301000(o);

    return D_00554C14 ? a : func_002F88E0(D_00587110);
}

static inline const char* NameOf(igNamedObject* o) {
    return o->_name ? o->_name : D_004E0858;
}

extern "C" {

/* -- */

void beTextureCtrl_prepare(beTextureCtrl* mgr) {
    mgr->_isFirstTime = true;
    igAssign(mgr->_work, (beTextureCtrlWork*)NULL);
}

igMetaObject* beTextureCtrl_getInfoMeta(void) {
    return beTextureCtrlInfo_Meta;
}

/* Collect the info's textures or attribute sets. */
void beTextureCtrl_entry(beTextureCtrl* mgr, beBaseInfoRam* ram) {
    beTextureCtrlInfo* info;

    if (mgr->_work == NULL) {
        void* pool = PoolOf2(mgr);

        igUnref(mgr->_work);
        mgr->_work = func_00342630(pool);
    }
    info = (beTextureCtrlInfo*)ram->_parentInfo;
    if (info->_isTexture) {
        s32 n = info->_dataList->_count;
        s32 i;

        for (i = 0; i < n; i++) {
            func_00301E30(mgr->_work->_textureList, ((igObject**)info->_dataList->_data)[i]);
        }
    } else {
        s32 n = info->_dataList->_count;
        s32 i;

        for (i = 0; i < n; i++) {
            func_00301E30(mgr->_work->_attributeList, ((igObject**)info->_dataList->_data)[i]);
        }
    }
}

/* First frame: bind each attribute set to the texture of the same name. */
void beTextureCtrl_updateRam(beTextureCtrl* mgr, beBaseInfoRam* ram) {
    if (mgr->_isFirstTime) {
        if (!((beTextureCtrlInfo*)ram->_parentInfo)->_isTexture) {
            s32 n = mgr->_work->_attributeList->_count;
            s32 i;

            for (i = 0; i < n; i++) {
                beTextureCtrlWork* work = mgr->_work;
                beTextureCtrlAttr* a = ((beTextureCtrlAttr**)work->_attributeList->_data)[i];
                beTextureCtrlData* tex =
                    (beTextureCtrlData*)func_003022C0(work->_textureList, D_00585BE0, NameOf(a));

                if (tex != NULL) {
                    igTextureBindAttrRef bind(func_00132810(PoolOf2(mgr)), IG_ADOPT);
                    s32 c;
                    s32 j;
                    igAttrSet* set;

                    ((igTextureBindAttrV*)bind.p)->setTexture(tex->_tex);
                    set = a->_attrSet;
                    c = set->_attributes->_count;
                    for (j = 0; j < c; j++) {
                        igObject* o = ((igObject**)set->_attributes->_data)[j];

                        if (IsA(o, igTextureBindAttr_Meta)) {

                            s32 r;

                            func_00302000(&r, set->_attributes, o, 0);
                            break;
                        }
                    }
                    func_00301E30(set->_attributes, bind);
                }
            }
        }
        mgr->_isFirstTime = false;
    }
}

igMetaObject* beTextureCtrl_getMeta(void) {
    return beTextureCtrl_Meta;
}

}
