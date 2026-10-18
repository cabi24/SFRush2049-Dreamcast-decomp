#ifndef DC_SOUND_TYPES_H
#define DC_SOUND_TYPES_H
#include "dc_types.h"
typedef struct tMidiSDKState {
 char opaque0[0x340];
 int beginFlag; /* record+348: poolA4CE, storeA496. */
 char opaque344[0x400-0x344]; /* next named object atrecord+408 */
} tMidiSDKState;
typedef struct tMidiJob tMidiJob;
struct tMidiJob {
 tMidiJob *next,*prev; /* record0 list traversal;8-byte LL prefix. */
 tMidiSDKState state; /* +8: A3AA/A3C6/A3D6 and all SDK helper calls */
 int workBytes,readBytes; /* +408/+40C: A0B8/A0A8; A3B4/A3BC */
 void *workHandle,*readBuffer1,*readBuffer2; /* +410/414/418:A0B0/A0A0/A0C2 */
 char path[32]; /* +41C:poolA148/copyA07C, following byte+43C */
 signed char loop,cancel; /* 43C/43D: A084; A118/A146 */
 unsigned char volume,pan; /* 43E/43F:A094/A09A; volume extuA2A6 */
 int song,ticks; /* 440/444: A0C8/A29C */
 signed char stopIssued; /* 448:A224/A33A/A41E */
}; /* Last observed byte448; full allocation end is not claimed. */
typedef struct tStreamJob {
 struct tStreamJob *next,*prev;
 char opaque08[0x56-8];
 signed char cancel; /* A568..56A; no full stride claimed. */
} tStreamJob;
extern tDLList midiFree8C177B3C,midiActive8C177B4C,midiPending8C177B5C;
extern tDLList streamFree8C177FC0,streamActive8C177FD0,streamPending8C177FE0;
extern char *songPaths8C0BA818[18]; /* indexedA070; threshold18 routesstream. End not independently mapped. */
extern float songMasterVolume8C17843C;
#define strcpy _builtin_strcpy
void DeallocateMemory(void *);
int sdkFree8C0A1BB4(void *);
void RemoveLLE(tDLList *,void *);
void InsertLLE(tDLList *,void *,void *);
signed char convertVolume8C016508(float);
int sdkPath8C09FCC6(tMidiSDKState *,const char *);
int sdkBuffers8C09FBB6(tMidiSDKState *,void *,int,void *,int);
int sdkReadBuffer8C09FAF4(tMidiSDKState *,void *);
int sdkStop8C09FD6A(tMidiSDKState *);
int sdkReset8C09F734(tMidiSDKState *);
int sdkClose8C09F9F2(tMidiSDKState *);
int sdkDone8C09FA24(tMidiSDKState *,int *);
int sdkBusy8C09FAC4(tMidiSDKState *,int *);
int sdkStart8C09FB2C(tMidiSDKState *);
int sdkVolume8C09F7FC(tMidiSDKState *,int,int);
void sdkTick8C0A0064(tMidiSDKState *);
void audioFlush8C06B1A8(void);
void func_8C06A5D0(int,int);
void func_8C06A738(int);
void func_8C06A836(void);
void func_8C06A0DA(int);
tMidiJob *func_8C06A320(void);
void func_8C06A348(tMidiJob *,int);
void func_8C06A39C(tMidiJob *);
void func_8C06A3E0(tMidiJob *);
void func_8C06A40E(tMidiJob *);
int func_8C06A438(tMidiJob *);
int func_8C06A462(tMidiJob *);
void func_8C06A48C(tMidiJob *);
void func_8C06A504(tMidiJob *);
void func_8C06A52C(tMidiJob *,int);
#endif
