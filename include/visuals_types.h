#ifndef VISUALS_TYPES_H
#define VISUALS_TYPES_H
#include "dc_types.h"
/* Target-only ROM record slice: index*72 at8C05E800..814,radius+16 storeE816. */
typedef struct dcVisualRomObj { char pad0[16]; float radius; char pad14[72-20]; } dcVisualRomObj;
typedef struct dcVisualRomBase { dcVisualRomObj *romObjDefs; char pad4[4]; } dcVisualRomBase;
/* Table stride8 and pointer+0:8C05E9A6/EA8C; texture stride48 fromE9A2..AE. */
typedef struct dcVisualTexBase { tTexture *texDefs; char pad4[4]; } dcVisualTexBase;
/* CarParts stride96 and frame/hood/tires at8C05EE7C..EEBA;
 * tsheen+8 atEF0E; sheen+34 atEEE2; extent52*96 atECDE. */

typedef char visuals_check_carparts[(sizeof(CarParts)==96)?1:-1];
extern dcVisualRomBase gRomObjDefPtrs[];
extern dcVisualTexBase gTexDefPtrs[];
extern tCarLoadLink gLink[];
extern const char *CarNames[];
extern volatile signed char gNumModels;
extern signed char gMineCones,gCarMines;
extern unsigned int gGameState;
extern tUVMatrix *gCarMatrices[];
extern unsigned short gObjList[701],gTexList[19];
extern tTexture *gWheelTextures[48];
extern CarParts gCarParts[52];
extern signed short gCarModelNums[52],gEnvCount;
extern signed char gCarPaintJobInited[52],gCarTeamColor[4];
extern tVisual gEnvirons[100],*gEnvList,*gFreeVis;
extern const char *const WeaponNames[8],*const WingNames[12],*const ObjNames[61],*const TrkNames[163];
extern const char *const WheelObjNames[1],*const TexNames[19],*const carObjNames[9];
extern char *gWheelNames[48],*sheenName[11];
extern const char MineName[],CarPartFormat[];
extern float unitmatrix[4][3];
int MBOX_FindObject_WORK(const char *,signed char,signed char,int);
tTexture *MBOX_FindTexture_WORK(const char *,unsigned short *,signed char,signed char,int);
void SetTargetDefs(void);
void InitVisualsCarScr(void);
void *memset(void *,int,unsigned int);
int sprintf(char *,const char *,...);
void MBOX_HideObject(int,int,int);
void MBOX_ShowObject(int,int,int);
void ZOID_SetTexture(signed short,tTexture *,int);
void ZOID_SetColor(signed short,tRGBA *);
void ZOID_SetEnvColor(signed short,tRGBA *);
tUVMatrix *ZOID_NewMatrix(void);
signed short MBOX_NewObjectCam(int,tUVMatrix *,signed short,int,unsigned int);
#endif
