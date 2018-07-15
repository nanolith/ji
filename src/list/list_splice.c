/**
 * \brief Splice two lists together.
 *
 * \copyright Justin Handville 2018.  All rights reserved.  Please see LICENSE
 *            for licensing.
 */

#include <model_check/assert.h>
#include <ej/list.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief The list_splice method will splice two lists into one.  After this
 * method is called, the x list will contain all nodes, and the y list will be
 * empty.
 *
 * \param x             The x list to splice with the values from the y list.
 * \param y             The y list to destructively splice.
 */
void list_splice(list_t* x, list_t* y)
{
    MODEL_ASSERT(PROP_VALID_LIST(x));
    MODEL_ASSERT(PROP_VALID_LIST(y));

    /* if there are no elements in x, then take y's head and tail. */
    if (NULL == x->head)
    {
        MODEL_ASSERT(PROP_VALID_LIST_EMPTY(x));

        x->head = y->head;
        x->tail = y->tail;
        x->size = y->size;

        MODEL_ASSERT(PROP_VALID_LIST(x));
    }
    /* if there are elements in x and y, then splice the list. */
    else if (NULL != y->head)
    {
        MODEL_ASSERT(PROP_VALID_LIST_NOT_EMPTY(x));
        MODEL_ASSERT(PROP_VALID_LIST_NOT_EMPTY(y));

        x->tail->next = y->head;
        y->head->prev = x->tail;
        x->tail = y->tail;
        x->size += y->size;

        MODEL_ASSERT(PROP_VALID_LIST_NOT_EMPTY(x));
    }
    /* otherwise, x has elements but y has none, so do nothing. */
    else
    {
        MODEL_ASSERT(PROP_VALID_LIST_NOT_EMPTY(x));
        MODEL_ASSERT(PROP_VALID_LIST_EMPTY(y));
    }

    /* In each case, we can clean up y. This is either a no-op or clears the
      * list.*/
    MODEL_ASSERT(PROP_VALID_LIST(y));
    y->head = y->tail = NULL;
    y->size = 0;

    /* y is now empty. */
    MODEL_ASSERT(PROP_VALID_LIST_EMPTY(y));
}
