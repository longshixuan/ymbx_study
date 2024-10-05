#include "httpserver.hpp"
#include <iostream>
#include <memory>
#include <string>

void Usage(const std::string &str)
{
    std::cout << "Usage:\n\t" << str << " port" << std::endl;
}
std::string Response(const std::string& httprequest){
    std::cout << httprequest << std::endl;
    return httprequest;
}
int main(int args, char *argv[])
{
    if (args != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t port = atoi(argv[1]);
    std::unique_ptr<httpServer> uphs(new httpServer(Response,port));
    uphs->InitServer();
    uphs->Start();
    return 0;
}
