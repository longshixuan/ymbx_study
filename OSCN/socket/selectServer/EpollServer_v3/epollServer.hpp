#pragma once
#include "epoller.hpp"
#include "log.hpp"
#include "protocol.hpp"
#include "sock.hpp"
#include "widget.hpp"
#include <functional>
#include <unordered_map>
#include <sys/types.h>
#include <sys/socket.h>

namespace ymbx
{

    class Connection;
    class EpollServer;
    class Request;
    class Response;

    static uint16_t default_port = 8888;
    static std::string default_ip = "127.0.0.1";
    using func_t = std::function<Response(const Request &)>;
    using callback_t = std::function<void(Connection *)>;

    class Connection
    {
    public:
        Connection(int fd, const std::string &ip, uint16_t port, uint32_t events, EpollServer *epollserver) : _fd(fd), _ip(ip), _port(port), _events(events), _epollserver(epollserver) {}
        void Register(callback_t reader, callback_t writer, callback_t excepter)
        {
            _reader = reader;
            _writer = writer;
            _excepter = excepter;
        }
        ~Connection() {}

    public:
        // IO相关信息
        int _fd;
        std::string _inbuffer;
        std::string _outbuffer;

        // 事件状态
        uint32_t _events;

        // //业务处理回调函数
        callback_t _reader;
        callback_t _writer;
        callback_t _excepter;

        // 回调指针
        EpollServer *_epollserver;

        // 调试相关信息
        std::string _ip;
        uint16_t _port;
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
            AddConnection(_sock.GetFD(), EPOLLIN | EPOLLET);
            // LogMessage( DEBUG, "init server success");
        }
        // 事件派发器
        void Dispatcher()
        {
            // 获取连接属于读事件
            while (true)
            {
                LoopOnce();
            }
        }
        // 连接管理器
        void Accepter(Connection *conn)
        {
            do
            {
                int err = 0;
                std::string ip;
                uint16_t port;
                int apfd = _sock.Accept(&ip, &port,&err);
                if (apfd > 0)
                {
                    LogMessage(DEBUG, "accpet link success,fd:%d,link address:%s:%d", conn->_fd, conn->_ip.c_str(), conn->_port);
                    AddConnection(apfd, EPOLLIN | EPOLLET, ip, port);
                }else{
                    if (err == EAGAIN || err == EWOULDBLOCK)
                    {
                        break;
                    }
                    else if (err == EINTR)
                    {
                        continue;
                    }
                    else
                    {
                        LogMessage(WARNING, "accpet link error,error message:%s[%d]", strerror(err), err);
                        continue;
                    }
                }
            } while (conn->_events & EPOLLET);
        }
        bool Recever(Connection *conn)
        {
            bool isread = true;
            do
            {
                char request[1024];
                ssize_t s = recv(conn->_fd, request, sizeof(request) - 1, 0);
                if (s > 0)
                {
                    request[s] = 0;
                    conn->_inbuffer += request;
                }
                else if (s == 0)
                {
                    isread = false;
                    conn->_excepter(conn);
                    break;
                }
                else
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                    {
                        break;
                    }
                    else if (errno == EINTR)
                    {
                        continue;
                    }
                    else
                    {
                        isread = false;
                        conn->_excepter(conn);
                        break;
                    }
                }

            } while (conn->_events & EPOLLIN);
            return isread;
        }
        void HandlerRequest(Connection *conn)
        {
            bool flag = false;
            while (!flag)
            {
                // 1.读出data
                std::string retStr;
                int retn = ReadMes(conn->_inbuffer, &retStr);
                if (retn > 0)
                {
                    // 2.去除header
                    removeHeader(retStr, retn);
                    // 3.反序列化
                    Request req;
                    req.ReSerialize(retStr);
                    // 4.处理请求
                    Response resp = _func(req);
                    // 5.序列化
                    std::string respStr;
                    resp.Serialize(&respStr);
                    // 6.添加header
                    addHeader(respStr);
                    // 7.添加到outbuffer
                    conn->_outbuffer += respStr;
                }
                else
                {
                    flag = true;
                }
            }
        }
        void Reader(Connection *conn)
        {
            // 实际读取
            // //  std::cout << "read data..." << std::endl;
            // if(conn->_fd==_sock.GetFD()){
            //     Accepter(conn);
            //     return;
            // }
            if (!Recever(conn))
                return;
            HandlerRequest(conn);
            // 读取完毕后触发一次发送————写入时一般会直接写，只有没写完才会交给epoll
            if (!conn->_outbuffer.empty())
            {
                conn->_writer(conn);
            }
        }
        void Writer(Connection *conn)
        {
            // 处理写
            bool flag = true;
            do
            {
                int s = send(conn->_fd, conn->_outbuffer.c_str(), conn->_outbuffer.size(), 0);
                if (s < 0)
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                    {
                        break;
                    }
                    else if (errno == EINTR)
                    {
                        continue;
                    }
                    else
                    {
                        conn->_excepter(conn);
                        flag = false;
                        break;
                    }
                }
                conn->_outbuffer.erase(0, s);
                if (conn->_outbuffer.empty())
                {
                    break;
                }
            } while (conn->_events & EPOLLOUT);
            if (!flag)
                return;
            if (conn->_outbuffer.empty())
            {
                conn->_events &= (~EPOLLOUT);
            }
            else
            {
                conn->_events |= EPOLLOUT;
            }
        }
        void Excepter(Connection *conn)
        {
            CloseConnection(conn);
        }
        void LoopOnce()
        {
            int rfds = _epoller.WaitEvents(_epevs, MAX_EVENTS, TIMEOUT);
            // std::cout << "handle events..." << std::endl;
            for (int i = 0; i < rfds; ++i)
            {
                int fd = _epevs[i].data.fd;
                uint32_t events = _epevs[i].events;
                // 此处将异常处理全部都交给EPOLLIN 和 EPOLLOUT
                if (events & EPOLLERR)
                {
                    _epevs[i].events |= EPOLLIN;
                    _epevs[i].events |= EPOLLOUT;
                }
                // 判断连接是否存在
                // 读事件
                if ((events & EPOLLIN) && (ConnectionIsExist(fd)))
                {
                    _connections[fd]->_reader(_connections[fd]);
                }
                // 写事件
                if ((events & EPOLLOUT) && (ConnectionIsExist(fd)))
                {
                    _connections[fd]->_writer(_connections[fd]);
                }
            }
        }
        void AddConnection(int fd, uint32_t events, std::string &ip = default_ip, uint16_t port = default_port)
        {
            // LT模式下可以为阻塞或非阻塞，ET模式下必须为非阻塞
            if (events & EPOLLET)
            {
                Util::SetNoBlock(fd);
            }
            Connection *conn = new Connection(fd, ip, port, events, this);
            if (conn->_fd == _sock.GetFD())
            {
                conn->Register(std::bind(&EpollServer::Accepter, this, std::placeholders::_1), nullptr, nullptr);
            }
            else
            {
                conn->Register(std::bind(&EpollServer::Reader, this, std::placeholders::_1), std::bind(&EpollServer::Writer, this, std::placeholders::_1), std::bind(&EpollServer::Excepter, this, std::placeholders::_1));
            }
            _connections.insert(std::make_pair(fd, conn));
            _epoller.AddModEvents(fd, events, EPOLL_CTL_ADD);
            LogMessage(DEBUG, "add connection success,fd:%d,connection address:%s:%d", conn->_fd, conn->_ip.c_str(), conn->_port);
        }
        void CloseConnection(Connection *conn)
        {
            // 从epoll模型中移除
            _epoller.DelEvents(conn->_fd);
            // 关闭文件描述符
            close(conn->_fd);
            // 从连接管理器中移除
            _connections.erase(conn->_fd);
            // 删除connection对象
            LogMessage(DEBUG, "close connection success,fd:%d,", conn->_fd);
            delete conn;
        }
        bool ConnectionIsExist(int fd)
        {
            return _connections.find(fd) != _connections.end();
        }
        ~EpollServer()
        {
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