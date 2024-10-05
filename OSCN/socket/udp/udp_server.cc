#include "udp_server.hpp"
#include<memory>
#include<ctype.h>
#include<vector>
#include<cstdio>
using namespace ymbx;
using namespace std;

void Usage(char* s){
    std::cout << "Usage:\n\t" << s << " port" << std::endl;
}

//将业务处理和udp连接解耦
std::string ToUpper(const std::string& s){
    std::string ret;
    for(auto c:s){
        if(islower(c))
            ret+=toupper(c);
        else{
            ret += c;
        }
    }
    return ret;
}

bool isSafe(const std::string& command){
    std::vector<std::string> badMan = {"rm", "mv", "cp", "kill"};
    for(auto& e:badMan){
        if(command.find(e)!=std::string::npos){
            return false;
        }
    }
    return true;
}
std::string ExeCommand(const std::string& command){
    //进行安全检查
    if(!isSafe(command))
        return "bad!!!";
    // 便捷的用来处理系统命令函数
    FILE *pp = popen(command.c_str(), "r");
    if(pp==nullptr)
        return "none";
    std::string ret;
    char buffer[1024];
    while (fgets(buffer,sizeof(buffer),pp))
    {
        ret += buffer;
    }
    pclose(pp);
    return ret;
}
std::string echoServer(const std::string& message){
    return message;
}
int main(int args, char *argv[])
{
    if(args!=2){
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t port = atoi(argv[1]);
    // unique_ptr<udpServer> us(new udpServer(port));
    // unique_ptr<udpServer> us(new udpServer(ToUpper,port));
    // unique_ptr<udpServer> us(new udpServer(ExeCommand, port));
    unique_ptr<udpServer> us(new udpServer(echoServer, port));

    // us->InitServer();
    us->Start();
    return 0;
}