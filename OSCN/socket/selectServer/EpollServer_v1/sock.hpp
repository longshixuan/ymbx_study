#pragma once
#include<sys/types.h>
#include<sys/socket.h>
#include "log.hpp"
#include<cstring>
#include<cerrno>
#include<arpa/inet.h>
#include<unistd.h>


namespace ymbx{
    class YMBXSocket
    {
        const static int defaultsock = -1;

    public:
        YMBXSocket(int sock = defaultsock) : _sock(sock) {}
        void Socket()
        {
            _sock = socket(AF_INET, SOCK_STREAM, 0);
            if (_sock < 0)
            {
                LogMessage(FATAL, "create socket error,error code:%d,error message:%s", errno, strerror(errno));
                exit(ERR_CREATESOCK);
            }
        }
        void Listen()
        {
            int n = listen(_sock, MAX_FD);
            if (n < 0)
            {
                LogMessage(FATAL, "listen socket error,error code:%d,error message:%s", errno, strerror(errno));
                exit(ERR_LISTENSOCK);
            }
        }
        void Bind(uint16_t port)
        {
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = INADDR_ANY;
            addr.sin_port = ntohs(port);
            int n = bind(_sock, (struct sockaddr *)&addr, sizeof(addr));
            if (n < 0)
            {
                LogMessage(FATAL, "bind socket error,error code:%d,error message:%s", errno, strerror(errno));
                exit(ERR_BINDSOCK);
            }
        }
        int Accept(std::string *ip, uint16_t *port)
        {
            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            int retfd = accept(_sock, (struct sockaddr *)&client, &len);
            if (retfd < 0)
            {
                LogMessage(ERROR, "accept socket error,error code:%d,error message:%s", errno, strerror(errno));
                return -1;
            }
            *ip = inet_ntoa(client.sin_addr);
            *port = ntohs(client.sin_port);
            return retfd;
        }
        void Connect(const std::string &ip, const uint16_t &port)
        {
            struct sockaddr_in client;
            client.sin_family = AF_INET;
            client.sin_addr.s_addr = inet_addr(ip.c_str());
            client.sin_port = htons(port);
            int n = connect(_sock, (struct sockaddr *)&client, sizeof(client));
            if (n < 0)
            {
                LogMessage(ERROR, "connect socket error,error code:%d,error message:%s", errno, strerror(errno));
            }
        }
        int GetFD()
        {
            return _sock;
        }
        void Close()
        {

            if ( _sock != defaultsock)
            {
                close(_sock);
            }
        }
        ~YMBXSocket() {}

    private:
        int _sock;
    };
}