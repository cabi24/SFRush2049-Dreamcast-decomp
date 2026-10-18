#ifndef DC_MODESCR_TYPES_H
#define DC_MODESCR_TYPES_H
#include "dc_types.h"
typedef struct tMenuButton {int buttonType,objIdx;float angle;tUVMatrix mat;unsigned char alpha;char pad0[3];} tMenuButton;
typedef char checkModeButton[(sizeof(tMenuButton)==64)?1:-1]; /*E26C stride/extent340*/
struct tPS_File {char pad0[0x10];unsigned char pak;}; /*DD3E..48 offset10,extu.b*/
typedef struct tModeText {char pad0[4];unsigned char **single;char pad1[4];unsigned short *tab;unsigned char **taba;} tModeText;
extern tModeText gText; /*DF88+4,D FCC+c/DFD0+10;index21and23*/
#define gPlayers ((tPadState *)0x8C006000) /*DD2C+4C,DE58fixed6004c*/
extern signed short gNumPlayers;
extern unsigned int gSkipToGameState,gGameState,gActiveEdges;
extern signed char gSoftReset;
extern int gRace,exitModeScreen,gModeMenuSelect;
extern _tBlit *gModeMenuBlit;
extern signed char gModeMenuInited;
extern signed char validOption[4][7]; /*E1CE..E1E6 row7,indexed signed byte*/
extern float gButtonScale,gButtonSpace,maxY,buttonZ,cursorZ,buttonX,gModeMenuCursorSpeed;
extern tCamera gCamera[4];
extern unsigned char gNumModels;
extern volatile float gFrameSeconds; /*E6B8/E6DA repeated nocallloads*/
extern float unitmatrix[3][3];
extern tRGBA flashColors[2];
void setGameState(unsigned int);
void PlayStartABSound(unsigned int);
void PlayUDSound(unsigned int);
int PS_PakChanged(unsigned char);
void SaveFlush(int);
void createBG(void);
void removeBG(void);
void ClearRushControls(void);
unsigned char GetPlayerRaceType(tPlayerProfile **); /*DE5E/DE60 extu.b*/
int IsTrackUnlocked(int);
_tBlit *NewMultiBlit(signed short,signed short,const tBlitDef *,signed short);
void RemoveBlit(_tBlit *);
void HAL_SetTextDepth(float);
int HAL_SetFont(int);
void HAL_SetTextTranslucency(float);
void TB_SetTextColor(int);
void TB_SetJust(int,int);
signed short TB_CenterText(void *,signed short);
void TB_DrawText(signed short,signed short,void *);
signed short TB_GetFontLineHeight(void);
void TB_Sprintf(void *,void *,...);
void TB_FlashTextColors(tRGBA,tRGBA);
void ZOID_ProjectWindow(int,float *,tUVMatrix *,float *,signed short *);
void *memcpy(void *,const void *,unsigned int);
int MBOX_FindObject_WORK(const char *,signed char,signed char,int);
signed short MBOX_NewObject(int,tUVMatrix *,int,unsigned int);
void ZOID_SetColor(signed short,tRGBA *);
tTexture *MBOX_FindTexture_WORK(const char *,unsigned short *,signed char,signed char,int);
void ZOID_SetTexture(signed short,tTexture *,int);
void ZOID_RemoveObject(signed short);
void MBOX_ShowObject(int,int,int);
void MBOX_HideObject(int,int,int);
void ZeroCameras(void);
void WPitchUV(float,float (*)[3]);
void WRollUV(float,float (*)[3]);
void ScaleUV(float (*)[3],float (*)[3],float);
void CopyUV(float (*)[3],float (*)[3]);
#define fabs _builtin_fabsf
#endif
