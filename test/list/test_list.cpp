/**
 * \brief Unit tests for the list container.
 *
 * \copyright Justin Handville 2018.  All rights reserved.  Please see LICENSE
 *            for licensing.
 */

#include <ej/list.h>
#include <gtest/gtest.h>

struct foo
{
    disposable_t hdr;
    int val;
};

/* forward decls */
static foo* foo_create(int val);
static void foo_disposer_mock(disposable_t* disp);
static void foo_disposer_mock_clear();
static bool foo_disposer_mock_called;
static disposable_t* foo_disposer_mock_param_disp;

/**
 * A list can be initialized as an empty list.
 */
TEST(list, init)
{
    list_t list;

    memset(&list, 0xFE, sizeof(list));

    /* initialize the list. */
    ASSERT_EQ(0, list_init(&list));

    /* the head and tail should be NULL. */
    EXPECT_EQ(nullptr, list.head);
    EXPECT_EQ(nullptr, list.tail);

    /* the size should be 0. */
    EXPECT_EQ(0U, list.size);
}

/**
 * Dispose works on a list.
 */
TEST(list, simple_dispose)
{
    list_t list;

    /* initialize the list. */
    ASSERT_EQ(0, list_init(&list));

    /* the list can be disposed. */
    dispose((disposable_t*)&list);
}

/**
 * push_front works on a list.
 */
TEST(list, push_front_empty)
{
    list_t list;

    /* initialize the list. */
    ASSERT_EQ(0, list_init(&list));

    /* our empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list));

    /* create a foo object to place on the list. */
    foo* f = foo_create(7);

    /* pushing to the front of the list should work. */
    ASSERT_EQ(0, list_push_front(&list, (disposable_t*)f));

    /* our not empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list));

    /* There is one item in the list. */
    EXPECT_EQ(1U, list.size);

    /* Both the head and the tail point to this item. */
    ASSERT_NE(nullptr, list.head);
    ASSERT_NE(nullptr, list.tail);
    EXPECT_EQ(list.head, list.tail);

    /* The list element is the foo pointer. */
    EXPECT_EQ((disposable_t*)f, list.head->data);

    /* clear the f disposer mock. */
    foo_disposer_mock_clear();

    /* the list can be disposed, which will dispose and free the f pointer. */
    dispose((disposable_t*)&list);

    /* the mock disposer should have been called. */
    EXPECT_TRUE(foo_disposer_mock_called);
    /* the f structure should have been passed to this disposer. */
    EXPECT_EQ((disposable_t*)f, foo_disposer_mock_param_disp);
}

/**
 * push_back works on a list.
 */
TEST(list, push_back_empty)
{
    list_t list;

    /* initialize the list. */
    ASSERT_EQ(0, list_init(&list));

    /* our empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list));

    /* create a foo object to place on the list. */
    foo* f = foo_create(7);

    /* pushing to the back of the list should work. */
    ASSERT_EQ(0, list_push_back(&list, (disposable_t*)f));

    /* our not empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list));

    /* There is one item in the list. */
    EXPECT_EQ(1U, list.size);

    /* Both the head and the tail point to this item. */
    ASSERT_NE(nullptr, list.head);
    ASSERT_NE(nullptr, list.tail);
    EXPECT_EQ(list.head, list.tail);

    /* The list element is the foo pointer. */
    EXPECT_EQ((disposable_t*)f, list.head->data);

    /* clear the f disposer mock. */
    foo_disposer_mock_clear();

    /* the list can be disposed, which will dispose and free the f pointer. */
    dispose((disposable_t*)&list);

    /* the mock disposer should have been called. */
    EXPECT_TRUE(foo_disposer_mock_called);
    /* the f structure should have been passed to this disposer. */
    EXPECT_EQ((disposable_t*)f, foo_disposer_mock_param_disp);
}

/**
 * push_front works as a stack.
 */
TEST(list, push_front_stack)
{
    list_t list;

    /* initialize the list. */
    ASSERT_EQ(0, list_init(&list));

    /* our empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list));

    /* create a foo object to place on the list. */
    foo* f1 = foo_create(1);
    foo* f2 = foo_create(2);
    foo* f3 = foo_create(3);

    /* pushing to the front of the list should work each time. */
    ASSERT_EQ(0, list_push_front(&list, (disposable_t*)f1));
    ASSERT_EQ(0, list_push_front(&list, (disposable_t*)f2));
    ASSERT_EQ(0, list_push_front(&list, (disposable_t*)f3));

    /* our not empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list));

    /* There are three items in the list. */
    EXPECT_EQ(3U, list.size);

    /* Both the head and the tail are not null. */
    ASSERT_NE(nullptr, list.head);
    ASSERT_NE(nullptr, list.tail);

    /* The first element is the third foo pointer. */
    EXPECT_EQ((disposable_t*)f3, list.head->data);
    /* The next element is not null, and points to the second foo pointer. */
    ASSERT_NE(nullptr, list.head->next);
    EXPECT_EQ((disposable_t*)f2, list.head->next->data);
    /* The nextnext element is not null, and points to the first foo pointer. */
    ASSERT_NE(nullptr, list.head->next->next);
    EXPECT_EQ((disposable_t*)f1, list.head->next->next->data);

    /* From tail, the first element is the first foo pointer. */
    EXPECT_EQ((disposable_t*)f1, list.tail->data);
    /* The prev element is not null, and points to the second foo pointer. */
    ASSERT_NE(nullptr, list.tail->prev);
    EXPECT_EQ((disposable_t*)f2, list.tail->prev->data);
    /* The prevprev element is not null, and points to the third foo pointer. */
    ASSERT_NE(nullptr, list.tail->prev->prev);
    EXPECT_EQ((disposable_t*)f3, list.tail->prev->prev->data);

    /* the list can be disposed, which will dispose and free each pointer. */
    dispose((disposable_t*)&list);
}

/**
 * push_back works as a queue.
 */
TEST(list, push_back_queue)
{
    list_t list;

    /* initialize the list. */
    ASSERT_EQ(0, list_init(&list));

    /* our empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list));

    /* create a foo object to place on the list. */
    foo* f1 = foo_create(1);
    foo* f2 = foo_create(2);
    foo* f3 = foo_create(3);

    /* pushing to the back of the list should work each time. */
    ASSERT_EQ(0, list_push_back(&list, (disposable_t*)f1));
    ASSERT_EQ(0, list_push_back(&list, (disposable_t*)f2));
    ASSERT_EQ(0, list_push_back(&list, (disposable_t*)f3));

    /* our not empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list));

    /* There are three items in the list. */
    EXPECT_EQ(3U, list.size);

    /* Both the head and the tail are not null. */
    ASSERT_NE(nullptr, list.head);
    ASSERT_NE(nullptr, list.tail);

    /* The first element is the first foo pointer. */
    EXPECT_EQ((disposable_t*)f1, list.head->data);
    /* The next element is not null, and points to the second foo pointer. */
    ASSERT_NE(nullptr, list.head->next);
    EXPECT_EQ((disposable_t*)f2, list.head->next->data);
    /* The nextnext element is not null, and points to the third foo pointer. */
    ASSERT_NE(nullptr, list.head->next->next);
    EXPECT_EQ((disposable_t*)f3, list.head->next->next->data);

    /* From tail, the first element is the third foo pointer. */
    EXPECT_EQ((disposable_t*)f3, list.tail->data);
    /* The prev element is not null, and points to the second foo pointer. */
    ASSERT_NE(nullptr, list.tail->prev);
    EXPECT_EQ((disposable_t*)f2, list.tail->prev->data);
    /* The prevprev element is not null, and points to the first foo pointer. */
    ASSERT_NE(nullptr, list.tail->prev->prev);
    EXPECT_EQ((disposable_t*)f1, list.tail->prev->prev->data);

    /* the list can be disposed, which will dispose and free each pointer. */
    dispose((disposable_t*)&list);
}

/**
 * pop_front pops a single element off of the stack.
 */
TEST(list, pop_front_single)
{
    list_t list;

    /* initialize the list. */
    ASSERT_EQ(0, list_init(&list));

    /* our empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list));

    /* create a foo object to place on the list. */
    foo* f = foo_create(1);

    /* pushing to the front of the list should work each time. */
    ASSERT_EQ(0, list_push_front(&list, (disposable_t*)f));

    /* our not empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list));

    /* we should be able to pop the element off of the list. */
    foo* f2 = nullptr;
    ASSERT_EQ(0, list_pop_front(&list, (disposable_t**)&f2));

    /* the list is now empty. */
    ASSERT_TRUE(PROP_VALID_LIST_EMPTY(&list));

    /* f2 is valid and points to our f. */
    ASSERT_EQ(f, f2);

    /* clean up f2. */
    dispose((disposable_t*)f2);
    free(f2);

    /* we should not be able to pop another element off of the list. */
    foo* f3 = nullptr;
    ASSERT_NE(0, list_pop_front(&list, (disposable_t**)&f3));

    /* the list can be disposed, which will dispose and free each pointer. */
    dispose((disposable_t*)&list);
}

/**
 * pop_front pops a the first element off of a list, preserving other elements.
 */
TEST(list, pop_front_multiple)
{
    list_t list;

    /* initialize the list. */
    ASSERT_EQ(0, list_init(&list));

    /* our empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list));

    /* create three foo objects to place on the list. */
    foo* f1 = foo_create(1);
    foo* f2 = foo_create(2);
    foo* f3 = foo_create(3);

    /* pushing to the back of the list should work each time. */
    ASSERT_EQ(0, list_push_back(&list, (disposable_t*)f1));
    ASSERT_EQ(0, list_push_back(&list, (disposable_t*)f2));
    ASSERT_EQ(0, list_push_back(&list, (disposable_t*)f3));

    /* our not empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list));

    /* we should be able to pop the element off of the list. */
    foo* fa = nullptr;
    ASSERT_EQ(0, list_pop_front(&list, (disposable_t**)&fa));

    /* the list is not empty, because there are two elements left.*/
    ASSERT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list));
    ASSERT_EQ(2U, list.size);

    /* fa is valid and points to our f1. */
    ASSERT_EQ(fa, f1);

    /* clean up fa. */
    dispose((disposable_t*)fa);
    free(fa);

    /* we should be able to pop the element off of the list. */
    foo* fb = nullptr;
    ASSERT_EQ(0, list_pop_front(&list, (disposable_t**)&fb));

    /* the list is not empty, because there is one elements left.*/
    ASSERT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list));
    ASSERT_EQ(1U, list.size);

    /* fb is valid and points to our f2. */
    ASSERT_EQ(fb, f2);

    /* clean up fb. */
    dispose((disposable_t*)fb);
    free(fb);

    /* we should be able to pop the element off of the list. */
    foo* fc = nullptr;
    ASSERT_EQ(0, list_pop_front(&list, (disposable_t**)&fc));

    /* the list is empty. */
    ASSERT_TRUE(PROP_VALID_LIST_EMPTY(&list));

    /* fc is valid and points to our f3. */
    ASSERT_EQ(fc, f3);

    /* clean up fc. */
    dispose((disposable_t*)fc);
    free(fc);

    /* we should not be able to pop another element off of the list. */
    foo* fd = nullptr;
    ASSERT_NE(0, list_pop_front(&list, (disposable_t**)&fd));

    /* the list can be disposed, which will dispose and free each pointer. */
    dispose((disposable_t*)&list);
}

/**
 * pop_back pops a single element off of the stack.
 */
TEST(list, pop_back_single)
{
    list_t list;

    /* initialize the list. */
    ASSERT_EQ(0, list_init(&list));

    /* our empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list));

    /* create a foo object to place on the list. */
    foo* f = foo_create(1);

    /* pushing to the front of the list should work each time. */
    ASSERT_EQ(0, list_push_front(&list, (disposable_t*)f));

    /* our not empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list));

    /* we should be able to pop the element off of the list. */
    foo* f2 = nullptr;
    ASSERT_EQ(0, list_pop_back(&list, (disposable_t**)&f2));

    /* the list is now empty. */
    ASSERT_TRUE(PROP_VALID_LIST_EMPTY(&list));

    /* f2 is valid and points to our f. */
    ASSERT_EQ(f, f2);

    /* clean up f2. */
    dispose((disposable_t*)f2);
    free(f2);

    /* we should not be able to pop another element off of the list. */
    foo* f3 = nullptr;
    ASSERT_NE(0, list_pop_back(&list, (disposable_t**)&f3));

    /* the list can be disposed, which will dispose and free each pointer. */
    dispose((disposable_t*)&list);
}

/**
 * We can insert an element in a singleton list.
 */
TEST(list, insert_single)
{
    list_t list;

    /* initialize the list. */
    ASSERT_EQ(0, list_init(&list));

    /* our empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list));

    /* create a foo object to place on the list. */
    foo* f = foo_create(2);
    foo* n = foo_create(1);

    /* pushing to the front of the list should work each time. */
    ASSERT_EQ(0, list_push_front(&list, (disposable_t*)f));

    /* our not empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list));

    /* the head element should be our element. */
    ASSERT_NE(nullptr, list.head);
    ASSERT_EQ((disposable_t*)f, list.head->data);

    /* we should be able to insert the n element into the list. */
    ASSERT_EQ(0, list_insert(&list, list.head, (disposable_t*)n));

    /* the head element is now n. */
    ASSERT_NE(nullptr, list.head);
    EXPECT_EQ((disposable_t*)n, list.head->data);

    /* the head->next element should be f. */
    ASSERT_NE(nullptr, list.head->next);
    EXPECT_EQ((disposable_t*)f, list.head->next->data);

    /* tthe tail element should be f. */
    ASSERT_NE(nullptr, list.tail);
    EXPECT_EQ((disposable_t*)f, list.tail->data);

    /* the list can be disposed, which will dispose and free each pointer. */
    dispose((disposable_t*)&list);
}

/**
 * We can insert an element in a multi list.
 */
TEST(list, insert_multi)
{
    list_t list;

    /* initialize the list. */
    ASSERT_EQ(0, list_init(&list));

    /* our empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list));

    /* create foo objects to place on the list. */
    foo* f1 = foo_create(1);
    foo* f2 = foo_create(2);
    foo* f3 = foo_create(3);

    /* pushing to the back of the list should work each time. */
    ASSERT_EQ(0, list_push_back(&list, (disposable_t*)f1));
    ASSERT_EQ(0, list_push_back(&list, (disposable_t*)f3));

    /* our not empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list));
    /* there are 2 elements in the list. */
    EXPECT_EQ(2U, list.size);

    /* the head element should be f1. */
    ASSERT_NE(nullptr, list.head);
    ASSERT_EQ((disposable_t*)f1, list.head->data);

    /* the tail element should be f3. */
    ASSERT_NE(nullptr, list.tail);
    ASSERT_EQ((disposable_t*)f3, list.tail->data);

    /* insert f2 before f3. */
    ASSERT_EQ(0, list_insert(&list, list.tail, (disposable_t*)f2));

    /* our not empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list));

    /* there are 3 elements in the list. */
    EXPECT_EQ(3U, list.size);

    /* the head element is valid and is f1. */
    ASSERT_NE(nullptr, list.head);
    EXPECT_EQ((disposable_t*)f1, list.head->data);

    /* the tail element is valid and is f3. */
    ASSERT_NE(nullptr, list.tail);
    EXPECT_EQ((disposable_t*)f3, list.tail->data);

    /* the head->next element is valid and is f2. */
    ASSERT_NE(nullptr, list.head->next);
    EXPECT_EQ((disposable_t*)f2, list.head->next->data);

    /* the tail->prev element is valid and is f2. */
    ASSERT_NE(nullptr, list.tail->prev);
    EXPECT_EQ((disposable_t*)f2, list.tail->prev->data);

    /* f2's next and prev are valid, and are f3 and f1, respectively. */
    ASSERT_NE(nullptr, list.head->next->next);
    ASSERT_NE(nullptr, list.head->next->prev);
    EXPECT_EQ((disposable_t*)f3, list.head->next->next->data);
    EXPECT_EQ((disposable_t*)f1, list.head->next->prev->data);

    /* the list can be disposed, which will dispose and free each pointer. */
    dispose((disposable_t*)&list);
}

/**
 * We can append an element in a singleton list.
 */
TEST(list, append_single)
{
    list_t list;

    /* initialize the list. */
    ASSERT_EQ(0, list_init(&list));

    /* our empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list));

    /* create a foo object to place on the list. */
    foo* f = foo_create(2);
    foo* n = foo_create(1);

    /* pushing to the front of the list should work each time. */
    ASSERT_EQ(0, list_push_front(&list, (disposable_t*)f));

    /* our not empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list));

    /* the head element should be our element. */
    ASSERT_NE(nullptr, list.head);
    ASSERT_EQ((disposable_t*)f, list.head->data);

    /* we should be able to append the n element into the list. */
    ASSERT_EQ(0, list_append(&list, list.head, (disposable_t*)n));

    /* the head element is still f. */
    ASSERT_NE(nullptr, list.head);
    EXPECT_EQ((disposable_t*)f, list.head->data);

    /* the head->next element should be n. */
    ASSERT_NE(nullptr, list.head->next);
    EXPECT_EQ((disposable_t*)n, list.head->next->data);

    /* tthe tail element should be n. */
    ASSERT_NE(nullptr, list.tail);
    EXPECT_EQ((disposable_t*)n, list.tail->data);

    /* the list can be disposed, which will dispose and free each pointer. */
    dispose((disposable_t*)&list);
}

/**
 * We can append an element in a multi list.
 */
TEST(list, append_multi)
{
    list_t list;

    /* initialize the list. */
    ASSERT_EQ(0, list_init(&list));

    /* our empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list));

    /* create foo objects to place on the list. */
    foo* f1 = foo_create(1);
    foo* f2 = foo_create(2);
    foo* f3 = foo_create(3);

    /* pushing to the back of the list should work each time. */
    ASSERT_EQ(0, list_push_back(&list, (disposable_t*)f1));
    ASSERT_EQ(0, list_push_back(&list, (disposable_t*)f2));

    /* our not empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list));
    /* there are 2 elements in the list. */
    EXPECT_EQ(2U, list.size);

    /* the head element should be f1. */
    ASSERT_NE(nullptr, list.head);
    ASSERT_EQ((disposable_t*)f1, list.head->data);

    /* the tail element should be f2. */
    ASSERT_NE(nullptr, list.tail);
    ASSERT_EQ((disposable_t*)f2, list.tail->data);

    /* insert f3 after f2. */
    ASSERT_EQ(0, list_append(&list, list.tail, (disposable_t*)f3));

    /* our not empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list));

    /* there are 3 elements in the list. */
    EXPECT_EQ(3U, list.size);

    /* the head element is valid and is f1. */
    ASSERT_NE(nullptr, list.head);
    EXPECT_EQ((disposable_t*)f1, list.head->data);

    /* the tail element is valid and is f3. */
    ASSERT_NE(nullptr, list.tail);
    EXPECT_EQ((disposable_t*)f3, list.tail->data);

    /* the head->next element is valid and is f2. */
    ASSERT_NE(nullptr, list.head->next);
    EXPECT_EQ((disposable_t*)f2, list.head->next->data);

    /* the tail->prev element is valid and is f2. */
    ASSERT_NE(nullptr, list.tail->prev);
    EXPECT_EQ((disposable_t*)f2, list.tail->prev->data);

    /* f2's next and prev are valid, and are f3 and f1, respectively. */
    ASSERT_NE(nullptr, list.head->next->next);
    ASSERT_NE(nullptr, list.head->next->prev);
    EXPECT_EQ((disposable_t*)f3, list.head->next->next->data);
    EXPECT_EQ((disposable_t*)f1, list.head->next->prev->data);

    /* the list can be disposed, which will dispose and free each pointer. */
    dispose((disposable_t*)&list);
}

/**
 * We can remove an element from a singleton list.
 */
TEST(list, remove_single)
{
    list_t list;

    /* initialize the list. */
    ASSERT_EQ(0, list_init(&list));

    /* our empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list));

    /* create a foo object to place on the list. */
    foo* f = foo_create(1);

    /* pushing to the back of the list should work each time. */
    ASSERT_EQ(0, list_push_back(&list, (disposable_t*)f));

    /* our not empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list));
    /* there are 1 elements in the list. */
    EXPECT_EQ(1U, list.size);

    /* the head element should be f. */
    ASSERT_NE(nullptr, list.head);
    ASSERT_EQ((disposable_t*)f, list.head->data);

    /* the tail element should be f. */
    ASSERT_NE(nullptr, list.tail);
    ASSERT_EQ((disposable_t*)f, list.tail->data);

    /* remove f. */
    foo* x = nullptr; 
    ASSERT_EQ(0, list_remove(&list, list.head, (disposable_t**)&x));

    /* the x element is valid. */
    ASSERT_NE(nullptr, x);
    ASSERT_EQ(f, x);

    /* clean up. */
    dispose((disposable_t*)x);
    free(x);

    /* our empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list));

    /* there are 0 elements in the list. */
    EXPECT_EQ(0U, list.size);

    /* the head element is null. */
    ASSERT_EQ(nullptr, list.head);

    /* the tail element is null. */
    ASSERT_EQ(nullptr, list.tail);

    /* the list can be disposed, which will dispose and free each pointer. */
    dispose((disposable_t*)&list);
}

/**
 * We can remove an element from the middle of a list.
 */
TEST(list, remove_multi)
{
    list_t list;

    /* initialize the list. */
    ASSERT_EQ(0, list_init(&list));

    /* our empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list));

    /* create foo objects to place on the list. */
    foo* f1 = foo_create(1);
    foo* f2 = foo_create(2);
    foo* f3 = foo_create(3);

    /* pushing to the back of the list should work each time. */
    ASSERT_EQ(0, list_push_back(&list, (disposable_t*)f1));
    ASSERT_EQ(0, list_push_back(&list, (disposable_t*)f2));
    ASSERT_EQ(0, list_push_back(&list, (disposable_t*)f3));

    /* our not empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list));
    /* there are 3 elements in the list. */
    EXPECT_EQ(3U, list.size);

    /* the head element should be f1. */
    ASSERT_NE(nullptr, list.head);
    ASSERT_EQ((disposable_t*)f1, list.head->data);

    /* the tail element should be f3. */
    ASSERT_NE(nullptr, list.tail);
    ASSERT_EQ((disposable_t*)f3, list.tail->data);

    /* the middle element should be f2. */
    ASSERT_NE(nullptr, list.head->next);
    ASSERT_EQ((disposable_t*)f2, list.head->next->data);

    /* remove f2. */
    foo* x = nullptr; 
    ASSERT_EQ(0, list_remove(&list, list.head->next, (disposable_t**)&x));

    /* the x element is valid. */
    ASSERT_NE(nullptr, x);
    ASSERT_EQ(f2, x);

    /* clean up. */
    dispose((disposable_t*)x);
    free(x);

    /* our list now has two elements. */
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list));
    /* there are 2 elements in the list. */
    EXPECT_EQ(2U, list.size);

    /* the head element should be f1. */
    ASSERT_NE(nullptr, list.head);
    ASSERT_EQ((disposable_t*)f1, list.head->data);

    /* the tail element should be f3. */
    ASSERT_NE(nullptr, list.tail);
    ASSERT_EQ((disposable_t*)f3, list.tail->data);

    /* head next points to tail. */
    ASSERT_EQ(list.tail, list.head->next);

    /* tail prev points to head. */
    ASSERT_EQ(list.head, list.tail->prev);

    /* head prev is null. */
    ASSERT_EQ(nullptr, list.head->prev);

    /* tail next is null. */
    ASSERT_EQ(nullptr, list.tail->next);

    /* the list can be disposed, which will dispose and free each pointer. */
    dispose((disposable_t*)&list);
}

/**
 * Splicing two empty lists does nothing.
 */
TEST(list, splice_empty_empty)
{
    list_t list1, list2;

    /* initialize the lists. */
    ASSERT_EQ(0, list_init(&list1));
    ASSERT_EQ(0, list_init(&list2));

    /* our empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list1));
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list2));

    /* after splicing, list1 and list2 will both still be empty. */
    list_splice(&list1, &list2);

    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list1));
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list2));

    /* the lists can be disposed, which will dispose and free each pointer. */
    dispose((disposable_t*)&list1);
    dispose((disposable_t*)&list2);
}

/**
 * We should be able to splice a list with an empty list.
 */
TEST(list, splice_multi_empty)
{
    list_t list1, list2;

    /* initialize the lists. */
    ASSERT_EQ(0, list_init(&list1));
    ASSERT_EQ(0, list_init(&list2));

    /* our empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list1));
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list2));

    /* create foo objects to place on the list. */
    foo* f1 = foo_create(1);
    foo* f2 = foo_create(2);
    foo* f3 = foo_create(3);

    /* pushing to the back of the list should work each time. */
    ASSERT_EQ(0, list_push_back(&list1, (disposable_t*)f1));
    ASSERT_EQ(0, list_push_back(&list1, (disposable_t*)f2));
    ASSERT_EQ(0, list_push_back(&list1, (disposable_t*)f3));

    /* our not empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list1));
    /* there are 3 elements in the list. */
    EXPECT_EQ(3U, list1.size);

    /* after splicing, list1 should still contain three elements, and list2
     * should still be empty. */
    list_splice(&list1, &list2);

    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list1));
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list2));

    /* the elements are as we expect them. */
    ASSERT_NE(nullptr, list1.head);
    ASSERT_EQ((disposable_t*)f1, list1.head->data);
    ASSERT_NE(nullptr, list1.head->next);
    ASSERT_EQ((disposable_t*)f2, list1.head->next->data);
    ASSERT_NE(nullptr, list1.head->next->next);
    ASSERT_EQ((disposable_t*)f3, list1.head->next->next->data);

    /* the elements are as we expect from the tail back. */
    ASSERT_NE(nullptr, list1.tail);
    ASSERT_EQ((disposable_t*)f3, list1.tail->data);
    ASSERT_NE(nullptr, list1.tail->prev);
    ASSERT_EQ((disposable_t*)f2, list1.tail->prev->data);
    ASSERT_NE(nullptr, list1.tail->prev->prev);
    ASSERT_EQ((disposable_t*)f1, list1.tail->prev->prev->data);

    /* the lists can be disposed, which will dispose and free each pointer. */
    dispose((disposable_t*)&list1);
    dispose((disposable_t*)&list2);
}

/**
 * We should be able to splice an empty list with a multi list.
 */
TEST(list, splice_empty_multi)
{
    list_t list1, list2;

    /* initialize the lists. */
    ASSERT_EQ(0, list_init(&list1));
    ASSERT_EQ(0, list_init(&list2));

    /* our empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list1));
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list2));

    /* create foo objects to place on the list. */
    foo* f1 = foo_create(1);
    foo* f2 = foo_create(2);
    foo* f3 = foo_create(3);

    /* pushing to the back of the list should work each time. */
    ASSERT_EQ(0, list_push_back(&list2, (disposable_t*)f1));
    ASSERT_EQ(0, list_push_back(&list2, (disposable_t*)f2));
    ASSERT_EQ(0, list_push_back(&list2, (disposable_t*)f3));

    /* our not empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list2));
    /* there are 3 elements in the list. */
    EXPECT_EQ(3U, list2.size);

    /* after splicing, list1 should contain three elements, and list2 should be
     * empty. */
    list_splice(&list1, &list2);

    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list1));
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list2));

    /* the elements are as we expect them. */
    ASSERT_NE(nullptr, list1.head);
    ASSERT_EQ((disposable_t*)f1, list1.head->data);
    ASSERT_NE(nullptr, list1.head->next);
    ASSERT_EQ((disposable_t*)f2, list1.head->next->data);
    ASSERT_NE(nullptr, list1.head->next->next);
    ASSERT_EQ((disposable_t*)f3, list1.head->next->next->data);

    /* the elements are as we expect from the tail back. */
    ASSERT_NE(nullptr, list1.tail);
    ASSERT_EQ((disposable_t*)f3, list1.tail->data);
    ASSERT_NE(nullptr, list1.tail->prev);
    ASSERT_EQ((disposable_t*)f2, list1.tail->prev->data);
    ASSERT_NE(nullptr, list1.tail->prev->prev);
    ASSERT_EQ((disposable_t*)f1, list1.tail->prev->prev->data);

    /* the lists can be disposed, which will dispose and free each pointer. */
    dispose((disposable_t*)&list1);
    dispose((disposable_t*)&list2);
}

/**
 * We should be able to splice two lists with elements.
 */
TEST(list, splice_multi_multi)
{
    list_t list1, list2;

    /* initialize the lists. */
    ASSERT_EQ(0, list_init(&list1));
    ASSERT_EQ(0, list_init(&list2));

    /* our empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list1));
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list2));

    /* create foo objects to place on the list. */
    foo* f1 = foo_create(1);
    foo* f2 = foo_create(2);
    foo* f3 = foo_create(3);
    foo* f4 = foo_create(4);
    foo* f5 = foo_create(5);
    foo* f6 = foo_create(6);

    /* pushing to the back of the list should work each time. */
    ASSERT_EQ(0, list_push_back(&list1, (disposable_t*)f1));
    ASSERT_EQ(0, list_push_back(&list1, (disposable_t*)f2));
    ASSERT_EQ(0, list_push_back(&list1, (disposable_t*)f3));
    ASSERT_EQ(0, list_push_back(&list2, (disposable_t*)f4));
    ASSERT_EQ(0, list_push_back(&list2, (disposable_t*)f5));
    ASSERT_EQ(0, list_push_back(&list2, (disposable_t*)f6));

    /* our not empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list1));
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list2));
    /* there are 3 elements in the list. */
    EXPECT_EQ(3U, list1.size);
    EXPECT_EQ(3U, list2.size);

    /* after splicing, list1 should contain six elements, and list2 should be
     * empty. */
    list_splice(&list1, &list2);

    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list1));
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list2));

    /* the elements are as we expect them. */
    ASSERT_NE(nullptr, list1.head);
    ASSERT_EQ((disposable_t*)f1, list1.head->data);
    ASSERT_NE(nullptr, list1.head->next);
    ASSERT_EQ((disposable_t*)f2, list1.head->next->data);
    ASSERT_NE(nullptr, list1.head->next->next);
    ASSERT_EQ((disposable_t*)f3, list1.head->next->next->data);
    ASSERT_NE(nullptr, list1.head->next->next->next);
    ASSERT_EQ((disposable_t*)f4, list1.head->next->next->next->data);
    ASSERT_NE(nullptr, list1.head->next->next->next->next);
    ASSERT_EQ((disposable_t*)f5, list1.head->next->next->next->next->data);
    ASSERT_NE(nullptr, list1.head->next->next->next->next->next);
    ASSERT_EQ((disposable_t*)f6,
              list1.head->next->next->next->next->next->data);

    /* the elements are as we expect from the tail back. */
    ASSERT_NE(nullptr, list1.tail);
    ASSERT_EQ((disposable_t*)f6, list1.tail->data);
    ASSERT_NE(nullptr, list1.tail->prev);
    ASSERT_EQ((disposable_t*)f5, list1.tail->prev->data);
    ASSERT_NE(nullptr, list1.tail->prev->prev);
    ASSERT_EQ((disposable_t*)f4, list1.tail->prev->prev->data);
    ASSERT_NE(nullptr, list1.tail->prev->prev->prev);
    ASSERT_EQ((disposable_t*)f3, list1.tail->prev->prev->prev->data);
    ASSERT_NE(nullptr, list1.tail->prev->prev->prev->prev);
    ASSERT_EQ((disposable_t*)f2, list1.tail->prev->prev->prev->prev->data);
    ASSERT_NE(nullptr, list1.tail->prev->prev->prev->prev->prev);
    ASSERT_EQ((disposable_t*)f1,
              list1.tail->prev->prev->prev->prev->prev->data);

    /* the lists can be disposed, which will dispose and free each pointer. */
    dispose((disposable_t*)&list1);
    dispose((disposable_t*)&list2);
}

/**
 * Test that we can split a singleton list.
 */
TEST(list, split_singleton)
{
    list_t list1, list2;

    /* initialize the lists. */
    ASSERT_EQ(0, list_init(&list1));
    ASSERT_EQ(0, list_init(&list2));

    /* our empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list1));
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list2));

    /* create foo object to place on the list. */
    foo* f = foo_create(1);

    /* pushing to the back of the list should work each time. */
    ASSERT_EQ(0, list_push_back(&list1, (disposable_t*)f));

    /* our not empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list1));
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list2));
    /* there are 3 elements in the list. */
    EXPECT_EQ(1U, list1.size);
    EXPECT_EQ(0U, list2.size);

    /* after splitting, list1 should be empty, and list2 should contain f. */
    list_split(&list1, list1.head, &list2);

    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list1));
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list2));

    /* the elements are as we expect them. */
    ASSERT_NE(nullptr, list2.head);
    ASSERT_EQ((disposable_t*)f, list2.head->data);
    ASSERT_EQ(nullptr, list2.head->next);
    ASSERT_EQ(nullptr, list2.head->prev);
    ASSERT_NE(nullptr, list2.tail);
    ASSERT_EQ((disposable_t*)f, list2.tail->data);
    ASSERT_EQ(nullptr, list2.tail->next);
    ASSERT_EQ(nullptr, list2.tail->prev);

    /* the lists can be disposed, which will dispose and free each pointer. */
    dispose((disposable_t*)&list1);
    dispose((disposable_t*)&list2);
}

/**
 * Test that we can split the head of a multi list.
 */
TEST(list, split_multi_head)
{
    list_t list1, list2;

    /* initialize the lists. */
    ASSERT_EQ(0, list_init(&list1));
    ASSERT_EQ(0, list_init(&list2));

    /* our empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list1));
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list2));

    /* create foo objects to place on the list. */
    foo* f1 = foo_create(1);
    foo* f2 = foo_create(2);
    foo* f3 = foo_create(3);

    /* pushing to the back of the list should work each time. */
    ASSERT_EQ(0, list_push_back(&list1, (disposable_t*)f1));
    ASSERT_EQ(0, list_push_back(&list1, (disposable_t*)f2));
    ASSERT_EQ(0, list_push_back(&list1, (disposable_t*)f3));

    /* our not empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list1));
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list2));
    /* there are 3 elements in the list. */
    EXPECT_EQ(3U, list1.size);
    EXPECT_EQ(0U, list2.size);

    /* after splitting, list1 should be empty, and list2 should contain fs. */
    list_split(&list1, list1.head, &list2);

    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list1));
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list2));

    /* the elements are as we expect them. */
    ASSERT_NE(nullptr, list2.head);
    ASSERT_EQ((disposable_t*)f1, list2.head->data);
    ASSERT_NE(nullptr, list2.head->next);
    ASSERT_EQ((disposable_t*)f2, list2.head->next->data);
    ASSERT_NE(nullptr, list2.head->next->next);
    ASSERT_EQ((disposable_t*)f3, list2.head->next->next->data);

    /* the elements are as we expect them tail first. */
    ASSERT_NE(nullptr, list2.tail);
    ASSERT_EQ((disposable_t*)f3, list2.tail->data);
    ASSERT_NE(nullptr, list2.tail->prev);
    ASSERT_EQ((disposable_t*)f2, list2.tail->prev->data);
    ASSERT_NE(nullptr, list2.tail->prev->prev);
    ASSERT_EQ((disposable_t*)f1, list2.tail->prev->prev->data);

    /* the lists can be disposed, which will dispose and free each pointer. */
    dispose((disposable_t*)&list1);
    dispose((disposable_t*)&list2);
}

/**
 * Test that we can split the tail of a multi list.
 */
TEST(list, split_multi_tail)
{
    list_t list1, list2;

    /* initialize the lists. */
    ASSERT_EQ(0, list_init(&list1));
    ASSERT_EQ(0, list_init(&list2));

    /* our empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list1));
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list2));

    /* create foo objects to place on the list. */
    foo* f1 = foo_create(1);
    foo* f2 = foo_create(2);
    foo* f3 = foo_create(3);

    /* pushing to the back of the list should work each time. */
    ASSERT_EQ(0, list_push_back(&list1, (disposable_t*)f1));
    ASSERT_EQ(0, list_push_back(&list1, (disposable_t*)f2));
    ASSERT_EQ(0, list_push_back(&list1, (disposable_t*)f3));

    /* our not empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list1));
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list2));
    /* there are 3 elements in the list. */
    EXPECT_EQ(3U, list1.size);
    EXPECT_EQ(0U, list2.size);

    /* after splitting, list1 should be empty, and list2 should contain fs. */
    list_split(&list1, list1.tail, &list2);

    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list1));
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list2));

    /* the elements are as we expect them. */
    ASSERT_NE(nullptr, list2.head);
    ASSERT_EQ((disposable_t*)f3, list2.head->data);
    ASSERT_EQ(nullptr, list2.head->next);

    /* the elements are as we expect them tail first. */
    ASSERT_NE(nullptr, list2.tail);
    ASSERT_EQ((disposable_t*)f3, list2.tail->data);
    ASSERT_EQ(nullptr, list2.tail->prev);

    /* the elements are as we expect them. */
    ASSERT_NE(nullptr, list1.head);
    ASSERT_EQ((disposable_t*)f1, list1.head->data);
    ASSERT_NE(nullptr, list1.head->next);
    ASSERT_EQ((disposable_t*)f2, list1.head->next->data);
    ASSERT_EQ(nullptr, list1.head->next->next);

    /* the elements are as we expect them tail first. */
    ASSERT_NE(nullptr, list1.tail);
    ASSERT_EQ((disposable_t*)f2, list1.tail->data);
    ASSERT_NE(nullptr, list1.tail->prev);
    ASSERT_EQ((disposable_t*)f1, list1.tail->prev->data);
    ASSERT_EQ(nullptr, list1.tail->prev->prev);

    /* the lists can be disposed, which will dispose and free each pointer. */
    dispose((disposable_t*)&list1);
    dispose((disposable_t*)&list2);
}

/**
 * Test that we can split the middle of a multi list.
 */
TEST(list, split_multi_middle)
{
    list_t list1, list2;

    /* initialize the lists. */
    ASSERT_EQ(0, list_init(&list1));
    ASSERT_EQ(0, list_init(&list2));

    /* our empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list1));
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list2));

    /* create foo objects to place on the list. */
    foo* f1 = foo_create(1);
    foo* f2 = foo_create(2);
    foo* f3 = foo_create(3);

    /* pushing to the back of the list should work each time. */
    ASSERT_EQ(0, list_push_back(&list1, (disposable_t*)f1));
    ASSERT_EQ(0, list_push_back(&list1, (disposable_t*)f2));
    ASSERT_EQ(0, list_push_back(&list1, (disposable_t*)f3));

    /* our not empty list property is valid. */
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list1));
    EXPECT_TRUE(PROP_VALID_LIST_EMPTY(&list2));
    /* there are 3 elements in the list. */
    EXPECT_EQ(3U, list1.size);
    EXPECT_EQ(0U, list2.size);

    /* after splitting, list1 contains f1, and list2 contains f2/f3. */
    list_split(&list1, list1.head->next, &list2);

    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list1));
    EXPECT_TRUE(PROP_VALID_LIST_NOT_EMPTY(&list2));

    /* the elements are as we expect them. */
    ASSERT_NE(nullptr, list2.head);
    ASSERT_EQ((disposable_t*)f2, list2.head->data);
    ASSERT_NE(nullptr, list2.head->next);
    ASSERT_EQ((disposable_t*)f3, list2.head->next->data);
    ASSERT_EQ(nullptr, list2.head->next->next);

    /* the elements are as we expect them tail first. */
    ASSERT_NE(nullptr, list2.tail);
    ASSERT_EQ((disposable_t*)f3, list2.tail->data);
    ASSERT_NE(nullptr, list2.tail->prev);
    ASSERT_EQ((disposable_t*)f2, list2.tail->prev->data);
    ASSERT_EQ(nullptr, list2.tail->prev->prev);

    /* the elements are as we expect them. */
    ASSERT_NE(nullptr, list1.head);
    ASSERT_EQ((disposable_t*)f1, list1.head->data);
    ASSERT_EQ(nullptr, list1.head->next);

    /* the elements are as we expect them tail first. */
    ASSERT_NE(nullptr, list1.tail);
    ASSERT_EQ((disposable_t*)f1, list1.tail->data);
    ASSERT_EQ(nullptr, list1.tail->prev);

    /* the lists can be disposed, which will dispose and free each pointer. */
    dispose((disposable_t*)&list1);
    dispose((disposable_t*)&list2);
}

static foo* foo_create(int val)
{
    foo* ret = (foo*)malloc(sizeof(foo));
    memset(ret, 0, sizeof(foo));

    ret->hdr.dispose = &foo_disposer_mock;
    ret->val = val;

    return ret;
}

static void foo_disposer_mock(disposable_t* disp)
{
    foo_disposer_mock_called = true;
    foo_disposer_mock_param_disp = disp;
}

static void foo_disposer_mock_clear()
{
    foo_disposer_mock_called = false;
    foo_disposer_mock_param_disp = nullptr;
}
