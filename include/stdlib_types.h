#ifndef STDLIB_TYPES_H
#define STDLIB_TYPES_H
/* Target mul.l/load/store and16-bit extraction;no GC object layouts. */
typedef char stdlib_uint32[(sizeof(unsigned int)==4)?1:-1];
typedef char stdlib_short16[(sizeof(short)==2)?1:-1];
int rand(void); /* fmath Random caller027138/2715C. */
unsigned int rand_r(unsigned int *seed); /* DC read/writeR4;GC shim ignoresit. */
#endif
