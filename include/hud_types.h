/* Private HUD target-backed overlays. */
#ifndef HUD_TYPES_H
#define HUD_TYPES_H
#include "dc_types.h"
typedef struct tHudCoin {
 signed short idx; /* ShowHUD 8C0347CA..E6, RemoveHUD 8C034B12..38. */
 char pad0[2];
 tUVMatrix mat; /* Coin stride0x34; matrix type/extent existing common30. */
} tHudCoin;
typedef struct tBlitSet tBlitSet;
typedef struct tHudText {
 char pad0[4];
 unsigned char **single; /* +4: CheckResults 8C03518C..192, text indices pools523C..40. */
 char pad1[0x0c-8];
 unsigned short *tab; /* ShowHUD 8C034922..930; absolute gText+0c. */
 unsigned char **taba; /* ShowHUD 8C03492A..93C; absolute gText+10. */
} tHudText;
typedef char dc_hud_coin_stride[(sizeof(tHudCoin)==0x34)?1:-1];
#define kBattle 6
#define kStunt 4
extern unsigned int gGameState;
extern int gPauseState,gRace,gShowCredits;
extern signed short gNumWindows,markers_valid;
extern float gCheckPtTimeout[4],gLapTime[4],gElapsedTime[4];
extern _tBlit *HudBlit1,*HudBlit2,*multiPlayerBlit,*ResultBlit;
extern tHudCoin gPlayerCoin[4][2];
extern tBlitSet *resultPane[5];
extern int gCredit,gCorner,gCreditWidth,gNumCreditNames;
extern volatile float gSeconds,gFrameSeconds; /* MAT3 IRQ timer semantics; DC load order 48E0..EC/4BBE..C8. */
extern float hudObjAngle;
extern unsigned char *cStrCreditNames[14][11];
extern tHudText gText;
extern const tBlitDef RaceMultiBlit[90],HudMultiBlit[5],TwoPlayerMultiBlit[1],ThreePlayerMultiBlit[1],OverlayMultiBlit[1];
extern _tBlit *NewMultiBlit(signed short,signed short,const tBlitDef *,signed short);
extern void RemoveBlit(_tBlit *);
extern void ShowHUDExtra(void),RemoveHUDExtra(void),UpdateHUDExtraPostCamera(void);
extern void _AddBattlePoint(signed char,signed char,signed char);
extern void RemovePanes(int);
extern tBlitSet *InitPane(int,int,int,int,int,int (*)(_tBlit *),unsigned int,int);
extern void SetCountdownTimer(float);
extern float GetCountdownTimeRemaining(void),Random(float);
extern void HAL_SetFont(int);
extern unsigned int HAL_GetTextWidth(unsigned char *,signed short);
extern void ZOID_RemoveObject(signed short);
extern int animCoins();
#define kObstacle 5
#define gGameCar ((tCarData *)0x8C004000) /* Fixed DC arena: target4D18/4FAC; canonical lesson model/circuit. */
extern tModelData model[6];
#define gPlayers ((tPadState *)0x8C006000) /* Fixed DC arena: target4E60/524C. */
#define gModelTime (*(float *)0x8C14CB64) /* updateFrame target4D20. */
extern signed char gNumActiveCars,dot_order_list[6],gDeathMode,gRankedPlayer[4];
extern signed short num_humans,gNumLaps;
#define TrackTimeStr ((unsigned char (*)[8])0x8C134304) /* updateFrame target4D14, successive +8 outputs. */
extern unsigned char gLap[4];
extern unsigned char playerPaneColors[4][2][4];
extern int wrongWayPos[4][4][2],ties;
extern const tBlitDef ResultsMultiBlit[1];
typedef struct tHudStuntPlayer {
 char pad0[0x14];
 int totalPoints; /* CheckResults 8C035404..414: 0x78 stride, base gStuntPlayer+14=8C14FBF8. */
 char pad1[0x78-0x18];
} tHudStuntPlayer;
extern tHudStuntPlayer gStuntPlayer[4];
extern unsigned char cvt_time(float,unsigned char *,char);
extern void ChangePaneColor(tBlitSet *,unsigned char *,unsigned char *);
extern void ChangePaneSize(tBlitSet *,int,int,int,int);
extern signed short TB_GetFontLineHeight(void);
extern unsigned int TB_Strlen(void *);
extern void TB_Sprintf(void *,void *,...);
#endif
