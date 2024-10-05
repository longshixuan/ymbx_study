#pragma once
#include<iostream>
#include<cstring>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unordered_map>
#include<strings.h>
#include<string>
#include<functional>
#include"err.hpp"
#include"cirque.hpp"
#include"mythread.hpp"
#include"mymutex.hpp"

//网络字节序规定是大端传输
namespace ymbx{
    using func_t = std::function<std::string (std::string)>;

    class udpServer
    {
    public:
        udpServer(func_t func,uint16_t port):_func(func),_port(port){
            pthread_mutex_init(&_mtx, nullptr);
            //???
            _rp = new Thread(1,std::bind(&udpServer::Recv,this));
            _sp = new Thread(2, std::bind(&udpServer::Send, this));
        }
        void Start(){
            //创建套接字
            _sock = socket(AF_INET, SOCK_DGRAM, 0);
            if(_sock<0){
                std::cout << "creat socket error:" << strerror(errno) << std::endl;
                exit(CREATE_ERR);
            }
            //绑定IP地址和端口号
                //初始化structu addr
            struct sockaddr_in local;
            //将结构体清零
            bzero(&local, sizeof(struct sockaddr_in));
            local.sin_family = AF_INET;
            local.sin_port = htons(_port);
            //云服务器上不允许给定IP地址，所以设置为所有IP
            //虚拟机或者本地运行的需要给IP地址
            local.sin_addr.s_addr = INADDR_ANY;
            int bret = bind(_sock, (struct sockaddr*)&local, sizeof(local));
            if(bret<0){
                std::cout << "bind socket error:" << strerror(errno) << std::endl;
                exit(BIND_ERR);
            }
            _rp->run();
            _sp->run();
        }
        void addUser(std::string name,struct sockaddr_in &client)
        {
            lockGuard lg(&_mtx);
            _onlineusr[name] = client;
            // std::cout << "add success:" << name << std::endl;
        }
        //处理sockaddr_in获取IP和PORT
        std::string getIPort(const struct sockaddr_in& clients){
            std::string name;
            name += inet_ntoa(clients.sin_addr);
            name += ":";
            name += std::to_string(clients.sin_port);
            return name;
        }
        void Recv(){
            while(true){
                //接收
                char buffer[1024];
                // std::string message;
                struct sockaddr_in clients;
                socklen_t len = sizeof(clients);
                // std::cout << "server recv" << std::endl;
                int n = recvfrom(_sock, buffer, sizeof(buffer)-1, 0,(struct sockaddr*)&clients, &len);
                // std::cout << "server recv" << std::endl;

                if(n>0){
                    buffer[n] = 0;
                    // std::cout << "server recv error:" << strerror(errno)<<std::endl;
                    // exit(RECV_ERR);
                }
                std::string name=getIPort(clients);
                addUser(name,clients);
                _rq.push(std::make_pair(clients, buffer));
                // _rq.push(buffer);
            }
        }
        void Send(){
            while(true){
                std::pair<struct sockaddr_in, std::string> task;
                // std::string task;

                _rq.pop(&task);

                // 进行业务处理
                    //针对每个具体的用户进行广播
                // struct sockaddr_in clients = task.first;
                // socklen_t len = sizeof(clients);
                // std::string buffer = task.second;
                // std::string name = getIPort(clients);
                // if(_onlineusr.find(name)!=_onlineusr.end()){
                //     std::string ret = _func(buffer);
                //     // 可以获取接收到的客户端IP和port
                //     // std::cout << inet_ntoa(clients.sin_addr) << ":" << clients.sin_port << "#"<<ret<<std::endl;
                //     // 发送
                //     sendto(_sock, ret.c_str(), ret.size(), 0, (struct sockaddr *)&clients, len);
                // }else{
                //     std::cout << "no this user, please try again" << std::endl;
                // }

                    //模拟群消息聊天
                
                for (auto &e : _onlineusr)
                {
                    std::string message = _func(getIPort(task.first));
                    std::cout << message<< "#" << task.second << std::endl;
                    // 发送
                    message = message + "#" + task.second;
                    sendto(_sock, message.c_str(), message.size(), 0, (struct sockaddr *)&(e.second), sizeof(e.second));
                }
            }
        }
        ~udpServer(){
            pthread_mutex_destroy(&_mtx);
            _rp->join();
            _sp->join();
            delete _rp;
            delete _sp;
        }

    private:
        int _sock;
        uint16_t _port;
        func_t _func;
        std::unordered_map<std::string,struct sockaddr_in> _onlineusr;
        cirQue <std::pair<struct sockaddr_in, std::string>> _rq;
        // cirQue <std::string> _rq;
        pthread_mutex_t _mtx;
        Thread *_sp;
        Thread* _rp;
        // std::string _ip;
    };
}