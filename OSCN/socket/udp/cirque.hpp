#pragma once

#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <vector>

#define gcap 5
namespace ymbx
{
    template <class T>
    class cirQue
    {
    private:
        void Lock(pthread_mutex_t &m)
        {
            pthread_mutex_lock(&m);
        }
        void Unlock(pthread_mutex_t &m)
        {
            pthread_mutex_unlock(&m);
        }

    public:
        cirQue(size_t cap = gcap) : _q(cap), _capacity(cap), _c_idx(0), _p_idx(0)
        {
            sem_init(&_p_sem, 0, _capacity);
            sem_init(&_c_sem, 0, 0);
            pthread_mutex_init(&_c_mutex, nullptr);
            pthread_mutex_init(&_p_mutex, nullptr);
        }
        void push(const T &data)
        {
            // 使用信号量时，在进入之前已经预定了资源，所以无需判断
            // 临界资源是否被当作整体使用，决定使用锁还是sem
            sem_wait(&_p_sem);
            Lock(_p_mutex);
            // if(_p_idx>=_c_idx && _p_idx<_c_idx+_capacity){
            _q[(_p_idx++) % _capacity] = data;
            Unlock(_p_mutex);
            // }
            sem_post(&_c_sem);
        }
        void pop(T *data)
        {
            sem_wait(&_c_sem);
            Lock(_c_mutex);
            // if(_c_idx<_p_idx || _c_idx+_capacity>=_p_idx){
            *data = _q[(_c_idx++) % _capacity];
            Unlock(_c_mutex);
            // }
            sem_post(&_p_sem);
        }
        ~cirQue()
        {
            sem_destroy(&_p_sem);
            sem_destroy(&_c_sem);
            pthread_mutex_destroy(&_c_mutex);
            pthread_mutex_destroy(&_p_mutex);
        }

    private:
        std::vector<T> _q;
        size_t _c_idx;
        size_t _p_idx;
        size_t _capacity;
        sem_t _c_sem;
        sem_t _p_sem;
        pthread_mutex_t _c_mutex;
        pthread_mutex_t _p_mutex;
    };
}