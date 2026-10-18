/* Private resurrect overlays and declarations. C89, target-backed offsets. */
#ifndef RESURRECT_TYPES_H
#define RESURRECT_TYPES_H
#include "dc_types.h"
#define kBattle 6
#define kObstacle 5
#define kStunt 4
#define kCircuit 3
#define kGhost 2
#define kPractice 1
#define kOneRace 0
#define gGameCar ((tCarData *)0x8C004000)
#define gPlayers ((tPadState *)0x8C006000)
extern tModelData model[6];
typedef struct anon_pos_mph_8 { signed short pos[3]; unsigned char mph; char pad0[1]; } anon_pos_mph_8; /* 8byte stride48102; mph+6 at480F2/48100. */
typedef struct anon_numPts_pts_8 { unsigned short numPts; char pad0[2]; anon_pos_mph_8 *pts; } anon_numPts_pts_8; /* base14C59C, ptr+4 pool481AC. */
typedef struct anon_resurrect_firstPath_10 {
 unsigned char resurrect; char pad_first[4-1];
 signed char lastPath; char pad0[1]; unsigned short lastIdx;
 signed char startSection; char pad1[1]; unsigned short numPts; signed short (*pts)[3];
} anon_resurrect_firstPath_10; /* stride10: target4826C/492D8; fields byte0,4,8/word6,a/ptrc in helpers. */
typedef struct anon_numPts_pts_10 {
 unsigned short numPts; char pad0[2]; signed short (*pts)[3]; unsigned char numAlt; char pad1[3]; anon_resurrect_firstPath_10 *alt;
} anon_numPts_pts_10; /* base14C58C; numPts+0,pts+4,numAlt+8,alt+C, target helpers. */
typedef struct anon_pos_normal_50 {
 float pos[3],normal[3]; int radius; char pad0[0x22-0x1c]; signed short pathPts[20]; char pad1[0x50-0x4a];
} anon_pos_normal_50; /* checkpoint stride50, pathPts+22 pool481A8=baseBAB4+C+22. */
typedef struct anon_startTime_loopPt_32c {
 signed short startTime,loopPt,finishPt,startPt,numPoints; char pad0[2]; anon_pos_normal_50 points[10];
} anon_startTime_loopPt_32c; /* existing checkpoint target32C; loop+2,finish+4,points+C. */
typedef struct anon_track_car_a { char pad0[8]; unsigned char owner; char pad1[1]; } anon_track_car_a; /* target47F58..6C:baseC5A4+8,strideA. */
typedef char resurrect_model_size[(sizeof(tModelData)==0x808)?1:-1];
typedef char resurrect_car_size[(sizeof(tCarData)==0x3cc)?1:-1];
typedef char resurrect_res_data_size[(sizeof(tResurrect)==0x70)?1:-1];
typedef char resurrect_path_size[(sizeof(anon_numPts_pts_10)==0x10)?1:-1];
typedef char resurrect_checkpoint_size[(sizeof(anon_pos_normal_50)==0x50)?1:-1];
extern float unitmatrix[4][3];
extern void ExtractYaw(float (*)[3],float *);
extern surftyp *RoadPosition(float *,float *,float (*)[3]);
extern void CopyUV(float (*)[3],float (*)[3]),fyaw(float,float,float (*)[3]);
extern void syminit(tModelData *),multigo(signed short);
extern signed char gDeathMode,gTrackNum,gBackwards,gSuperSpeed,gEndGame,gCoastFlag,gNumActiveCars;
extern int gRace;
extern float gModelTime;
extern void CrashedStunt(int),InitBattlePlayer(signed short);
extern anon_numPts_pts_8 gDronePaths[4];
extern anon_startTime_loopPt_32c gCheckpoints;
extern anon_numPts_pts_10 gResPath;
extern anon_track_car_a gLink[13];
extern void SetNewPathIndex(tModelData *,signed short),make_super_speed(tModelData *);
extern float magnitude(float *),fatan2(float,float),fasin(float),Random(float),sinf(float),cosf(float);
extern float _builtin_sqrtf(float),_builtin_fabsf(float);
extern unsigned int gGameState;
extern signed short gNumPlayers,gNumLaps;
extern void NextResPathPt(int,int,int *,int *),BodyVector(float *,float *,float (*)[3]);
extern void WorldVector(float *,float *,float (*)[3]),TB_Vector3Normalize(float *),NormalVector(float *);
extern signed short GetNextCheckpoint(signed short),FindClosestPathPt(signed short *,signed short);
extern void SetDroneResPathIndex(tModelData *,signed short,signed short),SyncMaxpathToLastCheckpoint(signed short);
extern int FindResPt(int,int,int *,int *,int),NumPtsToCheckpoint(int,int,int,int),CountdownTimeOut(void);
extern void TracePathPtForward(int,int,int,int,int *,int *),PassedCP(tModelData *,float);
#endif
