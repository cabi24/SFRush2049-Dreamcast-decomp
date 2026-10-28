/* DC graphics unit. Uncertain SDK roles retain address-backed private names. */
#include "gfx_types.h"
signed char gRenderInProgress;
void func_8C02E118(void) {}
void func_8C02E11C(void) {}
void func_8C02E120(void) {}
void func_8C02E124(void)
{
    unsigned int duration;
    gRenderInProgress = 0;
    duration = dcReadTick();
    dcDuration = dcTickDiff(dcFrameTick,duration);
    duration = dcConvertTick(dcDuration);
    if (duration > 40000) return;
    {
        dcDuration = dcRenderTime = duration;
        if (dcMaxRenderTime < dcRenderTime) dcMaxRenderTime = dcRenderTime;
    }
}
void func_8C02E18C(void)
{
    tGfxBorderColor border;
    int result;
    float *field48;
    dcVideoInit(0,0);
    dcInitDevice(dcDevice);
    border.value = 0;
    result = dcSetBorderColor(border);
    dcCheckKm(result,"kmSetBorderColor");
    dcVertexBuffer = (void *)((unsigned int)dcAlloc(0x180000) | 0xa0000000);
    dcTables[0] = dcTableA;
    dcTables[1] = dcTableB;
    dcGfxConfig.size = 0x1f8;
    dcGfxConfig.flags = 0x11;
    dcGfxConfig.tables = dcTables;
    dcGfxConfig.count = 2;
    dcGfxConfig.v14 = 0x440000;
    dcGfxConfig.v18 = 0x400;
    dcGfxConfig.v1c = 0;
    dcGfxConfig.p20 = dcBufferInfo;
    dcGfxConfig.p24 = dcPassInfo;
    dcGfxConfig.v28 = 1;
    dcGfxConfig.buffer = dcVertexBuffer;
    field48 = &dcGfxConfig.f48;
    dcGfxConfig.bufferSize = 0x180000;
    dcGfxConfig.v34 = 1;
    dcGfxConfig.v38 = 0;
    dcGfxConfig.v44 = 0;
    *field48 = 0;
    dcGfxConfig.f4c = 0;
    dcGfxConfig.f50 = 80.0f;
    dcGfxConfig.f54 = 0;
    dcGfxConfig.f58 = 20.0f;
    result = dcSetSystemConfiguration(&dcGfxConfig);
    dcCheckKm(result,"kmSetSystemConfiguration");
    dcRegisterA(func_8C02E118,0);
    dcRegisterB(func_8C02E11C,0);
    dcRegisterC(func_8C02E120,0);
    dcRegisterD(func_8C02E124,0);
    dcCreateContext(dcContextA,0x08000000);
    dcCreateContext(dcContextB,0x10000000);
    dcContextSetup();
    HFOV = 90.0f;
    ZOID_InitializeWindow(0,0,0,HFOV,0.0f,640.0f,480.0f,320.0f,224.0f);
}
void func_8C02E2A0(void) {}
void func_8C02E350(void)
{
    dcBeginSceneHook();
    dcCheckKm(dcKmBeginScene(&dcGfxConfig),"kmBeginScene");
    dcCheckKm(dcKmBeginPass(dcPassInfo),"kmBeginPass");
    dcSelectContext(-1);
    dcTextBegin();
    HAL_StartText();
    dcBeginSceneDone();
}
void func_8C02E390(void)
{
    dcTextEnd();
    dcCheckKm(dcKmEndPass(dcPassInfo),"kmEndPass");
    gRenderInProgress = 1;
    dcEndSceneMode(0);
    dcCheckKm(dcKmEndScene(&dcGfxConfig),"kmEndScene");
    dcPrevFrameTick = dcFrameTick;
    dcFrameTick = dcReadTick();
    dcFrameTime = dcConvertTick(dcTickDiff(dcPrevFrameTick,dcFrameTick));
}
void InitGraphics(void)
{
    func_8C02E18C();
    ZOID_Init();
    InitBlits();
    InitCameras();
}
void UpdateGraphics(int drawBlits,int updateEnvirons)
{
    if (drawBlits) DrawBlits();
    if (updateEnvirons) UpdateEnvirons();
    UpdateCameras();
    ZOID_EndFrame(0);
}
