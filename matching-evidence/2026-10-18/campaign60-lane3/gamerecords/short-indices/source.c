#include "gamerecords_types.h"

void func_8C02B174(void);

/* Anonymous map identity retained; target loops twelve tracks,three groups,five scores. */
void func_8C02B114(void)
{
 int i,j,k;
 func_8C02B174();
 for(i=0;i<12;i++)
  for(j=0;j<3;j++)
   for(k=0;k<5;k++)
    gTotalNames[i][j][k]=(tPlayerProfile **)-1;
 memset(gTempNames,0,sizeof(gTempNames));
 memset(gTempNameAges,0,sizeof(gTempNameAges));
}

/* Anonymous map identity retained; each target memset size proves this opaque extent. */
void func_8C02B174(void)
{
 memset(gTotalRaceStats,0,sizeof(gTotalRaceStats));
 memset(gTotalStuntStats,0,sizeof(gTotalStuntStats));
 memset(gTotalBattleStats,0,sizeof(gTotalBattleStats));
 memset(gTotalObstacleStats,0,sizeof(gTotalObstacleStats));
 memset(gTotalCircuitStats,0,sizeof(gTotalCircuitStats));
}

void FindWinner(void)
{
    signed short i;
    signed short j;
    ties = 0;
    for (i = 0; i < 4; i++) tied[i] = 0;
    for (i = 0; i < gNumWindows; i++) gRankedPlayer[i] = i;
    if (gRace == kStunt) {
        for (i = 0; i < gNumPlayers - 1; i++) {
            for (j = 0; j < gNumPlayers - 1; j++) {
                if (gRecordsStuntPlayer[(int)model[gRankedPlayer[j]].slot].totalPoints < gRecordsStuntPlayer[(int)model[gRankedPlayer[j+1]].slot].totalPoints) {
                    signed char temp = gRankedPlayer[j+1];
                    gRankedPlayer[j+1] = gRankedPlayer[j];
                    gRankedPlayer[j] = temp;
                }
            }
        }
        for (i = 1; i < gNumPlayers; i++) {
            if (gRecordsStuntPlayer[(int)model[gRankedPlayer[0]].slot].totalPoints == gRecordsStuntPlayer[(int)model[gRankedPlayer[i]].slot].totalPoints) {
                ties++;
                tied[i] = 1;
            }
        }
    } else if (gRace == kBattle) {
        for (i = 0; i < gNumPlayers - 1; i++) {
            for (j = 0; j < gNumPlayers - 1; j++) {
                if (gGameCar[(int)model[gRankedPlayer[j]].slot].battle.Points < gGameCar[(int)model[gRankedPlayer[j+1]].slot].battle.Points) {
                    signed char temp = gRankedPlayer[j+1];
                    gRankedPlayer[j+1] = gRankedPlayer[j];
                    gRankedPlayer[j] = temp;
                }
            }
        }
        for (i = 0; i < gNumPlayers - 1; i++) {
            for (j = 0; j < gNumPlayers - 1; j++) {
                if (gTeamPoints[gCarTeam[model[gRankedPlayer[j]].slot]] < gTeamPoints[gCarTeam[model[gRankedPlayer[j+1]].slot]]) {
                    signed char temp = gRankedPlayer[j+1];
                    gRankedPlayer[j+1] = gRankedPlayer[j];
                    gRankedPlayer[j] = temp;
                }
            }
        }
        for (i = 1; i < gNumPlayers; i++) {
            if (gGameCar[(int)model[gRankedPlayer[0]].slot].battle.Points == gGameCar[(int)model[gRankedPlayer[i]].slot].battle.Points) {
                ties++;
                tied[i] = 1;
            }
        }
    } else if (gRace != kPractice) {
        for (i = 0; i < gNumPlayers - 1; i++) {
            for (j = 0; j < gNumPlayers - 1; j++) {
                if (gGameCar[(int)model[gRankedPlayer[j]].slot].place > gGameCar[(int)model[gRankedPlayer[j+1]].slot].place) {
                    signed char temp = gRankedPlayer[j+1];
                    gRankedPlayer[j+1] = gRankedPlayer[j];
                    gRankedPlayer[j] = temp;
                }
            }
        }
        for (i = 1; i < gNumPlayers; i++) {
            if (gGameCar[(int)model[gRankedPlayer[0]].slot].place == gGameCar[(int)model[gRankedPlayer[i]].slot].place) {
                ties++;
                tied[i] = 1;
            }
        }
    }
}

