/* Semantic MAT3 HAL_Support body; private frozen native SHC candidate. */
#include "HAL_Support_types.h"

unsigned char HAL_IntLog2(unsigned int num)
{
    unsigned char shift;

    for (shift = 31; shift; shift--) {
        if (num & (1 << shift))
            return shift;
    }
    return 0;
}
