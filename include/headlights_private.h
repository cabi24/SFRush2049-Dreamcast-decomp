#include "dc_types.h"
/* Private C89 overlays: target instruction evidence in evidence/target_listing.txt. */
#ifndef HEADLIGHTS_PRIVATE_H
#define HEADLIGHTS_PRIVATE_H
typedef tMatrix3 tUV3;



               /* target signed slot multiply3CC,8C02FDD6/FDE0. */

              /* slot*(256+1)*8,8C030588..596. */
typedef struct CarParts {
    char pad0[0x14];
    int headlight[4];       /* +14..20,8C02FE3A/58/94/B2. */
    int extraparts[3];      /* +24 at8C0306E8, preserved original bug. */
    char pad1[0x60-0x30];
} CarParts;                /* slot*3*32,8C02FDF2..FDFE. */
typedef struct tHeadlightInstance {
    unsigned int flags;    /* target8C03065E..662. */
    char pad0[0x10-4];
    float w;              /* target8C02FE6A/70. */
    char pad1[0x58-0x14];
} tHeadlightInstance;      /* target8C02FE60/66 stride58. */
typedef struct surftyp { unsigned short flags; } surftyp; /* only+0 word known. */
extern tCarData gGameCar[];
extern tModelData model[];
extern CarParts gCarParts[];
extern tHeadlightInstance ObjInstanceList[];
extern unsigned short gObjList[]; /* 8C02FE28/30 wordzeroextend. */
extern signed char gTrackNum, gInvisibleCar;
tUVMatrix *ZOID_NewMatrix(void);
tUVMatrix *ZOID_FindMatrix(signed short);
signed short MBOX_NewObject(int,tUVMatrix *,int,unsigned int);
void MBOX_HideObject(int,int,int);
void MBOX_ShowObject(int,int,int);
void ZOID_SetColor(signed short,tRGBA *);
surftyp *ShotToWorldCollision(float *,float *,float (*)[3],float,int,int);
void CopyUV(float (*)[3],float (*)[3]);
float NormalVector(float *);
void WorldVector(float *,float *,float (*)[3]);
#define fabs _builtin_fabsf
#define sqrtf _builtin_sqrtf
typedef char checkVisual[(sizeof(tVisual)==0x18)?1:-1];
typedef char checkCarData[(sizeof(tCarData)==0x3CC)?1:-1];
typedef char checkModelData[(sizeof(tModelData)==0x808)?1:-1];
typedef char checkCarParts[(sizeof(CarParts)==0x60)?1:-1];
typedef char checkInstance[(sizeof(tHeadlightInstance)==0x58)?1:-1];
#endif
