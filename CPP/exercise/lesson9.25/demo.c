#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<assert.h>
#include<stdlib.h>
#include<sys/wait.h>

int main(){
  pid_t id= fork();
  assert(id>=0);
  if(id ==0){
    int cnt=5;
    while(cnt--){
      printf("我是子进程，我%dS后退出了，pid:%d,ppid:%d\n",cnt,getpid(),getppid());
      sleep(1);
    }
    exit(107);
  }
  while(1){
  int status=0;
  pid_t ret=waitpid(id,&status,WNOHANG);
    if(ret<-1){
      printf("出错了\n");
      exit(1);
    }else if(ret ==0){
      printf("再等等\n");
      sleep(1);
      continue;
    }else{
      if(WIFEXITED(status)){

      printf("我是父进程，pid:%d,ppid:%d,我回收的进程id是:%d,我的exit_code是:%d\n",getpid(),getppid(),ret,WEXITSTATUS(status));
      }else{
        printf("我的退出信号是:%d\n",status&0x7F);
      }
   break;
    }
  }
  //sleep(5);
//  for(int i=0;i<150;i++){
//    printf("%d:%s\n",i,strerror(i));
//  }
  return 0;
}
