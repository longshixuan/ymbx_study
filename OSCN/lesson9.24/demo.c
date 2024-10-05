#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

//int main(int argc, char * argv[],char * envp[]){
int main(){
  while(1){
    printf(".");
    fflush(stdout);
    sleep(1);
  }
//  if(argc!=2){
//    printf("please use -a|-b|-c\n");
//  }else if(strcmp(argv[1],"-a")==0){
//    printf("this is -a\n");
//  }else if(strcmp(argv[1],"-b")==0){
//    printf("this is -b\n");
//  }else if(strcmp(argv[1],"-c")==0){
//    printf("this is -c\n");
//  }else{
//
//  }

//  printf("argc:%d\n",argc);
//  for(int i=0;argv[i];i++){
//    printf("argv[%d]->%s\n",i,argv[i]);
//  }
  //char * pwd=getenv("PWD");
  //if(pwd==NULL) perror("getenv");
  //else{
  //printf("%s\n",pwd);}
//  extern char** environ;
//  for(int i=0;environ[i];i++){
//    printf("environ[%d]->%s\n",i,environ[i]);
//  }
  //for(int i=0;envp[i];i++){
  //  printf("envp[%d]->%s\n",i,envp[i]);
  //}
//  pid_t ret=fork();
//  if(ret==0){
//    while(1){
//      printf("我是子进程，pid:%d,ppid:%d\n",getpid(),getppid());
//      sleep(1);
//    }
//  }else if(ret > 0){
//    while(1){
//      printf("我是父进程，pid:%d,ppid:%d\n",getpid(),getppid());
//      sleep(1);
//    }
//  }else{}
  return 0;
}
