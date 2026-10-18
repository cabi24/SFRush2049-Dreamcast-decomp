#ifndef SELECTCARLOAD_PRIVATE_H
#define SELECTCARLOAD_PRIVATE_H
#include "dc_types.h"
typedef struct SelectWave {
 int wave; signed char loading,slot; char opaque6[6];
} SelectWave; /* +0/+4/+5, stride0xC: 8C08AEFC..AF14. */
typedef struct SelectCar {
 signed char created,hidden,ready,loading;
 int waveInst;
 signed char PaintJob; char opaque9;
 signed short pjModelNum;
 int rims;
 float frontTireScale,rearTireScale;
} SelectCar; /* stride0x18: 8C08AF34..AF52; pjModelNum +A: B522. */
typedef struct SelectCarParts {
 int frame; char opaque4[4]; int tsheen,brakes,trim;
 int headlight[4],extraparts[3];
 int hood,sheen; char opaque38[4]; int tires[4]; char opaque4C[20];
} SelectCarParts; /* 0x60: 8C08B4AE..4C4; named clears 8C08B55A..57E. */
typedef struct SelectObject {
 unsigned int flags; int culling; char opaque8[0x58-8];
} SelectObject; /* stride0x58, +0/+4: 8C08B89A..8C8. */
extern tCarLoadLink gLink[];
extern SelectWave gSelectCarWavePtrs[16];
extern SelectCar gSelectCar[52];
extern SelectCarParts gCarParts[52];
extern SelectObject ObjInstanceList[];
extern signed char gCarPaintJobInited[];
extern signed short gCarModelNums[52],gNumWindows;
extern int gCarPaintJobWaveNums[13];
extern tCar *gCarModels[13];
extern float gFrontTireScale[5][13],gRearTireScale[5][13];
extern signed char gTireRims[5][13];
extern tTexture *gWheelTextures[48];
extern volatile signed char gFrameScale;
extern float FSCLS[3],unitmatrix[3][3];
extern int LoadWave(int,int,int,int,void *);
extern int WaveInstanceReady(int);
extern void UnloadWaveInstance(int,int);
extern tUVMatrix *ZOID_FindMatrix(int);
extern void ScaleUV(float (*)[3],float (*)[3],float),CopyUV(float (*)[3],float (*)[3]);
extern void CreateCar(signed short,signed short,int);
extern void MBOX_ShowObject(int,int,int),MBOX_HideObject(int,int,int);
extern void func_8C068B9A(int,int);
extern void InitCarDynamicObjs(signed short,signed short,signed short);
extern void SetSheen(signed short,signed short,signed short);
extern void SetupCarColor(signed short,signed short,unsigned char);
extern void SetFrameColor(signed short,tRGBA *);
extern void ZOID_SetColor(int,tRGBA *);
extern void ZOID_UpdateObject(int,float *,float (*)[3]);
extern void ZOID_SetTexture(int,tTexture *,int);
void func_8C08AED0(void),func_8C08AF1C(void);
int LoadSelectCar(signed short,signed short,int *);
void UnloadSelectCar(signed short);
void SetSelectTireSizes(signed short,signed short);
void CreateSelectCar(signed short,signed short,signed short);
void UpdateSelectCar(signed short,signed short,unsigned char,signed short,float *,float (*)[3],unsigned char);
#endif
