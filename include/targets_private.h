#include "dc_types.h"


/* Targets-specific overlays: every accessed offset is checked against target listing. */
typedef struct tTarget tTarget;
typedef struct tBasicTargetPath tBasicTargetPath;
typedef struct tTargetStructPath tTargetStructPath;
typedef struct tBBoxColl {
    float left; /* 0x0 */
    float right; /* 0x4 */
    float front; /* 0x8 */
    float back; /* 0xc */
    float top; /* 0x10 */
    float bot; /* 0x14 */
    float rad; /* 0x18 */
    float weight; /* 0x1c */
} tBBoxColl;
typedef struct tMiniModelData {
    tBBoxColl *bboxInfo; /* 0x0 */
    float uvs[3][3]; /* 0x4 */
    float pos[3]; /* 0x28 */
    float vel[3]; /* 0x34 */
} tMiniModelData;
struct tTarget {
    tStatLLE ll; /* 0x0 */
    unsigned char Flags; /* 0x4 */
    int mbIdx; /* 0x8 */
    int dlIdx; /* 0xc */
    signed short infoIdx; /* 0x10 */
    tUVMatrix mat; /* 0x14 */
    float pos[3]; /* 0x44 */
    signed short type; /* 0x50 */
    float radius; /* 0x54 */
    signed short otype; /* 0x58 */
    signed short frames; /* 0x5a */
    signed char slot; /* 0x5c */
    int sound; /* 0x60 */
    signed char sndState; /* 0x64 */
    signed char owner; /* 0x65 */
    float timeStamp; /* 0x68 */
    void * data; /* 0x6c */
};
typedef struct anon_pos_norm_44 {
    float pos[3]; /* 0x0 */
    float norm[3]; /* 0xc */
    float scale[3]; /* 0x18 */
    float quat[4]; /* 0x24 */
    float distToNext; /* 0x34 */
    float timeToNext; /* 0x38 */
    float speed; /* 0x3c */
    unsigned int flags; /* 0x40 */
} anon_pos_norm_44;
struct tTargetStructPath {
    tBasicTargetPath *pathData; /* 0x0 */
    float timeElapsed; /* 0x4 */
    float curDistToNext; /* 0x8 */
    signed short ptOn; /* 0xc */
    unsigned short flags; /* 0xe */
    float curVel; /* 0x10 */
    float maxVel; /* 0x14 */
    float lastPos[3]; /* 0x18 */
    float lastUVS[3][3]; /* 0x24 */
};
struct tBasicTargetPath {
    char name[16]; /* 0x0 */
    unsigned int flags; /* 0x10 */
    signed short numPts; /* 0x14 */
    signed short collIdx; /* 0x16 */
    void * trigPtr; /* 0x18 */
    anon_pos_norm_44 *pts; /* 0x1c */
    tTargetStructPath *parent; /* 0x20 */
};
typedef struct tTargetInfo {
    char *name; /* 0x0 */
    char *mboxname; /* 0x4 */
    void (*startFunc)(tTarget *); /* 0x8 */
    void (*animFunc)(tVisual *, signed short); /* 0xc */
    signed short collFuncIdx; /* 0x10 */
    unsigned short Flags; /* 0x12 */
    signed short objListIdx; /* 0x14 */
    signed char structureID; /* 0x16 */
    signed char structureIdx; /* 0x17 */
    float collVal; /* 0x18 */
    int startSnd; /* 0x1c */
    int moveSnd; /* 0x20 */
    int stopSnd; /* 0x24 */
    int replace; /* 0x28 */
    float fadeDist; /* 0x2c */
    float oRad; /* 0x30 */
} tTargetInfo;
typedef struct tTargetStructRot {
    float angV[3]; /* 0x0 */
} tTargetStructRot;
typedef struct tTargetStructVel {
    float vel[3]; /* 0x0 */
} tTargetStructVel;
typedef struct tTargetStructVelRot {
    float angV[3]; /* 0x0 */
    float vel[3]; /* 0xc */
} tTargetStructVelRot;
typedef struct tPlayerMines {
    tTarget *m[4]; /* 0x0 */
    int num; /* 0x10 */
} tPlayerMines;
typedef struct tAlwaysCol {
    tStatLLE ll; /* 0x0 */
    tTarget *t; /* 0x4 */
} tAlwaysCol;
typedef struct tTargetGroup {
    signed short parent; /* 0x0 */
    unsigned char unused; /* 0x2 */
    unsigned char flags; /* 0x3 */
    float minh; /* 0x4 */
    float maxh; /* 0x8 */
    float minv; /* 0xc */
    float maxv; /* 0x10 */
    unsigned short child[4]; /* 0x14 */
} tTargetGroup;
typedef struct anon_mat_30 {
    tUVMatrix mat; /* 0x0 */
} anon_mat_30;
typedef struct tTargetStructMine {
    int dlIdx; /* 0x0 */
    int glowdls[2]; /* 0x4 */
    float flashTime; /* 0xc */
    anon_mat_30 mats[2]; /* 0x10 */
    float glowTimes[2]; /* 0x70 */
    signed char glowCnts[2]; /* 0x78 */
    unsigned char alphs[2]; /* 0x7a */
    float sc[2]; /* 0x7c */
    signed short on; /* 0x84 */
    signed short used; /* 0x86 */
} tTargetStructMine;
typedef struct tTargetStructCoin {
    int dlIdx; /* 0x0 */
    tUVMatrix mat; /* 0x4 */
    float glowTime; /* 0x34 */
    float sc; /* 0x38 */
    float angV[3]; /* 0x3c */
    unsigned short cameras; /* 0x48 */
    unsigned short whichcoin; /* 0x4a */
} tTargetStructCoin;
typedef struct tModelPart {
    char name[16]; /* 0x0 */
    tUVMatrix mat; /* 0x10 */
    unsigned int dlflags; /* 0x40 */
    signed short next; /* 0x44 */
    signed short child; /* 0x46 */
    signed short sfx1; /* 0x48 */
    signed short sfx2; /* 0x4a */
    int tidx; /* 0x4c */
    float min[3]; /* 0x50 */
    float max[3]; /* 0x5c */
} tModelPart;
typedef struct anon_colfront_colrear_10 {
    float colfront; /* 0x0 */
    float colrear; /* 0x4 */
    float colside; /* 0x8 */
    float colheight; /* 0xc */
} anon_colfront_colrear_10;
typedef struct anon_paths_count_8 {
    tBasicTargetPath *paths; /* 0x0 */
    int count; /* 0x4 */
} anon_paths_count_8;

typedef struct tTargetsObject { char pad0[8]; tUVMatrix *mat; char pad1[0x58-0xC]; } tTargetsObject;
typedef struct tTargetsModelDef { char name[16]; float radius; char pad0[0x48-0x14]; } tTargetsModelDef;
typedef struct anon_romObjDefs_count_8 { tTargetsModelDef *romObjDefs; int count; } anon_romObjDefs_count_8;
typedef struct anon_texDefs_count_8 { tTexture *texDefs; int count; } anon_texDefs_count_8;

extern tModelData model[];
extern tCarData gGameCar[];
extern tTargetsObject ObjInstanceList[];
extern anon_romObjDefs_count_8 gRomObjDefPtrs[];
extern anon_texDefs_count_8 gTexDefPtrs[];
extern surftyp *surf;
extern anon_colfront_colrear_10 collsize[];
extern unsigned short gObjList[],gMineTextures[];
extern int gWprObjList[];
extern signed char gCarTeam[];
extern unsigned char gNumModels;
extern int gNumPlayers,gRace,gPauseState;
extern unsigned int gGameState;
extern signed char gRestartGame,gBackwards;
extern volatile float gFrameSeconds;
extern float unitmatrix[4][3];
extern anon_paths_count_8 gTargetPaths;
extern signed char gMineCones,gCarMines,gRandomWeapons,gDemolitionBattle;
extern tTargetGroup *gTargetGroups;
extern int numgTargetGroups;
extern tTarget **gTargetGroupList;
extern int numgTargetGroupList;
extern unsigned short gMaskedRandomWeapons;
extern int gPathCount;
extern signed char Restarting,gTargetStructCount[8];
extern tMiniModelData *gBBoxModelStructs;
extern int gBBoxModelCount,gBBoxModelUsed,gBattleMineCnt;
extern int gGoldCoinCnt,gWhichGoldCoin,gSilverCoinCnt,gWhichSilverCoin;
extern tAlwaysCol *gAlwaysCol;
extern void *gTargetOwnStructPtrs[8],*gTargetStructPtrs[8];
extern tStatList gAlwaysColList,gTargetList;
extern tPlayerMines gPlayerMines[4];
extern tTarget gTargetArray[150];
void _StartPathTarget(tTarget *);
void _AnimatePathTarget(tVisual *,signed short);
void func_8C056058(void);
void PauseTargetPaths(tTarget *);
enum { kNoPause=0,kGhost=2,kBattle=6 };
#define sqrtf _builtin_sqrtf
#define fabs _builtin_fabsf
void AddToEnvList(tVisual *v);
void * AllocateMemoryHeap(void * heap, unsigned int size);
void BattleDamage(tCarData *car, tCarData *hit, int damage);
void BodyVector(float *uvec, float *bodvec, float (*uvs)[3]);
void CopyUV(float (*src)[3], float (*dst)[3]);
void DeallocateMemory(void * ptr);
signed short FindWorldGroup(float *pos);
void FoundCoin(int p, int coin);
void FreeStatLLE(tStatList *list, void * element);
void * GetStatLLE(tStatList *list);
tVisual *GrabEnvEntry();
void IncBattlePowerUps();
void InitBattlePowerUps(int restart);
void InitStatLL(tStatList *list, void * array, int elementSize, int numElements, char dbl);
int IsCoinFound(int p, int coin);
void LookInDir(float *lookdir, tUVMatrix *mat);
int MBOX_FindObject_WORK(const char *name, signed char firstModel, signed char lastModel, int flag);
tTexture *MBOX_FindTexture_WORK(const char *name, unsigned short *index, signed char firstModel, signed char lastModel, int flag);
float MBOX_GetObjectRadius(int mbidx);
void MBOX_GetPosition(float *pos, int dlidx);
void MBOX_HideObject(int dlidx, int mode, int camlink);
signed short MBOX_NewObject(int obj, tUVMatrix *mat, int parent, unsigned int flags);
void MBOX_ShowObject(int dlidx, int mode, int camlink);
int PlayTargetSound(int fxid, int slot, float *pos, unsigned char priority);
void PreInitBattlePowerUps();
void ProximityDamage(float *pos, float *old, int owner, float radiussq, int damage);
float Random(float max);
void ReleaseEnviron(tVisual *v, int unlink);
void RemoveCollisionObject(int num);
void ResetCollisionObject(int num);
surftyp *RoadPosition(float *ipos, float *opos, float (*uvs)[3]);
void RotateUV(float *pyr, float (*uvs)[3]);
void StartBlast(void * data, int linkToCar, float scale, int playSound);
void WorldVector(float *bodvec, float *uvec, float (*uvs)[3]);
void YawUV(float angle, float (*uv)[3]);
tRGBA *ZOID_GetColor(signed short objnum);
unsigned int ZOID_ObjectFlags(signed short objnum);
void ZOID_RemoveObject(signed short objnum);
void ZOID_SetColor(signed short objnum, tRGBA *col);
void ZOID_SetObjectDef(signed short objnum, unsigned short def);
void ZOID_SetObjectFlags(int objnum, unsigned int flags);
void ZOID_SetTexture(signed short objnum, tTexture *tex, int lod);
void setMiniFBCollisionForce(tModelData *m, tModelData *m1, tMiniModelData *m2, float *dir, float *pos);
unsigned int strlen(const char *);
int strncmp(const char *,const char *,unsigned int);
char *strstr(const char *,const char *);
typedef char checkTargetsModel[(sizeof(tModelData)==0x808)?1:-1];
typedef char checkTargetsTarget[(sizeof(tTarget)==0x70)?1:-1];
typedef char checkTargetsPoint[(sizeof(anon_pos_norm_44)==0x44)?1:-1];
typedef char checkTargetsInfo[(sizeof(tTargetInfo)==0x34)?1:-1];
typedef char checkTargetsMini[(sizeof(tMiniModelData)==0x40)?1:-1];
