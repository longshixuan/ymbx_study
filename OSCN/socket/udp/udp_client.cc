#include "udp_client.hpp"
#include<memory>
using namespace ymbx;
using namespace std;

void Usage(char *s)
{
    std::cout << "Usage:\n\t" << s <<" IP"<< " port" << std::endl;
}
int main(int args,char* argv[])
{
    if(args!=3){
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);
    unique_ptr<clientServer> cs(new clientServer(ip,port));
    // cs->initServer();
    cs->Start();
    return 0;
}