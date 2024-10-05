#pragma once
#include <iostream>
#include"sock.hpp"
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
#include<string>
#include<error.h>




namespace ymbx
{
    #define READ_EVENT 0x1
    #define WRITE_EVENT (0x1<<1)
    #define EXCEPT_EVENT (0x1<<2)


    typedef struct FDEvent{
        int fd;
        uint8_t event;
    }type_t;
    
    const static uint16_t defaultport = 8888;
    const static int N = sizeof(fd_set) * 8;
    const static int defaultfd = -1;
    const static uint8_t defaultevent = 0;
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
            for (int i = 0;i<N;++i){
                _fdarr[i].fd = defaultfd;
                _fdarr[i].event = defaultevent;
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
                    break;
                }
            }
            if (i >= N)
            {
                close(sock);
                LogMessage(Error, PATH)("fd is full,please wait for a moment...");
            }
        }
        void ServiceInput(int& fd){
            char buffer[1024];
            int s = recv(fd, buffer, sizeof(buffer) - 1,0);
            if(s>0){
                buffer[s - 1] = 0;
                std::cout << "client# " << buffer << std::endl;
            }
            else if (s == 0)
            {
                fd = defaultfd;
                close(fd);
                LogMessage(Info, PATH)("end of file...");
            }
            else
            {
                fd = defaultfd;
                close(fd);
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
        void HandlerEvent(fd_set& rfds,fd_set& wfds){
            for (int i = 0; i < N;++i){
                if (_fdarr[i].event == READ_EVENT && FD_ISSET(_fdarr[i].fd, &rfds)){
                    if (_fdarr[i].fd == _listensock.getFD())
                    {
                        LogMessage(Info, PATH)("processing...");
                        Accepter();
                    }
                    else if (_fdarr[i].fd != _listensock.getFD())
                    {
                        // 进行读写操作
                        ServiceInput(_fdarr[i].fd);
                    }
                }
                if (_fdarr[i].event == WRITE_EVENT && FD_ISSET(_fdarr[i].fd, &wfds)){
                    ServiceOutput(_fdarr[i].fd,"success");
                }
            }
                

        }
        void PrintFD(){
            for(auto e:_fdarr){
                if(e.fd==defaultfd)
                    continue;
                std::cout << e.fd << " ";
            }
            std::cout << std::endl;
        }
        void Start()
        {
            _fdarr[0] .fd= _listensock.getFD();
            _fdarr[0].event = READ_EVENT;
            // struct timeval中nullptr表示阻塞调用，{0，0}表示非阻塞调用，{n，0}表示n秒内无事件就绪即timeout一次
            while (true)
            {
                //传入最大fd数+1
                //每次循环都需要重新将需要内核监听的fd写入rfds中
                fd_set rfds;
                fd_set wfds;
                FD_ZERO(&wfds);
                FD_ZERO(&rfds);
                int maxfd = _fdarr[0].fd;
                for (int i = 0; i < N; ++i)
                {
                    if(_fdarr[i].fd==defaultfd)
                        continue;
                    if(_fdarr[i].event==READ_EVENT){
                        FD_SET(_fdarr[i].fd, &rfds);
                    }
                    if(_fdarr[i].event==WRITE_EVENT){
                        FD_SET(_fdarr[i].fd, &wfds);
                    }
                    if(maxfd<_fdarr[i].fd){
                        maxfd = _fdarr[i].fd;
                    }
                }
                struct timeval timeout = {2, 0};
                // FD_SET(_listensock.getFD(), &rfds);
                int srn = select(maxfd+1, &rfds, &wfds, nullptr, &timeout);
                if(srn>0){
                    HandlerEvent(rfds,wfds);
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
        ~SelectServer() {}

    private:
        ymbxSocket _listensock;
        uint16_t _port;
        type_t _fdarr[N];
    };
}