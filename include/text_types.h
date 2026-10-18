#ifndef TEXT_TYPES_H
#define TEXT_TYPES_H
/* Dreamcast fields proven at 8C05B1F0..B246. MAT3 layout is retained. */
typedef struct tText {
    unsigned short numSingle; /* +0: B1F0/B1F4/B248 */
    unsigned char **single;   /* +4: B1F2/B252 */
    unsigned short numTab;    /* +8: B1FE/B202 */
    unsigned short numTabA;   /* +a: B210/B214 */
    unsigned short *tab;      /* +c: B200 */
    unsigned char **taba;     /* +10: B212/B272 */
    unsigned short numTab2;   /* +14: B21E/B222 */
    unsigned short numTab2a;  /* +16: B230/B234 */
    unsigned short numTab2b;  /* +18: B244/B288 */
    unsigned short *tab2;     /* +1c: B220 */
    unsigned short *tab2a;    /* +20: B232 */
    unsigned char **tab2b;    /* +24: B246/B292 */
} tText;
/* B1AA/B1AE and B1BE/B1C2 prove name starts +0x14. No full profile size claimed. */
typedef struct tTextPlayerName {
    char pad0[0x14];
    unsigned char name[1];
} tTextPlayerName;
typedef char dc_check_text_size[(sizeof(tText)==0x28)?1:-1];
extern unsigned char gLargestTextWave;
extern tTextPlayerName *gJustPlayers[4];
int LoadWave(int waveNum, int loadAgain, int unused, int handle, void *heap);
void LockWaveInstance(int idx);
void ReplaceWave(int curWave, int newWave);
void *GetWaveInstanceAddr(int idx);
int sprintf(char *dest, const char *format, ...);
void LoadTextWave(void);
#endif
