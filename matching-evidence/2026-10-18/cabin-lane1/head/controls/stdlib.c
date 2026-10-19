#include "stdlib_types.h"

/* Actual state address from047C00;target initial word075BCD15. */
static unsigned int stdlib_seed = 123456789u;

int rand(void)
{
    stdlib_seed = stdlib_seed * 1103515245u + 12345u;
    return ((int)stdlib_seed >> 16) & 0x7fff;
}

unsigned int rand_r(unsigned int *seed)
{
    *seed = *seed * 1103515245u + 12345u;
    return (*seed >> 16) & 0x7fff;
}
