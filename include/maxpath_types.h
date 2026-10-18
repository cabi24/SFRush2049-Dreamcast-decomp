/* Private maxpath target-backed overlays. See evidence/layout-proof.json.
 * Checkpoint stride50: Sync A4EE..A4FE; point.pathPts+22; aggregate+C.
 * Physical rwv220/rwr22C stay distinct from reckon.rwr794 anduvs7A0.
 */
#include "dc_types.h"
typedef struct anon_pos_mph_8 {
    signed short pos[3]; /* 0x0 */
    unsigned char mph; /* 0x6 */
    signed char hints; /* 0x7 */
} anon_pos_mph_8;
typedef struct anon_numPts_pts_8 {
    unsigned short numPts; /* 0x0 */
    anon_pos_mph_8 *pts; /* 0x4 */
} anon_numPts_pts_8;
typedef struct anon_resurrect_firstPath_10 {
    unsigned char resurrect; /* 0x0 */
    signed char firstPath; /* 0x1 */
    unsigned short firstIdx; /* 0x2 */
    signed char lastPath; /* 0x4 */
    unsigned short lastIdx; /* 0x6 */
    signed char pad_unproven_startSection; /* +8 unused by this unit; no named field claim. */
    unsigned short numPts; /* 0xa */
    signed short (*pts)[3]; /* 0xc */
} anon_resurrect_firstPath_10;
typedef struct anon_numPts_pts_10 {
    unsigned short numPts; /* 0x0 */
    signed short (*pts)[3]; /* 0x4 */
    unsigned char numAlt; /* 0x8 */
    anon_resurrect_firstPath_10 *alt; /* 0xc */
} anon_numPts_pts_10;
typedef struct tMaxpathCheckpoint {
 char pad0[0x22];
 signed short pathPts[20]; /* Sync A4F2/504:base12BAE2=checkpoints+C+22. */
 float distToNext; /* FindMaxpathIntervalsB8BC..B95A, row+4C. */
} tMaxpathCheckpoint;
typedef struct tMaxpathCheckpoints {
 char pad_start[2]; /* +0 unused by this unit. */
 signed short loopPt,finishPt,startPt,numPoints;
 char pad0[2];
 tMaxpathCheckpoint points[10]; /* +C; row stride50.10-row allocation end not newly claimed. */
} tMaxpathCheckpoints;
typedef char check_pathpoint[(sizeof(tPathPoint)==0x2c)?1:-1];
typedef char check_pathpoint8[(sizeof(anon_pos_mph_8)==8)?1:-1];
typedef char check_path8[(sizeof(anon_numPts_pts_8)==8)?1:-1];
typedef char check_altpath[(sizeof(anon_resurrect_firstPath_10)==0x10)?1:-1];
typedef char check_respath[(sizeof(anon_numPts_pts_10)==0x10)?1:-1];
typedef char check_checkpointrow[(sizeof(tMaxpathCheckpoint)==0x50)?1:-1];
extern tModelData model[6];
extern tCarData gGameCar[6];
extern float gModelTime;
extern signed short gNumActiveCars,num_drones,drones[6];
extern tMaxpathCheckpoints gCheckpoints;
extern anon_numPts_pts_10 gResPath;
extern anon_numPts_pts_8 gDronePaths[4];
extern float gFirstLapDistance,gLapDistance;
extern signed char gGravityScale;
extern signed short current_drone;
extern float gLastPathSwitchTime;
extern int gPathSearch[15];
extern float last_save_dir[6][3],last_dir[6][3];
extern float lastthrottle[6],lastbrake[6],lastwheel[6];
void ReadPathData(void);
signed short NextPathPt(signed short,signed short);
signed short PrevPathPt(signed short,signed short);
void NextResPathPt(int,int,int *,int *);
int TraceCheck(int,int);
float GetPathDistance(int,int);
int AreWeSolid(signed short);
float TB_Vector2Normalize(float *);
float TB_Vector3Normalize(float *);
void BodyVector(float *,float *,float (*)[3]);
float magnitude(float *);
void TargetPathAvoid(tModelData *,float *);
void TB_DrawTextC(signed short,signed short,int,void *);
float fatan2(float,float);
