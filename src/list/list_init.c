/**
 * \brief Initialize a linked list.
 *
 * \copyright Justin Handville 2018.  All rights reserved.  Please see LICENSE
 *            for licensing.
 */

#include <model_check/assert.h>
#include <ej/list.h>
#include <stdlib.h>
#include <string.h>

/* forward decls */
static void list_dispose(disposable_t* disp);

/**
 * \brief The list_init method creates a new empty linked list.
 *
 * \param list          The list to initialize.
 *
 * \returns 0 on success and non-zero on failure.
 */
int list_init(list_t* list)
{
    MODEL_ASSERT(NULL != list);

    /* clear the list. */
    memset(list, 0, sizeof(list_t));

    /* set our dispose method. */
    list->hdr.dispose = &list_dispose;

    /* the list is now valid. */
    MODEL_ASSERT(PROP_VALID_LIST(list) && PROP_VALID_LIST_EMPTY(list));

    return 0;
}

/**
 * \brief Dispose of a list and clean up nodes.
 *
 * \param disp      The list to dispose.
 */
static void list_dispose(disposable_t* disp)
{
    list_t* list = (list_t*)disp;

    /* we are disposing a valid list. */
    MODEL_ASSERT(PROP_VALID_LIST(list));

    if (list->size > 0)
    {
        MODEL_ASSERT(PROP_VALID_LIST_NOT_EMPTY(list));

        list_node_t* i = list->head;
        while (i != NULL)
        {
            list_node_t* tmp = i->next;

            /* clean up the node and data. */
            dispose(i->data);
            free(i->data);
            free(i);

            i = tmp;
        }
    }
    else
    {
        MODEL_ASSERT(PROP_VALID_LIST_EMPTY(list));
    }
}
