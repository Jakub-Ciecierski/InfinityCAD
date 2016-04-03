//
// Created by jakub on 11/11/15.
//

#include <gtest/gtest.h>

#include <stdexcept>
#include <math/geometry.h>

/*
 * Testing distance
 */
TEST(MathGeometry, Distance_Two_Points_Equal) {
    double expectedDist, actualDist;
    const int size = 2;

    expectedDist = 8.485280f;
    int data1[size] = {3, 3};
    int data2[size] = {-3, -3};

    Point<int> p1(data1, size);
    Point<int> p2(data2, size);

    actualDist = math::euclideanDistance(p1, p2);

    EXPECT_EQ((int)expectedDist, (int)actualDist);
}

/*
 * Testing mean
 */
TEST(MathGeometry, CenterOfMass_EmptyContainer_EmptyPoint) {
    int expectedSize, actualSize;
    std::vector<Point<int>> points;

    expectedSize = 0;

    Point<int> actualPoint = math::centerOfMass(&points);
    actualSize = actualPoint.size();

    EXPECT_EQ(expectedSize, actualSize);
}

/*
 * Testing mean
 */
TEST(MathGeometry, CenterOfMass_Points_Mean) {
    const int pointsCount = 5;
    const int size = 3;

    int data[size] = {2, 2, 2};

    int expData[size] = {2, 2, 2};

    Point<int> expectedPoint(expData, size);

    std::vector<Point<int>> points(pointsCount);
    for(int i = 0; i < pointsCount; i++){
        Point<int> p(data, size);
        points[i] = p;
    }

    Point<int> actualPoint = math::centerOfMass(&points);

    EXPECT_EQ(expectedPoint, actualPoint);
}


TEST(MathGeometry, BoundedPoint_MaxDeltaSmallerThanChange_ProperPoint) {
    const int size = 3;

    double maxDelta = 4.0f;

    double dataSrc[size] = {2.0f, 2.0f, 2.0f};
    double dataDest[size] = {3.0f, 8.8f, 10.0f};

    double dataExpected[size] = {3.0f, 6.0f, 6.0f};

    Point<double> srcPoint(dataSrc, size);
    Point<double> destPoint(dataDest, size);

    Point<double> expectedPoint(dataExpected, size);

    Point<double> actualPoint = math::boundPoint(srcPoint,
                                                 destPoint,
                                                 maxDelta);

    EXPECT_EQ(expectedPoint, actualPoint);
}

TEST(MathGeometry, BoundedPoint_MaxDeltaBiggerThanChange_ProperPoint) {
    const int size = 3;

    double maxDelta = 10.0f;

    double dataSrc[size] = {2.0f, 2.0f, 2.0f};
    double dataDest[size] = {3.0f, 8.8f, 10.0f};

    double dataExpected[size] = {3.0f, 8.8f, 10.0f};

    Point<double> srcPoint(dataSrc, size);
    Point<double> destPoint(dataDest, size);

    Point<double> expectedPoint(dataExpected, size);

    Point<double> actualPoint = math::boundPoint(srcPoint,
                                                 destPoint,
                                                 maxDelta);

    EXPECT_EQ(expectedPoint, actualPoint);
}
