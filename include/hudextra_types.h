#ifndef HUDEXTRA_TYPES_H
#define HUDEXTRA_TYPES_H
#include "dc_types.h"
#include "text_types.h"
#include "hudextra_zoidblit_types.h"
typedef _tBlit _tBlit_2;
typedef struct tBlitSet tBlitSet;
#define kBattle 6
#define kStunt 4
/* Full0x78 stride also verified by integrated stunt. Own points/time/show
 * accesses93D30..60 and942C8..E6 agree with those fields. */
typedef struct tStuntPlayer {
 signed short show,crash;
 float time;
 int maxStuntPoints,points,pointsHud,totalPoints,numstunts,lastStuntType;
 signed short stunts[10],stuntsHud[10],totalStunts[10];
 char pad5c[4];
 float wingTime;
 signed char wingBonus,wingBonusHud,doneBonus,doneBonusHud;
 int bonus,bonusHud,showBonus,done;
} tStuntPlayer;
/* DC coin stride48, not MAT3's54: show loop93A3E and94344/944AC.
 * Matrix+4; alpha+34 945B6; screen38 9452E, start3c/dest40
 * 943C4/943F0 and94454..A8; scrollTime44 94458/94524. */
typedef struct dcHudStuntCoin {
 signed short type,idx;
 tUVMatrix mat;
 float alpha;
 signed short screenPos[2],startPos[2],destPos[2];
 float scrollTime;
} dcHudStuntCoin;
typedef struct dcHudIcon {signed short type,idx;tUVMatrix mat;} dcHudIcon;
typedef struct dcHudBattleCoin {signed short idx;char pad2[2];tUVMatrix mat;} dcHudBattleCoin;
/* 95064..7C stride18, 95120..32 byte fields and9514C..19C floats. */
typedef struct dcHudBattlePoint {
 signed char active,cnt,plfrom,plto;
 float startx,starty,endx,endy;
 signed char p;
 char pad15[3];
} dcHudBattlePoint;
/* Only palBase+14 is named; stride18 multiply and store94894..A8. */
struct anon_name_start_18 {char pad0[20];char *palBase;};
/* Recovered byte-aligned explicit RGBA initialization at94A84..A98. */
typedef struct dcHudByteColor {unsigned char c[4];} dcHudByteColor;
typedef char hudextra_stunt_player[(sizeof(tStuntPlayer)==0x78)?1:-1];
typedef char hudextra_coin[(sizeof(dcHudStuntCoin)==0x48)?1:-1];
typedef char hudextra_icon[(sizeof(dcHudIcon)==0x34)?1:-1];
typedef char hudextra_battle_coin[(sizeof(dcHudBattleCoin)==0x34)?1:-1];
typedef char hudextra_point[(sizeof(dcHudBattlePoint)==0x18)?1:-1];
typedef char hudextra_pal[(sizeof(struct anon_name_start_18)==0x18)?1:-1];
extern unsigned int gGameState;
extern int gRace,gPauseState;
extern signed char gOutOfTime;
extern int playerTextColor[4];
extern signed short gNumWindows,gNumPlayers;
extern unsigned char gNumModels;
extern signed char gOptions[],gCarTeam[4];
extern float gModelTime,hudObjAngle;
extern volatile float gFrameSeconds;
extern tCamera gCamera[4];
extern tStuntPlayer gStuntPlayer[4];
extern tText gText;
#define gGameCar ((tCarData *)0x8C004000)
#define gPlayers ((tPadState *)0x8C006000)
extern tZoidObjectDC ObjInstanceList[];
extern unsigned short gObjList[];
extern anon_palette_texture_20 gBlitInstanceList[];
extern int wrongWayPos[4][4][2];
/* External BSS starts/widths proved by pool and indexed strides; no BSS credit. */
extern dcHudStuntCoin stuntCoin[4][10];
extern dcHudIcon weaponIcon[4],powerupIcon[4];
extern dcHudBattleCoin battleCoin[4];
extern dcHudBattlePoint battlePoint[4];
extern signed char battlePointCnt;
extern _tBlit *gHudExtraBlit;
extern anon_name_start_18 healthPal[4];
extern int bonuscnt[4];
/* Root-approved final anonymous function; callback330C proves entry. */
int func_8C0951C0();
int animSpeed(),animSpeedText(),animTach(),animGear(),animCountdownTimer();
void RecordBattlePoints(signed char,signed char,signed char);
_tBlit *NewMultiBlit(signed short,signed short,const tBlitDef *,signed short);
void RemoveBlit(_tBlit *);int Hidden(_tBlit *,int);void UpdateBlit(_tBlit *);
tBlitSet *InitPane(int,int,int,int,int,int (*)(_tBlit *),unsigned int,int);
void ChangePaneColor(tBlitSet *,unsigned char *,unsigned char *);
int HAL_SetFont(int);unsigned int HAL_GetTextWidth(void *,signed short);
void HAL_SetTextDepth(float);void HAL_SetTextTranslucency(float);
void TB_SetJust(int,int);void TB_SetTextColor(int);
signed short TB_GetFontLineHeight(void),TB_CenterText(void *,signed short);
void TB_DrawText(signed short,signed short,void *);
void TB_Sprintf(void *,void *,...);
void TB_DrawTextScrollBox(signed short,signed short,signed short,signed short,signed short,signed short,void *);
signed short MBOX_NewObject(int,tUVMatrix *,int,unsigned int);
int MBOX_FindObject_WORK(const char *,signed char,signed char,int);
tTexture *MBOX_FindTexture_WORK(const char *,unsigned short *,signed char,signed char,int);
void MBOX_ShowObject(int,int,int);void MBOX_HideObject(int,int,int);
void ZOID_RemoveObject(signed short);void ZOID_SetTexture(signed short,tTexture *,int);
void ZOID_SetColor(signed short,void *);
void ZOID_WindowTo3D(int,signed short *,tUVMatrix *,float,float *);
void ScaleUV(float (*)[3],float (*)[3],float);
void YawUV(float,float (*)[3]);void PitchUV(float,float (*)[3]);
#endif
