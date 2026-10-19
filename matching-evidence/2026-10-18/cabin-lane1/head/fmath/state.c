/* state.c - game state machine: per-state setup (waves, banks, controllers) on each transition.
   Original: D:\rush_ngc\Rush_NGC\GameSource\src\state.c; assembly: build/2049LE/asm/GameSource/src/state.s */
#include "dc_types.h"
#include "state_types.h"
extern signed short gNumPlayers;
extern signed short gNumWindows;
extern tPadState gPlayers[4];
extern signed char gOptions[41];
extern anon_name_first_24 gSfxBanks[9];
extern signed char gUpsideDownTrack;
extern signed char gRestartGame;
extern int gRace;
extern signed char gTrackNum;
extern unsigned char gFogColors[27][3];
extern int FindWaveInstance(int waveNum);
extern void LockWaveInstance(int idx);
extern void UnlockWaveInstance(int idx);
extern int LoadWave(int waveNum, int loadAgain, int unused, int handle, void *heap);
extern void InitWaves(void);
extern void SetOptions(void);
extern void SetCountdownTimer(float seconds);
extern void AudioLoadBank(anon_name_first_24 *bank, int lock);
extern void InitRecords(void);
extern void HAL_PurgeFonts(void);
extern int HAL_SetFont(int fontID);
extern void AudSetFxVol(float volume, float time);
extern void AudKillSong(void);
extern void AudioUnloadAllBanks(void);
extern void SetControllerActive(int which, int active);
extern void SetControllerStickEdges(int which, int active);
extern void PS_LockFile(tPS_File **psfileh, int lock);
extern void SaveFlush(int force);
extern void SaveServer(int forceUpdate, int rumbleOnly);
extern void UnloadWaveAll(int waveNum);
extern tWindowInfo *ZOID_SetFog(signed short min, signed short max, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned int winidx);
void setGameState(unsigned int state);

/* The target retains these bodies inline and emits _LockAndLoad separately. */
#pragma inline(_LoadFrontEndWaves)
#pragma inline(_UnloadFontWaves)
#pragma inline(setGameState)


unsigned int gGameState = 0xffffffff;
unsigned int gNewGameState = 1;
unsigned int gSkipToGameState = 1;
signed char gGameStarted;
signed char gSofdecShown;
signed char gLicenseShown;
signed char gIntroMovieShown;
signed char pfsInited;
signed char lcdInited;



static void _LockAndLoad(int waveNum);
static void _LoadFrontEndWaves(void);
static void _UnloadFrontEndWaves(void);
static void _UnloadFontWaves(void);
static void initGameState(void);
static void uninitGameState(void);

/* 0x80042a04, size 0x50 */
static void _LockAndLoad(int waveNum)
{
    int inst; /* r0 */

    /* 8C050D8E keeps both query/load results in the eventual call argument register. */
    inst = FindWaveInstance(waveNum);
    if (inst < 0) {
        LockWaveInstance(LoadWave(waveNum, 0, 0, 0, 0));
    }
}

/* 0x80042a54, size 0x34 */
static void _LoadFrontEndWaves(void)
{
    _LockAndLoad(40);
    _LockAndLoad(44);
    _LockAndLoad(45);
}

/* 0x80042a88, size 0x40 */
static void _UnloadFrontEndWaves(void)
{
    UnlockWaveInstance(FindWaveInstance(40));
    UnlockWaveInstance(FindWaveInstance(44));
    UnlockWaveInstance(FindWaveInstance(45));
}

/* 0x80042ac8, size 0x20 */
static void _UnloadFontWaves(void)
{
    HAL_PurgeFonts();
}

/* 0x80042ae8, size 0x538 */
static void initGameState(void)
{
    int i; /* r30 */

    if (gGameState & 0x1) {
        gGameStarted = 0;
        gSofdecShown = 0;
        gLicenseShown = 0;
        gIntroMovieShown = 0;
        pfsInited = 0;
        lcdInited = 0;
        InitWaves();
        gNumPlayers = 1;
        gNumWindows = 1;
        SetOptions();
        SetCountdownTimer(30.0f);
        AudioLoadBank(&gSfxBanks[0], 1);
        InitRecords();
        setGameState(0x2);
    } else if (gGameState & 0x2) {
        _UnloadFontWaves();
        InitWaves();
        AudSetFxVol(gOptions[13] / 10.0f, 0.100000002f);
        _LoadFrontEndWaves();
        SetControllerActive(-1, 0);
        SetControllerActive(0, 1);
        SetControllerStickEdges(-1, 1);
    } else if (gGameState & 0x40000000) {
    } else if (gGameState & 0x4) {
        _UnloadFontWaves();
        InitWaves();
        AudSetFxVol(gOptions[13] / 10.0f, 0.100000002f);
        _LoadFrontEndWaves();
        if (!(gSkipToGameState & 0x7cdfc0)) {
            for (i = 0; i < gNumPlayers; i++) {
                if (gPlayers[i].save) {
                    PS_LockFile((*gPlayers[i].save)->fileh, 0);
                }
            }
        }
        AudKillSong();
        AudioUnloadAllBanks();
        AudioLoadBank(&gSfxBanks[3], 0);
        SetControllerActive(-1, 0);
        SetControllerActive(0, 1);
        SetControllerStickEdges(-1, 1);
        gUpsideDownTrack = 0;
    } else if (gGameState & 0x8) {
        _UnloadFrontEndWaves();
        _UnloadFontWaves();
        InitWaves();
    } else if (gGameState & 0x10) {
        SetControllerActive(-1, 0);
        SetControllerActive(0, 1);
        SetControllerStickEdges(-1, 1);
    } else if (gGameState & 0x20) {
        int p;

        for (p = 0; p < gNumPlayers; p++) {
            SetControllerActive(p, 1);
            SetControllerStickEdges(p, 1);
        }
        for (; p < 4; p++) {
            SetControllerActive(p, 0);
            /* Target retains empty per-controller feature check: 8C050F8E/8C0510CA. */
            if (p > 1) {
            }
        }
    } else if (gGameState & 0x40) {
        SetControllerActive(-1, 0);
        SetControllerActive(0, 1);
    } else if (gGameState & 0x200) {
    } else if (gGameState & 0x400) {
    } else if (gGameState & 0x800) {
    } else if (gGameState & 0x1000) {
    } else if (gGameState & 0x2000) {
    } else if (gGameState & 0x8000000) {
    } else if (gGameState & 0x4000) {
        LoadWave(43, 0, 0, 0, 0);
    } else if (gGameState & 0x100) {
        int p;

        LoadWave(42, 0, 0, 0, 0);
        for (p = 0; p < gNumPlayers; p++) {
            SetControllerActive(p, 1);
            SetControllerStickEdges(p, 1);
        }
        for (; p < 4; p++) {
            SetControllerActive(p, 0);
            /* Target retains empty per-controller feature check: 8C050F8E/8C0510CA. */
            if (p > 1) {
            }
        }
    } else if (gGameState & 0x80) {
        LoadWave(46, 0, 0, 0, 0);
        SetControllerActive(-1, 0);
        SetControllerActive(0, 1);
    } else if (gGameState & 0x40000) {
        if (!gRestartGame) {
            _UnloadFrontEndWaves();
            _UnloadFontWaves();
            InitWaves();
        }
        if (!gRestartGame) {
            SaveFlush(1);
        }
        if (!gRestartGame) {
            /* DC font setup: target 8C051144..8C051178. */
            HAL_SetFont(13);
            HAL_SetFont(10);
            HAL_SetFont(11);
            HAL_SetFont(12);
            HAL_SetFont(4);
            HAL_SetFont(1);
            HAL_SetFont(2);
            HAL_SetFont(3);
            HAL_SetFont(9);
            HAL_SetFont(8);
            HAL_SetFont(5);
            HAL_SetFont(6);
            HAL_SetFont(7);
        }
        if (!gRestartGame) {
            AudKillSong();
            AudioUnloadAllBanks();
            AudioLoadBank(&gSfxBanks[4], 0);
            AudioLoadBank(&gSfxBanks[2], 0);
            if (gRace == 6) {
                AudioLoadBank(&gSfxBanks[1], 0);
            } else if (gRace != 4) {
                if (gTrackNum == 2) {
                    AudioLoadBank(&gSfxBanks[6], 0);
                }
                AudioLoadBank(&gSfxBanks[5], 0);
            }
        }
        SetControllerStickEdges(-1, 0);
    } else if (gGameState & 0x1000000) {
        SetControllerStickEdges(-1, 1);
    } else if (gGameState & 0x800000) {
        SaveServer(1, 0);
        SaveFlush(1);
    } else if (gGameState & 0x20000000) {
        _UnloadFrontEndWaves();
        InitWaves();
        LoadWave(41, 0, 0, 0, 0);
    }
    if (gGameState & 0x7c03fffe) {
        ZOID_SetFog(0, 0, gFogColors[19][0], gFogColors[19][1], gFogColors[19][2], 0xff, 0);
        ZOID_SetFog(0, 0, gFogColors[19][0], gFogColors[19][1], gFogColors[19][2], 0xff, 1);
    }
}

/* 0x80043020, size 0x84 */
static void uninitGameState(void)
{
    if (gGameState & 0x4000) {
        UnloadWaveAll(43);
    } else if (gGameState & 0x100) {
        UnloadWaveAll(42);
        InitWaves();
    } else if (gGameState & 0x80) {
        UnloadWaveAll(46);
    } else if (gGameState & 0x20000000) {
        UnloadWaveAll(41);
        _LoadFrontEndWaves();
    } else if (gGameState & 0x10000000) {
        _UnloadFontWaves();
    }
}

/* 0x800430a4, size 0x3c */
void updateGameState(void)
{
    if (gNewGameState != gGameState) {
        uninitGameState();
        gGameState = gNewGameState;
        initGameState();
    }
}

/* 0x800430e0, size 0x8 */
void setGameState(unsigned int state)
{
    gNewGameState = state;
}

