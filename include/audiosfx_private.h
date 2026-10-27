#ifndef AUDIOSFX_PRIVATE_H
#define AUDIOSFX_PRIVATE_H
#include "dc_types.h"

typedef struct DcAudioHandle {
    char opaque0[0x2C];
    int resource_id; /* +2C: 8C0161DE,8C01633C (voice+3C). */
    char opaque1[0x40 - 0x30];
} DcAudioHandle;
typedef struct tSfxVoice {
    signed char used, started, stop, stopped, stopAll, fail;
    char opaque0[0x08 - 0x06];
    unsigned int index; /* +08: 8C0165B6. */
    int soundId; /* +0C: 8C016222. */
    DcAudioHandle handle; /* +10..50: 8C016578..584 memset64; stride54. */
    unsigned int basePitch; /* +50: 8C0162F0,8C016458..46E unsigned->float. */
} tSfxVoice;
typedef struct tSfxCmd {
    tDLLE dlle;
    signed char cmd, failCounter; /* +08/+09: 8C016646,8C016ABC. */
    char opaque0[0x0C - 0x0A];
    tSfxVoice *sound; /* +0C: 8C016654. */
    volatile float vol, lrpan, fbpan, pitch; /* +10..20: compare and separate reread 8C016A5E/64, A70/76. */
} tSfxCmd;
typedef struct DcSfxBank {
    char name[20];
    int first, last; /* +14/+18: 8C01617E/184. */
    signed char loaded, locked; /* +1C/+1D: 8C015FE4/FEA. */
    char opaque0[0x20 - 0x1E];
    unsigned int handle; /* +20: 8C015FD6; stride24: 8C016190. */
} DcSfxBank;
typedef char aud_voice_size[(sizeof(tSfxVoice)==0x54)?1:-1];
typedef char aud_cmd_size[(sizeof(tSfxCmd)==0x20)?1:-1];
typedef char aud_handle_size[(sizeof(DcAudioHandle)==0x40)?1:-1];
typedef char aud_bank_size[(sizeof(DcSfxBank)==0x24)?1:-1];

extern tSfxVoice *gSfxInstances; /* 8C123C7C */
extern int gNumChannels; /* 8C123C80 */
extern int gStoppedChannels[1]; /* 8C123C84 */
extern int gNumInstances; /* 8C123C88 */
extern tSfxCmd *gCmds; /* 8C123C8C */
extern tDLList gFreeQ, gCmdQ; /* 8C123C90/8C123CA0 */
extern signed char gInited, gStopAllEffects; /* 8C0BAD90/8C0BAD98 */
extern unsigned int gNextIndex; /* 8C0BAD94: initialized1. */
extern int gAudNumLoadedBanks; /* 8C0BAD9C */
extern signed char gAudBankEvictionEnabled; /* 8C0BADA0 */
extern unsigned int gAudNumActiveResources; /* 8C0BADA4 */
extern unsigned char gAudVolumeCurve[256]; /* 8C0BADA8; indexed8-bit, ownership unresolved. */
extern DcSfxBank *gAudLoadedBanks[]; /* 8C123CB0; allocation end unresolved. */
extern int gAudActiveResources[]; /* 8C123CD4; allocation end unresolved. */
extern DcSfxBank gAudBankTable[9]; /* 8C0A7904; 9*24-byte named-bank records. */
extern float gSfxMasterVol; /* 8C178440 */
extern signed char gExitGame; /* 8C0BEBBC */
extern signed char gMono; /* 8C0C7C50 */

extern void *AllocateMemoryHeap(int, unsigned int);
extern void DeallocateMemory(void *);
extern void PurgeableMemory(void *, int);
extern void *memset(void *, int, unsigned int);
extern void *memcpy(void *, const void *, unsigned int);
extern void InitLL(tDLList *, int, int);
extern void RemoveLLE(tDLList *, void *);
extern void InsertLLE(tDLList *, void *, void *);
extern int func_8C0A0EE0(const char *, unsigned int *);
extern int func_8C0A0F2A(const char *, void *, unsigned int);
extern void func_8C039718(void);
extern int func_8C0A18E6(unsigned int *);
extern int func_8C0A191C(unsigned int *, unsigned int, int, int, int);
extern int func_8C0A0E48(unsigned int, const void *, unsigned int);
extern int func_8C0A1BB4(unsigned int);
extern float func_8C06AEC8(void);
extern int func_8C06AF30(float, float);
extern int func_8C0A2A60(unsigned int, int, DcAudioHandle *);
extern int func_8C0A2BF0(DcAudioHandle *);
extern int func_8C0A29BE(DcAudioHandle *);
extern int func_8C0A2DB2(DcAudioHandle *, int);
extern int func_8C0A2E1C(DcAudioHandle *, unsigned int);
extern int func_8C0A2C26(DcAudioHandle *, unsigned int *);
extern int func_8C0A3118(DcAudioHandle *);
extern int func_8C0A2EEA(DcAudioHandle *);
extern int func_8C0A2F10(DcAudioHandle *);
extern int func_8C0A2D2C(DcAudioHandle *, unsigned int);
extern int func_8C09CB80(void);
extern void func_8C0D84D4(int);

void func_8C015EF0(DcSfxBank *, int);
void func_8C016044(void);
void func_8C016128(DcSfxBank *);
DcSfxBank *func_8C016174(int);
int func_8C0161B2(DcAudioHandle *);
int func_8C016210(tSfxVoice *, float, float);
int func_8C01631E(tSfxVoice *);
int func_8C01632A(tSfxVoice *);
int func_8C0163BC(tSfxVoice *, float);
int func_8C016406(tSfxVoice *, float);
int func_8C016454(tSfxVoice *, float);
int func_8C0164AA(int);
signed char func_8C016508(float);
int func_8C016538(tSfxVoice **);
tSfxCmd *func_8C0165F4(void);
tSfxCmd *func_8C016612(tSfxVoice *);
void _FreeCmd(tSfxCmd *); /* Accepted map label; body is MAT3 _SendCmd. */
void AudioSfxInit(int);
void func_8C0167E8(void);
void AudioSfxServer(void);
unsigned int AudioPlayEffect(int, float, float);
int AudioEffectPlaying(unsigned int);
void AudioStopEffect(unsigned int);
void func_8C016C3C(void);
int func_8C016CA8(void);
void AudioSetEffectVolume(unsigned int, float);
void AudioSetEffectLRPan(unsigned int, float);
void AudioSetEffectFBPan(unsigned int, float);
void AudioSetEffectPitch(unsigned int, float);
void func_8C016ED0(int);

#define FREE_VOICE(s) do { tSfxVoice *theSound=(s); theSound->used=0; theSound->index=0; } while(0)
#define RELEASE_CMD(c) do { tSfxCmd *releaseCmd=(c); RemoveLLE(&gCmdQ,releaseCmd); InsertLLE(&gFreeQ,releaseCmd,gFreeQ.head); } while(0)
#endif
