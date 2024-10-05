#include <iostream>
#include <unistd.h>
#include <cassert>
#include <fcntl.h>
#include "task.hpp"
#include <vector>
#include <stdlib.h>
#include <cstring>
#include <cerrno>
#include <sys/types.h>
#include <sys/wait.h>

Task t;
class ChildProcess
{
public:
    /* data */
    pid_t _ch_pid;
    int _fd;
    // int cnt;

public:
    ChildProcess(int fd, pid_t chpid) : _fd(fd), _ch_pid(chpid){
        // cnt = 1;
    };
    ~ChildProcess(){};
};
static int cnt = 1;
void WaitCommand()
{
    //子进程执行任务
    while(true){
        int command;
        // std::cout << "准备读取管道内容" << std::endl;
        int n = read(0, &command, sizeof(command));
        // std::cout << "成功读取管道内容" << std::endl;
        if (n == sizeof(int))
        {
            t.Execute(command);
        }
        else if (n == 0)
        {
            std::cout << "无内容可读：" << getpid()<< "：我退出了"<< std::endl;
            break;
        }   
        else
        {
            std::cout << errno << strerror(errno) << std::endl;
            break;
        }
    }
};
//创建进程池
void CreatProcess(std::vector<ChildProcess> *procpool) {
    std::vector<int> fds;
    for (int i = 0; i < CNT_PROC; i++)
    {
        int ppfd[2] = {0};
        int pt = pipe(ppfd);
        assert(pt != -1);
        (void)pt;
        pid_t fk = fork();
        assert(fk != -1);
        if (fk == 0)
        {
            // 子进程
            // 关闭写fd
            close(ppfd[1]);
            for (int i = 0; i < fds.size();i++){
                close(fds[i]);
            }
            // 直接从标准输入中读取
            dup2(ppfd[0], 0);
            // 子进程执行命令
            WaitCommand();
            close(ppfd[0]);
            exit(0);
        }
        // 父进程
        close(ppfd[0]);
        procpool->push_back(ChildProcess(ppfd[1], fk));
        fds.push_back(ppfd[1]);
    }
};
int ShowCommand(){
    std::cout<< "###############################"<<std::endl;
    std::cout<< "##   0.打印日志   1.网络请求  #"<<std::endl;
    std::cout<< "##   2.获取消息   3.退出      #"<<std::endl;
    std::cout<< "###############################"<<std::endl;
    std::cout<<"请选择#";
    int command;
    std::cin>>command;
    return command;
}
//进程控制
void CtrlProcess(std::vector<ChildProcess> &procpool){
    while(true){
        //任务类型
        int command = ShowCommand();
        if(command==3)
            break;
        if(command<0 || command > 3)
            continue;
        // 执行任务进程
        int index = rand() % procpool.size();
        std::cout<<"进程编号:"<<cnt++ <<",进程id:"<<procpool[index]._ch_pid<<"执行了"<<command<<std::endl;
        // 执行任务
        write(procpool[index]._fd, &command,sizeof(command));
        // std::cout << "成功输入管道" << std::endl;
        sleep(1);
    }
};
void WaitProcess(std::vector<ChildProcess> &procpool)
{
    for (int i = 0; i < procpool.size();i++){
        //关闭与子进程相关的文件描述符
        close(procpool[i]._fd);
        //进程等待，回收进程
        waitpid(procpool[i]._ch_pid, NULL, 0);
    }
};
int main()
{
    //定义进程池——存储进程及文件描述符
    std::vector<ChildProcess> procpool;
    //1、创建进程池
    CreatProcess(&procpool);

    //2、进程控制
    CtrlProcess(procpool);
    
    //3、回收进程
    WaitProcess(procpool);
    return 0;
}