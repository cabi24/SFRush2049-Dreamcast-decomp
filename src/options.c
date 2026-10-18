/* Private Dreamcast options candidate. See notes/NOTES.md for target evidence. */
#include "options_types.h"

static void _SetDefaultOptions(void);





void InitOptions(void)
{
    int i;
    for (i = 0; i < 4; i++) {
        gPlayers[i].controller = 5;
        gPlayers[i].trans = 0;
        gPlayers[i].listener = -1;
        gPlayers[i].slot = i;
    }
    _SetDefaultOptions();
    gNumPlayers = gNumWindows = 1;
}


static void _SetDefaultOptions(void)
{
    int i; 

    SetControls(1);
    for (i = 0; i < 40; i++) {
        gOptions[i] = GetDefaultOption(i);
    }
    AudSetFxVol(gOptions[13] / 10.0f, 0.050000001f);
    AudSetMusicVol(gOptions[12] / 10.0f, 0.050000001f);
    gNumLaps = gOptions[20];
    gNumDrones = gOptions[21];
    gShowTrackMap = gOptions[0];
    gGravityScale = gOptions[22];
    gShowRadar = gOptions[1];
    gShowElapsed = gOptions[2];
    gShowRemaining = gOptions[3];
    gShowTach = gOptions[4];
    gShowSpeed = gOptions[5];
    gShowOdometer = gOptions[6];
    gShowPlace = gOptions[7];
    gShowShifter = gOptions[8];
    gDifficulty = gOptions[23];
    gGhostSelect = gOptions[24];
    gCatchup = gOptions[25];
    gBackwards = gOptions[26];
    gMirrorWaves = gOptions[27] > 0;
    gMirrorControls = gOptions[27] > 1;
    gFogIntensity = gOptions[28];
    gWind = gOptions[29];
    gShowMetric = gOptions[9];
    gDeathMode = gOptions[30];
    gShowPlayerArrows = gOptions[10];
    gShowCoins = gOptions[11];
    gLang = gOptions[15];
    gShowWrongWay = gOptions[17];
    gStuntMode = gOptions[31];
    gStuntFlow = gOptions[32];
    gStuntModeMins = gOptions[34];
    gStuntModePoints = gOptions[33];
    gStuntModeDeaths = gOptions[35];
    gBattleMode = gOptions[36];
    gBattleModeMins = gOptions[38];
    gBattleModePoints = gOptions[37];
    gNoCarCollisions = 0;
    gFogColor = 0;
    gMineCones = 0;
    gCarMines = 0;
    gRandomWeapons = 0;
    gDemolitionBattle = 0;
    gCheatState[3] = 0;
    gUpsideDownTrack = 0;
    gNoAutoAbort = 0;
    gInvincible = 0;
    gInvisibleCar = 0;
    gInvisibleTrack = 0;
    gNoBrakes = 0;
    gSpecialTires = 0;
    gMassiveMass = 0;
    gSuicideMode = 0;
    gCheatState[13] = 0;
    gCheatState[16] = 0;
    gCheatState[17] = 0;
    gCheatState[18] = 0;
    gSuperSpeed = 0;
    gResurrectInPlace = 0;
    gFrameScale = 0;
    gEnableTireScaling = 0;
    for (i = 0; i < 4; i++) {
        gDriveHulk[i] = 0;
    }
}


signed char GetDefaultOption(int opt)
{
    switch (opt) {
    case 20:
        return 3;
    case 21:
        return 5;
    case 0:
        return 1;
    case 22:
        return 1;
    case 1:
        return 1;
    case 2:
        return 1;
    case 3:
        return 1;
    case 4:
        return 1;
    case 5:
        return 1;
    case 6:
        return 1;
    case 7:
        return 1;
    case 8:
        return 1;
    case 23:
        return 2;
    case 24:
        return 0;
    case 25:
        return 2;
    case 26:
        return 0;
    case 27:
        return 0;
    case 28:
        return 0;
    case 29:
        return 0;
    case 9:
        return !optionsFlag_8C1224C4 || gLang;
    case 30:
        return 0;
    case 10:
        return 1;
    case 11:
        return 1;
    case 12:
        return 8;
    case 13:
        return 10;
    case 14:
        return 18;
    case 15:
        return gLang;
    case 16:
        return 1;
    case 17:
        return 1;
    case 18:
        return 0;
    case 19:
        return 0;
    case 31:
        return 0;
    case 32:
        return 0;
    case 34:
        return 5;
    case 33:
        return 69;
    case 35:
        return 30;
    case 36:
        return 0;
    case 38:
        return 8;
    case 37:
        return 10;
    case 39:
        return 0;
    }
}


void SetOptions(void)
{
    int i; 
    int j; 

    if (!(gGameState & 8)) {
        for (i = 0; i < gNumPlayers; i++) {
            if (gPlayers[i].save) {
                for (j = 0; j < 20; j++) {
                    SavePlayerOption(gPlayers[i].save, j, gOptions[j]);
                }
            }
        }
    }
    gShowTrackMap = gOptions[0];
    gShowRadar = gOptions[1];
    gShowElapsed = gOptions[2];
    gShowRemaining = gOptions[3];
    gShowTach = gOptions[4];
    gShowSpeed = gOptions[5];
    gShowPlace = gOptions[7];
    gShowShifter = gOptions[8];
    gShowMetric = gOptions[9];
    gShowPlayerArrows = gOptions[10];
    gShowWrongWay = gOptions[17];
    gNoAutoAbort = gCheatState[4];
    gInvincible = gCheatState[6];
    gNoBrakes = gCheatState[9];
    gSpecialTires = gCheatState[10];
    gMassiveMass = gCheatState[11];
    gSuicideMode = gCheatState[12];
    gSuperSpeed = gCheatState[5];
    gResurrectInPlace = gCheatState[19];
    if (gGameState & 8) {
        gNumLaps = 3;
        gNumDrones = 6;
        gBackwards = 0;
        gMirrorWaves = 0;
        gMirrorControls = 0;
        gFogIntensity = 0;
        gWind = 0;
        gGravityScale = 1;
        gDifficulty = 2;
        gCatchup = 2;
        gDeathMode = 0;
    } else {
        switch (gRace) {
        case 0:
            gNumLaps = gOptions[20];
            gNumDrones = gOptions[21];
            if (gNumPlayers > 2) {
                gNumDrones = 0;
            }
            gBackwards = gOptions[26];
            gMirrorWaves = gOptions[27] > 0;
            gMirrorControls = gOptions[27] > 1;
            gFogIntensity = gOptions[28];
            gWind = gOptions[29];
            gGravityScale = gOptions[22];
            gDifficulty = gOptions[23];
            gCatchup = gOptions[25];
            gDeathMode = gOptions[30];
            gShowOdometer = gOptions[6];
            gShowCoins = gOptions[11];
            break;
        case 1:
            gNumLaps = 1;
            gNumDrones = 0;
            gBackwards = gOptions[26];
            gMirrorWaves = gOptions[27] > 0;
            gMirrorControls = gOptions[27] > 1;
            gFogIntensity = gOptions[28];
            gWind = gOptions[29];
            gGravityScale = gOptions[22];
            gDifficulty = gOptions[23];
            gCatchup = gOptions[25];
            gDeathMode = 0;
            gShowOdometer = gOptions[6];
            gShowCoins = gOptions[11];
            break;
        case 2:
            gNumLaps = 3;
            gNumDrones = 0;
            gBackwards = gOptions[26];
            gMirrorWaves = 0;
            gMirrorControls = 0;
            gFogIntensity = gOptions[28];
            gWind = 0;
            gGravityScale = 1;
            gDifficulty = 2;
            gGhostSelect = gOptions[24];
            gCatchup = 0;
            gDeathMode = 0;
            gShowOdometer = gOptions[6];
            gShowCoins = gOptions[11];
            gNoAutoAbort = 0;
            gInvincible = 0;
            gNoBrakes = 0;
            gSpecialTires = 0;
            gMassiveMass = 0;
            gSuicideMode = 0;
            gSuperSpeed = 0;
            gResurrectInPlace = 0;
            break;
        case 3:
            gNumLaps = 3;
            gNumDrones = 5;
            gBackwards = gCircuitTracks[gCircuitTrack].backward;
            gMirrorWaves = gCircuitTracks[gCircuitTrack].mirror;
            gMirrorControls = 0;
            gFogIntensity = gCircuitTracks[gCircuitTrack].fog;
            gWind = gCircuitTracks[gCircuitTrack].wind;
            gGravityScale = 1;
            gDifficulty = (*(*gPlayers[gCircuitPlayer].save)->datah)->circuit.difficulty;
            gCatchup = 2;
            gDeathMode = (*(*gPlayers[gCircuitPlayer].save)->datah)->circuit.death;
            gShowOdometer = gOptions[6];
            gShowCoins = gOptions[11];
            break;
        case 4:
            gNumLaps = 1;
            gNumDrones = 0;
            gBackwards = 0;
            gMirrorWaves = gOptions[27] > 0;
            gMirrorControls = gOptions[27] > 1;
            gFogIntensity = gOptions[28];
            gWind = gOptions[29];
            gGravityScale = gOptions[22];
            gDifficulty = 0;
            gCatchup = 0;
            gDeathMode = 0;
            gStuntMode = gOptions[31];
            gStuntFlow = gOptions[32];
            gStuntModePoints = gOptions[33];
            gStuntModeMins = gOptions[34];
            gStuntModeDeaths = gOptions[35];
            gShowTrackMap = 0;
            gShowOdometer = 0;
            gShowCoins = gOptions[11];
            break;
        case 5:
            gNumLaps = 1;
            gNumDrones = 0;
            gBackwards = 0;
            gMirrorWaves = gOptions[27] > 0;
            gMirrorControls = gOptions[27] > 1;
            gFogIntensity = 0;
            gWind = 0;
            gGravityScale = gOptions[22];
            gDifficulty = 0;
            gCatchup = 0;
            gDeathMode = 0;
            gShowTrackMap = 0;
            gShowOdometer = 0;
            gShowCoins = 0;
            break;
        case 6:
            gNumLaps = 1;
            gNumDrones = 0;
            gBackwards = 0;
            gMirrorWaves = gOptions[27] > 0;
            gMirrorControls = gOptions[27] > 1;
            gFogIntensity = gOptions[28];
            gWind = gOptions[29];
            gGravityScale = gOptions[22];
            gDifficulty = 0;
            gCatchup = 0;
            gDeathMode = 0;
            gShowSpeed = 0;
            gBattleMode = gOptions[36];
            gBattleModePoints = gOptions[37];
            gBattleModeMins = gOptions[38];
            gShowTrackMap = 0;
            gShowOdometer = 0;
            gShowCoins = 0;
            break;
        }
    }
    if (gGameState & 0x7c03fffe) {
        gMirrorWaves = 0;
        gMirrorControls = 0;
    }
}

