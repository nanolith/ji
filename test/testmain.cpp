/**
 * \file testmain.cpp
 *
 * This is the command-line test runner for ji unit tests.
 *
 * \copyright Justin Handville 2018.  All rights reserved.  Please see LICENSE
 *            for licensing.
 */

#include <gtest/gtest.h>
#include <iostream>

using namespace std;

GTEST_API_ int main(int argc, char* argv[])
{
    cout << "Running main() from " << __FILE__ << endl;

    testing::InitGoogleTest(&argc, argv);    

    return RUN_ALL_TESTS();
}
