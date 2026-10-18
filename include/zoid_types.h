#ifndef ZOID_TYPES_H
#define ZOID_TYPES_H
#include "dc_types.h"
typedef union zoidColor {tRGBA bytes;unsigned int rgba;} zoidColor;
typedef struct tZoidUnitObject {
 unsigned int flags; /* +0:689FC store;68C7C/window-mask. */
 unsigned int culling; /* +4:68A00 store. */
 tUVMatrix *mat; /* +8:68A00;68722..72C base+8. */
 float scale,w; /* +c,+10:68A02/68A08. */
 float look[3]; /* +14,+18,+1c:68A10..20. */
 float ouls,oult; /* +20,+24:68A24/28. */
 unsigned short index; /* +28:687D4..DC extu.w;68A2C store. */
 signed short child,next,numLights; /* +2a/+2c/+2e:68A32/36/3A. */
 void *lights; /* +30:68A3E. */
 tTexture *texture[4]; /* +34,+38,+3c,+40:68A40..46. */
 void *tile,*tileSize,*palette; /* +44/+48/+4c:68A4A/4E/52; pointed types unproven. */
 zoidColor color; /* +50:68A56 word clear. */
 tRGBA envcolor; /* +54: canonical renderer target692D6..DE, untouched here. */
} tZoidUnitObject;
typedef struct zoidRomDef {char name[16];char pad10[72-16];} zoidRomDef;
/* Name passed directly to strcmp at687FC; stride72=(idx*8+idx)*8,687E4..7FA. */
typedef struct zoidRomBank {zoidRomDef *romObjDefs;char pad4[4];} zoidRomBank;
/* Bank stride8 and pointer+0 at687E6..7F4; second word unproven here. */
extern tZoidUnitObject ObjInstanceList[];
extern signed short ObjHead;
extern tUVMatrix gMatrices[];
extern zoidRomBank gRomObjDefPtrs[];
extern float unitmatrix[4][3],gGlobalCameraFacingScale;
extern int gNumObjects,gMaxObjects,gNumMatrices,gMaxMatrices,gNumBlitInsts,gNumPolyInsts;
void CopyUV(float src[3][3],float dst[3][3]);
int strcmp(const char *,const char *);
/* Canonical mbox verified same target09BB5C reduced-clobber intrinsic. */
#define strcmp _builtin_strcmp
void ZOID_ResetWindows(void);
void zoidlight_func_8C068F0E(void);
void ZOID_Reset(void);
void ZOID_InsertObject(signed short,signed short);
signed short ZOID_LastSibling(signed short);
signed short ZOID_ObjParent(signed short);
signed short ZOID_ObjPrevObj(signed short);
void _RemoveObjectTree(signed short,int,int);
void ZOID_RemoveObjectTree(signed short,int);
int func_8C068C68(tZoidUnitObject *,int);
typedef char checkZoidStride[(sizeof(tZoidUnitObject)==88)?1:-1];
typedef char checkRomStride[(sizeof(zoidRomDef)==72)?1:-1];
typedef char checkBankStride[(sizeof(zoidRomBank)==8)?1:-1];
#endif
