#include <iostream>
#include <sys/types.h>
#include <signal.h>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>


void Usage(){
    std::cout << "OPTION:\n\t";
    std::cout << "./mykill 信号  进程id" << std::endl;
}

void myhandler(int sig){
    std::cout << "get a signal:" << sig << std::endl;
    int n=alarm(20);
    std::cout << n << std::endl;

}
int main(int argc,char* argv[]){
    signal(SIGALRM, myhandler);
    std::cout << getpid() << std::endl;
    unsigned int seconds = 20;
    unsigned int rd = alarm(seconds);
    int cnt = 0;
    while (true)
    {
        // std::cout << rd << std::endl;
        // sleep(1);
        if(cnt==5){
            alarm(0);
        }
        cnt++;
        sleep(1);
    }
    // if(argc!=3){
    //     Usage();
    //     exit(1);
    // }
    // int sig = atoi(argv[1]);
    // pid_t pid = atoi(argv[2]);
    // abort();
    // signal(sig, myhandler);
    // raise(sig);
    // int n=kill(pid, sig);
    // if(n!=0){
    //     std::cerr << errno << ":" << strerror(errno) << std::endl;
    // }

    // signal(SIGINT, myhandler);
    return 0;
}  