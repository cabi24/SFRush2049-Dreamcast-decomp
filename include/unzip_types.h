#ifndef UNZIP_TYPES_H
#define UNZIP_TYPES_H
typedef struct huft huft;
typedef union tHuftValue {
    unsigned short n; /* target huft numeric word+4: 105E8/1060E */
    huft *t; /* target pointer+4: 1058A/107A8 */
} tHuftValue;
struct huft {
    unsigned char e; /* target byte+0: inflate_codes10758 and huft_build105E0 */
    unsigned char b; /* target byte+1: inflate_codes10770, huft_build10580 */
    tHuftValue v;
}; /* stride8: target10552/1059E and 1075C shifts. */
typedef char dc_check_huft[(sizeof(huft)==8)?1:-1];
void *memset(void *dest,int value,unsigned int size);
void *mem_func_8C040380(void *heap,unsigned int size);
void DeallocateMemory(void *ptr);
#endif
