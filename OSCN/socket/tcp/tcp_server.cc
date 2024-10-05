#include "tcp_server.hpp"
#include<iostream>
#include<memory>
#include "daemon.hpp"

using namespace std;
using namespace ymbx;

void Usage(char *s)
{
    std::cout << "Usage:\n\t" << s << " port" << std::endl;
}
std::string echoServer(const std::string& message){
    return message;
}
int main(int args,char* argv[])
{
    if(args!=2){
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t port = atoi(argv[1]);
    unique_ptr<tcpServer> ts(new tcpServer(echoServer,port));
    ts->InitServer();
    // Daemon();
    ts->Start();
    return 0;
}