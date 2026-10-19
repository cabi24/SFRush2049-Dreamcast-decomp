#include "zoidlight_types.h"

/* B addresses from pools068F20/24; -1 is a signed short sentinel. */
signed short gNumLights;
tZoidLightDataDC *gLights;

/* MAT3 semantic body, DC object stride88 rather than GC96. */
void ZOID_SetObjLights(int idx, int numLights, tZoidLightDataDC *lights)
{
    ObjInstanceList[idx].numLights = numLights;
    ObjInstanceList[idx].lights = lights;
}

/* Reset-called helper; descriptive source identity remains unproved. */
void func_8C068F0E(void)
{
    gNumLights = -1;
    gLights = 0;
}
