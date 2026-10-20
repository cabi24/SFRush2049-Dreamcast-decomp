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

void func_8C06C524(void);
void func_8C06C4A6(void);
void func_8C06CA7C(void);
int func_8C06C986(int);
void func_8C06CA6A(void);
int func_8C06CA0A(int);

void func_8C06C664(int a,int b,int c,int d,int e)
{
    func_8C0D4764();
    dcPeripheralInit[0]=a;dcPeripheralInit[1]=b;dcPeripheralInit[2]=c;dcPeripheralInit[3]=d;dcPeripheralInit[4]=e;
}

void func_8C06C692(void)
{
    struct {int a,b,c,d,e,f,g,h;} params;
    int index;
    tDCPeripheral *p;
    func_8C06ED90();
    memset(&params,0,sizeof(params));
    params.a=dcPeripheralInit[1];params.b=dcPeripheralInit[2];params.c=dcPeripheralInit[3];params.d=dcPeripheralInit[4];
    func_8C0D448A(&params);
    func_8C0D26EA(dcPeripheralInit[0]);
    func_8C0D262E(func_8C06CA7C,0);
    for(index=0;index<1;index++) {
        p=&dcPeripheral[index];
        p->state=0;p->object=0;memset(&p->config,0,sizeof(p->config));
    }
    dcPeripheralActive=0;
}

void func_8C06C712(void)
{
    char name[20];
    int location;
    void *file;
    name[4]='A';name[2]='D';name[12]='F';name[8]='G';name[1]='I';name[6]='L';name[0]='M';name[7]='O';name[11]='S';name[3]='W';name[5]='Y';name[9]=name[7];name[13]=name[2];name[10]='.';name[14]=0;
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
    return dcPeripheral[index].state;
}

int func_8C06C95C(int index)
{
    dcPeripheral[index].object->ops->pause(dcPeripheral[index].object,0);
    return func_8C06CA0A(index);
}

int func_8C06C986(int index)
{
    tDCPeripheral *p;
    if(index>=0 && index<1) {
        p=&dcPeripheral[index];
        if(p->state) {
            p->object->ops->stop(p->object);
            p->object->ops->release(p->object);
            DeallocateMemory(p->config.buffer);
        }
        p->state=0;p->object=0;memset(&p->config,0,sizeof(p->config));
        dcPeripheralActive=0;
        for(index=0;index<1;index++)if(dcPeripheral[index].state)dcPeripheralActive=1;
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
    int index;
    int result=0;
    for(index=0;index<1;index++)if(dcPeripheral[index].object)result=1;
    return result;
}
void func_8C06CA6A(void) {int result;result=func_8C06C986(0);}
void func_8C06CA76(void) {func_8C0D265C();}
/* Target-proven empty SDKcallback CA7C: rts;nop. */
void func_8C06CA7C(void) {}
