#include <ej/list.h>
#include <model_check/assert.h>
#include <stdbool.h>

typedef struct foo
{
    disposable_t hdr;
    int val;
} foo_t;

static void dispose_foo(disposable_t* disp)
{
}

static foo_t* create_foo(int val)
{
    foo_t* ret = (foo_t*)malloc(sizeof(foo_t));
    if (NULL == ret)
        return ret;

    ret->hdr.dispose = &dispose_foo;
    ret->val = val;

    return ret;
}

int main(int argc, char* argv[])
{
    list_t list;
    foo_t *f1, *f2, *f3, *f4, *f5;

    if (0 != list_init(&list))
    {
        return 1;
    }

    f1 = create_foo(1);
    f2 = create_foo(2);
    f3 = create_foo(3);
    f4 = create_foo(4);
    f5 = create_foo(5);

    if (NULL == f1 || NULL == f2 || NULL == f3 || NULL == f4 || NULL == f5)
    {
        if (f1) free(f1);
        if (f2) free(f2);
        if (f3) free(f3);
        if (f4) free(f4);
        if (f5) free(f5);
        return 2;
    }

    /* add f2 to the list */
    if (0 != list_push_front(&list, (disposable_t*)f2))
    {
        free(f1); free(f2); free(f3); free(f4); free(f5);
        dispose((disposable_t*)&list);
        return 3;
    }

    MODEL_ASSERT(PROP_VALID_LIST_NOT_EMPTY(&list));
    MODEL_ASSERT(1U == list.size);
    MODEL_ASSERT(f2 == list.head->data);

    /* we should be able to insert f1 before f2. */
    if (0 != list_insert(&list, list.head, (disposable_t*)f1))
    {
        free(f1); free(f3); free(f4); free(f5);
        dispose((disposable_t*)&list);
        return 4;
    }

    /* verify that data is in the correct order in the list. */
    MODEL_ASSERT(PROP_VALID_LIST_NOT_EMPTY(&list));
    MODEL_ASSERT(2U == list.size);
    MODEL_ASSERT(f1 == list.head->data);
    MODEL_ASSERT(f2 == list.head->next->data);
    MODEL_ASSERT(f2 == list.tail->data);
    MODEL_ASSERT(f1 == list.tail->prev->data);

    /* add f5 to the list. */
    if (0 != list_push_back(&list, (disposable_t*)f5))
    {
        free(f3); free(f4); free(f5);
        dispose((disposable_t*)&list);
        return 4;
    }

    /* verify that data is in the correct order in the list. */
    MODEL_ASSERT(PROP_VALID_LIST_NOT_EMPTY(&list));
    MODEL_ASSERT(3U == list.size);
    MODEL_ASSERT(f1 == list.head->data);
    MODEL_ASSERT(f2 == list.head->next->data);
    MODEL_ASSERT(f5 == list.head->next->next->data);
    MODEL_ASSERT(f5 == list.tail->data);
    MODEL_ASSERT(f2 == list.tail->prev->data);
    MODEL_ASSERT(f1 == list.tail->prev->prev->data);

    /* insert f4 before f5. */
    if (0 != list_insert(&list, list.tail, (disposable_t*)f4))
    {
        free(f3); free(f4);
        dispose((disposable_t*)&list);
        return 4;
    }

    /* verify that data is in the correct order in the list. */
    MODEL_ASSERT(PROP_VALID_LIST_NOT_EMPTY(&list));
    MODEL_ASSERT(4U == list.size);
    MODEL_ASSERT(f1 == list.head->data);
    MODEL_ASSERT(f2 == list.head->next->data);
    MODEL_ASSERT(f4 == list.head->next->next->data);
    MODEL_ASSERT(f5 == list.head->next->next->next->data);
    MODEL_ASSERT(f5 == list.tail->data);
    MODEL_ASSERT(f4 == list.tail->prev->data);
    MODEL_ASSERT(f2 == list.tail->prev->prev->data);
    MODEL_ASSERT(f1 == list.tail->prev->prev->prev->data);

    /* insert f3 before f4. */
    if (0 != list_insert(&list, list.tail->prev, (disposable_t*)f3))
    {
        free(f3);
        dispose((disposable_t*)&list);
        return 4;
    }

    /* verify that data is in the correct order in the list. */
    MODEL_ASSERT(PROP_VALID_LIST_NOT_EMPTY(&list));
    MODEL_ASSERT(5U == list.size);
    MODEL_ASSERT(f1 == list.head->data);
    MODEL_ASSERT(f2 == list.head->next->data);
    MODEL_ASSERT(f3 == list.head->next->next->data);
    MODEL_ASSERT(f4 == list.head->next->next->next->data);
    MODEL_ASSERT(f5 == list.head->next->next->next->next->data);
    MODEL_ASSERT(f5 == list.tail->data);
    MODEL_ASSERT(f4 == list.tail->prev->data);
    MODEL_ASSERT(f3 == list.tail->prev->prev->data);
    MODEL_ASSERT(f2 == list.tail->prev->prev->prev->data);
    MODEL_ASSERT(f1 == list.tail->prev->prev->prev->prev->data);

    dispose((disposable_t*)&list);

    return 0;
}
