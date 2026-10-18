#include "dc_types.h"
/* Private state.c proposals, based on DC target instructions. */
#ifndef DC_STATE_TYPES_H
#define DC_STATE_TYPES_H



typedef struct anon_name_first_24 {
    char pad0[0x24]; /* stride24: bank1 at 8C0511AA; bank6 offsetD8 at 8C0511BC. */
} anon_name_first_24;

typedef char dc_check_state_player[(sizeof(tPadState)==0x50)?1:-1];
typedef char dc_check_state_bank[(sizeof(anon_name_first_24)==0x24)?1:-1];
#endif
