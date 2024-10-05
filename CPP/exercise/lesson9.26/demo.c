#include<stdio.h>
#include<unistd.h>
#include<assert.h>
#include<sys/types.h>
#include<stdlib.h>

int main(){
  extern char** environ;
  pid_t id =fork();
  assert(id>=0);
  if(id==0){
    //child
    printf("我是子进程,我的id是:%d\n",getpid());
  //  execl("/usr/bin/ls","ls","-a","-l",NULL);
    //execlp("ls","ls","-a","-l",NULL);
    putenv("Myenv=123456");
    //execle("./exec/otherproc","otherproc",NULL,environ);
    char * group[]={
      "ls",
      "-a",
      "-l",
      NULL
    };
    execvp("ls",group);
    //execv("/usr/bin/ls",group);
   // execve("/usr/bin/ls",group,environ);
    exit(1);
  }
  sleep(5);
  printf("我是父进程,我的id是:%d\n",getpid());
  return 0;
}
