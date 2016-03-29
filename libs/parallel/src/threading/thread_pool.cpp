//
// Created by jakub on 11/24/15.
//

#include "threading/thread_pool.h"
#include <error.h>
#include <stdexcept>
#include <thread>

using namespace threading;

namespace threading{
    const unsigned int T_BOSS = 0;
}

ThreadPool::ThreadPool() {
    threadCount = 1;
    if(pthread_mutex_init(&mutex, NULL) < 0) ERR("pthread_mutex_init");

    if(pthread_barrier_init(&barrier, NULL, 1) < 0)
        ERR("pthread_barrier_init");
}


ThreadPool::ThreadPool(unsigned int threadCount) {
    this->threadCount = threadCount;
    if(pthread_mutex_init(&mutex, NULL) < 0) ERR("pthread_mutex_init");

    if(pthread_barrier_init(&barrier, NULL, threadCount) < 0)
        ERR("pthread_barrier_init");
}

ThreadPool::~ThreadPool(){
    if(pthread_mutex_destroy(&mutex) < 0) ERR("pthread_mutex_destroy");

    if(pthread_barrier_destroy(&barrier) < 0) ERR("pthread_barrier_destroy");

}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

unsigned int ThreadPool::size() const{
    return threadCount;
}

//-----------------------------------------------------------//

void ThreadPool::synchronizeAll() {
    int ret_val = pthread_barrier_wait(&(this->barrier));
    if(ret_val != 0 && ret_val != PTHREAD_BARRIER_SERIAL_THREAD)
        ERR("pthread_barrier_wait");
}

void ThreadPool::lockAll() {
    if (pthread_mutex_lock(&(this->mutex)) != 0)
        ERR("pthread_mutex_lock");
}

void ThreadPool::unlockAll() {
    if(pthread_mutex_unlock(&(this->mutex)) != 0 )
        ERR("pthread_mutex_unlock");
}

//-----------------------------------------------------------//

void ThreadPool::createThread(void* (*func)(void*), void* argv){
    pthread_t thread;
    if(pthread_create(&thread, NULL, func, argv) < 0)
        ERR("pthread_create");
    threads.push_back(thread);
}

//-----------------------------------------------------------//

void ThreadPool::joinAll() {
    int size = threads.size();

    for(int i = 0; i < size;i++){
        if (pthread_join(threads[i], NULL) != 0)
            ERR("pthread_join");
    }
    threads.clear();
}

void ThreadPool::join(unsigned int id) {
    if(id < 0 || id >= threads.size())
        throw std::invalid_argument("Index out of bound");
    if (pthread_join(threads[id], NULL) != 0)
        ERR("pthread_join");

    threads.erase(threads.begin() + id);
}

//-----------------------------------------------------------//

//-----------------------------------------------------------//
//  PRIVATE METHODS
//-----------------------------------------------------------//