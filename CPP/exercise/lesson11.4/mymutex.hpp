#pragma once
#include <pthread.h>


class Mutex
{
public:
    Mutex(pthread_mutex_t* pmutex):_pmutex(pmutex){}
    void lock()
    {
        pthread_mutex_lock(_pmutex);
    }
    void unlock()
    {
        pthread_mutex_unlock(_pmutex);
    }
    ~Mutex(){}
private:
    pthread_mutex_t *_pmutex;
};

class lockGuard
{
public:
    lockGuard(pthread_mutex_t*pmutex):_mutex(pmutex){
        _mutex.lock();
    }
    ~lockGuard()
    {
        _mutex.unlock();
    }

private:
    Mutex _mutex;
};
