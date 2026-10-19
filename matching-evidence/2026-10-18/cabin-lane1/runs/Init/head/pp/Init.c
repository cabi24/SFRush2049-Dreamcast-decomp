/* Init.c: DC SDK start-up, video selection and exit sequencing.
 * MAT3 supplies Pal60/startup roles; DC-only entries retain their map labels.
 */
#line 1 "Init_private.h"
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
#line 5 "Init.c"

static const char gInitAudioDriverName[]="audio64.drv";

void func_8C039280(int mode,int color,int options)
{
    char *start;
    gInitHeapAlias|=0xA0000000;
    _builtin_set_imask(15);
    func_8C0D98F8();
    start=(char *)((gInitMemoryEnd|0x80000000)&~31);
    func_8C09B1E4(start+32,((gInitMemoryEnd&0xE0000000)|0x0D000000)-(unsigned int)start-32);
    func_8C0DAE8E(0);
    func_8C0DAFE0(mode,color,1,0);
    func_8C0DC260(options);
    func_8C0D9922();
    func_8C0D7290(0,gInitGraphicsBufferA,gInitGraphicsBufferB);
    func_8C0D7E5C();
    _builtin_set_imask(0);
}
void func_8C039308(void)
{
    func_8C0D7E86();
    func_8C0D77B0();
    func_8C09B240();
    func_8C0D9932();
    _builtin_set_imask(15);
}
void func_8C039332(DcInitCallbackArg *arg)
{
    arg->value=4;
}
void Pal60(int on)
{
    if(on) {
        func_8C02E390();func_8C02E350();
        func_8C02E390();func_8C02E350();
        func_8C0DAFE0(56,0,1,0);
        osTvType=1;
        gRetraceRate=60.0f;
        gRetraceTime=0.016666668f;
    } else {
        func_8C0DAFE0(122,0,1,0);
        func_8C02E390();func_8C02E350();
        func_8C02E390();func_8C02E350();
        osTvType=0;
        gRetraceRate=50.0f;
        gRetraceTime=0.020000001f;
    }
}

void func_8C03943C(void)
{
    void *memory;
    int country;
    int language;
    int supported;
    memory=func_8C09B2B4(0x4000);
    func_8C0D84B8(memory);
    func_8C0D850A(&gInitMonoSetting);
    func_8C0D863A(&gInitSdkSetting);
    func_8C0D8558(&supported);
    func_8C0D8610(&country);
    if(supported!='1' || country=='1' ||
       (country=='2' && osTvType!=1 && osTvType!=3) ||
       (country=='3' && osTvType!=0 && osTvType!=3) ||
       (country=='0' && osTvType!=1 && osTvType!=3)) {
        func_8C039718();
    }
    func_8C0D8588(&language);
    switch(language) {
    case 4: case '4':gInitLanguageMode=1;break;
    case '5': case 5:gInitLanguageMode=2;break;
    case 2: case '2':gInitLanguageMode=3;break;
    case 3: case '3':gInitLanguageMode=5;break;
    default:gInitLanguageMode=0;break;
    }
    func_8C0D84CA();
    func_8C09B380(memory);
}

void func_8C03954A(void)
{
    int mode;
    int color;
    int options;
    volatile int flags;
    unsigned int size;
    void *driver;
    func_8C0D8974(2);
    flags=2;
    gInitCableType=func_8C0D7E0A();
    switch(gInitCableType) {
    case 1:
        osTvType=3;gRetraceRate=60.0f;gRetraceTime=0.016666668f;mode=49;color=0;options=1;
        break;
    case 2:
        osTvType=0;gRetraceRate=50.0f;gRetraceTime=0.020000001f;mode=122;color=0;options=1;
        break;
    case 0:
        mode=56;osTvType=1;gRetraceRate=60.0f;gRetraceTime=0.016666668f;color=0;options=1;
        break;
    default:
        func_8C0D8458();
        goto platform;
    }
platform:
    func_8C06C664(gInitVideoDescriptor,mode,color,options,flags);
    func_8C039280(mode,color,options);
    if(!osTvType) {
        func_8C0DAD60(func_8C039332,0);
        func_8C0DAFE0(mode,color,1,0);
    }
    func_8C03943C();
    if(!func_8C072690(28,48)) { }
    InitGraphics();
    InitMemory(1000,500);
    SaveInit();
    HAL_InitText();
    func_8C06B82C();
    driver=func_8C06BA3A(gInitAudioDriverName,0,&size,0,0);
    DeallocateMemory(driver);
    AudioInit(64,64,1,5,64,1.0f,200.0f,1);
    AudMono(gInitMonoSetting);
    InitRushControls();
    SetControllerActive(-1,0);
    InitOptions();
    InitializeTimer();
}

void func_8C039718(void)
{
    unsigned int frame;
    gInitExitFlag=0;
    func_8C06C712();
    func_8C06C4A6();
    AudioShutdown();
    func_8C06E7BC();
    while(!func_8C06E5B4()) { }
    func_8C06B954();
    if(gFrameCounter) {
        frame=gFrameCounter;
        while(gFrameCounter<frame+60) { }
    }
    func_8C039308();
    func_8C06ED90();
    func_8C0D8458();
}
