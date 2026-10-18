/* Private startup/video/byte-LZ contracts from actual target registers. */
#ifndef UNK_8C010218_TYPES_H
#define UNK_8C010218_TYPES_H
#include "dc_types.h"
extern signed short dcVideoOffsetX,dcVideoOffsetY; /* 010228/22C, pools010260/264. */
extern void func_8C0DAE20(int,int); /* 010254..25C: R4=x,R5=y+8. */
extern void dcModelService(int); /* startup010270/274, existingalias0726F4. */
extern void *memset(void *,int,unsigned int); /* 010278..27E,09BE04. */
extern unsigned char dcStartupBuffer[]; /* 8C004000, cleared4000bytes; endnotindependentlyknown. */
extern void func_8C03954A(void),func_8C03A3AE(void);
#endif
