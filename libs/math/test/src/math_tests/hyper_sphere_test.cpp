//
// Created by jakub on 1/9/16.
//


#include <gtest/gtest.h>
#include <math/hyper_sphere.h>

TEST(HyperSphere, CenterIsInHyperSphere) {
    int dim = 3;
    Point<double> center(dim);
    center[0] = 2.0;
    center[1] = 2.0;
    center[2] = 2.0;

    double radius = 5.0;

    HyperSphere hyperSphere(center, radius);

    bool expectedValue = true;
    bool actualValue = hyperSphere.isInSphere(center);

    EXPECT_EQ(expectedValue, actualValue);
}

TEST(HyperSphere, PointIsInHyperSphere_CorrectlyComputed) {
    int dim = 3;
    Point<double> center(dim);
    center[0] = 2.0;
    center[1] = 2.0;
    center[2] = 2.0;

    double radius = 5.0;

    HyperSphere hyperSphere(center, radius);

    Point<double> pointInSphere(dim);
    pointInSphere[0] = 3.0;
    pointInSphere[1] = 3.5;
    pointInSphere[2] = 3.0;

    bool expectedValue = true;
    bool actualValue = hyperSphere.isInSphere(pointInSphere);

    EXPECT_EQ(expectedValue, actualValue);
}

TEST(HyperSphere, PointIsNotInHyperSphere_CorrectlyComputed) {
    int dim = 3;
    Point<double> center(dim);
    center[0] = 2.0;
    center[1] = 2.0;
    center[2] = 2.0;

    double radius = 5.0;

    HyperSphere hyperSphere(center, radius);

    Point<double> pointInSphere(dim);
    pointInSphere[0] = 10.0;
    pointInSphere[1] = 20.0;
    pointInSphere[2] = 30.0;

    bool expectedValue = false;
    bool actualValue = hyperSphere.isInSphere(pointInSphere);

    EXPECT_EQ(expectedValue, actualValue);
}