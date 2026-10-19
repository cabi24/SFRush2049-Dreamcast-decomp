/* bstree.c - binary search tree of caller-owned nodes (left/right links at the start of each node). */
#include "..\include\bstree_types.h"

#pragma inline(ClearBST)
void ClearBST(tOrderedList *tree);

typedef struct BSTNode {
    void *left;  /* +0: target 8C018CFA/8C018D6C */
    void *right; /* +4: target 8C018CFE/8C018D86 */
} BSTNode;

static void _InsertBSTE(tOrderedList *tree, void *root, void *element);
static void _WalkBST(tOrderedList *tree, void *root, int *num, void (*WalkFunc)(void *, int));

/* 0x8009ccd0 */
void InitBST(tOrderedList *tree, int (*OrderFunc)(void *, void *))
{
    ClearBST(tree);
    tree->OrderFunc = OrderFunc;
}

/* 0x8009cd0c */
void ClearBST(tOrderedList *tree)
{
    tree->root = 0;
    tree->numElements = 0;
}

/* 0x8009cd1c */
void InsertBSTE(tOrderedList *tree, void *element)
{
    _InsertBSTE(tree, &tree->root, element);
}

/* 0x8009cd44 */
static void _InsertBSTE(tOrderedList *tree, void *root, void *element)
{
    int x;

    if (*(void **)root == 0) {
        *(void **)root = element;
        ((BSTNode *)*(void **)root)->left = 0;
        ((BSTNode *)*(void **)root)->right = 0;
        tree->numElements++;
        return;
    }
    x = tree->OrderFunc(element, *(void **)root);
    if (x < 0)
        _InsertBSTE(tree, &((BSTNode *)*(void **)root)->left, element);
    else
        _InsertBSTE(tree, &((BSTNode *)*(void **)root)->right, element);
}

/* 0x8009cdf8 */
void WalkBST(tOrderedList *tree, void (*WalkFunc)(void *, int))
{
    int num = 0;

    _WalkBST(tree, tree->root, &num, WalkFunc);
}

/* 0x8009ce2c */
static void _WalkBST(tOrderedList *tree, void *root, int *num, void (*WalkFunc)(void *, int))
{
    if (root) {
        _WalkBST(tree, ((BSTNode *)root)->left, num, WalkFunc);
        WalkFunc(root, *num);
        (*num)++;
        _WalkBST(tree, ((BSTNode *)root)->right, num, WalkFunc);
    }
}
