#pragma once
#include"sock.hpp"
#include "epoller.hpp"
#include "widget.hpp"
#include "log.hpp"
#include<functional>
#include<unordered_map>



namespace ymbx{
    static uint16_t default_port = 8888;
    using func_t = std::function<std::string(std::string)>;

    class Connection{
    public:
        Connection(int fd):_fd(fd){}

        ~Connection(){}
    public:
        int _fd;
        std::string _inbuffer;
        std::string _outbuffer;

    };
    class EpollServer
    {
    public:
        EpollServer(func_t func, uint16_t port = default_port) : _port(port), _func(func) {}

        void InitServer()
        {
            _sock.Socket();
            _sock.Bind(_port);
            _sock.Listen();
            _epoller.Create();
            AddConnection(_sock.GetFD(), EPOLLIN);
            // LogMessage( DEBUG, "init server success");
        }
        void Accepter(Connection* conn){
            std::string ip;
            uint16_t port;
            int apfd = _sock.Accept(&ip, &port);
            std::cout << "accept success,fd:" << apfd << ",ip:" << ip << ",port:" << port << std::endl;
            AddConnection(apfd, EPOLLIN);
        }
        void Reader(Connection * conn){
            // 实际读取
            //  std::cout << "read data..." << std::endl;
            char request[1024];
            ssize_t s = recv(conn->_fd, request, sizeof(request) - 1, 0);
            if (s > 0)
            {
                request[s - 1] = 0;
                conn->_inbuffer += request;
                //判断读完了没有，没读完继续读，读完对数据进行处理

                std::cout << "client# " << request << std::endl;
                // std::string response = _func(request);
            }
            else if (s == 0)
            {
                _epoller.DelEvents(conn->_fd);
                close(conn->_fd);
                LogMessage(INFO, "end of file...");
            }
            else
            {
                _epoller.DelEvents(conn->_fd);
                close(conn->_fd);
                LogMessage(ERROR, "read error>>> %s[%d]", strerror(errno), errno);
            }
        }
        void Writer(Connection* conn){
            
        }
        void Excepter(Connection* conn){

        }
        void HandlerEvents(struct epoll_event* epevs,int rfds){
            // std::cout << "handle events..." << std::endl;
            for (int i = 0; i < rfds;++i){
                int fd = epevs[i].data.fd;
                uint32_t events = epevs[i].events;
                //读事件
                if(events & EPOLLIN){
                    if(fd==_sock.GetFD()){
                        Accepter(_connections[fd]);
                    }
                    else
                    {
                        Reader(_connections[fd]);
                    }
                }
                //写事件
                if(events & EPOLLOUT){
                    std::cout << "writing..." << std::endl;
                }
                if(events & EPOLLERR){
                    std::cout << "error..." << std::endl;
                }
            }
        }
        void AddConnection(int fd,uint32_t events){
            _epoller.AddEvents(fd, EPOLLIN);
            Connection *conn = new Connection(fd);
            _connections.insert(std::make_pair(fd, conn));
        }
        void Start()
        {
            //获取连接属于读事件
            while (true)
            {
                int rfds = _epoller.WaitEvents(_epevs, MAX_EVENTS, TIMEOUT);
                if(rfds==-1){
                    LogMessage(ERROR, "wait events error,error code:%d,error info:%s", errno, strerror(errno));
                    continue;
                }else if(rfds==0){
                    LogMessage(INFO, "timeout...");
                    continue;
                }else{
                    HandlerEvents(_epevs, rfds);
                }
            }
        }
        ~EpollServer() {
            _epoller.Close();
            _sock.Close();
        }

    private:
        uint16_t _port;
        YMBXSocket _sock;
        Epoller _epoller;
        func_t _func;
        struct epoll_event _epevs[MAX_EVENTS];
        std::unordered_map<int, Connection *> _connections;
    };
}