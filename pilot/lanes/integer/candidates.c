/* Private static-analysis scaffolds, not compiled, promoted or byte-matched.
 * Requires 32-bit target pointers. No held-out enginetorque body is included.
 * interp return declaration is deliberately provisional; see notes.md.
 */
#include <stdint.h>
#include <stddef.h>

_Static_assert(sizeof(void *) == 4, "Dreamcast candidate requires 32-bit pointers");

/* Distinct DC library implementation; do not substitute MAT3 bsearch.c. */
void *dc_bsearch_candidate(void *key, void *base, uint32_t nmemb,
                           uint32_t size, int (*compar)(void *, void *))
{
    uint32_t lower = 1;
    uint32_t upper = nmemb;
    while (lower >= 1 && lower <= upper) {
        uint32_t mid = lower + ((upper - lower) >> 1);
        unsigned char *current = (unsigned char *)base + (mid - 1) * size;
        int result = compar(key, current);
        if (result < 0) upper = mid - 1;
        if (result > 0) lower = mid + 1;
        if (result == 0) return current;
    }
    return NULL;
}

/* Only defined signed32 C arithmetic is in scope. Original DC source might
 * declare a signedshort result; target returns without EXTS.W and no standalone
 * callers were located. Validate raw32 and low16 separately before deciding.
 */
int32_t dc_interp_raw32_candidate(int32_t a, int32_t b, int32_t rem, int32_t total)
{
    return a + rem * (b - a) / total;
}

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

/* MAT3 ll.c semantics agree with all traced DC paths and offsets. This is a
 * source hypothesis, not equivalent-C or exact-emission acceptance.
 */
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
