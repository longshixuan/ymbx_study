#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <cassert>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include "head.hpp"
#include <cerrno>
#include <cstring>

int main(){
    int fd=open("./namepipe", O_RDONLY);
    assert(fd >= 0);
    // int cnt = 0;
    while (true)
    {
        char buffer[CNT_NUM];
        // std::cout << "fd:" << fd << std::endl;
        int cnt = read(fd, buffer, sizeof(buffer));
        if(cnt>0){
            fprintf(stdout,"%s", buffer);
            fflush(stdout);
        }
        else if (cnt == 0)
        {
            std::cout << "没内容，我退出了" << std::endl;
            break;
        }
        else
        {
            std::cout << errno << strerror(errno) << std::endl;
            break;
        }
    }
    return 0;
}