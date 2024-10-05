#include"tcp_client.hpp"
#include<iostream>
#include<memory>

using namespace ymbx;

void Usage(char *s)
{
    std::cout << "Usage:\n\t" << s << " IP" << " port" << std::endl;
}
int main(int args,char* argv[])
{
    if(args!=3){
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);
    std::unique_ptr<tcpClient> tc(new tcpClient(ip,port));
    tc->InitClient();
    tc->Start();
    return 0;
}