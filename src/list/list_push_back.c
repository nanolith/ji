/**
 * \brief Push a value to the back of the linked list.
 *
 * \copyright Justin Handville 2018.  All rights reserved.  Please see LICENSE
 *            for licensing.
 */

#include <model_check/assert.h>
#include <ej/list.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief The list_push_back method pushes a data value onto the front of the
 * linked list.  The ownership of this data is transferred to the list and may
 * be dispose()d and free()d if deleted or if the list is dispose()d.
 *
 * \param list          The list to modify.
 * \param data          The data item to push onto the list.
 *
 * \returns 0 on success and non-zero on failure.
 */
int list_push_back(list_t* list, disposable_t* data)
{
    MODEL_ASSERT(PROP_VALID_LIST(list));
    MODEL_ASSERT(PROP_VALID_DISPOSABLE(data));

    /* attempt to allocate a list node. */
    list_node_t* node = (list_node_t*)malloc(sizeof(list_node_t));
    if (NULL == node)
        return 1;

    /* populate the list node. */
    node->prev = node->next = NULL;
    node->data = data;

    /* push the node to the back of the list. */
    if (list->tail)
        list->tail->next = node;
    node->prev = list->tail;
    list->tail = node;

    /* edge case for empty list. */
    if (NULL == list->head)
        list->head = list->tail;

    /* there is now an extra element in the list. */
    ++list->size;

    /* the list is valid and not empty. */
    MODEL_ASSERT(PROP_VALID_LIST(list) && PROP_VALID_LIST_NOT_EMPTY(list));

    return 0;
}
