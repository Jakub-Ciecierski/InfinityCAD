//
// Created by jakub on 11/22/15.
//

#ifndef AC_LOGGER_ENGINE_H
#define AC_LOGGER_ENGINE_H

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

#include "logger/logger_settings.h"

/*
 * The engine running logger
 */

namespace logger
{
    namespace engine
    {
        /*
         * Makes full path to log directory
         */
        std::string makeLogPath(const std::string& filename);

        //-----------------------------------------------------------//

        /*
         * Open the extra output file.
         * The ofstream is located in logger_settings
         */
        void openExtraFile(std::string filename);
        /*
         * Close the extra output file.
         * The ofstream is located in logger_settings
         */
        void closeExtraFile();

        //-----------------------------------------------------------//

        /*
         * Prints header of a given verbose lvl
         */
        void printVerboseAndTimeStamp(Verbose verb, int depth);

        /*
         * Prints the intent of given depth
         */
        void printIntent(Verbose verb, int depth);

        /*
         * Prints each line in the string with new intent.
         */
        void printComplexLines(Verbose verb, std::string line);


        /*
         * These two functions are needed since templates are cool !!!
         */
        void printObject(File t);
        void printObject(Verbose t);

        //-----------------------------------------------------------//
        //  TEMPLATES
        //-----------------------------------------------------------//

        /*
         * Checks if is a valid string, array of chars etc.
         */
        template <typename T>
        bool isValidString(T t){
            return (std::is_same<T, std::string>::value
                    || std::is_same<T, const char*>::value
                    || std::is_same<T, char*>::value);
        }

        /*
         * Prints the object to all streams.
         */
        template <typename T>
        void printObject(T t){
            // Split the lines of the string
            if(isValidString(t)){
                std::stringstream ss;
                ss << t;
                std::string line = ss.str();

                printComplexLines(settings::CURRENT_VERBOSE, line);
            }
            else{
                std::cout << t;

                settings::DEFAULT_FILE_STREAM << t;

                if(settings::EXTRA_FILE_STREAM.is_open())
                    settings::EXTRA_FILE_STREAM << t;
            }
        }

        /*
         * The Stop condition for recursivePrint.
         * Recursively print all arguments to all available streams
         */
        template <typename T>
        void recursivePrint(T t)
        {
            printObject(t);

            // Finish with end lines
            std::cout << std::endl;
            settings::DEFAULT_FILE_STREAM << std::endl;
            if(settings::EXTRA_FILE_STREAM.is_open())
                settings::EXTRA_FILE_STREAM << std::endl;
        }

        /*
         * Recursively print all arguments to all available streams
         */
        template<typename T, typename... Args>
        void recursivePrint(T t, Args... args)
        {
            printObject(t);

            // Keep on printing.
            recursivePrint(args...);
        }
    }
}

#endif //AC_LOGGER_ENGINE_H
