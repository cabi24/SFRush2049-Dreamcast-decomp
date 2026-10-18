#ifndef DC_QUEUE_INIT_TYPES_H
#define DC_QUEUE_INIT_TYPES_H
#include "dc_types.h"
typedef struct tDCInitNode {
    tDLLE ll; /* direct list insertion handle0 writes intrusive prefix. */
    char opaque[0x44c-8]; /* bounds/increment069E60..80 and069EF2..9F0C. */
} tDCInitNode;
typedef char dc_init_node_size[(sizeof(tDCInitNode)==0x44c)?1:-1];
extern tDLList midiFree8C177B3C,midiActive8C177B4C,midiPending8C177B5C;
extern tDLList streamFree8C177FC0,streamActive8C177FD0,streamPending8C177FE0;
extern tDCInitNode midiInitNodes8C177B6C[1],streamInitNodes8C177FF0[1];
/* Defined/credited by frozen dc_sound,not allocated in this unit. */
extern int midiWorkBytes8C0C7C2C,midiReadBytes8C0C7C30;
extern unsigned int dcQueueWorkWord8C0C7C34; /* SDK output is one opaque32-bit word; no pointer type inferred. */
extern void *midiRead1_8C0C7C38,*midiRead2_8C0C7C3C;
void InitLL(tDLList *,char,char);
void InsertLLE(tDLList *,void *,void *);
int func_8C0A18E6(unsigned int *);
int func_8C0A191C(unsigned int *,unsigned int,int,int,int);
void *AllocateMemoryHeap(void *,unsigned int);
void PurgeableMemory(void *,int);
#endif
