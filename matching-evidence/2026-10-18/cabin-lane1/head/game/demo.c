#include "demo_types.h"

extern signed char gSoftReset;
extern unsigned int gActiveEdges;
extern signed short gNumWindows;
extern unsigned int gGameState;
extern int gRace;
extern signed char gTrackNum;
extern signed char gNumActiveCars;
extern int gShowCredits;
extern tDemoModel model[];
extern tDemoPadState gPlayers[4];
extern char **dcDemoTextTable;

extern int CountdownTimeOut(void);
extern void setGameState(unsigned int);
extern void *NewMultiBlit(signed short, signed short, const tDemoBlitDef *, signed short);
extern void SetCountdownTimer(float);
extern void ClearRushControls(void);
extern void RemoveBlit(void *);
extern void HAL_SetTextDepth(float);
extern void TB_SetJust(int, int);
extern int HAL_SetFont(int);
extern void TB_SetTextColor(int);
extern void TB_DrawTextScrollBox(signed short, signed short, signed short, signed short, signed short, signed short, void *);
extern void fake_init(signed short);
extern int PrePlay(int, int);
extern void CarSetup(void);
extern void Update_MDrive(void);
extern void InitCPTime(void);
extern void PlayGame(void);
extern void UnloadTrack();
extern void End_MDrive(signed short);
extern void modelstop(void);
extern void StopListeners(void);
extern void RemoveHUD(void);
extern void RemoveEnvirons(void);
extern void RemoveCar(signed short);

void initDemoGame(void);
void uninitDemoGame(void);
int animText(void);
void playDemoGame(int show, int wait);
void endDemoGame(void);
void nextAvailDemoTrack(void);

static const tDemoBlitDef multiblit[1] = {
    (void *)0xffffffff, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, &animText, 0
};
static signed char inited = 0;
static void *blit = 0;
static signed char preplayDone = 0;
static signed short gCurrentDemoTrack = -1;

void updateDemoGame(void)
{
    if (!inited) {
        initDemoGame();
    }
    playDemoGame(1, 0);
    if (gSoftReset && preplayDone) {
        uninitDemoGame();
        setGameState(4);
    } else if (((gActiveEdges & 1) && preplayDone) || CountdownTimeOut()) {
        uninitDemoGame();
        if (gCurrentDemoTrack == 5 && CountdownTimeOut()) {
            setGameState(2);
        } else {
            setGameState(4);
        }
    }
}

void initDemoGame(void)
{
    blit = NewMultiBlit(0, 0, multiblit, 1);
    SetCountdownTimer(30.0f);
    nextAvailDemoTrack();
    inited = 1;
    ClearRushControls();
}

void uninitDemoGame(void)
{
    if (blit) {
        RemoveBlit(blit);
        blit = 0;
    }
    playDemoGame(0, 0);
    inited = 0;
}

int animText(void)
{
    char text[40];
    HAL_SetTextDepth(0.0f);
    TB_SetJust(1, 1);
    HAL_SetFont(11);
    TB_SetTextColor(22);
    TB_DrawTextScrollBox(160, 185, 320, 110, -1, 0, dcDemoTextTable[32]);
    TB_SetTextColor(1);
    TB_SetJust(0, 3);
    HAL_SetTextDepth(-1.0f);
    return 1;
}

void playDemoGame(int show, int wait)
{
    static signed char gDemoInited = 0;

    if (show) {
        if (!gDemoInited) {
            gDemoInited = 1;
            gNumWindows = 1;
            if (!(gGameState & 0x2000000)) {
                gRace = 0;
                fake_init(gCurrentDemoTrack);
            }
            if (wait) {
            }
        }
        if (!preplayDone && PrePlay(gGameState & 8, 0)) {
            CarSetup();
            Update_MDrive();
            InitCPTime();
            preplayDone = 1;
        } else if (preplayDone) {
            PlayGame();
        }
    } else {
        gDemoInited = 0;
        preplayDone = 0;
        endDemoGame();
        if (!(gGameState & 0x2000000)) {
            UnloadTrack(gTrackNum);
        }
    }
}

void endDemoGame(void)
{
    signed short i;
    for (i = 0; i < gNumActiveCars; i++) {
        End_MDrive(model[i].slot);
    }
    modelstop();
    StopListeners();
    RemoveHUD();
    RemoveEnvirons();
    for (i = 0; i < gNumActiveCars; i++) {
        RemoveCar(i);
    }
    for (i = 0; i < 4; i++) {
        gPlayers[i].slot = i;
    }
}

void nextAvailDemoTrack(void)
{
    gShowCredits = 0;
    gCurrentDemoTrack++;
    if (gCurrentDemoTrack > 5) {
        gShowCredits = 1;
        gCurrentDemoTrack = 0;
    }
}
