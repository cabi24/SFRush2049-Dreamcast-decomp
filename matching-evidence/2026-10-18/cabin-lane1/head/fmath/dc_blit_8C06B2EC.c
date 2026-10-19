#include "dc_blit_private.h"

tRGBA blitPrimColor={255,255,255,255};
tRGBA blitColor={255,255,255,255};
unsigned int blitAttribute;
int blitXmin,blitXmax,blitYmin,blitYmax;
unsigned short blitZDist;
int blitLUT,blitDLnum,blitMode;
void *blitColors;
static float gZValue;

int func_8C06B2EC(int value)
{
    if(value<0) value=-value;
    if(value>1024) { /* target comparison has no branch/action */ }
    if(value>512) return 10;
    if(value>256) return 9;
    if(value>128) return 8;
    if(value>64) return 7;
    if(value>32) return 6;
    if(value>16) return 5;
    if(value>8) return 4;
    if(value>4) return 3;
    if(value>2) return 2;
    return value>1;
}

void BlitInit(void)
{
    blitAttribute=0;
    blitXmin=0;
    blitYmin=0;
    blitXmax=639;
    blitYmax=479;
    blitZDist=0;
    blitPrimColor.c[3]=255;
    blitDLnum++;
    if(blitDLnum>=2) blitDLnum=0;
    blitMode=-1;
    blitLUT=-1;
    blitColors=0;
    gZValue=1000.1f;
}

/* Entry independently called by EndGraphics02E394..398 via02E460. */
void BlitFinish(void)
{
}

#pragma inline(BlitSetAttribute)
void BlitSetAttribute(unsigned int attribute)
{
    if((blitAttribute&attribute)!=attribute) blitAttribute|=attribute;
}

void BlitClearAttribute(unsigned int attribute)
{
    if(blitAttribute&attribute) blitAttribute&=~attribute;
}

void BlitZDist(unsigned short zDist)
{
    blitZDist=zDist;
    BlitSetAttribute(0x10);
}

void BlitXLU(unsigned short xlu)
{
    blitPrimColor.c[3]=xlu;
    BlitSetAttribute(0x20);
    blitColors=0;
}

void BlitScissor(int ulx,int uly,int lrx,int lry)
{
    if(ulx<0) blitXmin=0; else blitXmin=ulx;
    if(uly<0) blitYmin=0; else blitYmin=uly;
    if(lrx>639) blitXmax=639; else blitXmax=lrx;
    if(lry>479) blitYmax=479; else blitYmax=lry;
}

void BlitClearColors(void)
{
    blitColors=0;
    blitColor.c[0]=255;
    blitColor.c[1]=255;
    blitColor.c[2]=255;
    blitColor.c[3]=255;
}

void BlitColors(void *address)
{
    blitColor.c[0]=((unsigned char *)address)[2];
    blitColor.c[1]=((unsigned char *)address)[1];
    blitColor.c[2]=((unsigned char *)address)[0];
    blitColor.c[3]=((unsigned char *)address)[3];
}

void BlitDraw(tDCBlitTexture *tex,int x0,int y0,int x1,int y1,int s,int t)
{
    tDCBlitVertex verts[4];
    float sf,tf,sf1,tf1;
    if(x0<blitXmin) {
        if(!(blitAttribute&4)) s+=blitXmin-x0;
        x0=blitXmin;
    }
    if(y0<blitYmin) {
        if(!(blitAttribute&8)) t+=blitYmin-y0;
        y0=blitYmin;
    }
    if(x1>blitXmax) {
        if(blitAttribute&4) s-=blitXmax-x1;
        x1=blitXmax;
    }
    if(y1>blitYmax) {
        if(blitAttribute&8) t-=blitYmax-y1;
        y1=blitYmax;
    }
    if(x0>x1||y0>y1) return;
    if(tex->flags&0x20) {
        sf=(float)s/tex->width;
        tf=(float)t/tex->height;
        sf1=(float)((x1-x0)+s)/tex->width;
        tf1=(float)((y1-y0)+t)/tex->height;
    } else {
        sf=(0.5f+s)/tex->width;
        tf=(0.5f+t)/tex->height;
        sf1=(0.5f+((x1-x0)+s))/tex->width;
        tf1=(0.5f+((y1-y0)+t))/tex->height;
    }
    verts[3].fX=x0; verts[3].fY=y0;
    verts[2].fX=x1; verts[2].fY=y0;
    verts[1].fX=x1; verts[1].fY=y1;
    verts[0].fX=x0; verts[0].fY=y1;
    if(blitAttribute&0x10) {
        verts[3].fZ=1.0f/blitZDist;
        verts[2].fZ=1.0f/blitZDist;
        verts[1].fZ=1.0f/blitZDist;
        verts[0].fZ=1.0f/blitZDist;
    } else {
        verts[3].fZ=gZValue; verts[2].fZ=gZValue;
        verts[1].fZ=gZValue; verts[0].fZ=gZValue;
    }
    if(blitAttribute&0x20) blitColor.c[3]=blitPrimColor.c[3];
    if(blitAttribute&4) {
        verts[3].fU=sf1; verts[2].fU=sf;
        verts[1].fU=sf; verts[0].fU=sf1;
    } else {
        verts[3].fU=sf; verts[2].fU=sf1;
        verts[1].fU=sf1; verts[0].fU=sf;
    }
    if(blitAttribute&8) {
        verts[3].fV=tf1; verts[2].fV=tf1;
        verts[1].fV=tf; verts[0].fV=tf;
    } else {
        verts[3].fV=tf; verts[2].fV=tf;
        verts[1].fV=tf1; verts[0].fV=tf1;
    }
    func_8C06F062(tex->strip,4,verts,blitColor.rgba);
    gZValue+=0.100000002f;
}

void BlitRect(int x0,int y0,int x1,int y1,unsigned char *color)
{
    float z;
    z=gZValue;
    if(blitAttribute&0x10) z=1.0f/blitZDist;
    if(x0<blitXmin) x0=blitXmin;
    if(y0<blitYmin) y0=blitYmin;
    if(x1>blitXmax) x1=blitXmax;
    if(y1>blitYmax) y1=blitYmax;
    if(x0>x1||y0>y1) return;
    if(blitAttribute&0x20) {
        func_8C06F1EC(x0,y0,x1+1,y1+1,color[0],color[1],color[2],blitPrimColor.c[3],z);
    } else {
        func_8C06F1EC(x0,y0,x1+1,y1+1,color[0],color[1],color[2],color[3],z);
    }
    gZValue+=0.100000002f;
}
