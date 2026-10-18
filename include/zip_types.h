/* DC zlib layouts: target allocation16A8@8C067EA8; stream28-byte frame@8C067EA0;
 * state prefix offsets@8C067EC2..8C067F14, trees/descriptor stores@8C066624..664A,
 * heap/bit fields in pools8C066812..6818,8C0669C0..69CA,8C0676FA..7706.
 * ct_data stride4, ushort freq/code+0,len/dad+2 from tree access loops.
 * Initialized descriptor words at8C0C7B0C/7B20/7B34 prove pointer/int layout20.
 * Unused header bytes remain padding; no arbitrary backend types are imported. */
#ifndef ZIP_TYPES_H
#define ZIP_TYPES_H
typedef struct z_stream_s z_stream_s;
typedef struct internal_state internal_state;
typedef struct config_s {
    unsigned short good_length; /* 0x0 */
    unsigned short max_lazy; /* 0x2 */
    unsigned short nice_length; /* 0x4 */
    unsigned short max_chain; /* 0x6 */
} config_s;

/* size 0x2 */
typedef union anon_freq_code_2 {
    unsigned short freq; /* 0x0 */
    unsigned short code; /* 0x0 */
} anon_freq_code_2;

/* size 0x2 */
typedef union anon_dad_len_2 {
    unsigned short dad; /* 0x0 */
    unsigned short len; /* 0x0 */
} anon_dad_len_2;

/* size 0x4 */
typedef struct ct_data_s {
    anon_freq_code_2 fc; /* 0x0 */
    anon_dad_len_2 dl; /* 0x2 */
} ct_data_s;

/* size 0x14 */
typedef struct static_tree_desc_s {
    ct_data_s *static_tree; /* 0x0 */
    int *extra_bits; /* 0x4 */
    int extra_base; /* 0x8 */
    int elems; /* 0xc */
    int max_length; /* 0x10 */
} static_tree_desc_s;

/* size 0xc */
typedef struct tree_desc_s {
    ct_data_s *dyn_tree; /* 0x0 */
    int max_code; /* 0x4 */
    static_tree_desc_s *stat_desc; /* 0x8 */
} tree_desc_s;

/* size 0x16a8 */
struct internal_state {
    z_stream_s *strm; /* 0x0 */
    unsigned char *pending_buf; /* 0x4 */
    unsigned char *pending_out; /* 0x8 */
    int pending; /* 0xc */
    char pad10[4]; /* 0x10..0x14: no used field is proven */
    unsigned int w_size; /* 0x14 */
    unsigned int w_bits; /* 0x18 */
    unsigned int w_mask; /* 0x1c */
    unsigned char *window; /* 0x20 */
    unsigned int window_size; /* 0x24 */
    unsigned short *prev; /* 0x28 */
    unsigned short *head; /* 0x2c */
    unsigned int ins_h; /* 0x30 */
    unsigned int hash_size; /* 0x34 */
    unsigned int hash_bits; /* 0x38 */
    unsigned int hash_mask; /* 0x3c */
    unsigned int hash_shift; /* 0x40 */
    long block_start; /* 0x44 */
    unsigned int match_length; /* 0x48 */
    unsigned short prev_match; /* 0x4c */
    int match_available; /* 0x50 */
    unsigned int strstart; /* 0x54 */
    unsigned int match_start; /* 0x58 */
    unsigned int lookahead; /* 0x5c */
    unsigned int prev_length; /* 0x60 */
    unsigned int max_chain_length; /* 0x64 */
    unsigned int max_lazy_match; /* 0x68 */
    int level; /* 0x6c */
    unsigned int good_match; /* 0x70 */
    int nice_match; /* 0x74 */
    ct_data_s dyn_ltree[573]; /* 0x78 */
    ct_data_s dyn_dtree[61]; /* 0x96c */
    ct_data_s bl_tree[39]; /* 0xa60 */
    tree_desc_s l_desc; /* 0xafc */
    tree_desc_s d_desc; /* 0xb08 */
    tree_desc_s bl_desc; /* 0xb14 */
    unsigned short bl_count[16]; /* 0xb20 */
    int heap[573]; /* 0xb40 */
    int heap_len; /* 0x1434 */
    int heap_max; /* 0x1438 */
    unsigned char depth[573]; /* 0x143c */
    unsigned char *l_buf; /* 0x167c */
    unsigned int lit_bufsize; /* 0x1680 */
    unsigned int last_lit; /* 0x1684 */
    unsigned short *d_buf; /* 0x1688 */
    unsigned int opt_len; /* 0x168c */
    unsigned int static_len; /* 0x1690 */
    unsigned int compressed_len; /* 0x1694 */
    unsigned int matches; /* 0x1698 */
    int last_eob_len; /* 0x169c */
    unsigned short bi_buf; /* 0x16a0 */
    int bi_valid; /* 0x16a4 */
};

/* size 0x1c */
struct z_stream_s {
    unsigned char *next_in; /* 0x0 */
    unsigned int avail_in; /* 0x4 */
    unsigned int total_in; /* 0x8 */
    unsigned char *next_out; /* 0xc */
    unsigned int avail_out; /* 0x10 */
    unsigned int total_out; /* 0x14 */
    internal_state *state; /* 0x18 */
};
void *AllocateMemoryHeapHigh(void *,unsigned int);
void appFree(void *);
void *memcpy(void *,const void *,unsigned int);
void *memset(void *,int,unsigned int);
void _tr_init(internal_state *);
unsigned int _tr_flush_block(internal_state *,char *,unsigned int,int);
int _tr_tally(internal_state *,unsigned int,unsigned int);
int deflate_slow(internal_state *);
typedef char zip_check_ct_data[(sizeof(ct_data_s)==4)?1:-1];
typedef char zip_check_desc[(sizeof(static_tree_desc_s)==20)?1:-1];
typedef char zip_check_tree_desc[(sizeof(tree_desc_s)==12)?1:-1];
typedef char zip_check_internal[(sizeof(internal_state)==0x16a8)?1:-1];
typedef char zip_check_stream[(sizeof(z_stream_s)==28)?1:-1];
#endif
