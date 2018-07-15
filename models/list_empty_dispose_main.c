#include <ej/list.h>
#include <model_check/assert.h>
#include <stdbool.h>

int main(int argc, char* argv[])
{
    list_t list;

    if (0 != list_init(&list))
    {
        return 1;
    }

    dispose((disposable_t*)&list);

    return 0;
}
