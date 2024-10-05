#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main(){
//  int count=0;
//  while(1){
//    printf("我正在运行,%d\n",count);
//    count++;
//  }
  pid_t ret = fork();
  if(ret == 0)
  {
    while(1){
    printf("我是子进程，pid:%d,ppid:%d\n",getpid(),getppid());
    sleep(1);
  }
  }
  else if(ret >0){
    while(1){
    printf("我是父进程，pid:%d,ppid:%d\n",getpid(),getppid());
    sleep(1);
  }
  }
  return 0;
}
