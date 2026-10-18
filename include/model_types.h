#include "dc_types.h"
/* Private DC model overlays; offsets and strides proven by this unit. */
#ifndef MODEL_TYPES_H
#define MODEL_TYPES_H





typedef struct tLink {
    char pad0[8];
    unsigned char owner; /* +8:8C040C7C/7E byte then extu.b. */
    char pad1[0x0a-9]; /* strideA:8C040C8E; six end+3C at8C040C74. */
} tLink;

typedef char dc_check_model_size[(sizeof(tModelData)==0x808)?1:-1];
typedef char dc_check_gcar_size[(sizeof(tCarData)==0x3cc)?1:-1];
typedef char dc_check_link_size[(sizeof(tLink)==0x0a)?1:-1];

#endif
