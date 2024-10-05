#include "calClient.hpp"
#include <iostream>
#include <memory>

void Usage(char *s)
{
    std::cout << "the format:\n\t" << s << "  port" << std::endl;
}
int main(int args,char* argv[])
{
    if(args!=3){
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);
    std::unique_ptr<calClient> cc(new calClient(ip,port));
    cc->InitServer();
    cc->Start();
    return 0;
}