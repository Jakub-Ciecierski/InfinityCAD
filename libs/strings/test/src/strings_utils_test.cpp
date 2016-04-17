//
// Created by jakub on 11/23/15.
//


#include <gtest/gtest.h>

#include <vector>
#include <thread>
#include "string_utils.h"

//-----------------------------------------------------------//
//  DECLARATIONS
//-----------------------------------------------------------//



//-----------------------------------------------------------//
//  DEFINITIONS
//-----------------------------------------------------------//


//-----------------------------------------------------------//
//  TESTS
//-----------------------------------------------------------//


TEST(StringUtils, SplitString) {

    std::vector<std::string> actualValues;

    actualValues.push_back("My name");
    actualValues.push_back("is");
    actualValues.push_back("Kuba");

    std::string delimiter = "\n";
    std::string s = actualValues[0] + delimiter
                + actualValues[1] + delimiter
                + actualValues[2];

    std::vector<std::string> tokens;
    tokens = str_util::splitString(s, "\n");

    EXPECT_EQ(tokens.size(), actualValues.size());

    for(unsigned int i = 0; i < tokens.size(); i++){
        EXPECT_EQ(tokens[i], actualValues[i]);
    }
}