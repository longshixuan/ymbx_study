#pragma once
#include "sock.hpp"
#include <pthread.h>
#include<functional>

#define Default_port 8889

class httpServer;
class ThreadData
{
public:
    ThreadData(int sock, std::string &ip, uint16_t port, httpServer *ptr) : _sock(sock), _ip(ip), _port(port), _this(ptr) {}
    ~ThreadData() {}

public:
    int _sock;
    std::string _ip;
    uint16_t _port;
    httpServer *_this;
};

class httpServer
{
    using func_t = std::function<std::string(const std::string &)>;
public:
    httpServer(func_t func,uint16_t port = Default_port) : _func(func),_port(port) {}
    void InitServer()
    {
        _listensock.Socket();
        _listensock.Bind(_port);
        _listensock.Listen();
    }
    void ServiceIO(int sock)
    {
        //从网络中读取文件---->>>>检查文件是否完整---->>>>去掉报头---->>>>进行反序列化---->>>>进行业务处理---->>>>序列化，添加报头---->>>>发送
        std::string httpRequest;
        // std::cout << "start print" << std::endl;
        //缺少反序列化的一步
        while (true)
        {
            char buffer[1024];
            int rn = recv(sock, buffer, sizeof(buffer) , 0);
            if (recv <= 0)
            {
                LogMessage(Error, PATH)("read error,%d:%s", errno, strerror(errno));
                break;
            }
            httpRequest += buffer;
        }
        std::string response = _func(httpRequest);
        //缺少序列化的一步
        send(sock, response.c_str(), response.size(), 0);
        _listensock.closeFD(sock);
        return;
    }
    static void *threadRoutine(void *args)
    {
        pthread_detach(pthread_self());
        ThreadData *td = static_cast<ThreadData *>(args);
        td->_this->ServiceIO(td->_sock);
        delete td;
        return nullptr;
    }
    void Start()
    {
        while (true)
        {
            struct sockaddr_in client;
            int sock = _listensock.Accept(&client);
            if (sock < 0)
            {
                std::cout << "accept error" << std::endl;
                continue;
            }
            std::cout << "accept success" << std::endl;
            // std::cout << "accept:" << sock << std::endl;
            std::string clientip = inet_ntoa(client.sin_addr);
            uint16_t clientport = ntohs(client.sin_port);
            pthread_t tid;
            ThreadData *td = new ThreadData(sock, clientip, clientport, this);
            pthread_create(&tid, nullptr, threadRoutine, td);
        }
    }
    ~httpServer() {
    }

private:
    ymbxSocket _listensock;
    uint16_t _port;
    func_t _func;
};