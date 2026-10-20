#ifndef GAMERECORDS_TYPES_H
#define GAMERECORDS_TYPES_H
#include "dc_types.h"
/* Stunt stride120 at02B23A/B264, totalPoints+14 atB27C/B282, signedcompareB284;
   maxStuntPoints+8 at8C02BF22, totalStunts+48 at8C02BF70 (add#72). */
typedef struct dcRecordsStuntPlayer {
 char pad00[0x08];
 int maxStuntPoints;
 char pad0c[0x14-0x0c];
 int totalPoints;
 char pad18[0x48-0x18];
 signed short totalStunts[10];
 char pad5c[120-0x5c];
} dcRecordsStuntPlayer;
typedef char records_check_stunt[(sizeof(dcRecordsStuntPlayer)==120)?1:-1];
#define gStuntPlayer ((dcRecordsStuntPlayer *)0x8C14FBE4) /* FindWinner8C02B232 hoists the address into r14 with the stride in r7 (mul.l), the gGameCar constant pattern. */
extern tModelData model[6];
#define gGameCar ((tCarData *)0x8C004000) /* Fixed DC arena (canonical model/circuit lesson); RecordPractice8C02BD42/Obstacle8C02C170 hoist the constant. */
#define gPlayers ((tPadState *)0x8C006000) /* Fixed DC arena: pool8C02B760. */
extern tPlayerProfile *gJustPlayers[4];
extern signed short gNumWindows,gNumPlayers,gNumActiveCars,gNumLaps;
extern int gRace,ties,gStuntModeMins;
extern unsigned int gGameState;
extern signed char gRankedPlayer[4],tied[4],gCarTeam[4],gTeamPoints[4];
extern signed char gTrackNum,gBackwards,gDeathMode;
extern unsigned char gLap[4],gCircuitPlayer;
extern float gBestLapTime[4],gLapTimes[4][8];
extern signed char gBattlePoints[4][4];
extern signed char gNewTotalScore[3][5]; /* 8C12D314: ResetHighScores8C02C4BA..DC. */
extern signed short gCheatState[];
extern tCircuitCar gCircuitCars[6];
extern signed char gForceUnlockedCircuits[4];
extern signed char gUnlockedCircuits[4][4],gUnlockedTracks[4][19],gUnlockedFrames[4][6];
extern signed char gUnlockedTires[4][5],gUnlockedEngines[4][9],gUnlockedTrans[4][8],gUnlockedCars[4][13];
extern tRaceStats gTotalRaceStats[12];
extern tStuntStats gTotalStuntStats[4];
extern tBattleStats gTotalBattleStats[8];
extern tObstacleStats gTotalObstacleStats[1];
extern tCircuitStats gTotalCircuitStats[4];
extern tPlayerProfile **gTotalNames[12][3][5]; /* targetB122/B12C end+2D0. */
extern unsigned char gTempNames[20][13]; /* B15A length104. */
extern unsigned short gTempNameAges[20]; /* B164 length28. */
void *memset(void *,int,unsigned int);
int strcmp(const char *,const char *);
#define strcmp _builtin_strcmp /* FindTempNameIdx8C02DE1A reloads the helper address per iteration (builtin expansion). */
char *strcpy(char *,const char *);
#define strcpy _builtin_strcpy /* FindTempNameIdx8C02DE4C..58: dest r0, src r1 helper ABI; strcmp stays a normal call (r4/r5). */
void SavePlayerStats(tPlayerProfile **,unsigned char);
void SavePlayerCoins(tPlayerProfile **,unsigned char);
void SavePlayerCircuitStats(tPlayerProfile **,unsigned char);
int CountBits(unsigned int,unsigned char);
#define kOneRace 0
#define kPractice 1
#define kGhost 2
#define kCircuit 3
#define kStunt 4
#define kObstacle 5
#define kBattle 6
#endif
