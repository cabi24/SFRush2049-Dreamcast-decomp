#include "..\include\world_types.h"
extern volatile unsigned char gNumModels;
extern unsigned int gGameState;
extern int *gWorldData;
extern anon_woff_wname_14 *WorldTable;
extern tModelPart *WorldBase;
extern signed short Worlddlidx;
extern tModelPart *cur_world_obj;
extern int SetupTarget(void *,signed short,float *,int,signed char,int,int);
extern int MBOX_FindObject_WORK(const char *,signed char,signed char,int);
extern signed short MBOX_NewObjectCam(int,tUVMatrix *,signed short,int,unsigned int);
extern signed short MBOX_NewObject(int,tUVMatrix *,int,unsigned int);
extern void func_8C0686F0(float *,float *,signed short);
extern signed short func_8C0687B6(char *);
extern void func_8C05145C(void);
extern int strcmp(const char *,const char *);
extern char *strcpy(char *,const char *);
/* Exact helper ABI forms verified in mbox; no redeclarations of reserved builtins. */
#define strcpy _builtin_strcpy
#define strcmp _builtin_strcmp
extern void *bsearch(void *,void *,unsigned long,unsigned long,int (*)(void *,void *));
static void NewWorldObject(tModelPart *,int);
static int worldcmp(const anon_woff_wname_14 *,const anon_woff_wname_14 *);
static anon_woff_wname_14 *FindWorld(const char *);

/* 0x8005dd58, size 0x218 */
static void NewWorldObject(tModelPart *Wnode, int Parent)
{
    signed short dlindex; /* r27 */
    int mbobj; /* r0 */
    tModelPart *ptr; /* r26 */
    signed short cur_dlindex; /* r30 */

    cur_dlindex = -1;
    for (ptr = Wnode; ptr; ptr = ptr->next >= 0 ? WorldBase + ptr->next : 0) {
        cur_world_obj = ptr;
        if (!SetupTarget(ptr, Parent, 0, 1, -1, 1, 0)) {
            mbobj = MBOX_FindObject_WORK(ptr->name, 0, gNumModels - 1, 1);
            if (gGameState & 0x8) {
                dlindex = MBOX_NewObjectCam(mbobj, &ptr->mat, 1, Parent, ptr->dlflags);
            } else {
                dlindex = MBOX_NewObject(mbobj, &ptr->mat, Parent, ptr->dlflags);
            }
            if ((ptr->min[0] != ptr->max[0] || ptr->min[1] != ptr->max[1]) && Parent == -1) {
                func_8C0686F0(ptr->min, ptr->max, dlindex);
            }
            if (Worlddlidx == -1) {
                Worlddlidx = dlindex;
            }
            if (cur_dlindex == -1) {
                cur_dlindex = dlindex;
            }
        }
    }
    for (; Wnode; Wnode = Wnode->next >= 0 ? WorldBase + Wnode->next : 0) {
        if (Wnode->child >= 0) {
            NewWorldObject(WorldBase + Wnode->child, cur_dlindex);
        }
        cur_dlindex++;
    }
}

/* 0x8005df70, size 0x28 */
static int worldcmp(const anon_woff_wname_14 *awhdr, const anon_woff_wname_14 *bwhdr)
{
    int result;
    result = strcmp(awhdr->wname, bwhdr->wname);
    return result;
}

/* 0x8005df98, size 0x48 */
static anon_woff_wname_14 *FindWorld(const char *wstr)
{
    anon_woff_wname_14 *ptr; /* r0 */
    anon_woff_wname_14 whdr; /* stack */

    strcpy(whdr.wname, wstr);
    ptr = bsearch(&whdr, WorldTable, *gWorldData, sizeof(anon_woff_wname_14), (int (*)(void *, void *))worldcmp);
    if (!ptr) {
    }
    return ptr;
}

/* 0x8005dfe0, size 0x4c */
signed short NewWorld(const char *World, signed short Parent)
{
    anon_woff_wname_14 *whdr; /* r0 */

    Worlddlidx = -1;
    whdr = FindWorld(World);
    WorldBase = (tModelPart *)whdr->woff;
    NewWorldObject(WorldBase, Parent);
    func_8C05145C();
    return Worlddlidx;
}

/* 0x8005e02c, size 0xc0 */
signed short FindWorldGroup(float *pos)
{
    tModelPart *Wnode; /* r5 */
    float delta[3];

    Wnode = WorldBase;
    for (;;) {
        delta[0] = pos[0] - Wnode->mat.uvs[3][0];
        delta[1] = pos[1] - Wnode->mat.uvs[3][1];
        delta[2] = pos[2] - Wnode->mat.uvs[3][2];
        if (Wnode->min[0] <= delta[0] && delta[0] <= Wnode->max[0] && Wnode->min[1] <= delta[1] && Wnode->min[2] <= delta[2] &&
            delta[2] <= Wnode->max[2]) {
            return func_8C0687B6(Wnode->name);
        }
        if (Wnode->next >= 0) {
            Wnode = WorldBase + Wnode->next;
        } else {
            return -1;
        }
    }
}

/* 0x8005e0ec, size 0x10 */
void WorldClear(void)
{
    WorldTable = 0;
    WorldBase = 0;
}

