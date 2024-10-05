#include<stdio.h>

int add(int a,int b){
  return a+b;
}


int main()
{
  int i=9;
  int j=8;
  int sum=add(i,j);
  printf("%d\n",sum);
  return 0;
}
