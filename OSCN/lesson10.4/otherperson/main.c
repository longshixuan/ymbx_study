#include<stdio.h>
#include"myadd.h"
#include"mysub.h"
#include<ncurses.h>

int main()
{
  int x=10,y=20;

  printf("the sum:%d\n",myadd(x,y));
  printf("the sub:%d\n",mysub(x,y));
  return 0;
}
