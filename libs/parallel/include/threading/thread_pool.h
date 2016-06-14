//
// Created by jakub on 11/24/15.
//

#ifndef AC_THREADPOOL_H
#define AC_THREADPOOL_H

#include <pthread.h>
#include <vector>

/*
 * ThreadPool creates and destroys threads.
 *
 */
namespace threading
{
    extern const unsigned int T_BOSS;

    class ThreadPool {
    private:
        // The actual threads
        std::vector<pthread_t> threads;

        unsigned int threadCount;

    public:

        ThreadPool();
        ThreadPool(unsigned int threadCount);

        ~ThreadPool();

        // Mutex to synchronize all threads
        pthread_mutex_t mutex;
        pthread_barrier_t barrier;

        unsigned int size() const;

        /*
         * Synchronizes all threads in the pool.
         * Each calling thread stops and wait until all
         * threads in the pool reach this point
         */
        void synchronizeAll();

        /*
         * Calling thread locks access for entire thread pool until
         * unlockAll has been called.
         */
        void lockAll();

        /*
         * Unlocks access from all threads.
         */
        void unlockAll();

        /*
         * Create a thread which is stored in vector of threads
         */
        void createThread(void *(*func)(void *), void *argv);

        /*
         * Wait for a thread with given id
         */
        void join(unsigned int id);

        /*
         * Waits for all threads in vector of threads.
         */
        void joinAll();
    };
}

#endif //AC_THREADPOOL_H
