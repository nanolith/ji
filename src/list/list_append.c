/**
 * \brief Append a data value into the list after the given node.
 *
 * \copyright Justin Handville 2018.  All rights reserved.  Please see LICENSE
 *            for licensing.
 */

#include <model_check/assert.h>
#include <ej/list.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief The list_append method will append the given data value AFTER the
 * given node in the list.  This method assumes that the provided node is part
 * of the list; it is extremely important that the caller ensures that this is
 * true. The ownership of this data is transferred to the list who is
 * responsible for dispose()ing and free()ing it.
 *
 * \param list          The list to modify.
 * \param node          The list node after which this data is appended.
 * \param data          The data to append.
 *
 * \returns 0 on success and non-zero on failure.
 */
int list_append(list_t* list, list_node_t* node, disposable_t* data)
{
    MODEL_ASSERT(PROP_VALID_LIST(list));
    MODEL_ASSERT(NULL != node);
    MODEL_ASSERT(NULL != data);

    /* create a node for the new element. */
    list_node_t* newnode = (list_node_t*)malloc(sizeof(list_node_t));
    if (NULL == newnode)
        return 1;

    /* fix up the tail appending past the tail. */
    if (list->tail == node)
        list->tail = newnode;

    /* weave in the node. */
    newnode->data = data;
    newnode->prev = node;
    newnode->next = node->next;
    if (node->next)
        node->next->prev = newnode;
    node->next = newnode;
    ++list->size;

    MODEL_ASSERT(PROP_VALID_LIST(list));

    return 0;
}
