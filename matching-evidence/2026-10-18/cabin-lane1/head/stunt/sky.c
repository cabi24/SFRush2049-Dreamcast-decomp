#include "dc_types.h"

extern signed char gFogIntensity;
extern dcSkyWindowInfo ZoidWinInfo[];
extern signed char gTrackNum;
extern signed short gNumWindows;
extern volatile unsigned char gNumModels;
extern dcSkyCamera gCamera[];
extern int MBOX_FindObject_WORK(const char *, signed char, signed char, int);
extern tUVMatrix *ZOID_NewMatrix(void);
extern signed short MBOX_NewObjectCam(int, tUVMatrix *, signed short, int, unsigned int);
extern tPoly *ZOID_NewPoly(int, float (*)[3], unsigned short, tRGBA *, unsigned short, int);
extern void ZOID_SetPolyZ(tPoly *, signed short);
void SkyClear(void);

tUVMatrix *skyMatrices[4];

void SkyInit(void)
{
    int winidx;
    int mbobj;
    tRGBA color;
    float xyz[2][3];

    if (gFogIntensity > 0) {
        SkyClear();
        color.c[0] = ZoidWinInfo[0].fogcolor[0];
        color.c[1] = ZoidWinInfo[0].fogcolor[1];
        color.c[2] = ZoidWinInfo[0].fogcolor[2];
        color.c[3] = ZoidWinInfo[0].fogcolor[3];
        xyz[0][0] = 0.0f;
        xyz[0][1] = 0.0f;
        xyz[1][0] = 640.0f;
        xyz[1][1] = 480.0f;
        ZOID_SetPolyZ(ZOID_NewPoly(2, xyz, 0, &color, 0x600f, 0), 1);
        return;
    }
    if (gTrackNum == 7 || gTrackNum == 8 || gTrackNum == 9 ||
        gTrackNum == 10 || gTrackNum == 11 || gTrackNum == 12 ||
        gTrackNum == 13 || gTrackNum == 17 || gTrackNum == 18) {
        SkyClear();
        return;
    }
    for (winidx = 0; winidx < gNumWindows; winidx++) {
        mbobj = MBOX_FindObject_WORK("SKYSKY", 0, gNumModels - 1, 1);
        skyMatrices[winidx] = ZOID_NewMatrix();
        skyMatrices[winidx]->mat3.uvs[0][0] = 2.5f;
        skyMatrices[winidx]->mat3.uvs[1][1] = 2.5f;
        skyMatrices[winidx]->mat3.uvs[2][2] = 2.5f;
        MBOX_NewObjectCam(mbobj, skyMatrices[winidx], 1 << winidx, -1, 0);
    }
}

void SkyClear(void)
{
    int winidx;
    for (winidx = 0; winidx < gNumWindows; winidx++)
        skyMatrices[winidx] = 0;
}

void SkyUpdate(int winidx)
{
    if (skyMatrices[winidx]) {
        skyMatrices[winidx]->mat3.pos[0] = gCamera[winidx].mat.mat3.pos[0];
        skyMatrices[winidx]->mat3.pos[2] = gCamera[winidx].mat.mat3.pos[2];
    }
}


