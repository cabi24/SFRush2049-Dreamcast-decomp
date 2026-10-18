#ifndef DC_AUDIOSCR_TYPES_H
#define DC_AUDIOSCR_TYPES_H
#include "dc_types.h"
typedef struct tBlitSet tBlitSet;
typedef struct tMenuButton {
    int buttonType,objIdx; float angle; tUVMatrix mat;
    unsigned char alpha; char pad0[3];
} tMenuButton; /*64-byte initialized records8C0BAECC..B1CC; init17F82/80DA */
typedef struct anon_x_w_10 {int x,w,h,cx;} anon_x_w_10;
typedef struct tAudioText {
    char pad0[4]; unsigned char **single;
    char pad1[4]; unsigned short *tab; unsigned char **taba;
} tAudioText;
typedef struct tAudioPadInfo {
    volatile signed char attached; char pad0[0x14-1];
} tAudioPadInfo; /*repeated no-call loads172C8/2D2; stride14 at172BC..2C6*/
typedef char checkAudioButton[(sizeof(tMenuButton)==64)?1:-1];
extern int gSong,gLastSong,gAudioMenuSelect,exitAudioScreen;
extern signed char inited,panesCreated,gControllerRemoved;
extern _tBlit *blit;
extern tBlitSet *gTitlePane;
extern anon_x_w_10 menuRect;
extern float gAudioMenuCursorSpeed;
extern int dcAudioMode;
extern volatile unsigned int dcAudioLastFrame,gFrameCounter;
extern unsigned char dcAudioBuffer[];
int func_8C0D84B8(void *); /*84B8 validates and stores pointer, unsigned extent unknown*/
int func_8C0D84CA(void); /*84CA clears stored pointer; no argument*/
int func_8C0D850A(int *); /*17444 call; success0 permits int result stack load17450*/
extern unsigned int gGameState,gActiveEdges,gContEdges[4];
extern float gContStick[4][2];
extern int playerInPause;
extern signed char gOptions[40],gSoftReset;
extern tAudioPadInfo gControllerData[4];
extern tAudioText gText;
extern unsigned char *gSongNames[20];
extern tCamera gCamera[4];
extern volatile unsigned char gNumModels;
extern volatile float gFrameSeconds;
extern float unitmatrix[3][3];
extern tRGBA flashColors[2];
void AudKillSong(void);
void AudSetMusicVol(float,float);
void AudSetFxVol(float,float);
int AudPlaySong(int,int);
void AudMono(int);
void StartGameMusic(void);
void PlayStartABSound(unsigned int);
void PlayUDSound(unsigned int);
void PlayLRSound(unsigned int);
void SaveGlobals(void);
void createBG(void);
void removeBG(void);
void ClearRushControls(void);
void setGameState(unsigned int);
void setPauseState(int);
_tBlit *NewMultiBlit(signed short,signed short,const tBlitDef *,signed short);
void RemoveBlit(_tBlit *);
tBlitSet *InitPane(int,int,int,int,int,int (*)(_tBlit *),unsigned int,int);
void ChangePaneSize(tBlitSet *,int,int,int,int);
void RemovePanes(int);
int HAL_SetFont(int);
unsigned int HAL_GetTextWidth(void *,signed short);
void HAL_SetTextDepth(float);
void HAL_SetTextTranslucency(float);
signed short TB_GetFontLineHeight(void);
/* SHC places the last fixed vararg parameter on the stack. Frozen TB_text
 * definition/prologue confirms two fixed parameters;174E6/F2/F4 call matches. */
void TB_Sprintf(void *,void *,...);
void TB_DrawText(signed short,signed short,void *);
signed short TB_CenterText(void *,signed short);
signed short TB_RightText(void *,signed short);
void TB_SetTextColor(int);
void TB_SetJust(int,int);
void TB_Strcpy(void *,void *);
void TB_FlashTextColors(tRGBA,tRGBA);
void ZOID_ProjectWindow(int,float *,tUVMatrix *,float *,signed short *);
void *memcpy(void *,const void *,unsigned int);
int MBOX_FindObject_WORK(const char *,signed char,signed char,int);
signed short MBOX_NewObject(int,tUVMatrix *,int,unsigned int);
tTexture *MBOX_FindTexture_WORK(const char *,unsigned short *,signed char,signed char,int);
void ZOID_SetColor(signed short,tRGBA *);
void ZOID_SetTexture(signed short,tTexture *,int);
void ZOID_RemoveObject(signed short);
void MBOX_ShowObject(int,int,int);
void MBOX_HideObject(int,int,int);
void ZeroCameras(void);
void WPitchUV(float,float (*)[3]);
void WRollUV(float,float (*)[3]);
void ScaleUV(float (*)[3],float (*)[3],float);
#define fabs _builtin_fabsf
#endif
