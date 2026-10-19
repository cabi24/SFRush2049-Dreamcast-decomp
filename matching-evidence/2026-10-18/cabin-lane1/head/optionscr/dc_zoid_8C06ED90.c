#include "dc_render_prefix.h"

void func_8C06ED90(void)
{
    _builtin_set_imask(15);
    func_8C0D9076(0,0x10000000);
    DC_HW_CONTROL&=~0x20;
    func_8C0D8BC0();
    _builtin_set_imask(0);
}

/* Actual status dispatch has no observable case actions in target; not an unknown empty body. */
void func_8C06EDC2(int result,char *name)
{
    switch(result) {
    case 0: break; case 1: break; case 2: break; case 3: break;
    case 4: break; case 5: break; case 6: break; case 7: break;
    case 8: break; case 9: break; case 10: break; case 120: break;
    case 0xdddd: break;
    }
}

void func_8C06EDFC(int mode)
{
    if(mode<0) {
        if(dcSampleEnabled8C0C9754) {
            dcSavedOpaque8C19F4F8=dcPass8C19E970.slots[0]-8;
            dcSavedTrans8C19F4FC=dcPass8C19E970.slots[2]-8;
            dcSavedPunch8C19F500=dcPass8C19E970.slots[4]-8;
            dcSampleEnabled8C0C9754=0;
        }
    } else if(mode==0) {
        dcUsedOpaque8C19F4E0=dcPass8C19E970.slots[0]-dcSavedOpaque8C19F4F8;
        if(dcUsedOpaque8C19F4E0>dcMaxOpaque8C19F4EC) dcMaxOpaque8C19F4EC=dcUsedOpaque8C19F4E0;
        dcUsedTrans8C19F4E4=dcPass8C19E970.slots[2]-dcSavedTrans8C19F4FC;
        if(dcUsedTrans8C19F4E4>dcMaxTrans8C19F4F0) dcMaxTrans8C19F4F0=dcUsedTrans8C19F4E4;
        dcUsedPunch8C19F4E8=dcPass8C19E970.slots[4]-dcSavedPunch8C19F500;
        if(dcUsedPunch8C19F4E8>dcMaxPunch8C19F4F4) dcMaxPunch8C19F4F4=dcUsedPunch8C19F4E8;
        dcSampleEnabled8C0C9754=1;
    }
}

void func_8C06EECC(void)
{
    dcRendererStats stats;
    stats.selector=7;
    func_8C0DC2A0(&stats);
    dcStatsValue8C19E9D0=stats.value28;
    if(dcStatsValue8C19E9D0>dcStatsMaximum8C19E9D4) dcStatsMaximum8C19E9D4=dcStatsValue8C19E9D0;
    if(dcPerfA8C19E95C>0) { /* target unused compareEEF4 */ }
    if(dcPerfB8C19E964>0) { /* target unused compareEEFC */ }
    if(dcPerfC8C19E96C>0) { /* target unused compareEF00 */ }
    func_8C06EDFC(1);
}

dcSQWord *func_8C06EF10(dcSQWord *out,dcClipVertex40 *vertex,unsigned int flags)
{
    out[0]=flags;
    out[1]=vertex->words[0];out[2]=vertex->words[1];out[3]=vertex->words[2];
    out[4]=vertex->words[3];out[5]=vertex->words[4];out[6]=vertex->words[5];
    out[7]=0;
    _builtin_prefetch((void *)out);
    dcPackets8C19E940++;
    return out+8;
}

dcSQWord *func_8C06EF40(dcSQWord *out,dcClipVertex40 *vertices,int nverts,int *face)
{
    int i,mask,previousIndex;
    unsigned int flags;
    dcClipVertex40 *vertex,*previous;
    i=0;mask=0;previousIndex=-2;flags=0xe0000000;
    while(flags==0xe0000000) {
        if(face[i]<nverts) {
            if(i==3) flags=0xf0000000;
            vertex=&vertices[face[i]];
            if(vertex->clip) mask|=4;
            if(!mask) {
                if(previousIndex>=0) dcPlainTriangles8C19E930++;
                out=func_8C06EF10(out,vertex,flags);
            } else if(previousIndex>=0) {
                if(mask!=7) {
                    out=dcClipFunctions8C0C9778[((previousIndex&1)<<3)|mask](out,previous,vertices,vertex,previousIndex,flags);
                } else {
                    dcClippedTriangles8C19E950++;
                }
            }
            previousIndex++;
            previous=vertex;
            mask>>=1;
            if(previousIndex>2) previousIndex=1;
        }
        i++;
    }
    return out;
}

void func_8C06F062(unsigned int *strip,int nverts,dcVertexBits20 *vertices,unsigned int color)
{
    int *face;
    dcSQWord *out;
    dcVertexBits20 *vertex;
    int i,index;
    if(dcReverseQuad8C15FA14) face=dcCornerOrderB8C0C9768;
    else face=dcCornerOrderA8C0C9758;
    dcCurrentList8C19E9DC=(*strip>>24)&7;
    *dcPass8C19E970.listType=dcCurrentList8C19E9DC;
    dcSelectedSlot8C19E9D8=&dcPass8C19E970.slots[dcCurrentList8C19E9DC];
    DC_QACR1=(unsigned int)*dcSelectedSlot8C19E9D8>>24;
    DC_QACR0=(unsigned int)*dcSelectedSlot8C19E9D8>>24;
    out=(dcSQWord *)(((unsigned int)*dcSelectedSlot8C19E9D8&0x03ffffff)|0xe0000000);
    func_8C0DAC20(out,strip,32);
    _builtin_prefetch((void *)out);
    out+=8;
    if(*dcPass8C19E970.listType==0) dcListStrips8C19E900[0]++;
    else if(*dcPass8C19E970.listType==2) dcListStrips8C19E900[1]++;
    else if(*dcPass8C19E970.listType==4) dcListStrips8C19E900[2]++;
    dcStrips8C19E920++;
    dcTriangles8C19E928+=nverts-2;
    dcVertices8C19E938+=nverts;
    for(i=0;i<4;i++) {
        index=face[i];
        if(index<nverts) {
            vertex=&vertices[index];
            if(i==3) out[0]=0xf0000000; else out[0]=0xe0000000;
            out[1]=vertex->x;out[2]=vertex->y;out[3]=vertex->z;
            out[4]=vertex->u;out[5]=vertex->v;out[6]=color;out[7]=0;
            _builtin_prefetch((void *)out);
            out+=8;
            dcPackets8C19E940++;
        }
    }
    *dcSelectedSlot8C19E9D8=(unsigned int *)(((unsigned int)*dcSelectedSlot8C19E9D8&0xfc000000)+((unsigned int)out&0x03ffffff));
}
