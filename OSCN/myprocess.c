#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main(){
//  while(1){
//    printf("hello myprocess,我的进程ID是%d,我的父进程是%d\n",getpid(),getppid());
//    sleep(1);
//  }
//  printf("AAAAAAAAAAAAAAAAAAAAAAAA,pid:%d\n",getpid()); 
//  pid_t ret=fork();
//  printf("BBBBBBBBBBBBBBBBBBBBBBB,pid:%d,ppid:%d,&ret:%d\n",getpid(),getppid(),&ret); 
  
  pid_t ret=fork();
  if(ret==0){
    while(1){
      printf("我是子进程，pid:%d,ppid:%d\n",getpid(),getppid());
      sleep(1);
    }
  }else if(ret>0){
    while(1){
      printf("我是父进程，pid:%d,ppid:%d\n",getpid(),getppid());
      sleep(1);
    }
  }else{
  printf("创建失败\n");
  }
  return 0;
}
