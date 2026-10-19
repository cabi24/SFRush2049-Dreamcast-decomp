/* Target-backed Dreamcast MIDI/job wrappers. Anonymous names/bounds preserved. */
#include "dc_sound_types.h"
static int midiWorkBytes8C0C7C2C=0x10000;
static int midiReadBytes8C0C7C30=0x20000;
static void *midiWork8C0C7C34=0;
static void *midiRead1_8C0C7C38=0;
static void *midiRead2_8C0C7C3C=0;
static int commandDelay8C0C7C40=0;
static int songLock8C0C7C44=0;

void func_8C069F78(void)
{
 int i;
 for(i=0;i<20;i++)func_8C06A0DA(i);
 while(midiActive8C177B4C.head || midiPending8C177B5C.head){}
 while(streamActive8C177FD0.head || streamPending8C177FE0.head){}
 DeallocateMemory(midiRead1_8C0C7C38);
 DeallocateMemory(midiRead2_8C0C7C3C);
 sdkFree8C0A1BB4(midiWork8C0C7C34);
}

void func_8C069FD0(int song,int loop)
{
 tMidiJob *p;
 /* Target retains unused range comparisons here; their original assert is unknown. */
 if(song>=18){func_8C06A5D0(song,loop);return;}
 for(p=midiActive8C177B4C.head;p;p=p->next)if(p->song==song && !p->cancel)return;
 for(p=midiPending8C177B5C.head;p;p=p->next) {
  if(p->song==song) {if(p->cancel)p->cancel=0;return;}
 }
 p=func_8C06A320();
 strcpy(p->path,songPaths8C0BA818[song]);
 p->loop=loop;
 p->volume=convertVolume8C016508(songMasterVolume8C17843C);
 p->pan=63;
 p->readBuffer1=midiRead1_8C0C7C38;
 p->readBytes=midiReadBytes8C0C7C30;
 p->workHandle=midiWork8C0C7C34;
 p->workBytes=midiWorkBytes8C0C7C2C;
 p->readBuffer2=midiRead2_8C0C7C3C;
 p->song=song;
 InsertLLE((tDLList *)&midiPending8C177B5C,p,midiPending8C177B5C.head);
}

void func_8C06A0DA(int song)
{
 tMidiJob *p;
 songLock8C0C7C44++;
 if(song>=18)func_8C06A738(song);
 else {
  for(p=midiActive8C177B4C.head;p;p=p->next) {
   if(p->song==song) {
    if(!p->cancel){p->cancel=1;func_8C06A3E0(p);}
    songLock8C0C7C44--;return;
   }
  }
  for(p=midiPending8C177B5C.head;p;p=p->next) {
   if(p->song==song) {if(!p->cancel)p->cancel=1;break;}
  }
 }
 songLock8C0C7C44--;
}

int func_8C06A1A0(void)
{
 if(midiActive8C177B4C.head || midiPending8C177B5C.head || streamActive8C177FD0.head || streamPending8C177FE0.head)return 0;
 return 1;
}

int func_8C06A1CA(void)
{
 if(midiActive8C177B4C.head || midiPending8C177B5C.head)return 1;
 return 0;
}

void func_8C06A1E4(void)
{
 tMidiJob *p,*next;
 if(songLock8C0C7C44)return;
 func_8C06A836();
 for(p=midiActive8C177B4C.head;p;p=next) {
  next=p->next;
  if(!commandDelay8C0C7C40) {
   if(p->cancel){if(!p->stopIssued)func_8C06A40E(p);else func_8C06A3E0(p);return;}
   if(p->loop && func_8C06A438(p))func_8C06A504(p);
  }
  sdkTick8C0A0064(&p->state);p->ticks++;
  if(p->volume!=convertVolume8C016508(songMasterVolume8C17843C))func_8C06A52C(p,convertVolume8C016508(songMasterVolume8C17843C));
 }
 if(commandDelay8C0C7C40){commandDelay8C0C7C40--;return;}
 for(p=midiPending8C177B5C.head;p;p=next) {
  next=p->next;
  if(p->cancel)func_8C06A348(p,0);
  else {
   func_8C06A39C(p);func_8C06A48C(p);
   RemoveLLE((tDLList *)&midiPending8C177B5C,p);
   InsertLLE((tDLList *)&midiActive8C177B4C,p,midiActive8C177B4C.head);
  }
 }
}

tMidiJob *func_8C06A320(void)
{
 tMidiJob *p=midiFree8C177B3C.tail;
 RemoveLLE((tDLList *)&midiFree8C177B3C,p);
 p->cancel=0;p->stopIssued=0;p->ticks=0;return p;
}

void func_8C06A348(tMidiJob *p,int active)
{
 if(active)RemoveLLE((tDLList *)&midiActive8C177B4C,p);
 else RemoveLLE((tDLList *)&midiPending8C177B5C,p);
 InsertLLE((tDLList *)&midiFree8C177B3C,p,midiFree8C177B3C.head);
}

void func_8C06A39C(tMidiJob *p)
{
 sdkPath8C09FCC6(&p->state,p->path);
 sdkBuffers8C09FBB6(&p->state,p->readBuffer1,p->readBytes,p->workHandle,p->workBytes);
 sdkReadBuffer8C09FAF4(&p->state,p->readBuffer2);
}

void func_8C06A3E0(tMidiJob *p)
{
 sdkStop8C09FD6A(&p->state);sdkReset8C09F734(&p->state);sdkClose8C09F9F2(&p->state);
 func_8C06A348(p,1);commandDelay8C0C7C40=0;
}

void func_8C06A40E(tMidiJob *p)
{
 commandDelay8C0C7C40=0;p->stopIssued=1;
 if(func_8C06A462(p))sdkStop8C09FD6A(&p->state);
}

int func_8C06A438(tMidiJob *p)
{
 int result=0;
 if(!sdkDone8C09FA24(&p->state,&result))return 0;
 return result;
}

int func_8C06A462(tMidiJob *p)
{
 int result=0;
 if(!sdkBusy8C09FAC4(&p->state,&result))return 0;
 return result;
}

void func_8C06A48C(tMidiJob *p)
{
 p->state.beginFlag=1;
 sdkStart8C09FB2C(&p->state);audioFlush8C06B1A8();
 sdkVolume8C09F7FC(&p->state,(signed char)p->volume,0);audioFlush8C06B1A8();
 commandDelay8C0C7C40=0;
}

void func_8C06A504(tMidiJob *p)
{
 sdkStop8C09FD6A(&p->state);sdkReset8C09F734(&p->state);sdkClose8C09F9F2(&p->state);
 func_8C06A39C(p);func_8C06A48C(p);
}

void func_8C06A52C(tMidiJob *p,int volume)
{
 sdkVolume8C09F7FC(&p->state,volume,0);audioFlush8C06B1A8();p->volume=volume;
}

tStreamJob *func_8C06A554(void)
{
 tStreamJob *p=streamFree8C177FC0.tail;
 RemoveLLE((tDLList *)&streamFree8C177FC0,p);p->cancel=0;return p;
}

void func_8C06A574(tStreamJob *p,int active)
{
 if(active)RemoveLLE((tDLList *)&streamActive8C177FD0,p);
 else RemoveLLE((tDLList *)&streamPending8C177FE0,p);
 InsertLLE((tDLList *)&streamFree8C177FC0,p,streamFree8C177FC0.head);
}
