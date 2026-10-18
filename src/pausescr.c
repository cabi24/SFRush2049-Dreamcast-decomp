#include "pausescr_types.h"

static void updatePauseStateCreate(void);
static void _TitlePane(void);
static int animPause(_tBlit *);
static int animTitleText(_tBlit *);
static int animPauseText(_tBlit *);
static int animSureText(_tBlit *);
static int animRumbleText(_tBlit *);
static int animRumblePane(_tBlit *);
void setPauseState(int);
#pragma inline(setPauseState)
static const tBlitDef DimMultiBlit[1] = { 0,0,0,320,220,-1,-1,-1,-1,0,128,animPause,0 };
static const tBlitDef TitleMultiBlit[1] = { (void *)-1,0,0,0,0,-1,-1,-1,-1,0,0,animTitleText,0 };
static const tBlitDef PauseMultiBlit[1] = { (void *)-1,0,0,0,0,-1,-1,-1,-1,0,0,animPauseText,0 };
static const tBlitDef SureMultiBlit[1] = { (void *)-1,0,0,0,0,-1,-1,-1,-1,0,0,animSureText,0 };
static const tBlitDef RumbleMultiBlit[1] = { (void *)-1,0,0,0,0,-1,-1,-1,-1,0,0,animRumbleText,0 };
int gPauseState = 0;
static dcPauseRect gPlayerRects[4][4] = {
    0, 0, 320, 110, 160, 55, 0, 0, 320, 220, 160, 110, 0, 0, 320, 220, 160, 110, 0, 0, 320, 220,
    160, 110, 0, 0, 320, 110, 160, 55, 0, 110, 320, 110, 160, 165, 0, 0, 320, 220, 160, 110, 0, 0,
    320, 220, 160, 110, 0, 0, 160, 110, 80, 55, 160, 0, 160, 110, 240, 55, 0, 110, 160, 110, 80,
    165, 0, 0, 320, 220, 160, 110, 0, 0, 160, 110, 80, 55, 160, 0, 160, 110, 240, 55, 0, 110, 160,
    110, 80, 165, 160, 110, 160, 110, 240, 165
};
static dcPauseTitlePos titlePos = {160,10};
static _tBlit *DimBlit = 0,*TitleBlit = 0,*PauseBlit = 0,*SureBlit = 0,*RumbleBlit = 0;
static void *gTitlePane = 0;
static signed char gControllerRemoved = 0,gYes = 0;
int playerInPause;
static int pauseMenuSelect;
signed char gPakInserted[4];

/* Identity retained until a caller proves a semantic name. */
int func_8C046928(int pak) { return PS_PakInserted(pak); }

void updatePauseScreen(void)
{
    int i;
    int result,code;
    signed char removed;
    if ((gPauseState == 0 || gPauseState == 1 || gPauseState == 6 || gPauseState == 7 || gPauseState == 8) && gSoftReset) {
        gRestartGame = 0;
        setPauseState(0);
        ResumeTimer();
        AudStopAllEffects();
        AbortGame();
        return;
    }
    SaveServer(0,0);
    switch (gPauseState) {
    case 0:
        for (i = 0; i < gNumWindows; i++) {
            int pause = (gPlayers[i].edges & 1) || !gControllerData[i].attached;
            removed = 0;
            if (gPakInserted[i] && (*gPlayers[i].save)->fileh &&
                !PS_PakInserted(((dcPauseFile *)*(*gPlayers[i].save)->fileh)->pak)) {
                gPakInserted[i] = 0;
                pause = 1;
                removed = 1;
            } else if ((*gPlayers[i].save)->fileh &&
                       PS_PakInserted(((dcPauseFile *)*(*gPlayers[i].save)->fileh)->pak)) {
                gPakInserted[i] = 1;
            }
            if (pause) {
                playerInPause = i;
                gControllerRemoved = !gControllerData[i].attached;
                goto pause_game;
            }
        }
        return;
    pause_game:
        setPauseState(1);
        pauseMenuSelect = 0;
        PauseTimer();
        AudPauseAllEffects();
        PauseGameMusic();
        StopListeners();
        modelstop();
        PauseTargets();
        SetControllerStickEdges(-1, 1);
        SaveFlush(1);
        AudPlayEffect(7, 0, 1, 0);
        if (removed) SaveMessage(((dcPauseFile *)*(*gPlayers[i].save)->fileh)->pak,11,0,0,&result,&code,func_8C046928);
        break;
    case 5: updateOptionScreen(); break;
    case 4: updateControlScreen(); break;
    case 2: updateAudioScreen(); break;
    case 3: updateVideoScreen(); break;
    case 1:
        if (gPlayers[playerInPause].edges & 10) {
            AudPlayEffect(0, 0, 1, 0);
            switch (pauseMenuSelect) {
            case 0:
            resume_game:
                setPauseState(0);
                ResumeTimer();
                StartListeners();
                AudResumeAllEffects();
                ResumeGameMusic();
                if (!(gGameState & 0x200000)) StartGameMusic();
                modelgo();
                SetControllerStickEdges(-1, 0);
                return;
            case 6:
                if (gModelTime < 15.0f) {
                    gRestartGame = 1;
                    goto abort_game;
                } else setPauseState(7);
                break;
            case 7:
                if (gModelTime < 15.0f) {
                    gRestartGame = 0;
                    if (gGameState & 0x400000) goto retire_game;
                    goto abort_game;
                } else setPauseState(8);
                break;
            case 3: setPauseState(2); break;
            case 4: setPauseState(3); break;
            case 1: setPauseState(5); break;
            case 2: gPauseControl[playerInPause] = 1; setPauseState(4); break;
            case 5: setPauseState(6); break;
            }
        } else if (gPlayers[playerInPause].edges & 0x400) {
            PlayUDSound(gPlayers[playerInPause].edges);
            if (--pauseMenuSelect < 0) pauseMenuSelect = 7;
        } else if (gPlayers[playerInPause].edges & 0x800) {
            PlayUDSound(gPlayers[playerInPause].edges);
            if (++pauseMenuSelect >= 8) pauseMenuSelect = 0;
        } else if (gPlayers[playerInPause].edges & 5) {
            PlayStartABSound(gPlayers[playerInPause].edges);
            goto resume_game;
        }
        break;
    case 7: case 8:
        if (gPlayers[playerInPause].edges & 0x3000) {
            gYes = !gYes;
            PlayLRSound(gPlayers[playerInPause].edges);
        }
        if (gPlayers[playerInPause].edges & 10) {
            if (gYes) {
                if ((gGameState & 0x400000) && gPauseState == 8) {
                retire_game:
                    gRetireGame = 1;
                    setPauseState(0);
                    ResumeTimer();
                    AudStopAllEffects();
                } else {
                    gRestartGame = gPauseState == 7;
                abort_game:
                    setPauseState(0);
                    ResumeTimer();
                    AudStopAllEffects();
                    AbortGame();
                }
            } else setPauseState(1);
        } else if (gPlayers[playerInPause].edges & 5) {
            PlayStartABSound(gPlayers[playerInPause].edges);
            setPauseState(1);
        }
        break;
    case 6:
        if (gPlayers[playerInPause].edges & 15) {
            PlayStartABSound(gPlayers[playerInPause].edges);
            setPauseState(1);
        }
        break;
    }
    if (gPauseState && gControllerRemoved != !gControllerData[playerInPause].attached) {
        gControllerRemoved = !gControllerData[playerInPause].attached;
        if (gPauseState != 5 && gPauseState != 4 && gPauseState != 2 && gPauseState != 3) _TitlePane();
    }
}

void updatePauseStateCleanup(void)
{
    int p;
    switch (gPauseState) {
    case 0:
        if (DimBlit) { RemoveBlit(DimBlit); DimBlit = 0; }
        for (p = 0; p < gNumPlayers; p++)
            if ((*gPlayers[p].save)->datah) SaveSelectPlayer(gPlayers[p].save);
    case 1: case 5: case 4: case 2: case 3: case 6: case 7: case 8:
        if (TitleBlit) { RemoveBlit(TitleBlit); TitleBlit = 0; }
        if (PauseBlit) { RemoveBlit(PauseBlit); PauseBlit = 0; }
        if (SureBlit) { RemoveBlit(SureBlit); SureBlit = 0; }
        if (RumbleBlit) { RemoveBlit(RumbleBlit); RumbleBlit = 0; }
    }
    RemovePanes(0);
    gTitlePane = 0;
}

static void updatePauseStateCreate(void)
{
    int max;
    int width;
    int i;
    int x;
    int y;
    switch (gPauseState) {
    case 1:
        if (!DimBlit) DimBlit = NewMultiBlit(0, 0, DimMultiBlit, 1);
        _TitlePane();
        HAL_SetFont(13);
        x = titlePos.x;
        y = titlePos.y + 2 * TB_GetFontLineHeight();
        HAL_SetFont(10);
        max = 0;
        for (i = 0; i < 8; i++) {
            width = HAL_GetTextWidth(gText.taba[i + 1 + gText.tab[58]], -1);
            if (max < width) max = width;
        }
        max /= 2;
        InitPane(x - max - 8, y - 4, x + max + 8, y + 8 * TB_GetFontLineHeight() + 4, 176, 0, 0, 0);
        TitleBlit = NewMultiBlit(0, 0, TitleMultiBlit, 1);
        PauseBlit = NewMultiBlit(0, 0, PauseMultiBlit, 1);
        break;
    case 6:
        gYes = 0;
        _TitlePane();
        HAL_SetFont(10);
        x = gPlayerRects[0][1].cx;
        y = gPlayerRects[0][1].cy - TB_GetFontLineHeight() / 2;
        width = HAL_GetTextWidth(gText.single[238], -1);
        width /= 2;
        InitPane(x - width - 8, y - 4, x + width + 8, y + TB_GetFontLineHeight() + 4, 176, animRumblePane, 0, 0);
        HAL_SetFont(11);
        width = HAL_GetTextWidth(gText.single[181], -1);
        width = (unsigned int)width > HAL_GetTextWidth(gText.single[180], -1) ? width : HAL_GetTextWidth(gText.single[180], -1);
        width /= 2;
        for (max = 0; max < gNumPlayers; max++) {
            x = gPlayerRects[gNumPlayers - 1][max].cx;
            y = gPlayerRects[gNumPlayers - 1][max].cy - TB_GetFontLineHeight();
            InitPane(x - width - 8, y - 4, x + width + 8, y + 2 * TB_GetFontLineHeight() + 4, 176, 0, 0, 0);
        }
        TitleBlit = NewMultiBlit(0, 0, TitleMultiBlit, 1);
        RumbleBlit = NewMultiBlit(0, 0, RumbleMultiBlit, 1);
        break;
    case 7: case 8:
        gYes = 0;
        _TitlePane();
        HAL_SetFont(13);
        x = titlePos.x;
        y = titlePos.y + 2 * TB_GetFontLineHeight();
        HAL_SetFont(10);
        width = HAL_GetTextWidth(gText.single[227], -1);
        width /= 2;
        InitPane(x - width - 8, y - 4, x + width + 8, y + 2 * TB_GetFontLineHeight() + 4, 176, 0, 0, 0);
        TitleBlit = NewMultiBlit(0, 0, TitleMultiBlit, 1);
        SureBlit = NewMultiBlit(0, 0, SureMultiBlit, 1);
        break;
    }
}

static void _TitlePane(void)
{
    int x;
    int y;
    int width;
    unsigned char *str;
    unsigned char str1[64];
    HAL_SetFont(13);
    str = str1;
    if (gControllerRemoved) {
        TB_Sprintf(str1, "CONTROLLER %d %s", playerInPause + 1, gText.single[242]);
        width = HAL_GetTextWidth(str, -1);
    } else if (gPauseState == 7 || gPauseState == 8) {
        width = HAL_GetTextWidth(gText.taba[gText.tab[58] + gPauseState], -1);
    } else if (gPauseState == 6) {
        width = 0;
    } else {
        TB_Sprintf(str1, "%s %s", (*gPlayers[playerInPause].save)->name, gText.taba[gText.tab[58]]);
        width = HAL_GetTextWidth(str, -1);
    }
    x = titlePos.x;
    y = titlePos.y;
    if (!gTitlePane) gTitlePane = InitPane(-20, 0, -10, 10, 176, 0, 0, 0);
    if (!width) ChangePaneSize(gTitlePane, -20, 0, -10, 10);
    else {
        width /= 2;
        ChangePaneSize(gTitlePane, x - width - 8, y - 4, x + width + 8, y + TB_GetFontLineHeight() + 4);
    }
}

void setPauseState(int state)
{
    gPauseState = state;
    updatePauseStateCleanup();
    updatePauseStateCreate();
}

static int animPause(_tBlit *blt)
{
    static dcPauseColor color = { 0,0,0,128 };
    if (Hidden(blt, pauseMenuSelect != 0)) return 1;
    blt->palette = &color;
    UpdateBlit(blt);
    blt->animFunc = 0;
    return 1;
}

static int animTitleText(_tBlit *blt)
{
    int x;
    int y;
    unsigned char *str;
    unsigned char str1[40];
    if (Hidden(blt, 0)) return 1;
    HAL_SetTextDepth(0.0f);
    HAL_SetFont(13);
    x = titlePos.x;
    y = titlePos.y;
    str = str1;
    if (gControllerRemoved) {
        TB_Sprintf(str1, "CONTROLLER %d %s", playerInPause + 1, gText.single[242]);
        TB_SetTextColor(1);
    } else if (gPauseState == 7 || gPauseState == 8) {
        str = gText.taba[gText.tab[58] + gPauseState];
        TB_SetTextColor(1);
    } else if (gPauseState == 6) {
        str1[0] = 0;
    } else {
        TB_Sprintf(str1, "%s %s", (*gPlayers[playerInPause].save)->name, gText.taba[gText.tab[58]]);
        TB_SetTextColor(playerTextColor[playerInPause]);
    }
    TB_DrawText(TB_CenterText(str, x), y, str);
    HAL_SetTextDepth(-1.0f);
    return 1;
}

static int animPauseText(_tBlit *blt)
{
    int i;
    int x;
    int y;
    unsigned char *str;
    if (Hidden(blt, 0)) return 1;
    HAL_SetTextDepth(0.0f);
    HAL_SetFont(13);
    x = titlePos.x;
    y = titlePos.y + 2 * TB_GetFontLineHeight();
    HAL_SetFont(10);
    for (i = 0; i < 8; i++) {
        TB_SetTextColor(i == pauseMenuSelect ? 22 : 1);
        str = gText.taba[i + 1 + gText.tab[58]];
        TB_DrawText(TB_CenterText(str, x), y, str);
        y += TB_GetFontLineHeight();
    }
    HAL_SetTextDepth(-1.0f);
    return 1;
}

static int animSureText(_tBlit *blt)
{
    int width;
    int x;
    int y;
    unsigned char *str;
    if (Hidden(blt, gPauseState < 7)) return 1;
    HAL_SetTextDepth(0.0f);
    HAL_SetFont(13);
    x = titlePos.x;
    y = titlePos.y + 2 * TB_GetFontLineHeight();
    HAL_SetFont(10);
    TB_SetTextColor(1);
    str = gText.single[227];
    TB_DrawText(TB_CenterText(str, x), y, str);
    y += TB_GetFontLineHeight();
    width = HAL_GetTextWidth(gText.single[227], -1);
    x = titlePos.x - width / 4;
    str = gText.taba[gText.tab[25]];
    TB_SetTextColor(gYes ? 22 : 1);
    TB_DrawText(TB_CenterText(str, x), y, str);
    x = titlePos.x + width / 4;
    str = gText.taba[gText.tab[25] + 1];
    TB_SetTextColor(gYes ? 1 : 22);
    TB_DrawText(TB_CenterText(str, x), y, str);
    HAL_SetTextDepth(-1.0f);
    return 1;
}

static int animRumbleText(_tBlit *blt)
{
    unsigned char *str;
    unsigned short i;
    signed short x;
    signed short y;
    int allRumblesPresent = 1;
    if (Hidden(blt, gPauseState != 6)) return 1;
    HAL_SetTextDepth(0.0f);
    for (i = 0; i < gNumPlayers; i++)
        if (!PS_PakRumble(gPlayers[i].controller*2) && !PS_PakRumble(gPlayers[i].controller*2+1)) allRumblesPresent = 0;
    if (!allRumblesPresent) {
        HAL_SetFont(10);
        x = gPlayerRects[0][1].cx;
        y = gPlayerRects[0][1].cy - TB_GetFontLineHeight() / 2;
        TB_SetTextColor(22);
        str = gText.single[238];
        TB_DrawText(TB_CenterText(str, x), y, str);
    }
    HAL_SetFont(11);
    TB_SetTextColor(1);
    for (i = 0; i < gNumPlayers; i++) {
        x = gPlayerRects[gNumPlayers - 1][i].cx;
        y = gPlayerRects[gNumPlayers - 1][i].cy - TB_GetFontLineHeight();
        TB_SetTextColor(playerTextColor[i]);
        str = (*gPlayers[i].save)->name;
        TB_DrawText(TB_CenterText(str, x), y, str);
        y += TB_GetFontLineHeight();
        str = (PS_PakRumble(gPlayers[i].controller*2) || PS_PakRumble(gPlayers[i].controller*2+1)) ? gText.single[181] : gText.single[180];
        TB_SetTextColor(1);
        TB_DrawText(TB_CenterText(str, x), y, str);
    }
    HAL_SetTextDepth(-1.0f);
    return 1;
}

static int animRumblePane(_tBlit *blt)
{
    int i;
    int allRumblesPresent = 1;
    for (i = 0; i < gNumPlayers; i++)
        if (!PS_PakRumble(gPlayers[i].controller*2) && !PS_PakRumble(gPlayers[i].controller*2+1)) allRumblesPresent = 0;
    Hidden(blt, allRumblesPresent);
    return 1;
}
