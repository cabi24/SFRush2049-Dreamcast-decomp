/* pane.c - bordered rectangles (panes) built from five blits.
   Original: D:\rush_ngc\Rush_NGC\GameSource\src\pane.c; assembly: build/2049LE/asm/GameSource/src/pane.s */
#line 1 "pane_types.h"
/* DC pane overlays; addresses prove every named field. */
#ifndef PANE_TYPES_H
#define PANE_TYPES_H
typedef struct tPaneBlit _tBlit_2;
typedef _tBlit_2 _tBlit;
struct tPaneBlit {
    char pad0[4];
    void *palette;           /* +4: InitPane8C0447A4 */
    char pad1[0x0e-8];
    signed short x;          /* +0x0e: ChangePaneSize8C044856 */
    signed short y;          /* +0x10: ChangePaneSize8C044862 */
    char pad2[0x14-0x12];
    signed short width;      /* +0x14: ChangePaneSize8C04486E */
    signed short height;     /* +0x16: ChangePaneSize8C04487A */
    unsigned char alpha;     /* +0x18: InitPane8C04478C/4798 */
    char pad3[0x28-0x19];
    int (*animFunc)(_tBlit_2 *); /* +0x28: InitPane8C0447B0 */
    unsigned int animID;     /* +0x2c: InitPane8C0447BC */
    int animDTA;             /* +0x30: InitPane8C0447C4 */
};
typedef struct tBlitSet {
    signed char used;        /* +0: InitPane8C044722/476C */
    signed char perm;        /* +1: InitPane8C04476A */
    unsigned char blackColor[4]; /* +2: InitPane8C04474A..4756 */
    unsigned char grayColor[4];  /* +6: InitPane8C04475A..4766 */
    char pad0[0x0c-0x0a];
    _tBlit_2 *blits[5];      /* +0xc: InitPane8C044770; stride0x20:472E,4BA8 */
} tBlitSet;
typedef char dc_check_blitset[(sizeof(tBlitSet)==0x20)?1:-1];
typedef char dc_check_pane_blit_overlay[(sizeof(_tBlit_2)==0x34)?1:-1];
#endif
#line 4 "pane.c"

extern _tBlit *NewBlit(const char *, int, int, int);
extern void UpdateBlit(_tBlit *);
extern void RemoveBlit(_tBlit *);
void ChangePaneSize(tBlitSet *, int, int, int, int);

#define SWAP(a, b) ((a) ^= (b), (b) ^= (a), (a) ^= (b))

static tBlitSet gPaneBlits[16] = { 0 }; /* explicit `= { 0 }`: the target keeps this object in initialised data (.data/.sdata), not .bss */

/* 0x800305e8, size 0x218 */
tBlitSet *InitPane(int x1, int y1, int x2, int y2, int alpha, int (*animFunc)(_tBlit_2 *), unsigned int animID, int permanent)
{
    int i; /* r3 (the DWARF lists a second i in r21) */
    tBlitSet *pane; /* r28 */
    for (i = 0, pane = gPaneBlits; i < 16; i++, pane++) {
        if (!pane->used) {
            break;
        }
    }
    if (i >= 16) {
        return pane;
    }
    if (x1 > x2) {
        SWAP(x1, x2);
    }
    if (y1 > y2) {
        SWAP(y1, y2);
    }
    pane->blackColor[0] = 0;
    pane->blackColor[1] = 0;
    pane->blackColor[2] = 0x10;
    pane->blackColor[3] = alpha;
    pane->grayColor[0] = 0;
    pane->grayColor[1] = 0;
    pane->grayColor[2] = 0xc0;
    pane->grayColor[3] = alpha;
    pane->perm = permanent;
    pane->used = 1;
    for (i = 0; i < 5; i++) {
        pane->blits[i] = (_tBlit_2 *)NewBlit(0, x1, y1, 0);
        pane->blits[i]->alpha = alpha;
        pane->blits[i]->palette = (void *)pane->blackColor;
        pane->blits[i]->animFunc = animFunc;
        pane->blits[i]->animID = animID;
        pane->blits[i]->animDTA = 0;
    }
    pane->blits[0]->animDTA = (int)pane;
    pane->blits[0]->palette = (void *)pane->grayColor;
    pane->blits[1]->palette = (void *)pane->grayColor;
    pane->blits[3]->palette = (void *)pane->grayColor;
    pane->blits[4]->palette = (void *)pane->grayColor;
    ChangePaneSize(pane, x1, y1, x2, y2);
    return pane;
}

/* 0x80030800, size 0x1f0 */
void ChangePaneSize(tBlitSet *pane, int x1, int y1, int x2, int y2)
{
    int i; /* r30 */
    if (x1 > x2) {
        SWAP(x1, x2);
    }
    if (y1 > y2) {
        SWAP(y1, y2);
    }
    for (i = 0; i < 5; i++) {
        pane->blits[i]->x = x1;
        pane->blits[i]->y = y1;
        pane->blits[i]->width = x2 - x1;
        pane->blits[i]->height = y2 - y1;
    }
    pane->blits[0]->height = 1;
    pane->blits[1]->width = 1;
    pane->blits[1]->y += 1;
    pane->blits[1]->height -= 2;
    pane->blits[2]->x += 1;
    pane->blits[2]->y += 1;
    pane->blits[2]->width -= 2;
    pane->blits[2]->height -= 2;
    pane->blits[3]->x += pane->blits[3]->width - 1;
    pane->blits[3]->width = 1;
    pane->blits[3]->y += 1;
    pane->blits[3]->height -= 2;
    pane->blits[4]->y += pane->blits[4]->height - 1;
    pane->blits[4]->height = 1;
    for (i = 0; i < 5; i++) {
        UpdateBlit((_tBlit *)pane->blits[i]);
    }
}

/* 0x800309f0, size 0xc8 */
/* Semantic counterpart only; DC map identity remains anonymous. */
void func_8C044914(tBlitSet *pane, unsigned char *paneColor, unsigned char *borderColor)
{
    int i; /* r30 */
    pane->blackColor[0] = paneColor[0];
    pane->blackColor[1] = paneColor[1];
    pane->blackColor[2] = paneColor[2];
    pane->blackColor[3] = paneColor[3];
    pane->grayColor[0] = borderColor[0];
    pane->grayColor[1] = borderColor[1];
    pane->grayColor[2] = borderColor[2];
    pane->grayColor[3] = borderColor[3];
    for (i = 0; i < 5; i++) {
        if (i == 2) {
            pane->blits[i]->palette = (void *)pane->blackColor;
            pane->blits[i]->alpha = paneColor[3];
        } else {
            pane->blits[i]->palette = (void *)pane->grayColor;
            pane->blits[i]->alpha = borderColor[3];
        }
        UpdateBlit((_tBlit *)pane->blits[i]);
    }
}

/* 0x80030ab8, size 0x9c */
void RemovePanes(int permanent)
{
    int j; /* r27 */
    tBlitSet *cursor;
    tBlitSet *pane;

    for (cursor = gPaneBlits; cursor < &gPaneBlits[16]; cursor++) {
        pane = cursor;
        if (pane->used && (permanent || !pane->perm)) {
            for (j = 0; j < 5; j++) {
                if (pane->blits[j]) {
                    RemoveBlit((_tBlit *)pane->blits[j]);
                    pane->blits[j] = 0;
                }
            }
            pane->used = 0;
        }
    }
}

/* 0x80030b54, size 0x88 */
void RemovePane(tBlitSet *pane)
{
    int j; /* r29 */

    if (pane && pane->used) {
        for (j = 0; j < 5; j++) {
            if (pane->blits[j]) {
                RemoveBlit((_tBlit *)pane->blits[j]);
                pane->blits[j] = 0;
            }
        }
        pane->used = 0;
    }
}

