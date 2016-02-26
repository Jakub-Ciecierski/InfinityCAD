//
// Created by jakub on 11/15/15.
//

#include <gtest/gtest.h>
#include <clock.h>

//-----------------------------------------------------------//
//  DECLARATIONS
//-----------------------------------------------------------//

/*
 * Shows how to measure time using clock.h
 */
void measureTimeTest();

//-----------------------------------------------------------//
//  DEFINITIONS
//-----------------------------------------------------------//

void measureTimeTest(){
    const int c = 10000 * 10000;

    clk::startClock();

    for(int i = 0; i < c; i++){
        ;
    }

    double seconds = clk::stopClock();

    std::cout << "Time finished after: "
        << seconds << " seconds"  << std::endl;
}

//-----------------------------------------------------------//
//  TESTS
//-----------------------------------------------------------//

TEST(Clock, HowTo_MeasureTime) {
    //measureTimeTest();
}

TEST(Clock, StopClock_Before_StartClock_Value_Minus1) {
    double expectedValue = -1;
    double actualSeconds = clk::stopClock();

    EXPECT_EQ(expectedValue, actualSeconds);
}

TEST(Clock, StartClock_StopClock_Value_Not_Minus1) {
    double not_expectedValue = -1;

    clk::startClock();
    double actualSeconds = clk::stopClock();

    EXPECT_NE(not_expectedValue, actualSeconds);
}
