#pragma once
#include "sock.hpp"
#include "log.hpp"
#include<vector>
#include<string>
#include "protocol.hpp"
#include<pthread.h>

#define MAX_TRY_CON 5
class calClient
{
public:
    calClient(std::string ip,uint16_t port):_ip(ip),_port(port) {}
    void InitServer()
    {
        //1.创建套接字
        _sock.Socket();
        // 2.绑定IP和端口号————操作系统完成
        //3.发送连接请求
        int cnt = MAX_TRY_CON;
        while (cnt){
            if(_sock.Connect(_ip, _port)){
                break;
            }
            LogMessage(Error, "log/log.txt")("try connect again:%d",--cnt);
            sleep(1);
        }
        if(cnt==0){
            exit(CONNECT_ERR);
        }
    }
    Request opCinStr(const std::string& cinstr){
        std::vector<std::string> opv;
        opv.push_back("+");
        opv.push_back("-");
        opv.push_back("*");
        opv.push_back("/");
        opv.push_back("%");
        for(auto e:opv){
            size_t pos = cinstr.find(e);
            if (pos != std::string::npos)
            {
                int x = stoi(cinstr.substr(0, pos));
                int y = stoi(cinstr.substr(pos + 1));
                char op = cinstr[pos];
                return Request(x, y, op);
            }
        }
        
        return Request();
    }
    void Start()
    {
        std::string inbuffer;
        while(true){
            std::cout << "Enter>>>";
            std::string cinstr;
            std::getline(std::cin, cinstr);
            Request req = opCinStr(cinstr);
            std::string message_in;
            req.Serialize(&message_in);
            message_in=addHeader(message_in);
            int sn = send(_sock.getFD(), message_in.c_str(), message_in.size(), 0);
            if(sn<0){
                LogMessage(Error, "log/log.txt")("send message error,%d:%s", errno, strerror(errno));
                continue;
            }
            // std::cout << "client send success:" <<message_in<< std::endl;
            std::string retmes;
            int n = ReadMes(_sock.getFD(), inbuffer, &retmes);
            if (n == -2)
            {
                break;
            }
            else if (n == -1 || n==0)
            {
                continue;
            }
            std::cout << "client recv success:" << retmes << std::endl;
            retmes=removeHeader(retmes,n);
            Response res;
            res.ReSerialize(retmes);
            LogMessage(Info, "log/log.txt")("calculate result--->>>%d[code:%d]", res.getResult(), res.getCode());
            std::cout << res.getResult() << "[" << res.getCode() << "]" << std::endl;
        }
        _sock.closeFD(_sock.getFD());
    }
    ~calClient() {}

private:
    ymbxSocket _sock;
    std::string _ip;
    uint16_t _port;
};