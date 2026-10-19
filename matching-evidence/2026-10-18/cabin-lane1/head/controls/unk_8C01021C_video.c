/* Targetproved compilationgroup; historicalfilenameunknown. */
#include "unk_8C010218_types.h"

void func_8C01021C(int *x,int *y)
{
 *x=0;
 *y=0;
}

void func_8C010224(int *x,int *y)
{
 *x=dcVideoOffsetX;
 *y=dcVideoOffsetY;
}

void func_8C010232(int x,int y)
{
 if(x>=-32 && x<=32) dcVideoOffsetX=x;
 if(y>=-32 && y<=32) dcVideoOffsetY=y;
 func_8C0DAE20(dcVideoOffsetX,dcVideoOffsetY+8);
}

