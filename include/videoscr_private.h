#ifndef VIDEO_PRIVATE_H
#define VIDEO_PRIVATE_H
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
typedef struct tVideoPadInfo {
    volatile signed char attached; /* +0, stride14:8C041E9E..AE and reread8C041EB8 */
    char pad0[0x14-1];
} tVideoPadInfo;
typedef struct tVideoText {
    char pad0[4];
    unsigned char **single; /* +4:8C042304 */
    char pad1[0xc-8];
    unsigned short *tab; /* +c:8C04242E; +34 gives tab17 */
    unsigned char **taba; /* +10:8C042434; element indexed words */
} tVideoText;
typedef char checkOptionsButton[(sizeof(tMenuButton)==0x40)?1:-1];
typedef char checkOptionsPad[(sizeof(tVideoPadInfo)==0x14)?1:-1];
typedef char checkOptionsBlitDef[(sizeof(tBlitDef)==0x24)?1:-1];
extern volatile unsigned int gGameState;
extern unsigned int gActiveEdges,gActiveAutoEdges,gContEdges[4],gAutoEdges[4];
extern int playerInPause;
extern signed char gSoftReset,gOptions[40];
extern signed short gLang;
extern tVideoPadInfo gControllerData[4];
extern tVideoText gText;
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

typedef struct VideoMenuRect { int x,w,h,cx; } VideoMenuRect;
extern unsigned int gActiveLevels,gContLevels[4];
extern signed char osTvType;
extern int osCblType;
extern signed char videoInited,videoPanesCreated,videoControllerRemoved;
extern signed char videoYesFrequency,videoConfirmFrequency,videoTestFrequency;
extern int videoHorzCenter,videoVertCenter,videoMenuSelect;
extern int videoExit,videoButtonsJustInited,videoButtonsInited;
extern float videoButtonScale,videoCursorSpeed;
extern signed char videoOptionValid[4];
extern VideoMenuRect videoMenuRect;
extern _tBlit *videoBlit;
extern tBlitSet *videoTitlePane,*videoConfirmPane,*videoTestPane;
extern short videoTestWidth,videoTestHeight,videoConfirmWidth,videoConfirmHeight;
#define inited videoInited
#define panesCreated videoPanesCreated
#define gControllerRemoved videoControllerRemoved
#define gYesFrequency videoYesFrequency
#define gConfirmFrequency videoConfirmFrequency
#define gTestFrequency videoTestFrequency
#define gHorzCenter videoHorzCenter
#define gVertCenter videoVertCenter
#define gVideoMenuSelect videoMenuSelect
#define exitVideoScreen videoExit
#define buttonsJustInited videoButtonsJustInited
#define buttonsInited videoButtonsInited
#define gButtonScale videoButtonScale
#define cursorSpeed videoCursorSpeed
#define optionValid videoOptionValid
#define menuRect videoMenuRect
#define blit videoBlit
#define gTitlePane videoTitlePane
#define gConfirmPane videoConfirmPane
#define gTestPane videoTestPane
#define kMenuBackward 2
#define kMenuStay 0
extern int CountdownTimeOut(void);
extern void SetCountdownTimer(float),Pal60(int),VideoSetOffset(int,int),SaveFlush(int);
extern signed short TB_RightText(void *,signed short),TB_GetTextNumLines(short,void *);
extern void TB_DrawTextScrollBox(short,short,short,short,int,int,void *);
extern void RemovePane(tBlitSet *);
extern void func_8C06F1EC(int,int,int,int,int,int,int,int,float);
void updateVideoScreen(void),initScreen(void),_TitlePane(void),uninitScreen(void);
int animPanes(void),animText(void);
void animButtonText(void),drawColorBars(void);
signed char validOption(int);
void checkVideoChange(void),calcMenuRect(void),initButtons(void),uninitButtons(void),updateButtons(void);
#endif
