#include "dc_types.h"
/* Private checkpoint overlays. All offsets follow target instructions/pools. */
#ifndef CHECKPOINT_TYPES_H
#define CHECKPOINT_TYPES_H


typedef struct anon_pos_normal_50 {
    float pos[3]; /* 8C01BE08..42 */
    float normal[3]; /* 8C01BE62..AA */
    int radius; /* 8C01BEE2..EA */
    char pad0[0x50 - 0x1c]; /* index*80 at 8C01BDFE..BE08 */
} anon_pos_normal_50;
typedef struct anon_startTime_loopPt_32c {
    signed short startTime;
    signed short loopPt;
    signed short finishPt; /* PassedCP 8C01C046 +4 */
    signed short startPt; /* pointer 8C01C0F4 = base+6 */
    signed short numPoints;
    char pad0[2];
    anon_pos_normal_50 points[10]; /* BAC0 = BAB4+C */
} anon_startTime_loopPt_32c;
typedef struct anon_track_car_a {
    char pad0[8];
    unsigned char owner; /* 8C01BDC8..CA; stride 8C01BF22 */
    char pad1[1];
} anon_track_car_a;

typedef char check_model[(sizeof(tModelData)==0x808)?1:-1];
typedef char check_gc[(sizeof(tCarData)==0x3cc)?1:-1];
typedef char check_point[(sizeof(anon_pos_normal_50)==0x50)?1:-1];
typedef char check_checkpoints[(sizeof(anon_startTime_loopPt_32c)==0x32c)?1:-1];
typedef char check_link[(sizeof(anon_track_car_a)==0xa)?1:-1];
extern tModelData model[6];
extern tCarData gGameCar[6];
extern anon_track_car_a gLink[13];
extern tPadState gPlayers[4];
extern signed char gNumActiveCars, gEndGame, gTrackNum;
extern signed short gNumLaps, gNumCars, num_humans, gNumPlayers;
extern int gRace;
extern unsigned int gGameState;
extern float gLapDistance;
extern volatile float gSeconds, gFrameSeconds;
extern signed char finish_cam[6];
extern float finish_cam_stime[6];
void ReadCheckpointData(void);
float magnitude(float *v);
void PassedCP(tModelData *m,float time);
void CarReportsGameOver(int slot,float score);
void FindMaxpathIntervals(void);
void PlayGameSound(int sound,int slot,int a,int b);
unsigned int GetNextCheckpoint(unsigned int cp);
void ShowCheckpointTime(int slot,float time);
void RecordLap(int slot,float time);
int AudPlaySequence(int priority,float timeout,int replace,int subId,int num,...);
void AddCountdownTime(float time);
float GetCountdownTimeRemaining(void);
#endif
