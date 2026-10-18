#include "dc_types.h"
/* DC MB wad layouts, proven by 8C03CF94..8C03D090. */
#ifndef DC_MB_WAD_TYPES_H
#define DC_MB_WAD_TYPES_H
        /* stride12: MBFind8C03D028..D034 */

typedef char dc_check_wadentry[(sizeof(anon_tag_address_c)==12)?1:-1];
typedef char dc_check_wad[(sizeof(anon_base_TOCentries_c)==12)?1:-1];
#endif
