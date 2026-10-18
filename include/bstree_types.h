/* Dreamcast bstree layouts; target displacements at 8C018CBC..8C018D98. */
#ifndef BSTREE_TYPES_H
#define BSTREE_TYPES_H
typedef struct tOrderedList {
    int (*OrderFunc)(void *, void *); /* +0: InitBST8C018CC6, insertion8C018D0E */
    long numElements;              /* +4: ClearBST8C018CCE, insertion8C018D02 */
    void *root;                    /* +8: ClearBST8C018CCA, WalkBST8C018D48 */
} tOrderedList;
typedef char dc_check_orderedlist[(sizeof(tOrderedList)==0x0c)?1:-1];
#endif
