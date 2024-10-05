#pragma once
#include<sys/epoll.h>
#include "widget.hpp"
#include "log.hpp"
#include<cerrno>
#include<cstring>
#include<unistd.h>


namespace ymbx
{
    class Epoller
    {
        const static int defaultepfd = -1;

    public:
        Epoller(int epfd=defaultepfd):_epfd(epfd){}
        void Create(){
            _epfd = epoll_create(EPOLL_CREATE_SIZE);
            if(_epfd<0){
                LogMessage(ERROR, "epoll_create error,error code:%d,error message:%s", errno, strerror(errno));
            }
        }
        void AddModEvents(int fd,uint32_t events,int op){
            struct epoll_event ev;
            ev.data.fd = fd;
            ev.events = events;
            int n = epoll_ctl(_epfd, op, fd, &ev);
            if(n<0){
                LogMessage(ERROR, "add epoll events error,error code:%d,error message:%s", errno, strerror(errno));
            }
        }
        int WaitEvents(struct epoll_event* events,int maxevents,int timeout){
            return epoll_wait(_epfd, events, maxevents, timeout);
        }
        void DelEvents(int fd){
            int n=epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, nullptr);
            if(n<0){
                LogMessage(ERROR, "delete epoll events error,error code:%d,error message:%s", errno, strerror(errno));
            }
        }
        void Close(){
            if(_epfd!=defaultepfd){
                close(_epfd);
            }
        }
        int GetEpfd(){
            return _epfd;
        }
        ~Epoller(){}
    private:
        int _epfd;
    };
}