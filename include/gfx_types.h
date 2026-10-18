#ifndef GFX_TYPES_H
#define GFX_TYPES_H
/* Private partial graphics config. Member names describe offsets, not an unverified SDK typedef. */
typedef struct tGfxConfig {
    unsigned int size, flags; /* +0=1F8,+4=11:8C02E1D0..DE */
    void **tables; /* +8:8C02E1E2 */
    unsigned int count; /* +C=2:8C02E1E6 */
    char gap10[4];
    unsigned int v14,v18,v1c; /* 8C02E1EA..F2 */
    void *p20,*p24; /* 8C02E1F4..F6 */
    unsigned int v28;
    void *buffer; /* +2C:8C02E1FC..FE */
    unsigned int bufferSize,v34,v38; /* +30..38:8C02E208..20C */
    char gap3c[8];
    unsigned int v44; /* 8C02E20E */
    float f48,f4c,f50,f54,f58; /* 8C02E212..22C */
} tGfxConfig;
typedef union tGfxBorderColor { unsigned int value; unsigned char rgba[4]; } tGfxBorderColor;
extern signed char gRenderInProgress;
extern tGfxConfig dcGfxConfig;
extern unsigned char dcDevice[],dcTableA[],dcTableB[],dcBufferInfo[],dcPassInfo[],dcContextA[],dcContextB[];
extern void *dcVertexBuffer;
extern void *dcTables[2];
extern unsigned int dcFrameTick,dcPrevFrameTick,dcRenderTime,dcMaxRenderTime,dcFrameTime;
extern volatile unsigned int dcDuration; /* callback stores then reloads before convert at8C02E13C..142 */
extern float HFOV;
void dcVideoInit(int a,int b);
int dcInitDevice(void *p);
int dcSetBorderColor(tGfxBorderColor c);
void dcCheckKm(int result,char *name);
void *dcAlloc(unsigned int size);
int dcSetSystemConfiguration(tGfxConfig *p);
void dcRegisterA(void (*f)(void),int value);
void dcRegisterB(void (*f)(void),int value);
void dcRegisterC(void (*f)(void),int value);
void dcRegisterD(void (*f)(void),int value);
void dcCreateContext(void *p,unsigned int flags);
void dcContextSetup(void);
void *ZOID_InitializeWindow(int idx,void *vp,void *s,float hang,float vang,float width,float height,float hcenter,float vcenter);
unsigned int dcReadTick(void);
unsigned int dcTickDiff(unsigned int a,unsigned int b);
unsigned int dcConvertTick(unsigned int a);
void dcBeginSceneHook(void);
int dcKmBeginScene(tGfxConfig *p);
int dcKmBeginPass(void *p);
void dcSelectContext(int n);
void dcTextBegin(void);
void HAL_StartText(void);
void dcBeginSceneDone(void);
void dcTextEnd(void);
int dcKmEndPass(void *p);
void dcEndSceneMode(int mode);
int dcKmEndScene(tGfxConfig *p);
void ZOID_Init(void);
void InitBlits(void);
void InitCameras(void);
void DrawBlits(void);
void UpdateEnvirons(void);
void UpdateCameras(void);
void ZOID_EndFrame(int mode);
typedef char check_config[(sizeof(tGfxConfig)==0x5c)?1:-1];
#endif
