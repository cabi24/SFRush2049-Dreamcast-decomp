#ifndef ZOIDLIGHT_TYPES_H
#define ZOIDLIGHT_TYPES_H
/* Opaque pointed type: this unit never dereferences light data. */
typedef struct tZoidLightDataDC tZoidLightDataDC;
typedef struct tZoidLightObjectDC {
    char pad0[0x2e];
    signed short numLights; /* 068F02/08: short store at+2E. */
    tZoidLightDataDC *lights; /* 068F0C: pointer store at+30. */
    char pad34[0x58-0x34]; /* 068EFC/8F00 proves88-byte object stride. */
} tZoidLightObjectDC;
typedef char zoidlight_check_object[(sizeof(tZoidLightObjectDC)==0x58)?1:-1];
extern tZoidLightObjectDC ObjInstanceList[];
#endif
