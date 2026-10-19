#include "memsys_types.h"
static signed char sInited=0;
static int gMemOldMask;
tHeap *gMainHeap;
#pragma inline(_ConvertHeap,_GetSemaphore,_ReleaseSemaphore)
static tHeap *_ConvertHeap(void *h) {if(!h)return gMainHeap;return h;}
static void _GetSemaphore(void) {gMemOldMask=SetInterruptMask(15);}
static void _ReleaseSemaphore(void) {SetInterruptMask(gMemOldMask);}
void _InitHeap(tHeap *heap, unsigned short numInitialMasters, unsigned short numAdditionalMasters);
unsigned int _LargestAvailableMemoryChunk(tHeap *heap);
void * _AllocateMemory(tHeap *heap, unsigned int size, void * master, int internal);
void _ReallocateMemory(void * ptr, unsigned int newSize, int internal);
void _DeallocateMemory(void * ptr, int internal);
unsigned int _GetPtrSize(void * ptr, int internal);
tHeap *_FindHeap(void * ptr);
tMemoryChunk *_FindChunk(tHeap *heap, void * ptr, int internal);
void func_8C04002A(tHeap *);
unsigned int func_8C0401E0(tHeap *);
void _FillFreeMem(void *,unsigned int);

void InitMemory(unsigned short numInitialMasters, unsigned short numAdditionalMasters)
{
 unsigned int avail, heap_size;
 tHeap *heap;
 if (!sInited) sInited=1;
 sdk_heap_bounds(&heap_size,&avail);
 heap=sdk_heap_alloc(avail);
 gMainHeap=heap;
 heap->endRam=(unsigned char *)(((unsigned int)heap+avail)&~31);
 _InitHeap(*(tHeap *volatile *)&gMainHeap,numInitialMasters,numAdditionalMasters);
}
void func_8C03FD54(void *heap, int (*emergency)(unsigned int))
{
 _GetSemaphore();
 _ConvertHeap(heap)->emergencyFunc=emergency;
 _ReleaseSemaphore();
}
void *func_8C03FD8A(void *h,unsigned int size,unsigned short initial,unsigned short additional)
{
 tHeap *heap,*p;
 _GetSemaphore();
 heap=_AllocateMemory(_ConvertHeap(h),size+64,0,1);
 heap->endRam=(unsigned char *)heap+((size+95)&~31);
 _InitHeap(heap,initial,additional);
 for(p=gMainHeap;p;p=p->next) if(!p->next) {p->next=heap;break;}
 _ReleaseSemaphore();
 return heap;
}

void _InitHeap(tHeap *heap, unsigned short numInitialMasters, unsigned short numAdditionalMasters)
{
    heap->magic = 0xfedcba98;
    heap->next = 0;
    heap->chunks = (tMemoryChunk *)((unsigned char *)heap + 64);
    heap->chunks->magic = 0xfedcba98;
    heap->chunks->next = 0;
    heap->chunks->prev = 0;
    heap->chunks->size = heap->endRam - (unsigned char *)heap->chunks - 32;
    heap->chunks->master = 0;
    heap->chunks->used = 0;
    heap->chunks->internal = 0;
    heap->chunks->locked = 0;
    heap->lastChunk = heap->chunks;
    heap->masters.numMasters = numInitialMasters;
    heap->numAdditionalMasters = numAdditionalMasters;
    heap->masters.nextBlock = 0;
    if (numInitialMasters)
    {
        heap->masters.masters = _AllocateMemory(heap,numInitialMasters*4,0,1);
        memset(heap->masters.masters,0,numInitialMasters*4);
    }
    else heap->masters.masters = 0;
}

int func_8C03FE8E(void *h,unsigned int size)
{
 tHeap *heap;
 unsigned int oldSize;
 unsigned char *end;

 _GetSemaphore();heap=_ConvertHeap(h);
 if(heap->lastChunk->used) {_ReleaseSemaphore();return 0;}
 size=(size+31)&~31;
 oldSize=heap->endRam-(unsigned char *)heap-64;
 end=(unsigned char *)heap+((size+95)&~31);
 if(size<oldSize) {
  if(heap->lastChunk->size<oldSize-size) {_ReleaseSemaphore();return 0;}
  heap->lastChunk->size-=oldSize-size;
  heap->endRam=end;
  _ReallocateMemory(heap,size+64,1);
 } else if(size>oldSize) {
  _ReallocateMemory(heap,size+64,1);
  heap->lastChunk->size+=size-oldSize;
  heap->endRam=end;
 }
 _ReleaseSemaphore();return 1;
}
void func_8C03FF46(void *h)
{
 tHeap *heap,*p;

 _GetSemaphore();heap=_ConvertHeap(h);
 for(p=gMainHeap;p;p=p->next) if(p->next==heap) {p->next=heap->next;break;}
 _DeallocateMemory(heap,1);
 _ReleaseSemaphore();
}
void func_8C03FFBC(void *h)
{
 tHeap *heap; tMemoryChunk *chunk;
 _GetSemaphore();heap=_ConvertHeap(h);
 for(chunk=heap->chunks;chunk;chunk=chunk->next)
  if(chunk->used&&!chunk->locked&&!chunk->master&&!chunk->internal) {
   _DeallocateMemory((unsigned char *)chunk+32,0);chunk=heap->chunks;
  }
 func_8C04002A(heap);_ReleaseSemaphore();
}
void func_8C04002A(tHeap *heap)
{
 tMemoryChunk *chunk,*nextChunk,*freeChunk,*newChunk,*prevChunk;
 tMasterBlock *block,*lastBlock;unsigned int size,i;int adjacent;
 for(chunk=heap->chunks;chunk;chunk=nextChunk) {
  nextChunk=chunk->next;
  if(!chunk->used||!chunk->master||chunk->locked)continue;
  adjacent=0;

 for(freeChunk=heap->chunks;freeChunk;freeChunk=freeChunk->next) {
  if(freeChunk==chunk) {freeChunk=0;break;}
  if(!freeChunk->used) {
   if(freeChunk->size>=chunk->size) break;
   if(freeChunk->next==chunk) {adjacent=1;break;}
  }
 }
  if(!freeChunk)continue;

 freeChunk->master=chunk->master;
 *(void **)freeChunk->master=(unsigned char *)freeChunk+32;
 freeChunk->used=1;
 freeChunk->internal=chunk->internal;
 freeChunk->locked=chunk->locked;
 chunk->master=0;
 size=chunk->size;
 memmove((unsigned char *)freeChunk+32,(unsigned char *)chunk+32,size);
 if(adjacent || freeChunk->size-size>=64) {
  newChunk=(tMemoryChunk *)((unsigned char *)freeChunk+size+32);
  newChunk->next=adjacent?nextChunk:freeChunk->next;
  if(newChunk->next) newChunk->next->prev=newChunk;else heap->lastChunk=newChunk;
  newChunk->prev=freeChunk;
  newChunk->size=freeChunk->size-(adjacent?0:size+32);
  newChunk->master=0;newChunk->used=adjacent;newChunk->internal=0;newChunk->locked=0;newChunk->magic=0xfedcba98;
  freeChunk->next=newChunk;freeChunk->size=size;
  if(adjacent)chunk=newChunk;
 }

  prevChunk=chunk->prev;
  _DeallocateMemory((unsigned char *)chunk+32,0);
  if(nextChunk->magic!=0xfedcba98) {
   if(chunk->magic!=0xfedcba98)nextChunk=prevChunk->next;
   else nextChunk=chunk->next;
  }
 }
 lastBlock=&heap->masters;
 for(block=lastBlock->nextBlock;block;block=block->nextBlock) {
  for(i=0;i<block->numMasters;i++)if(((void **)block->masters)[i])break;
  if(i>=block->numMasters) {
   lastBlock->nextBlock=block->nextBlock;
   _DeallocateMemory(block->masters,1);_DeallocateMemory(block,1);block=lastBlock;
  }
 }
}

unsigned int func_8C0401AA(void *h)
{unsigned int size;_GetSemaphore();size=func_8C0401E0(_ConvertHeap(h));_ReleaseSemaphore();return size;}
unsigned int func_8C0401E0(tHeap *heap)
{tMemoryChunk *chunk;unsigned int size=0;for(chunk=heap->chunks;chunk;chunk=chunk->next)if(!chunk->used)size+=chunk->size;return size;}

unsigned int LargestAvailableMemoryHeapChunk(void * heap)
{
    unsigned int maxChunk;
    _GetSemaphore();
    maxChunk = _LargestAvailableMemoryChunk(_ConvertHeap(heap));
    _ReleaseSemaphore();
    return maxChunk;
}
unsigned int _LargestAvailableMemoryChunk(tHeap *heap)
{
    tMemoryChunk *chunk;
    unsigned int maxChunk=0;
    for(chunk=heap->chunks;chunk;chunk=chunk->next) if(!chunk->used && chunk->size>maxChunk) maxChunk=chunk->size;
    return maxChunk;
}
void * AllocateMemoryHeap(void * heap, unsigned int size)
{
    void *ptr;
    _GetSemaphore();
    ptr = _AllocateMemory(_ConvertHeap(heap),size,0,0);
    _ReleaseSemaphore();
    return ptr;
}
void * _AllocateMemory(tHeap *heap, unsigned int size, void * master, int internal)
{
    tMemoryChunk *chunk;
    tMemoryChunk *newChunk;
    unsigned int freeMem=0,maxFree=0;
    size = (size + 31) & ~31;

    retry:
    for(chunk=heap->chunks;chunk;chunk=chunk->next)
    {
        if(!chunk->used)
        {
            if(chunk->size>maxFree)maxFree=chunk->size;
            if(chunk->size>=size)break;
        }
    }
    if(!chunk && heap->emergencyFunc)
    {
        _ReleaseSemaphore();
        if(heap->emergencyFunc(size))
        {
            _GetSemaphore();
            goto retry;
        }
        _GetSemaphore();
    }
    if(chunk->size-size>=64)
    {
        newChunk=(tMemoryChunk *)((unsigned char *)chunk+size+32);
        newChunk->next=chunk->next;
        if(newChunk->next)newChunk->next->prev=newChunk;
        else heap->lastChunk=newChunk;
        newChunk->prev=chunk;
        newChunk->size=chunk->size-size-32;
        newChunk->master=0;
        newChunk->used=0;
        newChunk->internal=0;
        newChunk->locked=0;
        newChunk->magic=0xfedcba98;
        chunk->next=newChunk;
        chunk->size=size;
    }
    chunk->used=1;
    chunk->master=master;
    chunk->internal=internal;
    return (unsigned char *)chunk+32;
}
void * AllocateMemoryHeapHigh(void * h, unsigned int size)
{
    tHeap *heap;
    tMemoryChunk *chunk,*newChunk;
    unsigned int freeMem,maxFree;
    _GetSemaphore();
    heap=_ConvertHeap(h);
    size=(size+31)&~31;

    freeMem=0;
    maxFree=0;
    retry:
    for(chunk=heap->lastChunk;chunk;chunk=chunk->prev)
    {
        if(!chunk->used)
        {
            if(chunk->size>maxFree)maxFree=chunk->size;
            if(chunk->size>=size)break;
        }
    }
    if(!chunk && heap->emergencyFunc)
    {
        _ReleaseSemaphore();
        if(heap->emergencyFunc(size))
        {
            _GetSemaphore();
            goto retry;
        }
        _GetSemaphore();
    }
    if(chunk->size-size>=64)
    {
        newChunk=(tMemoryChunk *)((unsigned char *)chunk+chunk->size-size);
        newChunk->next=chunk->next;
        if(newChunk->next)newChunk->next->prev=newChunk;
        else heap->lastChunk=newChunk;
        newChunk->prev=chunk;
        newChunk->size=size;
        newChunk->master=0;
        newChunk->used=1;
        newChunk->internal=0;
        newChunk->locked=0;
        newChunk->magic=0xfedcba98;
        chunk->next=newChunk;
        chunk->size-=size+32;
        chunk=newChunk;
    }
    else
    {
        chunk->master=0;
        chunk->used=1;
        chunk->internal=0;
    }
    _ReleaseSemaphore();
    return (unsigned char *)chunk+32;
}

void *func_8C040474(void *ptr,unsigned int size)
{
 tHeap *heap;tMemoryChunk *chunk,*newChunk;

 _GetSemaphore();heap=_FindHeap(ptr);size=(size+31)&~31;
 for(chunk=heap->chunks;chunk;chunk=chunk->next)
  if((unsigned int)ptr>=(unsigned int)chunk&&(!chunk->next||(unsigned int)ptr<(unsigned int)chunk->next)) {
   break;
  }

 if((unsigned char *)chunk+32!=(unsigned char *)ptr) {
  newChunk=(tMemoryChunk *)((unsigned char *)ptr-32);newChunk->next=chunk->next;
  if(newChunk->next)newChunk->next->prev=newChunk;else heap->lastChunk=newChunk;
  newChunk->master=0;newChunk->used=1;newChunk->internal=0;newChunk->locked=0;newChunk->magic=0xfedcba98;
  newChunk->size=(unsigned char *)chunk+chunk->size-(unsigned char *)ptr+32;
  if((unsigned char *)ptr-(unsigned char *)chunk-32>=64) {
   newChunk->prev=chunk;chunk->next=newChunk;chunk->size-=newChunk->size+32;
  }else {
   newChunk->prev=chunk->prev;chunk->prev->next=newChunk;
   chunk->prev->size+=(unsigned char *)ptr-(unsigned char *)chunk-32;
  }
  chunk=newChunk;
 }
 if(chunk->size-size>=64) {
  newChunk=(tMemoryChunk *)((unsigned char *)chunk+size+32);newChunk->next=chunk->next;
  if(newChunk->next)newChunk->next->prev=newChunk;else heap->lastChunk=newChunk;
  newChunk->prev=chunk;newChunk->size=chunk->size-size-32;
  newChunk->master=0;newChunk->used=0;newChunk->internal=0;newChunk->locked=0;newChunk->magic=0xfedcba98;
  chunk->next=newChunk;chunk->size=size;
 }
 chunk->master=0;chunk->used=1;chunk->internal=0;_ReleaseSemaphore();return (unsigned char *)chunk+32;
}

void ReallocateMemory(void * ptr, unsigned int newSize)
{
    _GetSemaphore();
    _ReallocateMemory(ptr,newSize,0);
    _ReleaseSemaphore();
}
void _ReallocateMemory(void * ptr, unsigned int newSize, int internal)
{
    tHeap *heap;
    tMemoryChunk *chunk,*nextChunk,*newChunk;
    newSize=(newSize+31)&~31;

    heap=_FindHeap(ptr);
    chunk=_FindChunk(heap,ptr,internal);
    if(newSize<=chunk->size)
    {
        nextChunk=chunk->next;
        if(!nextChunk || nextChunk->used)
        {
            if(chunk->size-newSize>=64)
            {
                newChunk=(tMemoryChunk *)((unsigned char *)chunk+newSize+32);
                newChunk->next=nextChunk;
                if(newChunk->next)newChunk->next->prev=newChunk;
                else heap->lastChunk=newChunk;
                newChunk->prev=chunk;
                newChunk->size=chunk->size-newSize-32;
                newChunk->master=0;
                newChunk->used=0;
                newChunk->internal=0;
                newChunk->locked=0;
                newChunk->magic=0xfedcba98;
                chunk->next=newChunk;
                chunk->size=newSize;
            }
        }
        else if(chunk->size-newSize>=32)
        {
            newChunk=(tMemoryChunk *)((unsigned char *)chunk+newSize+32);
            newChunk->next=nextChunk->next;
            if(newChunk->next)newChunk->next->prev=newChunk;
            else heap->lastChunk=newChunk;
            newChunk->prev=chunk;
            newChunk->size=chunk->size-newSize+nextChunk->size;
            newChunk->master=0;
            newChunk->used=0;
            newChunk->internal=0;
            newChunk->locked=0;
            newChunk->magic=0xfedcba98;
            chunk->next=newChunk;
            chunk->size=newSize;
        }
    }
    else
    {
        nextChunk=chunk->next;

        if(nextChunk->size-(newSize-chunk->size)<32)
        {
            chunk->next=nextChunk->next;
            if(chunk->next)chunk->next->prev=chunk;
            else heap->lastChunk=chunk;
            newSize=chunk->size+nextChunk->size+32;
        }
        else
        {
            newChunk=(tMemoryChunk *)((unsigned char *)chunk+newSize+32);
            newChunk->next=nextChunk->next;
            if(newChunk->next)newChunk->next->prev=newChunk;
            else heap->lastChunk=newChunk;
            newChunk->prev=chunk;
            newChunk->size=chunk->size+nextChunk->size-newSize;
            newChunk->master=0;
            newChunk->used=0;
            newChunk->internal=0;
            newChunk->locked=0;
            newChunk->magic=0xfedcba98;
            chunk->next=newChunk;
        }
        chunk->size=newSize;
    }
}
void * AllocateHandleHeap(void * h, unsigned int size)
{
    tHeap *heap;
    void *master;
    tMasterBlock *block;
    int i;
    _GetSemaphore();
    heap=_ConvertHeap(h);
    block=&heap->masters;
    if(!heap->masters.masters)
    {
        block->numMasters=heap->numAdditionalMasters;
        block->masters=_AllocateMemory(heap,heap->numAdditionalMasters*4,0,1);
        memset(block->masters,0,heap->numAdditionalMasters*4);
        block->nextBlock=0;
    }
    for(;;)
    {
        master=block->masters;
        for(i=0;i<block->numMasters;i++,master=(unsigned char *)master+4)if(!*(void **)master)break;
        if(i<block->numMasters)break;
        if(!block->nextBlock)
        {
            block->nextBlock=_AllocateMemory(heap,sizeof(tMasterBlock),0,1);
            block->nextBlock->numMasters=heap->numAdditionalMasters;
            block->nextBlock->masters=_AllocateMemory(heap,heap->numAdditionalMasters*4,0,1);
            memset(block->nextBlock->masters,0,heap->numAdditionalMasters*4);
            block->nextBlock->nextBlock=0;
        }
        block=block->nextBlock;
    }
    *(void **)master=_AllocateMemory(heap,size,master,0);
    _ReleaseSemaphore();
    return master;
}
void _FillFreeMem(void *ptr,unsigned int size)
{unsigned int *p=ptr;unsigned int n,i; n=size/4;for(i=0;i<n;i++,p++)*p=0x7fff0bad;}

void DeallocateMemory(void * ptr)
{
    _GetSemaphore();
    _DeallocateMemory(ptr,0);
    _ReleaseSemaphore();
}
void _DeallocateMemory(void * ptr, int internal)
{
    tHeap *heap;
    tMemoryChunk *chunk,*nextChunk,*prevChunk;
    heap=_FindHeap(ptr);
    chunk=_FindChunk(heap,ptr,internal);
    _FillFreeMem((unsigned char *)chunk+32,chunk->size);
    if(chunk->master)*(void **)chunk->master=0;
    nextChunk=chunk->next;
    if(nextChunk&&!nextChunk->used)
    {
        chunk->next=nextChunk->next;
        if(chunk->next)chunk->next->prev=chunk;
        else heap->lastChunk=chunk;
        chunk->size+=nextChunk->size+32;
        _FillFreeMem(nextChunk,32);
    }
    chunk->master=0;
    chunk->used=0;
    chunk->internal=0;
    chunk->locked=0;
    prevChunk=chunk->prev;
    if(prevChunk&&!prevChunk->used)
    {
        prevChunk->next=chunk->next;
        if(prevChunk->next)prevChunk->next->prev=prevChunk;
        else heap->lastChunk=prevChunk;
        prevChunk->size+=chunk->size+32;
        _FillFreeMem(chunk,32);
    }
}
void PurgeableMemory(void * ptr, int purgeable)
{
    tHeap *heap;
    tMemoryChunk *chunk;
    _GetSemaphore();
    heap=_FindHeap(ptr);
    chunk=_FindChunk(heap,ptr,0);
    if(chunk->master)
    {
        _ReleaseSemaphore();
        return;
    }
    if(purgeable)
    {
        if(chunk->locked)chunk->locked--;
    }
    else
    {
        if(chunk->locked<255)chunk->locked++;
    }
    _ReleaseSemaphore();
}

void func_8C040990(void *ptr)
{tHeap *heap;tMemoryChunk *chunk;_GetSemaphore();heap=_FindHeap(ptr);chunk=_FindChunk(heap,ptr,0);if(chunk->locked<255)chunk->locked++;_ReleaseSemaphore();}
void func_8C0409D4(void *ptr)
{tHeap *heap;tMemoryChunk *chunk;_GetSemaphore();heap=_FindHeap(ptr);chunk=_FindChunk(heap,ptr,0);if(chunk->locked)chunk->locked--;_ReleaseSemaphore();}
void func_8C040A16(void *ptr)
{
 tHeap *heap;tMemoryChunk *chunk,*nextChunk,*freeChunk,*newChunk;unsigned int size;int adjacent=0;
 _GetSemaphore();heap=_FindHeap(ptr);chunk=_FindChunk(heap,ptr,0);
 nextChunk=chunk->next;

 for(freeChunk=heap->chunks;freeChunk;freeChunk=freeChunk->next) {
  if(freeChunk==chunk) {freeChunk=0;break;}
  if(!freeChunk->used) {
   if(freeChunk->size>=chunk->size) break;
   if(freeChunk->next==chunk) {adjacent=1;break;}
  }
 }
 if(!freeChunk) {_ReleaseSemaphore();return;}

 freeChunk->master=chunk->master;
 *(void **)freeChunk->master=(unsigned char *)freeChunk+32;
 freeChunk->used=1;
 freeChunk->internal=chunk->internal;
 freeChunk->locked=chunk->locked;
 chunk->master=0;
 size=chunk->size;
 memmove((unsigned char *)freeChunk+32,(unsigned char *)chunk+32,size);
 if(adjacent || freeChunk->size-size>=64) {
  newChunk=(tMemoryChunk *)((unsigned char *)freeChunk+size+32);
  newChunk->next=adjacent?nextChunk:freeChunk->next;
  if(newChunk->next) newChunk->next->prev=newChunk;else heap->lastChunk=newChunk;
  newChunk->prev=freeChunk;
  newChunk->size=freeChunk->size-(adjacent?0:size+32);
  newChunk->master=0;newChunk->used=adjacent;newChunk->internal=0;newChunk->locked=0;newChunk->magic=0xfedcba98;
  freeChunk->next=newChunk;freeChunk->size=size;
  if(adjacent)chunk=newChunk;
 }
 _DeallocateMemory((unsigned char *)chunk+32,0);_ReleaseSemaphore();
}

unsigned int GetPtrSize(void * ptr)
{
    unsigned int size;
    _GetSemaphore();
    size=_GetPtrSize(ptr,0);
    _ReleaseSemaphore();
    return size;
}
unsigned int _GetPtrSize(void * ptr, int internal)
{
    tHeap *heap;
    tMemoryChunk *chunk;
    heap=_FindHeap(ptr);
    chunk=_FindChunk(heap,ptr,internal);
    return chunk->size;
}
tHeap *_FindHeap(void * ptr)
{
    tHeap *heap,*lastHeap=0;
    for(heap=gMainHeap;heap;heap=heap->next)if((unsigned int)ptr>=(unsigned int)heap->chunks&&(unsigned int)ptr<(unsigned int)heap->endRam)lastHeap=heap;
    return lastHeap;
}
tMemoryChunk *_FindChunk(tHeap *heap, void * ptr, int internal)
{
    tMemoryChunk *chunk;
    tMasterBlock *block;
    for(block=&heap->masters;block;block=block->nextBlock)
    {
        if(!block->masters)break;
        if((unsigned int)ptr>=(unsigned int)block->masters&&(unsigned int)ptr<(unsigned int)block->masters+block->numMasters*4)
        {
            ptr=*(void **)ptr;
            break;
        }
    }
    for(chunk=heap->chunks;chunk;chunk=chunk->next)if(chunk->internal==internal&&(unsigned int)ptr>=(unsigned int)chunk&&(!chunk->next||(unsigned int)ptr<(unsigned int)chunk->next))break;
    return chunk;
}
