#ifndef DC_CHEATSCR_TYPES_H
#define DC_CHEATSCR_TYPES_H
#include "dc_types.h"
typedef struct tCheatText {
    char pad0[4]; unsigned char **single; /*A804: +4*/
    char pad1[0x1c-8];
    unsigned short *tab2; /*A89A +1c, A89E/A8A2 unsigned16*/
    unsigned short *tab2a; /*A8A0 +20, A8A8/A8AE unsigned16*/
    unsigned char **tab2b; /*A8B2 +24, A8B8/B A stride4*/
} tCheatText;
typedef char checkCheatText[(sizeof(tCheatText)==0x28)?1:-1];
extern tCheatText gText;
extern unsigned int gActiveEdges,gActiveAutoEdges;
extern signed char gSoftReset;
extern signed char gCheatActive[]; /*A2BE byte, index bounded0..22; allocation unknown*/
extern signed short gCheatState[]; /*A366/A36C/A4E6 store16, A8B0 signed16*/
extern signed char gNoCarCollisions,gMineCones,gCarMines,gNoAutoAbort,gSuperSpeed;
extern signed char gInvincible,gInvisibleCar,gInvisibleTrack,gNoBrakes,gSpecialTires;
extern signed char gMassiveMass,gSuicideMode,gDemolitionBattle,gRandomWeapons;
extern signed char gResurrectInPlace,gFrameScale,gEnableTireScaling,gFogColor;
/* Byte read/store targets A356..A4F2; signed compare cycles A3FC/A404/A4DA. */
int AudPlayEffect(int,int,int,unsigned char);
void InitEgg(int);
int CheckForEgg(void);
void PlayLRSound(unsigned int);
void createBG(void);
void removeBG(void);
void setGameState(unsigned int);
_tBlit *NewMultiBlit(signed short,signed short,const tBlitDef *,signed short);
void RemoveBlit(_tBlit *);
int HAL_SetFont(int);
unsigned char HAL_GetFontAscent(void); /*A80E extu.b after A80A call*/
void HAL_SetTextDepth(float);
void TB_SetTextColor(int);
void TB_DrawText(signed short,signed short,void *);
signed short TB_CenterText(void *,signed short);
signed short TB_RightText(void *,signed short);
signed short TB_GetFontLineHeight(void);
/* Frozen TB_text verifies SHC's last fixed fmt argument lives on entrySP. */
void TB_Sprintf(void *,void *,...);
#endif
