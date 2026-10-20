#ifndef SAVE_TYPES_H
#define SAVE_TYPES_H
#include "dc_types.h"
#include "save_icons.h"
/* Pak file record (paksysdc). DC packs the names tighter than the GameCube: filename+18 (13),
   name+25 (9), ext+2e (5); _GetUniqueExtension 8C04E792/E7A0, SaveServer 8C04B85C/B86E. */
struct tPS_File {
    tDLLE ll; /* +0 next: B856/B944. */
    void (*RemoveFile)(tPS_File **); /* +8: _ReadPlayer 8C04CE66. */
    void (*MoveFile)(tPS_File **,tPS_File **); /* +c: 8C04CE6E. */
    unsigned char pak; /* +10: _ReadPlayer 8C04CE26 mov#16. */
    char pad11[3];
    unsigned int id; /* +14: CreatePlayer 8C04DA32 mov#20. */
    char filename[13]; /* +18: strcpy source 8C04CE76 add#24. */
    char name[9]; /* +25: 8C04E792 add#37. */
    char ext[5]; /* +2e: 8C04E7A0 add#46. */
    char pad33[1];
    unsigned int size; /* +34 */
    unsigned int blocks; /* +38 */
    void *data; /* +3c: BA10, _ReadPlayer 8C04CE84. */
};
typedef char check_ps_file[(sizeof(tPS_File)==0x40)?1:-1];
/* Ghost file header (same layout as MAT3): CompressGhost 8C04E1D4..E200, _DecodeGhost 8C04DF66..DFCA. */
typedef struct tSaveHeader {
    unsigned int headerChecksum;
    unsigned char revision; /* +4: _CheckAndRepairGhost 8C04DFF0. */
    signed char flag; /* +5: _DecodeGhost 8C04DFCA. */
    signed char track; /* +6 */
    unsigned char backward; /* +7 */
    unsigned char car,trans;
    signed char settings[10];
    unsigned char name[13]; /* +14: _ReadGhostHeader 8C04DD78 add#20. */
    char pad21[1];
    unsigned short tournamentKey; /* +22: CompressGhost 8C04E0F8 mov#34. */
    unsigned int playerId[2]; /* +24/+28: 8C04DD86/DD8A. */
    unsigned int id;
    float retraceTime;
    float finish; /* +34: 8C04DD84 mov#52. */
    unsigned int zippedSize; /* +38: 8C04DF9A. */
    unsigned int unzippedSize; /* +3c: 8C04DF7E. */
    unsigned int zippedChecksum; /* +40: 8C04E004 add#64. */
    unsigned int unzippedChecksum; /* +44: 8C04DFC4 mov#68. */
    unsigned char *start; /* +48: mov#72. */
    unsigned int offset; /* +4c: mov#76. */
    unsigned int size; /* +50: mov#80. */
} tSaveHeader; /* 0x54: header+1 at add#84. */
typedef struct tGhostFile {
    tDLLE ll;
    tPS_File **fileh; /* +8: _MovePlayerFile 8C04BB9A. */
    signed char track; /* +c: 8C04DD76. */
    unsigned char backward; /* +d: 8C04DD7E. */
    unsigned char name[13]; /* +e: 8C04DD82 add#14. */
    char pad1b[1];
    unsigned int playerId[2]; /* +1c/+20: 8C04DD88/DD8C. */
    float finish; /* +24: 8C04DD92 mov#36. */
    unsigned char **datah; /* +28: PurgeGhostData 8C04DEDE. */
    tSaveHeader **header; /* +2c: 8C04DD1C. */
} tGhostFile; /* stride30: SaveGhostInLimbo memmove 8C04E61C..E62A (i*48), _NewGhost 8C04DC84. */
/* Dreamcast training record (TRN files): _ReadTraining 8C04DC42..DC68, CompressGhost 8C04E0F4. */
typedef struct tSaveTraining {
    unsigned int checksum; /* +0: checksum over the following40 bytes. */
    unsigned char body[0x22];
    unsigned short tournamentKey; /* +26: copied into the ghost header. */
    unsigned int stamp; /* +28: newer records replace the kept copy (cmp/hi). */
} tSaveTraining; /* 0x2c: SaveInit memset 8C04B720, _ReadTraining memcpy 8C04DC62. */
typedef char check_save_profile[(sizeof(tPlayerProfile)==0x40)?1:-1];
typedef char check_save_data[(sizeof(tSaveData)==0x810)?1:-1];
typedef char check_save_ghost[(sizeof(tGhostFile)==0x30)?1:-1];
typedef char check_save_header[(sizeof(tSaveHeader)==0x54)?1:-1];
typedef char check_save_training[(sizeof(tSaveTraining)==0x2c)?1:-1];
typedef char check_save_settings[(sizeof(tRaceSettings)==0x14 && sizeof(tBattleSettings)==0x10 &&
    sizeof(tStuntSettings)==0x14 && sizeof(tObstacleSettings)==0xc && sizeof(tCarSettings)==0x10)?1:-1];

extern tPlayerProfile *gJustPlayers[4],gRealJustPlayers[4];
extern tSaveData *gJustPlayerData[4],gRealJustPlayerData[4];
extern tGhostFile *gRealLimboGhosts,**gLimboGhosts;
extern unsigned char gNumLimbo;
extern tDLList gPlayerFiles,gGhostFiles,gPS_Files[8];
extern tSaveTraining gTrainingRecord; /* 8C152628 */
extern signed char gOptions[40];
extern signed char gTrackNum;
extern signed short gNumPlayers,gLang;
#define gPlayers ((tPadState *)0x8C006000) /* Fixed DC arena: _RemovePlayerFile pool 8C04BC44. */
extern unsigned int gFrameCounter;
extern unsigned char gInitSdkSetting[]; /* SDK system id bytes: CreatePlayer 8C04DA7C..DAAE. */
extern signed char gCheatActive[];
extern signed short gCheatState[];
extern signed char gCarHandling[],gCarEngines[],gCarTires[],gCarFrame[],gPhysicsWingRows[],gCarPaintJob[],gTireRims[],gCarHorn[];
extern float gFrontTireScale[5][13],gRearTireScale[5][13]; /* MAT3 cars.c shape; row 0 at 8C0BBE64/8C0BBF68 */
extern float gRetraceRate;
extern float ghostRaceTimes[];

void InitLL(tDLList *,signed char,signed char);
void InsertLLE(tDLList *,void *,void *);
void RemoveLLE(tDLList *,void *);
void *AllocateMemoryHeap(int,unsigned int);
void *AllocateMemoryHeapHigh(int,unsigned int);
void *AllocateHandleHeap(int,unsigned int);
void DeallocateMemory(void *);
unsigned int GetPtrSize(void *);
void PurgeableMemory(void *,int);
void *memset(void *,int,unsigned int);
void *memcpy(void *,const void *,unsigned int);
void *memmove(void *,const void *,unsigned int);
int memcmp(const void *,const void *,unsigned int);
char *strcpy(char *,const char *);
int strcmp(const char *,const char *);
unsigned int strlen(const char *);
int sprintf(char *,const char *,...);
/* TargetB666..678 and 8C04CE78 use the verified SHC R0/R1 helper ABI (mbox lesson). */
#define strcpy _builtin_strcpy
#define strcmp _builtin_strcmp
int strncmp(const char *,const char *,unsigned int);
int rand_r(unsigned int *);
int unzipdata(unsigned char *,unsigned int,unsigned char *);
int zip(unsigned char *,unsigned int,unsigned char *,unsigned int,int,int,int);
void PS_PakServer(int,int);
int PS_PakChanged(int),PS_PakInserted(int),PS_PakRumble(int),PakErrorActive(int);
void SavePreparePakScan8C06C58C(int); /* target poolB9C4; backend semantics/return left external */
void PS_ReadFile(tPS_File **);
void PS_WriteFile(tPS_File **,void *,unsigned int);
int PS_FlushFile(tPS_File **,int);
void PS_FlushPak(int,int);
void PS_PurgeFile(tPS_File **);
void PS_DeleteFile(tPS_File **);
tPS_File **PS_CreateFile(int,const char *,const char *,unsigned int,const char *,const char *,const char *,signed short *,char *);
void PS_ErrorFunc(int,int,char *,int,signed char *,signed char *,int);
unsigned int PS_GetBlocks(unsigned int);
int GdFsGetFileSize8C0D6BC8(const char *,int *); /* _InitPlayer 8C04CBB4: GD-ROM size of 1ST_READ.BIN. */
void SetDefaultControls(signed char *);
signed char GetDefaultOption(int);
void LoadTextWave(void);
void VideoSetOffset(int,int);
void AddPlayerScores(tPlayerProfile **);
void RemovePlayerScores(tPlayerProfile **);

void SaveInit(void);
void SaveServer(int,int);
void SaveFlush(int);
void SaveGlobals(void);
void SaveSelectPlayer(tPlayerProfile **);
void _RemovePlayerFile(tPS_File **);
void _MovePlayerFile(tPS_File **,tPS_File **);
unsigned char GetPlayerRaceType(tPlayerProfile **);
void SavePlayerRaceType(tPlayerProfile **,int);
unsigned char GetPlayerCar(tPlayerProfile **);
void SavePlayerCar(tPlayerProfile **,unsigned char);
unsigned char GetPlayerCarView(tPlayerProfile **);
void SavePlayerCarView(tPlayerProfile **,unsigned char);
unsigned char GetPlayerCarTrans(tPlayerProfile **);
void SavePlayerCarTrans(tPlayerProfile **,unsigned char);
unsigned char GetPlayerTrack(tPlayerProfile **,int);
void SavePlayerTrack(tPlayerProfile **,int,unsigned char);
signed char GetPlayerOption(tPlayerProfile **,unsigned char);
void SavePlayerOption(tPlayerProfile **,unsigned char,signed char);
void _SaveRaceSetting(tPlayerProfile **,unsigned char,signed char,unsigned char);
void _SaveBattleSetting(tPlayerProfile **,unsigned char,signed char,unsigned char);
void _SaveStuntSetting(tPlayerProfile **,unsigned char,signed char,unsigned char);
void _SaveObstacleSetting(tPlayerProfile **,unsigned char,signed char,unsigned char);
signed char GetPlayerCarSetting(tPlayerProfile **,unsigned char,unsigned char);
void SavePlayerCarSetting(tPlayerProfile **,unsigned char,unsigned char,signed char);
void SavePlayerControls(tPlayerProfile **,signed char *);
void SavePlayerStats(tPlayerProfile **,unsigned char);
void SavePlayerCoins(tPlayerProfile **,unsigned char);
void SavePlayerCircuit(tPlayerProfile **);
void SavePlayerCircuitStats(tPlayerProfile **,unsigned char);
unsigned int _CalcChecksum(void *,unsigned int);
void _SetDefaultCarSettings(tPlayerProfile **,int);
void _InitPlayer(tPlayerProfile **);
int ClearPlayerRecords(tPlayerProfile **);
int _AlreadyReadPlayer(tPS_File **);
tPlayerProfile **_ReadPlayer(tPS_File **);
int ReadPlayerData(tPlayerProfile **);
void PurgePlayerData(void);
int _CheckAndRepairPlayer(tPlayerProfile **,int);
tPlayerProfile **CreatePlayer(int,unsigned char *);
void DeletePlayer(tPlayerProfile **);
void _ReadTraining(tPS_File **);
tGhostFile **_NewGhost(void);
void _FreeGhost(tGhostFile **);
int _AlreadyReadGhost(tPS_File **);
tGhostFile **_ReadGhostHeader(tPS_File **);
int _ReadGhostData(tGhostFile **,int,int);
int ReadGhostData(tGhostFile **);
void PurgeGhostData(tGhostFile **);
void _ScrambleGhost(tSaveHeader *);
void _DecodeGhost(tGhostFile **);
int _CheckAndRepairGhost(tGhostFile **,int,int);
tGhostFile **CompressGhost(tSaveHeader *,unsigned char *);
int SaveGhost(int,void *,tGhostFile **,int);
void DeleteGhost(tGhostFile **);
void SaveGhostInLimbo(tSaveHeader *,void *);
int GhostFileAvailable(tPlayerProfile **,int,int,int);
int GetGhostExtension(int,int,int,char *);
int _GetUniqueExtension(int,char *,char *,char *);
int _VerifyPlayer(tPlayerProfile **,int);
int _VerifyGhost(tGhostFile **,int);
int _VerifyFile(tPS_File **,int);
#endif
