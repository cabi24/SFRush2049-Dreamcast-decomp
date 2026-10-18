#include "dc_types.h"
/* Target cache header offsets BA8C..C4, BD0C..52; payload starts3C. */
typedef struct tDCFileCache {
    struct tDCFileCache **next;
    struct tDCFileCache **prev;
    char name[32];
    unsigned int size;
    int accesses;
    unsigned int lastAccess;
    int rating;
    signed char generation;
    char pad39[3];
} tDCFileCache;
typedef char dc_cache_header_stride[(sizeof(tDCFileCache)==0x3C)?1:-1];
