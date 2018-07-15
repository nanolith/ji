/**
 * \brief Split a list at a given node, creating two lists.
 *
 * \copyright Justin Handville 2018.  All rights reserved.  Please see LICENSE
 *            for licensing.
 */

#include <model_check/assert.h>
#include <ej/list.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief The list_split method will split the list on the given node.  The
 * original list x will contain all entries BEFORE the node, and the new y list
 * will contain this node and all entries AFTER this node.  It is expected that
 * the y list is empty, and node belongs to the original x list.
 *
 * \param x             The x list to split, which will contain all values
 *                      BEFORE node after this operation is complete.
 * \param node          The node on which the list is split.
 * \param y             The y list to receive half of the list, starting with
 *                      node, and all nodes AFTER node after this operation is
 *                      complete.
 */
void list_split(list_t* x, list_node_t* node, list_t* y)
{
    MODEL_ASSERT(PROP_VALID_LIST_NOT_EMPTY(x));
    MODEL_ASSERT(PROP_VALID_LIST_EMPTY(y));
    MODEL_ASSERT(NULL != node);

    if (node == x->head)
    {
        y->head = node;
        y->tail = x->tail;
        y->size = x->size;
        x->head = NULL;
        x->tail = NULL;
        x->size = 0U;
        MODEL_ASSERT(PROP_VALID_LIST_EMPTY(x));
        MODEL_ASSERT(PROP_VALID_LIST_NOT_EMPTY(y));
    }
    else if (node == x->tail)
    {
        x->tail = node->prev;
        x->tail->next = NULL;
        node->prev = NULL;
        node->next = NULL;
        y->head = node;
        y->tail = node;
        --x->size;
        y->size = 1;
        MODEL_ASSERT(PROP_VALID_LIST_NOT_EMPTY(x));
        MODEL_ASSERT(PROP_VALID_LIST_NOT_EMPTY(y));
    }
    else
    {
        y->tail = x->tail;
        x->tail = node->prev;
        y->head = node;
        x->tail->next = NULL;
        node->prev = NULL;
        while (node)
        {
            ++y->size;
            --x->size;
            node = node->next;
        }
        MODEL_ASSERT(PROP_VALID_LIST_NOT_EMPTY(x));
        MODEL_ASSERT(PROP_VALID_LIST_NOT_EMPTY(y));
    }
}
