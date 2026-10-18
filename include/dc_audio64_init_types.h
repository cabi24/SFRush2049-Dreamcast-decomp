/* Private target-backed Audio64 SDK error prefix: observed flag+0/error+100.
 * This0x104-byte view proves no SDK allocation end. */
typedef struct tDCAudioErrorPrefix {
    signed char flag;
    char pad1[0x100-1];
    unsigned int error;
} tDCAudioErrorPrefix;
typedef char dc_audio_error_prefix_check[(sizeof(tDCAudioErrorPrefix)==0x104)?1:-1];
