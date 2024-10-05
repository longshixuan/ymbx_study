#pragma once

namespace ymbx
{
    #define MAX_FD 1024
    #define EPOLL_CREATE_SIZE 128
    #define MAX_EVENTS 128
    #define TIMEOUT -1



    enum LOG_LEVEL
    {
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL
    };
}