#include "dc_types.h"
/* countdown.c - MAT3 semantic reference with DC changes proven by target instructions. */


extern signed short gNumWindows;
extern unsigned char gNumModels;
extern tCamera gCamera[4];
extern float GetCountdownTimeRemaining(void);
extern void U64WorldVector(float *in, float *out, float (*uvs)[3]);
extern tTexture *MBOX_FindTexture_WORK(const char *name, unsigned short *index, signed char firstModel, signed char lastModel, int flag);
extern tPoly *ZOID_NewPoly(int nverts, float (*xyz)[3], unsigned short texidx, tRGBA *color, unsigned short flags, int useTidx);
extern void ZOID_UpdatePoly(tPoly *poly, int nverts, float (*xyz)[3], unsigned short texidx, tRGBA *color, unsigned short flags, int useTidx);
extern void ZOID_ShowPoly(tPoly *poly);
extern void ZOID_RemovePoly(tPoly *poly);
extern int PlayGameSound(int fxid, int subId, int replace, unsigned char priority);
extern int sprintf(char *str, const char *format, ...);
extern void setGameState(int state);

#define NULL 0

signed short gCountDown;

void ShowCountdown(int active)
{
    static tPoly *poly[4];
    static unsigned short polyTidx;
    static unsigned short lastCountDown;
    static tRGBA polyColor = { 0xff, 0xff, 0xff, 0xc0 };
    static float polyXYZ[4][3] = {
        { 1.0f, 1.0f, 6.0f }, { -1.0f, 1.0f, 6.0f }, { -1.0f, -1.0f, 6.0f }, { 1.0f, -1.0f, 6.0f }
    };
    float xyz[4][3];
    float dist;
    int i;
    int winidx;
    char str[20];

    for (winidx = 0; winidx < gNumWindows; winidx++) {
        if (active) {
            /* SHC truncates literals; target pool 8C0227A4 is BA83126F. */
            dist = GetCountdownTimeRemaining() - 0.0010000001f;
            dist = 12.0f * (dist - (float)(int)dist);
            if (dist < 5.0f)
                dist = 5.0f;
            for (i = 0; i < 4; i++) {
                polyXYZ[i][2] = dist;
                /* 8C0226C0..6DE: 0x98 stride; camera+0x30 to 8C026BE4. */
                U64WorldVector(polyXYZ[i], xyz[i], gCamera[winidx].n64.mat3.uvs);
                xyz[i][0] += gCamera[winidx].mat.mat3.pos[0];
                xyz[i][1] += gCamera[winidx].mat.mat3.pos[1];
                xyz[i][2] += gCamera[winidx].mat.mat3.pos[2];
            }
            if (!poly[winidx]) {
                MBOX_FindTexture_WORK("CNTDWN3", &polyTidx, 0, gNumModels - 1, 1);
                /* Target flag pool at 8C0227D8. */
                poly[winidx] = ZOID_NewPoly(4, xyz, polyTidx, &polyColor, (1 << winidx) | 0x82c0, 1);
                lastCountDown = 4;
            } else if (lastCountDown > gCountDown) {
                if (winidx == gNumWindows - 1) {
                    sprintf(str, "CNTDWN%d", gCountDown);
                    MBOX_FindTexture_WORK(str, &polyTidx, 0, gNumModels - 1, 1);
                    lastCountDown = gCountDown;
                    if (gCountDown == 3)
                        PlayGameSound(0x58, 0, 1, 0);
                    else if (gCountDown == 2)
                        PlayGameSound(0x5a, 0, 1, 0);
                    else
                        PlayGameSound(0x4e, 0, 1, 0);
                }
                ZOID_UpdatePoly(poly[winidx], 4, xyz, polyTidx, &polyColor, 0, 1);
                ZOID_ShowPoly(poly[winidx]);
            } else {
                ZOID_UpdatePoly(poly[winidx], 4, xyz, polyTidx, &polyColor, 0, 1);
            }
        } else {
            if (poly[winidx]) {
                ZOID_RemovePoly(poly[winidx]);
                poly[winidx] = NULL;
            }
            gCountDown = -1;
        }
    }
}

void CountDown(void)
{
    /* SHC truncates literals; target pool 8C022948 is 3F7FBE77. */
    gCountDown = 0.99900002f + GetCountdownTimeRemaining();
    if (gCountDown <= 0)
        setGameState(0x100000);
}
