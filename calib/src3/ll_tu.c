/* ll.c as one translation unit for the DC exact-link test: InitLL, ClearLL, InsertLLE, RemoveLLE.
   Target span 0x8C03A1B8..0x8C03A35E (422 bytes). InsertLLE is the shape-3 body (exact on its own). */
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

void InitLL(tDLList *list, char handle, char dbl);
void ClearLL(tDLList *list);
void InsertLLE(tDLList *list, void *insert, void *before);
void RemoveLLE(tDLList *list, void *element);

void InitLL(tDLList *list, char handle, char dbl)
{
    list->handle = handle;
    list->dbl = dbl;
    ClearLL(list);
}

void ClearLL(tDLList *list)
{
    list->head = 0;
    list->tail = 0;
    list->numElements = 0;
}

void InsertLLE(tDLList *list, void *insert, void *before)
{
    tDLLE *in;
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
                if (list->handle)
                    (*(tDLLE **)b4->prev)->next = insert;
                else
                    ((tDLLE *)b4->prev)->next = insert;
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
            if (list->handle)
                (*(tDLLE **)list->tail)->next = insert;
            else
                ((tDLLE *)list->tail)->next = insert;
        } else {
            list->head = insert;
        }
        list->tail = insert;
    }
    in->next = before;
    list->numElements++;
}

void RemoveLLE(tDLList *list, void *element)
{
    tDLLE *prev;
    tDLLE **prevh;
    tDLLE *el;

    if (!element)
        return;
    if (list->handle)
        el = *(tDLLE **)element;
    else
        el = element;
    if (list->dbl) {
        if (el->next) {
            if (list->handle)
                (*(tDLLE **)el->next)->prev = el->prev;
            else
                ((tDLLE *)el->next)->prev = el->prev;
        } else {
            list->tail = el->prev;
        }
        if (el->prev) {
            if (list->handle)
                (*(tDLLE **)el->prev)->next = el->next;
            else
                ((tDLLE *)el->prev)->next = el->next;
        } else {
            list->head = el->next;
        }
    } else {
        if (element == list->head) {
            list->head = el->next;
            if (list->head == 0)
                list->tail = 0;
        } else if (list->handle) {
            prevh = list->head;
            while (prevh) {
                if ((*prevh)->next == element) {
                    (*prevh)->next = el->next;
                    if (el->next == 0)
                        list->tail = prevh;
                    break;
                }
                prevh = (*prevh)->next;
            }
        } else {
            prev = list->head;
            while (prev) {
                if (prev->next == element) {
                    prev->next = el->next;
                    if (el->next == 0)
                        list->tail = prev;
                    break;
                }
                prev = prev->next;
            }
        }
    }
    list->numElements--;
}
