#pragma once
#include <cassert>
#include <cstdlib>
#include<functional>
#include <iostream>
#include <pthread.h>

namespace ymbx
{
    class Thread
    {
    public:
        enum
        {
            NEW,
            RUNNING,
            EXITED
        };
        // typedef void (*func)();
        using func = std::function<void()>;

    public:
        Thread(int num, func fun) : _fun(fun), _status(NEW)
        {
            // std::cout << "thread-" << _num << "，tid:" << _tid;
            char name[128];
            snprintf(name, sizeof(name), "thread-%d", num);
            _name = name;
        }
        pthread_t threadid()
        {
            if (_status == RUNNING)
            {
                return _tid;
            }
            else
            {
                return 0;
            }
        }
        std::string threadname() { return _name; }

        static void *runhelper(void *args)
        {
            Thread *ts = static_cast<Thread *>(args);
            (*ts)();
            return nullptr;
        }
        void operator()() // 仿函数
        {
            if (_fun != nullptr)
                _fun();
        }
        void run()
        {
            int n = pthread_create(&_tid, nullptr, runhelper, (void *)this);
            // assert(n == 0);
            if (n != 0)
            {
                std::cout << "create error" << std::endl;
                exit(1);
            }
            _status = RUNNING;
        }
        void join()
        {
            int n = pthread_join(_tid, nullptr);
            // assert(n == 0);
            if (n != 0)
            {
                std::cout << "join error" << std::endl;
            }
            _status = EXITED;
        }
        ~Thread()
        {
        }

    private:
        pthread_t _tid;
        int _status;
        func _fun;
        // void *_args;
        std::string _name;
    };
}

// class Thread
// {
// public:
//     typedef enum
//     {
//         NEW = 0,
//         RUNNING,
//         EXITED
//     } ThreadStatus;
//     typedef void (*func_t)(void *);

// public:
//     Thread(int num, func_t func, void *args) : _tid(0), _status(NEW), _func(func), _args(args)
//     {
//         char name[128];
//         snprintf(name, sizeof(name), "thread-%d", num);
//         _name = name;
//     }
//     int status() { return _status; }
//     std::string threadname() { return _name; }
//     pthread_t threadid()
//     {
//         if (_status == RUNNING)
//             return _tid;
//         else
//         {
//             return 0;
//         }
//     }
//     // 是不是类的成员函数，而类的成员函数，具有默认参数this,需要static
//     // 但是会有新的问题：static成员函数，无法直接访问类属性和其他成员函数
//     static void *runHelper(void *args)
//     {
//         Thread *ts = (Thread *)args; // 就拿到了当前对象
//         // _func(_args);
//         (*ts)();
//         return nullptr;
//     }
//     void operator()() // 仿函数
//     {
//         if (_func != nullptr)
//             _func(_args);
//     }
//     void run()
//     {
//         int n = pthread_create(&_tid, nullptr, runHelper, this);
//         if (n != 0)
//             exit(1);
//         _status = RUNNING;
//     }
//     void join()
//     {
//         int n = pthread_join(_tid, nullptr);
//         if (n != 0)
//         {
//             std::cerr << "main thread join thread " << _name << " error" << std::endl;
//             return;
//         }
//         _status = EXITED;
//     }
//     ~Thread()
//     {
//     }

// private:
//     pthread_t _tid;
//     std::string _name;
//     func_t _func; // 线程未来要执行的回调
//     void *_args;
//     ThreadStatus _status;
// };