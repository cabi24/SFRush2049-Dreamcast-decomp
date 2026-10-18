#include "dc_types.h"
#ifndef DC_WORLD_TYPES_H
#define DC_WORLD_TYPES_H
/* Private target-backed world layouts. */

typedef struct tModelPart {
    char name[16]; /* +0 passed as MBOX name8C065CC8; matrix+0x10 in8C065CE6 */
    tUVMatrix mat; /* +0x10 */
    unsigned int dlflags; /* +0x40:8C065CD6/DA and8C065CEC/F0 */
    signed short next; /* +0x44:8C065D3A/3C; signed test8C065D40 */
    signed short child; /* +0x46:8C065D62/64/66 */
    char pad0[0x50-0x48];
    float min[3]; /* +0x50:8C065D00/04,8C065E5C..98 */
    float max[3]; /* +0x5c:8C065CFE,8C065E7E; max[2]+0x64:8C065E9A..A0 */
} tModelPart; /* size0x68:8C065D44/48,8C065D6C/72 signed16 index multiplication */

typedef char dc_check_world_matrix[(sizeof(tUVMatrix)==0x30)?1:-1];
typedef char dc_check_world_part[(sizeof(tModelPart)==0x68)?1:-1];
typedef char dc_check_world_header[(sizeof(anon_woff_wname_14)==0x14)?1:-1];
#endif
