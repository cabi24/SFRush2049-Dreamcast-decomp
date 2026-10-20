/* Target LCD/peripheral helpers; original anonymous identities retained. */
#include "dc_peripheral_types.h"
extern void *memcpy(void *,const void *,unsigned int);
extern void *memset(void *,int,unsigned int);
extern unsigned char dcLcdSplash[];
extern unsigned char dcLcdDigits[];
extern unsigned char dcLcdIcons[];
extern unsigned char dcLcdSmallIcon[];
extern unsigned char dcLcdLabelA[];
extern unsigned char dcLcdLabelB[];
extern unsigned char dcLcdLargeDigits[];
#define dcController ((tDCControllerPortPrefix *)0x8C006000)
extern void func_8C0D6D7C(void);
extern void func_8C0D6DDA(void);
extern void func_8C0D6DEC(int,void *,int);
extern void func_8C0D4764(void);
extern void func_8C06ED90(void);
extern void func_8C0D448A(void *);
extern void func_8C0D26EA(int);
extern void func_8C0D262E(void (*)(void),int);
extern void func_8C0D461A(void);
extern void *func_8C0D6BDE(const char *,int);
extern int func_8C0D6368(void *,int *);
extern void func_8C06ED2A(void);
extern void func_8C0D3E70(void);
extern void func_8C0D2628(void);
extern void func_8C0D3ECC(void);
extern unsigned int func_8C0D4952(tDCPeripheralConfig *);
extern void *AllocateMemoryHeap(int,unsigned int);
extern void DeallocateMemory(void *);
extern tDCPeripheralObject *func_8C0D2A40(tDCPeripheralConfig *);
extern void func_8C0D437C(tDCPeripheralObject *,int);
extern void func_8C0D265C(void);
extern int func_8C0A3BF2(void);
extern void func_8C0DAA98(int);


int dcPeripheralSystemStatus;

void func_8C06C524(void);
void func_8C06C4A6(void);
void func_8C06CA7C(void);
int func_8C06C986(int);
void func_8C06CA6A(void);
int func_8C06CA0A(int);

void func_8C06CA98(void) {dcPeripheralSystemStatus=func_8C0A3BF2();}
int func_8C06CAA8(void) {return dcPeripheralSystemStatus!=-3;}
void func_8C06CAB4(void) {signed char enabled=dcPeripheralSystemStatus!=-3;if(enabled)func_8C0DAA98(enabled);}
