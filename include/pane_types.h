/* DC pane overlays; addresses prove every named field. */
#ifndef PANE_TYPES_H
#define PANE_TYPES_H
typedef struct tPaneBlit _tBlit_2;
typedef _tBlit_2 _tBlit;
struct tPaneBlit {
    char pad0[4];
    void *palette;           /* +4: InitPane8C0447A4 */
    char pad1[0x0e-8];
    signed short x;          /* +0x0e: ChangePaneSize8C044856 */
    signed short y;          /* +0x10: ChangePaneSize8C044862 */
    char pad2[0x14-0x12];
    signed short width;      /* +0x14: ChangePaneSize8C04486E */
    signed short height;     /* +0x16: ChangePaneSize8C04487A */
    unsigned char alpha;     /* +0x18: InitPane8C04478C/4798 */
    char pad3[0x28-0x19];
    int (*animFunc)(_tBlit_2 *); /* +0x28: InitPane8C0447B0 */
    unsigned int animID;     /* +0x2c: InitPane8C0447BC */
    int animDTA;             /* +0x30: InitPane8C0447C4 */
};
typedef struct tBlitSet {
    signed char used;        /* +0: InitPane8C044722/476C */
    signed char perm;        /* +1: InitPane8C04476A */
    unsigned char blackColor[4]; /* +2: InitPane8C04474A..4756 */
    unsigned char grayColor[4];  /* +6: InitPane8C04475A..4766 */
    char pad0[0x0c-0x0a];
    _tBlit_2 *blits[5];      /* +0xc: InitPane8C044770; stride0x20:472E,4BA8 */
} tBlitSet;
typedef char dc_check_blitset[(sizeof(tBlitSet)==0x20)?1:-1];
typedef char dc_check_pane_blit_overlay[(sizeof(_tBlit_2)==0x34)?1:-1];
#endif
