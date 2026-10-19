/* Dreamcast zoidtexture unit, 8C0692AC..8C0693C4.
 * The two unresolved function identities deliberately retain mapped names.
 */
#include "..\include\zoidtexture_private.h"

#define COPY_RGBA(dst, src) (*(unsigned int *)&(dst) = *(unsigned int *)(src))

void ZOID_SetColor(signed short objnum, tRGBA *col)
{
    tZoidObjectDC *obj = &ObjInstanceList[objnum];
    COPY_RGBA(obj->color, col);
}

tRGBA *ZOID_GetColor(signed short objnum)
{
    tZoidObjectDC *obj = &ObjInstanceList[objnum];
    return &obj->color;
}

void ZOID_SetEnvColor(signed short objnum, tRGBA *col)
{
    tZoidObjectDC *obj = &ObjInstanceList[objnum];
    COPY_RGBA(obj->envcolor, col);
}

void ZOID_SetPalette(signed short objnum, void *palette)
{
    tZoidObjectDC *obj = &ObjInstanceList[objnum];
    obj->palette = palette;
}

void ZOID_SetTexture(signed short objnum, tTexture *tex, int lod)
{
    tZoidObjectDC *obj = &ObjInstanceList[objnum];

    if (lod < 0) {
        obj->texture[0] = tex;
        obj->texture[1] = tex;
        obj->texture[2] = tex;
        obj->texture[3] = tex;
    } else {
        obj->texture[lod] = tex;
    }
}

void func_8C069318(signed short objnum, void *value)
{
    tZoidObjectDC *obj = &ObjInstanceList[objnum];
    obj->field48 = value;
}

int func_8C069328(unsigned short objidx, signed short lod, unsigned short texidx)
{
    tZoidModelDef *robj = &gRomObjDefPtrs[objidx >> 10].romObjDefs[objidx & 0x3ff];
    int i;

    if (lod >= 0) {
        if (lod >= robj->lodCount)
            return 0;
        robj->lod[lod].texidx = texidx;
        robj->lod[lod].flags |= 0x8000;
    } else {
        for (i = 0; i < robj->lodCount; i++) {
            robj->lod[i].texidx = texidx;
            robj->lod[lod].flags |= 0x8000; /* DC 8C069394..3A8 indexes flags by lod. */
        }
    }
    return 1;
}
