/**
 * @file test_all.cpp
 * @author Reiji Tokuda.
 */

#include <iostream>

#include <gtest/gtest.h>


int main(int argc, char * argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
