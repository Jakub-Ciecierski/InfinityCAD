//
// Created by jakub on 11/10/15.
//

#include <gtest/gtest.h>

#include <stdexcept>
#include <math/point.h>

/*
 * Testing default constructor
 */
TEST(MathPoint, Default_Constructor_Size_Empty) {
    int expectedValue, actualValue;
    Point<double> p;

    actualValue = p.size();
    expectedValue = 0;

    EXPECT_EQ(expectedValue, actualValue);
}

/*
 * Testing size constructor
 */
TEST(MathPoint, Size_Constructor_Size_Defined) {
    int expectedValue, actualValue;
    int size;

    size = 15;

    Point<double> p(size);

    actualValue = p.size();
    expectedValue = size;

    EXPECT_EQ(expectedValue, actualValue);
}

/*
 * Testing constructor with data elements, the size
 */
TEST(MathPoint, Data_Constructor_Size_Defined) {
    int expectedValue, actualValue;

    const int size = 15;
    int data[size];

    for(int i = 0; i < size; i++){
        data[i] = i;
    }

    Point<int> p(data, size);

    actualValue = p.size();
    expectedValue = size;

    EXPECT_EQ(expectedValue, actualValue);
}

/*
 * Testing accessor operator, data must match
 */
TEST(MathPoint, Accessor_Operator_Data) {
    int expectedValue, actualValue;

    const int size = 15;
    int data[size];

    for(int i = 0; i < size; i++){
        data[i] = i;
    }

    Point<int> p(data, size);

    for(int i = 0; i < size; i++){
        actualValue = p[i];
        expectedValue = i;

        EXPECT_EQ(expectedValue, actualValue);
    }
}

/*
 * Testing adding dimension
 */
TEST(MathPoint, Add_Dimension_New_Size) {
    int expectedValue, actualValue;

    const int size = 15;

    Point<int> p(size);

    p.addDimension();

    expectedValue = size + 1;
    actualValue = p.size();

    EXPECT_EQ(expectedValue, actualValue);
}

/*
 * Testing adding dimensions
 */
TEST(MathPoint, Add_Dimensions_New_Size) {
    int expectedValue, actualValue;

    const int size = 15;
    const int dimsToAdd = 14;

    Point<int> p(size);

    p.addDimensions(14);

    expectedValue = size + dimsToAdd;
    actualValue = p.size();

    EXPECT_EQ(expectedValue, actualValue);
}


/*
 * Testing adding dimension with specific value
 */
TEST(MathPoint, Add_Dimension_New_Element) {
    int expectedValue, actualValue;

    const int size = 15;
    int elementToAdd = -1;

    Point<int> p(size);

    for(int i = 0; i < size; i++){
        p[i] = i;
    }
    p.addDimension(elementToAdd);

    int newSize = p.size();

    expectedValue = elementToAdd;
    actualValue = p[newSize - 1];

    EXPECT_EQ(expectedValue, actualValue);
}

/*
 * Testing accessor operator out of bound
 */
TEST(MathPoint, Accessor_Operator_Out_Of_Bound) {
    const int size = 15;

    Point<int> p(size);

    EXPECT_THROW(p[size], std::invalid_argument);
    EXPECT_THROW(p[-1], std::invalid_argument);
}


/*
 * Testing adding two pointers
 */
TEST(MathPoint, Add_Points_New_Point) {
    int expectedValue, actualValue;

    const int size = 15;

    Point<int> p1(size);
    Point<int> p2(size);

    for(int i = 0; i < size; i++){
        p1[i] = i;
        p2[i] = -i;
    }

    Point<int> newPoint = p1 + p2;

    for(int i = 0; i < size; i++){
        actualValue = newPoint[i];
        expectedValue = 0;

        EXPECT_EQ(expectedValue, actualValue);
    }
}

/*
 * Testing adding two pointers
 */
TEST(MathPoint, Add_Equal_Operator_Points_New_Point) {
    const int size = 5;

    int data1[size] = {3, 4, 5, 6, 7};
    int data2[size] = {6, 8, 10, 12, 14};

    Point<int> actualPoint(data1, size);
    Point<int> expectedPoint(data2, size);

    actualPoint += actualPoint;

    EXPECT_EQ(expectedPoint, actualPoint);

}


/*
 * Testing subtracting two pointers
 */
TEST(MathPoint, Subtract_Points_New_Point) {
    int expectedValue, actualValue;

    const int size = 15;

    Point<int> p1(size);
    Point<int> p2(size);

    for(int i = 0; i < size; i++){
        p1[i] = i;
        p2[i] = -i;
    }

    Point<int> newPoint = p1 - p2;

    for(int i = 0; i < size; i++){
        actualValue = newPoint[i];
        expectedValue = i+i;
        EXPECT_EQ(expectedValue, actualValue);
    }
}

/*
 * Testing equality of two points
 */
TEST(MathPoint, Equal_Points) {
    Point<int> expectedPoint, actualPoint;

    const int size = 15;

    Point<int> p1(size);
    Point<int> p2(size);

    for(int i = 0; i < size; i++){
        p1[i] = i;
        p2[i] = i;
    }

    EXPECT_EQ(p1, p2);
}

/*
 * Testing not equality of two points
 */
TEST(MathPoint, Not_Equal_Points) {
    Point<int> expectedPoint, actualPoint;

    const int size = 15;

    Point<int> p1(size);
    Point<int> p2(size);

    for (int i = 0; i < size; i++) {
        p1[i] = i;
        p2[i] = -i;
    }

    EXPECT_NE(p1, p2);

}

/*
 * Testing
 */
TEST(MathPoint, Divider_Equality_Operator_Equal) {
    int divider = 3;
    const int size = 15;

    Point<int> expectedPoint(size);
    Point<int> actualPoint(size);

    for(int i = 0; i < size; i++){
        actualPoint[i] = 9;
    }
    actualPoint /= divider;

    for(int i = 0; i < size; i++){
        expectedPoint[i] = divider;
    }

    EXPECT_EQ(expectedPoint, actualPoint);
}

/*
 * Testing
 */
TEST(MathPoint, Divider_Operator_Equal) {
    int divider = 3;
    const int size = 15;

    Point<int> expectedPoint(size);
    Point<int> actualPoint(size);

    for(int i = 0; i < size; i++){
        actualPoint[i] = 9;
    }
    actualPoint = actualPoint / divider;

    for(int i = 0; i < size; i++){
        expectedPoint[i] = divider;
    }

    EXPECT_EQ(expectedPoint, actualPoint);
}

/*
 * Testing
 */
TEST(MathPoint, Sum_Operator_Equal) {
    int summer = 3;
    const int size = 5;

    int data1[size] = {0, 0, 0, 0, 0};
    int data2[size] = {3, 3, 3, 3, 3};

    Point<int> actualPoint(data1, size);
    Point<int> expectedPoint(data2, size);

    actualPoint = actualPoint + summer;


    EXPECT_EQ(expectedPoint, actualPoint);
}

/*
 * Testing
 */
TEST(MathPoint, Sum_Equal_Operator_Equal) {
    int summer = 3;
    const int size = 5;

    int data1[size] = {0, 0, 0, 0, 0};
    int data2[size] = {3, 3, 3, 3, 3};

    Point<int> actualPoint(data1, size);
    Point<int> expectedPoint(data2, size);

    actualPoint += summer;
    EXPECT_EQ(expectedPoint, actualPoint);
}

TEST(MathPoint, PointDouble_NoValueSpecified_ExpectedZeroInEachDimension) {
    const int size = 5;

    Point<double> point(size);

    double expectedValue = 0;

    for(int i = 0 ; i < size; i++){
        double actualValue = point[i];
        EXPECT_EQ(expectedValue, actualValue);
    }
}