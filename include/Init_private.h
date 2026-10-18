#ifndef INIT_PRIVATE_H
#define INIT_PRIVATE_H
typedef struct DcInitCallbackArg {
    char opaque0[4];
    int value; /* +4: 8C039336 mov.l r3,@(4,r4). */
} DcInitCallbackArg;
extern unsigned int gInitHeapAlias; /* 8C0D6F4C */
extern unsigned int gInitMemoryEnd; /* 8C115210 */
extern int gInitCableType; /* 8C1224C0 */
extern signed char osTvType; /* 8C1224C4:8C03936A,8C03959C. */
extern float gRetraceRate, gRetraceTime; /* Existing timer-owned D words. */
extern volatile unsigned int gFrameCounter; /* Timer-owned8C12253C; reload8C039758. */
extern char gInitGraphicsBufferA[],gInitGraphicsBufferB[]; /* 8C134540/8C140560; extent unknown. */
extern int gInitMonoSetting,gInitSdkSetting; /* 8C14C580/8C14C584 */
extern short gInitLanguageMode; /* 8C15C988:8C0394F4 mov.w. */
extern short gInitExitFlag; /* 8C14CBAC:8C039724 mov.w. */
extern char gInitVideoDescriptor[]; /* 8C19E970; opaque SDK-owned descriptor. */

extern void func_8C0D98F8(void);
extern void func_8C09B1E4(void *,unsigned int);
extern void func_8C0DAE8E(int);
extern void func_8C0DAFE0(int,int,int,int);
extern void func_8C0DC260(int);
extern void func_8C0D9922(void);
extern void func_8C0D7290(int,void *,void *);
extern void func_8C0D7E5C(void);
extern void func_8C0D7E86(void);
extern void func_8C0D77B0(void);
extern void func_8C09B240(void);
extern void func_8C0D9932(void);
extern void func_8C02E350(void);
extern void func_8C02E390(void);
extern void *func_8C09B2B4(unsigned int);
extern void func_8C0D84B8(void *);
extern void func_8C0D850A(int *);
extern void func_8C0D863A(int *);
extern void func_8C0D8558(int *);
extern void func_8C0D8610(int *);
extern void func_8C0D8588(int *);
extern void func_8C0D84CA(void);
extern void func_8C09B380(void *);
extern void func_8C0D8974(int);
extern int func_8C0D7E0A(void);
extern void func_8C0D8458(void);
extern void func_8C06C664(void *,int,int,int,int);
extern void func_8C0DAD60(void (*)(DcInitCallbackArg *),int);
extern int func_8C072690(int,int);
extern void InitGraphics(void);
extern void InitMemory(unsigned short,unsigned short);
extern void SaveInit(void);
extern void HAL_InitText(void);
extern void func_8C06B82C(void);
extern void *func_8C06BA3A(const char *,int,unsigned int *,int,int);
extern void DeallocateMemory(void *);
extern void AudioInit(unsigned char,unsigned char,unsigned char,int,int,float,float,int);
extern void AudMono(int);
extern void InitRushControls(void);
extern void SetControllerActive(int,int);
extern void InitOptions(void);
extern void InitializeTimer(void);
extern void func_8C06C712(void);
extern void func_8C06C4A6(void);
extern void AudioShutdown(void);
extern void func_8C06E7BC(void);
extern int func_8C06E5B4(void);
extern void func_8C06B954(void);
extern void func_8C06ED90(void);

void func_8C039280(int,int,int);
void func_8C039308(void);
void func_8C039332(DcInitCallbackArg *);
void Pal60(int);
void func_8C03943C(void);
void func_8C03954A(void);
void func_8C039718(void);
#endif
