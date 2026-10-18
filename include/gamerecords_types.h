#ifndef GAMERECORDS_TYPES_H
#define GAMERECORDS_TYPES_H
#include "dc_types.h"
/* Stunt stride120 at02B23A/B264, totalPoints+14 atB27C/B282, signedcompareB284. */
typedef struct dcRecordsStuntPlayer {
 char pad00[0x14];
 int totalPoints;
 char pad18[120-0x18];
} dcRecordsStuntPlayer;
typedef char records_check_stunt[(sizeof(dcRecordsStuntPlayer)==120)?1:-1];
extern dcRecordsStuntPlayer gRecordsStuntPlayer[];
extern tModelData model[6];
extern tCarData gGameCar[6];
extern signed short gNumWindows,gNumPlayers;
extern int gRace,ties;
extern signed char gRankedPlayer[4],tied[4],gCarTeam[4],gTeamPoints[4];
/* Opaque target extents only, no unproven statistics fields:memsetB178..B1A0. */
extern unsigned char gTotalRaceStats[12][96],gTotalStuntStats[4][64];
extern unsigned char gTotalBattleStats[8][12],gTotalObstacleStats[1][24],gTotalCircuitStats[4][28];
extern tPlayerProfile **gTotalNames[12][3][5]; /* targetB122/B12C end+2D0. */
extern unsigned char gTempNames[20][13]; /* B15A length104. */
extern unsigned short gTempNameAges[20]; /* B164 length28. */
void *memset(void *,int,unsigned int);
#define kStunt 4
#define kBattle 6
#define kPractice 1
#endif
