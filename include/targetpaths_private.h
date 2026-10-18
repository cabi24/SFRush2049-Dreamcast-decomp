#include "dc_types.h"
#ifndef TARGETPATHS_PRIVATE_H
#define TARGETPATHS_PRIVATE_H
typedef tCarData tTargetGameCar;
typedef struct tTarget tTarget;
typedef struct tBasicTargetPath tBasicTargetPath;
typedef struct tTargetStructPath tTargetStructPath;

typedef struct tUV3 { float uvs[3][3]; float pos[3]; } tUV3;

 /* Target+4 index,+C data,+10 timeStamp,+14 callback; stride18. */
struct tTarget {
    char pad0[4];
    unsigned char Flags;  /* target568FC byte+4. */
    char pad1[0xC-5];
    int dlIdx;
    signed short infoIdx; /* target568F2 word+10; info stride34. */
    tUVMatrix mat;        /* targetaudio pos+38=14+24; UV+14. */
    char pad2[0x50-0x44];
    signed short type;
    char pad3[0x58-0x52];
    signed short otype,frames;
    char pad4[0x60-0x5C];
    int sound;            /* targetPause57612/18 long+60. */
    signed char sndState; /* targetPause5760C/10 byte+64. */
    signed char owner;
    char pad5[0x6C-0x66];
    void *data;           /* target560A6/A8 +6C pointer. */
};
typedef struct anon_pos_norm_44 {
    float pos[3],norm[3],scale[3],quat[4];
    float distToNext,timeToNext,speed;
    unsigned int flags;
} anon_pos_norm_44; /* target561A2..B2 index*68; flags+40. */
struct tTargetStructPath {
    tBasicTargetPath *pathData; /* target560AA pointer+0. */
    float timeElapsed;
    char pad0[4];
    signed short ptOn;      /* target560BA mov.w+0C. */
    unsigned short flags;   /* target560B2/B4 mov.w+0E thenextu.w. */
    float curVel,maxVel;    /* target560BE..C8 float+10; moveSound+14. */
    float lastPos[3];
    float lastUVS[3][3];
};
struct tBasicTargetPath {
    char pad0[0x10];
    unsigned int flags;   /* target561B4 long+10. */
    signed short numPts;  /* target561A0 signedword+14. */
    signed short collIdx; /* target560AC signedword+16. */
    void *trigPtr;
    anon_pos_norm_44 *pts; /* target560C0 long+1C. */
    tTargetStructPath *parent;
};
typedef struct tTargetInfo {
    char pad0[0xC];
    void (*animFunc)(tVisual *,signed short);
    signed short collFuncIdx;
    char pad1[2];
    signed short objListIdx;
    char pad2[0x1C-0x16];
    int startSnd,moveSnd,stopSnd,replace;
    float fadeDist;
    char pad3[4];
} tTargetInfo; /* target57586/588 stride52; audio fields+1C/+20/+24/+28/+2C. */

 /* Calibrated shared statll layout; sizeof18, opaque here. */
extern tTargetGameCar gGameCar[]; /* only index0 referenced: target56374..384. */
extern tTargetInfo targetInfo[];
extern tTarget *CollisionTargetPtrs[]; /* target56230=8C15861C,30slots. */
extern int numCollisionTargetPtrs; /* target56234=8C158694. */
extern float gFrameSeconds,unitmatrix[4][3];
extern int gRace,gPauseState;
extern unsigned int gGameState;
extern unsigned short gObjList[];
extern tStatList gTargetList;
enum { kNoPause=0,kGhost=2,kObstacle=5 };
void MoveCollisionObject(int,float *);
void RotateCollisionObject(int,float *,float (*)[3],float (*)[3]);
void ResetCollisionObject(int);
void InterpQuats(float,float *,float *,float *);
void CreateQuatMat(float *,float *,int);
void CopyUV(float (*)[3],float (*)[3]);
void TransposeUV(float (*)[3]);
tVisual *GrabEnvEntry(void);
void AddToEnvList(tVisual *);
void ReleaseEnviron(tVisual *,int);
void ZOID_SetObjectDef(signed short,unsigned short);
void ZOID_RemoveObject(signed short);
void FreeStatLLE(tStatList *,void *);
void UpdateMiniModelStruct(int,float *,float *,float (*)[3]);
void AudStopEmitter(int);
int AudEmitterPlaying(int);
int EmitGameSound(float *,float *,float,float,float,float,int,int,int,unsigned char);
void AudUpdateEmitter(int,float *,float *,float,float);
#define sqrtf _builtin_sqrtf
#define path_abs(x) ((x)<0?-(x):(x))
typedef char checkVisual[(sizeof(tVisual)==0x18)?1:-1];
typedef char checkTarget[(sizeof(tTarget)==0x70)?1:-1];
typedef char checkPathPoint[(sizeof(anon_pos_norm_44)==0x44)?1:-1];
typedef char checkPathData[(sizeof(tTargetStructPath)==0x48)?1:-1];
typedef char checkBasicPath[(sizeof(tBasicTargetPath)==0x24)?1:-1];
typedef char checkTargetInfo[(sizeof(tTargetInfo)==0x34)?1:-1];
#endif
