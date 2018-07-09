/**
 * \brief Disposable interface.
 *
 * \copyright Justin Handville 2018.  All rights reserved.  Please see LICENSE
 *            for licensing.
 */
#ifndef  EJ_DISPOSABLE_HEADER_GUARD
# define EJ_DISPOSABLE_HEADER_GUARD

#ifdef   __cplusplus
extern "C" {
#endif /*__cplusplus*/

#include <stddef.h>

/**
 * \brief Disposable interface.
 *
 * The disposable interface provides a unified mechanism for releasing resources
 * associated with a data structure.  A disposable data structure adds a \ref
 * disposable_t header as the first element, which allows the structure to be
 * cast to a \ref disposable_t and passed to the dispose() method.  The
 * structure sets up its own disposer in the header as part of its
 * initialization, which is called by dispose() to do final cleanup.
 */
typedef struct disposable
{
    void (*dispose)(struct disposable*);
} disposable_t;

/**
 * \brief Dispose of a disposable structure.
 *
 * The dispose() method calls the disposable structure's dispose method,
 * allowing this structure to perform any final resource management before this
 * structure is freed by the caller.
 *
 * \param disp              the disposable structure to dispose.
 */
void dispose(disposable_t* disp);

/**
 * \brief Model checking property for the disposable interface.
 */
#define PROP_VALID_DISPOSABLE(disp) \
    (NULL != (disp) && \
     NULL != (disp)->dispose)

#ifdef   __cplusplus
}
#endif /*__cplusplus*/

#endif /*EJ_DISPOSABLE_HEADER_GUARD*/
