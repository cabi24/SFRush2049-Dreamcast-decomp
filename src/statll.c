#include "dc_types.h"
/* statll.c - Dreamcast fixed-size linked-list pool.
 * Three DC functions, 0x8C05138C..0x8C05145A.
 * InitStatLL contains the work performed by MAT3's separate ClearStatLL.
 */

void InitStatLL(tStatList *list, void *array, int elementSize, int numElements, char dbl)
{
    char *base;
    int i;

    list->list = array;
    list->numElements = numElements;
    list->elementSize = elementSize;
    list->dbl = dbl;
    base = list->list;
    list->store = base;
    list->head = 0;
    if (base == 0)
        return;
    for (i = 0; i < list->numElements - 1; i++)
        *(char **)(base + i * list->elementSize) = base + (i + 1) * list->elementSize;
    *(char **)(base + i * list->elementSize) = 0;
}

void *GetStatLLE(tStatList *list)
{
    tStatLLE *el = list->store;

    if (el) {
        list->store = el->next;
        el->next = list->head;
        if (list->dbl) {
            ((tDLLE *)el)->prev = 0;
            if (list->head)
                ((tDLLE *)list->head)->prev = (tDLLE *)el;
        }
        list->head = el;
    }
    return el;
}

void FreeStatLLE(tStatList *list, void *element)
{
    tStatLLE *el;
    tStatLLE *prev;
    tStatLLE *cur;

    if (!element)
        return;
    el = element;
    if (list->dbl) {
        if (el->next)
            ((tDLLE *)el->next)->prev = ((tDLLE *)el)->prev;
        if (((tDLLE *)el)->prev)
            ((tStatLLE *)((tDLLE *)el)->prev)->next = el->next;
        else
            list->head = el->next;
    } else {
        prev = 0;
        cur = list->head;
        while (cur) {
            if (el == cur) {
                if (prev)
                    prev->next = el->next;
                else
                    list->head = el->next;
                break;
            }
            prev = cur;
            cur = cur->next;
        }
    }
    el->next = list->store;
    list->store = el;
}
