//
// Created by jakub on 11/22/15.
//

#include <gtest/gtest.h>
#include <logger/log.h>
#include <vector>

//-----------------------------------------------------------//
//  DECLARATIONS
//-----------------------------------------------------------//


void howToLog();

//-----------------------------------------------------------//
//  DEFINITIONS
//-----------------------------------------------------------//

void howToLog(){
    std::string test = "Default\nVerbose";
    std::string test2 = "Lol\nlol1";

    logger::initLog();

    logger::log(Verbose(PSO_V), File("test.txt"),test ,3, 2, 3.0f, test2);
    logger::log(Verbose(PSO_V), test ,3, 2, 3.0f, test2);
    logger::log(File("test.txt"),test ,3, 2, 3.0f, test2);
    logger::log(test ,3, 2, 3.0f, test2, " Constant char");

    logger::closeLog();
}

//-----------------------------------------------------------//
//  TESTS
//-----------------------------------------------------------//

TEST(Logger, Init) {

}