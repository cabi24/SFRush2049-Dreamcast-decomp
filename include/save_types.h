#ifndef SAVE_TYPES_H
#define SAVE_TYPES_H
#include "dc_types.h"
typedef struct tSaveFile {
 tDLLE ll; /* +0 next: B856/B944; only +0 used here. */
 char pad08[0x25-0x08];
 char name[8]; /* +25: B85C, eight compared bytes; final terminator capacity unknown. */
 char pad2d;
 char ext[3]; /* +2E: B86E/B95C; TRN compares3, allocation end unknown. */
 char pad31[0x3c-0x31];
 void *data; /* +3C: BA10 */
} tSaveFile; /* Proven end40 from finaldata, not full platform allocation. */
typedef struct tSaveDataDC810 {
 char pad00[0x0d];
 unsigned char name[13]; /* B678 pointsdata+0D for strcpy */
 char pad1a[0x810-0x1a]; /* allocation stride0810: B646/694 and poolB770 */
} tSaveDataDC810;
typedef struct tSaveProfile {
 char pad00[0x14];
 unsigned char name[13]; /* B672 profile+14 */
 char pad21[0x3c-0x21];
 tSaveDataDC810 **datah; /* B664/B66E +3c; DC stride40: i<<6 B64C..658 */
} tSaveProfile;
typedef struct tSaveGhost {
 char pad00[0x30]; /* stride30 B716, alloc36*30=6c0 B772 */
} tSaveGhost;
typedef char check_save_profile[(sizeof(tSaveProfile)==0x40)?1:-1];
typedef char check_save_data[(sizeof(tSaveDataDC810)==0x810)?1:-1];
typedef char check_save_ghost[(sizeof(tSaveGhost)==0x30)?1:-1];
extern tSaveProfile *gJustPlayers[4],gRealJustPlayers[4];
extern tSaveDataDC810 *gJustPlayerData[4],gRealJustPlayerData[4];
extern tSaveGhost *gRealLimboGhosts,**gLimboGhosts;
extern unsigned char gNumLimbo;
extern tDLList gPlayerFiles,gGhostFiles,gPS_Files[8];
extern unsigned char gSaveWork8C152628[44]; /* exact memset size2c B720 */
extern signed char gOptions[40];
void _InitPlayer(tSaveProfile **);
void InitLL(tDLList *,signed char,signed char);
void *AllocateMemoryHeap(int,unsigned int);
void PurgeableMemory(void *,int);
void *memset(void *,int,unsigned int);
/* TargetB666..678 uses the verified SHC R0/R1 helper ABI (mbox lesson). */
#define strcpy _builtin_strcpy
int strncmp(const char *,const char *,unsigned int);
void PS_PakServer(int,int);
int PS_PakChanged(int),PS_PakInserted(int),PS_PakRumble(int),PakErrorActive(int);
void SavePreparePakScan8C06C58C(int); /* target poolB9C4; backend semantics/return left external */
int _VerifyFile(tSaveFile **,int);
int _AlreadyReadPlayer(tSaveFile **),_AlreadyReadGhost(tSaveFile **);
tSaveProfile **_ReadPlayer(tSaveFile **);
tSaveGhost **_ReadGhostHeader(tSaveFile **);
void SaveReadTraining8C04DC14(tSaveFile **); /* targetB964, TRN path; no backend reconstruction */
void PS_FlushFile(tSaveFile **,int);
signed char GetPlayerOption(tSaveProfile **,unsigned char);
void SavePlayerOption(tSaveProfile **,unsigned char,signed char);
void VideoSetOffset(int,int);
#endif

