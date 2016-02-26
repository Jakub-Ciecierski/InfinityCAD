//
// Created by jakub on 11/15/15.
//

#include <iostream>
#include "clock.h"

namespace clk
{
    // The time in seconds
    double g_prevSecond = -1;

    //-----------------------------------------------------------//

    /*
     * Starts the clock
     */
    void startClock(){
        g_prevSecond = currentSecond();
    }

    /*
     * Stops the clock.
     */
    double stopClock(){
        // If g_prevSecond was not initiated return -1
        if (g_prevSecond == -1)
            return -1;

        // Calculate delta time
        double currSecond = currentSecond();
        double delta = (currSecond - g_prevSecond);

        // Set the value back to -1
        g_prevSecond = -1;

        return delta;
    }


    std::string currentFullDateString(std::string date_sep,
                                      std::string between_sep,
                                      std::string time_sep) {
        // get time now
        time_t t = time(0);
        struct tm *now = localtime(&t);
        std::string timeStr;

        // Year
        timeStr += std::to_string(now->tm_year + 1900)
                   + date_sep;
        // Month
        timeStr += std::to_string(now->tm_mon + 1)
                   + date_sep;
        // Day
        timeStr += std::to_string(now->tm_mday);

        timeStr += between_sep;

        // Hour
        timeStr += std::to_string(now->tm_hour)
                   + time_sep;
        // Minute
        timeStr += std::to_string(now->tm_min)
                   + time_sep;
        // Second
        timeStr += std::to_string(now->tm_sec);

        return timeStr;
    }

    std::string currentTimeString(std::string time_sep) {
        // get time now
        time_t t = time(0);
        struct tm *now = localtime(&t);
        std::string timeStr;

        // Hour
        timeStr += std::to_string(now->tm_hour)
                   + time_sep;
        // Minute
        timeStr += std::to_string(now->tm_min)
                   + time_sep;
        // Second
        timeStr += std::to_string(now->tm_sec);

        return timeStr;
    }


    //-----------------------------------------------------------//

    /*
     * CurrentSecond()
     */
    /* WINDOWS */
    #if defined(_WIN32)
        #if !defined(WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
    double currentSecond(void)
    {
        LARGE_INTEGER t;
        static double oofreq;
        static int checkedForHighResTimer;
        static BOOL hasHighResTimer;

        if (!checkedForHighResTimer) {
            hasHighResTimer = QueryPerformanceFrequency(&t);
            oofreq = 1.0 / (double)t.QuadPart;
            checkedForHighResTimer = 1;
        }
        if (hasHighResTimer) {
            QueryPerformanceCounter(&t);
            return (double)t.QuadPart * oofreq;
        }
        else {
            return (double)GetTickCount();
        }
    }

    /* LINUX */
    #elif defined(__linux__) || defined(__APPLE__)

    #include <stddef.h>
    #include <sys/time.h>

    double currentSecond(void) {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return (double) tv.tv_sec + (double) tv.tv_usec / 1000000.0;
    }

    /* UNKNOWN */
    #else
    #error unsupported platform
    #endif

    //-----------------------------------------------------------//
}