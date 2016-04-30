//
// Created by jakub on 4/30/16.
//

#include <gtest/gtest.h>

#include <infinity_cad/geometry/quaternion.h>

using namespace std;
using namespace glm;

TEST(quaternion, rotatesCorrectly) {
    vec3 vec(1,2,3);
    vec3 axis(1,1,1);
    axis = normalize(axis);
    float angle = 120;

    vec3 expectedValue(3,1,2);
    vec3 actualValue = Quaternion::rotate(vec, axis, angle);

    EXPECT_EQ(expectedValue, actualValue);
}
