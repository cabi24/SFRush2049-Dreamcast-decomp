#ifndef MDRIVE_TYPES_H
#define MDRIVE_TYPES_H
#include "dc_types.h"
#define kGhost 2
#define kStunt 4
#define kBattle 6
#define kObstacle 5
#define gGameCar ((tCarData *)0x8C004000)
#define gPlayers ((tPadState *)0x8C006000)
typedef struct anon_track_car_a {
 unsigned char track,car,PaintJob; char pad0[3]; unsigned char polepos,flags,owner; char pad1;
} anon_track_car_a; /* strideA:D68C..698; fields0,1,2,6,7,8 in init/fake. */
typedef struct anon_colfront_colrear_10 { float colfront,colrear,colside,colheight; } anon_colfront_colrear_10; /* DC colsize stride10, fourloadsDC10..44. */
typedef struct _tSusp { float fwdforcemin,fwdforcerange,fwdangle,bckforcemin,bckforcerange,bckangle,sideforcemin,sideforcerange,sideangle; } _tSusp; /* actual float loads0/4/8/C/10/14/18/1C/20 in F168..F26C. */
typedef struct tSkidData { int forceTable[5]; float skidThreshold,smokeThreshold; } tSkidData; /* target F98Aindexedlongs and F9D8offset14/F?18; stride1C. */
typedef struct dcGhostHeaderSlice { char pad0[5]; signed char flag; } dcGhostHeaderSlice; /* EAD2 byte+5, signed cmp/pz. */
typedef struct dcGhostFileSlice { char pad0[0x2c]; dcGhostHeaderSlice **header; } dcGhostFileSlice; /* EACC/EACE/EAD0:handle then+2C handle. */
extern tModelData model[6];
extern tCamera gCamera[1];
extern anon_track_car_a gLink[13];
extern tCar *gCarModels[13];
extern signed char gCarHandling[5][13],gCarEngines[5][13],gCarTires[5][13],gCarFrame[5][13],gTireRims[5][13];
extern signed char gCarSuspensions[13],gDriveHulk[4];
extern _tSusp *carSuspensions[3];
extern anon_colfront_colrear_10 collsize[13];
extern dcGhostFileSlice **gGhosts[4];
extern signed short modelrun,gNumPlayers,gNumLaps;
extern signed char gTrackNum,gUseCatchup,gOneMoreCP,gDeathMode,gCoastFlag;
extern unsigned int gGameState;
extern int gRace,gAPlayerReallyFinished;
extern signed short gStuntsDone;
extern float gRetraceTime,gFrameSeconds;
extern void InitCarVisuals(tCarData *),InitCarPath(tModelData *,tCarData *);
extern signed short GetNextCheckpoint(signed short);
extern void CarReportsGameOver(int,float),SetResurrectionData(signed short,tModelData *,int),Resurrect(tModelData *);
extern void StartCarSounds(tModelData *),StopCarSounds(tModelData *),UpdateCarSounds(tModelData *);
extern void ZOID_UpdateObject(signed short,float *,float (*)[3]),CheckCameraView(void);
extern void UpdateModelControls(void),dead_reckon_all(void),game_reckon_all(float),UpdateListeners(void);
extern int GhostUpdate(int,float *);
extern void AbortCar(signed short,int),collision(tModelData *),CheckCrash(signed short),FinishResurrecting(tModelData *),UpdateResurrectingCar(tModelData *);
extern void sym(tModelData *),MaxPathControls(tModelData *),update_reckon_base(int,float);
extern void PS_SetRumblePercent(int,int),PS_RumbleStop(void),PS_RumbleGo(void);
extern void CopyUV(float (*)[3],float (*)[3]),PitchUV(float,float (*)[3]),RollUV(float,float (*)[3]),WorldVector(float *,float *,float (*)[3]);
extern float _builtin_sqrtf(float),_builtin_fabsf(float);
#endif
