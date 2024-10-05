#include<stdio.h>
#include<unistd.h>
int main(){
  int i=0;
  for(;i<=100;i++){
    printf("[");
    int j=i;
    while(j--){
      printf("*");
    }
    int count=100-i;
    while(count--){
      printf(" ");
    }
    printf("][%3d%]",i);
    printf("\r");
    fflush(stdout);
    usleep(100000);
  }
    printf("\n");
   return 0;
}
