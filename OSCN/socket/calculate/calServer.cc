#include "calServer.hpp"
#include <iostream>
#include<memory>
#include<string>

void Usage(char* s){
    std::cout << "the format:\n\t"<<s<<"  port" << std::endl;
}
Response calCulate(const Request& req){
    int x = req.getX();
    int y = req.getY();
    char op = req.getOP();
    // int result;
    // int code;
    // std::cout << x<<" "<<op <<" " <<y<< std::endl;
    Response res(0, 0);
    switch (op)
    {
    case '+':
        res._result = x + y;
        break;
    case '-':
        res._result = x - y;
        break;
    case '*':
        res._result = x * y;
        break;
    case '/':
        if(y==0)
            res._code = DivisionZero;
        else{
            res._result = x / y;
        }
        break;
    case '%':
        if(y==0){
            res._code = ModZero;
        }else{
            res._result = x % y;
        }
        break;
    default:
        res._code = Other;
        break;
    }
    // std::cout << result << "   " << code << std::endl;
    return res;
}
int main(int args,char* argv[]){
    if(args!=2){
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t port = atoi(argv[1]);
    std::unique_ptr<calServer> cs(new calServer(calCulate,port));
    cs->InitServer();
    cs->Start();
    return 0;
}