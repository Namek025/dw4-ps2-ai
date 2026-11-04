#include "game.h"

extern "C" {
void func_0039E090(beShadow01* mgr, beShadow01Info* info);
bool beBaseInfoManager_appendInfo(beBaseInfoManager* mgr, beBaseInfo* info);
bool beTargetObj_getPlayerCenter(igVec3f* out);
void func_001D4480(void);
void func_001D4E60(igProjectiveShadowShader* shader);
void func_001C5D10(igGroup* group, igNode* child);
extern void* D_0055F0C0;
extern void* D_00562868;
extern igMetaObject* beShadow01Info_Meta;
extern igMetaObject* beShadow01_Meta;
}

/* Vtable slot 33 (+0x8C) of an igLightAttr: update the light from a position. */
class igLightAttrV {
public:
    IG_VPAD_33
    virtual void update(igVec3f* pos);
};

#ifdef NON_MATCHING
/* Nonmatching: s-register/a-register order of the clear loops still differs (distance 31). */
void beShadow01_prepare(beShadow01* mgr) {
    s32 off;
    beShadow01InfoList* list = mgr->_svInfoList;
    s32 lim;
    s32 i;
    s32 k;
    s32 j;
    s32 n;
    s32 count = list->_count;

    if (0 < count) {
        i = 0;
        off = 0;
        do {
            igObject* o = *(igObject**)((char*)list->_data + off);

            if (o != NULL) {
                o->_refCount = o->_refCount - 1;
                if (((u64)((s64)(s32)o->_refCount << 0x29) >> 0x29) == 0) {
                    func_002FF820(o);
                }
            }
            i++;
            off += 4;
        } while (i < count);
    }

    {
        n = list->_count;

        j = 0;
        if (0 < n) {
            if (n > 8) {
                lim = n - 8;
                k = 0;
                do {
                    j += 8;
                    *(s32*)((char*)list->_data + k) = 0;
                    *(s32*)((char*)list->_data + k + 4) = 0;
                    *(s32*)((char*)list->_data + k + 8) = 0;
                    *(s32*)((char*)list->_data + k + 12) = 0;
                    *(s32*)((char*)list->_data + k + 16) = 0;
                    *(s32*)((char*)list->_data + k + 20) = 0;
                    *(s32*)((char*)list->_data + k + 24) = 0;
                    *(s32*)((char*)list->_data + k + 28) = 0;
                    k += 0x20;
                } while (j < lim);
            }
            k = j * 4;
            if (j < n) {
                do {
                    j++;
                    *(s32*)((char*)list->_data + k) = 0;
                    k += 4;
                } while (j < n);
            }
        }
    }

    list->_count = 0;
    if ((mgr->_svInfoList->_count > 0) == false) {
        func_002D6680(mgr->_svInfoList, 0, 4);
    }
    mgr->_savePos.x = 0.0f;
    mgr->_savePos.y = 0.0f;
    mgr->_savePos.z = 0.0f;
}
#else
INCLUDE_ASM("asm/nonmatchings/beShadow01", beShadow01_prepare);
#endif

igMetaObject* beShadow01_getInfoMeta(void) {
    return beShadow01Info_Meta;
}

void func_0039E090(beShadow01* mgr, beShadow01Info* info) {
    igProjectiveShadowShader* shader;
    s32 count;
    s32 i;
    s32 off;

    i = 0;
    shader = info->_projShadowShader;
    count = mgr->_svInfoList->_count;
    if (0 < count) {
        off = 0;
        do {
            beShadow01Info* o = *(beShadow01Info**)((char*)mgr->_svInfoList->_data + off);

            if (o->_casterNode != NULL) {
                func_00301E30(shader->_shadowCasters, o->_casterNode);
            }
            if (o->_receiverNode != NULL) {
                func_001C5D10(shader, o->_receiverNode);
            }
            i++;
            off += 4;
        } while (i < count);
    }

    {
        beShadow01InfoList* list2 = mgr->_svInfoList;
        s32 cnt;
        s32 ii;
        s32 oo;

        cnt = list2->_count;
        ii = 0;
        if (0 < cnt) {
            oo = 0;
            do {
                igObject* r = *(igObject**)((char*)list2->_data + oo);

                if (r != NULL) {
                    r->_refCount = r->_refCount - 1;
                    if (((u64)((s64)(s32)r->_refCount << 0x29) >> 0x29) == 0) {
                        func_002FF820(r);
                    }
                }
                ii++;
                oo += 4;
            } while (ii < cnt);
        }

        {
            s32 n = list2->_count;
            s32 j = 0;

            if (0 < n) {
                if (n > 8) {
                    s32 lim = n - 8;
                    s32 k = 0;
                    do {
                        j += 8;
                        *(s32*)((char*)list2->_data + k) = 0;
                        *(s32*)((char*)list2->_data + k + 4) = 0;
                        *(s32*)((char*)list2->_data + k + 8) = 0;
                        *(s32*)((char*)list2->_data + k + 12) = 0;
                        *(s32*)((char*)list2->_data + k + 16) = 0;
                        *(s32*)((char*)list2->_data + k + 20) = 0;
                        *(s32*)((char*)list2->_data + k + 24) = 0;
                        *(s32*)((char*)list2->_data + k + 28) = 0;
                        k += 0x20;
                    } while (j < lim);
                }
                {
                    s32 k2 = j * 4;
                    if (j < n) {
                        do {
                            j++;
                            *(s32*)((char*)list2->_data + k2) = 0;
                            k2 += 4;
                        } while (j < n);
                    }
                }
            }
        }

        list2->_count = 0;
        if ((mgr->_svInfoList->_count > 0) == false) {
            func_002D6680(mgr->_svInfoList, 0, 4);
        }
    }
}

bool beShadow01_appendInfo(beShadow01* mgr, beShadow01Info* info) {
    if (func_00300CD0(info, beShadow01Info_Meta)) {
        igProjectiveShadowShader* s = info->_projShadowShader;

        if (s != NULL) {
            s->_drawReceiver = false;
            s->_useOrthographic = true;
            s->_blurState = false;
            s->_cutoff = 22.5f;
            if (s->_top == 5.0f) {
                s->_top = 100.0f;
                s->_bottom = -100.0f;
                s->_right = 100.0f;
                s->_left = -100.0f;
            }
            func_0039E090(mgr, info);
            beBaseInfoManager_appendInfo(mgr, info);
        } else {
            func_00301E30(mgr->_svInfoList, info);
        }
        return true;
    }
    return false;
}

#ifdef NON_MATCHING
/* Nonmatching: the first if is laid out as beqz/b in the original, not bnez; ram/info s-registers
   are swapped; the pos stores differ. */
void beShadow01_updateRam(beShadow01* mgr, beBaseInfoRam* ram) {
    igVec3f pos;
    beShadow01Info* info = (beShadow01Info*)ram->_parentInfo;
    igProjectiveShadowShader* shader;
    igVec3f* lp;
    igLightAttr* light;
    s32 changed;

    if (ram->_st0 == 0) {
        func_0039E090(mgr, info);
        shader = info->_projShadowShader;
        if (shader->_inited == 0) {
            func_001D4480();
            func_001D4E60(shader);
            shader->_inited = 1;
        }
        ((s32*)D_00562868)[3] = 0;
        ram->_st0++;
        ram->_st3 = 0;
        ram->_st2 = 0;
        ram->_st1 = 0;
    } else {
    }
    if (beTargetObj_getPlayerCenter(&pos) != 0) {
        igVec3f* saved = &mgr->_savePos;

        pos.y += 200.0f;
        changed = 1;
        if (pos.x == saved->x) {
            changed = 0;
        }
        if (changed == 0) {
            changed = 1;
            if (pos.y == saved->y) {
                changed = 0;
            }
        }
        if (changed == 0) {
            changed = 1;
            if (pos.z == saved->z) {
                changed = 0;
            }
        }
        if (changed != 0) {
            light = info->_projShadowShader->_light;
            lp = (igVec3f*)((char*)light + ((s32*)D_0055F0C0)[2]);
            ((igLightAttrV*)light)->update(&pos);
            light->_cachedPosition.x = pos.x;
            light->_cachedPosition.y = pos.y;
            light->_cachedPosition.z = pos.z;
            light->_cachedDirection.x = lp->x;
            light->_cachedDirection.y = lp->y;
            light->_cachedDirection.z = lp->z;
            mgr->_savePos.x = pos.x;
            mgr->_savePos.y = pos.y;
            mgr->_savePos.z = pos.z;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/beShadow01", beShadow01_updateRam);
#endif

igMetaObject* beShadow01_getMeta(void) {
    return beShadow01_Meta;
}
