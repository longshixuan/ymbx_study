#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>

int ticket = 10000;
pthread_mutex_t mutex;

void *threadRoutine(void *args)
{
    std::string ret = static_cast<const char *>(args);
    while(true)
    {
        pthread_mutex_lock(&mutex);
        if (ticket > 0)
        {
            usleep(2000);
            std::cout << ret << " get a ticket:" << ticket-- << std::endl;
            pthread_mutex_unlock(&mutex);
        }
        else
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
        usleep(2000);
    }
}
int main()
{
    pthread_mutex_init(&mutex, nullptr);
    pthread_t t1, t2, t3;
    pthread_create(&t1, nullptr, threadRoutine, (void *)"thread-1");
    pthread_create(&t2, nullptr, threadRoutine, (void *)"thread-2");
    pthread_create(&t3, nullptr, threadRoutine, (void *)"thread-3");

    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
    pthread_join(t3, nullptr);
    return 0;
}
// //线程局部存储
// __thread int global = 5;
// // pthread_mutex_t
// void *threadRun(void *args)
// {
//     std::string ret = static_cast<const char *>(args);
//     // int cnt = 5;
//     // while (cnt)
//     // {
//     //     // sleep(1);
//     //     std::cout << ret << ",cnt:"<<cnt-- << std::endl;
//     // }
//     // //线程分离可以自己主动分离，也可以由主线程进行分离
//     // pthread_detach(pthread_self());
//     // pthread_exit((void *)4);
//     while (true)
//     {
//         if(global>=0)
//         {
//             sleep(1);
//             std::cout << ret << ":tid:"<<pthread_self()<<"->global:" << global-- <<":&:"<<&global<< std::endl;
//         }
//         else
//             break;
//     }
//     return nullptr;
// }
// int main()
// {
//     pthread_t tid[3];
//     for (int i = 0; i < 3; i++)
//     {
//         char *name=new char[64];
//         snprintf(name, sizeof(name), "pthread-%d", i);
//         pthread_create(&tid[i], nullptr, threadRun, name);
//     }
//     // sleep(1);
//     // pthread_detach(tid[1]);
//     // 主线程死掉，则该进程的所有线程都死掉
//     for (int i = 0; i < 3; i++)
//     {
//         int n = pthread_join(tid[i], nullptr);
//         if (n != 0)
//         {
//             std::cerr << "wait fail" << std::endl;
//         }
//     }
//     // while (true)
//     // {
//     //     sleep(1);
//     // }
//     // sleep(2);
//     // pthread_cancel(tid);
//     return 0;
// }

// void *threadRun(void *args)
// {
//     const char *ret = static_cast<const char *>(args);
//     int cnt = 5;
//     while(cnt)
//     {
//         std::cout << ret << "cnt:" << cnt-- << std::endl;
//         sleep(1);
//         if(cnt==2)
//         {
//             pthread_cancel(getpid());
//         }
//     }
//     pthread_exit((void*)4);
// }
// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid, nullptr, threadRun, (void*)"hello world");
//     //主线程死掉，则该进程的所有线程都死掉
//     while(true)
//     {
//         sleep(1);
//     }
//     // sleep(2);
//     // pthread_cancel(tid);
//     return 0;
// }

// // using namespace std;
// #define NUM 10
// enum
// {
//     OK = 0,
//     WAIT
// };
// class threadData
// {
// public:
//     threadData(const std::string &name, int id, int top) : _name(name), _id(id), _top(top)
//     {
//     }
//     ~threadData() {}

// public:
//     // 输入型参数
//     std::string _name;
//     int _id;
//     int _top;

//     // 输出型参数
//     // int _status;
//     int _result;
// };
// void *pthreadRun(void *args)
// {
//     // 执行操作
//     threadData *ret = static_cast<threadData *>(args);
//     // sleep(3);
//     std::cout << ret->_name << "->id:" << pthread_self() << std::endl;
//     for (int i = 1; i <= ret->_top; ++i)
//     {
//         ret->_result += i;
//     }
//     // std::cout << "compute done" << std::endl;
//     // ret->_status = OK;
//     pthread_exit(ret);
// }

// int main()
// {
//     pthread_t ptid[NUM];
//     for (int i = 0; i < NUM; ++i)
//     {
//         char tname[64];
//         snprintf(tname, sizeof(tname), "pthreadid:%d", i);
//         threadData *td = new threadData(tname, i, 10 + i);
//         // 创建线程
//         pthread_create(ptid + i, nullptr, pthreadRun, td);
//         sleep(1);
//     }

//     // 线程等待
//     // sleep(20);
//     void *val = nullptr;
//     for (int i = 0; i < NUM; ++i)
//     {
//         int n=pthread_join(ptid[i], &val);
//         if(n!=0)
//             std::cerr << "wait error" << std::endl;
//         // 回收结果
//         //  if(val->_status==OK){
//         threadData *td = static_cast<threadData *>(val);
//         std::cout << "id:" << td->_id << "->result:" << td->_result << std::endl;
//         delete td;
//         // }
//         // pthread_cancel(ptid[i]);
//         // sleep(3);
//     }
//     while (true)
//     {
//         sleep(1);
//     }
//     return 0;
// }