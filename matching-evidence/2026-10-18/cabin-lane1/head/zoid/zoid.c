#pragma inline(ZOID_RemoveObjectTree)
#include "zoid_types.h"
int gNumObjects,gMaxObjects,gNumMatrices,gMaxMatrices;

void ZOID_Reset(void)
{
    gNumObjects = 0;
    gNumBlitInsts = 0;
    gNumMatrices = 0;
    gNumPolyInsts = 0;
    ObjHead = -1;
    ZOID_ResetWindows();
    gGlobalCameraFacingScale = 1.0f;
    zoidlight_func_8C068F0E();
}

void ZOID_Init(void)
{
    ZOID_Reset();
}

tUVMatrix *ZOID_FindMatrix(signed short objnum)
{
    return ObjInstanceList[objnum].mat;
}

tUVMatrix *ZOID_NewMatrix(void)
{
 tUVMatrix *mat;
 gNumMatrices++;
 if(gNumMatrices>gMaxMatrices)gMaxMatrices=gNumMatrices;
 mat=&gMatrices[gNumMatrices];
 mat->uvs[3][0]=0.0f;mat->uvs[3][1]=0.0f;mat->uvs[3][2]=0.0f;
 CopyUV(unitmatrix,mat->uvs);return mat;
}

void ZOID_UpdateObject(signed short objnum, float *pos, float (*uvs)[3])
{
    tUVMatrix *mat; /* r6 */

    mat = ObjInstanceList[objnum].mat;
    if (pos) {
        mat->uvs[3][0] = pos[0];
        mat->uvs[3][1] = pos[1];
        mat->uvs[3][2] = pos[2];
    }
    if (uvs) {
        CopyUV(uvs, mat->uvs);
    }
}

signed short ZOID_FindObjInstance(char *name)
{
    int i; /* r30 */
    tZoidUnitObject *oip; /* r29 */

    for (i = 0, oip = ObjInstanceList; i < gNumObjects; i++, oip++) {
        if (!strcmp(name, gRomObjDefPtrs[oip->index >> 10].romObjDefs[oip->index & 0x3ff].name)) {
            return i;
        }
    }
    return -1;
}

signed short ZOID_ObjParent(signed short objnum)
{
    int i; /* r5 */

    for (i = 0; i < gNumObjects; i++) {
        if (ObjInstanceList[i].child == objnum) {
            return i;
        }
    }
    return -1;
}

signed short ZOID_TrueObjParent(signed short objnum)
{
    int i; /* r7 */
    short target = (signed short)objnum;
    int l; /* r0 */

    for (i = 0; i < gNumObjects; i++) {
        if ((int)target == ObjInstanceList[i].child) {
            return i;
        }
        l = ObjInstanceList[i].child;
        while (l != -1) {
            l = ObjInstanceList[l].next;
            if ((int)target == l) {
                return i;
            }
        }
    }
    return -1;
}

signed short ZOID_ObjPrevObj(signed short objnum)
{
    int i; /* r5 */

    for (i = 0; i < gNumObjects; i++) {
        if (ObjInstanceList[i].next == objnum) {
            return i;
        }
    }
    return -1;
}

signed short ZOID_LastSibling(signed short objnum)
{
    signed short curobj; /* r6 */
    signed short nextobj; /* r3 */

    nextobj = objnum;
    do {
        curobj = nextobj;
        nextobj = ObjInstanceList[curobj].next;
    } while (nextobj >= 0 && nextobj != objnum);
    return curobj;
}

void ZOID_InsertObject(signed short objnum, signed short parent)
{
    signed short child; /* r3 */
    signed short sibling; /* r0 */

    if (parent < 0) {
        if (ObjHead < 0) {
            ObjHead = objnum;
        } else {
            sibling = ZOID_LastSibling(ObjHead);
            ObjInstanceList[sibling].next = objnum;
        }
    } else {
        child = ObjInstanceList[parent].child;
        if (child == -1) {
            ObjInstanceList[parent].child = objnum;
        } else {
            sibling = ZOID_LastSibling(child);
            ObjInstanceList[sibling].next = objnum;
        }
    }
}

signed short ZOID_AddObject(int objidx, tUVMatrix *mat, signed short parent, unsigned int flags)
{
    unsigned int objnum; /* r0 */
    int i; /* r31 */

    for (i = 0; i < gNumObjects; i++) {
        if (ObjInstanceList[i].index == 0xffff) {
            break;
        }
    }
    if (i == gNumObjects) {
        gNumObjects++;
    }
    if (gMaxObjects < gNumObjects) {
        gMaxObjects = gNumObjects;
    }
    objnum = i;
    ObjInstanceList[objnum].flags = flags;
    ObjInstanceList[objnum].culling = 0;
    ObjInstanceList[objnum].mat = mat;
    ObjInstanceList[objnum].scale = 1.0f;
    ObjInstanceList[objnum].w = 1.0f;
    ObjInstanceList[objnum].look[0] = unitmatrix[2][0];
    ObjInstanceList[objnum].look[1] = unitmatrix[2][1];
    ObjInstanceList[objnum].look[2] = unitmatrix[2][2];
    ObjInstanceList[objnum].ouls = 0.0f;
    ObjInstanceList[objnum].oult = 0.0f;
    ObjInstanceList[objnum].index = objidx;
    ObjInstanceList[objnum].child = -1;
    ObjInstanceList[objnum].next = -1;
    ObjInstanceList[objnum].numLights = -1;
    ObjInstanceList[objnum].lights = 0;
    ObjInstanceList[objnum].texture[0] = 0;
    ObjInstanceList[objnum].texture[1] = 0;
    ObjInstanceList[objnum].texture[2] = 0;
    ObjInstanceList[objnum].texture[3] = 0;
    ObjInstanceList[objnum].tile = 0;
    ObjInstanceList[objnum].tileSize = 0;
    ObjInstanceList[objnum].palette = 0;
    ObjInstanceList[objnum].color.rgba = 0;
    ZOID_InsertObject(objnum, parent);
    return objnum;
}

void _RemoveObjectTree(signed short objnum, int removeChildren, int removeNext)
{
    signed short idx; /* r0 */

    if (removeNext && ObjInstanceList[objnum].next >= 0) {
        _RemoveObjectTree(ObjInstanceList[objnum].next, 1, 1);
        ObjInstanceList[objnum].next = -1;
    }
    if (removeChildren && ObjInstanceList[objnum].child >= 0) {
        _RemoveObjectTree(ObjInstanceList[objnum].child, 1, 1);
        ObjInstanceList[objnum].child = -1;
    }
    idx = ZOID_ObjParent(objnum);
    if (idx >= 0) {
        ObjInstanceList[idx].child = ObjInstanceList[objnum].next;
    } else if (ObjHead == objnum) {
        ObjHead = ObjInstanceList[objnum].next;
    } else {
        idx = ZOID_ObjPrevObj(objnum);
        if (idx >= 0) {
            ObjInstanceList[idx].next = ObjInstanceList[objnum].next;
        }
    }
    ObjInstanceList[objnum].index = 0xffff;
    ObjInstanceList[objnum].child = -1;
    ObjInstanceList[objnum].next = -1;
    if (objnum == gNumObjects - 1) {
        do {
            gNumObjects--;
        } while (ObjInstanceList[gNumObjects - 1].index == 0xffff && gNumObjects > 0);
    }
}

void ZOID_RemoveObjectTree(signed short objnum, int removeChildren)
{
    _RemoveObjectTree(objnum, removeChildren, 0);
}

void ZOID_RemoveObject(signed short objnum)
{
    ZOID_RemoveObjectTree(objnum, 0);
}

void ZOID_SetObjectDef(signed short objnum, unsigned short def)
{
    ObjInstanceList[objnum].index = def;
}

void ZOID_SetObjectParent(signed short objnum, signed short newparent)
{
    signed short parent; /* r0 */
    signed short idx; /* r0 */
    tZoidUnitObject *object;

    parent = ZOID_ObjParent(objnum);
    if (parent != -1 && parent == newparent) {
        return;
    }
    object=&ObjInstanceList[objnum];
    if (parent >= 0) {
        ObjInstanceList[parent].child = object->next;
    } else if (ObjHead == objnum) {
        ObjHead = object->next;
    } else {
        idx = ZOID_ObjPrevObj(objnum);
        if (idx >= 0) {
            ObjInstanceList[idx].next = object->next;
        }
    }
    object->next = -1;
    ZOID_InsertObject(objnum, newparent);
}

int func_8C068C68(tZoidUnitObject *obj,int window)
{
 unsigned int mask=0x100<<window;
again:
 if(!(obj->flags&mask))return 1;
 if(obj->child>=0) {
  if(func_8C068C68(&ObjInstanceList[obj->child],window))return 1;
 }
 if(obj->next>=0) {obj=&ObjInstanceList[obj->next];goto again;}
 return 0;
}
int func_8C068CC6(tZoidUnitObject *instance,int window)
{
 tZoidUnitObject *obj=instance;
 if(!(obj->flags&(0x100<<window)))return 1;
 if(obj->child>=0) {
  if(func_8C068C68(&ObjInstanceList[obj->child],window))return 1;
 }
 return 0;
}

unsigned int ZOID_ObjectFlags(signed short objnum)
{
    return ObjInstanceList[objnum].flags;
}

void ZOID_SetObjectFlags(int objnum, unsigned int flags)
{
    ObjInstanceList[objnum].flags = flags;
}
