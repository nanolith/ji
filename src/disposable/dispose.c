/**
 * \brief dispose method for disposable interface.
 *
 * \copyright Justin Handville 2018.  All rights reserved.  Please see LICENSE
 *            for licensing.
 */

#include <ej/disposable.h>

/**
 * \brief Dispose of a disposable structure.
 *
 * The dispose() method calls the disposable structure's dispose method,
 * allowing this structure to perform any final resource management before this
 * structure is freed by the caller.
 *
 * \param disp              the disposable structure to dispose.
 */
void dispose(disposable_t* disp)
{
    disp->dispose(disp);
}
