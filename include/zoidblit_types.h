/* DC zoidblit instance layout; differs from MAT3 at flags/hide. */
#ifndef ZOIDBLIT_TYPES_H
#define ZOIDBLIT_TYPES_H
typedef struct tTexture tTexture;
typedef struct anon_name_start_18 anon_name_start_18;
typedef struct anon_palette_texture_20 {
    anon_name_start_18 *palette; /* +0: Update8C068D3A */
    tTexture *texture;          /* +4: Update8C068D38 */
    unsigned short texIndex;  /* +8: Update8C068D3C */
    signed short x;           /* +0xa: Update8C068D40 */
    signed short y;           /* +0xc: Update8C068D44 */
    unsigned short z;         /* +0xe: SetData8C068DC2 */
    signed short width;       /* +0x10: Update8C068D48 */
    signed short height;      /* +0x12: Update8C068D4E */
    unsigned char alpha;      /* +0x14: SetData8C068DB4/DB6 */
    unsigned char flags;      /* +0x15: Flip8C068EA2..EAA; DC byte, MAT3u16+0x16 */
    signed char hide;         /* +0x16: SetData8C068D9A/D9E; DC+0x16, MAT3+0x15 */
    char pad0[0x18-0x17];
    signed short top;         /* +0x18: Crop8C068D68 */
    signed short left;        /* +0x1a: Crop8C068D74 */
    signed short bot;         /* +0x1c: Crop8C068D7E */
    signed short right;       /* +0x1e: Crop8C068D90 */
} anon_palette_texture_20;    /* stride32: Update8C068D2C..D36 */
typedef char dc_check_blitinstance[(sizeof(anon_palette_texture_20)==0x20)?1:-1];
#endif
