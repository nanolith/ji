/**
 * \brief Remove a data value from the list, returning ownership of it to the
 * caller.
 *
 * \copyright Justin Handville 2018.  All rights reserved.  Please see LICENSE
 *            for licensing.
 */

#include <model_check/assert.h>
#include <ej/list.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief The list_remove method will remove the given node from the list,
 * returning the data value to be dispose()d and free()d by the caller.
 * This method assumes that the provided node is part of the list; it is
 * extremely important that the caller ensures that this is true. The ownership
 * of this data is transferred to the list who is responsible for dispose()ing
 * and free()ing it.
 *
 * \param list          The list to modify.
 * \param node          The list node to be removed.
 * \param data          Pointer to the data pointer returned to the caller.
 *
 * \returns 0 on success and non-zero on failure.
 */
int list_remove(list_t* list, list_node_t* node, disposable_t** data)
{
    MODEL_ASSERT(PROP_VALID_LIST_NOT_EMPTY(list));
    MODEL_ASSERT(NULL != node);
    MODEL_ASSERT(NULL != data);

    if (node->prev)
    {
        node->prev->next = node->next;
    }
    else
    {
        /* fixup for head. */
        list->head = node->next;

        if (list->head)
            list->head->prev = NULL;
    }

    if (node->next)
    {
        node->next->prev = node->prev;
    }
    else
    {
        /* fixup for tail. */
        list->tail = node->prev;

        if (list->tail)
            list->tail->next = NULL;
    }

    /* the list is one smaller. */
    --list->size;

    /* pass the data to the caller. */
    *data = node->data;

    /* cleanup. */
    free(node);

    return 0;
}
