#pragma once

#include "err.hpp"
#include "mythread.hpp"
#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <functional>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

namespace ymbx
{
    class clientServer
    {
    public:
        clientServer(std::string ip, uint16_t port) : _ip(ip), _port(port)
        {
            //对于类内成员如果不加&则无法判断具体是哪个类的实例化————不同类内成员函数的名称可能相同
            _rp = new Thread(2, std::bind(&clientServer::Recv, this));
            _sp = new Thread(1, std::bind(&clientServer::Send, this));
        }
        void Start()
        {
            // 创建套接字
            _sock = socket(AF_INET, SOCK_DGRAM, 0);
            if (_sock < 0)
            {
                std::cout << "create socket error:" << strerror(errno) << std::endl;
                exit(CREATE_ERR);
            }
            // 绑定IP和port：作为客户端不需要自己手动绑定，而是由OS在系统调用时自动绑定
            _rp->run();
            _sp->run();
        }
        void Send()
        {
            while (true)
            {
                std::string message;
                std::cerr << "Message:";
                // std::cin >> buffer;
                std::getline(std::cin, message);
                struct sockaddr_in server;
                memset(&server, 0, sizeof(server));
                server.sin_family = AF_INET;
                server.sin_addr.s_addr = inet_addr(_ip.c_str());
                server.sin_port = ntohs(_port);
                socklen_t len = sizeof(server);

                // 发送给服务器
                int n = sendto(_sock, message.c_str(), message.size(), 0, (struct sockaddr *)&server, len);
                if (n < 0)
                {
                    std::cout << "send error:" << strerror(errno) << std::endl;
                    exit(SEND_ERR);
                }
                // std::cout << "send success" << std::endl;
            }
        }
        void Recv()
        {
            while (true)
            {
                // 接收服务器消息
                char ret[2048];
                struct sockaddr_in retserver;
                socklen_t retlen = sizeof(retserver);
                int retn = recvfrom(_sock, ret, sizeof(ret) - 1, 0, (struct sockaddr *)&retserver, &retlen);
                if (retn > 0)
                {
                    ret[retn] = 0;
                    std::cout << ret << std::endl;
                }
            }
        }
        ~clientServer() {
            _rp->join();
            _sp->join();
            delete _rp;
            delete _sp;
        }

    private:
        int _sock;
        std::string _ip;
        uint16_t _port;
        Thread *_rp;
        Thread *_sp;
    };
}