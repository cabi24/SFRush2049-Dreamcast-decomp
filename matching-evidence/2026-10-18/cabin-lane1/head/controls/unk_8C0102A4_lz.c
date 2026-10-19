/* Targetproved compilationgroup; historicalfilenameunknown. */
#include "unk_8C010218_types.h"

unsigned int func_8C0102A4(unsigned char *source,unsigned char *output)
{
 int count=0,flags=0,bits=0,distance;
 unsigned char *start=output,*copy;
 for(;;) {
  if(!bits) {flags=*source++;bits=8;}
  bits--;
  if(flags&1) {
   count=*source++;
   *output++=count;
  } else {
   distance=*source++;
   count=15;
   count&=distance;
   distance&=0xf0;
   distance<<=4;
   distance|=*source++;
   if(!(distance|count)) return output-start;
   copy=output-distance;
   count++;
   do {*output++=*copy++;} while(--count>=0);
  }
  flags>>=1;
 }
}
