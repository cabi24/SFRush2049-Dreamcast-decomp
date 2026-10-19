/* waves.c: Dreamcast wave loading, texture reuse, and WAD relocation.
 * MAT3 supplies resource identities; DC platform behavior follows target instructions.
 */
#include "..\include\waves_private.h"

const signed char gWaveModel[315] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
const signed char gWaveReplace[315] = {
    1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void InitWaves(void)
{
 int i;
 while (gRenderInProgress) {}
 ZOID_Reset(); InitBlits();
 for (i=0;i<64;i++) {
  if (gWaveInstance[i].ptr && !gWaveInstance[i].locked) {
   if (gWaveInstance[i].wave>=77 && gWaveInstance[i].wave<233) UnloadWaveInstance(i,0);
   else UnloadWaveInstance(i,1);
  }
 }
 gNumModels=0;
 for (i=0;i<64;i++) if (gWaveInstance[i].ptr) gNumModels=i+1;
 func_8C03FFBC(0);
 InitTextWave();
}
int LoadWave(int waveNum,int loadAgain,int loadBackground,int handle,void *heap)
{
 int idx;
 if (waveNum<0 || waveNum>=315) {}
 if (!loadAgain) {
  if ((idx=_FindWaveInstance(waveNum,handle))>=0) {
   if (gWaveInstance[idx].model) MirrorWaveInstance(idx);
   return idx;
  }
 }
 idx=_NewWaveInstance(waveNum,handle,heap);
 _ReadWave(idx);
 return idx;
}
void ReplaceWave(int curWave,int newWave)
{
 int idx;
 if (newWave<0 || newWave>=315) {}
 if (_FindWaveInstance(newWave,-1)<0) {
  if ((idx=_FindWaveInstance(curWave,-1))>=0) {
   while (gRenderInProgress) {}
   _ReplaceWaveInstance(idx,newWave); _ReadWave(idx);
  }
 }
}
int WaveInstanceReady(int idx)
{
 _ValidateWaveInstance(idx,0,0); return gWaveInstance[idx].ready;
}
void *GetWaveInstanceAddr(int idx)
{
 _ValidateWaveInstance(idx,0,0); return gWaveInstance[idx].ptr;
}
void UnloadWaveAll(int waveNum)
{
 int i,found;
 WaveInstance *winst;
 winst=gWaveInstance; found=0;
 for (i=0;i<64;i++,winst++) {
  if (winst->ptr && winst->wave==waveNum) {
   if (winst->locked) {}
   found=1;
   if (waveNum>=77 && waveNum<233) UnloadWaveInstance(i,0);
   else UnloadWaveInstance(i,1);
  }
 }
 if (!found) {}
}
void UnloadWaveInstance(int idx,int UnloadTextures)
{
 WaveInstance *winst;
 int i;
 winst=&gWaveInstance[idx];
 if (winst->ptr) {
  if (!winst->loaded) {}
  while (gRenderInProgress) {}
  if (winst->model && UnloadTextures) {
   for (i=0;i<gTexDefPtrs[idx].count;i++)
    if (!(gTexDefPtrs[idx].texDefs[i].xflags&0x8000)) func_8C06EAD0(&gTexDefPtrs[idx].texDefs[i]);
  }
  DeallocateMemory(winst->ptr); winst->ptr=0;
  memset(&gRomObjDefPtrs[idx],0,sizeof(gRomObjDefPtrs[idx]));
  memset(&gTexDefPtrs[idx],0,sizeof(gTexDefPtrs[idx]));
  memset(&gPalDefPtrs[idx],0,sizeof(gPalDefPtrs[idx]));
 }
}
int _NewWaveInstance(int waveNum,int handle,void *heap)
{
 WaveInstance *winst;
 int idx;
 for (idx=0;idx<64;idx++) {
  if (!gWaveInstance[idx].ptr) {
   winst=&gWaveInstance[idx];
   winst->loaded=0; winst->ready=0; winst->locked=0; winst->mirrored=0;
   winst->wave=waveNum; winst->handle=handle;
   if (handle) winst->ptr=AllocateHandleHeap(heap,(((gWaveSizes[waveNum]+2047)/2048*2048)+31)&~31);
   else winst->ptr=AllocateMemoryHeap(heap,(((gWaveSizes[waveNum]+2047)/2048*2048)+31)&~31);
   winst->size=GetPtrSize(winst->ptr);
   _SetWaveInstanceModel(idx); return idx;
  }
 }
 /* Target 8C065536 falls through to the epilogue without assigning r0. */
}
void _ReplaceWaveInstance(int idx,int newWave)
{
 WaveInstance *winst;
 int i;
 winst=&gWaveInstance[idx];
 _ValidateWaveInstance(idx,1,1);
 if (((gWaveSizes[newWave]+31)&~31)>winst->size) {}
 if (winst->model && winst->wave<77 && winst->wave>=233) {
  for (i=0;i<gTexDefPtrs[idx].count;i++)
   if (!(gTexDefPtrs[idx].texDefs[i].xflags&0x8000)) func_8C06EAD0(&gTexDefPtrs[idx].texDefs[i]);
 }
 winst->loaded=0; winst->ready=0; winst->wave=newWave; winst->mirrored=0;
 _SetWaveInstanceModel(idx);
}
void _SetWaveInstanceModel(int idx)
{
 if ((gWaveInstance[idx].model=gWaveModel[gWaveInstance[idx].wave]) && idx>=gNumModels) gNumModels=idx+1;
}
void LockWaveInstance(int idx)
{
 WaveInstance *winst;
 winst=&gWaveInstance[idx]; _ValidateWaveInstance(idx,0,0); PurgeableMemory(winst->ptr,0); winst->locked=1;
}
void UnlockWaveInstance(int idx)
{
 WaveInstance *winst;
 winst=&gWaveInstance[idx]; _ValidateWaveInstance(idx,0,0); PurgeableMemory(winst->ptr,1); winst->locked=0;
}
int ModelActive(int idx)
{
 WaveInstance *winst;
 if (idx<0 || idx>=64) {}
 winst=&gWaveInstance[idx]; if (!winst->ptr || !winst->ready) return 0; return winst->model;
}
int FindWaveInstance(int waveNum)
{
 return _FindWaveInstance(waveNum,-1);
}
int _FindWaveInstance(int waveNum,signed char handle)
{
 int i;
 for (i=0;i<64;i++) {
  if (gWaveInstance[i].ptr && gWaveInstance[i].wave==waveNum)
   if (handle<0 || gWaveInstance[i].handle==handle) return i;
 }
 return -1;
}
void _ValidateWaveInstance(int idx,int loaded,int ready)
{
 WaveInstance *winst;
 if (idx<0 || idx>=64) {}
 winst=&gWaveInstance[idx];
 if (!winst->ptr) {}
 if (loaded && !winst->loaded) {}
 if (ready && !winst->ready) {}
}
void _ReadWave(int idx)
{
 unsigned int size;
 WaveInstance *winst;
 int waveNum;
 void *compressed;
 winst=&gWaveInstance[idx]; waveNum=winst->wave;
 if (!gWaveCompressed[waveNum]) {
  size=winst->size;
  func_8C06BA3A(gWaveFilenames[waveNum],winst->handle ? *(void **)winst->ptr : winst->ptr,&size,1,0);
 } else {
  compressed=func_8C06BA3A(gWaveFilenames[waveNum],0,&size,gWaveSizes[waveNum]<800000,1);
  size=func_8C0102A4(compressed,winst->handle ? *(void **)winst->ptr : winst->ptr);
  DeallocateMemory(compressed);
 }
 if (!gWaveReplace[waveNum]) ReallocateMemory(winst->ptr,size);
 winst->loaded=1;
 if (gWaveInstance[idx].wave>=77 && gWaveInstance[idx].wave<233) func_8C06588C(idx,1,0);
 else func_8C06588C(idx,1,1);
}
/* Empty target entries are retained by address; no MAT3 identity is forced. */
void func_8C06585E(void) {}
void func_8C065862(int idx)
{
 if (gWaveInstance[idx].wave>=77 && gWaveInstance[idx].wave<233) func_8C06588C(idx,0,0);
 else func_8C06588C(idx,0,1);
}
void func_8C06588C(int idx,int first,int loadTextures)
{
 WaveWad wad;
 unsigned int *WADptr;
 int *TXLDptr,TXLDcnt;
 unsigned int IMAGptr;
 int offset;
 int i,j,reused,modelIdx;
 if (!gWaveInstance[idx].model) { gWaveInstance[idx].ready=1; return; }
 if (!first && !gWaveInstance[idx].handle) return;
 if (gWaveInstance[idx].handle) WADptr=*(unsigned int **)gWaveInstance[idx].ptr;
 else WADptr=(unsigned int *)gWaveInstance[idx].ptr;
 modelIdx=idx;
 if (first) MBSetupWad(&wad,WADptr);
 else {
  offset=(int)*(unsigned int **)gWaveInstance[idx].ptr-(int)gWaveInstance[idx].lastPtr;
  MBOffsetWad(&wad,WADptr,offset);
 }
 gWaveInstance[idx].lastPtr=*(unsigned char **)gWaveInstance[idx].ptr;
 gRomObjDefPtrs[modelIdx].romObjDefs=MBGetFromWad(&wad,0x4F424844,&gRomObjDefPtrs[modelIdx].count);
 gTexDefPtrs[modelIdx].texDefs=MBGetFromWad(&wad,0x54584844,&gTexDefPtrs[modelIdx].count);
 gPalDefPtrs[modelIdx].palDefs=MBGetFromWad(&wad,0x504C4844,&gPalDefPtrs[modelIdx].count);
 if (gWaveInstance[idx].wave>=233 && gWaveInstance[idx].wave<252)
  gTargetPaths.paths=MBGetFromWad(&wad,0x50544844,&gTargetPaths.count);
 TXLDptr=MBGetFromWad(&wad,0x54584C44,&TXLDcnt);
 IMAGptr=(unsigned int)MBGetFromWad(&wad,0x494D4147,0);
 for (i=0;i<gTexDefPtrs[modelIdx].count;i++) {
  gTexDefPtrs[modelIdx].texDefs[i].data+=IMAGptr;
  if (first) {
   reused=0;
   if (gTexDefPtrs[modelIdx].texDefs[i].xflags&0x40000) {
    for (j=0;j<i;j++) {
     if (gTexDefPtrs[modelIdx].texDefs[i].data==gTexDefPtrs[modelIdx].texDefs[j].data) {
      gTexDefPtrs[modelIdx].texDefs[i].cached2C=gTexDefPtrs[modelIdx].texDefs[j].cached2C;
      if (!(gTexDefPtrs[modelIdx].texDefs[i].xflags&0x20000))
       gTexDefPtrs[modelIdx].texDefs[i].cached28=gTexDefPtrs[modelIdx].texDefs[j].cached28;
      gTexDefPtrs[modelIdx].texDefs[i].xflags|=0x8000;
      reused=1; break;
     }
    }
   }
   if (!reused && loadTextures) func_8C06E95C(&gTexDefPtrs[modelIdx].texDefs[i]);
  }
 }
 for (i=0;i<gPalDefPtrs[modelIdx].count;i++) gPalDefPtrs[modelIdx].palDefs[i].data+=IMAGptr;
 if (first) {
  for (i=0;i<gRomObjDefPtrs[modelIdx].count;i++) {
   for (j=0;j<gRomObjDefPtrs[modelIdx].romObjDefs[i].lodCount;j++) {
    gRomObjDefPtrs[modelIdx].romObjDefs[i].lod[j].displayList=(unsigned int *)((int)gRomObjDefPtrs[modelIdx].romObjDefs[i].lod[j].displayList+(first ? (int)WADptr : offset));
    func_8C026104(modelIdx,gRomObjDefPtrs[modelIdx].romObjDefs[i].lod[j].flags&16,gRomObjDefPtrs[modelIdx].romObjDefs[i].lod[j].displayList);
   }
  }
 }
 if (gWaveInstance[idx].wave>=233 && gWaveInstance[idx].wave<252) {
  for (i=0;i<gTargetPaths.count;i++) {
   gTargetPaths.paths[i].points+=(first ? (int)WADptr : offset);
   gTargetPaths.paths[i].trigger+=(first ? (int)WADptr : offset);
  }
 }
 gWaveInstance[idx].ready=1; MirrorWaveInstance(idx);
}
void func_8C065C1A(void) {}
void MirrorWaveInstance(int idx)
{
 _ValidateWaveInstance(idx,1,1);
 if (!gWaveInstance[idx].model || gWaveInstance[idx].mirrored==(gMirrorWaves!=0)) return;
 gWaveInstance[idx].mirrored=(gMirrorWaves!=0);
}
