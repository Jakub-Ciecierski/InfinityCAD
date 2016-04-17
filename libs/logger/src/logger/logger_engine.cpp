//
// Created by jakub on 11/22/15.
//

#include "logger/logger_engine.h"
#include <string_utils.h>
#include <clock.h>

namespace logger
{
    namespace engine
    {
        std::string makeLogPath(const std::string& filename) {
            std::string fullFileName =
                    settings::FULL_DIR_STR
                           + settings::PATH_SEPARATOR
                           + filename;
            return fullFileName;
        }

        //-----------------------------------------------------------//

        void openExtraFile(std::string filename) {
            std::string fileName = makeLogPath(filename);
            settings::EXTRA_FILE_STREAM.open(fileName, std::ofstream::app);
        }

        void closeExtraFile() {
            settings::EXTRA_FILE_STREAM.close();
        }

        //-----------------------------------------------------------//

        void printVerboseAndTimeStamp(Verbose verb, int depth){
            // STDOUT
            std::string getTimeStr =
                clk::currentTimeString(settings::TIME_SEPARATOR);
            std::cout
                << verb.color
                << "[" << verb.name << "]"
                << colors::CLR

                << colors::CYAN
                << " [" << getTimeStr << "]"
                << colors::CLR;


            // DEFAULT FILE
            settings::DEFAULT_FILE_STREAM
                << "[" << verb.name << "]"
                << " [" << getTimeStr << "]";

            // EXTRA FILE
            if(settings::EXTRA_FILE_STREAM.is_open()){
                settings::EXTRA_FILE_STREAM
                    << "[" << verb.name << "]"
                    << " [" << getTimeStr << "]";
            }

            printIntent(verb, depth);
        }

        void printIntent(Verbose verb, int depth){
            // STDOUT
            std::cout
                << "\n"
                << verb.color
                << settings::LOG_INTENT_WHITESPACE;
            for(int i = 0; i < depth; i++)
                std::cout << settings::LOG_INTENT_WHITESPACE;
            std::cout << settings::LOG_INTENT
                <<colors::CLR;

            // DEFAULT FILE
            settings::DEFAULT_FILE_STREAM
                << "\n"
                << settings::LOG_INTENT_WHITESPACE;
            for(int i = 0; i < depth; i++)
                settings::DEFAULT_FILE_STREAM<< settings::LOG_INTENT_WHITESPACE;
            settings::DEFAULT_FILE_STREAM << settings::LOG_INTENT;

            // EXTRA FILE
            if(settings::EXTRA_FILE_STREAM.is_open()) {
                settings::EXTRA_FILE_STREAM
                    << "\n";
                if (settings::EXTRA_FILE.doStyle){
                    settings::EXTRA_FILE_STREAM
                        << settings::LOG_INTENT_WHITESPACE;
                    for (int i = 0; i < depth; i++)
                        settings::EXTRA_FILE_STREAM
                        << settings::LOG_INTENT_WHITESPACE;
                    settings::EXTRA_FILE_STREAM << settings::LOG_INTENT;
                }
            }
        }

        void printComplexLines(Verbose verb, std::string line){
            ++settings::CURRENT_VERBOSE_DEPTH;

            std::vector<std::string> tokens = str_util::splitString(line,"\n");

            for(unsigned int i = 0; i < tokens.size(); i++){
                if(i!=0)
                    printIntent(verb, settings::CURRENT_VERBOSE_DEPTH);
                // STDOUT
                std::cout << tokens[i];
                // DEFAULT FILE
                settings::DEFAULT_FILE_STREAM << tokens[i];
                // EXTRA FILE
                if (settings::EXTRA_FILE_STREAM.is_open()){
                        settings::EXTRA_FILE_STREAM << tokens[i];
                }
            }
        }

        void printObject(File t){}
        void printObject(Verbose t){}
    }
}