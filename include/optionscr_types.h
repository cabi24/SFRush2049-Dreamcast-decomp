#ifndef DC_OPTIONSCR_TYPES_H
#define DC_OPTIONSCR_TYPES_H
#include "dc_types.h"

typedef _tBlit _tBlit_2;
typedef struct tBlitSet tBlitSet;
 /* stride24: finalized blit8C018B86..96; descriptors8C0AC1AC/1D0 */
typedef struct tMenuButton {
    int buttonType; /* +0:8C042B80 */
    int objIdx; /* +4:8C042B70,8C042EDE */
    float angle; /* +8:8C0427E6..EA */
    tUVMatrix mat; /* +c:8C042C16..1E; pos+30/34/38:8C042DA4/AC/B4 */
    unsigned char alpha; /* +3c:8C0427FC..80C unsigned conversion */
    char pad0[0x40-0x3d]; /* stride40:8C042C6E/70 and bound800:8C042EC6 */
} tMenuButton;
typedef struct tOptionsPadInfo {
    volatile signed char attached; /* +0, stride14:8C041E9E..AE and reread8C041EB8 */
    char pad0[0x14-1];
} tOptionsPadInfo;
typedef struct tOptionsText {
    char pad0[4];
    unsigned char **single; /* +4:8C042304 */
    char pad1[0xc-8];
    unsigned short *tab; /* +c:8C04242E; +34 gives tab17 */
    unsigned char **taba; /* +10:8C042434; element indexed words */
} tOptionsText;
typedef char checkOptionsButton[(sizeof(tMenuButton)==0x40)?1:-1];
typedef char checkOptionsPad[(sizeof(tOptionsPadInfo)==0x14)?1:-1];
typedef char checkOptionsBlitDef[(sizeof(tBlitDef)==0x24)?1:-1];
extern unsigned int gGameState,gActiveEdges,gActiveAutoEdges,gContEdges[4],gAutoEdges[4];
extern int playerInPause;
extern signed char gSoftReset,gOptions[40];
extern signed short gLang;
extern tOptionsPadInfo gControllerData[4];
extern tOptionsText gText;
extern tCamera gCamera[4];
extern tRGBA flashColors[2];
extern volatile unsigned char gNumModels;
extern volatile float gFrameSeconds;
extern float unitmatrix[3][3];
/* Discontiguous target locations; allocation ownership/bounds remain unclaimed. */
extern signed short gOptionMenuSize,gMenuCount,gFirstOption;
extern float optionsCursorSpeed,optionsScrollSpeed;
void PlayStartABSound(unsigned int);
void PlayUDSound(unsigned int);
void PlayLRSound(unsigned int);
void LoadTextWave(void);
void SetOptions(void);
void setGameState(unsigned int);
void setPauseState(int);
void SaveGlobals(void);
void createBG(void);
void removeBG(void);
void ClearRushControls(void);
_tBlit *NewMultiBlit(signed short,signed short,const tBlitDef *,signed short);
void RemoveBlit(_tBlit *);
int HAL_SetFont(int);
unsigned int HAL_GetTextWidth(void *,signed short);
void HAL_SetTextDepth(float);
void HAL_SetTextTranslucency(float);
signed short TB_GetFontLineHeight(void);
signed short TB_CenterText(void *,signed short);
void TB_Sprintf(void *,void *,...);
void TB_DrawText(signed short,signed short,void *);
void TB_SetTextColor(int);
void TB_SetJust(int,int);
/* DC8C05A7FC/802 reads two RGBA values from incoming stack; no pointers. */
void TB_FlashTextColors(tRGBA,tRGBA);
tBlitSet *InitPane(int,int,int,int,int,int (*)(_tBlit *),unsigned int,int);
void ChangePaneSize(tBlitSet *,int,int,int,int);
void RemovePanes(int);
void ZOID_ProjectWindow(int,float *,tUVMatrix *,float *,signed short *);
void ZeroCameras(void);
void *memcpy(void *,const void *,unsigned int);
int MBOX_FindObject_WORK(const char *,signed char,signed char,int);
signed short MBOX_NewObject(int,tUVMatrix *,int,unsigned int);
tTexture *MBOX_FindTexture_WORK(const char *,unsigned short *,signed char,signed char,int);
void ZOID_SetColor(signed short,tRGBA *);
void ZOID_SetTexture(signed short,tTexture *,int);
void ZOID_RemoveObject(signed short);
void MBOX_HideObject(int,int,int);
void MBOX_ShowObject(int,int,int);
void WPitchUV(float,float (*)[3]);
void WRollUV(float,float (*)[3]);
void ScaleUV(float (*)[3],float (*)[3],float);
#define fabs _builtin_fabsf
#endif
