//
// Created by jakub on 11/23/15.
//

#include "string_utils.h"

namespace str_util
{

    std::vector<std::string> splitString(std::string s,
                                         std::string delimiter) {
        std::vector<std::string> tokens;

        size_t pos = 0;
        std::string token;
        while ((pos = s.find(delimiter)) != std::string::npos) {
            token = s.substr(0, pos);
            tokens.push_back(token);
            s.erase(0, pos + delimiter.length());
        }
        tokens.push_back(s);
        return tokens;
    }

    std::string makePath(std::string dir, std::string appendToDir){
        std::string path;
        std::string PATH_SEPARATOR = "/";

        path += dir + PATH_SEPARATOR + appendToDir;
        return path;
    }

    std::string makePath(const std::vector<std::string>& toPath){
        std::string path;
        std::string PATH_SEPARATOR = "/";

        for(unsigned int i = 0; i < toPath.size(); i++){
            path += toPath[i] + PATH_SEPARATOR;
        }
        return path;
    }

    std::string replaceChar(std::string str, char ch1, char ch2) {
        for (int i = 0; i < str.length(); ++i) {
            if (str[i] == ch1)
                str[i] = ch2;
        }

        return str;
    }
}