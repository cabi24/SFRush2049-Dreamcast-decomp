/* Private Dreamcast pak prefix. Target offsets, not MAT3/PS2 layout. */
#include "dc_types.h"
typedef struct tDCPak {
    char pad0;
    signed char inserted; /* +1:CBE2/CBE4 */
    char pad2[3];
    signed char damaged; /* +5:CB52/CB80 */
    signed char wasRemoved; /* +6:CBEA */
    char pad7[0xD8-7];
    int error; /* +D8:CBC8..CBDC; native full-word comparisons */
    char padDC[0xE0-0xDC];
    tDLList files; /* +E0:CB30/CB3E; indexed strideF0 CBC2 */
} tDCPak;
typedef char dc_check_pak_stride[(sizeof(tDCPak)==0xF0)?1:-1];
