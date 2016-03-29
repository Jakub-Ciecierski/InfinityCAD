//
// Created by jakub on 12/3/15.
//


#include <gtest/gtest.h>
#include <math/matrix.h>



//-----------------------------------------------------------//
//  DECLARATIONS
//-----------------------------------------------------------//

void matrixTimeTest();

//-----------------------------------------------------------//
//  DEFINITIONS
//-----------------------------------------------------------//

void matrixTimeTest(){
    unsigned int n = 10000;
    unsigned int m = 10000;

    Matrix<float> matrix(n, m);

    //clk::startClock();
    for(unsigned int i = 0; i < n; i++)
        for(unsigned int j = 0; j < m; j++)
            matrix.getEntry(i, j);

    //double setEntryTime = clk::stopClock();


    //clk::startClock();
    for(unsigned int i = 0; i < n; i++)
        for(unsigned int j = 0; j < m; j++)
            matrix[i][j];

    //double operatorTime = clk::stopClock();

    //std::cout << "Set Entry Time: " << setEntryTime << std::endl;
    //std::cout << "Operator Time: " << operatorTime << std::endl;
}

//-----------------------------------------------------------//
//  TESTS
//-----------------------------------------------------------//

TEST(Math, Matrix_TimeTest) {
    //matrixTimeTest();
}

TEST(Math, Matrix_AccessorOperator) {
    unsigned int n = 3;
    unsigned int m = 2;

    float expectedValue = 0.5f;

    Matrix<float> matrix(n, m);

    matrix[0][0] = expectedValue;

    EXPECT_EQ(expectedValue, matrix[0][0]);
}

TEST(Math, Matrix_SetValue_GetValue_Same) {
    unsigned int n = 3;
    unsigned int m = 2;

    float expectedValue = 0.5f;

    Matrix<float> matrix(n, m);

    for(unsigned int i = 0; i < n; i++)
        for(unsigned int j = 0; j < m; j++)
            matrix.setEntry(i, j, expectedValue);

    for(unsigned int i = 0; i < n; i++)
        for(unsigned int j = 0; j < m; j++)
            EXPECT_EQ(expectedValue, matrix[i][j]);
}
