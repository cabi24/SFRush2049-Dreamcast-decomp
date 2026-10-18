#include "dc_types.h"
/* C89 overlays; named fields backed by road target literals/instructions. */
#ifndef ROAD_PRIVATE_H
#define ROAD_PRIVATE_H
typedef tCar tRoadCar;
typedef tRekonState tRoadReckon;
typedef tCarData tRoadGameCar;
 /* Ends at last accessed field; no original full-size claim. */


 /* Ends7C8; no new original allocation-size claim. */
 /* stride3CC targetpoolACC8 andAB7E/84. */
extern tRoadGameCar gGameCar[];
extern signed char gSuperSpeed; /* targetAB68 byte load. */
float invmagnitude(float *);
void CopyUV(float (*)[3],float (*)[3]);
void WorldVector(float *,float *,float (*)[3]);
void CollideCarBody(tModelData *);
void TireSurf(tModelData *,float *,float *,int *,float (*)[3],int);
void collision(tModelData *);
typedef char checkRoadInit[(sizeof(tRoadInit)==0x40)?1:-1];
typedef char checkRoadReckon[(sizeof(tRoadReckon)==0x90)?1:-1];
typedef char checkRoadModel[(sizeof(tModelData)==0x808)?1:-1];
typedef char checkRoadGameCar[(sizeof(tRoadGameCar)==0x3CC)?1:-1];
#endif
