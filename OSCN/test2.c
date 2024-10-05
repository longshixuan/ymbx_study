#include<stdio.h>
#include<unistd.h>

int main(){
  int i=0;
  for(i=10;i--;i>0){
    printf("%d",i);
    fflush(stdout);
    sleep(2);
    printf("\r");
  }
  //printf("hello world\n");
  //printf("hello world\n");
  //printf("hello world\n");
  //printf("hello world\n");
  //printf("hello world\n");
  return 0;
}
