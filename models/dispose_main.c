#include <ej/disposable.h>
#include <model_check/assert.h>
#include <stdbool.h>

typedef struct foo
{
    disposable_t hdr;
} foo_t;

static bool foo_disposed = false;

static void dispose_foo(disposable_t* disp)
{
    foo_disposed = true;
}

int main(int argc, char* argv[])
{
    foo_t foo;
    foo.hdr.dispose = &dispose_foo;

    MODEL_ASSERT(foo_disposed == false);
    dispose((disposable_t*)&foo);
    MODEL_ASSERT(foo_disposed == true);
}
