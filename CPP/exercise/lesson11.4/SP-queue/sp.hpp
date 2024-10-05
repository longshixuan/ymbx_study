#pragma once
#include <pthread.h>
#include <queue>

#define gcapacity 5
template <class T>
class blockQueue
{
public:
    blockQueue(int capacity=gcapacity):_capacity(capacity)
    {
        pthread_mutex_init(&_mutex,nullptr);
        pthread_cond_init(&_ccond,nullptr);
        pthread_cond_init(&_pcond,nullptr);
    }
    bool isFull() { return _tq.size() == _capacity; }
    bool isEmpty() { return _tq.empty(); }
    void push(const T& t)
    {
        //判断缓冲区中数据是否满即是访问共享资源，需要被保护
        pthread_mutex_lock(&_mutex);
        while(isFull())
        {
            //实现原理：将锁释放掉，然后加入条件变量队列中等待
            //          恢复时，持有锁才能向后运行
            pthread_cond_wait(&_pcond, &_mutex);
        }
        _tq.push(t);
        // if(_tq.size()>=_capacity/2)
        // {
        //     pthread_cond_signal(&_ccond);
        // }
        pthread_cond_signal(&_ccond);
        pthread_mutex_unlock(&_mutex);
    }
    void pop(T* out)
    {
        pthread_mutex_lock(&_mutex);
        while(isEmpty())
        {
            pthread_cond_wait(&_ccond, &_mutex);
        }
        *out = _tq.front();

        //此处传入的是模板类型，并未实例化具体类型
        // (*out)();
        _tq.pop();
        // if(_tq.size()<_capacity/2)
        // {
        //     pthread_cond_signal(&_pcond);
        // }
        pthread_cond_signal(&_pcond);
        pthread_mutex_unlock(&_mutex);
    }
    ~blockQueue()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_ccond);
        pthread_cond_destroy(&_pcond);
    }

private:
    std::queue<T> _tq;
    int _capacity;
    pthread_mutex_t _mutex;
    pthread_cond_t _ccond;
    pthread_cond_t _pcond;
};