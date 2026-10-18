#ifndef DC_TITLESCR_TYPES_H
#define DC_TITLESCR_TYPES_H
#include "dc_types.h"
typedef struct tTitleText {char pad0[4];unsigned char **single;} tTitleText;
extern tTitleText gText; /*D154/D15A first pool pointsgText+4; index32 D1AA=80*/
extern unsigned int gSkipToGameState,gGameState,gActiveEdges;
extern signed char gGameStarted,gSoftReset,gIntroMovieShown; /*CFCA signed movie-count<2*/
#define gPlayers ((tPadState *)0x8C006000) /*D128/12C/130;byte+1,stride50*/
void setGameState(unsigned int);
void func_8C06C712(void); /*CFB4/D04E no argument; DC movie cleanup companion*/
int func_8C06CA0A(int); /*CF92/CFA2 idR4,result1/2 checked*/
void func_8C06C986(int); /*CFB0/D04A idR4; return ignored*/
void func_8C06C692(void); /*CFCE no argument*/
int func_8C06C808(const char *); /*CFE4 filenameR4, returns movie index or-1*/
_tBlit *NewMultiBlit(signed short,signed short,const tBlitDef *,signed short);
void SetCountdownTimer(float);
void ClearRushControls(void);
void RemoveBlit(_tBlit *);
int AudPlayEffect(int,int,int,unsigned char);
int CountdownTimeOut(void);
void HAL_SetTextDepth(float);
void TB_SetJust(int,int);
int HAL_SetFont(int);
void TB_SetTextColor(int);
void TB_DrawTextScrollBox(signed short,signed short,signed short,signed short,signed short,signed short,void *);
void TB_DrawText(signed short,signed short,void *);
#endif
