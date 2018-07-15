/**
 * \brief This header defines the list type: a doubly-linked list.
 *
 * \copyright Justin Handville 2018.  All rights reserved.  Please see LICENSE
 *            for licensing.
 */

#ifndef  EJ_LIST_HEADER_GUARD
# define EJ_LIST_HEADER_GUARD

#include <ej/disposable.h>

#ifdef   __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * A linked list node contains a pointer to the next and previous entries.  It
 * also contains a pointer to the data for this node, which is a disposable
 * instance.
 */
typedef struct list_node
{
    struct list_node* next;
    struct list_node* prev;
    disposable_t* data;
} list_node_t;

/**
 * A linked list holds the head and tail for the list.
 */
typedef struct list
{
    disposable_t hdr;
    list_node_t* head;
    list_node_t* tail;

    size_t size;
} list_t;

/**
 * \brief The list_init method creates a new empty linked list.
 *
 * \param list          The list to initialize.
 *
 * \returns 0 on success and non-zero on failure.
 */
int list_init(list_t* list);

/**
 * \brief The list_push_front method pushes a data value onto the front of the
 * linked list.  The ownership of this data is transferred to the list and may
 * be dispose()d and free()d if deleted or if the list is dispose()d.
 *
 * \param list          The list to modify.
 * \param data          The data item to push onto the list.
 *
 * \returns 0 on success and non-zero on failure.
 */
int list_push_front(list_t* list, disposable_t* data);

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
int list_push_back(list_t* list, disposable_t* data);

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
int list_pop_front(list_t* list, disposable_t** data);

/**
 * \brief The list_pop_back method pops a data value off of the back of the
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
int list_pop_back(list_t* list, disposable_t** data);

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
int list_insert(list_t* list, list_node_t* node, disposable_t* data);

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
int list_append(list_t* list, list_node_t* node, disposable_t* data);

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
int list_remove(list_t* list, list_node_t* node, disposable_t** data);

/**
 * \brief The list_splice method will splice two lists into one.  After this
 * method is called, the x list will contain all nodes, and the y list will be
 * empty.
 *
 * \param x             The x list to splice with the values from the y list.
 * \param y             The y list to destructively splice.
 */
void list_splice(list_t* x, list_t* y);

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
void list_split(list_t* x, list_node_t* node, list_t* y);

/**
 * \brief Model checking property for an empty list.
 */
#define PROP_VALID_LIST_EMPTY(list) \
    (NULL != (list) && \
     (list)->size == 0U && \
     NULL == (list)->head && \
     NULL == (list)->tail)

/**
 * \brief Model checking property for a non-empty list.
 */
#define PROP_VALID_LIST_NOT_EMPTY(list) \
    (NULL != (list) && \
     (list)->size > 0U && \
     NULL != (list)->head && \
     NULL != (list)->tail)

/**
 * \brief Model checking property for a list.
 */
#define PROP_VALID_LIST(list) \
    (PROP_VALID_LIST_EMPTY(list) || PROP_VALID_LIST_NOT_EMPTY(list))

#ifdef   __cplusplus
}
#endif /*__cplusplus*/

#endif /*EJ_LIST_HEADER_GUARD*/
