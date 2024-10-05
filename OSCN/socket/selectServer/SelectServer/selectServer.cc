#include<iostream>
#include"selectServer.hpp"
#include<memory>
#include"err.hpp"
#include "log.hpp"
#include<string>
#include<stdlib.h>
#include "widget.hpp"


using namespace ymbx;
void Usage(std::string mes)
{
    std::cout << "correct format:\n\t" << mes << " port" << std::endl;
}
int main(int args,char* argv[]){
    // fd_set fd;
    // std::cout << sizeof(fd) * 8 << std::endl; //1024
    if(args!=2){
        Usage(argv[0]);
        LogMessage(Error,PATH)("parameter is error");
    }
    uint16_t port =atoi(argv[1]);
    std::unique_ptr<SelectServer> ups(new SelectServer);
    ups->InitServer();
    ups->Start();
    return 0;
}