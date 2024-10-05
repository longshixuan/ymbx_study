#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(){
  printf("我是另一个程序,pid:%d,Myenv:%s\n",getpid(),getenv("Myenv"));
  return 0;
}
