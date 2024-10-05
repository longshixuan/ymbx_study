#pragma once

#include "err.hpp"
#include "log.hpp"
#include <cstring>
#include <fcntl.h>
#include <signal.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace ymbx
{
    // 守护进程：
    // 1.守护进程不能是进程组组长
    // 2.忽略异常信号
    // 3.关闭不需要的文件描述符
    // 4.更改工作路径
    void Daemon()
    {
        // 忽略异常信号
        signal(SIGCHLD, SIG_IGN);
        signal(SIGPIPE, SIG_IGN);

        // 守护进程不设置为进程组组长
        if (fork() > 0)
            exit(0);
        // 创建守护进程
        pid_t ret = setsid();
        if (ret == -1)
        {
            logMessage(Fatal, "daemon create error>>>%d:%s", errno, strerror(errno));
            exit(DAEMON_ERR);
        }
        // 关闭文件描述符
        int fd = open("/dev/null", O_RDWR);
        if (fd < 0)
        {
            logMessage(Fatal, "open error>>>%d:%s", errno, strerror(errno));
            exit(OPEN_ERR);
        }
        dup2(fd, 0);
        dup2(fd, 1);
        dup2(fd, 2);
        close(fd);
    }
}