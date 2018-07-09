/**
 * \brief malloc / free implementations for CBMC.
 *
 * By default, memory allocations "always work" for CBMC, which is problematic
 * for modeling many real systems in which memory allocation can fail by
 * returning NULL.  This implementation of malloc / free behaves more closely to
 * stuff found in the wild.
 *
 * \copyright Justin Handville 2018.  All rights reserved.  Please see LICENSE
 *            for licensing.
 */

#define CBMC_NO_MALLOC_OVERRIDE

#include <model_check/assert.h>
#include <stdbool.h>
#include <stdlib.h>

bool nondet_bool();

/**
 * \brief malloc implementation that behaves like pathological cases in the roal
 * world.
 *
 * \param size      The size to attempt to allocate.
 *
 * \returns NULL randomly, or allocated memory.
 */
void* cbmc_malloc(size_t size)
{
    if (nondet_bool())
    {
        return malloc(size);
    }
    else
    {
        return NULL;
    }
}

/**
 * \brief free implementation that checks for null arguments.
 *
 * \param ptr       The pointer to free.
 */
void cbmc_free(void* ptr)
{
    MODEL_ASSERT(NULL != ptr);

    free(ptr);
}
