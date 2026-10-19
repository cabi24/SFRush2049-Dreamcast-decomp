/* DC title screen; MAT3 semantic reference with target-proven movie flow. */
#include "titlescr_types.h"
int animText(void);
static const tBlitDef blits[3] = {
    "RUSH_SPLASH_A",0,0,-1,-1,-1,-1,-1,-1,0,0xff,0,0,
    "RUSH_SPLASH_B",255,0,-1,-1,-1,-1,-1,-1,0,0xff,0,1,
    (void *)0xffffffff,0,0,0,0,-1,-1,-1,-1,0,0,(void *)&animText,0
};
static int inited = 0;
static _tBlit *blit = 0;
static int movieIdx = -1;

void updateTitleScreen(void)
{
    if (gSkipToGameState != 1) {
        if (gSkipToGameState != gGameState) {
            setGameState(16);
            return;
        }
        gSkipToGameState = 1;
    }
    if (movieIdx != -1) {
        if (!(gActiveEdges & 15) &&
            (func_8C06CA0A(movieIdx) == 1 || func_8C06CA0A(movieIdx) == 2)) return;
        func_8C06C986(movieIdx);
        func_8C06C712();
        movieIdx = -1;
    }
    if (movieIdx == -1 && gIntroMovieShown < 2) {
        func_8C06C692();
        movieIdx = func_8C06C808(gIntroMovieShown == 0 ? "midwaylogo.sfd" : "rushlogo.sfd");
        gIntroMovieShown++;
        if (movieIdx != -1) return;
        func_8C06C712();
    }
    if (!inited) {
        blit = NewMultiBlit(0,0,blits,3);
        SetCountdownTimer(5.0f);
        inited = 1;
        if (gGameStarted && gSoftReset) gSoftReset = 0;
        gGameStarted = 0;
        ClearRushControls();
    }
    if (gActiveEdges & 1) {
        int i;
        if (movieIdx != -1) {
            func_8C06C986(movieIdx);
            func_8C06C712();
            movieIdx = -1;
        }
        if (blit) {
            RemoveBlit(blit);
            blit = 0;
        }
        inited = 0;
        AudPlayEffect(52,0,1,0);
        gPlayers[0].controller = 0;
        for (i=1;i<4;i++) gPlayers[i].controller = 5;
        setGameState(16);
    } else if (CountdownTimeOut()) {
        if (blit) {
            RemoveBlit(blit);
            blit = 0;
        }
        inited = 0;
        setGameState(8);
    }
}

int animText(void)
{
    HAL_SetTextDepth(0.0f);
    TB_SetJust(1,1);
    HAL_SetFont(11);
    TB_SetTextColor(22);
    TB_DrawTextScrollBox(160,185,320,110,-1,0,gText.single[32]);
    TB_SetTextColor(0);
    HAL_SetFont(12);
    TB_DrawText(160,205,gText.single[244]);
    TB_SetJust(0,3);
    HAL_SetTextDepth(-1.0f);
    return 1;
}
