#include "dc_queue_init_types.h"

void func_8C069E34(void)
{
    tDCInitNode *node;
    unsigned int available;
    InitLL(&midiFree8C177B3C,0,1);
    InitLL(&midiPending8C177B5C,0,1);
    InitLL(&midiActive8C177B4C,0,1);
    node=midiInitNodes8C177B6C;
    if(node<midiInitNodes8C177B6C+1) {
        do {
            InsertLLE(&midiFree8C177B3C,node,midiFree8C177B3C.head);
            node++;
        } while(node<midiInitNodes8C177B6C+1);
    }
    midiWorkBytes8C0C7C2C=0x10000;
    midiReadBytes8C0C7C30=0x20000;
    func_8C0A18E6(&available);
    func_8C0A191C(&dcQueueWorkWord8C0C7C34,midiWorkBytes8C0C7C2C,32,2,0);
    midiRead1_8C0C7C38=AllocateMemoryHeap(0,midiReadBytes8C0C7C30);
    PurgeableMemory(midiRead1_8C0C7C38,0);
    midiRead2_8C0C7C3C=AllocateMemoryHeap(0,2048);
    PurgeableMemory(midiRead2_8C0C7C3C,0);
    InitLL(&streamFree8C177FC0,0,1);
    InitLL(&streamPending8C177FE0,0,1);
    InitLL(&streamActive8C177FD0,0,1);
    node=streamInitNodes8C177FF0;
    if(node<streamInitNodes8C177FF0+1) {
        do {
            InsertLLE(&streamFree8C177FC0,node,streamFree8C177FC0.head);
            node++;
        } while(node<streamInitNodes8C177FF0+1);
    }
}
