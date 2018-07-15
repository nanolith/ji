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
    foo_t *f1, *f2, *f3;

    if (0 != list_init(&list))
    {
        return 1;
    }

    f1 = create_foo(1);
    f2 = create_foo(2);
    f3 = create_foo(3);

    if (NULL == f1 || NULL == f2 || NULL == f3)
    {
        if (f1) free(f1);
        if (f2) free(f2);
        if (f3) free(f3);
        return 2;
    }

    if (0 != list_push_back(&list, f1))
    {
        free(f1); free(f2); free(f3); dispose((disposable_t*)&list);
        return 3;
    }

    if (0 != list_push_back(&list, f2))
    {
        free(f2); free(f3); dispose((disposable_t*)&list);
        return 4;
    }

    if (0 != list_push_back(&list, f3))
    {
        free(f3); dispose((disposable_t*)&list);
        return 5;
    }

    MODEL_ASSERT(NULL != list.head);
    MODEL_ASSERT(f1 == list.head->data);
    MODEL_ASSERT(NULL != list.head->next);
    MODEL_ASSERT(f2 == list.head->next->data);
    MODEL_ASSERT(NULL != list.head->next->next);
    MODEL_ASSERT(f3 == list.head->next->next->data);

    foo_t* fa = NULL;
    foo_t* fb = NULL;
    foo_t* fc = NULL;
    MODEL_ASSERT(3U == list.size);
    MODEL_ASSERT(0 == list_pop_front(&list, (disposable_t**)&fa));
    MODEL_ASSERT(2U == list.size);
    MODEL_ASSERT(0 == list_pop_front(&list, (disposable_t**)&fb));
    MODEL_ASSERT(1U == list.size);
    MODEL_ASSERT(0 == list_pop_front(&list, (disposable_t**)&fc));
    MODEL_ASSERT(PROP_VALID_LIST_EMPTY(&list));

    MODEL_ASSERT(fa == f1);
    MODEL_ASSERT(fb == f2);
    MODEL_ASSERT(fc == f3);

    free(f1); free(f2); free(f3);

    dispose((disposable_t*)&list);

    return 0;
}
