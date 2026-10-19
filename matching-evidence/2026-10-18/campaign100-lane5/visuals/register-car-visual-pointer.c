#include "visuals_types.h"
static void InitWheelDynamicObjs(void);
static void InitDynamicTextures(void);

float gDummyVtx[4][3] = { 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f };

tRGBA SheenColors[19][2] = {
    126, 221, 252, 255, 53, 91, 255, 255, 255, 255, 255, 255, 253, 175, 128, 255, 112, 179, 253,
    255, 113, 223, 254, 255, 255, 150, 202, 255, 255, 199, 144, 255, 255, 150, 202, 255, 158, 253,
    228, 255, 100, 166, 254, 255, 113, 240, 254, 255, 255, 255, 255, 255, 255, 246, 164, 255, 128,
    128, 128, 255, 128, 128, 128, 255, 128, 128, 128, 255, 128, 128, 128, 255, 126, 221, 252, 255,
    53, 91, 255, 255, 255, 255, 255, 255, 255, 228, 144, 255, 255, 255, 255, 255, 253, 231, 158,
    255, 255, 255, 255, 255, 255, 205, 196, 255, 255, 162, 175, 255, 255, 159, 229, 255, 128, 128,
    128, 255, 128, 128, 128, 255, 94, 142, 170, 255, 255, 207, 172, 255, 200, 200, 200, 255, 255,
    207, 159, 255, 242, 195, 103, 255, 242, 195, 103, 255, 255, 255, 255, 255, 255, 255, 255, 255
};

tRGBA Track1Colors[26] = {
    255, 255, 255, 255, 150, 150, 150, 255, 125, 125, 125, 255, 100, 100, 100, 255, 75, 75, 75,
    255, 50, 50, 50, 255, 40, 40, 40, 255, 30, 30, 30, 255, 20, 20, 20, 255, 215, 255, 252, 255,
    175, 223, 255, 255, 179, 253, 248, 255, 179, 216, 253, 255, 200, 200, 255, 255, 170, 170, 255,
    255, 128, 128, 255, 255, 254, 198, 231, 255, 254, 155, 214, 255, 254, 204, 190, 255, 255, 173,
    147, 255, 255, 245, 200, 255, 251, 254, 137, 255, 240, 255, 193, 255, 228, 255, 143, 255, 190,
    254, 190, 255, 148, 254, 154, 255
};

tRGBA Track4Colors[26] = {
    200, 200, 200, 255, 180, 180, 180, 255, 160, 160, 160, 255, 140, 140, 140, 255, 120, 120, 120,
    255, 100, 100, 100, 255, 75, 75, 75, 255, 50, 50, 50, 255, 25, 25, 25, 255, 215, 255, 252, 255,
    82, 127, 172, 255, 179, 253, 248, 255, 179, 216, 253, 255, 200, 200, 255, 255, 170, 170, 255,
    255, 128, 128, 255, 255, 254, 198, 231, 255, 254, 155, 214, 255, 210, 200, 180, 255, 255, 173,
    147, 255, 255, 245, 200, 255, 251, 254, 137, 255, 190, 200, 103, 255, 228, 255, 143, 255, 190,
    254, 190, 255, 148, 254, 154, 255
};

tRGBA Track5Colors[26] = {
    255, 255, 255, 255, 200, 200, 200, 255, 175, 175, 175, 255, 150, 150, 150, 255, 125, 125, 125,
    255, 100, 100, 100, 255, 75, 75, 75, 255, 50, 50, 50, 255, 25, 25, 25, 255, 215, 255, 252, 255,
    175, 223, 255, 255, 179, 253, 248, 255, 179, 216, 253, 255, 200, 200, 255, 255, 170, 170, 255,
    255, 128, 128, 255, 255, 254, 198, 231, 255, 254, 155, 214, 255, 254, 204, 190, 255, 255, 173,
    147, 255, 255, 245, 200, 255, 251, 254, 137, 255, 240, 255, 193, 255, 228, 255, 143, 255, 190,
    254, 190, 255, 148, 254, 154, 255
};

tRGBA *gTrackColors[19] = {
    (void *)&Track1Colors, (void *)&Track1Colors, (void *)&Track1Colors, (void *)&Track4Colors,
    (void *)&Track5Colors, (void *)&Track1Colors, (void *)&Track1Colors, (void *)&Track1Colors,
    (void *)&Track1Colors, (void *)&Track1Colors, (void *)&Track1Colors, (void *)&Track1Colors,
    (void *)&Track1Colors, (void *)&Track1Colors, (void *)&Track5Colors, (void *)&Track5Colors,
    (void *)&Track5Colors, (void *)&Track5Colors, (void *)&Track5Colors
};

static float TSCLS[3] = { 0.60000003129243851f, 1.700000062584877f, 2.5f };

void InitTrackDynamicObjs(void)
{
    register int i;
    for (i = 469; i < 530; i++)
        gObjList[i] = MBOX_FindObject_WORK(ObjNames[i - 469], 0, gNumModels - 1, 0);
    gRomObjDefPtrs[gObjList[528] >> 10].romObjDefs[gObjList[528] & 1023].radius = 100.0f;
    gRomObjDefPtrs[gObjList[529] >> 10].romObjDefs[gObjList[529] & 1023].radius = 100.0f;
    for (; i < 538; i++)
        gObjList[i] = MBOX_FindObject_WORK(WeaponNames[i - 530], 0, gNumModels - 1, 0);
    for (; i < 550; i++)
        gObjList[i] = MBOX_FindObject_WORK(WingNames[i - 538], 0, gNumModels - 1, 0);
    if (gMineCones || gCarMines)
        gObjList[i++] = MBOX_FindObject_WORK(MineName, 0, gNumModels - 1, 0);
    for (; i < 701; i++)
        gObjList[i] = MBOX_FindObject_WORK(TrkNames[i - 550], 0, gNumModels - 1, 0);
    SetTargetDefs();
}

static void InitWheelDynamicObjs(void)
{
    unsigned short index; /* stack */
    int i; /* r28 */
    for (i = 0; i < 48; i++) {
        MBOX_FindTexture_WORK(gWheelNames[i], &index, 0, gNumModels - 1, 1);
        gWheelTextures[i] = &gTexDefPtrs[index >> 10].texDefs[index & 1023];
    }
    for(i=0;i<1;i++)
        gObjList[468+i] = MBOX_FindObject_WORK(WheelObjNames[i], 0, gNumModels - 1, 1);
}

static void InitDynamicTextures(void)
{
    int i; /* r29 */
    for (i = 0; i < 19; i++)
        MBOX_FindTexture_WORK(TexNames[i], &gTexList[i], 0, gNumModels - 1, 0);
}

void InitCarDynamicObjs(signed short slot, signed short car, signed short model)
{
    int i; /* r27 */
    char partName[20]; /* stack */
    int objIndex;
    objIndex = slot * 9;
    for (i = 0; i < 9; i++, objIndex++) {
        sprintf(partName, CarPartFormat, CarNames[car], carObjNames[i]);
        gObjList[objIndex] = MBOX_FindObject_WORK(partName, model, model, 1);
    }
}

void InitEnvs(void)
{
    register int i; /* r4 */
    gFreeVis = gEnvirons;
    for (i = 0; i < 99; i++) {
        int next = i + 1;
        gEnvirons[i].next = &gEnvirons[next];
        gEnvirons[i].objnum = -1;
    }
    gEnvirons[i].next = 0;
    gEnvirons[i].objnum = -1;
    gEnvList = 0;
    gEnvCount = 0;
}

void InitVisuals(int cars)
{
    int i; /* r6 */
    for (i = 0; i < 4; i++) gCarTeamColor[i] = i;
    if (cars) {
        memset(gCarParts, 0, sizeof(gCarParts));
        if (gGameState & 0x100) {
            InitVisualsCarScr();
            for (i = 0; i < 468; i++) gObjList[i] = 0;
        } else {
            for (i = 0; i < 13; i++) {
                InitCarDynamicObjs(i, gLink[i].car, gCarModelNums[i]);
                gCarPaintJobInited[i] = 0;
            }
        }
        InitWheelDynamicObjs();
        InitDynamicTextures();
    }
}

void InitCarVisuals(tCarData *gc)
{
    register tVisual *v; /* r5 */
    signed short i; /* r6 */
    for (v = gc->visuals, i = 0; i < 22; i++, v++) {
        v->func = 0;
        v->objnum = -1;
    }
}

void SetSheen(signed short slot, signed short player, signed short sheenIdx)
{
    tTexture *tex; /* r3 */
    CarParts *car; /* r30 */
    unsigned short texidx; /* stack */
    signed short cam; /* r31 */
    if (sheenIdx < -1 || sheenIdx > 10) return;
    switch (player) {
        case 0: cam = 1; break;
        case 1: cam = 2; break;
        case 2: cam = 4; break;
        case 3: cam = 8; break;
    }
    car = &gCarParts[slot + player * 13];
    if (sheenIdx == -1) MBOX_HideObject(car->sheen, 0, cam);
    else {
        tex = MBOX_FindTexture_WORK(sheenName[sheenIdx], &texidx, 0, gNumModels - 1, 1);
        ZOID_SetTexture(car->sheen, tex, -1);
        MBOX_ShowObject(car->sheen, 0, cam);
    }
}

void SetFrameColor(signed short slot, tRGBA *col)
{
    CarParts *car; /* r31 */
    car = &gCarParts[slot];
    ZOID_SetColor(car->frame, col);
    ZOID_SetColor(car->hood, col);
    ZOID_SetColor(car->tires[0], col);
    ZOID_SetColor(car->tires[1], col);
    ZOID_SetColor(car->tires[2], col);
    ZOID_SetColor(car->tires[3], col);
}

void SetSheenColor(signed short slot, tRGBA *col, tRGBA *env)
{
    CarParts *car; /* r31 */
    car = &gCarParts[slot];
    ZOID_SetColor(car->sheen, col);
    ZOID_SetEnvColor(car->sheen, env);
}

void SetTSheenColor(signed short slot, tRGBA *col, tRGBA *env)
{
    CarParts *car; /* r31 */
    car = &gCarParts[slot];
    ZOID_SetColor(car->tsheen, col);
    ZOID_SetEnvColor(car->tsheen, env);
}

int InitCarObj(int objnum, signed short camlink, int parent, unsigned int flags, signed short slot, signed short carObjNum, int forGame)
{
    tUVMatrix *mat; /* r4 */
    if (forGame) mat = ZOID_NewMatrix();
    else if (carObjNum == -1) {
        flags |= 128;
        mat = (tUVMatrix *)unitmatrix;
    } else mat = ((tUVMatrix **)gCarMatrices)[slot / 13 * 12 + carObjNum];
    return (signed short)MBOX_NewObjectCam(objnum, mat, camlink, parent, flags);
}
