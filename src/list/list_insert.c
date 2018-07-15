/**
 * \brief Insert a data value into the list before the given node.
 *
 * \copyright Justin Handville 2018.  All rights reserved.  Please see LICENSE
 *            for licensing.
 */

#include <model_check/assert.h>
#include <ej/list.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief The list_insert method will insert the given data value BEFORE the
 * given node in the list.  This method assumes that the provided node is part
 * of the list; it is extremely important that the caller ensures that this is
 * true. The ownership of this data is transferred to the list who is
 * responsible for dispose()ing and free()ing it.
 *
 * \param list          The list to modify.
 * \param node          The list node before which this data is inserted.
 * \param data          The data to insert.
 *
 * \returns 0 on success and non-zero on failure.
 */
int list_insert(list_t* list, list_node_t* node, disposable_t* data)
{
    MODEL_ASSERT(PROP_VALID_LIST(list));
    MODEL_ASSERT(NULL != node);
    MODEL_ASSERT(NULL != data);

    /* create a new node to hold the data. */
    list_node_t* newnode = (list_node_t*)malloc(sizeof(list_node_t));
    if (NULL == newnode)
        return 1;

    /* fix up the head if inserting before head. */
    if (list->head == node)
        list->head = newnode;

    /* weave in the node. */
    newnode->data = data;
    newnode->next = node;
    newnode->prev = node->prev;
    if (node->prev)
        node->prev->next = newnode;
    node->prev = newnode;
    ++list->size;

    MODEL_ASSERT(PROP_VALID_LIST(list));

    return 0;
}
