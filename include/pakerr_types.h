#ifndef PAKERR_TYPES_H
#define PAKERR_TYPES_H
#include "dc_types.h"
#include "text_types.h"
/* DC stride34 and row9c: pools373E,3FFE; full8-row initializer bound4e0 at3FFC. */
typedef struct tPakError {
 signed char done,choice1,choice2; /* 362C,364C;3FB8 */
 char pad03;
 void *name; int data; /* 3FAE/3FB0 */
 signed char cont; /* 4078 */
 signed char contActive[4],contStick[4],contDebug[4]; /* 3FBE..4040:0d,11,15 */
 signed char rumble; /* 40B4..B8 */
 char pad1a[2];
 int err; /* 3EF2/+1c */
 signed short left,top,width,height; /* 40BA..D0:+20,22,24,26 */
 unsigned int edges,levels,timeout; /* 3614/+28;41FE/4202;+30,36B0 */
} tPakError;
typedef char check_pakerr_size[(sizeof(tPakError)==0x34)?1:-1];
extern tPakError gPakError[8][3];
extern tText gText;
extern unsigned char *gPS_PakName[8],*gPS_ContName[4];
extern unsigned int gGameState,gActiveEdges,gActiveLevels;
extern volatile unsigned int gFrameCounter;
extern float gRetraceRate;
extern signed short gNumWindows;
void PlayLRSound(unsigned int);
void PlayStartABSound(unsigned int);
void AudPlayEffect(int,void *,int,int);
int HAL_SetFont(int);
void TB_SetTextColor(int);
void TB_SetJust(int,int);
signed short TB_GetFontLineHeight(void);
signed short TB_CenterText(void *,signed short);
void TB_DrawText(signed short,signed short,void *);
void TB_DrawTextScrollBox(signed short,signed short,signed short,signed short,signed short,signed short,void *);
void *TB_Strcpy(void *,void *);
void *TB_Strcat(void *,void *);
int GetControllerActive(int);
int GetControllerStickEdges(int);
void SetControllerActive(int,int);
void SetControllerStickEdges(int,int);
int PS_PakRumble(int);
void UpdateRushControls(void);
_tBlit *NewMultiBlit(signed short,signed short,const tBlitDef *,signed short);
void RemoveBlit(_tBlit *);
void UpdateBlit(_tBlit *);
void ZOID_GetFog(signed short *,signed short *,signed char *,signed char *,signed char *,signed char *,int);
void ZOID_SetFog(signed short,signed short,int,int,int,int,int);
void UpdateFrame(void);
void SaveServer(int,int);
#endif
