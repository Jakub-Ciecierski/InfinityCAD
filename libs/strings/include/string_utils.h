//
// Created by jakub on 11/23/15.
//

#ifndef AC_STRING_UTILS_H
#define AC_STRING_UTILS_H

#include <string>
#include <vector>

namespace str_util
{
    /*
     * Splits the string s into tokens, substrings which
     * are seperated by delimiter.
     */
    std::vector<std::string> splitString(std::string s,
                                         std::string delimiter);

    /*
     * Creates a correct string defining a PATH
     */
    std::string makePath(std::string dir, std::string appendToDir);

    /*
     * Creates a correct string defining a PATH
     */
    std::string makePath(const std::vector<std::string>& toPath);

    std::string replaceChar(std::string str, char ch1, char ch2);
}


#endif //AC_STRING_UTILS_H