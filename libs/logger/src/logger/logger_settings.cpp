//
// Created by jakub on 11/22/15.
//

#include "logger/logger_settings.h"

namespace logger
{
    namespace settings
    {
        //-----------------------------------------------------------//
        //  VARIABLES
        //-----------------------------------------------------------//

        std::string LOG_MAIN_DIR                            = "logs";
        std::string LOG_CURR_DIR                            = "TEST";

        // Name of the default file.
        // The file will be saved in the:
        // LOG_MAIN_DIR/LOG_CURR_DIR directory
        std::string DEFAULT_FILE_NAME                       = "log.txt";

        // Extra file
        File EXTRA_FILE = File();
        // Default File output
        std::ofstream DEFAULT_FILE_STREAM;
        // Extra file output
        std::ofstream EXTRA_FILE_STREAM;

        Verbose DEFAULT_VERBOSE                             = Verbose(INFO_V);
        Verbose CURRENT_VERBOSE                             = Verbose(INFO_V);
        int CURRENT_VERBOSE_DEPTH                           = 0;

        //-----------------------------------------------------------//
        //  CONSTANTS
        //-----------------------------------------------------------//

        // Full path directory to current log dir
        std::string FULL_DIR_STR                            = "";

        /* ------ STYLES ----- */

        // String separator between date variables
        std::string DATE_SEPARATOR                          = "-";
        // String separator between time variables
        std::string TIME_SEPARATOR                          = ":";
        // String separator between path statements
        std::string BETWEEN_SEPARATOR                       = "_";

        // The separator between paths
        std::string PATH_SEPARATOR                          = "/";


        // The intent style between beginning of line and the message itself
        std::string LOG_INTENT                               = "|-----o ";
        std::string LOG_INTENT_WHITESPACE                    = "   ";

    }
}