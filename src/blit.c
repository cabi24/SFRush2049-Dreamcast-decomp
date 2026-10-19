/* Private DC blit candidate, mapped0x8C018790..0x8C018CBC. */
#include "blit_types.h"

static unsigned char gNoTexBlitDefaultColor[4] = { 0, 0, 0, 255 };
_tBlit Blits[200];
_tBlit *BlitPtrs[200];
int NumBlits;


void InitBlits(void)
{
    int i; 

    for (i = 0; i < 200; i++) {
        BlitPtrs[i] = &Blits[i];
    }
    NumBlits = 0;
    inactiveBlit = 0;
}


void RemoveBlit(_tBlit *dbp)
{
    int i; 

    while (dbp) {
        for (i = 0; i < NumBlits; i++) {
            if (dbp == BlitPtrs[i]) {
                break;
            }
        }
        if (i == NumBlits || NumBlits <= 0) {
        }
        ZOID_RemoveBlit(dbp->blidx);
        NumBlits--;
        BlitPtrs[i] = BlitPtrs[NumBlits];
        BlitPtrs[NumBlits] = dbp;
        dbp = dbp->child;
    }
}


void DrawBlits(void)
{
    int i; 
    _tBlit *blit; 

    for (i = 0; i < NumBlits; i++) {
        blit = BlitPtrs[i];
        if (blit->animFunc && blit->name != (const char *)-1) {
            if (!blit->animFunc(blit)) {
                RemoveBlit(blit);
                i--;
                continue;
            }
        }
        if (blit->hide != 0) {
        }
    }
}


void InitBlit(_tBlit *blit)
{
    tTexture *ti; 

    if (!blit->name) {
        blit->texIndex = 0;
        blit->texture = 0;
        blit->width = 0;
        blit->height = 0;
        blit->palette = (void *)gNoTexBlitDefaultColor;
    } else if (blit->name == (const char *)-1) {
        blit->texIndex = 0;
        blit->texture = 0;
        blit->width = 0;
        blit->height = 0;
        blit->palette = 0;
    } else {
        ti = MBOX_FindTexture_WORK(blit->name, &blit->texIndex, 0, gNumModels - 1, 1);
        blit->texture = ti;
        blit->width = ti->width;
        blit->height = ti->height;
        blit->palette = 0;
    }
    blit->z = 0;
    blit->alpha = 0xff;
    blit->top = -1;
    blit->bot = -1;
    blit->left = -1;
    blit->right = -1;
    blit->color = -1;
}


void UpdateBlit(_tBlit *blit)
{
    ZOID_UpdateBlit(blit->blidx, blit->texIndex, blit->texture, blit->palette, blit->x, blit->y, blit->width,
                    blit->height);
    ZOID_CropBlit(blit->blidx, blit->top, blit->bot, blit->left, blit->right);
    ZOID_SetBlitData(blit->blidx, blit->hide, blit->alpha, blit->z);
    ZOID_FlipBlit(blit->blidx, blit->flip);
    ZOID_TextBlit(blit->blidx, blit->name == (const char *)-1);
}


_tBlit *NewBlit(const char *name, int x, int y, unsigned int cont)
{
    register _tBlit *blit;
    int drawX = x;
    int drawY = y;

    if (NumBlits >= 200) {
        return 0;
    }
    blit = BlitPtrs[NumBlits];
    NumBlits++;
    if (cont) {
        drawX /= 2;
        drawY = (drawY - 4) / 2.0f;
    }
    blit->name = name;
    blit->x = drawX;
    blit->y = drawY;
    blit->hide = 0;
    blit->flip = 0;
    blit->init = 1;
    blit->child = 0;
    blit->animFunc = 0;
    blit->animID = -1;
    blit->animDTA = -1;
    blit->data = 0;
    InitBlit(blit);
    blit->blidx = ZOID_AddBlit(blit->texIndex, 0, 0, drawX, drawY, -1, -1);
    UpdateBlit(blit);
    return blit;
}


void RenameBlit(_tBlit *blit, const char *name, int preserve)
{
    tTexture *ti; 

    blit->name = name;
    if (preserve) {
        if (!(ti = MBOX_FindTexture_WORK(blit->name, &blit->texIndex, 0, gNumModels - 1, 1))) {
        }
        blit->texture = ti;
    } else {
        InitBlit(blit);
    }
    UpdateBlit(blit);
}


void SelectBlit(_tBlit *blt, int index, int hSize, int vSize)
{
    int col; 
    int row;
    int secPerRow; 

    if (!blt) {
        return;
    }
    if (!blt->texture || !blt->texture->width) {
        RenameBlit(blt, blt->name, 1);
        if (!blt->texture) {
            return;
        }
    }
    secPerRow = blt->texture->width / hSize;
    if (!secPerRow) {
        secPerRow = 1;
    }
    row = index / secPerRow;
    col = index % secPerRow;
    blt->top = row * vSize;
    blt->bot = blt->top + vSize - 1;
    if (blt->flip) {
        col = secPerRow - col - 1;
        blt->left = blt->texture->width % hSize + col * hSize;
        blt->right = blt->left + hSize - 1;
    } else {
        blt->left = col * hSize;
        blt->right = blt->left + hSize - 1;
    }
    UpdateBlit(blt);
}


_tBlit *NewMultiBlit(signed short ulx, signed short uly, const tBlitDef *mblit, signed short nblits)
{
    _tBlit *rootblit; 
    _tBlit *lastblit; 
    _tBlit *curblit; 
    int i; 

    if (nblits <= 0) {
        return 0;
    }
    rootblit = 0;
    lastblit = 0;
    for (i = 0; i < nblits; i++) {
        curblit = NewBlit(mblit[i].texname, ulx + mblit[i].dulx, uly + mblit[i].duly, mblit[i].animid & 0x80000000);
        curblit->alpha = mblit[i].alpha;
        curblit->z = mblit[i].zdepth;
        curblit->left = mblit[i].left;
        curblit->top = mblit[i].top;
        curblit->right = mblit[i].right;
        curblit->bot = mblit[i].bot;
        if (mblit[i].width >= 0) {
            curblit->width = mblit[i].width;
        }
        if (mblit[i].height >= 0) {
            curblit->height = mblit[i].height;
        }
        UpdateBlit(curblit);
        if (mblit[i].animfunc) {
            curblit->animID = mblit[i].animid & 0x7fffffff;
            if (mblit[i].texname == (const char *)-1) {
                curblit->texture = (void *)mblit[i].animfunc;
                curblit->palette = (void *)curblit;
                UpdateBlit(curblit);
            } else {
                curblit->animFunc = mblit[i].animfunc;
                if (!curblit->animFunc(curblit)) {
                    RemoveBlit(curblit);
                    return rootblit;
                }
            }
        }
        if (i == 0) {
            rootblit = curblit;
        } else {
            lastblit->child = curblit;
        }
        lastblit = curblit;
    }
    return rootblit;
}


/* Target-only child-chain position adjustment, anonymous identity preserved. */
void func_8C018C88(_tBlit *blit, int x, int y)
{
    if (blit) {
        blit->x += x;
        blit->y += y;
        UpdateBlit(blit);
        func_8C018C88(blit->child, x, y);
    }
}
