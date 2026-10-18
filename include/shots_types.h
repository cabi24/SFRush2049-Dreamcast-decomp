#ifndef DC_SHOTS_TYPES_H
#define DC_SHOTS_TYPES_H
#include "dc_types.h"
typedef struct tShotCaseDC {
 struct tShotCaseDC *next; /* 962EC; singleStatLLprefix0. */
 unsigned int mbIdx; /* +4:96302. */
 tUVMatrix mat; /* +8, position+2C/+30/+34:96318/32A/33A. */
 float time; /* +38:962EA/F0/FA. */
 float vel[3]; /* +3C/+40/+44:96314/24/34/44. */
} tShotCaseDC; /* stride48 andcount30:97152..15E. */
typedef struct tShotInfoDC tShotInfoDC;
typedef struct tShotDC {
 struct tShotDC *next;
 unsigned int mbIdx; /* +4:97344; width32 fromsignextendedMBOXobjectreturn. */
 tUVMatrix mat; /* +8:97334/33A; position+2C/+30/+34. */
 char unknown38[4]; /* objectstride3C proven97150; unusedfieldsemantics unclaimed. */
} tShotDC;
struct tShotInfoDC {
 tShotInfoDC *next;
 signed char slot,slotDamaged,type,flags; /* +4..7:97454/62/6A/70. */
 char unknown08[4];
 float incTime; /* +C:97474. */
 char unknown10[4];
 float vel[3],pos[3],old[3]; /* +14/+20/+2C:97476..4BA. */
 float uvs[3][3]; /* +38:974A8 andCopyUV. */
 char unknown5c[4]; /* gap betweenknownUVending5C andobj+60; semantics unclaimed. */
 tShotDC *obj,*obj2; /* +60/+64:9745A/5E andShotDone972D8/EA. */
}; /* stride68/count24:97128..134; fullfieldsemantics onlywherecited. */
typedef struct tShotHitPolyDC {char opaque[8];} tShotHitPolyDC; /* stride8:97142; fields unused/unclaimed here. */
typedef struct tShotShieldDC {int dlIdx;char unknown04[0x34-4];} tShotShieldDC; /* stride34/fourrecords97121A..230, onlydlIdxusedhere. */
typedef struct tShotsDamageView {char unknown00[0x3a0];signed short damageThisFrame;} tShotsDamageView; /* 963E0/E2; minimumviewonly, nofullsizeclaim. */
#define SHOT_DAMAGE(c) (((tShotsDamageView *)(c))->damageThisFrame)
typedef char checkShotsCase[(sizeof(tShotCaseDC)==0x48)?1:-1];
typedef char checkShotsObject[(sizeof(tShotDC)==0x3c)?1:-1];
typedef char checkShotsInfo[(sizeof(tShotInfoDC)==0x68)?1:-1];
extern volatile float gFrameSeconds;
extern float gGravity;
extern tModelData model[];
extern tCarData gGameCar[];
extern signed short gNumCars;
extern signed char gCarTeam[]; /* 963B4team8C15D020. */
extern tStatList gShotCaseList,gShotList,gHitPolyList,gShotObjList;
extern tShotCaseDC gShotCase[30];
extern tShotInfoDC gShots[24];
extern tShotHitPolyDC gHitPolys[24];
extern tShotDC gShotObjs[24];
extern int gWprObjList[10],gWfxObjList[7];
extern unsigned short wallTidx,gMineTextures[5],gRipTextures[5];
extern tShotShieldDC gCarShield[4];
extern unsigned char gNumModels;
extern float unitmatrix[3][3];
void ZOID_RemoveObject(signed short);
void FreeStatLLE(tStatList *,void *);
void *GetStatLLE(tStatList *);
void InitStatLL(tStatList *,void *,int,int,signed char);
void AddBattlePoint(int,int,int);
void BodyVector(float *,float *,float (*)[3]);
float fatan2(float,float);
extern float _builtin_fabsf(float);
#define fabs _builtin_fabsf
int MBOX_FindObject_WORK(const char *,int,int,int);
tTexture *MBOX_FindTexture_WORK(const char *,unsigned short *,int,int,int);
signed short MBOX_NewObject(int,tUVMatrix *,int,unsigned int);
void InitStreaks(void);
void CopyUV(float (*)[3],float (*)[3]);
void PitchUV(float,float (*)[3]);
void BattleDamage(tCarData *,tCarData *,int);
#endif
