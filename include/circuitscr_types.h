#ifndef DC_CIRCUITSCR_TYPES_H
#define DC_CIRCUITSCR_TYPES_H
#include "dc_types.h"
#include "text_types.h"
typedef struct tBlitSet tBlitSet;
/* Same target-backed screen record as optionscr; own proof C9F6/CA24,
 * matrix+c CA32/CA54, alpha+3c D4B2/D4B6, four strides40 CE4C..CE9A. */
typedef struct tMenuButton {
 int buttonType,objIdx;
 float angle;
 tUVMatrix mat;
 unsigned char alpha;
 char pad3d[3];
} tMenuButton;
/* Byte-aligned RGBA source constants at B2346 and B2363; CA00..1A byte copy. */
typedef struct dcCircuitColor { unsigned char c[4]; } dcCircuitColor;
typedef char check_circuit_button[(sizeof(tMenuButton)==0x40)?1:-1];
typedef char check_circuit_color[(sizeof(dcCircuitColor)==4)?1:-1];
#define gPlayers ((tPadState *)0x8C006000)
extern unsigned char gCircuitPlayer,gNumModels;
extern signed char gCircuitSize,gSoftReset;
extern signed short gNumPlayers;
extern unsigned int gSkipToGameState,gGameState,gActiveEdges,gActiveAutoEdges;
extern int gRace;
extern volatile float gFrameSeconds;
extern float unitmatrix[4][3];
extern tCamera gCamera[4];
extern tText gText;
extern dcCircuitColor flashColors[2];
extern tCircuitCar gCircuitCars[6];
extern tZoidObjectDC ObjInstanceList[]; /* B138 stride58; B142 base+10; B146 float store. */
extern unsigned char *gDefaultNames[];
extern unsigned char gPasswordAlphabet[32];
extern signed char gNumCircuitTracks[4];
extern unsigned char gUnlockedCircuits[][4],gUnlockedTracks[][7],gForceUnlockedCircuits[4];
/* Static storage addresses are individually proved by the target listing.
 * The unreferenced word CCBC0 and byte CCBF2 are intentionally unowned. */
extern int gCircuitMenuButtonsInited,gCircuitMenuButtonsJustInited;
extern float gCircuitMenuGButtonScale,gCircuitMenuGButtonSpace,gCircuitMenuMaxY,gCircuitMenuButtonZ,gCircuitMenuCursorZ,gCircuitMenuButtonX,minY;
extern _tBlit *gCircuitMenuBlit;
extern signed char gCircuitMenuInited,gEnterCircuitCode,gConfirmNewCircuit,gInvalidCode;
extern signed char gWrongNumPlayers,gContinueEnabled,gStatsEnabled,gShowTrophy;
extern int gCircuitScreenExit;
extern char *gTrophyNames[3],*gTrophySheenNames[3];
extern int trophyObjIdx,trophySheenObjIdx;
extern dcCircuitColor trophySheenColor;
extern tBlitSet *pane;
extern int gCircuitMenuSelect,newCircuitType;
extern tCircuit *gCircuit;
extern unsigned char gCircuitMenuGCircuitCode[32];
extern signed char gYes;
extern tUVMatrix gTrophyMat;
extern signed short gCodeSelect,gCodeLen,gReviewTrack,gProgressWidth,gProgressHeight,gProgressTop,gMenuBottom;
extern float gCircuitMenuCursorSpeed,gCircuitMenuScrollSpeed;
/* AF7A..84 dereferences file handle and loads unsigned VMU slot at+10. */
struct tPS_File { char pad0[16]; unsigned char pak; };
int PS_PakChanged(int);
void setGameState(unsigned int); void createBG(void); void removeBG(void);
void NewCircuit(int); void ContinueCircuit(void); int IsCircuitUnlocked(int); int SetCircuitCode(unsigned char *);
void SavePlayerRaceType(tPlayerProfile **,int);
void AudPlayEffect(int,int,int,int);
void PlayUDSound(unsigned int); void PlayLRSound(unsigned int); void PlayExitSound(unsigned int);
void *memcpy(void *,const void *,unsigned int);
void CopyUV(float (*)[3],float (*)[3]);
void ScaleUV(float (*)[3],float (*)[3],float);
void WPitchUV(float,float (*)[3]); void WRollUV(float,float (*)[3]); void YawUV(float,float (*)[3]);
void ZeroCameras(void);
int MBOX_FindObject_WORK(const char *,signed char,signed char,int);
signed short MBOX_NewObject(int,tUVMatrix *,int,unsigned int);
tTexture *MBOX_FindTexture_WORK(const char *,unsigned short *,signed char,signed char,int);
void MBOX_ShowObject(int,int,int); void MBOX_HideObject(int,int,int);
void ZOID_SetTexture(signed short,tTexture *,int);
void ZOID_SetColor(signed short,dcCircuitColor *); void ZOID_SetEnvColor(signed short,dcCircuitColor *);
void ZOID_RemoveObject(signed short);
void ZOID_ProjectWindow(int,float *,tUVMatrix *,float *,signed short *);
_tBlit *NewMultiBlit(signed short,signed short,const tBlitDef *,signed short);
void RemoveBlit(_tBlit *);
tBlitSet *InitPane(int,int,int,int,int,int (*)(_tBlit *),unsigned int,int);
void RemovePane(tBlitSet *); void ChangePaneSize(tBlitSet *,int,int,int,int);
void HAL_SetTextDepth(float); void HAL_SetTextTranslucency(float);
int HAL_SetFont(int); unsigned char HAL_GetFontAscent(void);
unsigned int HAL_GetTextWidth(void *,signed short);
signed short TB_GetFontLineHeight(void); signed short TB_CenterText(void *,signed short);
void TB_SetJust(int,int); void TB_SetTextColor(int);
void TB_FlashTextColors(dcCircuitColor,dcCircuitColor);
void TB_DrawText(signed short,signed short,void *);
void TB_DrawEllipseText(signed short,signed short,signed short,void *);
void TB_DrawTextScrollBox(signed short,signed short,signed short,signed short,signed short,signed short,void *);
void TB_Sprintf(void *,void *,...); void TB_Strcpy(void *,void *); void TB_Strcat(void *,void *);
float _builtin_fabsf(float);
#define fabs _builtin_fabsf
#endif
