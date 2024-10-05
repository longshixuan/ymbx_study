#pragma once
#include <iostream>
#include"sock.hpp"
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
#include<string>
#include<error.h>
#include<sys/poll.h>



namespace ymbx
{
    typedef struct pollfd type_t;
    
    const static uint16_t defaultport = 8888;
    const static int N = 2048;
    const static int defaultfd = -1;
    const static short defaultevent = -1;
    class SelectServer
    {
    public:
        SelectServer(uint16_t port=defaultport):_port(port) {}
        void InitServer()
        {
            _listensock.Socket();
            _listensock.Bind(_port);
            _listensock.Listen();
            //将数组初始化为defaultfd
            _fdarr = new type_t[N];
            for (int i = 0; i < N; ++i)
            {
                _fdarr[i].fd = defaultfd;
                _fdarr[i].events = defaultevent;
                _fdarr[i].revents = defaultevent;
            }
        }
        void Accepter(){
            struct sockaddr_in client;
            int sock = _listensock.Accept(&client);
            if (sock < 0)
                return;
            // 此处不可以直接read/recv————因为不确定sock上的数据是否就绪，因此需要将sock交给select管理
            int i = 0;
            for (; i < N; ++i)
            {
                if (_fdarr[i].fd== defaultfd)
                {
                    _fdarr[i].fd= sock;
                    _fdarr[i].events = POLLIN;
                    break;
                }
            }
            if (i >= N)
            {
                close(sock);
                // 可以动态扩容
                LogMessage(Error, PATH)("fd is full,please wait for a moment...");
            }
        }
        void ServiceInput(int i){
            char buffer[1024];
            int s = recv(_fdarr[i].fd, buffer, sizeof(buffer) - 1,0);
            if(s>0){
                buffer[s - 1] = 0;
                std::cout << "client# " << buffer << std::endl;
            }
            else if (s == 0)
            {
                _fdarr[i].fd = defaultfd;
                _fdarr[i].events = defaultevent;
                _fdarr[i].revents = defaultevent;
                close(_fdarr[i].fd);
                LogMessage(Info, PATH)("end of file...");
            }
            else
            {
                _fdarr[i].fd = defaultfd;
                _fdarr[i].events = defaultevent;
                _fdarr[i].revents = defaultevent;
                close(_fdarr[i].fd);
                LogMessage(Error, PATH)("read error>>> %s[%d]", strerror(errno), errno);
            }
            
        }
        //此处从fd中读写有bug
        void ServiceOutput(int& fd,std::string response){
            // 将信息回显给client
            std::string echobuffer = "select server#";
            echobuffer += response;
            echobuffer += '\n';
            send(fd, echobuffer.c_str(), echobuffer.size(), 0);
        }
        void HandlerEvent(){
            for (int i = 0; i < N;++i){
                if(_fdarr[i].fd==defaultfd)
                    continue;
                if (_fdarr[i].revents == POLLIN)
                {
                    if (_fdarr[i].fd == _listensock.getFD())
                    {
                        LogMessage(Info, PATH)("processing...");
                        Accepter();
                    }
                    else if (_fdarr[i].fd != _listensock.getFD())
                    {
                        // 进行读写操作
                        ServiceInput(i);
                    }
                }
                if (_fdarr[i].revents == POLLOUT){
                    // ServiceOutput(_fdarr[i].fd,"success");
                    //TODO
                }
            }
                

        }
        void PrintFD(){
            for (int i = 0; i < N;++i)
            {
                if(_fdarr[i].fd==defaultfd)
                    continue;
                std::cout << _fdarr[i].fd << " ";
            }
            std::cout << std::endl;
        }
        void Start()
        {
            _fdarr[0] .fd= _listensock.getFD();
            _fdarr[0].events = POLLIN;
            // struct timeval中nullptr表示阻塞调用，{0，0}表示非阻塞调用，{n，0}表示n秒内无事件就绪即timeout一次
            while (true)
            {
                int srn = poll(_fdarr,N,1000);
                if(srn>0){
                    HandlerEvent();
                    LogMessage(Info, PATH)("select recv fd:%d",srn);
                }
                else if (srn == 0)
                {
                    // std::cout << "timeout" << std::endl;
                    LogMessage(Info, PATH)("timeout,continue...");
                }
                else{
                    // std::cout<<"error"<<std::endl;
                    LogMessage(Error, PATH)("error description:%s[%d]", strerror(errno), errno);
                }
                PrintFD();
                sleep(1);
                // switch(srn)
                // {
                //     case 0:
                //         LogMessage(Info, PATH)("timeout,continue...");
                //         break;
                //     case -1:
                //         LogMessage(Error, PATH)("error description:%s[%d]", strerror(errno), errno);
                //         break;
                //     default:
                //         LogMessage(Info, PATH)("select recv fd:%d", srn);
                //         HandlerEvent();
                //         break;
                // }
            }
        }
        ~SelectServer() {
            if(_fdarr)
                delete[] _fdarr;
        }

    private:
        ymbxSocket _listensock;
        uint16_t _port;
        type_t* _fdarr;
    };
}