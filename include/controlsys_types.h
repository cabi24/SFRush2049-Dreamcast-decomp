#include "dc_types.h"
#ifndef CONTROLSYS_TYPES_H
#define CONTROLSYS_TYPES_H
/* DC aggregate: Init zeros80 bytes atEE08, all indexed accesses use stride20. */

/* SDK peripheral overlay from F28A/F33C/F438..F48E and F328. */
typedef struct tDCPadInfo {
 unsigned int type; /* F32A */
 char pad0[0x14-4];
 char product_name[1]; /* F384 +20; no allocation extent inferred */
} tDCPadInfo;
typedef struct tDCPad {
 char pad0[4];
 unsigned int support,on; /* +4:F28A;+8:F3AC */
 char pad1[0x18-0x0c];
 unsigned short r,l; /* +18/+1a:F480/F488 */
 signed short x1,y1,x2,y2; /* +1c/+1e/+20/+22:F438..F45A */
 char pad2[0x30-0x24];
 tDCPadInfo *info; /* +30:F328 */
} tDCPad;
typedef char dc_check_control_info[(sizeof(tPadInfo)==0x14)?1:-1];
typedef char dc_check_sdk_pad_overlay[(sizeof(tDCPad)==0x34)?1:-1];
extern volatile unsigned int gFrameCounter;
extern float gRetraceRate,gRetraceTime;
int SetInterruptMask(int level);
void RegisterRetraceCallback(int kind,void(*callback)(int),int a,int b);
void ModelThread(int arg);
void *memset(void *dest,int value,unsigned int size);
unsigned int strlen(const char *str);
int strncmp(const char *a,const char *b,unsigned int n);
void sdk_func_8C0D7686(void);
tDCPad *sdk_func_8C0D778C(int port);
void InitControlSys(void);
void UpdateControls(void);
void SetControllerActive(int which,int active);
void SetControllerStickEdges(int which,int active);
void ControlIRQ(void);
float _PotRange(int pot,float range,float max);
#endif
