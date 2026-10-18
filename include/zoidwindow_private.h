#include "dc_types.h"
/* DC window layout verified by stores/loads across 8C06940A..8C069E20. */
#ifndef ZOIDWINDOW_PRIVATE_H
#define ZOIDWINDOW_PRIVATE_H



             /* stride72, ScaleWindows 8C069590 add#72. */
extern float HFOV;
extern unsigned int gBgColor;
extern float gViewableDistance;
extern unsigned char gNumZoidWindows;
extern signed char gSplitScreen;
extern tWindowInfo ZoidWinInfo[4];
extern tScissor scissor0,scissor1,scissor2,scissorul,scissorur,scissorll,scissorlr;
extern signed char gMirrorWaves,gUpsideDownTrack;
float ftan(float arg);
float fatan(float arg);
void BodyVector(float *world,float *body,float (*uv)[3]);
void WorldVector(float *body,float *world,float (*uv)[3]);
void ZOID_ResetWindows(void);
tWindowInfo *ZOID_InitializeWindow(int idx,tViewport *vp,tScissor *s,float hang,float vang,float width,float height,float hcenter,float vcenter);
void ZOID_ScaleWindows(float sx,float sy);
tWindowInfo *ZOID_SetWindow(unsigned int idx,float hangle,float vangle,float width,float height,float hcenter,float vcenter);
tWindowInfo *ZOID_SetFog(signed short min,signed short max,unsigned char red,unsigned char green,unsigned char blue,unsigned char alpha,unsigned int idx);
tWindowInfo *ZOID_GetFog(signed short *min,signed short *max,unsigned char *red,unsigned char *green,unsigned char *blue,unsigned char *alpha,unsigned int idx);
tWindowInfo *ZOID_GetWindow(int idx);
tWindowInfo *ZOID_SetViewport(int idx,tViewport *vp,tScissor *s);
void ZOID_SetScissor(tScissor *s,unsigned short ulx,unsigned short uly,unsigned short lrx,unsigned short lry);
int ZOID_NumWindows(int num);
int ZOID_SplitScreen(int num);
int ZOID_SplitMenuScreen(int num);
void ZOID_SetBGColor(unsigned short color);
typedef char checkDCWindow[(sizeof(tWindowInfo)==0x48)?1:-1];
typedef char checkDCViewport[(sizeof(tViewport)==0x20)?1:-1];
typedef char checkDCScissor[(sizeof(tScissor)==8)?1:-1];
#endif
