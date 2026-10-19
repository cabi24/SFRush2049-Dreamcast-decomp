/* Target-derived native VMU vibration and Kamui texture wrappers. */
#include "dc_kamui_tex_types.h"

int func_8C06E5B4(void)
{
 tDcVibrationRequest request[16]; int success=1,i,j;
 for(i=0;i<8;i++) {
  if(!dcControllerPresent[i/2].present || !func_8C0D7954(dcVibrationPorts[i])) {
   if(dcVibrationRecords[i].active) {
    dcVibrationRecords[i].active=0;
    dcVibrationRecords[i].submittedLevel=0;
    dcVibrationRecords[i].level=0;
    continue;
   }
  } else {
   if(!dcVibrationRecords[i].active) {
    if(func_8C0D7B2A(dcVibrationPorts[i],dcVibrationRecords[i].info)) continue;
    if(dcVibrationRecords[i].info[1]>16) dcVibrationRecords[i].info[1]=16;
    func_8C0D7CFC(dcVibrationPorts[i],func_8C06E51A,i);
    if(!dcVibrationRecords[i].active) continue;
   }
  }
  if(dcVibrationRecords[i].submittedLevel==dcVibrationRecords[i].level) continue;
  for(j=0;j<dcVibrationRecords[i].info[1];j++) {
   request[j].channel=j+1;
   request[j].continuous=((unsigned char *)&dcVibrationRecords[i])[0x56+j*8+11]?1:0;
   request[j].unknown4=0;
   if(((unsigned char *)&dcVibrationRecords[i])[0x56+j*8+10])
    request[j].power=(unsigned int)(dcVibrationRecords[i].level*8)/100;
   request[j].frequency=(((unsigned char *)&dcVibrationRecords[i])[0x56+j*8+15]-((unsigned char *)&dcVibrationRecords[i])[0x56+j*8+14])/2+(char)((unsigned char *)&dcVibrationRecords[i])[0x56+j*8+14];
  }
  func_8C0D9076(request,128);
  if(func_8C0D7A2E(dcVibrationPorts[i],request,dcVibrationRecords[i].info[1])) success=0;
  else dcVibrationRecords[i].submittedLevel=dcVibrationRecords[i].level;
 }
 return success;
}

void func_8C06E7B4(void) { dcVibrationEnabled=1; }

void func_8C06E7BC(void)
{
 tDcVibrationRecord *p;
 dcVibrationEnabled=0;
 for(p=dcVibrationRecords;p<dcVibrationRecords+8;p++) if(p->active) p->level=0;
}

void func_8C06E834(int player,int level)
{
 int i,slot;
 if(dcVibrationEnabled) {
  slot=player*2;
  for(i=0;i<2;i++,slot++) if(dcVibrationRecords[slot].active) {
   dcVibrationRecords[slot].level=level>10?100:level*10;
   if(dcVibrationRecords[slot].level>99) dcVibrationRecords[slot].level=99;
  }
 }
}

tDcTexture **func_8C06E8BC(tDcTexture *texture)
{
 int i,old=dcTextureScanIndex; tDcTexture **p=dcTextureTable+old;
 for(i=old;i<1024;i++,p++) if(*p==texture) break;
 if(i>=1024) { for(i=0,p=dcTextureTable;i<old;i++,p++) if(*p==texture) break; }
 dcTextureScanIndex=i;
 return p;
}

void func_8C06E908(void)
{
 int i,mode=gMirrorWaves?2:3;
 func_8C0DB9C0(&dcKmDefaultContext,mode);
 func_8C0DB9C0(&dcKmOtherContext,mode);
 for(i=0;i<1024;i++) if(dcTextureTable[i]) func_8C0DB9C0(dcTextureTable[i]->context,mode);
}

void func_8C06E95C(tDcTexture *texture)
{
 tDcTexture **slot; tDcKmSurface *surface; tDcKmContext *context; unsigned int format;
 slot=func_8C06E8BC(0);
 surface=dcKmSurfaces+dcTextureScanIndex;
 context=dcKmContexts+dcTextureScanIndex;
 if(!texture->kind) format=2; else format=texture->kind==1;
 if(texture->extraFlags&0x2000) {
  if(texture->flags&0x04000000) format|=0x400; else format|=0x300;
 } else if(texture->extraFlags&0x4000) {
  if(texture->flags&0x04000000) format|=0x200;
  else if(texture->width!=texture->height) format|=0xd00;
  else format|=0x100;
 } else format|=0x900;
 dcCheckKm(func_8C0DBFE0(surface,texture->width,texture->height,format),dcCreateTextureName);
 dcCheckKm(func_8C0DC100(surface,texture->pixels),dcLoadTextureName);
 if(!(texture->extraFlags&0x20000)) texture->surface=surface;
 if(!surface->pixels) texture->context=&dcKmDefaultContext;
 else {
  gTextureRamUsed+=surface->bytes;
  if(dcTextureRamHigh<gTextureRamUsed) dcTextureRamHigh=gTextureRamUsed;
  func_8C06EBB2(context,surface,texture->flags);
  texture->context=context;
  dcTextureCount++;
  *slot=texture;
 }
}

void func_8C06EAD0(tDcTexture *texture)
{
 tDcKmSurface *surface;
 *func_8C06E8BC(texture)=0;
 dcTextureCount--;
 surface=dcKmSurfaces+dcTextureScanIndex;
 gTextureRamUsed-=surface->bytes;
 dcCheckKm(func_8C0DC0A0(surface),dcFreeTextureName);
}

void func_8C06EB30(tDcKmContext *context,unsigned int format)
{
 tDcKmSetup setup;
 setup.size=0x90;setup.unknown8=2;setup.unknownC=0;setup.unknown18=1;setup.unknown1C=4;
 setup.mirrorMode=gMirrorWaves?2:3;
 setup.unknown24=0;setup.unknown58=0;setup.unknown5C=0;setup.unknown60=0;setup.unknown64=0;
 if(format==0x10000000) {setup.listType=2;setup.srcBlend=8;setup.dstBlend=6;setup.alpha=1;}
 else {setup.listType=0;setup.srcBlend=10;setup.dstBlend=11;setup.alpha=0;}
 setup.unknown84=3;
 func_8C0DBBE0(context,&setup);
}

void func_8C06EBB2(tDcKmContext *context,tDcKmSurface *surface,unsigned int flags)
{
 tDcKmSetup setup;
 setup.size=0x90;setup.unknown8=2;setup.unknownC=0;setup.unknown14=0;setup.unknown18=1;setup.unknown1C=4;
 setup.mirrorMode=gMirrorWaves?2:3;setup.unknown24=0;setup.unknown28=0;
 setup.unknown58=0;setup.unknown5C=0;setup.unknown60=0;setup.unknown64=0;setup.unknown6C=0;
 if((flags&0x30000)==0x30000)setup.clampU=3;
 else if(flags&0x10000)setup.clampU=2;
 else if(flags&0x20000)setup.clampU=1;
 else setup.clampU=0;
 if((flags&0xc0000)==0xc0000)setup.clampV=3;
 else if(flags&0x40000)setup.clampV=2;
 else if(flags&0x80000)setup.clampV=1;
 else setup.clampV=0;
 setup.filter=(flags&0x20)?0:1;
 setup.unknown7C=0;setup.unknown80=2;setup.surface=surface;setup.unknown84=3;
 if((flags&0x40)||surface->pixelFormat==0x10000000) {
  setup.listType=2;setup.dstBlend=(flags&0x10)&&!gFogIntensity?10:6;setup.srcBlend=8;setup.alpha=1;
 } else if(surface->pixelFormat==0x08000000) {
  setup.listType=0;setup.srcBlend=10;setup.dstBlend=11;setup.alpha=0;
 } else if(surface->pixelFormat==0) {
  setup.listType=4;setup.srcBlend=8;setup.dstBlend=(flags&0x10)&&!gFogIntensity?10:6;setup.alpha=1;
 }
 func_8C0DBD60(context,&setup);
}
