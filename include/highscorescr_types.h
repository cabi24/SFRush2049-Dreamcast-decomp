#ifndef HIGHSCORESCR_TYPES_H
#define HIGHSCORESCR_TYPES_H
#include "dc_types.h"
/* Profile copy64 at030CF4/D0E; ll+0/+4 storesD16..18,fileh+8D1C,datah+3CD20. */
typedef struct dcHighProfile { tDLLE ll; tPS_File **fileh; char pad0c[0x3c-0xc]; tSaveData **datah; } dcHighProfile;
/* Ghost name+14 at030BD4 and fileh+8 at031114. No backing extent claimed. */
typedef struct dcHighGhost { char pad0[8]; tPS_File **fileh; char pad0c[8]; unsigned char name[1]; } dcHighGhost;
typedef struct anon_x_y_c { signed short x,y,w,h,cx,cy; } anon_x_y_c;
typedef char high_check_profile[(sizeof(dcHighProfile)==64)?1:-1];
extern signed char gSoftReset,gTrackNum,gOptions[],gSaveGhostFile,gSaveGhostFiles,gGhostPak,gBackwards,gCircuitTrack,gCircuitSize;
extern unsigned char gCircuitPlayer;
extern signed short gNumPlayers,gThisTrack;
extern int gRace,gHighScoreState;
extern signed char gDrawName[4],gStuffUnlocked[5],circuitCode;
extern unsigned char gScoreName[4][13],gCircuitCode[32],gBlocksNeeded;
extern dcHighGhost *gGhostToSave,**gSavedGhost;
extern dcHighProfile *gGhostPlayer,gRealGhostPlayer;
extern tPadState gPlayers[4];
extern tCarData gGameCar[6];
extern void *namePanes[4],*scorePanes[3],*ghostPanes[3],*codePane;
extern signed char gPrevUnlockedTracks[4][19],gPrevUnlockedCars[4][13],gPrevUnlockedCircuits[4][4],gPrevUnlockedTrans[4][8],gPrevUnlockedEngines[4][9],gPrevUnlockedTires[4][5],gPrevUnlockedFrames[4][6];
extern signed char gUnlockedTracks[4][19],gUnlockedCars[4][13],gUnlockedCircuits[4][4],gUnlockedTrans[4][8],gUnlockedEngines[4][9],gUnlockedTires[4][5],gUnlockedFrames[4][6];
void _SetHighScoreState(int);
void _RankScores(void);
void ControlUnlocked(void);
void ControlCircuitCode(void);
void ControlName(int);
void ControlGhost(void);
void ControlScores(void);
void RemovePane(void *);
void TB_Strcpy(unsigned char *,unsigned char *);
void UnloadWaveAll(int);
dcHighGhost **CompressGhost(dcHighGhost *,unsigned char *);
int SaveGhost(int,dcHighProfile **,dcHighGhost **,int);
void SaveServer(int,int);
void SaveFlush(int);
void *memcpy(void *,const void *,unsigned int);
int memcmp(const void *,const void *,unsigned int);
void AudKillSong(void);
void AudPlaySong(int,int);
void AudSetMusicVol(float,float);
void RecordGameOver(void);
void UpdateCircuit(unsigned char *,float);
void GetCircuitCode(unsigned char *);
void CalcUnlockedStuff(void);
void ClearRushControls(void);
void PS_LockFile(tPS_File **,int);
void setGameState(unsigned int);
#endif
