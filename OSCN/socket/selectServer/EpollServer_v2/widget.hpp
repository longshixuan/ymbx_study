#pragma once

namespace ymbx
{
    #define MAX_FD 1024
    #define EPOLL_CREATE_SIZE 128
    #define MAX_EVENTS 128
    #define TIMEOUT -1


//与错误信息有关
    enum ERROR_CATEGORY
    {
        ERR_USAGE,
        ERR_CREATESOCK,
        ERR_LISTENSOCK,
        ERR_BINDSOCK,
        ERR_ACCEPTSOCK,

    };

//与日志等级有关
    enum LOG_LEVEL
    {
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL
    };
}