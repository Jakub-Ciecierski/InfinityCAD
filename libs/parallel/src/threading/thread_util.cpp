//
// Created by jakub on 12/1/15.
//

#include <unistd.h>
#include "threading/thread_util.h"

namespace threading
{
    int getNumberOfCores(){
        return std::thread::hardware_concurrency();
    }

    void threadSleep(int seconds){
        sleep(seconds);
    }
}
