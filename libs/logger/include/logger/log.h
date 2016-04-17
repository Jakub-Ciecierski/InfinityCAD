//
// Created by jakub on 11/22/15.
//

#ifndef AC_LOG_H
#define AC_LOG_H

#include <string>
#include <iostream>
#include <vector>
#include <fstream>

#include "logger/logger_settings.h"
#include "logger/logger_engine.h"

namespace logger
{
    /*
     * Initiates the log functionality.
     */
    void initLog();

    void closeLog();

    void makeDir(std::string dir);

    // -------------------------------------------------------------------------

    /*
     * Logs arguments to default file and cout
     */
    template<typename T, typename... Args>
    inline void log(T t, Args... args)
    {
        settings::CURRENT_VERBOSE = settings::DEFAULT_VERBOSE;
        settings::CURRENT_VERBOSE_DEPTH = 0;

        engine::printVerboseAndTimeStamp(settings::DEFAULT_VERBOSE, 0);
        engine::recursivePrint(t, args...);


        settings::CURRENT_VERBOSE_DEPTH = 0;
    }

    /*
     * Logs arguments to File f, default file and cout
     */
    template<typename... Args>
    inline void log(File f, Args... args)
    {
        settings::CURRENT_VERBOSE = settings::DEFAULT_VERBOSE;
        settings::CURRENT_VERBOSE_DEPTH = 0;

        engine::openExtraFile(f.name);
        settings::EXTRA_FILE = f;

        engine::printVerboseAndTimeStamp(settings::DEFAULT_VERBOSE, 0);
        engine::recursivePrint(args...);

        engine::closeExtraFile();

        settings::CURRENT_VERBOSE_DEPTH = 0;
    }

    /*
     * Logs arguments to default file and cout
     */
    template<typename... Args>
    inline void log(Verbose verb, Args... args)
    {
        settings::CURRENT_VERBOSE = verb;
        settings::CURRENT_VERBOSE_DEPTH = 0;

        engine::printVerboseAndTimeStamp(verb, 0);
        engine::recursivePrint(args...);

        settings::CURRENT_VERBOSE_DEPTH = 0;
    }

    template<typename... Args>
    inline void log(File f, Verbose verb, Args... args)
    {
        settings::CURRENT_VERBOSE = verb;
        settings::CURRENT_VERBOSE_DEPTH = 0;

        engine::openExtraFile(f.name);
        settings::EXTRA_FILE = f;

        engine::printVerboseAndTimeStamp(verb, 0);
        engine::recursivePrint(args...);

        settings::CURRENT_VERBOSE_DEPTH = 0;

        engine::closeExtraFile();
    }

    template<typename... Args>
    inline void log(Verbose verb, File f, Args... args)
    {
        settings::CURRENT_VERBOSE = verb;
        settings::CURRENT_VERBOSE_DEPTH = 0;

        engine::openExtraFile(f.name);
        settings::EXTRA_FILE = f;

        engine::printVerboseAndTimeStamp(verb, 0);
        engine::recursivePrint(args...);

        settings::CURRENT_VERBOSE_DEPTH = 0;

        engine::closeExtraFile();
    }
}

#endif //AC_LOGGER2_H
