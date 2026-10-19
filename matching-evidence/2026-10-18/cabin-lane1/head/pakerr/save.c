/* Private bounded Dreamcast save reconstruction. Later backend/serialization bodies remain external. */
#include "save_types.h"
static signed char gInited=0;
static signed char gGlobalsSet=0;
signed char gSaveGhostFiles=1;

void SaveInit(void)
{
 int i;
 for(i=0;i<4;i++) {
  gJustPlayers[i]=&gRealJustPlayers[i];
  gRealJustPlayers[i].datah=&gJustPlayerData[i];
  gJustPlayerData[i]=&gRealJustPlayerData[i];
  _InitPlayer(&gJustPlayers[i]);
  strcpy((char *)(*gJustPlayers[i]->datah)->name,(char *)gJustPlayers[i]->name);
 }
 InitLL(&gPlayerFiles,1,1); InitLL(&gGhostFiles,1,1);
 gNumLimbo=3;
 gRealLimboGhosts=AllocateMemoryHeap(0,36*sizeof(*gRealLimboGhosts));
 PurgeableMemory(gRealLimboGhosts,0);
 memset(gRealLimboGhosts,0,36*sizeof(*gRealLimboGhosts));
 gLimboGhosts=AllocateMemoryHeap(0,36*sizeof(*gLimboGhosts));
 PurgeableMemory(gLimboGhosts,0);
 for(i=0;i<36;i++)gLimboGhosts[i]=&gRealLimboGhosts[i];
 memset(gSaveWork8C152628,0,44);
 gGlobalsSet=0;gInited=1;
}

void SaveServer(int forceUpdate,int rumbleOnly)
{
 int pak;
 tSaveFile **fileh,**next;
 static signed char sInitCountdown=3;
 static signed char sReadingPak[8]={0,0,0,0,0,0,0,0};
 static signed char sPakChanged[8]={0,0,0,0,0,0,0,0};
 PS_PakServer(forceUpdate,rumbleOnly);
 if(sInitCountdown>1) {sInitCountdown--;return;}
 for(pak=0;pak<8;pak++) {
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
    if(!strncmp("RUSH2049",(*fileh)->name,8) && !strncmp("TRN",(*fileh)->ext,3))SaveReadTraining8C04DC14(fileh);
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
 tSaveFile **fileh;
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

void SaveSelectPlayer(tSaveProfile **player)
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
