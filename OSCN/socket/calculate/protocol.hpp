#pragma once
#include <iostream>
#include<string>
#include<vector>
#include"util.hpp"
#include<utility>
#include "log.hpp"
#include "err.hpp"
#include<sys/types.h>
#include<sys/socket.h>
#include<cstring>

enum RetCode
{
    Correct = 0,
    DivisionZero,
    ModZero,
    Other
};
#define SEP " "
#define SEP_LEN strlen(SEP)
#define MES_SEP "\r\n"
#define MES_SEP_LEN strlen(MES_SEP)
//自定义发送请求的协议

std::string addHeader(const std::string &str)
{
    std::string result;
    result = std::to_string(str.size()) + MES_SEP + str + MES_SEP;
    return result;
}
std::string removeHeader(const std::string &str,int strLen)
{
    // std::cout << "removeheader start:" << str << std::endl;
    // size_t pos = str.find(MES_SEP);
    // if(pos==std::string::npos){
    //     return "";
    // }
    std::string result = str.substr(str.size() - MES_SEP_LEN-strLen,strLen);
    // std::cout << "removeheader success:" << result << std::endl;
    return result;
}

int ReadMes(int sockfd,std::string& inbuffer,std::string* result){
    // std::cout << "server read start" << std::endl;
    char buffer[1024];
    ssize_t rn=recv(sockfd, buffer, sizeof(buffer)-1, 0);
    if(rn<=0){
        LogMessage(Error, "log/log.txt")("read error,%d:%s", errno, strerror(errno));
        return -2;
    }
    buffer[rn] = 0;
    inbuffer += buffer;
    size_t pos = inbuffer.find(MES_SEP);
    if(pos==std::string::npos){
        return -1;
    }
    // std::cout << "inbuffer" <<inbuffer<< std::endl;
    int strLen = stoi(inbuffer.substr(0, pos));
    int targetLength = strLen + 2 * MES_SEP_LEN + inbuffer.substr(0, pos).size();
    // std::cout << "targetLength" << targetLength << std::endl;
    if(inbuffer.size()<targetLength){
        return 0;
    }
    *result = inbuffer.substr(0,targetLength);
    // std::cout << "result:" << *result << std::endl;
    inbuffer.erase(0, targetLength);
    // std::cout << "inbuffer:" << inbuffer << std::endl;
    // std::pair<int, std::string>
    //     retPair = removeHeader(inbuffer);
    // int slen = retPair.first;
    // std::string retStr = retPair.second;
    // if(slen>retStr.size()){
    //     std::cout << "length error:" << retStr.size() << "[" << slen << "]" << std::endl;
    //     return 2;
    // }
    // *result = retStr.substr(0,slen);
    // inbuffer.erase();
    return strLen;
}

//进行序列化可以使用现成的工具：json，xml，protobuf
class Request
{
public:
    Request() {}
    Request(int x,int y,char op):_x(x),_y(y),_op(op) {}
    bool ReSerialize(const std::string& ret){
        std::vector<std::string> reqv;
        Util().SepStr(ret,SEP, &reqv);
        _x = stoi(reqv[0]);
        _y = stoi(reqv[2]);
        _op = reqv[1][0];
        return true;
    }
    //自定义序列化方式
    bool Serialize(std::string *ret)
    {
        *ret = "";
        *ret = std::to_string(_x) + SEP + _op + SEP + std::to_string(_y);
        return true;
    }
    void PrintReq(){
        std::cout << _x << " " << _op << " " << _y << std::endl;
    }
    ~Request() {}


public:
    int getX() const {
        return _x;
    }
    int getY() const {
        return _y;
    }
    char getOP() const {
        return _op;
    }

private:
    int _x;
    int _y;
    char _op;
};

//自定义响应的协议
class Response
{
public:
    Response() {}
    Response(int result,int code):_result(result),_code(code) {}
    bool ReSerialize(const std::string& ret)
    {
        std::vector<std::string> reqv;
        Util().SepStr(ret, SEP, &reqv);
        _code = stoi(reqv[1]);
        _result = stoi(reqv[0]);
    }
    bool Serialize(std::string* ret)
    {
        *ret = "";
        *ret = std::to_string(_result) + SEP + std::to_string(_code);
        return true;
    }
    void PrintRes(){
        std::cout << _result << "   " << _code << std::endl;
    }
    ~Response() {}


public:
    int getResult(){
        return _result;
    }
    int getCode(){
        return _code;
    }
public:
    int _result;
    int _code;
};
