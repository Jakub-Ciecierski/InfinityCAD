//
// Created by jakub on 12/1/15.
//

#ifndef AC_THREAD_UTIL_H
#define AC_THREAD_UTIL_H

#include <thread>

/*
 * thread_util contains utility functions for thread management
 */
namespace threading
{
    /*
     * Sets the TRUE_THREAD_COUNT in global settings, according to the rules
     * written in the settings header.
     */
    void setTrueNumberOfThreads();

    /*
     * Return number of cores in the CPU.
     * Used to measures how many threads should be activated
     */
    int getNumberOfCores();

    void threadSleep(int seconds);
}


#endif //AC_THREAD_UTIL_H
