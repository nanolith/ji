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

    /* add f1 to the list */
    if (0 != list_push_back(&list, (disposable_t*)f1))
    {
        free(f1); free(f2); free(f3); free(f4); free(f5);
        dispose((disposable_t*)&list);
        return 3;
    }

    /* add f2 to the list */
    if (0 != list_push_back(&list, (disposable_t*)f2))
    {
        free(f2); free(f3); free(f4); free(f5);
        dispose((disposable_t*)&list);
        return 4;
    }

    /* add f3 to the list */
    if (0 != list_push_back(&list, (disposable_t*)f3))
    {
        free(f3); free(f4); free(f5);
        dispose((disposable_t*)&list);
        return 5;
    }

    /* add f4 to the list */
    if (0 != list_push_back(&list, (disposable_t*)f4))
    {
        free(f4); free(f5);
        dispose((disposable_t*)&list);
        return 6;
    }

    /* add f5 to the list */
    if (0 != list_push_back(&list, (disposable_t*)f5))
    {
        free(f5);
        dispose((disposable_t*)&list);
        return 7;
    }

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

    /* remove f3 from the list. */
    foo_t* fc = NULL;
    if (0 != list_remove(&list, list.head->next->next, (disposable_t**)&fc))
    {
        dispose((disposable_t*)&list);

        return 8;
    }

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

    MODEL_ASSERT(f3 == fc);

    /* remove f1 from the list. */
    foo_t* fa = NULL;
    if (0 != list_remove(&list, list.head, (disposable_t**)&fa))
    {
        free(f3);
        dispose((disposable_t*)&list);

        return 9;
    }

    MODEL_ASSERT(PROP_VALID_LIST_NOT_EMPTY(&list));
    MODEL_ASSERT(3U == list.size);
    MODEL_ASSERT(f2 == list.head->data);
    MODEL_ASSERT(f4 == list.head->next->data);
    MODEL_ASSERT(f5 == list.head->next->next->data);
    MODEL_ASSERT(f5 == list.tail->data);
    MODEL_ASSERT(f4 == list.tail->prev->data);
    MODEL_ASSERT(f2 == list.tail->prev->prev->data);

    MODEL_ASSERT(f1 == fa);

    /* remove f5 from the list. */
    foo_t* fe = NULL;
    if (0 != list_remove(&list, list.tail, (disposable_t**)&fe))
    {
        free(f3); free(f1);
        dispose((disposable_t*)&list);

        return 10;
    }

    MODEL_ASSERT(PROP_VALID_LIST_NOT_EMPTY(&list));
    MODEL_ASSERT(2U == list.size);
    MODEL_ASSERT(f2 == list.head->data);
    MODEL_ASSERT(f4 == list.head->next->data);
    MODEL_ASSERT(f4 == list.tail->data);
    MODEL_ASSERT(f2 == list.tail->prev->data);

    MODEL_ASSERT(f5 == fe);

    /* remove f2 from the list. */
    foo_t* fb = NULL;
    if (0 != list_remove(&list, list.head, (disposable_t**)&fb))
    {
        free(f3); free(f1); free(f5);
        dispose((disposable_t*)&list);

        return 11;
    }

    MODEL_ASSERT(PROP_VALID_LIST_NOT_EMPTY(&list));
    MODEL_ASSERT(1U == list.size);
    MODEL_ASSERT(f4 == list.head->data);
    MODEL_ASSERT(f4 == list.tail->data);

    MODEL_ASSERT(f2 == fb);

    /* remove f4 from the list. */
    foo_t* fd = NULL;
    if (0 != list_remove(&list, list.tail, (disposable_t**)&fd))
    {
        free(f3); free(f1); free(f5); free(f2);
        dispose((disposable_t*)&list);

        return 12;
    }

    /* the list is now empty. */
    MODEL_ASSERT(PROP_VALID_LIST_EMPTY(&list));

    MODEL_ASSERT(f4 == fd);

    free(f1); free(f2); free(f3); free(f4); free(f5);

    dispose((disposable_t*)&list);

    return 0;
}
