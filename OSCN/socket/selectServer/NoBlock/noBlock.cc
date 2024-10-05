#include<iostream>
#include<memory>
#include<unistd.h>
#include<cstring>
#include<cstdio>
#include<error.h>
#include<fcntl.h>
#include<functional>
#include<vector>



using func_t = std::function<void(void)>;
std::vector<func_t> taskArr;

void Message(){
    std::cout << "process message" << std::endl;
}
void MySQL(){
    std::cout << "process sql sentence" << std::endl;
}
void NetWork(){
    std::cout << "process net resource" << std::endl;
}
void DoTask(){
    for(auto & func:taskArr){
        func();
    }
}
void LoadTask(){
    taskArr.push_back(Message);
    taskArr.push_back(MySQL);
    taskArr.push_back(NetWork);
}
//core code:非阻塞轮询
void SetNoBlock(int fd){
    //获取文件状态标记：F_GETFL
    //获取文件描述符标记：F_GETFD
    int f1 = fcntl(fd, F_GETFL);
    if(f1<0){
        perror("fcntl");
        return;
    }
    fcntl(fd, F_SETFL, f1 | O_NONBLOCK);
}
const static int N = 64;
int main()
{
    char buffer[N];
    SetNoBlock(0);
    LoadTask();
    while (true)
    {
        std::cout << ">>>";
        fflush(stdout);
        sleep(1);
        //读取的结果：1.成功 2.结尾 3.出错 4.数据未就绪以出错方式输出
        ssize_t n = read(0, buffer, sizeof(buffer) - 1);
        if(n>0){
            buffer[n-1] = 0;
            std::cout << "buffer #" << buffer << std::endl;
        }
        else if(n==0){
            std::cout << "end of file" << std::endl;
            break;
        }
        else{
            if(errno==EAGAIN || errno==EWOULDBLOCK){
                std::cout << "data is readying..." << std::endl;
                DoTask();
                continue;
            }
            else if(errno==EINTR){
                //信号中断
                continue;
            }
            else{
                std::cerr << "read error:" << strerror(errno) << "[" << errno << "]" << std::endl;
            }
        }
    }

    return 0;
}