#pragma once

#define CNT_PROC 5
#include <iostream>
#include <vector>

#define COMMAND_LOG 0
#define COMMAND_REQUEST 1
#define COMMAND_INFO 2

typedef void (*fun_t)();

void PrintLog(){
    std::cout << "打印日志任务，正在执行..." << std::endl;
}
void NetRequest()
{
    std::cout << "网络请求，正在执行..." << std::endl;
}
void InfoGet()
{
    std::cout << "获取消息，正在执行..." << std::endl;
}

// class Task
// {
// public:
//     std::vector<fun_t> task_list;
// public:
//     Task(){
//         task_list.push_back(PrintLog);
//         task_list.push_back(NetRequest);
//         task_list.push_back(InfoGet);
//     };
//     void Execute(int command){
//         if(command>=0 && command < task_list.size()){
//             task_list[command]();
//         }
//     }
//     ~Task(){};
// };

class Task
{
public:
    Task()
    {};
    void Execute(int command)
    {
        if(command==COMMAND_LOG){
            PrintLog();
        }else if(command==COMMAND_REQUEST){
            NetRequest();
        }else if(command==COMMAND_INFO){
            InfoGet();
        }else
        {}
    }
    void PrintLog()
    {
        std::cout << "打印日志任务，正在执行..." << std::endl;
    }
    void NetRequest()
    {
        std::cout << "网络请求，正在执行..." << std::endl;
    }
    void InfoGet()
    {
        std::cout << "获取消息，正在执行..." << std::endl;
    }
    ~Task(){};
};