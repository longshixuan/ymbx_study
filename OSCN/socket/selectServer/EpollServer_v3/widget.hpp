#pragma once

namespace ymbx
{
#define MAX_FD 1024
#define EPOLL_CREATE_SIZE 128
#define MAX_EVENTS 128
#define TIMEOUT -1

    // 与错误信息有关
    enum ERROR_CATEGORY
    {
        ERR_USAGE = 100,
        ERR_CREATESOCK = 200,
        ERR_LISTENSOCK,
        ERR_BINDSOCK,
        ERR_ACCEPTSOCK,
        ERR_SETNOBLOCK = 300,

    };

    // 与日志等级有关
    enum LOG_LEVEL
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };

    // 协议返回状态
    enum RetCode
    {
        Correct = 0,
        DivisionZero,
        ModZero,
        Other
    };
}