#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <cstdio>


int main()
{
    int pipefd[2] = {0};
    int pip = pipe(pipefd);
    if(pip < 0){
        std::cout << errno << strerror(errno) << std::endl;
    }
    //std::cout << pipefd[0] << "  " << pipefd[1] << std::endl;
    int fd = fork();
    assert(fd != -1);
    if(fd==0){
        //子进程
        close(pipefd[0]);
        const std::string namestr = "hello, 我是子进程";
        int cnt = 1;
        char buffer[1024];
        while(true){
            snprintf(buffer, sizeof buffer, "%s:cnt:%d,PID:%d", namestr.c_str(), cnt++, getpid());
            write(pipefd[1], buffer, strlen(buffer));
            sleep(1);
        }
    }
    char buffer_rd[1024];
    close(pipefd[1]);
    while(true){
        int n = read(pipefd[0], buffer_rd, sizeof(buffer_rd) - 1);
        if(n>0){
            buffer_rd[n] = '\n';
            std::cout << "the rd msg:" << buffer_rd << std::endl;
        }
    }
    return 0;
}