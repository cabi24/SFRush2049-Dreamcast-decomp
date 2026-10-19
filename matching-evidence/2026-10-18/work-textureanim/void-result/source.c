#include "dc_types.h"

#define FRAME_TIME 0.033333302f

extern signed char gTrackNum;
extern signed char gRestartGame;
extern volatile unsigned char gNumModels;
extern volatile float gFrameSeconds;
extern volatile int gLastFrameCount;
extern tPaletteAnim *PaletteLists[];
extern tTextureSwap *TextureSwapLists[];
extern tTexture *MBOX_FindTexture_WORK(const char *, unsigned short *, signed char, signed char, int);

void InitTextureAnimation(void)
{
    tPaletteAnim *plist;
    tTextureSwap *tlist;
    void *ttDef;
    unsigned short index;
    int i;
    plist = PaletteLists[gTrackNum];
    if (plist) {
        for (; plist->name; plist++) {
            plist->time = FRAME_TIME;
            if (plist->state >= 9) {
                ttDef = MBOX_FindTexture_WORK(plist->name, &index, 0, gNumModels - 1, 1);
                plist->ptr = ttDef;
            }
        }
    }
    if (gRestartGame) {
        return;
    }
    tlist = TextureSwapLists[gTrackNum];
    if (!tlist) {
        return;
    }
    for (; tlist->num; tlist++) {
        for (i = 0; i < tlist->num; i++) {
            tlist->swapTexs[i].tex = MBOX_FindTexture_WORK(tlist->swapTexs[i].name, &index, 0, gNumModels - 1, 1);
            tlist->swapTexs[i].texBase = tlist->swapTexs[i].tex->strip;
            tlist->time = tlist->initTime;
            tlist->idx = tlist->startnum;
        }
    }
}

void TextureAnimation(void)
{
    tPaletteAnim *plist;
    tTextureSwap *tlist;
    tTexture *palette;
    plist = PaletteLists[gTrackNum];
    if (plist) {
        for (; plist->name; plist++) {
            plist->time -= gFrameSeconds;
            if (plist->time > 0.0f) {
                continue;
            }
            plist->time = FRAME_TIME * plist->speed;
            palette = plist->ptr;
            switch (plist->state) {
            case 9:
                plist->start += plist->offset * gLastFrameCount;
                if (plist->start < 0) {
                    plist->start += plist->end;
                } else if (plist->start >= plist->end) {
                    plist->start -= plist->end;
                }
                palette->uls = plist->start * (0.0625f / palette->width);
                break;
            case 10:
                plist->start += plist->offset * gLastFrameCount;
                if (plist->start < 0) {
                    plist->start += plist->end;
                } else if (plist->start >= plist->end) {
                    plist->start -= plist->end;
                }
                palette->ult = plist->start * (0.0625f / palette->height);
                break;
            }
        }
    }
    tlist = TextureSwapLists[gTrackNum];
    if (!tlist) {
        return;
    }
    for (; tlist->num; tlist++) {
        tlist->time -= gFrameSeconds;
        if (tlist->time <= 0.0f) {
            tlist->time = tlist->initTime;
            if (tlist->fd) {
                tlist->idx++;
                if (tlist->idx >= tlist->num) {
                    tlist->idx = 0;
                }
            } else {
                tlist->idx--;
                if (tlist->idx < 0) {
                    tlist->idx = tlist->num - 1;
                }
            }
            tlist->swapTexs[tlist->startnum].tex->strip = tlist->swapTexs[tlist->idx].texBase;
        }
    }
}
