//
// Created by jakub on 1/19/16.
//

#include <gtest/gtest.h>

#include <math/math.h>

/*
 * Testing distance
 */
TEST(Math_Max, FindsTheMaximumOfDoubleObjects) {
    double expectedValue = 9.0f;

    std::vector<double> v{0.0, -0.5, 5.1, 5.1, expectedValue};

    double actualValue = math::max(v);

    EXPECT_EQ(expectedValue, actualValue);
}
