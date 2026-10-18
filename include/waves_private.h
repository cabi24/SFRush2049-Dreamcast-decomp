#ifndef WAVES_PRIVATE_H
#define WAVES_PRIVATE_H
/* Private views proven by the waves unit; unresolved storage stays extern. */
typedef struct WaveInstance {
 signed char loaded,ready,locked,model;
 unsigned char mirrored;
 signed char handle;
 unsigned short wave;
 unsigned int size;
 unsigned char *ptr,*lastPtr;
} WaveInstance; /* 0x14: 8C06560E..61A; fields +0..10. */
typedef struct WaveTexture {
 char name[16];
 unsigned short width,height;
 char opaque14[4];
 unsigned char *data; /* +18: 8C0659F0..9F8. */
 unsigned int opaque1C;
 unsigned int xflags; /* +20: 8C065402,8C065A0E. */
 unsigned int opaque24;
 unsigned int cached28,cached2C; /* 8C065A3A..9C reuse fields. */
} WaveTexture; /* 0x30 stride 8C0659E0..9EA. */
typedef struct WaveLod {
 unsigned short texidx,flags;
 float radius;
 unsigned int *displayList;
} WaveLod;
typedef struct WaveModel {
 char name[16]; float radius; short unused,lodCount;
 WaveLod lod[4];
} WaveModel; /* 0x48 stride: 8C065B1E..B2A; lodCount +16, lod +18. */
typedef struct WavePalette {
 char opaque0[20]; unsigned char *data;
} WavePalette; /* 0x18, data +14: 8C065AE8..B00. */
typedef struct WavePath {
 char opaque0[24]; unsigned char *trigger,*points; unsigned int opaque20;
} WavePath; /* 0x24: 8C065BBC..BF0; pointers +18,+1C. */
typedef struct WaveModelTable { WaveModel *romObjDefs; int count; } WaveModelTable;
typedef struct WaveTextureTable { WaveTexture *texDefs; int count; } WaveTextureTable;
typedef struct WavePaletteTable { WavePalette *palDefs; int count; } WavePaletteTable;
typedef struct WavePathTable { WavePath *paths; int count; } WavePathTable;
typedef struct WaveWad { unsigned int *base; int count; void *toc; } WaveWad;
extern WaveInstance gWaveInstance[]; /* 8C15FA20, 64 accessed; allocation end unclaimed. */
extern volatile unsigned char gNumModels; /* 8C14CB1C: repeated address reload/store 8C06511A..5190. */
extern volatile signed char gRenderInProgress; /* Busy waits reload: 8C064FE8..4FEE,8C0653CA..3D0. */
extern signed char gMirrorWaves;
extern WaveModelTable gRomObjDefPtrs[];
extern WaveTextureTable gTexDefPtrs[];
extern WavePaletteTable gPalDefPtrs[];
extern WavePathTable gTargetPaths;
extern unsigned int gWaveSizes[];
extern int gWaveCompressed[]; /* 8C0B04C8: 8C065782. */
extern char *gWaveFilenames[]; /* 8C0C44C0: 8C0657B0. */
extern void ZOID_Reset(),InitBlits(),InitTextWave();
extern void func_8C03FFBC(int);
extern void *AllocateHandleHeap(void *,unsigned int);
extern void *AllocateMemoryHeap(void *,unsigned int);
extern unsigned int GetPtrSize(void *);
extern void DeallocateMemory(void *),ReallocateMemory(void *,unsigned int);
extern void PurgeableMemory(void *,int);
extern void *memset(void *,int,unsigned int);
extern void func_8C06EAD0(WaveTexture *);
extern void func_8C06E95C(WaveTexture *);
extern void *func_8C06BA3A(const char *,void *,unsigned int *,int,int);
extern unsigned int func_8C0102A4(void *,void *);
extern void MBSetupWad(WaveWad *,unsigned int *);
extern void MBOffsetWad(WaveWad *,unsigned int *,int);
extern void *MBGetFromWad(WaveWad *,unsigned int,int *);
extern void func_8C026104(int,int,unsigned int *);
void InitWaves(void);
int LoadWave(int,int,int,int,void *);
void ReplaceWave(int,int);
int WaveInstanceReady(int);
void *GetWaveInstanceAddr(int);
void UnloadWaveAll(int);
void UnloadWaveInstance(int,int);
int _NewWaveInstance(int,int,void *);
void _ReplaceWaveInstance(int,int),_SetWaveInstanceModel(int);
void LockWaveInstance(int),UnlockWaveInstance(int);
int ModelActive(int),FindWaveInstance(int),_FindWaveInstance(int,signed char);
void _ValidateWaveInstance(int,int,int),_ReadWave(int);
void func_8C06585E(void),func_8C065862(int),func_8C06588C(int,int,int),func_8C065C1A(void);
void MirrorWaveInstance(int);
#endif
