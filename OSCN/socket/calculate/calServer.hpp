#pragma once
#include "protocol.hpp"
#include "sock.hpp"
#include <functional>
#include <pthread.h>

#define DEFAULT_PORT 8989
class calServer;
class threadData
{
public:
    threadData(int sock, std::string ip, uint16_t port, calServer *calS) : _sock(sock), _ip(ip), _port(port), _calS(calS) {}
    ~threadData() {}

public:
    int _sock;
    std::string _ip;
    uint16_t _port;
    calServer *_calS;
};
using func_t = std::function<Response(const Request&)>;
class calServer
{
public:
    calServer(func_t func, uint16_t port = DEFAULT_PORT) : _func(func), _port(port) {}
    void InitServer()
    {
        // 1.创建套接字
        _sock.Socket();
        // 2.绑定IP和端口
        _sock.Bind(_port);
        // 3.监听端口
        _sock.Listen();
    }
    void ServerIO(int sockfd, std::string client, uint16_t port)
    {
        std::string inbuffer;
        while (true)
        {
            // 1.将数据从网络中读取出来
            // 2.对读取出来的数据进行验证
            std::string retmes;
            int n = ReadMes(sockfd, inbuffer, &retmes);
            if (n == -2)
            {
                break;
            }
            else if (n == -1 || n == 0)
            {
                continue;
            }
            // 3.0去除报头
            retmes = removeHeader(retmes, n);
            // std::cout << "server read success:" << retmes << std::endl;
            // 3.将网络数据转换为本地格式的数据————反序列化
            Request req;
            req.ReSerialize(retmes);
            req.PrintReq();
            // 4.实际的业务处理
            Response result = _func(req);
            result.PrintRes();
            // 5.将本地数据发送到网络中————序列化
            std::string res;
            result.Serialize(&res);
            res = addHeader(res);
            // std::cout << res << std::endl;
            send(sockfd, res.c_str(), res.size(), 0);
        }
        close(sockfd);
    }
    static void *threadRoutine(void *args)
    {
        pthread_detach(pthread_self());
        threadData *td = static_cast<threadData *>(args);
        td->_calS->ServerIO(td->_sock, td->_ip, td->_port);
        delete td;
        return nullptr;
    }
    void Start()
    {
        while (true)
        {
            struct sockaddr_in client;
            int fd = _sock.Accept(&client);
            if (fd < 0)
            {
                continue;
            }
            std::string cip = inet_ntoa(client.sin_addr);
            uint16_t cport = ntohs(client.sin_port);

            // 多线程或线程池调用
            pthread_t pid;
            threadData *td = new threadData(fd, cip, cport, this);
            pthread_create(&pid, nullptr, threadRoutine, td);
        }
    }
    ~calServer() {}

private:
    ymbxSocket _sock;
    uint16_t _port;
    func_t _func;
};