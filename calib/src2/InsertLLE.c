/* ll.c: InsertLLE. DC layout: handle +0, dbl +1, numElements +4, head +8, tail +12;
   node next +0, prev +4.
   Source shape change 2 (ternary prev selection, evidence: diamond joining into one store register) on top of change 1 (evidence: target reloads b4->prev, list->head, list->tail and *prevh
   from memory before every use, and selects `prev` through one shared store): the DC body
   does not cache those through the MAT3 `prevh`/`prev`/`direct` locals in the same way. */
typedef struct tDLLE {
    void *next;
    void *prev;
} tDLLE;

typedef struct tDLList {
    char handle;
    char dbl;
    int numElements;
    void *head;
    void *tail;
} tDLList;

void InsertLLE(tDLList *list, void *insert, void *before)
{
    tDLLE *in;
    tDLLE *prev;
    tDLLE **prevh;
    tDLLE *direct;
    tDLLE *b4;

    if (!insert)
        return;
    if (list->handle) {
        in = *(tDLLE **)insert;
        b4 = before ? *(tDLLE **)before : 0;
    } else {
        in = insert;
        b4 = before;
    }
    if (before) {
        if (list->dbl) {
            if (b4->prev) {
                prev = list->handle ? *(tDLLE **)b4->prev : b4->prev;
                prev->next = insert;
            } else {
                list->head = insert;
            }
            in->prev = b4->prev;
            b4->prev = insert;
        } else {
            if (list->head == before) {
                list->head = insert;
            } else if (list->handle) {
                prevh = list->head;
                while (prevh) {
                    if ((*prevh)->next == before)
                        (*prevh)->next = insert;
                    prevh = (*prevh)->next;
                }
            } else {
                direct = list->head;
                while (direct) {
                    if (direct->next == before)
                        direct->next = insert;
                    direct = direct->next;
                }
            }
        }
    } else {
        if (list->dbl)
            in->prev = list->tail;
        if (list->tail) {
            prev = list->handle ? *(tDLLE **)list->tail : list->tail;
            prev->next = insert;
        } else {
            list->head = insert;
        }
        list->tail = insert;
    }
    in->next = before;
    list->numElements++;
}
