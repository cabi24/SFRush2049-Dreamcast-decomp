/* Init.c: DC SDK start-up, video selection and exit sequencing.
 * MAT3 supplies Pal60/startup roles; DC-only entries retain their map labels.
 */
#include "..\include\Init_private.h"

static const char gInitAudioDriverName[]="audio64.drv";

void func_8C039280(int mode,int color,int options)
{
    char *start;
    gInitHeapAlias|=0xA0000000;
    _builtin_set_imask(15);
    func_8C0D98F8();
    func_8C09B1E4((start=(char *)((gInitMemoryEnd|0x80000000)&~31))+32,
        ((gInitMemoryEnd&0xE0000000)|0x0D000000)-(unsigned int)((gInitMemoryEnd|0x80000000)&~31)-32);
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
    unsigned int *driverSize=&size;
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
    driver=func_8C06BA3A(gInitAudioDriverName,0,driverSize,0,0);
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
