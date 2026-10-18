#include "dc_types.h"
#ifndef DC_FRONT_TYPES_H
#define DC_FRONT_TYPES_H
/* Private front overlays. DC evidence from front text and emitted descriptor data. */
typedef tPolyVert dcFrontVertex;


typedef struct _tBlit _tBlit;
typedef struct _tBlit_2 _tBlit_2;

typedef struct tBlitDef {
    const char *texname; /* descriptor fields: 8C0AA91C..93C and 8C0AA940..960 */
    signed short dulx,duly,width,height,top,bot,left,right;
    unsigned int zdepth,alpha;
    int (*animfunc)(_tBlit *);
    unsigned int animid;
} tBlitDef;
typedef struct anon_minX_maxX_38 {
    float minX; /* +0: 8C0273E4 */
    float maxX; /* +4: 8C027396/3A0 */
    float minY; /* +8: 8C027440/444 */
    float maxY; /* +0xc: 8C0273B2/3BC */
    float delX; /* +0x10: 8C02766E/670 */
    float delY; /* +0x14: 8C02768E/690 */
    float offX[4]; /* +0x18: 8C02754A and 8C0277C8 */
    float offY[4]; /* +0x28: 8C02756A and 8C0277EA */
} anon_minX_maxX_38; /* stride0x38: 8C02738E; window stride0xE0: pool8C0274C4 */
typedef char dc_check_front_poly[(sizeof(tPoly)==0x68)?1:-1];
typedef char dc_check_front_vertex[(sizeof(dcFrontVertex)==0x18)?1:-1];
typedef char dc_check_front_rect[(sizeof(anon_minX_maxX_38)==0x38)?1:-1];
typedef char dc_check_front_blitdef[(sizeof(tBlitDef)==0x24)?1:-1];
#endif

