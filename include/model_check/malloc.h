/**
 * \brief Model checking allocation.
 *
 * This header provides an alternative allocator for model checking.
 *
 * \copyright Justin Handville 2018.  All rights reserved.  Please see LICENSE
 *            for licensing.
 */
#ifndef  MODEL_CHECK_MALLOC_HEADER_GUARD
# define MODEL_CHECK_MALLOC_HEADER_GUARD

#include <stddef.h>

#ifdef   __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * \brief malloc implementation that behaves like pathological cases in the roal
 * world.
 *
 * \param size      The size to attempt to allocate.
 *
 * \returns NULL randomly, or allocated memory.
 */
void* cbmc_malloc(size_t size);

/**
 * \brief free implementation that checks for null arguments.
 *
 * \param ptr       The pointer to free.
 */
void cbmc_free(void* ptr);

#ifdef   __cplusplus
}
#endif /*__cplusplus*/

#endif /*MODEL_CHECK_MALLOC_HEADER_GUARD*/
