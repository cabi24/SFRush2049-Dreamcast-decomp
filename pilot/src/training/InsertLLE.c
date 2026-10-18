/* Training scaffold only: not equivalent-C, target-compiled, or byte-matched.
 * Derived from the pinned static lane; consult its mapping/ABI limits.
 */
#include <stdint.h>
#include <stddef.h>

typedef struct DCNode { void *next; void *prev; } DCNode;
typedef struct DCList {
    signed char handle;
    signed char dbl;
    int32_t numElements;
    void *head;
    void *tail;
} DCList;
_Static_assert(sizeof(DCList) == 16, "DCList size");
_Static_assert(offsetof(DCList, numElements) == 4, "DCList count");
_Static_assert(offsetof(DCList, head) == 8, "DCList head");
_Static_assert(offsetof(DCList, tail) == 12, "DCList tail");

void dc_InsertLLE_candidate(DCList *list, void *insert, void *before)
{
    DCNode *in, *direct, *prev, *b4;
    DCNode **prevh;
    if (!insert) return;
    if (list->handle) {
        in = *(DCNode **)insert;
        b4 = before ? *(DCNode **)before : NULL;
    } else {
        in = insert;
        b4 = before;
    }
    if (before) {
        if (list->dbl) {
            prevh = b4->prev;
            if (prevh) {
                if (list->handle) {
                    prev = *prevh;
                    prev->next = insert;
                } else {
                    ((DCNode *)prevh)->next = insert;
                }
            } else {
                list->head = insert;
            }
            in->prev = b4->prev;
            b4->prev = insert;
        } else {
            prevh = list->head;
            if ((void *)prevh == before) {
                list->head = insert;
            } else if (list->handle) {
                while (prevh) {
                    prev = *prevh;
                    if (prev->next == before) prev->next = insert;
                    prevh = (*prevh)->next;
                }
            } else {
                direct = (DCNode *)prevh;
                while (direct) {
                    if (direct->next == before) direct->next = insert;
                    direct = direct->next;
                }
            }
        }
    } else {
        if (list->dbl) in->prev = list->tail;
        if (list->tail) {
            if (list->handle) (*(DCNode **)list->tail)->next = insert;
            else ((DCNode *)list->tail)->next = insert;
        } else {
            list->head = insert;
        }
        list->tail = insert;
    }
    in->next = before;
    list->numElements++;
}
