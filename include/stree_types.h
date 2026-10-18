#include "dc_types.h"
#ifndef STREE_TYPES_H
#define STREE_TYPES_H
typedef tCarData tStreeGameCar;
/* Spatial layouts: InitSTree145C..14E2 proves header16/path132/tree20/surf24/vertex8/save32.
   streetyp fields: downleaf162E..168C; surftyp fields: PointInSurf18D2 onward.
   sverttyp packed position/overflow: PointInSurf and MoveCollisionObject378E onward.
   savetyp fields: ResetCollisionObject3C02..3C30, stride32 at3C3C.
   pathtyp scalar fields: fPathInterp22D4..2652 and fHalfPipeInterp2652..286C.
   collision record offsets+0/+4/+28/+2A/+2C: CarApply3CC2..3CD2 and AgainstSurface tail. */
typedef struct streetyp {
    signed short parent; /* 0x0 */
    unsigned char unused; /* 0x2 */
    unsigned char flags; /* 0x3 */
    signed short minh; /* 0x4 */
    signed short maxh; /* 0x6 */
    signed short minv; /* 0x8 */
    signed short maxv; /* 0xa */
    unsigned short child[4]; /* 0xc */
} streetyp;



typedef struct stheadertyp {
    unsigned short pnum; /* 0x0 */
    unsigned short tnum; /* 0x2 */
    unsigned short snum; /* 0x4 */
    unsigned short vnum; /* 0x6 */
    unsigned short savenum; /* 0x8 */
    unsigned short vlnum; /* 0xa */
    unsigned int slnum; /* 0xc */
} stheadertyp;

typedef struct pathtyp {
    float pos[3]; /* 0x0 */
    float uvs[3][3]; /* 0xc */
    float ve[3]; /* 0x30 */
    float vl[3]; /* 0x3c */
    float vr[3]; /* 0x48 */
    float width; /* 0x54 */
    float w; /* 0x58 */
    float m0; /* 0x5c */
    float m1; /* 0x60 */
    float wl; /* 0x64 */
    float ml; /* 0x68 */
    float wr; /* 0x6c */
    float mr; /* 0x70 */
    float m0l; /* 0x74 */
    float m0r; /* 0x78 */
    float m1l; /* 0x7c */
    float m1r; /* 0x80 */
} pathtyp;

typedef struct sverttyp {
    signed short pos[3]; /* 0x0 */
    unsigned short overflow; /* 0x6 */
} sverttyp;

typedef struct savetyp {
    unsigned short object; /* 0x0 */
    unsigned short snum; /* 0x2 */
    signed short uvs[3][3]; /* 0x4 */
    unsigned short vnum; /* 0x16 */
    signed short pos[3]; /* 0x18 */
    unsigned short overflow; /* 0x1e */
} savetyp;

typedef struct anon_surf_uvs_30 {
    surftyp *surf; /* 0x0 */
    float uvs[3][3]; /* 0x4 */
    signed short carEdge; /* 0x28 */
    signed short index; /* 0x2a */
    float dist; /* 0x2c */
} anon_surf_uvs_30;
/* Private model overlay. All omitted regions remain explicit gaps.
   Fields inherited in canonical layouts keep their proven offsets. */




typedef struct tStreeLink { char pad0[8]; unsigned char owner; char pad1; } tStreeLink; /* target54674..680 stride10,owner8 */
extern tStreeGameCar gGameCar[];
extern tStreeLink gLink[];
extern float unitmatrix[4][3]; /* target51888=base+36,52D38=base */
extern float _builtin_sqrtf(float);
extern float _builtin_fabsf(float);
#define sqrtf _builtin_sqrtf
#define fabs _builtin_fabsf
void BodyVector(float *,float *,float (*)[3]);
void WorldVector(float *,float *,float (*)[3]);
void CopyUV(float (*)[3],float (*)[3]);
void MulUVS(float (*)[3],float (*)[3],float (*)[3]);
float NormalVector(float *);
void froll(float,float,float (*)[3]);
void fpitch(float,float,float (*)[3]);
void AbortCar(signed short,int);
void AddBattlePoint(signed short,signed short,int);
int PlayGameSound(int,int,int,unsigned char);
void *memcpy(void *,const void *,unsigned int);
typedef char checkStreet[(sizeof(streetyp)==20)?1:-1];
typedef char checkSurf[(sizeof(surftyp)==24)?1:-1];
typedef char checkRoadHeader[(sizeof(stheadertyp)==16)?1:-1];
typedef char checkPath[(sizeof(pathtyp)==132)?1:-1];
typedef char checkVert[(sizeof(sverttyp)==8)?1:-1];
typedef char checkSave[(sizeof(savetyp)==32)?1:-1];
typedef char checkCollision[(sizeof(anon_surf_uvs_30)==48)?1:-1];
typedef char checkGameCar[(sizeof(tStreeGameCar)==0x3CC)?1:-1];
typedef char checkLink[(sizeof(tStreeLink)==10)?1:-1];
#endif
