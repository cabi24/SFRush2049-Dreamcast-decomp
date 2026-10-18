#include "dc_types.h"
/* Private state.c proposals, based on DC target instructions. */
#ifndef DC_STATE_TYPES_H
#define DC_STATE_TYPES_H
typedef struct tPS_File tPS_File;
typedef struct tPlayerProfile {
    char pad0[8];
    tPS_File **fileh; /* +8: 8C050EF2 mov.l @(8,r4),r4. */
} tPlayerProfile;
typedef struct tPadState {
    char pad0[0x4c];
    tPlayerProfile **save; /* +4C 8C050EE0..EE2; stride50 8C050EF6/F02. */
} tPadState;
typedef struct anon_name_first_24 {
    char pad0[0x24]; /* stride24: bank1 at 8C0511AA; bank6 offsetD8 at 8C0511BC. */
} anon_name_first_24;

typedef char dc_check_state_player[(sizeof(tPadState)==0x50)?1:-1];
typedef char dc_check_state_bank[(sizeof(anon_name_first_24)==0x24)?1:-1];
#endif
