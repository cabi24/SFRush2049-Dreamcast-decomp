/* save.c - player and ghost persistence (Dreamcast). Ported from the MAT3 GameCube source with the
   DC layouts and the DC-only training record, tournament-key scrambling and SDK system id. */
#include "save_types.h"
static signed char gInited=0;
static signed char gGlobalsSet=0;
signed char gSaveGhostFiles=1;

/* Static helpers the DC inlines without a standalone copy (not in the function map). */
#pragma inline(_NewPlayer, _FreePlayer, _NewGhost, _FreeGhost, _Pow)
#pragma inline(PurgePlayerData) /* empty; ClearPlayerRecords 8C04CD96 has no call, the standalone copy stays at 8C04D054 */
static tPlayerProfile **_NewPlayer(int pak, unsigned int id)
{
    return (tPlayerProfile **)AllocateHandleHeap(0, sizeof(tPlayerProfile));
}

static void _FreePlayer(tPlayerProfile **player)
{
    DeallocateMemory(player);
}

static int _Pow(int num, int exp)
{
    int result = 1;
    while (exp--) result *= num;
    return result;
}

void SaveInit(void)
{
 int i;
 int numLimbo; /* A variable, not a literal: the target loop compares i with a hoisted register (8C04B6D2 mov #36,r12 / 8C04B712 cmp/ge); the literal form unrolls by three (+20 bytes). */
 for(i=0;i<4;i++) {
  gJustPlayers[i]=&gRealJustPlayers[i];
  gRealJustPlayers[i].datah=&gJustPlayerData[i];
  gJustPlayerData[i]=&gRealJustPlayerData[i];
  _InitPlayer(&gJustPlayers[i]);
  strcpy((char *)(*gJustPlayers[i]->datah)->name,(char *)gJustPlayers[i]->name);
 }
 InitLL(&gPlayerFiles,1,1); InitLL(&gGhostFiles,1,1);
 gNumLimbo=3;
 numLimbo=36;
 gRealLimboGhosts=AllocateMemoryHeap(0,numLimbo*sizeof(*gRealLimboGhosts));
 PurgeableMemory(gRealLimboGhosts,0);
 memset(gRealLimboGhosts,0,numLimbo*sizeof(*gRealLimboGhosts));
 gLimboGhosts=AllocateMemoryHeap(0,numLimbo*sizeof(*gLimboGhosts));
 PurgeableMemory(gLimboGhosts,0);
 for(i=0;i<numLimbo;i++)gLimboGhosts[i]=&gRealLimboGhosts[i];
 memset(&gTrainingRecord,0,sizeof(gTrainingRecord));
 gGlobalsSet=0;gInited=1;
}

void SaveServer(int forceUpdate,int rumbleOnly)
{
 int pak;
 tPS_File **fileh;
 static signed char sInitCountdown=3;
 static signed char sReadingPak[8]={0,0,0,0,0,0,0,0};
 static signed char sPakChanged[8]={0,0,0,0,0,0,0,0};
 PS_PakServer(forceUpdate,rumbleOnly);
 if(sInitCountdown>1) {sInitCountdown--;return;}
 for(pak=0;pak<8;pak++) {
  tPS_File **next;
  if(!sInitCountdown && !sPakChanged[pak]) {
   if(!PS_PakChanged(pak) || !PS_PakInserted(pak) || PS_PakRumble(pak))continue;
  }
  sPakChanged[pak]=1;
  if(sReadingPak[pak])continue;
  sReadingPak[pak]=1;
  SavePreparePakScan8C06C58C(pak);
  for(fileh=gPS_Files[pak].head;fileh;fileh=next) {
   if(!PS_PakInserted(pak)||!_VerifyFile(fileh,pak))break;
   next=(*fileh)->ll.next;
   if(!strncmp("RUSH2049",(*fileh)->name,8) && !strncmp("P",(*fileh)->ext,1) && !_AlreadyReadPlayer(fileh)) {
    _ReadPlayer(fileh);
    if(PakErrorActive(pak) && PakErrorActive(pak)!=10)break;
   }
  }
  if(!fileh) {
   for(fileh=gPS_Files[pak].head;fileh;fileh=next) {
    if(!PS_PakInserted(pak)||!_VerifyFile(fileh,pak))break;
    next=(*fileh)->ll.next;
    if(!strncmp("RUSH2049",(*fileh)->name,8) && strncmp("P",(*fileh)->ext,1) && strncmp("TRN",(*fileh)->ext,3) && !_AlreadyReadGhost(fileh)) {
     _ReadGhostHeader(fileh);
     if(PakErrorActive(pak) && PakErrorActive(pak)!=10)break;
    }
   }
  }
  if(!fileh) {
   for(fileh=gPS_Files[pak].head;fileh;fileh=next) {
    if(!PS_PakInserted(pak)||!_VerifyFile(fileh,pak))break;
    next=(*fileh)->ll.next;
    if(!strncmp("RUSH2049",(*fileh)->name,8) && !strncmp("TRN",(*fileh)->ext,3))_ReadTraining(fileh);
   }
  }
  sReadingPak[pak]=0;
  if(!fileh)sPakChanged[pak]=0;
 }
 sInitCountdown=0;
}

void SaveFlush(int ignoreErrors)
{
 int pak;
 tPS_File **fileh;
 for(pak=0;pak<8;pak++) {
  for(fileh=gPS_Files[pak].head;fileh;fileh=(*fileh)->ll.next) {
   if((*fileh)->data)PS_FlushFile(fileh,ignoreErrors);
  }
 }
}

void SaveGlobals(void)
{
 gGlobalsSet=1;
}

void SaveSelectPlayer(tPlayerProfile **player)
{
 int i;
 if(!gGlobalsSet) {
  gGlobalsSet=1;
  for(i=0;i<20;i++)gOptions[i]=GetPlayerOption(player,i);
  VideoSetOffset(gOptions[18],gOptions[19]);
 } else {
  for(i=0;i<20;i++)SavePlayerOption(player,i,gOptions[i]);
 }
}

void _RemovePlayerFile(tPS_File **fileh)
{
    tPlayerProfile **player;
    tGhostFile **ghost;
    int i;
    for (player = gPlayerFiles.head; player; player = (*player)->ll.next) {
        if ((*player)->fileh == fileh) {
            for (i = 0; i < gNumPlayers; i++) {
                if (gPlayers[i].save == player) gPlayers[i].save = 0;
            }
            RemovePlayerScores(player);
            RemoveLLE(&gPlayerFiles, player);
            _FreePlayer(player);
            return;
        }
    }
    for (ghost = gGhostFiles.head; ghost; ghost = (*ghost)->ll.next) {
        if ((*ghost)->fileh == fileh) {
            RemoveLLE(&gGhostFiles, ghost);
            _FreeGhost(ghost);
            return;
        }
    }
}

void _MovePlayerFile(tPS_File **from, tPS_File **to)
{
    tPlayerProfile **player;
    tGhostFile **ghost;
    for (player = gPlayerFiles.head; player; player = (*player)->ll.next) {
        if ((*player)->fileh == from) {
            (*player)->fileh = to;
            return;
        }
    }
    for (ghost = gGhostFiles.head; ghost; ghost = (*ghost)->ll.next) {
        if ((*ghost)->fileh == from) {
            (*ghost)->fileh = to;
            return;
        }
    }
}

unsigned char GetPlayerRaceType(tPlayerProfile **player)
{
    return (*(*player)->datah)->raceType;
}

void SavePlayerRaceType(tPlayerProfile **player, int raceType)
{
    if ((*(*player)->datah)->raceType != raceType) {
        (*(*player)->datah)->raceType = raceType;
        PS_WriteFile((*player)->fileh, &(*(*player)->datah)->raceType, 1);
    }
}

unsigned char GetPlayerCar(tPlayerProfile **player)
{
    switch ((*(*player)->datah)->raceType) {
    case 6: return (*(*player)->datah)->battleSettings.car;
    case 4: return (*(*player)->datah)->stuntSettings.car;
    case 5: return (*(*player)->datah)->obstacleSettings.car;
    default: return (*(*player)->datah)->raceSettings.car;
    }
}

void SavePlayerCar(tPlayerProfile **player, unsigned char car)
{
    tSaveData *data = *(*player)->datah;
    switch (data->raceType) {
    case 0:
    case 1:
    case 2:
    case 3:
        if (data->raceSettings.car != car) {
            data->raceSettings.car = car;
            data->raceSettings.checksum = _CalcChecksum((char *)&data->raceSettings + 4, sizeof(data->raceSettings) - 4);
            PS_WriteFile((*player)->fileh, &data->raceSettings, sizeof(data->raceSettings));
        }
        break;
    case 6:
        if (data->battleSettings.car != car) {
            data->battleSettings.car = car;
            data->battleSettings.checksum = _CalcChecksum((char *)&data->battleSettings + 4, sizeof(data->battleSettings) - 4);
            PS_WriteFile((*player)->fileh, &data->battleSettings, sizeof(data->battleSettings));
        }
        break;
    case 4:
        if (data->stuntSettings.car != car) {
            data->stuntSettings.car = car;
            data->stuntSettings.checksum = _CalcChecksum((char *)&data->stuntSettings + 4, sizeof(data->stuntSettings) - 4);
            PS_WriteFile((*player)->fileh, &data->stuntSettings, sizeof(data->stuntSettings));
        }
        break;
    case 5:
        if (data->obstacleSettings.car != car) {
            data->obstacleSettings.car = car;
            data->obstacleSettings.checksum = _CalcChecksum((char *)&data->obstacleSettings + 4, sizeof(data->obstacleSettings) - 4);
            PS_WriteFile((*player)->fileh, &data->obstacleSettings, sizeof(data->obstacleSettings));
        }
        break;
    }
}

unsigned char GetPlayerCarView(tPlayerProfile **player)
{
    switch ((*(*player)->datah)->raceType) {
    case 6: return (*(*player)->datah)->battleSettings.carView;
    case 4: return (*(*player)->datah)->stuntSettings.carView;
    case 5: return (*(*player)->datah)->obstacleSettings.carView;
    default: return (*(*player)->datah)->raceSettings.carView;
    }
}

void SavePlayerCarView(tPlayerProfile **player, unsigned char view)
{
    tSaveData *data = *(*player)->datah;
    switch (data->raceType) {
    case 0:
    case 1:
    case 2:
    case 3:
        if (data->raceSettings.carView != view) {
            data->raceSettings.carView = view;
            data->raceSettings.checksum = _CalcChecksum((char *)&data->raceSettings + 4, sizeof(data->raceSettings) - 4);
            PS_WriteFile((*player)->fileh, &data->raceSettings, sizeof(data->raceSettings));
        }
        break;
    case 6:
        if (data->battleSettings.carView != view) {
            data->battleSettings.carView = view;
            data->battleSettings.checksum = _CalcChecksum((char *)&data->battleSettings + 4, sizeof(data->battleSettings) - 4);
            PS_WriteFile((*player)->fileh, &data->battleSettings, sizeof(data->battleSettings));
        }
        break;
    case 4:
        if (data->stuntSettings.carView != view) {
            data->stuntSettings.carView = view;
            data->stuntSettings.checksum = _CalcChecksum((char *)&data->stuntSettings + 4, sizeof(data->stuntSettings) - 4);
            PS_WriteFile((*player)->fileh, &data->stuntSettings, sizeof(data->stuntSettings));
        }
        break;
    case 5:
        if (data->obstacleSettings.carView != view) {
            data->obstacleSettings.carView = view;
            data->obstacleSettings.checksum = _CalcChecksum((char *)&data->obstacleSettings + 4, sizeof(data->obstacleSettings) - 4);
            PS_WriteFile((*player)->fileh, &data->obstacleSettings, sizeof(data->obstacleSettings));
        }
        break;
    }
}

unsigned char GetPlayerCarTrans(tPlayerProfile **player)
{
    switch ((*(*player)->datah)->raceType) {
    case 6: return (*(*player)->datah)->battleSettings.trans;
    case 4: return (*(*player)->datah)->stuntSettings.trans;
    case 5: return (*(*player)->datah)->obstacleSettings.trans;
    default: return (*(*player)->datah)->raceSettings.trans;
    }
}

void SavePlayerCarTrans(tPlayerProfile **player, unsigned char trans)
{
    tSaveData *data = *(*player)->datah;
    switch (data->raceType) {
    case 0:
    case 1:
    case 2:
    case 3:
        if (data->raceSettings.trans != trans) {
            data->raceSettings.trans = trans;
            data->raceSettings.checksum = _CalcChecksum((char *)&data->raceSettings + 4, sizeof(data->raceSettings) - 4);
            PS_WriteFile((*player)->fileh, &data->raceSettings, sizeof(data->raceSettings));
        }
        break;
    case 6:
        if (data->battleSettings.trans != trans) {
            data->battleSettings.trans = trans;
            data->battleSettings.checksum = _CalcChecksum((char *)&data->battleSettings + 4, sizeof(data->battleSettings) - 4);
            PS_WriteFile((*player)->fileh, &data->battleSettings, sizeof(data->battleSettings));
        }
        break;
    case 4:
        if (data->stuntSettings.trans != trans) {
            data->stuntSettings.trans = trans;
            data->stuntSettings.checksum = _CalcChecksum((char *)&data->stuntSettings + 4, sizeof(data->stuntSettings) - 4);
            PS_WriteFile((*player)->fileh, &data->stuntSettings, sizeof(data->stuntSettings));
        }
        break;
    case 5:
        if (data->obstacleSettings.trans != trans) {
            data->obstacleSettings.trans = trans;
            data->obstacleSettings.checksum = _CalcChecksum((char *)&data->obstacleSettings + 4, sizeof(data->obstacleSettings) - 4);
            PS_WriteFile((*player)->fileh, &data->obstacleSettings, sizeof(data->obstacleSettings));
        }
        break;
    }
}

unsigned char GetPlayerTrack(tPlayerProfile **player, int raceType)
{
    switch (raceType) {
    case 6: return (*(*player)->datah)->battleSettings.track;
    case 4: return (*(*player)->datah)->stuntSettings.track;
    case 5: return (*(*player)->datah)->obstacleSettings.track;
    default: return (*(*player)->datah)->raceSettings.track;
    }
}

void SavePlayerTrack(tPlayerProfile **player, int raceType, unsigned char track)
{
    tSaveData *data = *(*player)->datah;
    switch (raceType) {
    case 0:
    case 1:
    case 2:
        if (data->raceSettings.track != track) {
            data->raceSettings.track = track;
            data->raceSettings.checksum = _CalcChecksum((char *)&data->raceSettings + 4, sizeof(data->raceSettings) - 4);
            PS_WriteFile((*player)->fileh, &data->raceSettings, sizeof(data->raceSettings));
        }
        break;
    case 6:
        if (data->battleSettings.track != track) {
            data->battleSettings.track = track;
            data->battleSettings.checksum = _CalcChecksum((char *)&data->battleSettings + 4, sizeof(data->battleSettings) - 4);
            PS_WriteFile((*player)->fileh, &data->battleSettings, sizeof(data->battleSettings));
        }
        break;
    case 4:
        if (data->stuntSettings.track != track) {
            data->stuntSettings.track = track;
            data->stuntSettings.checksum = _CalcChecksum((char *)&data->stuntSettings + 4, sizeof(data->stuntSettings) - 4);
            PS_WriteFile((*player)->fileh, &data->stuntSettings, sizeof(data->stuntSettings));
        }
        break;
    case 5:
        if (data->obstacleSettings.track != track) {
            data->obstacleSettings.track = track;
            data->obstacleSettings.checksum = _CalcChecksum((char *)&data->obstacleSettings + 4, sizeof(data->obstacleSettings) - 4);
            PS_WriteFile((*player)->fileh, &data->obstacleSettings, sizeof(data->obstacleSettings));
        }
        break;
    default: return;
    }
    SavePlayerRaceType(player, raceType);
}

/* DC option numbers are the GameCube ones minus one (twenty global options). The four setting
   getters are static functions laid out after GetPlayerOption (8C04C1C2..C30A) and reached by
   tail-call bra after its epilogue; their unsigned char track parameter makes the callers compute
   gTrackNum+250 (mov.w 0xfa) instead of add #-6. */
static signed char _GetRaceSetting(tPlayerProfile **,unsigned char,unsigned char),_GetBattleSetting(tPlayerProfile **,unsigned char,unsigned char),
    _GetStuntSetting(tPlayerProfile **,unsigned char,unsigned char),_GetObstacleSetting(tPlayerProfile **,unsigned char,unsigned char);
signed char GetPlayerOption(tPlayerProfile **player, unsigned char option)
{
    if (option < 20) return (*(*player)->datah)->options[option];
    /* An else-if chain: SHC merges the two identical race tail calls (8C04C166/C174 both reach C176)
       while re-extending gTrackNum per test; separate if statements keep two copies. */
    if (gTrackNum >= 0 && gTrackNum < 6) return _GetRaceSetting(player, option, gTrackNum);
    else if (gTrackNum >= 19 && gTrackNum < 25) return _GetRaceSetting(player, option, gTrackNum);
    else if (gTrackNum >= 6 && gTrackNum < 14) return _GetBattleSetting(player, option, gTrackNum - 6);
    else if (gTrackNum >= 14 && gTrackNum < 18) return _GetStuntSetting(player, option, gTrackNum - 14);
    else return _GetObstacleSetting(player, option, gTrackNum - 18);
}

static signed char _GetRaceSetting(tPlayerProfile **player, unsigned char option, unsigned char track)
{
    tSaveData *data = *(*player)->datah;
    switch (option) {
    case 20: return data->raceSettings.laps;
    case 21: return data->raceSettings.drones;
    case 22: return data->raceSettings.gravity;
    case 23: return data->raceSettings.difficulty;
    case 25: return data->raceSettings.catchup;
    case 26: return data->raceSettings.backwards;
    case 27: return data->raceSettings.mirror;
    case 28: return data->raceSettings.fog;
    case 29: return data->raceSettings.wind;
    case 30: return data->raceSettings.deaths;
    default: return 0;
    }
}

static signed char _GetBattleSetting(tPlayerProfile **player, unsigned char option, unsigned char track)
{
    tSaveData *data = *(*player)->datah;
    switch (option) {
    case 22: return data->battleSettings.gravity;
    case 27: return data->battleSettings.mirror;
    case 28: return data->battleSettings.fog;
    case 29: return data->battleSettings.wind;
    case 36: return data->battleSettings.mode;
    case 37: return data->battleSettings.points;
    case 38: return data->battleSettings.minutes;
    case 39: return data->battleSettings.neg;
    default: return 0;
    }
}

static signed char _GetStuntSetting(tPlayerProfile **player, unsigned char option, unsigned char track)
{
    tSaveData *data = *(*player)->datah;
    switch (option) {
    case 22: return data->stuntSettings.gravity;
    case 27: return data->stuntSettings.mirror;
    case 28: return data->stuntSettings.fog;
    case 29: return data->stuntSettings.wind;
    case 31: return data->stuntSettings.mode;
    case 32: return data->stuntSettings.flow;
    case 33: return data->stuntSettings.points;
    case 34: return data->stuntSettings.minutes;
    case 35: return data->stuntSettings.deaths;
    default: return 0;
    }
}

static signed char _GetObstacleSetting(tPlayerProfile **player, unsigned char option, unsigned char track)
{
    tSaveData *data = *(*player)->datah;
    switch (option) {
    case 22: return data->obstacleSettings.gravity;
    case 27: return data->obstacleSettings.mirror;
    case 28: return data->obstacleSettings.fog;
    case 29: return data->obstacleSettings.wind;
    default: return 0;
    }
}

void SavePlayerOption(tPlayerProfile **player, unsigned char option, signed char value)
{
    tSaveData *data;
    if ((*player)->datah == 0) return;
    data = *(*player)->datah;
    if (option < 20) {
        if (data->options[option] != value) {
            data->options[option] = value;
            data->optChecksum = _CalcChecksum(data->options, 20);
            PS_WriteFile((*player)->fileh, &data->optChecksum, 24);
        }
    } else if (gTrackNum >= 0 && gTrackNum < 6) _SaveRaceSetting(player, option, value, gTrackNum);
    else if (gTrackNum >= 19 && gTrackNum < 25) _SaveRaceSetting(player, option, value, gTrackNum);
    else if (gTrackNum >= 6 && gTrackNum < 14) _SaveBattleSetting(player, option, value, gTrackNum - 6);
    else if (gTrackNum >= 14 && gTrackNum < 18) _SaveStuntSetting(player, option, value, gTrackNum - 14);
    else _SaveObstacleSetting(player, option, value, gTrackNum - 18);
}

void _SaveRaceSetting(tPlayerProfile **player, unsigned char option, signed char value, unsigned char track)
{
    tSaveData *data = *(*player)->datah;
    signed char *setting;
    switch (option) {
    case 20: setting = &data->raceSettings.laps; break;
    case 21: setting = &data->raceSettings.drones; break;
    case 22: setting = &data->raceSettings.gravity; break;
    case 23: setting = &data->raceSettings.difficulty; break;
    case 25: setting = &data->raceSettings.catchup; break;
    case 26: setting = &data->raceSettings.backwards; break;
    case 27: setting = &data->raceSettings.mirror; break;
    case 28: setting = &data->raceSettings.fog; break;
    case 29: setting = &data->raceSettings.wind; break;
    case 30: setting = &data->raceSettings.deaths; break;
    default: return;
    }
    if (*setting != value) {
        *setting = value;
        data->raceSettings.checksum = _CalcChecksum((char *)&data->raceSettings + 4, sizeof(data->raceSettings) - 4);
        PS_WriteFile((*player)->fileh, &data->raceSettings, sizeof(data->raceSettings));
    }
}

void _SaveBattleSetting(tPlayerProfile **player, unsigned char option, signed char value, unsigned char track)
{
    tSaveData *data = *(*player)->datah;
    signed char *setting;
    switch (option) {
    case 22: setting = &data->battleSettings.gravity; break;
    case 27: setting = &data->battleSettings.mirror; break;
    case 28: setting = &data->battleSettings.fog; break;
    case 29: setting = &data->battleSettings.wind; break;
    case 36: setting = &data->battleSettings.mode; break;
    case 37: setting = &data->battleSettings.points; break;
    case 38: setting = &data->battleSettings.minutes; break;
    case 39: setting = &data->battleSettings.neg; break;
    default: return;
    }
    if (*setting != value) {
        *setting = value;
        data->battleSettings.checksum = _CalcChecksum((char *)&data->battleSettings + 4, sizeof(data->battleSettings) - 4);
        PS_WriteFile((*player)->fileh, &data->battleSettings, sizeof(data->battleSettings));
    }
}

void _SaveStuntSetting(tPlayerProfile **player, unsigned char option, signed char value, unsigned char track)
{
    tSaveData *data = *(*player)->datah;
    signed char *setting;
    switch (option) {
    case 22: setting = &data->stuntSettings.gravity; break;
    case 27: setting = &data->stuntSettings.mirror; break;
    case 28: setting = &data->stuntSettings.fog; break;
    case 29: setting = &data->stuntSettings.wind; break;
    case 31: setting = &data->stuntSettings.mode; break;
    case 32: setting = &data->stuntSettings.flow; break;
    case 33: setting = &data->stuntSettings.points; break;
    case 34: setting = &data->stuntSettings.minutes; break;
    case 35: setting = &data->stuntSettings.deaths; break;
    default: return;
    }
    if (*setting != value) {
        *setting = value;
        data->stuntSettings.checksum = _CalcChecksum((char *)&data->stuntSettings + 4, sizeof(data->stuntSettings) - 4);
        PS_WriteFile((*player)->fileh, &data->stuntSettings, sizeof(data->stuntSettings));
    }
}

void _SaveObstacleSetting(tPlayerProfile **player, unsigned char option, signed char value, unsigned char track)
{
    tSaveData *data = *(*player)->datah;
    signed char *setting;
    switch (option) {
    case 22: setting = &data->obstacleSettings.gravity; break;
    case 27: setting = &data->obstacleSettings.mirror; break;
    case 28: setting = &data->obstacleSettings.fog; break;
    case 29: setting = &data->obstacleSettings.wind; break;
    default: return;
    }
    if (*setting != value) {
        *setting = value;
        data->obstacleSettings.checksum = _CalcChecksum((char *)&data->obstacleSettings + 4, sizeof(data->obstacleSettings) - 4);
        PS_WriteFile((*player)->fileh, &data->obstacleSettings, sizeof(data->obstacleSettings));
    }
}

signed char GetPlayerCarSetting(tPlayerProfile **player, unsigned char car, unsigned char setting)
{
    return (*(*player)->datah)->carSettings[car].settings[setting];
}

void SavePlayerCarSetting(tPlayerProfile **player, unsigned char car, unsigned char setting, signed char value)
{
    tSaveData *data = *(*player)->datah;
    if (data->carSettings[car].settings[setting] != value) {
        data->carSettings[car].settings[setting] = value;
        data->carSettings[car].checksum = _CalcChecksum(data->carSettings[car].settings, 12);
        PS_WriteFile((*player)->fileh, &data->carSettings[car], 16);
    }
}

void SavePlayerControls(tPlayerProfile **player, signed char *controls)
{
    tSaveData *data;
    if ((*player)->datah == 0) return;
    data = *(*player)->datah;
    if (memcmp(data->controls, controls, 12)) {
        memcpy((*player)->controls, controls, 12);
        memcpy(data->controls, controls, 12);
        data->contChecksum = _CalcChecksum(data->controls, 12);
        PS_WriteFile((*player)->fileh, &data->contChecksum, 16);
    }
}

void SavePlayerStats(tPlayerProfile **player, unsigned char track)
{
    tRaceStats *raceStats;
    tBattleStats *battleStats;
    tStuntStats *stuntStats;
    tObstacleStats *obstacleStats;
    tSaveData *data = *(*player)->datah;
    if ((track >= 0 && track < 6) || (track >= 19 && track < 25)) { /* 8C04C796 cmp/pz */
        if (track >= 19) raceStats = &data->raceStats[track - 13];
        else raceStats = &data->raceStats[track];
        raceStats->checksum = _CalcChecksum((char *)raceStats + 4, sizeof(*raceStats) - 4);
        PS_WriteFile((*player)->fileh, raceStats, sizeof(*raceStats));
    } else if (track >= 6 && track < 14) {
        battleStats = &data->battleStats[track - 6];
        battleStats->checksum = _CalcChecksum((char *)battleStats + 4, sizeof(*battleStats) - 4);
        PS_WriteFile((*player)->fileh, battleStats, sizeof(*battleStats));
    } else if (track >= 14 && track < 18) {
        stuntStats = &data->stuntStats[track - 14];
        stuntStats->checksum = _CalcChecksum((char *)stuntStats + 4, sizeof(*stuntStats) - 4);
        PS_WriteFile((*player)->fileh, stuntStats, sizeof(*stuntStats));
    } else {
        obstacleStats = &data->obstacleStats[track - 18];
        obstacleStats->checksum = _CalcChecksum((char *)obstacleStats + 4, sizeof(*obstacleStats) - 4);
        PS_WriteFile((*player)->fileh, obstacleStats, sizeof(*obstacleStats));
    }
}

void SavePlayerCoins(tPlayerProfile **player, unsigned char track)
{
    tRaceStats *raceStats;
    tStuntStats *stuntStats;
    tSaveData *data = *(*player)->datah;
    if ((track >= 0 && track < 6) || (track >= 19 && track < 25)) { /* 8C04C8A6 cmp/pz */
        if (track >= 19) track = (unsigned char)(track - 19);
        raceStats = &data->raceStats[track]; /* the parameter is spilled to the frame (8C04C8CE) and reloaded for the second row */
        raceStats->checksum = _CalcChecksum((char *)raceStats + 4, sizeof(*raceStats) - 4);
        PS_WriteFile((*player)->fileh, raceStats, 1);
        PS_WriteFile((*player)->fileh, &raceStats->coinsFound, 2);
        raceStats = &data->raceStats[(unsigned)(track + 6)]; /* 8C04C904 add #6 then scale: no (a+c)*k fold */
        PS_WriteFile((*player)->fileh, raceStats, 1);
        PS_WriteFile((*player)->fileh, &raceStats->coinsFound, 2);
    } else if (track >= 14 && track < 18) {
        stuntStats = &data->stuntStats[track - 14];
        stuntStats->checksum = _CalcChecksum((char *)stuntStats + 4, sizeof(*stuntStats) - 4);
        PS_WriteFile((*player)->fileh, stuntStats, 1);
        PS_WriteFile((*player)->fileh, &stuntStats->coinsFound, 2);
    }
}

void SavePlayerCircuit(tPlayerProfile **player)
{
    tCircuit *data = &(*(*player)->datah)->circuit;
    data->checksum = _CalcChecksum((char *)data + 4, sizeof(*data) - 4);
    PS_WriteFile((*player)->fileh, data, sizeof(*data));
}

void SavePlayerCircuitStats(tPlayerProfile **player, unsigned char circuit)
{
    tCircuitStats *data = &(*(*player)->datah)->circuitStats[circuit];
    data->checksum = _CalcChecksum((char *)data + 4, sizeof(*data) - 4);
    PS_WriteFile((*player)->fileh, data, sizeof(*data));
}

unsigned int _CalcChecksum(void *start, unsigned int size)
{
    signed char *ptr = start;
    unsigned int sum = 0;
    while (size > 0) { /* 8C04C9F2: cmp/hi against a zero register, not tst */
        switch (*ptr & 7) {
        case 0: sum -= *ptr++; break;
        case 1: sum |= *ptr++; break;
        case 2: sum &= *ptr++; break;
        case 3: sum ^= *ptr++; break;
        case 4: sum *= *ptr++; break;
        case 5: sum /= *ptr++; break;
        default: sum += *ptr++; break;
        }
        size--;
    }
    return sum;
}

void _SetDefaultCarSettings(tPlayerProfile **p, int car)
{
    SavePlayerCarSetting(p, car, 0, gCarHandling[car]);
    SavePlayerCarSetting(p, car, 1, gCarEngines[car]);
    SavePlayerCarSetting(p, car, 2, gCarTires[car]);
    SavePlayerCarSetting(p, car, 3, gCarFrame[car]);
    SavePlayerCarSetting(p, car, 4, gPhysicsWingRows[car]);
    SavePlayerCarSetting(p, car, 5, gCarPaintJob[car]);
    SavePlayerCarSetting(p, car, 6, gTireRims[car]);
    SavePlayerCarSetting(p, car, 7, gCarHorn[car]);
    /* Row 0 of the two-dimensional tables: this form gives the fr2/fr3 order at 8C04CAE4..CAEA and the
       add-then-load of the second row base at 8C04CB00 (the MAT3 pointer cast does not). */
    SavePlayerCarSetting(p, car, 8, (signed char)(100.0f * gFrontTireScale[0][car] - 75.0f));
    SavePlayerCarSetting(p, car, 9, (signed char)(100.0f * gRearTireScale[0][car] - 75.0f));
}

/* DC: the revision carries bit7 when the GD-ROM reports a small 1ST_READ.BIN (8C04CBB4..CBD4). */
void _InitPlayer(tPlayerProfile **player)
{
    tSaveData *data;
    int i;
    int track;
    struct { int size; int pad[2]; } info; /* 12-byte slot above fname: frame 36, size at sp+24 (8C04CBB8..CBCC) */
    char fname[20];
    unsigned char flag;
    int result;
    memset(*player, 0, sizeof(**player) - sizeof((*player)->datah));
    /* The boot file name is assembled byte by byte in this order (8C04CB68..CBA0), so the string
       never appears in the image. */
    fname[0] = '1'; fname[8] = '.'; fname[3] = '_'; fname[6] = 'A'; fname[9] = 'B'; fname[7] = 'D';
    fname[5] = 'E'; fname[10] = 'I'; fname[11] = 'N'; fname[4] = 'R'; fname[1] = 'S'; fname[2] = 'T';
    fname[12] = 0;
    flag = 0;
    data = *(*player)->datah;
    memset(data, 0, sizeof(*data));
    info.size = 0;
    result = GdFsGetFileSize8C0D6BC8(fname, &info.size); /* kept in a local: 8C04CBC0 mov r0,r4 before the test */
    if (result == 0 && info.size != 0 && info.size < 401000) flag = 0x80; /* pool 8C04CC98 = 0x61E68 */
    data->revision = 19 | flag;
    SetDefaultControls(data->controls);
    data->rumble = 1;
    data->contChecksum = _CalcChecksum(data->controls, 12);
    memcpy((*player)->controls, data->controls, 12);
    for (i = 0; i < 13; i++) _SetDefaultCarSettings(player, i);
    for (i = 0; i < 20; i++) SavePlayerOption(player, i, GetDefaultOption(i));
    track = gTrackNum;
    for (gTrackNum = 0; gTrackNum < 19; gTrackNum++) {
        for (i = 20; i < 40; i++) SavePlayerOption(player, i, GetDefaultOption(i));
    }
    gTrackNum = track;
    for (data->raceType = 0; data->raceType < 7; data->raceType++) {
        SavePlayerCar(player, 2);
        SavePlayerCarView(player, 2);
    }
    data->raceType = 0;
}

int ClearPlayerRecords(tPlayerProfile **player)
{
    tSaveData *data;
    if (!ReadPlayerData(player)) return 0;
    data = *(*player)->datah;
    memset(data->raceStats, 0, sizeof(data->raceStats));
    PS_WriteFile((*player)->fileh, data->raceStats, sizeof(data->raceStats));
    memset(data->stuntStats, 0, sizeof(data->stuntStats));
    PS_WriteFile((*player)->fileh, data->stuntStats, sizeof(data->stuntStats));
    memset(data->battleStats, 0, sizeof(data->battleStats));
    PS_WriteFile((*player)->fileh, data->battleStats, sizeof(data->battleStats));
    memset(data->obstacleStats, 0, sizeof(data->obstacleStats));
    PS_WriteFile((*player)->fileh, data->obstacleStats, sizeof(data->obstacleStats));
    memset(data->circuitStats, 0, sizeof(data->circuitStats));
    PS_WriteFile((*player)->fileh, data->circuitStats, sizeof(data->circuitStats));
    memset(&data->circuit, 0, sizeof(data->circuit));
    PS_WriteFile((*player)->fileh, &data->circuit, sizeof(data->circuit));
    if (!PS_FlushFile((*player)->fileh, 0)) {
        PurgePlayerData();
        return 0;
    }
    RemovePlayerScores(player);
    PS_FlushPak((*(*player)->fileh)->pak, 1);
    return 1;
}

int _AlreadyReadPlayer(tPS_File **fileh)
{
    tPlayerProfile **player;
    for (player = (tPlayerProfile **)gPlayerFiles.head; player; player = (tPlayerProfile **)(*player)->ll.next) {
        if ((*player)->fileh == fileh) return 1;
    }
    return 0;
}

tPlayerProfile **_ReadPlayer(tPS_File **fileh)
{
    tPlayerProfile **player;
    tPlayerProfile *p;
    tPlayerProfile **before;
    int i;
    int pak = (*fileh)->pak;
    signed char choice1;
    signed char choice2;
    PS_ReadFile(fileh);
    if (!_VerifyFile(fileh, pak)) return 0;
    if (!(*fileh)->data) return 0;
    player = _NewPlayer((*fileh)->pak, (*fileh)->id); /* the dead id load (8C04CE4A..CE52) survives only with both arguments read through *fileh */
    p = *player;
    p->fileh = fileh;
    (*p->fileh)->RemoveFile = _RemovePlayerFile;
    (*p->fileh)->MoveFile = _MovePlayerFile;
    strcpy(p->filename, (*p->fileh)->filename);
    p->datah = (tSaveData **)(*p->fileh)->data;
    if (_CheckAndRepairPlayer(player, 0)) {
        if (!_CheckAndRepairPlayer(player, 1)) {
            PS_ErrorFunc(pak, 12, p->filename, 0, &choice1, &choice2, 0);
            _FreePlayer(player);
            return 0;
        }
    }
    memcpy(p->name, (*p->datah)->name, sizeof(p->name));
    memcpy(p->id, (*p->datah)->id, sizeof(p->id));
    memcpy(p->controls, (*p->datah)->controls, sizeof(p->controls) + 1);
    if (!gGlobalsSet && !gPlayerFiles.head) {
        for (i = 0; i < 20; i++) gOptions[i] = GetPlayerOption(player, i);
        gOptions[16] = 1;
        gLang = gOptions[15] = GetPlayerOption(player, 15);
        LoadTextWave();
        VideoSetOffset(gOptions[18], gOptions[19]);
    }
    for (before = gPlayerFiles.head; before; before = (*before)->ll.next) {
        if (strcmp((char *)(*before)->name, (char *)p->name) > 0) break;
    }
    InsertLLE(&gPlayerFiles, player, before);
    AddPlayerScores(player);
    if (!strcmp((char *)p->name, "000F1CC5 EJK")) {
        for (i = 0; i < 23; i++) gCheatActive[23] = 1; /* sic: 8C04CF8A..CF96 stores gCheatActive[23] 23 times (constant index, counted down by -speed) */
        gCheatState[16] = 1;
        gCheatState[17] = 1;
        gCheatState[18] = 1;
        gSaveGhostFiles = 1;
    }
    return player;
}

int ReadPlayerData(tPlayerProfile **playerh)
{
    tPlayerProfile *player = *playerh;
    int pak;
    signed char choice1;
    signed char choice2;
    if (player->datah) return 1;
    pak = (*player->fileh)->pak;
    PS_ReadFile(player->fileh);
    if (!_VerifyPlayer(playerh, pak)) return 0;
    if (!(*player->fileh)->data) return 0;
    player->datah = (tSaveData **)(*player->fileh)->data;
    if (_CheckAndRepairPlayer(playerh, 0)) {
        if (!_CheckAndRepairPlayer(playerh, 1)) {
            PS_ErrorFunc((*player->fileh)->pak, 12, player->filename, 0, &choice1, &choice2, 0);
            return 0;
        }
    }
    SavePlayerControls(playerh, player->controls);
    return 1;
}

void PurgePlayerData(void)
{
}

int _CheckAndRepairPlayer(tPlayerProfile **player, int repair)
{
    int result = 0;
    int i;
    int j;
    int oldTracknum;
    tSaveData *data = *(*player)->datah;
    if ((data->revision & 63) != 19) { /* 8C04D0DA and #63: bit7 is the GD-ROM flag */
        if (repair) return 0;
        result = 1;
    }
    if (data->nameChecksum != _CalcChecksum(data->id, (char *)&data->optChecksum - (char *)data - 4)) {
        if (repair) {
            data->nameChecksum = _CalcChecksum(data->id, (char *)&data->optChecksum - (char *)data - 4);
            PS_WriteFile((*player)->fileh, data, 4);
        }
        result = 1;
    }
    if (data->optChecksum != _CalcChecksum(data->options, sizeof(data->options))) {
        if (repair) {
            for (i = 0; i < 20; i++) SavePlayerOption(player, i, gOptions[i]);
        }
        result = 1;
    }
    if (data->contChecksum != _CalcChecksum(data->controls, sizeof(data->controls) + 1)) {
        if (repair) {
            SetDefaultControls((*player)->controls);
            (*player)->rumble = 1;
            memcpy(data->controls, (*player)->controls, sizeof(data->controls) + 1);
            data->contChecksum = _CalcChecksum(data->controls, sizeof(data->controls) + 1);
            PS_WriteFile((*player)->fileh, &data->contChecksum, 16);
        }
        result = 1;
    }
    if (data->raceSettings.checksum != _CalcChecksum((char *)&data->raceSettings + 4, sizeof(data->raceSettings) - 4)) {
        if (repair) {
            oldTracknum = gTrackNum;
            gTrackNum = 0;
            for (i = 20; i < 40; i++) SavePlayerOption(player, i, gOptions[i]);
            gTrackNum = oldTracknum;
            memset(&data->raceSettings, 0, sizeof(data->raceSettings));
            PS_WriteFile((*player)->fileh, &data->raceSettings, sizeof(data->raceSettings));
        }
        result = 1;
    }
    if (data->battleSettings.checksum != _CalcChecksum((char *)&data->battleSettings + 4, sizeof(data->battleSettings) - 4)) {
        if (repair) {
            oldTracknum = gTrackNum;
            gTrackNum = 6;
            for (j = 20; j < 40; j++) SavePlayerOption(player, j, gOptions[j]);
            gTrackNum = oldTracknum;
            memset(&data->battleSettings, 0, sizeof(data->battleSettings));
            PS_WriteFile((*player)->fileh, &data->battleSettings, sizeof(data->battleSettings));
        }
        result = 1;
    }
    if (data->stuntSettings.checksum != _CalcChecksum((char *)&data->stuntSettings + 4, sizeof(data->stuntSettings) - 4)) {
        if (repair) {
            oldTracknum = gTrackNum;
            gTrackNum = 14;
            for (j = 20; j < 40; j++) SavePlayerOption(player, j, gOptions[j]);
            gTrackNum = oldTracknum;
            memset(&data->stuntSettings, 0, sizeof(data->stuntSettings));
            PS_WriteFile((*player)->fileh, &data->stuntSettings, sizeof(data->stuntSettings));
        }
        result = 1;
    }
    if (data->obstacleSettings.checksum != _CalcChecksum((char *)&data->obstacleSettings + 4, sizeof(data->obstacleSettings) - 4)) {
        if (repair) {
            oldTracknum = gTrackNum;
            gTrackNum = 18;
            for (j = 20; j < 40; j++) SavePlayerOption(player, j, gOptions[j]);
            gTrackNum = oldTracknum;
            memset(&data->obstacleSettings, 0, sizeof(data->obstacleSettings));
            PS_WriteFile((*player)->fileh, &data->obstacleSettings, sizeof(data->obstacleSettings));
        }
        result = 1;
    }
    for (i = 0; i < 6; i++) {
        if (data->raceStats[i].checksum != _CalcChecksum(&data->raceStats[i].scores, sizeof(data->raceStats[i]) - 4)) {
            if (repair) {
                memset(&data->raceStats[i], 0, sizeof(data->raceStats[i]));
                PS_WriteFile((*player)->fileh, &data->raceStats[i], sizeof(data->raceStats[i]));
            }
            result = 1;
        }
    }
    for (i = 0; i < 4; i++) {
        if (data->stuntStats[i].checksum != _CalcChecksum(&data->stuntStats[i].bestStunt, sizeof(data->stuntStats[i]) - 4)) {
            if (repair) {
                memset(&data->stuntStats[i], 0, sizeof(data->stuntStats[i]));
                PS_WriteFile((*player)->fileh, &data->stuntStats[i], sizeof(data->stuntStats[i]));
            }
            result = 1;
        }
    }
    for (i = 0; i < 8; i++) {
        if (data->battleStats[i].checksum != _CalcChecksum(&data->battleStats[i].totalMatches, sizeof(data->battleStats[i]) - 4)) {
            if (repair) {
                memset(&data->battleStats[i], 0, sizeof(data->battleStats[i]));
                PS_WriteFile((*player)->fileh, &data->battleStats[i], sizeof(data->battleStats[i]));
            }
            result = 1;
        }
    }
    for (i = 0; i < 1; i++) { /* DC loops over the single obstacle record like the other tables (8C04D638..D698) */
        if (data->obstacleStats[i].checksum != _CalcChecksum(&data->obstacleStats[i].bestTime, sizeof(data->obstacleStats[i]) - 4)) {
            if (repair) {
                memset(&data->obstacleStats[i], 0, sizeof(data->obstacleStats[i]));
                PS_WriteFile((*player)->fileh, &data->obstacleStats[i], sizeof(data->obstacleStats[i]));
            }
            result = 1;
        }
    }
    for (i = 0; i < 4; i++) {
        if (data->circuitStats[i].checksum != _CalcChecksum(&data->circuitStats[i].numCircuits, sizeof(data->circuitStats[i]) - 4)) {
            if (repair) {
                memset(&data->circuitStats[i], 0, sizeof(data->circuitStats[i]));
                PS_WriteFile((*player)->fileh, &data->circuitStats[i], sizeof(data->circuitStats[i]));
            }
            result = 1;
        }
    }
    if (data->circuit.checksum != _CalcChecksum((char *)&data->circuit + 4, sizeof(data->circuit) - 4)) { /* 8C04D7D8..D7DC: base plus 4 */
        if (repair) {
            memset(&data->circuit, 0, sizeof(data->circuit));
            PS_WriteFile((*player)->fileh, &data->circuit, sizeof(data->circuit));
        }
        result = 1;
    }
    for (i = 0; i < 13; i++) {
        if (data->carSettings[i].checksum != _CalcChecksum(&data->carSettings[i].settings, sizeof(data->carSettings[i]) - 4)) {
            if (repair) {
                _SetDefaultCarSettings(player, i);
                PS_WriteFile((*player)->fileh, &data->carSettings[i], sizeof(data->carSettings[i]));
            }
            result = 1;
        }
    }
    return result;
}

/* Map name func_8C04D98C. The DC builds the id from gInitSdkSetting bytes and the frame counter. */
tPlayerProfile **CreatePlayer(int pak, unsigned char *name)
{
    signed short playerPalette[16] = { SAVE_PLAYER_PALETTE };
    char playerPixels[512] = { SAVE_PLAYER_PIXELS };
    tPlayerProfile **player;
    tPlayerProfile **before;
    tPlayerProfile *p;
    tSaveData *data;
    tPS_File **fileh;
    char ext[16]; /* frame 0x234: twelve bytes more than ext[4] */
    _GetUniqueExtension(pak, "RUSH2049", "P", ext);
    fileh = PS_CreateFile(pak, "RUSH2049", ext, sizeof(*data), "RUSH 2049", "RUSH 2049 PLAYER DATA", (char *)name, playerPalette, playerPixels);
    if (!fileh) return 0;
    player = _NewPlayer((*fileh)->pak, (*fileh)->id);
    p = *player;
    p->datah = (tSaveData **)(*fileh)->data;
    data = *p->datah;
    _InitPlayer(player);
    p->fileh = fileh;
    (*fileh)->RemoveFile = _RemovePlayerFile;
    (*fileh)->MoveFile = _MovePlayerFile;
    strcpy(p->filename, (*fileh)->filename);
    data->id[0] = gFrameCounter;
    data->id[0] &= 0xffff;
    data->id[0] |= ((gInitSdkSetting[1] & 0xff) << 24) | ((gInitSdkSetting[5] & 0xff) << 16);
    data->id[1] = ((gInitSdkSetting[4] & 0xff) << 24) | ((gInitSdkSetting[2] & 0xff) << 16) | ((gInitSdkSetting[0] & 0xff) << 8) | (gInitSdkSetting[3] & 0xff);
    strcpy((char *)data->name, (char *)name);
    data->nameChecksum = _CalcChecksum(data->id, (char *)&data->optChecksum - (char *)data - 4);
    memcpy(p->name, data->name, sizeof(p->name));
    memcpy(p->id, data->id, sizeof(p->id));
    PS_WriteFile(p->fileh, data, sizeof(*data));
    if (!PS_FlushFile(p->fileh, 0)) {
        if (_VerifyFile(p->fileh, pak)) PS_DeleteFile(p->fileh);
        _FreePlayer(player);
        return 0;
    }
    PS_FlushPak(pak, 1);
    for (before = gPlayerFiles.head; before; before = (*before)->ll.next) {
        if (strcmp((char *)(*before)->name, (char *)p->name) > 0) break;
    }
    InsertLLE(&gPlayerFiles, player, before);
    return player;
}

void DeletePlayer(tPlayerProfile **player)
{
    if ((*player)->fileh) {
        if (ReadPlayerData(player)) PS_DeleteFile((*player)->fileh);
    } else {
        _InitPlayer(player);
    }
}

/* Map name func_8C04DC14: Dreamcast training ("TRN") record reader. */
void _ReadTraining(tPS_File **fileh)
{
    tSaveTraining *record;
    int pak = (*fileh)->pak;
    PS_ReadFile(fileh);
    if (!_VerifyFile(fileh, pak)) return;
    if (!(*fileh)->data) return;
    record = *(tSaveTraining **)(*fileh)->data;
    if (record->checksum == _CalcChecksum((char *)record + 4, sizeof(*record) - 4)) {
        if (gTrainingRecord.stamp < record->stamp) memcpy(&gTrainingRecord, record, sizeof(*record)); /* parked: target 8C04DC5C is cmp/hi r3,r1; bf, ours cmp/hs r1,r3; bt (same operands) */
    }
    PS_PurgeFile(fileh);
}

/* Map names func_8C04DC82 / func_8C04DC8A: inlined at their callers, standalone copies kept. */
tGhostFile **_NewGhost(void)
{
    return (tGhostFile **)AllocateHandleHeap(0, sizeof(tGhostFile));
}

void _FreeGhost(tGhostFile **ghost)
{
    DeallocateMemory(ghost);
}

int _AlreadyReadGhost(tPS_File **fileh)
{
    tGhostFile **ghost;
    for (ghost = (tGhostFile **)gGhostFiles.head; ghost; ghost = (tGhostFile **)(*ghost)->ll.next) {
        if ((*ghost)->fileh == fileh) return 1;
    }
    return 0;
}

tGhostFile **_ReadGhostHeader(tPS_File **fileh)
{
    tGhostFile **ghosth;
    tGhostFile *ghost;
    tGhostFile **before;
    tSaveHeader *header;
    int pak = (*fileh)->pak;
    signed char choice1;
    signed char choice2;
    PS_ReadFile(fileh);
    if (!_VerifyFile(fileh, pak)) return 0;
    if (!(*fileh)->data) return 0;
    ghosth = _NewGhost();
    ghost = *ghosth;
    memset(ghost, 0, sizeof(*ghost));
    ghost->fileh = fileh;
    (*ghost->fileh)->RemoveFile = _RemovePlayerFile;
    (*ghost->fileh)->MoveFile = _MovePlayerFile;
    ghost->datah = 0;
    ghost->header = (tSaveHeader **)(*fileh)->data;
    header = *ghost->header;
    header->start = 0;
    if (_CheckAndRepairGhost(ghosth, 1, 0)) {
        if (!_CheckAndRepairGhost(ghosth, 1, 1)) {
            PS_ErrorFunc((*fileh)->pak, 12, (*fileh)->filename, 0, &choice1, &choice2, 0);
            PS_PurgeFile(fileh);
            _FreeGhost(ghosth);
            return 0;
        }
    }
    ghost->track = header->track;
    ghost->backward = header->backward;
    memcpy(ghost->name, header->name, sizeof(ghost->name));
    ghost->playerId[0] = header->playerId[0];
    ghost->playerId[1] = header->playerId[1];
    ghost->finish = header->finish;
    for (before = gGhostFiles.head; before; before = (*before)->ll.next) {
        if ((*before)->finish > ghost->finish) break;
    }
    InsertLLE(&gGhostFiles, ghosth, before);
    return ghosth;
}

int _ReadGhostData(tGhostFile **ghosth, int decode, int unused)
{
    tGhostFile *ghost = *ghosth;
    int pak;
    signed char choice1;
    signed char choice2;
    if (ghost->header) {
        if (decode) _DecodeGhost(ghosth);
        return 1;
    }
    pak = (*ghost->fileh)->pak;
    PS_ReadFile(ghost->fileh);
    if (!_VerifyGhost(ghosth, pak)) return 0;
    if (!(*ghost->fileh)->data) return 0;
    ghost->header = (tSaveHeader **)(*ghost->fileh)->data;
    (*ghost->header)->start = 0;
    if (_CheckAndRepairGhost(ghosth, 1, 0)) {
        if (!_CheckAndRepairGhost(ghosth, 1, 1)) {
            PS_ErrorFunc((*ghost->fileh)->pak, 12, (*ghost->fileh)->filename, 0, &choice1, &choice2, 0);
            RemoveLLE(&gGhostFiles, ghosth);
            PS_PurgeFile(ghost->fileh);
            _FreeGhost(ghosth);
            return 0;
        }
    }
    if (decode) _DecodeGhost(ghosth);
    return 1;
}

int ReadGhostData(tGhostFile **ghosth)
{
    return _ReadGhostData(ghosth, 1, 1);
}

void PurgeGhostData(tGhostFile **ghosth)
{
    tGhostFile *ghost = *ghosth;
    if (ghost->datah) {
        DeallocateMemory(ghost->datah);
        ghost->datah = 0;
        (*ghost->header)->start = 0;
    }
}

/* Map name func_8C04DEFC: XOR the ghost data with the tournament key at rand_r strides. */
void _ScrambleGhost(tSaveHeader *header)
{
    unsigned int seed;
    unsigned int *p;
    if (header->tournamentKey != 0) { /* the explicit compare frees r3: the seed load is r1 at 8C04DF12 */
        seed = header->tournamentKey;
        p = (unsigned int *)header->start;
        while ((unsigned char *)p < header->start + header->unzippedSize - 4) {
            *p ^= header->tournamentKey;
            p += rand_r(&seed) % 69;
        }
    }
}

void _DecodeGhost(tGhostFile **ghosth)
{
    int size;
    unsigned int sum;
    tSaveHeader *header = *(*ghosth)->header;
    if (!header->start) {
        (*ghosth)->datah = AllocateHandleHeap(0, header->unzippedSize);
        header->start = *(*ghosth)->datah;
        size = unzipdata((unsigned char *)(header + 1), header->zippedSize, header->start);
        header->size = size / 3;
        if (size != header->unzippedSize) {
        }
        _ScrambleGhost(header);
        sum = _CalcChecksum(header->start, size);
        if (sum != header->unzippedChecksum) {
        }
        header->flag = 1;
    }
}

int _CheckAndRepairGhost(tGhostFile **ghosth, int data, int repair)
{
    int result = 0;
    tSaveHeader *header = *(*ghosth)->header;
    if ((header->revision & 63) != 20) { /* 8C04DFF4 and #63 */
        if (repair) return 0;
        result = 1;
    }
    if (header->headerChecksum != _CalcChecksum((char *)header + 4,
            (unsigned int)&header->zippedChecksum - (unsigned int)header - 4)) {
        if (repair) return 0;
        result = 1;
    }
    if (!data) return result;
    if (header->zippedChecksum != _CalcChecksum((unsigned char *)(header + 1), header->zippedSize)) {
        if (repair) return 0;
        result = 1;
    }
    return result;
}

tGhostFile **CompressGhost(tSaveHeader *header, unsigned char *blocks)
{
    tGhostFile *ghost;
    tGhostFile **ghosth;
    unsigned int maxsize;
    unsigned char *sorted;
    unsigned char *gdata;
    unsigned int num;
    unsigned int dataSize;
    unsigned int unzippedChecksum;
    unsigned int seed;
    unsigned int *p;
    unsigned char *end;
    char fname[20];
    struct { int size; int pad[2]; } info; /* the boot-file check's locals (frame 48 = 16 + 20 + 12); only the name stores survive */
    sorted = header->start;
    maxsize = (int)(gRetraceRate * (ghostRaceTimes[gTrackNum] + 10.0f));
    /* Size term first: the argument temporaries are created right to left, which puts the start
       temporary in the lower spill slot (8C04E0D0/E0D4). */
    memmove(header->size + header->start, header->start + maxsize, header->size);
    memmove(header->size * 2 + header->start, header->start + maxsize * 2, header->size);
    dataSize = header->size * 3;
    unzippedChecksum = _CalcChecksum(sorted, dataSize);
    header->tournamentKey = gTrainingRecord.tournamentKey;
    if (header->tournamentKey) {
        seed = header->tournamentKey;
        end = sorted + dataSize - 4;
        for (p = (unsigned int *)sorted; (unsigned char *)p < end; p += rand_r(&seed) % 69) {
            *p ^= header->tournamentKey;
        }
    }
    gdata = AllocateMemoryHeapHigh(0, dataSize);
    num = zip(sorted, dataSize, gdata, dataSize, 9, 10, 4);
    /* The scrambled boot-file name is assembled here too (8C04E15C..E198) and never used. */
    fname[0] = '1'; fname[8] = '.'; fname[3] = '_'; fname[6] = 'A'; fname[9] = 'B'; fname[7] = 'D';
    fname[5] = 'E'; fname[10] = 'I'; fname[11] = 'N'; fname[4] = 'R'; fname[1] = 'S'; fname[2] = 'T';
    fname[12] = 0;
    DeallocateMemory(header->start);
    header->start = 0;
    ghosth = _NewGhost();
    ghost = *ghosth;
    memset(ghost, 0, sizeof(*ghost));
    ghost->header = (tSaveHeader **)AllocateHandleHeap(0, num + sizeof(*header));
    memcpy(*ghost->header, header, sizeof(*header));
    (*ghost->header)->revision = 20;
    (*ghost->header)->start = 0;
    (*ghost->header)->offset = 0;
    (*ghost->header)->size = 0;
    (*ghost->header)->zippedSize = num;
    (*ghost->header)->unzippedSize = dataSize;
    (*ghost->header)->headerChecksum = _CalcChecksum(&(*ghost->header)->revision, (char *)&(*ghost->header)->zippedChecksum - (char *)*ghost->header - 4);
    (*ghost->header)->zippedChecksum = _CalcChecksum(gdata, num);
    (*ghost->header)->unzippedChecksum = unzippedChecksum;
    memcpy(*ghost->header + 1, gdata, num);
    DeallocateMemory(gdata);
    ghost->track = (*ghost->header)->track;
    ghost->backward = (*ghost->header)->backward;
    memcpy(ghost->name, (*ghost->header)->name, sizeof(ghost->name));
    ghost->playerId[0] = (*ghost->header)->playerId[0];
    ghost->playerId[1] = (*ghost->header)->playerId[1];
    ghost->finish = (*ghost->header)->finish;
    *blocks = PS_GetBlocks(GetPtrSize(ghost->header)); /* byte store 8C04E298 */
    return ghosth;
}

/* Map name func_8C04E2F4. The DC names the file with a generated extension instead of "TRN". */
int SaveGhost(int pak, void *unused, tGhostFile **saveGhosth, int limbo)
{
    /* Target stack layout: strings, icon pixels, then the palette. */
    signed short ghostPalette[16] = { SAVE_GHOST_PALETTE };
    char ghostPixels[512] = { SAVE_GHOST_PIXELS };
    char ext[16];
    char str[32];
    tGhostFile *ghost;
    tGhostFile **before;
    tSaveHeader *header;
    tSaveHeader **oldHeader;
    tPS_File **fileh;
    unsigned int size;
    header = *(*saveGhosth)->header;
    if (limbo) {
        SaveGhostInLimbo(header, header + 1);
        DeallocateMemory((*saveGhosth)->header);
        _FreeGhost(saveGhosth);
        return 1;
    }
    ghost = *saveGhosth; /* read after the limbo block: 8C04E384 reloads *saveGhosth for GetPtrSize */
    size = GetPtrSize(ghost->header);
    GetGhostExtension(pak, header->track, header->backward, ext);
    sprintf(str, "GHOST RACE: TRK %d %s: %01d:%02d.%03d", header->track + 1, header->backward ? "BCK" : "FWD",
        (unsigned int)(header->finish / 60.0f), (unsigned int)header->finish % 60, (unsigned int)(header->finish * 1000.0f) % 1000);
    fileh = PS_CreateFile(pak, "RUSH2049", ext, size, "RUSH 2049", str, (char *)ghost->name, ghostPalette, ghostPixels);
    if (!fileh) return 0;
    memcpy(*(void **)(*fileh)->data, *ghost->header, size);
    oldHeader = ghost->header;
    ghost->header = (tSaveHeader **)(*fileh)->data;
    PS_WriteFile(fileh, *ghost->header, size);
    if (!PS_FlushFile(fileh, 0)) {
        if (_VerifyFile(fileh, pak)) PS_DeleteFile(fileh);
        ghost->fileh = 0;
        ghost->header = oldHeader;
        return 0;
    }
    ghost->fileh = fileh;
    (*ghost->fileh)->RemoveFile = _RemovePlayerFile;
    (*ghost->fileh)->MoveFile = _MovePlayerFile;
    DeallocateMemory(oldHeader);
    PS_FlushPak(pak, 1);
    for (before = gGhostFiles.head; before; before = (*before)->ll.next) {
        if ((*before)->finish > ghost->finish) break;
    }
    InsertLLE(&gGhostFiles, saveGhosth, before);
    return 1;
}

void DeleteGhost(tGhostFile **ghosth)
{
    if ((*ghosth)->fileh) PS_DeleteFile((*ghosth)->fileh);
}

void SaveGhostInLimbo(tSaveHeader *header, void *data)
{
    tGhostFile **limboGhosts;
    tSaveHeader *limbo;
    int i;
    limboGhosts = gLimboGhosts + (header->track + (header->backward ? 6 : 0)) * 3;
    for (i = 0; i < gNumLimbo; i++) {
        if (!limboGhosts[i]->header) continue;
        limbo = *limboGhosts[i]->header;
        if (limbo->track == header->track && limbo->backward == header->backward) {
            if (!(header->finish < limbo->finish)) break;
        }
    }
    i--;
    if (i >= 0) {
        if (limboGhosts[0]->header) DeallocateMemory(limboGhosts[0]->header);
        if (i > 0) memmove(limboGhosts[0], limboGhosts[1], i * sizeof(*limboGhosts[0]));
        limboGhosts[i]->track = header->track;
        limboGhosts[i]->backward = header->backward;
        memcpy(limboGhosts[i]->name, header->name, sizeof(limboGhosts[i]->name));
        memcpy(limboGhosts[i]->playerId, header->playerId, sizeof(limboGhosts[i]->playerId));
        limboGhosts[i]->finish = header->finish;
        limboGhosts[i]->header = (tSaveHeader **)AllocateHandleHeap(0, sizeof(*header) + header->zippedSize);
        memcpy(*limboGhosts[i]->header, header, sizeof(*header));
        memcpy(*limboGhosts[i]->header + 1, data, header->zippedSize);
        (*limboGhosts[i]->header)->start = 0;
        (*limboGhosts[i]->header)->offset = (*limboGhosts[i]->header)->size = 0; /* one dereference for both stores: 8C04E69E..E6A8 */
    }
}

/* DC: only asks whether a unique extension exists for the track (8C04E6BA..E6D6). */
int GhostFileAvailable(tPlayerProfile **player, int pak, int track, int backward)
{
    char ext[4];
    return GetGhostExtension(pak, track, backward, ext);
}

int GetGhostExtension(int pak, int track, int backward, char *ext)
{
    char str[3];
    sprintf(str, "%d%s", track + 1, backward ? "B" : "F");
    return _GetUniqueExtension(pak, "RUSH2049", str, ext);
}

int _GetUniqueExtension(int pak, char *name, char *extPrefix, char *ext)
{
    int i;
    tPS_File **fileh;
    for (i = 0; ; i++) {
        if (i >= _Pow(10, 3 - strlen(extPrefix))) return 0;
        sprintf(ext, "%s%0*d", extPrefix, 3 - strlen(extPrefix), i);
        for (fileh = gPS_Files[pak].head; fileh; fileh = (*fileh)->ll.next) {
            if (!strcmp((*fileh)->name, name) && !strcmp((*fileh)->ext, ext)) break;
        }
        if (!fileh) break;
    }
    return 1;
}

int _VerifyPlayer(tPlayerProfile **playerh, int pak)
{
    tPlayerProfile **h;
    for (h = gPlayerFiles.head; h; h = (*h)->ll.next) {
        if (h == playerh) return _VerifyFile((*h)->fileh, pak);
    }
    return 0;
}

int _VerifyGhost(tGhostFile **ghosth, int pak)
{
    tGhostFile **h;
    for (h = gGhostFiles.head; h; h = (*h)->ll.next) {
        if (h == ghosth) return _VerifyFile((*h)->fileh, pak);
    }
    return 0;
}

int _VerifyFile(tPS_File **fileh, int pak)
{
    tPS_File **file;
    for (file = gPS_Files[pak].head; file; file = (*file)->ll.next) {
        if (file == fileh) return 1;
    }
    return 0;
}
