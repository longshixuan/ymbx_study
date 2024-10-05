#include "epollServer.hpp"
#include<memory>
#include "Util.hpp"
#include "err.hpp"
#include "log.hpp"
#include<stdlib.h>

using namespace ymbx;

std::string Echo(std::string request){
    return request+'\n';
}
int main(int args,char* argv[])
{
    if(args!=2){
        Usage(argv[0]);
        LogMessage(FATAL,"lack of parameter");
        exit(ERR_USAGE);
    }
    uint16_t port = atoi(argv[1]);
    std::unique_ptr<EpollServer> ep(new EpollServer(Echo, port));
    ep->InitServer();
    ep->Start();
    return 0;
}