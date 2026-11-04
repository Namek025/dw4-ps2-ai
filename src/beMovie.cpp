#include "game.h"

/* beMovie: movie playback manager (MWCC C++). */
extern "C" {
extern igMetaObject* igMovieManager_Meta;
extern igMetaObject* beMovieInfo_Meta;
extern igMetaObject* beMovie_Meta;

extern igMetaObject* bePadManager_Meta;
extern igMetaObject* beLua_Meta;
extern igMetaObject* beMovieInfoData_Meta;
extern char D_00510200[]; /* "MOVIE" */
extern char D_004E0858[]; /* "" */
void beMessenger_entry(beMessenger* m, beBaseInfoRam* ram, const char* group, const char* info, s32 ramNo);
bePadData* bePadManager_getPad(bePadManager* mgr, s32 n);
void func_00389FF0(beLua* lua, const char* name, s32 dir);

void beMovie_prepare(beMovie* mgr) {
    igAssign(mgr->_movie, (igMovieManager*)func_00236F80(mgr->_insight, igMovieManager_Meta));
}

void beMovie_entry(beMovie* mgr, beBaseInfoRam* ram) {
    beMessenger_entry(mgr->_messenger, ram, D_00510200, NULL, -1);
}

igMetaObject* beMovie_getInfoMeta(void) {
    return beMovieInfo_Meta;
}

#ifdef NON_MATCHING
/* Nonmatching: 22 bytes off in distance. The pad-test delay slot is not filled (the original
   computes the 0x4000 test inside the bnez's delay slot), and the lua/list schedule differs. */
/* Virtuals of igMovieManager, as called from here (+0x70, +0x80). */
class igMovieManagerV {
public:
    IG_VPAD_26
    virtual void unk70(void);                 /* +0x70 */
    virtual void unk74(void);                 /* +0x74 */
    virtual void unk78(void);                 /* +0x78 */
    virtual void unk7C(void);                 /* +0x7C */
    virtual s32 unk80(char* name);            /* +0x80 */
};

void beMovie_updateRam(beMovie* mgr, beBaseInfoRam* ram) {
    if (mgr->_playingName != NULL) {
        bePadData* pad = bePadManager_getPad((bePadManager*)func_00236F80(mgr->_insight, bePadManager_Meta), 0);
        s32 trig = pad->_trigger;

        if ((trig & 8) != 0 || (trig & 0x4000) != 0) {
            igMovieManagerV* m = (igMovieManagerV*)mgr->_movie;
            if (m != NULL) {
                m->unk70();
            }
        }
        {
            bool go;
            igMovieManagerV* m = (igMovieManagerV*)mgr->_movie;
            go = 0;
            if (m != NULL) {
                if (m->unk80(mgr->_playingName) == 2) {
                    go = 1;
                }
            } else {
                go = 1;
            }
            if (go == false) {
                return;
            }
        }
        {
            beLua* lua;
            beBaseInfoDataList* list;

            lua = (beLua*)func_00236F80(mgr->_insight, beLua_Meta);
            list = ram->_parentInfo->_dataList;
            if (0 < list->_count) {
                s32 i = 0;
                for (;;) {
                    beMovieInfoData* data = (beMovieInfoData*)((void**)list->_data)[i];
                    char* name;

                    data = (data != NULL && func_00300CD0(data, beMovieInfoData_Meta)) ? data : NULL;
                    if (data == NULL) {
                        return;
                    }
                    name = data->_name != NULL ? data->_name : D_004E0858;
                    if (strcmp(mgr->_playingName, name) == 0) {
                        func_00389FF0(lua, data->_callLuaName, 0);
                        break;
                    }
                    i++;
                    if (i >= list->_count) {
                        break;
                    }
                }
            }
        }
        {
            char* newName = func_002EAF60(GetStrPool(), NULL);

            ReleaseStr(mgr->_playingName);
            mgr->_playingName = newName;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/beMovie", beMovie_updateRam);
#endif

INCLUDE_ASM("asm/nonmatchings/beMovie", beMovie_handle);

}
IG_REF(beMovieInfo);
extern "C" {
/* __dt__14beMovieInfoRefFv (0x3A9190) is beMovieInfoRef's out-of-line destructor
   (exception cleanup of beMovie_handle, still asm). This caller, not in the unit's
   asm, makes MWCC emit it here. */
#ifndef NON_MATCHING
void emit___dt__14beMovieInfoRefFv(beMovieInfo* m) {
    beMovieInfoRef r(m, IG_ADOPT);

    func_002FF820(r);
}
#endif

igMetaObject* beMovie_getMeta(void) {
    return beMovie_Meta;
}

}
