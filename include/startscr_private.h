#ifndef STARTSCR_PRIVATE_H
#define STARTSCR_PRIVATE_H
#include "dc_types.h"
typedef _tBlit _tBlit_2;
typedef struct tStartButton {
    int buttonType; /* +0:8C08C60A */
    int objIdx; /* +4:8C08C5FA/C678 */
    float angle; /* +8:8C08C9C8/CA70 */
    tUVMatrix mat; /* +C:8C08C608; pos+30/34/38:CA54/58/60 */
} tStartButton; /* stride3C:C67E/C684,18-record bound438:C5DA */
typedef struct tStartText {
    char pad0[4];
    unsigned char **single;
    char pad1[4];
    unsigned short *tab;
    unsigned char **taba;
} tStartText; /* single4/tabC/taba10; target18C15C98C..9C */
typedef char check_start_button[(sizeof(tStartButton)==0x3c)?1:-1];
typedef char check_start_blit[(sizeof(tBlitDef)==0x24)?1:-1];
extern unsigned int gGameState,gSkipToGameState;
extern unsigned int gActiveEdges,gActiveLevels;
extern signed char gSoftReset,gGameStarted,gOptions[40],gCheatActive[64];
extern signed short gNumPlayers;
extern tPadInfo gControllerData[4]; /*20 stride:B9BA/B9CC */
extern tStartText gText;
extern tCamera gCamera[4];
extern tRGBA flashColors[2];
extern volatile unsigned char gNumModels;
extern volatile float gFrameSeconds;
extern float unitmatrix[3][3];
extern int startMenuSelect,startMaxPlayers;
extern int startExit;
extern float startButtonScale,startButtonSpace,startButtonX,startMaxY,startButtonZ,startCursorZ;
extern float startCursorSpeed,startPlayerCursorSpeed;
#define gStartMenuSelect startMenuSelect
#define gMaxPlayers startMaxPlayers
#define exitStartScreen startExit
#define gButtonScale startButtonScale
#define gButtonSpace startButtonSpace
#define buttonX startButtonX
#define maxY startMaxY
#define buttonZ startButtonZ
#define cursorZ startCursorZ
#define cursorSpeed startCursorSpeed
#define playerCursorSpeed startPlayerCursorSpeed
#define mblit startBlitDef
#define inited startInited
#define blit startBlit
#define validOption startValidOption
#define buttonNames startButtonNames
#define buttonInfo startButtonInfo
#define buttonsInited startButtonsInited
#define buttonsJustInited startButtonsJustInited
#define fabs _builtin_fabsf
float _builtin_fabsf(float);
void AudSetMusicVol(float,float);
void AudPlaySong(int,int);
void AudKillSong(void);
void AudStopAllEffects(void);
void setGameState(unsigned int);
void PlayStartABSound(unsigned int);
void PlayUDSound(unsigned int);
void PlayLRSound(unsigned int);
void createBG(void),removeBG(void),ClearRushControls(void);
_tBlit *NewMultiBlit(signed short,signed short,const tBlitDef *,signed short);
void RemoveBlit(_tBlit *);
void func_8C06CA98(void),func_8C06CAB4(void),func_8C039718(void);
signed char func_8C06CAA8(void);
int func_8C0727C8(void);
void func_8C07D910(int);
int func_8C07D92C(void);
#define InitEgg func_8C07D910
#define CheckForEgg func_8C07D92C
#define ExitSystem func_8C039718
int HAL_SetFont(int);
void HAL_SetTextDepth(float),HAL_SetTextTranslucency(float);
void HAL_SetTextColor(unsigned char,unsigned char,unsigned char,unsigned char);
signed short TB_GetFontLineHeight(void),TB_CenterText(void *,signed short);
void TB_Sprintf(void *,void *,...);
void TB_DrawText(signed short,signed short,void *);
void TB_SetTextColor(int),TB_SetJust(int,int);
void TB_FlashTextColors(tRGBA,tRGBA); /* targetTB ABI: two values on stack */
void ZOID_ProjectWindow(int,float *,tUVMatrix *,float *,signed short *);
void *memcpy(void *,const void *,unsigned int);
int MBOX_FindObject_WORK(const char *,signed char,signed char,int);
signed short MBOX_NewObject(int,tUVMatrix *,int,unsigned int);
tTexture *MBOX_FindTexture_WORK(const char *,unsigned short *,signed char,signed char,int);
void ZOID_SetColor(signed short,tRGBA *),ZOID_SetTexture(signed short,tTexture *,int),ZOID_RemoveObject(signed short);
void MBOX_HideObject(int,int,int),MBOX_ShowObject(int,int,int);
void WPitchUV(float,float (*)[3]),WRollUV(float,float (*)[3]);
void ScaleUV(float (*)[3],float (*)[3],float);
void ScaleUVXYZ(float (*)[3],float (*)[3],float,float,float);
void ZeroCameras(void);
void updateStartScreen(void),initScreen(void),uninitScreen(void);
int animText(void);
void animButtonText(void),initButtons(void),uninitButtons(void),updateButtons(void);
signed char optionValid(int);
#endif
