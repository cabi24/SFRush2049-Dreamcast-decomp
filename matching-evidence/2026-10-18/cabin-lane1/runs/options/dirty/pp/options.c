/* Private Dreamcast options candidate. See notes/NOTES.md for target evidence. */
#line 1 "options_types.h"
#ifndef DC_OPTIONS_TYPES_H
#define DC_OPTIONS_TYPES_H
/* Target-backed partial overlays. Unused fields stay padding. */
typedef struct tOptionsCircuit {
    char pad0[5];
    unsigned char death;
    char pad1[1];
    unsigned char difficulty;
} tOptionsCircuit;
typedef struct tOptionsSaveData {
    char pad0[0x6f4];
    tOptionsCircuit circuit; /* +6f9/+6fb: pools8C041B3A/38 */
} tOptionsSaveData;
typedef struct tOptionsProfile {
    char pad0[0x3c];
    tOptionsSaveData **datah; /* +3c:8C041A54/8C041A6C */
} tOptionsProfile;
typedef struct tOptionsPlayer {
    unsigned char slot;       /* +0:8C04124C/268 */
    unsigned char controller; /* +1:8C041240/25e */
    char pad0[0x46-2];
    unsigned char trans;      /* +46:8C041242/244 */
    char pad1[1];
    unsigned int listener;    /* +48:8C041246/248 */
    tOptionsProfile **save;   /* +4c:8C0416C8/6CC */
} tOptionsPlayer; /* stride50:8C041230..23C,8C0416BC..6C6 */
typedef struct tOptionsCircuitTrack {
    char pad0[1];
    unsigned char backward; /* +1:8C041A10 */
    unsigned char mirror;   /* +2:8C041A1C */
    unsigned char fog;      /* +3:8C041A28 */
    unsigned char wind;     /* +4:8C041A34 */
} tOptionsCircuitTrack; /* stride5:8C041A06..A0E */
typedef char dc_check_options_player[(sizeof(tOptionsPlayer)==0x50)?1:-1];
typedef char dc_check_options_profile[(sizeof(tOptionsProfile)==0x40)?1:-1];
typedef char dc_check_options_save[(sizeof(tOptionsSaveData)==0x6fc)?1:-1];
typedef char dc_check_options_circuit_track[(sizeof(tOptionsCircuitTrack)==5)?1:-1];
extern tOptionsPlayer gPlayers[4]; /* four-player InitOptions loop; sibling state/demo evidence */
extern tOptionsCircuitTrack gCircuitTracks[];
extern unsigned char gCircuitPlayer;
extern signed char gCircuitTrack;
extern signed char optionsFlag_8C1224C4;
extern unsigned int gGameState;
extern int gRace;
extern signed char gOptions[];
extern signed short gCheatState[];
extern signed char gDriveHulk[];
extern signed short gNumPlayers,gNumWindows,gNumLaps,gNumDrones,gLang;
extern int gStuntMode,gStuntFlow,gStuntModeMins,gStuntModePoints,gStuntModeDeaths;
extern int gBattleMode,gBattleModeMins,gBattleModePoints;
extern signed char gBackwards; /* target 8C12C657 */
extern signed char gCarMines; /* target 8C158699 */
extern signed char gCatchup; /* target 8C14CBAE */
extern signed char gDeathMode; /* target 8C12C656 */
extern signed char gDemolitionBattle; /* target 8C15869B */
extern signed char gDifficulty; /* target 8C12C650 */
extern signed char gEnableTireScaling; /* target 8C15CFDE */
extern signed char gFogColor; /* target 8C12C658 */
extern signed char gFogIntensity; /* target 8C15FF20 */
extern signed char gFrameScale; /* target 8C15CFDD */
extern signed char gGhostSelect; /* target 8C12C651 */
extern signed char gGravityScale; /* target 8C14C5F8 */
extern signed char gInvincible; /* target 8C12C571 */
extern signed char gInvisibleCar; /* target 8C15CFDC */
extern signed char gInvisibleTrack; /* target 8C15FF2C */
extern signed char gMassiveMass; /* target 8C12BAB0 */
extern signed char gMineCones; /* target 8C158698 */
extern signed char gMirrorControls; /* target 8C12C10F */
extern signed char gMirrorWaves; /* target 8C15FA14 */
extern signed char gNoAutoAbort; /* target 8C1503E6 */
extern signed char gNoBrakes; /* target 8C12C110 */
extern signed char gNoCarCollisions; /* target 8C12C0D0 */
extern signed char gRandomWeapons; /* target 8C15869A */
extern signed char gResurrectInPlace; /* target 8C1503E7 */
extern signed char gShowCoins; /* target 8C13423B */
extern signed char gShowElapsed; /* target 8C134232 */
extern signed char gShowMetric; /* target 8C13423C */
extern signed char gShowOdometer; /* target 8C134236 */
extern signed char gShowPlace; /* target 8C134237 */
extern signed char gShowPlayerArrows; /* target 8C13423A */
extern signed char gShowRadar; /* target 8C134231 */
extern signed char gShowRemaining; /* target 8C134233 */
extern signed char gShowShifter; /* target 8C134238 */
extern signed char gShowSpeed; /* target 8C134235 */
extern signed char gShowTach; /* target 8C134234 */
extern signed char gShowTrackMap; /* target 8C134230 */
extern signed char gShowWrongWay; /* target 8C134239 */
extern signed char gSpecialTires; /* target 8C12C572 */
extern signed char gSuicideMode; /* target 8C12C0D1 */
extern signed char gSuperSpeed; /* target 8C12C570 */
extern signed char gUpsideDownTrack; /* target 8C12B8A8 */
extern signed char gWind; /* target 8C12C568 */
void SetControls(int resetToDefault);
void AudSetFxVol(float volume,float time);
void AudSetMusicVol(float volume,float time);
void SavePlayerOption(tOptionsProfile **player,unsigned char option,signed char value);
signed char GetDefaultOption(int opt);
#endif
#line 3 "options.c"
#define gPlayers ((tOptionsPlayer *)0x8C006000)

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
    gNumWindows = 1;
    gNumPlayers = 1;
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

