/* Private DC layouts. References are target SH-4 instructions. */
#ifndef MEMSYS_TYPES_H
#define MEMSYS_TYPES_H
typedef struct tMemoryChunk tMemoryChunk;
typedef struct tMasterBlock tMasterBlock;
typedef struct tHeap tHeap;
struct tMemoryChunk {
 unsigned int magic; /* FE24,03340 */
 tMemoryChunk *next; /* FE28/+4,FE2C/+8; allocation links. */
 tMemoryChunk *prev;
 unsigned int size; /* FE36/+12. */
 void *master; /* FE3A/+16. */
 signed char used; /* FE3C..3E/+20; FF E8 tests. */
 signed char internal; /* FE40..44/+21; 40BE2 signed compare. */
 unsigned char locked; /* FE46..4A/+22;40956..958 extu.b. */
}; /* Object payload starts at +32; record is partial, size24 is NOT header size. */
struct tMasterBlock {
 unsigned int numMasters; /* FE58/+24 of heap,4079C/+0. */
 void *masters; /* FE6E/+28 of heap,40790/+4. */
 tMasterBlock *nextBlock; /* FE5E/+32 of heap,407AC/+8;407AE sizeof12. */
};
struct tHeap {
 unsigned int magic; /* FE1C/+0. */
 tHeap *next; /* FE20/+4. */
 tMemoryChunk *chunks; /* FE22/+8. */
 tMemoryChunk *lastChunk; /* FE52/+12. */
 unsigned char *endRam; /* FD3E/+16. */
 unsigned short numAdditionalMasters; /* FE5A/+20. */
 unsigned char pad16[2];
 tMasterBlock masters; /* FE58..FE6E/+24..32. */
 int (*emergencyFunc)(unsigned int); /* FD7A store+36;402DE/402EE call. */
}; /* +64 first chunk is allocator header extent, not sizeof(tHeap). */
typedef char checkMasterSize[(sizeof(tMasterBlock)==12)?1:-1];
typedef char checkHeapFields[(sizeof(tHeap)==40)?1:-1];
int SetInterruptMask(int mask);
void sdk_heap_bounds(unsigned int *first, unsigned int *second);
void *sdk_heap_alloc(unsigned int size);
void *memset(void *, int, unsigned int);
void *memmove(void *, const void *, unsigned int);
#endif
