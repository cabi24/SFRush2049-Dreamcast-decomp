/* Private audiomgr overlays. Shared tDLLE/list come from the lane snapshot.
 * DC evidence: command stride18 at3A8A/3AA2..AA6; Fx stride44 at3B0A..3B18;
 * sequence stride3C at3C02/3C06; listener3C at515C..5174; emitter2C at51B8..51C8.
 * Field references are recorded in evidence/layout-proof.json, not inferred from MAT3 alone.
 */
#include "dc_types.h"
typedef struct tBSTE tBSTE;
typedef struct tOrderedList tOrderedList;
typedef struct tFxSequence tFxSequence;
typedef struct anon_te_tag_c anon_te_tag_c;
typedef struct anon_ll_free_3c anon_ll_free_3c;
typedef struct tSfxEmitter tSfxEmitter;
typedef struct anon_ll_id_3c anon_ll_id_3c;
typedef struct anon_type_cmd_4 anon_type_cmd_4;
typedef struct anon_hdr_song_c anon_hdr_song_c;
typedef struct anon_hdr_4 anon_hdr_4;
typedef struct anon_hdr_replace_18 anon_hdr_replace_18;
typedef struct anon_hdr_tag_8 anon_hdr_tag_8;
typedef struct anon_hdr_vol_18 anon_hdr_vol_18;
typedef struct anon_hdr_vol_10 anon_hdr_vol_10;
typedef struct anon_hdr_mono_6 anon_hdr_mono_6;
typedef union tAudioMsg tAudioMsg;
struct tBSTE {
    void * left; /* 0x0 */
    void * right; /* 0x4 */
};
struct tOrderedList {
    int (*OrderFunc)(void *, void *); /* 0x0 */
    long numElements; /* 0x4 */
    void * root; /* 0x8 */
};
struct tFxSequence {
    tDLLE ll; /* 0x0 */
    tDLList *list; /* 0x8 */
    unsigned int id; /* 0xc */
    int state; /* 0x10 */
    int type; /* 0x14 */
    signed char paused; /* 0x18 */
    signed char loop; /* 0x19 */
    unsigned char numCmds; /* 0x1a */
    unsigned char priority; /* 0x1b */
    int wpriority; /* 0x1c */
    float startTime; /* 0x20 */
    float vol; /* 0x24 */
    float lrpan; /* 0x28 */
    float fbpan; /* 0x2c */
    float pitch; /* 0x30 */
    int fxid; /* 0x34 */
    int subId; /* 0x38 */
    unsigned int sysId; /* 0x3c */
    void * data; /* 0x40 */
};
struct anon_te_tag_c {
    tBSTE te; /* 0x0 */
    tFxSequence *tag; /* 0x8 */
};
struct anon_ll_free_3c {
    tDLLE ll; /* 0x0 */
    signed char free; /* 0x8 */
    signed char active; /* 0x9 */
    float pos[3]; /* 0xc */
    float dir[3]; /* 0x18 */
    float look[3]; /* 0x24 */
    float up[3]; /* 0x30 */
};
struct tSfxEmitter {
    tDLLE ll; /* 0x0 */
    signed char free; /* 0x8 */
    signed char active; /* 0x9 */
    float pos[3]; /* 0xc */
    float fadeDistance; /* 0x18 */
    float maxVol; /* 0x1c */
    float minVol; /* 0x20 */
    float basePitch; /* 0x24 */
    int sndTag; /* 0x28 */
};
struct anon_ll_id_3c {
    tDLLE ll; /* 0x0 */
    unsigned int id; /* 0x8 */
    signed char free; /* 0xc */
    signed char active; /* 0xd */
    signed char stop; /* 0xe */
    float startTime; /* 0x10 */
    float timeout; /* 0x14 */
    unsigned char next; /* 0x18 */
    int ids[4]; /* 0x1c */
    int subId; /* 0x2c */
    int replace; /* 0x30 */
    unsigned char priority; /* 0x34 */
    int sndTag; /* 0x38 */
};
struct anon_type_cmd_4 {
    signed short type; /* 0x0 */
    char cmd; /* 0x2 */
    signed char used; /* 0x3 */
};
struct anon_hdr_song_c {
    anon_type_cmd_4 hdr; /* 0x0 */
    int song; /* 0x4 */
    signed char loop; /* 0x8 */
};
struct anon_hdr_4 {
    anon_type_cmd_4 hdr; /* 0x0 */
};
struct anon_hdr_replace_18 {
    anon_type_cmd_4 hdr; /* 0x0 */
    int replace; /* 0x4 */
    float vol; /* 0x8 */
    float lrpan; /* 0xc */
    float fbpan; /* 0x10 */
    tFxSequence *tag; /* 0x14 */
};
struct anon_hdr_tag_8 {
    anon_type_cmd_4 hdr; /* 0x0 */
    tFxSequence *tag; /* 0x4 */
};
struct anon_hdr_vol_18 {
    anon_type_cmd_4 hdr; /* 0x0 */
    float vol; /* 0x4 */
    float lrpan; /* 0x8 */
    float fbpan; /* 0xc */
    float pitch; /* 0x10 */
    tFxSequence *tag; /* 0x14 */
};
struct anon_hdr_vol_10 {
    anon_type_cmd_4 hdr; /* 0x0 */
    float vol; /* 0x4 */
    float time; /* 0x8 */
    signed char music; /* 0xc */
    signed char sfx; /* 0xd */
};
struct anon_hdr_mono_6 {
    anon_type_cmd_4 hdr; /* 0x0 */
    signed char mono; /* 0x4 */
};
union tAudioMsg {
    anon_type_cmd_4 hdr; /* 0x0 */
    anon_hdr_song_c playSong; /* 0x0 */
    anon_hdr_4 killSong; /* 0x0 */
    anon_hdr_replace_18 playEffect; /* 0x0 */
    anon_hdr_tag_8 startEffect; /* 0x0 */
    anon_hdr_vol_18 changeEffect; /* 0x0 */
    anon_hdr_tag_8 intEffect; /* 0x0 */
    anon_hdr_tag_8 stopEffect; /* 0x0 */
    anon_hdr_4 stopAllEffects; /* 0x0 */
    anon_hdr_4 pauseAllEffects; /* 0x0 */
    anon_hdr_4 resumeAllEffects; /* 0x0 */
    anon_hdr_vol_10 setMasterVol; /* 0x0 */
    anon_hdr_mono_6 mono; /* 0x0 */
};
typedef char check_tFxSequence[(sizeof(tFxSequence)==0x44)?1:-1];
typedef char check_anon_te_tag_c[(sizeof(anon_te_tag_c)==0xc)?1:-1];
typedef char check_anon_ll_free_3c[(sizeof(anon_ll_free_3c)==0x3c)?1:-1];
typedef char check_tSfxEmitter[(sizeof(tSfxEmitter)==0x2c)?1:-1];
typedef char check_anon_ll_id_3c[(sizeof(anon_ll_id_3c)==0x3c)?1:-1];
typedef char check_tAudioMsg[(sizeof(tAudioMsg)==0x18)?1:-1];

void AudioInitSys(int,int,int);
void AudioShutdownSys(void);
void AudioPeriodicMaintenance(void);
int SetInterruptMask(int);
void *AllocateMemoryHeap(int,unsigned int);
void PurgeableMemory(void *,int);
void DeallocateMemory(void *);
void InitLL(tDLList *,char,char);
void InsertLLE(tDLList *,void *,void *);
void RemoveLLE(tDLList *,void *);
void InitBST(tOrderedList *,int (*)(void *,void *));
void InsertBSTE(tOrderedList *,void *);
void ClearBST(tOrderedList *);
void WalkBST(tOrderedList *,void (*)(void *,int));
unsigned char HAL_IntLog2(unsigned int);
void AudioSongPlay(int,int);
void AudioSongStop(int);
unsigned int AudioPlayEffect(int,float,float);
void AudioStopEffect(unsigned int);
void AudioSetEffectVolume(unsigned int,float);
void AudioSetEffectLRPan(unsigned int,float);
void AudioSetEffectFBPan(unsigned int,float);
void AudioSetEffectPitch(unsigned int,float);
int AudioEffectPlaying(unsigned int);
void AudioStopAllEffects(void);
void AudioSetMasterVolume(float,unsigned int,int,int);
void AudioSetMono(int);
float GetAudioTime(void);
int AudioTimeout(float,float);
float AudioTimeoutPerc(float,float);
float TB_Vector3Normalize(float *);
/* Address-proven BSS, no initialized image credit. */
extern int gSavedAudioMask;
extern tAudioMsg gAudioCommandQueue[128];
extern tAudioMsg *gStartCmds[128], *gStopCmds[128];
extern tOrderedList gSortedFxTagTree;
extern tDLList gFreeFxTags,gPlayingFxTags,gInterruptedFxTags;
extern unsigned int gTagShift,gTagMask;
extern tDLList gFreeListeners,gActiveListeners,gFreeEmitters,gActiveEmitters;
extern int gMaxSfx,gMaxMusic;
extern signed char gCalcDoppler;
extern tDLList gFreeSequences,gActiveSequences;
extern float gFrontDistance,gSoundSpeed;
/* Varargs body is not ported without verified native stdarg definitions. */
int AudPlaySequence(int,float,int,int,int,...);
