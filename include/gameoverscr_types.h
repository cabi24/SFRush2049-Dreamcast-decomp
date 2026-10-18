#ifndef DC_GAMEOVERSCR_TYPES_H
#define DC_GAMEOVERSCR_TYPES_H
#include "dc_types.h"

typedef _tBlit _tBlit_2;
typedef struct tBlitSet tBlitSet;

typedef struct tGameoverText {
 char pad0[4];
 unsigned char **single; /* +4:8C02935C */
 char pad1[0xc-8];
 unsigned short *tab; /* +c:8C0293C0/6; tab46 offset5c */
 unsigned char **taba; /* +10:8C0293C8 */
} tGameoverText;
typedef struct tGameoverStuntPlayer {
 char pad0[8];
 int maxStuntPoints; /* +8, stride78:pool8C02A0EC and8C029FFE..A00E */
 char pad1[0x14-0xc];
 int totalPoints; /* +14:pool8C02A0E0 */
 char pad2[0x48-0x18];
 signed short totalStunts[10]; /* +48:pool8C02AC2C and8C02ABAE..B4 */
 char pad3[0x78-0x5c]; /* stride78:8C029FDE and8C02AB70 */
} tGameoverStuntPlayer;
typedef char checkGameoverStunt[(sizeof(tGameoverStuntPlayer)==0x78)?1:-1];
typedef char checkGameoverBlitDef[(sizeof(tBlitDef)==0x24)?1:-1];
enum {kGameOverStateNone=0,kGameOverStateMain=1,kGameOverStateScore=2,kGameOverStateCount=3};
enum {kGameOverMainMenuScore=0,kGameOverMainMenuRaceAgain=1,kGameOverMainMenuNewMode=2,kGameOverMainMenuNewTrack=3,kGameOverMainMenuNewCar=4,kGameOverMainMenuNextRace=5,kGameOverMainMenuQuit=6,kGameOverMainMenuCount=7};
enum {kOneRace=0,kPractice=1,kGhost=2,kCircuit=3,kStunt=4,kObstacle=5,kBattle=6};
extern int gRace;
extern unsigned int gActiveEdges,gActiveAutoEdges;
extern unsigned int gSkipToGameState;
extern signed char gSoftReset,gTrackNum,gRestartGame,gNumActiveCars,gRankedPlayer[4],gCarTeam[4],gBattlePoints[4][4];
extern unsigned char gCircuitPlayer,gLap[4];
extern signed short gNumWindows,gNumPlayers,gNumCars,gNumLaps;
extern tGameoverText gText;
extern int playerTextColor[4];
extern tPlayerProfile *gGhostPlayer;
extern float gBestLapTime[4],gLapTimes[4][8];
extern tGameoverStuntPlayer gStuntPlayer[4];
extern tModelData model[6];
/* Separate fixed base and field formation:pool8C029568,8C029B80. */
#define gGameCar ((tCarData *)0x8C004000)
#define gPlayers ((tPadState *)0x8C006000)
void BlinkResurrectingCars(void);
void AudKillSong(void);
void UnloadTrack(signed char);
void RemoveEnvirons(void);
int ZOID_SplitScreen(int);
void RemoveMultiplayerHUD(void);
void ClearScreen(int);
void setGameState(unsigned int);
void PlayStartABSound(unsigned int);
void PlayUDSound(unsigned int);
void AudStopAllEffects(void);
void AbortGame(void);
_tBlit *NewMultiBlit(signed short,signed short,const tBlitDef *,signed short);
void RemoveBlit(_tBlit *);
void RemovePanes(int);
tBlitSet *InitPane(int,int,int,int,int,int (*)(_tBlit *),unsigned int,int);
int HAL_SetFont(int);
unsigned int HAL_GetTextWidth(void *,signed short);
void HAL_SetTextDepth(float);
signed short TB_GetFontLineHeight(void);
void TB_SetJust(int,int);
void TB_SetTextColor(int);
void TB_DrawText(signed short,signed short,void *);
void TB_DrawEllipseText(signed short,signed short,signed short,void *);
void TB_Sprintf(void *,void *,...);
void DC_SendFillRect(int,int,int,int,int,int,int,int,float);
#endif
