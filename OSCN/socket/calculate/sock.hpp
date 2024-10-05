#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<iostream>
#include<cstring>
#include "err.hpp"
#include "log.hpp"


#define MAX_CONNECT 100
class ymbxSocket{
    public:
        ymbxSocket() {}
        void Socket(){
            _listensock = socket(AF_INET, SOCK_STREAM, 0);
            if(_listensock<0){
                // LogMessage(Fatal, "log/log.txt").RecordMessage("create socket error,%d:%s", errno, strerror(errno));
                LogMessage(Fatal, "log/log.txt")("create socket error,%d:%s", errno, strerror(errno));
                exit(CREATE_ERR);
            }
            // LogMessage(Info, "log/log.txt").RecordMessage("create socket success");
            LogMessage(Info, "log/log.txt")("create socket success");
        }
        void Bind(uint16_t _port){
            struct sockaddr_in server;
            memset(&server, 0, sizeof(server));
            server.sin_family = AF_INET;
            server.sin_addr.s_addr = INADDR_ANY;
            server.sin_port = ntohs(_port);
            if (bind(_listensock, (struct sockaddr *)&server, sizeof(server)) < 0)
            {
                LogMessage(Fatal, "log/log.txt")("bind socket error,%d:%s", errno, strerror(errno));
                exit(BIND_ERR);
            }
            LogMessage(Info, "log/log.txt")("bind socket success");
        }
        void Listen(){
            if(listen(_listensock,MAX_CONNECT)<0){
                LogMessage(Warning, "log/log.txt")("listen socket error,%d:%s", errno, strerror(errno));
            }
            LogMessage(Info, "log/log.txt")("listen socket success");
        }
        int Accept(struct sockaddr_in* client){
            // struct sockaddr_in client;
            // memset(&client, 0, sizeof(client));
            socklen_t clen = sizeof(*client);
            int sockfd=accept(_listensock, (struct sockaddr*)client, &clen);
            if(sockfd<0){
                LogMessage(Error, "log/log.txt")("accept socket error,%d:%s", errno, strerror(errno));
                return -1;
            }
            LogMessage(Info, "log/log.txt")("accept socket success--->>>%s:%d",inet_ntoa(client->sin_addr) , ntohs(client->sin_port));
            return sockfd;
        }
        bool Connect(std::string ip,uint16_t port){
            struct sockaddr_in server;
            memset(&server, 0, sizeof(server));
            server.sin_family = AF_INET;
            server.sin_addr.s_addr = inet_addr(ip.c_str());
            server.sin_port = htons(port);
            if(connect(_listensock, (struct sockaddr*)&server, sizeof(server))<0){
                LogMessage(Error, "log/log.txt")("connect socket error,%d:%s", errno, strerror(errno));
                return false;
            }
            LogMessage(Info, "log/log.txt")("connect socket success");
            return true;
        }
        int getFD(){
            return _listensock;
        }
        void closeFD(int fd){
            if(fd!=_listensock){
                close(fd);
            }
        }
        ~ymbxSocket() {}

    private:
        int _listensock;
};