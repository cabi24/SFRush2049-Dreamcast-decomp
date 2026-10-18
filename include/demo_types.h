#ifndef DC_DEMO_TYPES_H
#define DC_DEMO_TYPES_H

/* DC endDemoGame: model stride pool 8C022C72 = 0x808;
   slot offset pool 8C022C74 = 0x7c6, mov.w at 8C022BD4. */
typedef struct tDemoModel {
    char pad0[0x7c6];
    signed short slot;
    char pad1[0x808 - 0x7c8];
} tDemoModel;

/* endDemoGame 8C022C18..238: index*5*16, byte store at +0. */
typedef struct tDemoPadState {
    unsigned char slot;
    char pad0[0x50 - 1];
} tDemoPadState;

/* Const definition proven by target 8C0AA8EC..8C0AA910; callback at +0x1c. */
typedef struct tDemoBlitDef {
    const char *texname;
    signed short dulx, duly, width, height;
    signed short top, bot, left, right;
    unsigned int zdepth;
    unsigned int alpha;
    int (*animfunc)(void);
    unsigned int animid;
} tDemoBlitDef;

typedef char dc_check_demo_model[(sizeof(tDemoModel) == 0x808) ? 1 : -1];
typedef char dc_check_demo_pad[(sizeof(tDemoPadState) == 0x50) ? 1 : -1];
typedef char dc_check_demo_blitdef[(sizeof(tDemoBlitDef) == 0x24) ? 1 : -1];

#endif
