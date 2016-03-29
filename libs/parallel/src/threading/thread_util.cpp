//
// Created by jakub on 12/1/15.
//

#include "threading/thread_util.h"

namespace threading
{
    int getNumberOfCores(){
        return std::thread::hardware_concurrency();
    }
}
