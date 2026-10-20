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


void func_8C06C524(void);
void func_8C06C4A6(void);
void func_8C06CA7C(void);
int func_8C06C986(int);
void func_8C06CA6A(void);
int func_8C06CA0A(int);

void func_8C06C08C(unsigned char *dest,unsigned char *source,int width,int rows,int stride,int index)
{
    int step=width*stride;
    source+=width*index;
    while (rows--) {
        memcpy(dest,source,width);
        dest+=48;
        source+=step;
    }
}

int func_8C06C0DC(unsigned char *dest,int number)
{
    int leading;
    int width=0;
    int digit;
    if (number>999) number=999;
    else if (number<0) number=0;
    if (number>99) {
        digit=number/100;
        number-=digit*100;
        func_8C06C08C(dest,dcLcdDigits,3,5,10,digit);
        width=4;
        dest+=4;
        leading=1;
    } else leading=0;
    if (leading || number>9) {
        digit=number/10;
        number-=digit*10;
        func_8C06C08C(dest,dcLcdDigits,3,5,10,digit);
        width+=4;
        dest+=4;
    }
    func_8C06C08C(dest,dcLcdDigits,3,5,10,number);
    width+=4;
    return width;
}

void func_8C06C18C(unsigned char *dest,int width,int rows,float value)
{
    int pixels=(int)(.5f+value*(float)width);
    while (rows--) {
        memset(dest,8,pixels);
        dest+=48;
    }
}

/* C1F0..C1FC spills FP arguments before integer arguments; register banks are independent. */
void func_8C06C1E6(unsigned char *dest,float barA,float barB,int valueA,int valueB)
{
    int pos;
    func_8C06C08C(dest+2,dcLcdIcons,7,6,2,0);
    func_8C06C18C(dest+10,36,6,barA);
    pos=2;
    pos+=func_8C06C0DC(dest+0x1E0+pos,valueA)+4;
    func_8C06C08C(dest+0x1E0+pos,dcLcdLabelA,28,5,1,0);
    func_8C06C08C(dest+0x332,dcLcdLabelB,24,5,1,0);
    pos=30;
    pos+=func_8C06C0DC(dest+0x330+pos,valueB)+1;
    func_8C06C08C(dest+0x330+pos,dcLcdSmallIcon,4,4,1,0);
    func_8C06C08C(dest+0x4E2,dcLcdIcons,7,6,2,1);
    func_8C06C18C(dest+0x4EA,36,6,barB);
}

void func_8C06C2A6(void) {func_8C0D6D7C();func_8C06C524();}
void func_8C06C2B2(void) {func_8C06C4A6();func_8C0D6DDA();}

void func_8C06C2BE(int player,float left,float right,int a,int b)
{
    unsigned char frame[1536];
    int unit=dcController[player].port*6+1;
    if (!(left>0.0f)) left=0.0f;else if (!(left<1.0f))left=1.0f;
    if (!(right>0.0f)) right=0.0f;else if (!(right<1.0f))right=1.0f;
    memset(frame,0,sizeof(frame));
    func_8C06C1E6(frame,left,right,a,b);
    func_8C0D6DEC(unit,frame,3);
}

void func_8C06C374(int player)
{
    unsigned char frame[1536];
    int unit=dcController[player].port*6+1;
    memset(frame,0,sizeof(frame));
    func_8C0D6DEC(unit,frame,3);
}

void func_8C06C3D0(void)
{
    unsigned char frame[1536];
    int port,slot,unit;
    for(port=0;port<4;port++) {
      for(slot=0;slot<2;slot++) {
        unit=port*6+slot+1;
        memset(frame,0,sizeof(frame));
        func_8C06C08C(frame+150,dcLcdLargeDigits,18,26,10,port);
        func_8C06C08C(frame+170,dcLcdLargeDigits,18,26,10,slot+4);
        func_8C0D6DEC(unit,frame,3);
      }
    }
}

void func_8C06C4A6(void)
{
    unsigned char frame[1536];
    int port,slot;
    unsigned int unit;
    memset(frame,0,sizeof(frame));
    for(port=0;port<4;port++)for(slot=0;slot<2;slot++){unit=port*6+slot+1;func_8C0D6DEC(unit,frame,3);}
}

void func_8C06C524(void)
{
    unsigned char frame[1536];
    int port,slot;
    unsigned int unit;
    memcpy(frame,dcLcdSplash,sizeof(frame));
    for(port=0;port<24;port+=6)for(slot=0;slot<2;slot++){unit=port+slot+1;func_8C0D6DEC(unit,frame,3);}
}

void func_8C06C58C(int index)
{
    unsigned char frame[1536];
    int port,slot;
    unsigned int unit;
    memcpy(frame,dcLcdSplash,sizeof(frame));
    port=index/2;
    slot=index%2;
    unit=port*6+slot+1;
    func_8C0D6DEC(unit,frame,3);
}

/* Target-proven empty callback C5E2: rts;nop, not an unmatched placeholder. */
void func_8C06C5E2(void) {}

void func_8C06C5E6(int port,int slot,int enabled)
{
    unsigned char frame[1536];
    int unit=port*6+slot+1;
    memset(frame,0,sizeof(frame));
    func_8C06C08C(frame+160,dcLcdLargeDigits,18,26,10,enabled?9:8);
    func_8C0D6DEC(unit,frame,3);
}
