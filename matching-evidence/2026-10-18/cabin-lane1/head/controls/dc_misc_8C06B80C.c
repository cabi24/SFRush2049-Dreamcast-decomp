/* Target-derived GDFS/cache wrappers; anonymous map identities preserved. */
#include "dc_misc_types.h"
extern signed char dcSharedDiscFlag;
extern unsigned char dcGdfsWork[];
extern unsigned char dcGdfsDirectory[];
extern unsigned char dcProbeBuffer[];
extern volatile unsigned int dcFrameCounter;
extern unsigned int dcInputBits;
extern volatile int dcMode_BFA78;
extern signed char dcMode_FDD4;
extern signed char dcMode_FDD6;
extern void func_8C039718(void);
extern int func_8C0D5536(int,void *,int,void *);
extern void *func_8C0D62D8(void *,int);
extern void func_8C0D6C66(void (*)(int,int),int);
extern void InitLL(tDLList *,int,int);
extern void InsertLLE(tDLList *,void *,void *);
extern void RemoveLLE(tDLList *,void *);
extern void *func_8C03FD8A(void *,unsigned int,int,int);
extern void func_8C03FD54(int,int (*)(unsigned int));
extern void func_8C03FF46(void *);
extern void func_8C0D6B52(void);
extern int func_8C06CA44(void);
extern int func_8C06A1CA(void);
extern void *func_8C0D6BDE(const char *,int);
extern int func_8C0D6426(void *,int,void *);
extern void func_8C0D62FC(void *);
extern int func_8C0D6C94(void);
extern int strcmp(const char *,const char *);
extern char *strcpy(char *,const char *);
extern void *memcpy(void *,const void *,unsigned int);
extern void *AllocateMemoryHeap(void *,unsigned int);
extern void *AllocateHandleHeap(void *,unsigned int);
extern void ReallocateMemory(void *,unsigned int);
extern void DeallocateMemory(void *);
extern unsigned int func_8C0401AA(void *);
extern unsigned int LargestAvailableMemoryHeapChunk(void *);
extern void func_8C03FFBC(void *);
extern int func_8C03FE8E(void *,unsigned int);
extern void func_8C0D6B3C(void);
extern int func_8C0D6334(void *,int *);
extern int func_8C0D6368(void *,int *);

void *dcFileCacheHeap = 0;
signed char dcDiscError = 0;
/* B prefix18483C..87C is64bytes; SDK work/probe arrays remain external. */
void *dcGdfsRoot;
unsigned int dcLastDiscFrame;
tDLList dcCachedFiles;
tDLList dcUnloadedFiles;
unsigned int dcCacheAvailable;
unsigned int dcNumLoads;
unsigned int dcCacheHits;
unsigned int dcCacheMisses;
unsigned int dcCacheHitSize;
unsigned int dcCacheMissSize;

int func_8C06BF9C(unsigned int);
tDCFileCache **func_8C06BAD4(tDLList *,const char *);
void func_8C06BB0C(const char *,void *,unsigned int);
void func_8C06BD7E(void);
int func_8C06BDE6(int,unsigned int,int);
void func_8C06BE4C(tDCFileCache **);
void *func_8C06BE76(const char *,void *,unsigned int *,int,int);

void func_8C06B80C(int context,int error)
{
    if (error==-23 || error==-37 || error==-33) {
        dcDiscError=1;
        dcSharedDiscFlag=1;
    }
}

void func_8C06B82C(void)
{
    int attempts=8;
    int error;
    do {
        error=func_8C0D5536(8,(void *)(((unsigned int)dcGdfsWork & ~31)+32),0x400,(void *)(((unsigned int)dcGdfsDirectory & ~31)+32));
        if (!error) break;
        if (error==-23 || error==-37 || error==-33) func_8C039718();
        attempts--;
    } while (attempts>0);
    dcGdfsRoot=func_8C0D62D8(dcGdfsDirectory,0x400);
    func_8C0D6C66(func_8C06B80C,0);
    InitLL(&dcCachedFiles,1,1);
    InitLL(&dcUnloadedFiles,1,1);
    dcCacheAvailable=5700000;
    dcFileCacheHeap=func_8C03FD8A(0,5700000,256,64);
    dcNumLoads=0;
    dcCacheMisses=0;
    dcCacheHits=0;
    dcCacheMissSize=0;
    dcCacheHitSize=0;
    func_8C03FD54(0,func_8C06BF9C);
}

void func_8C06B954(void)
{
    if (dcFileCacheHeap) {
        func_8C03FF46(dcFileCacheHeap);
        dcFileCacheHeap=0;
        func_8C0D6B52();
    }
}

void func_8C06B976(void)
{
    void *file;
    unsigned int pressed;
    if (func_8C06CA44() || func_8C06A1CA()) {
        dcLastDiscFrame=dcFrameCounter;
        return;
    }
    pressed=dcInputBits & 0x7C0000;
    if (pressed && !dcMode_BFA78) {
        dcLastDiscFrame=dcFrameCounter;
        return;
    }
    if (pressed && !dcMode_BFA78 && (!dcMode_FDD4 || dcMode_FDD6>18)) goto check_error;
    if (dcFrameCounter>dcLastDiscFrame+60) {
        dcLastDiscFrame=dcFrameCounter;
        file=func_8C0D6BDE("digitsmfontdat.lzs",0);
        if (file) {
            func_8C0D6426(file,1,dcProbeBuffer);
            func_8C0D62FC(file);
        }
    }
check_error:
    if (dcDiscError==1) func_8C039718();
}

int func_8C06BA20(void)
{
    int result=0;
    int state=func_8C0D6C94();
    if (state==6) result=1;
    return result;
}

void *func_8C06BA3A(const char *name,void *buffer,unsigned int *size,int cache,int check)
{
    tDCFileCache **fileh;
    tDCFileCache *file;
    dcNumLoads++;
    fileh=cache ? func_8C06BAD4(&dcCachedFiles,name):0;
    if (!fileh) return func_8C06BE76(name,buffer,size,cache,check);
    file=*fileh;
    dcCacheHits++;
    dcCacheHitSize+=file->size;
    *size=file->size;
    if (!buffer) buffer=AllocateMemoryHeap(0,file->size);
    memcpy(buffer,file+1,file->size);
    file->accesses++;
    file->lastAccess=dcNumLoads;
    return buffer;
}

tDCFileCache **func_8C06BAD4(tDLList *list,const char *name)
{
    tDCFileCache **fileh;
    for (fileh=list->head;fileh;fileh=(*fileh)->next)
        if (!strcmp((*fileh)->name,name)) return fileh;
    return 0;
}

void func_8C06BB0C(const char *name,void *buffer,unsigned int size)
{
    tDCFileCache **old;
    tDCFileCache **victim;
    tDCFileCache **fileh;
    tDCFileCache *file;
    unsigned int available;
    int compacted=0;
    int rating;
    dcCacheMisses++;
    dcCacheMissSize+=size;
    func_8C06BD7E();
    old=func_8C06BAD4(&dcUnloadedFiles,name);
    if (old) {
        (*old)->accesses++;
        (*old)->lastAccess=dcNumLoads;
        rating=func_8C06BDE6((*old)->accesses,0,size);
    } else rating=func_8C06BDE6(1,0,size);
    available=func_8C0401AA(dcFileCacheHeap);
    for (;;) {
        if (!compacted && available>=size+508) {
            compacted=1;
            func_8C03FFBC(dcFileCacheHeap);
            available=LargestAvailableMemoryHeapChunk(dcFileCacheHeap);
        }
        if (compacted && available>=size+508) break;
        victim=dcCachedFiles.tail;
        if (!victim || rating>=(*victim)->rating) {
            if (old) return;
            if (LargestAvailableMemoryHeapChunk(dcFileCacheHeap)<508) return;
            fileh=AllocateHandleHeap(dcFileCacheHeap,60);
            (*fileh)->accesses=1;
            (*fileh)->generation=0;
            (*fileh)->lastAccess=dcNumLoads;
            strcpy((*fileh)->name,name);
            InsertLLE(&dcUnloadedFiles,fileh,dcUnloadedFiles.head);
            return;
        }
        RemoveLLE(&dcCachedFiles,victim);
        ReallocateMemory(victim,60);
        InsertLLE(&dcUnloadedFiles,victim,dcUnloadedFiles.head);
        if (compacted) {
            func_8C03FFBC(dcFileCacheHeap);
            available=LargestAvailableMemoryHeapChunk(dcFileCacheHeap);
        } else available=func_8C0401AA(dcFileCacheHeap);
    }
    fileh=AllocateHandleHeap(dcFileCacheHeap,size+60);
    file=*fileh;
    if (old) {
        file->accesses=(*old)->accesses;
        file->generation=(*old)->generation+1;
        RemoveLLE(&dcUnloadedFiles,old);
        DeallocateMemory(old);
    } else {
        file->accesses=1;
        file->generation=1;
    }
    file->lastAccess=dcNumLoads;
    file->rating=rating;
    file->size=size;
    strcpy(file->name,name);
    memcpy(file+1,buffer,size);
    func_8C06BE4C(fileh);
}

void func_8C06BD7E(void)
{
    tDLList temp;
    tDCFileCache **fileh;
    tDCFileCache *file;
    InitLL(&temp,1,1);
    while ((fileh=dcCachedFiles.head)!=0) {
        RemoveLLE(&dcCachedFiles,fileh);
        InsertLLE(&temp,fileh,temp.head);
    }
    while ((fileh=temp.head)!=0) {
        RemoveLLE(&temp,fileh);
        file=*fileh;
        file->rating=func_8C06BDE6(file->accesses,dcNumLoads-file->lastAccess,file->size);
        func_8C06BE4C(fileh);
    }
}

int func_8C06BDE6(int count,unsigned int age,int size)
{
    return (count-age/(dcCacheAvailable/4000U))*-300 + size/1000;
}

void func_8C06BE4C(tDCFileCache **fileh)
{
    tDCFileCache **next;
    for (next=dcCachedFiles.head;next;next=(*next)->next) {
        if ((*fileh)->rating<(*next)->rating) break;
    }
    InsertLLE(&dcCachedFiles,fileh,next);
}

void *func_8C06BE76(const char *name,void *buffer,unsigned int *size,int cache,int check)
{
    void *file;
    int ssize;
    int location;
    int blocks;
    file=func_8C0D6BDE(name,0);
    if (!file) {
        func_8C0D6B3C();
        file=func_8C0D6BDE(name,0);
    }
    func_8C0D6334(file,&ssize);
    *size=ssize;
    blocks=(ssize+2047)>>11;
    if (!buffer) buffer=AllocateMemoryHeap(0,blocks<<11);
    if (check) {
        if (!func_8C0D6368(file,&location) || location<400195)
            *(volatile unsigned int *)0xA05F80D8 &= 0xFFFF0000;
    }
    func_8C0D6426(file,blocks,buffer);
    if (dcDiscError==1) func_8C039718();
    func_8C0D62FC(file);
    dcLastDiscFrame=dcFrameCounter;
    if (cache) func_8C06BB0C(name,buffer,*size);
    return buffer;
}

int func_8C06BF9C(unsigned int size)
{
    unsigned int needed=size+256;
    unsigned int available=func_8C0401AA(dcFileCacheHeap);
    int compacted=0;
    tDCFileCache **fileh;
    for (;;) {
        if (!compacted && available>=needed) {
            compacted=1;
            func_8C03FFBC(dcFileCacheHeap);
            available=LargestAvailableMemoryHeapChunk(dcFileCacheHeap);
        }
        if (compacted && available>=needed) break;
        fileh=dcCachedFiles.tail;
        if (!fileh) return 0;
        RemoveLLE(&dcCachedFiles,fileh);
        ReallocateMemory(fileh,60);
        InsertLLE(&dcUnloadedFiles,fileh,dcUnloadedFiles.head);
        if (compacted) {
            func_8C03FFBC(dcFileCacheHeap);
            available=LargestAvailableMemoryHeapChunk(dcFileCacheHeap);
        } else available=func_8C0401AA(dcFileCacheHeap);
    }
    dcCacheAvailable-=needed;
    return func_8C03FE8E(dcFileCacheHeap,dcCacheAvailable);
}
