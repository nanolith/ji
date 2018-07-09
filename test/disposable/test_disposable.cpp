/**
 * \brief Unit tests for the disposable interface.
 *
 * \copyright Justin Handville 2018.  All rights reserved.  Please see LICENSE
 *            for licensing.
 */

#include <ej/disposable.h>
#include <gtest/gtest.h>

/* forward decls */
static void mock_dispose(disposable_t* disp);
static void mock_dispose_clear();

struct foo
{
    disposable_t hdr;
};

disposable_t* mock_dispose_param_disp;
bool mock_dispose_called;

/**
 * The dispose method should call our mock dispose.
 */
TEST(disposable, dispose)
{
    foo test_struct;
    test_struct.hdr.dispose = &mock_dispose;

    /* preconditions */
        mock_dispose_clear();
        /* mock_dispose has NOT been called. */
        ASSERT_FALSE(mock_dispose_called);
        /* the disp parameter is null. */
        ASSERT_EQ(nullptr, mock_dispose_param_disp);

    /* call dispose on foo. */
    dispose((disposable_t*)&test_struct);

    /* postconditions */
        /* mock_dispose has been called. */
        EXPECT_TRUE(mock_dispose_called);
        /* the disp parameter is our test structure. */
        EXPECT_EQ((disposable_t*)&test_struct, mock_dispose_param_disp);
}

/**
 * \brief Mock for dispose.
 *
 * \param disp      The disposable structure to dispose.
 */
static void mock_dispose(disposable_t* disp)
{
    mock_dispose_param_disp = disp;
    mock_dispose_called = true;
}

/**
 * \brief Reset the dispose mock.
 */
static void mock_dispose_clear()
{
    mock_dispose_param_disp = nullptr;
    mock_dispose_called = false;
}
