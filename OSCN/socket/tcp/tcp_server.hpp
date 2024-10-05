#pragma once
#include "err.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <errno.h>
#include <functional>
#include <iostream>
#include <netinet/in.h>
#include <strings.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "log.hpp"

namespace ymbx
{
    using func_t = std::function<std::string(std::string)>;

    class tcpServer;
    class ThreadData
    {
    public:
        ThreadData(int id,struct sockaddr_in * sa,tcpServer* current):_sock(id),_sa(sa),_this(current){}
    public:
        int _sock;
        // std::string _ip;
        // uint16_t _port;
        struct sockaddr_in *_sa;
        tcpServer *_this;
    };
    class tcpServer
    {
    public:
        tcpServer(func_t func, uint16_t port) : _func(func), _listenport(port) {}
        void InitServer()
        {
            _listensock = socket(AF_INET, SOCK_STREAM, 0);
            if (_listensock < 0)
            {
                // std::cout << "create socket error" << strerror(errno) << std::endl;
                logMessage(Fatal, "create socket error>>>%d:%s", errno, strerror(errno));
                exit(CREATE_ERR);
            }
            struct sockaddr_in local;
            bzero(&local, sizeof(local));
            local.sin_family = AF_INET;
            local.sin_addr.s_addr = htonl(INADDR_ANY);
            local.sin_port = htons(_listenport);
            if (bind(_listensock, (struct sockaddr *)&local, sizeof(local)) != 0)
            {
                // std::cout << "bind socket error:" << strerror(errno) << std::endl;
                logMessage(Fatal, "bind socket error>>>%d:%s", errno, strerror(errno));
                exit(BIND_ERR);
            }
            // 监听socket
            int n = listen(_listensock, 1);
            if (n < 0)
            {
                // std::cout << "listen error" << std::endl;
                logMessage(Fatal, "listen error>>>%d:%s", errno, strerror(errno));
                exit(LISTEN_ERR);
            }
        }
        // 处理sockaddr_in获取IP和PORT
        std::string getIPort(const struct sockaddr_in &clients)
        {
            std::string name;
            name += inet_ntoa(clients.sin_addr);
            name += ":";
            name += std::to_string(clients.sin_port);
            return name;
        }
        void service(int sock,struct sockaddr_in& client)
        {
            // read和write只能用于流式操作
            while (true)
            {
                char buffer[1024];
                int rn = read(sock, buffer, sizeof(buffer) - 1);
                if (rn < 0)
                {
                    // std::cout << "don't receive message:" << strerror(errno) << std::endl;
                    logMessage(Info, "don't receive message>>>%d:%s", errno, strerror(errno));
                    continue;
                }
                else if (rn == 0)
                {
                    // std::cout << getIPort(client)<<"---client quit" << std::endl;
                    logMessage(Info, "client quit>>>%s", getIPort(client).c_str());
                	// sleep(5000);    
		            close(sock);
                    // sleep(5000);
                    break;
                }
                else
                {
                    buffer[rn] = 0;
                }

                // 业务处理
                std::string ret = _func(buffer);

                // std::cout << getIPort(client) << ":" << buffer << std::endl;
                logMessage(Info, "%s:%s", getIPort(client).c_str(), buffer);
                int wn = write(sock, ret.c_str(), ret.size());
                if (wn < 0)
                {
                    // std::cout << "write error:" << strerror(errno) << std::endl;
                    logMessage(Warning, "write error>>>%d:%s", errno, strerror(errno));
                    continue;
                }
            }
        }
        static void* threadRoutine(void* args){
            pthread_detach(pthread_self());
            ThreadData *td = static_cast<ThreadData *>(args);
            td->_this->service(td->_sock,*(td->_sa));
            delete td;
        }
        void Start()
        {
            signal(SIGCHLD, SIG_IGN);
            while (true)
            {
                sleep(10000);
                // struct sockaddr_in client;
                // socklen_t len = sizeof(client);
                // int _consock = accept(_listensock, (sockaddr *)&client, &len);
                // if (_consock < 0)
                // {
                //     // std::cout << "connect error:" << strerror(errno) << std::endl;
                //     logMessage(Error, "connect error>>>%d:%s", errno, strerror(errno));
                //     // exit(CONNECT_ERR);
                //     continue;
                // }
                // //内部进行读写和业务处理
                //     //只能进行单个客户端的访问
                // // service(_consock,client);

                //     //多进程处理
                // // pid_t id = fork();
                // // if(id<0){
                // //     std::cout << "fork process error:" << strerror(errno) << std::endl;
                // //     continue;
                // // }else if(id==0){
                // //     close(_listensock);
                // //     if(fork()>0)
                // //         exit(0);
                // //     service(_consock, client);
                // //     exit(0);
                // // }

                // // close(_consock);
                // // pid_t ret = waitpid(id, nullptr, 0);
                // // if(ret>0)
                // //     std::cout << "wait id:" << id << " success" << std::endl;

                //     //多线程版本
                // // std::string clientip = inet_ntoa(client.sin_addr);
                // // uint16_t clientport = ntohs(client.sin_port);
                // pthread_t tid;
                // ThreadData *td = new ThreadData(_consock, &client,this);
                // pthread_create(&tid, nullptr, threadRoutine, td);

                // //缺少一个线程池版本

            }
        }
        ~tcpServer() {}

private:
    int _listensock;
    uint16_t _listenport;
    func_t _func;
};
}
