/**
 * \brief Pop a value off of the front of a list.
 *
 * \copyright Justin Handville 2018.  All rights reserved.  Please see LICENSE
 *            for licensing.
 */

#include <model_check/assert.h>
#include <ej/list.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief The list_pop_front method pops a data value off of the front of the
 * linked list.  The ownership of this data is transferred to the caller who is
 * responsible for dispose()ing and free()ing it.
 *
 * If the list is empty, \ref LIST_ERROR_EMPTY will be returned, and the data
 * pointer will be set to NULL.
 *
 * \param list          The list to modify.
 * \param data          A pointer to the data pointer set to the popped value.
 *
 * \returns 0 on success and non-zero on failure.
 */
int list_pop_front(list_t* list, disposable_t** data)
{
    MODEL_ASSERT(PROP_VALID_LIST(list));
    MODEL_ASSERT(NULL != data);

    if (list->head)
    {
        /* list is NOT empty. */
        MODEL_ASSERT(PROP_VALID_LIST_NOT_EMPTY(list));

        /* capture data and the list node. */
        *data = list->head->data;
        list_node_t* node = list->head;

        /* fix up head's next or tail. */
        if (list->head->next)
            list->head->next->prev = NULL;
        else
            list->tail = NULL;

        /* fix up the head and size. */
        list->head = list->head->next;
        --list->size;

        /* clean up the node. */
        free(node);

        /* list invariant is maintained. */
        MODEL_ASSERT(PROP_VALID_LIST(list));

        return 0;
    }
    else
    {
        /* list IS empty. */
        MODEL_ASSERT(PROP_VALID_LIST_EMPTY(list));

        return 1;
    }
}
