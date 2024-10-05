#pragma once
#include "err.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <errno.h>
#include <iostream>
#include "log.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace ymbx
{
    class tcpClient
    {
    public:
        tcpClient(std::string ip,uint16_t port):_ip(ip),_port(port) {}
        void InitClient()
        {
            _sock = socket(AF_INET, SOCK_STREAM, 0);
            if (_sock < 0)
            {
                // std::cout << "create socket error" << strerror(errno) << std::endl;
                logMessage(Fatal, "create socket error>>>%d:%s", errno, strerror(errno));
            }
            struct sockaddr_in client;
            socklen_t clen = sizeof(client);
            bzero(&client, sizeof(client));
            client.sin_family = AF_INET;
            client.sin_addr.s_addr = inet_addr(_ip.c_str());
            client.sin_port = htons(_port);
            int cnt = 5;
            while (connect(_sock, (struct sockaddr *)&client, clen) != 0)
            {
                logMessage(Debug, "connect again>>>%d", --cnt);
                // std::cout << "connect again:" << --cnt << std::endl;
                sleep(1);
                if (cnt == 0)
                {
                    // std::cout << "connect error, please wait a moment:" << strerror(errno) << std::endl;
                    logMessage(Error, "connect error, please wait a moment>>>%d:%s", errno, strerror(errno));
                    exit(CONNECT_ERR);
                }
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
        void Start()
        {
            while (true)
            {
                std::string message;
                std::cout << "Message:";
                std::getline(std::cin, message);
                int wn=write(_sock, message.c_str(), message.size());
                if(wn<0){
                    // std::cout << "client write error:" << strerror(errno) << std::endl;
                    logMessage(Error, "client write error>>>%d:%s", errno, strerror(errno));
                    continue;
                }

                char buffer[1024];
                int rn = read(_sock, buffer, sizeof(buffer)-1);
                if(rn>0){
                    buffer[rn] = 0;
                    std::cout << buffer << std::endl;
                }else if(rn==0){
                    std::cout << "server quit" << std::endl;
                    logMessage(Error, "server quit");
			sleep(10000);
                   // break;
                }else{
                    // std::cout << "client read error:" << strerror(errno) << std::endl;
                    logMessage(Error, "client read error>>>%d:%s", errno, strerror(errno));
                    continue;
                }
            }
        }
        ~tcpClient() {}

    private:
        int _sock;
        uint16_t _port;
        std::string _ip;
    };
}
