#ifndef DC_CONTROLSCR_TYPES_H
#define DC_CONTROLSCR_TYPES_H
#include "dc_types.h"
typedef _tBlit _tBlit_2;
typedef struct tBlitSet tBlitSet;
typedef struct tMenuButton {
    int buttonType,objIdx;
    float angle;
    tUVMatrix mat;
    unsigned char alpha;
    char pad0[3];
} tMenuButton; /* +30/34/38 pos, +3c alpha; stride40 in8C02169A..94 */
typedef struct anon_x_y_18 {int x,y,w,h,cx,cy;} anon_x_y_18;
typedef struct tControlText {
    char pad0[4]; unsigned char **single;
    char pad1[4]; unsigned short *tab; unsigned char **taba;
} tControlText;
struct tPS_File {char pad0[0x10]; unsigned char pak;}; /* 8C02057C/586 */
typedef char checkControlButton[(sizeof(tMenuButton)==64)?1:-1];
#define gPlayers ((tPadState *)0x8C006000)
extern tPlayerProfile *gJustPlayers[4];
extern signed short gNumWindows;
extern volatile unsigned char gNumModels;
extern unsigned int gGameState;
extern signed char gSoftReset,gNewControls[4][11],gControlsSet[4];
extern signed char controlScrActive[4],prevControlScrActive[4],gConflict[4][11],gConflictExists[4];
extern signed short gControlSelect[4],gControlFirst[4],gControlMenuSize;
extern int buttonsInited[4],buttonsJustInited[4];
extern float cursorSpeed[4],scrollSpeed[4];
extern tCamera gCamera[4];
extern tPadInfo gControllerData[4];
extern tControlText gText;
extern int playerTextColor[4];
extern tRGBA flashColors[2];
extern volatile float gFrameSeconds;
extern float unitmatrix[3][3];
int validOptionCall(signed char,int); /* actual target calls8C02018C/22C/2F8 ->214E4 */
int PS_PakRumblePort(int);
#define PS_PakRumble(p) (PS_PakRumblePort((p)*2) || PS_PakRumblePort((p)*2+1))
int PS_PakInserted(int);
void SetDefaultControls(signed char *);
void SetControls(int);
void PlayStartABSound(unsigned int);
int AudPlayEffect(int,int,int,int);
void *memcpy(void *,const void *,unsigned int);
int memcmp(const void *,const void *,unsigned int);
_tBlit *NewMultiBlit(signed short,signed short,const tBlitDef *,signed short);
void RemoveBlit(_tBlit *);
int Hidden(_tBlit *,int);
void RenameBlit(_tBlit *,const char *,int);
void UpdateBlit(_tBlit *);
void SelectBlit(_tBlit *,signed short,signed short,signed short);
int HAL_SetFont(int);
void HAL_SetTextDepth(float);
void HAL_SetMenuTextDepth(float);
void HAL_SetTextTranslucency(float);
signed short TB_GetFontLineHeight(void);
signed short TB_CenterText(void *,signed short);
signed short TB_RightText(void *,signed short);
void TB_Sprintf(void *,void *,...);
void TB_DrawText(signed short,signed short,void *);
void TB_SetTextColor(int);
void TB_SetJust(int,int);
void TB_EllipseText(void *,void *,signed short);
void TB_DrawEllipseText(signed short,signed short,signed short,void *);
void TB_DrawTextScrollBox(signed short,signed short,signed short,signed short,signed short,signed short,void *);
void TB_FlashTextColors(tRGBA,tRGBA);
tBlitSet *InitPane(int,int,int,int,int,int (*)(_tBlit *),unsigned int,int);
void RemovePanes(int);
void setPauseState(int);
void ZOID_ProjectWindow(int,float *,tUVMatrix *,float *,signed short *);
void ZeroCameras(void);
int MBOX_FindObject_WORK(const char *,signed char,signed char,int);
signed short MBOX_NewObject(int,tUVMatrix *,int,unsigned int);
tTexture *MBOX_FindTexture_WORK(const char *,unsigned short *,signed char,signed char,int);
void ZOID_SetColor(signed short,tRGBA *);
void ZOID_SetTexture(signed short,tTexture *,int);
void ZOID_RemoveObject(signed short);
void MBOX_HideObject(int,int,int);
void MBOX_ShowObject(int,int,int);
void CopyUV(float (*)[3],float (*)[3]);
void WPitchUV(float,float (*)[3]);
void WRollUV(float,float (*)[3]);
void ScaleUV(float (*)[3],float (*)[3],float);
#define fabs _builtin_fabsf
#endif
