/* Target-derived Dreamcast pak initialization slice, 8C06CADC..CC40.
 * Unsupported platform server/file bodies remain external. Original 38-function
 * map and anonymous identities are retained; no placeholder code or spacing. */
#include "paksysdc_types.h"
/* Target string/pointer tables; references gPS_PakName/ContName are also
 * used by UI clients. DC names identify two VMU slots on each of four ports. */
const char gDCPakFileFormat[] = "%s.%s";
char *gPS_PakName[8] = {"A1", "A2", "B1", "B2", "C1", "C2", "D1", "D2"};
char *gPS_ContName[8] = {"A", "A", "B", "B", "C", "C", "D", "D"};
extern tDCPak gPaks[];
extern tDLList gPS_Files[];
extern void *gDCPakBuffer;
extern void (*gDCPakErrorFunc)();
extern signed char gDCPakInited;
extern void *mem_func_8C040380(int, unsigned int);
extern void PurgeableMemory(void *, int);
extern void *memset(void *, int, unsigned int);
extern void InitLL(tDLList *, int, int);
extern void _ClearPak(unsigned char);
extern void func_8C06CCFC(void);
extern void func_8C0D9A14(int, int, void *, void (*)(void));
extern int func_8C0D9AFA(void);
extern void DeallocateMemory(void *);

void PS_InitPakSys(void (*errorFunc)())
{
    int i;
    gDCPakBuffer = mem_func_8C040380(0, 0xF100);
    PurgeableMemory(gDCPakBuffer, 0);
    memset(gDCPakBuffer, 0, 0xF100);
    func_8C0D9A14(15, 255, gDCPakBuffer, func_8C06CCFC);
    gDCPakErrorFunc = errorFunc;
    for (i = 0; i < 8; i++) {
        InitLL(&gPaks[i].files, 1, 1);
        InitLL(&gPS_Files[i], 1, 1);
        gPaks[i].damaged = 0;
        _ClearPak((unsigned char)i);
    }
    gDCPakInited = 1;
}

void func_8C06CBA8(void)
{
    while (func_8C0D9AFA()) {}
    DeallocateMemory(gDCPakBuffer);
}

int func_8C06CBC2(int pak)
{
    if (gPaks[pak].error == 14) return 0;
    if (gPaks[pak].error == 2) return gPaks[pak].inserted;
    if (!gPaks[pak].inserted && !gPaks[pak].wasRemoved) return 1;
    return 0;
}
