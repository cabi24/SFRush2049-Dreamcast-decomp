#ifndef DC_WORLD_VISIBILITY_PRIVATE_H
#define DC_WORLD_VISIBILITY_PRIVATE_H
#include "dc_types.h"
typedef struct tModelPart {
 char name[16]; tUVMatrix mat; unsigned int dlflags;
 signed short next,child; char unknown48[8]; float min[3],max[3];
} tModelPart; /* sizeof68; matrix10;translation34;next44;min50/max5C */
typedef struct anon_x_z_10 {float x,z,dist,max;} anon_x_z_10;
typedef struct tWorldVisibilityObject {
 unsigned int flags; char unknown04[4]; tUVMatrix *mat; char unknown0c[0x58-0x0c];
} tWorldVisibilityObject; /* qualified fields0/8;stride58 at661AA/1BC */
typedef char check_worldpart[(sizeof(tModelPart)==0x68)?1:-1];
typedef char check_worldobject[(sizeof(tWorldVisibilityObject)==0x58)?1:-1];
typedef char check_window[(sizeof(tWindowInfo)==0x48)?1:-1];
extern tModelPart *WorldBase;
extern volatile signed short Worlddlidx;
extern tWorldVisibilityObject ObjInstanceList[];
extern tWindowInfo ZoidWinInfo[];
extern signed char gTrackNum,gFogIntensity,gInvisibleTrack;
extern signed short gNumPlayers;
extern int gPauseState;
extern float gViewableDistance;
#define sqrtf _builtin_sqrtf
float _builtin_sqrtf(float),Distance2D(float,float);
void WYawUV(float,tUVMatrix *);
void func_8C065F2C(tModelPart *,float *,int *,float *,int *,int *);
void func_8C06602E(int,float *,int),func_8C066278(int,float *);
#endif
