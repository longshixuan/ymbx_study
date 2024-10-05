#include <iostream>
#include <unistd.h>
#include <signal.h>

void myhandler(int sig)
{
    std::cout << "get a signal:" << sig << std::endl;
}
int main(){
    signal(SIGINT, myhandler);
    while(true){
        std::cout << "i am running..." << getpid()<<std::endl;
        sleep(1);
    }
}
