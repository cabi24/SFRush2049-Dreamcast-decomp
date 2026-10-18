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
