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

int dcPeripheralActive=0;
int dcPeripheralStateMap[5]={4,1,2,4,-1};
int dcPeripheralInit[5];
tDCPeripheral dcPeripheral[1];
int dcPeripheralSystemStatus;

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

void func_8C06C2BE(int player,int a,int b,float left,float right)
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
    int port,unit;
    signed short slot;
    for(port=0;port<4;port++) {
      unit=port*6+1;
      for(slot=0;slot<2;slot++) {
        memset(frame,0,sizeof(frame));
        func_8C06C08C(frame+150,dcLcdLargeDigits,18,26,10,port);
        func_8C06C08C(frame+170,dcLcdLargeDigits,18,26,10,slot+4);
        func_8C0D6DEC(unit++,frame,3);
      }
    }
}

void func_8C06C4A6(void)
{
    unsigned char frame[1536];
    int port,slot;
    memset(frame,0,sizeof(frame));
    for(port=0;port<24;port+=6)for(slot=0;slot<2;slot++)func_8C0D6DEC(port+slot+1,frame,3);
}

void func_8C06C524(void)
{
    unsigned char frame[1536];
    int port,slot;
    memcpy(frame,dcLcdSplash,sizeof(frame));
    for(port=0;port<24;port+=6)for(slot=0;slot<2;slot++)func_8C0D6DEC(port+slot+1,frame,3);
}

void func_8C06C58C(int index)
{
    unsigned char frame[1536];
    memcpy(frame,dcLcdSplash,sizeof(frame));
    func_8C0D6DEC(index/2*6+index%2+1,frame,3);
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

void func_8C06C664(int a,int b,int c,int d,int e)
{
    func_8C0D4764();
    dcPeripheralInit[0]=a;dcPeripheralInit[1]=b;dcPeripheralInit[2]=c;dcPeripheralInit[3]=d;dcPeripheralInit[4]=e;
}

void func_8C06C692(void)
{
    int params[8];
    tDCPeripheral *p;
    func_8C06ED90();
    memset(params,0,sizeof(params));
    params[0]=dcPeripheralInit[1];params[1]=dcPeripheralInit[2];params[2]=dcPeripheralInit[3];params[3]=dcPeripheralInit[4];
    func_8C0D448A(params);
    func_8C0D26EA(dcPeripheralInit[0]);
    func_8C0D262E(func_8C06CA7C,0);
    for(p=dcPeripheral;p<dcPeripheral+1;p++) {
        p->state=0;p->object=0;memset(&p->config,0,sizeof(p->config));
    }
    dcPeripheralActive=0;
}

void func_8C06C712(void)
{
    char name[15];
    int location;
    void *file;
    name[4]='A';name[12]='F';name[8]='G';name[1]='I';name[6]='L';name[0]='M';name[2]='D';name[7]='O';name[11]='S';name[3]='W';name[5]='Y';name[9]=name[7];name[13]=name[2];name[10]='.';name[14]=0;
    func_8C06CA6A();
    func_8C0D461A();
    file=func_8C0D6BDE(name,0);
    if(func_8C0D6368(file,&location)&&location>=400000)func_8C06ED2A();
}

void func_8C06C792(void) {if(dcPeripheralActive==1)func_8C0D3E70();}
void func_8C06C7A4(void) {if(dcPeripheralActive==1){func_8C0D2628();func_8C0D3ECC();}}

int func_8C06C808(void *context)
{
    int index;
    int mask;
    tDCPeripheral *p;
    for(index=0;index<1;index++)if(!dcPeripheral[index].object)break;
    if(index>=1)return-1;
    p=&dcPeripheral[index];
    memset(&p->config,0,sizeof(p->config));
    p->config.mode=1;p->config.field24=0;p->config.field28=0;
    p->config.fieldC=0x3E8000;p->config.width=320;p->config.height=240;p->config.format=3;
    p->config.bufferSize=func_8C0D4952(&p->config);
    p->config.buffer=AllocateMemoryHeap(0,p->config.bufferSize);
    if(!p->config.buffer){memset(&p->config,0,sizeof(p->config));return-1;}
    mask=_builtin_get_imask();
    _builtin_set_imask(15);
    p->object=func_8C0D2A40(&p->config);
    _builtin_set_imask(mask);
    if(!p->object){DeallocateMemory(p->config.buffer);memset(&p->config,0,sizeof(p->config));return-1;}
    func_8C0D437C(p->object,1);
    p->object->ops->start(p->object,context);
    func_8C06CA0A(index);
    dcPeripheralActive=1;
    return index;
}

int func_8C06C902(int index)
{
    dcPeripheral[index].object->ops->pause(dcPeripheral[index].object,1);
    dcPeripheral[index].state=3;
    return 3;
}

int func_8C06C95C(int index)
{
    dcPeripheral[index].object->ops->pause(dcPeripheral[index].object,0);
    return func_8C06CA0A(index);
}

int func_8C06C986(int index)
{
    tDCPeripheral *p,*q;
    if(index>=0 && index<1) {
        p=&dcPeripheral[index];
        if(p->state) {
            p->object->ops->stop(p->object);
            p->object->ops->release(p->object);
            DeallocateMemory(p->config.buffer);
        }
        p->state=0;p->object=0;memset(&p->config,0,sizeof(p->config));
        dcPeripheralActive=0;
        for(q=dcPeripheral;q<dcPeripheral+1;q++)if(q->state)dcPeripheralActive=1;
    }
    return 4;
}

int func_8C06CA0A(int index)
{
    tDCPeripheral *p=&dcPeripheral[index];
    if(p->object)p->state=dcPeripheralStateMap[p->object->ops->state(p->object)];else p->state=0;
    return p->state;
}

int func_8C06CA44(void)
{
    tDCPeripheral *p;
    int result=0;
    p=dcPeripheral;
    if(p<dcPeripheral+1) {
        do {
            if(p->object)result=1;
            p++;
        } while(p<dcPeripheral+1);
    }
    return result;
}
void func_8C06CA6A(void) {func_8C06C986(0);}
void func_8C06CA76(void) {func_8C0D265C();}
/* Target-proven empty SDKcallback CA7C: rts;nop. */
void func_8C06CA7C(void) {}
void func_8C06CA98(void) {dcPeripheralSystemStatus=func_8C0A3BF2();}
int func_8C06CAA8(void) {return dcPeripheralSystemStatus!=-3;}
void func_8C06CAB4(void) {signed char enabled=dcPeripheralSystemStatus!=-3;if(enabled)func_8C0DAA98(enabled);}
