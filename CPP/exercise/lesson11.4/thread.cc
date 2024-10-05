#include <iostream>
#include "mythread.hpp"
#include <string>
#include <unistd.h>
#include "mymutex.hpp"
#include <pthread.h>

int tickets = 1000;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// std::string toHex(pthread_t args)
// {
//     // std::string tid = static_cast<const char *>(args);
//     char name[128];
//     snprintf(name, sizeof(name), "0x%x", args);
//     return name;
// }
void threadRoutine(void *args)
{

    std::string msg = static_cast<const char *>(args);
    while (true)
    {
        lockGuard lockGuard(&mutex);
        if (tickets > 0)
        {
            usleep(2000);
            std::cout << msg << "get a ticket:" << tickets-- << std::endl;
        }
        else
        {
            break;
        }
        usleep(200);
    }
}

int main()
{
    Thread t1(1, threadRoutine, (void*)"thread-1");
    Thread t2(2, threadRoutine, (void*)"thread-2");
    Thread t3(3, threadRoutine, (void*)"thread-3");
    Thread t4(4, threadRoutine, (void*)"thread-4");
    // std::cout << t1.threadname() << ",tid:" << t1.threadid() << std::endl;
    // std::cout << t2.threadname() << ",tid:" << t2.threadid() << std::endl;
    // std::cout << t3.threadname() << ",tid:" << t3.threadid() << std::endl;
    // std::cout << t4.threadname() << ",tid:" << t4.threadid() << std::endl;

    t1.run();
    t2.run();
    t3.run();
    t4.run();
    // std::cout << t1.threadname() << ",tid:" << toHex(t1.threadid()) << std::endl;
    // std::cout << t2.threadname() << ",tid:" << t2.threadid() << std::endl;
    // std::cout << t3.threadname() << ",tid:" << t3.threadid() << std::endl;
    // std::cout << t4.threadname() << ",tid:" << t4.threadid() << std::endl;
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    // std::cout << t1.threadname() << ",tid:" << t1.threadid() << std::endl;
    // std::cout << t2.threadname() << ",tid:" << t2.threadid() << std::endl;
    // std::cout << t3.threadname() << ",tid:" << t3.threadid() << std::endl;
    // std::cout << t4.threadname() << ",tid:" << t4.threadid() << std::endl;
    return 0;
}

// using namespace std;
// int tickets = 1000;                                // 全局变量，共享对象
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // 这是我在外边定义的锁

// // class TData
// // {
// // public:
// //     TData(const string &name, pthread_mutex_t *mutex):_name(name), _pmutex(mutex)
// //     {}
// //     ~TData()
// //     {}
// // public:
// //     string _name;
// //     // pthread_mutex_t *_pmutex;
// // };

// void threadRoutine(void *args)
// {
//     std::string message = static_cast<const char *>(args);
//     while (true)
//     {
//         // pthread_mutex_lock(&mutex); // 加锁，是一个让不让你通过的策略
//         {
//             // LockGuard lockguard(&mutex); // RAII 风格的锁
//             if (tickets > 0)
//             {
//                 usleep(2000);
//                 cout << message << " get a ticket: " << tickets-- << endl; // 临界区
//             }
//             else
//             {
//                 break;
//             }
//         }

//         // 我们抢完一张票的时候，我们还要有后续的动作
//         // usleep(13);
//     }
//     // adfasdf
//     // asdfasdf
//     // {
//     //     LockGuard lockguard(&mutex); // RAII 风格的锁
//     //     asdf
//     //     asdf
//     // }
//     // asdfas
//     // dfasd
//     // fasdfasdf
// }

// // void threadRun(void *args)
// // {
// //     std::string message = static_cast<const char *>(args);
// //     int cnt = 10;
// //     while (cnt)
// //     {
// //         cout << "我是一个线程, " << message << ", cnt: " << cnt-- << endl;
// //         sleep(1);
// //     }
// // }

// int main()
// {
//     Thread t1(1, threadRoutine, (void *)"hellobit1");
//     Thread t2(2, threadRoutine, (void *)"hellobit2");
//     Thread t3(3, threadRoutine, (void *)"hellobit3");
//     Thread t4(4, threadRoutine, (void *)"hellobit4");

//     t1.run();
//     t2.run();
//     t3.run();
//     t4.run();

//     t1.join();
//     t2.join();
//     t3.join();
//     t4.join();

//     // cout << "thread name: " << t1.threadname() << " thread id: " << t1.threadid() << ",thread status: " << t1.status() << std::endl;
//     // cout << "thread name: " << t2.threadname() << " thread id: " << t2.threadid() << ",thread status: " << t2.status() << std::endl;

//     // t1.run();
//     // t2.run();
//     // cout << "thread name: " << t1.threadname() << " thread id: " << t1.threadid() << ",thread status: " << t1.status() << std::endl;
//     // cout << "thread name: " << t2.threadname() << " thread id: " << t2.threadid() << ",thread status: " << t2.status() << std::endl;

//     // t1.join();
//     // t2.join();
//     // cout << "thread name: " << t1.threadname() << " thread id: " << t1.threadid() << ",thread status: " << t1.status() << std::endl;
//     // cout << "thread name: " << t2.threadname() << " thread id: " << t2.threadid() << ",thread status: " << t2.status() << std::endl;

//     // pthread_mutex_t mutex;
//     // pthread_mutex_init(&mutex, nullptr);

//     // pthread_t tids[4];
//     // int n = sizeof(tids)/sizeof(tids[0]);
//     // for(int i = 0; i < n; i++)
//     // {
//     //     char name[64];
//     //     snprintf(name, 64, "thread-%d", i+1);
//     //     TData *td = new TData(name, &mutex);
//     //     pthread_create(tids+i, nullptr, threadRoutine, td);
//     // }

//     // for(int i = 0; i < n; i++)
//     // {
//     //     pthread_join(tids[i], nullptr);
//     // }

//     // pthread_mutex_destroy(&mutex);
//     return 0;
// }