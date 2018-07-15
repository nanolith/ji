/**
 * \brief Model checking assertions.
 *
 * This header provides some helper methods for performing model check
 * assertions with CBMC.
 *
 * \copyright Justin Handville 2018.  All rights reserved.  Please see LICENSE
 *            for licensing.
 */
#ifndef  MODEL_CHECK_ASSERT_HEADER_GUARD
# define MODEL_CHECK_ASSERT_HEADER_GUARD

#include <model_check/malloc.h>

#ifdef   __cplusplus
extern "C" {
#endif /*__cplusplus*/

#ifdef CBMC
# define MODEL_ASSERT(x)  __CPROVER_assert((x), #x)
# define MODEL_ASSUME(x)  __CPROVER_assume((x))
# define MODEL_EXEMPT(x)
# ifndef CBMC_NO_MALLOC_OVERRIDE
# define malloc           cbmc_malloc
# define free             cbmc_free
# endif
#else
# define MODEL_ASSERT(x)
# define MODEL_ASSUME(x)
# define MODEL_EXEMPT(x) x
#endif

#ifdef   __cplusplus
}
#endif /*__cplusplus*/

#endif /*MODEL_CHECK_ASSERT_HEADER_GUARD*/
