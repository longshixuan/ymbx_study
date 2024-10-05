#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <cassert>



// void myhandler(int signo){
//     std::cout << "signal:" << signo << ",pid:"<<getpid()<<std::endl;
//     // exit(1);
// }
// int main()
// {
//     // signal(SIGSEGV, myhandler);
//     int *p = nullptr;
//     // p = (int*)100;
//     *p = 100;
//     // sigset_t

//     // char arr[5] = {0};
//     // std::cout << arr[5] << std::endl;
//     // arr[5] = 'a';

//     // while (true)
//     // {
//     //     /* code */
//     //     std::cout << "i am running"
//     //               << ":pid:" << getpid() << std::endl;
//     // }

//     return 0;
// }

sigset_t set2,set3, oldset;
sigset_t pending;


static void printSig(const sigset_t &pending)
{
    for (int signum = 1; signum < 32; signum++)
    {
        if (sigismember(&pending, signum))
            std::cout << "1";
        else
            std::cout << "0";
    }
    std::cout << std::endl;
}
static void handler(int signo){
    std::cout << "mysigal:" << signo << std::endl;
    // std::cout << "pending2表：" << std::endl;
    int cnt = 20;
    while(cnt--){
        int ret = sigpending(&pending);
        printSig(pending);
        sleep(1);
    }
}
static void handler3(int signo){
    std::cout << "pending3表：" << std::endl;
    int ret = sigpending(&pending);
    printSig(pending);
}

//屏蔽信号集
int main()
{
    std::cout << getpid() << std::endl;
    struct sigaction act, oldact;
    act.sa_flags = 0;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, 4);
    sigaction(SIGINT, &act, &oldact);
    while(true){

    }
    // signal(SIGINT, handler);
    // signal(3, handler3);
    // 1、屏蔽2号信号集
    // sigset_t set,oldset;
    // sigemptyset(&set);
    // sigaddset(&set, SIGINT);
    // sigaddset(&set, 3);
    // sigprocmask(SIG_BLOCK, &set, &oldset);
    // sigset_t pending;
    // int ret = sigpending(&pending);
    // sigemptyset(&set2);
    // sigaddset(&set2, SIGINT);
    // sigaddset(&set2, 3);
    // sigprocmask(SIG_BLOCK, &set2, &oldset);
    // // sigprocmask(SIG_BLOCK, &set3, nullptr);

    // // 2、不断打印pending表
    // int cnt = 0;
    // while (true)
    // {
    //     // sigset_t pending;
    //     int ret = sigpending(&pending);
    //     assert(ret == 0);
    //     (void)ret;
    //     printSig(pending);
    //     sleep(1);

    //     if(cnt++==10){
    //         sigprocmask(SIG_UNBLOCK, &set2, nullptr);

    //         // break;
    //     }
    // }
    return 0;
}