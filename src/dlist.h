
#ifndef __DLIST_H__
#define __DLIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define prefetch(x) __builtin_prefetch(x)

#define offsetof(TYPE,MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define container_of(ptr, type, member) ({ \
        const typeof(((type *)0)->member) *__mptr = (ptr); \
        (type *)((char *)__mptr - offsetof(type,member));})

#define LIST_ENTRY(ptr, type, member) \
    container_of(ptr, type, member)

#define LIST_FOR_EACH(pos, head) \
    for (pos = (head)->pstNext; prefetch(pos->pstNext), pos != (head); \
            pos = pos->pstNext)

#define LIST_FOR_EACH_ENTRY(pos, head, member) \
    for (pos = LIST_ENTRY((head)->pstNext, typeof(*pos), member); \
            prefetch(pos->member.pstNext), &pos->member != (head); \
            pos = LIST_ENTRY(pos->member.pstNext, typeof(*pos), member))

#define LIST_FOR_EACH_SAFE(pos, n, head) \
        for (pos = (head)->pstNext, n = pos->pstNext; pos != (head); \
                 pos = n, n = pos->pstNext)

//如果链表为空，则不会进入该函数，里面的for循环什么都不打印，
//!!!但是如果链表为空，但是pos最后不为NULL，pos指向链表头
#define LIST_FOR_EACH_ENTRY_SAFE(pos, n, head, member)                  \
    for (pos = LIST_ENTRY((head)->pstNext, typeof(*pos), member),      \
            n = LIST_ENTRY(pos->member.pstNext, typeof(*pos), member); \
            &pos->member != (head);                                    \
            pos = n, n = LIST_ENTRY(n->member.pstNext, typeof(*n), member))


typedef struct tagDTQ_NODE
{
        struct tagDTQ_NODE *pstPrev;
        struct tagDTQ_NODE *pstNext;
}DTQ_NODE_S;

typedef struct tagDTQ_HEAD
{
	DTQ_NODE_S stHead;
}DTQ_HEAD_S;

static inline void list_ListNodeInit(DTQ_NODE_S *pstNode)
{
      pstNode->pstNext = (DTQ_NODE_S*)NULL;
      pstNode->pstPrev = (DTQ_NODE_S*)NULL;
      return;
}

static inline void list_ListHeadInit(DTQ_NODE_S *pstNode)
{
        pstNode->pstNext = pstNode;
        pstNode->pstPrev = pstNode;
        return;
}

static inline void list_AddBefore(DTQ_NODE_S *pstNext, DTQ_NODE_S *pstInst)
{
        pstInst->pstPrev = pstNext->pstPrev;
        pstInst->pstNext = pstNext;
        pstInst->pstPrev->pstNext = pstInst;
        pstInst->pstNext->pstPrev = pstInst;

        return;
}

static inline void list_Del(const DTQ_NODE_S *pstNode)
{
        pstNode->pstPrev->pstNext = pstNode->pstNext;
        pstNode->pstNext->pstPrev = pstNode->pstPrev;
        return;
}

static inline void list_AddTail(DTQ_NODE_S *pstList, DTQ_NODE_S *pstNode)
{
        list_AddBefore(pstList, pstNode);
        return;
}

static inline int list_empty(const DTQ_NODE_S *pstList)
{
    return pstList->pstNext == pstList;
}

#if 0
static inline void dui_FreeAll(DTQ_NODE_S *pstList, void (*pfFree)(void *))
{
     DTQ_NODE_S *pstCurNode;
     DTQ_NODE_S *pstNextNode;

    /* Free all node from list */
    LIST_FOR_EACH_SAFE(pstList, pstCurNode, pstNextNode)
    {
        pfFree(pstCurNode);
    }

    dui_ListHeadInit(pstList);
    return;
}
#endif


/*
list_add
list_add_tail
list_del
list_replace
list_move
list_is_last
list_empty
list_cut_position
list_splice
list_for_each
list_for_each_entry
*/

#endif /* __DLIST_H__ */

