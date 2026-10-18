#include "dc_types.h"
/* Dreamcast zoidpoly layouts, proven by 8C068F28..8C06929C. */
#ifndef ZOIDPOLY_TYPES_H
#define ZOIDPOLY_TYPES_H


               /* stride0x18: 8C06918A..9194 */




typedef struct tTexDefList {
    tTexture *texDefs;    /* +0, stride8: 8C069010..9016 */
    char pad0[8-4];
} tTexDefList;

typedef char dc_check_polyvert[(sizeof(tPolyVert)==0x18)?1:-1];
typedef char dc_check_poly[(sizeof(tPoly)==0x68)?1:-1];
typedef char dc_check_texture[(sizeof(tTexture)==0x30)?1:-1];
typedef char dc_check_texdeflist[(sizeof(tTexDefList)==8)?1:-1];
#endif
