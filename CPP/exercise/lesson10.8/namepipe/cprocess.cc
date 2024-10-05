#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <cassert>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <cstring>
#include "head.hpp"

int main()
{
    int n = mkfifo("./namepipe", 0666);
    assert(n == 0);
    (void)n;
    int fd = open("./namepipe", O_WRONLY);
    assert(fd >= 0);
    int cnt = 0;
    while (true)
    {
        char buffer[CNT_NUM];
        std::cout << "请输入#";
        char *rft = fgets(buffer, sizeof(buffer), stdin);
        assert(rft != NULL);
        // char *msg = "I am WRITTING";
        // snprintf(buffer, sizeof(buffer), "我是写进程：%d,我正在写id:%d\n", getpid(),cnt++);
        // std::cout << "我是写进程，我要写了" << std::endl;
        // std::cout << "fd:" << fd << std::endl;
        write(fd, buffer, strlen(buffer) - 1);
        // std::cout << "我是写进程，我写完了" << std::endl;
        sleep(1);
    }
    return 0;
}