#include <iostream>
#include <signal.h>

int quit=0;

void handler(int signo)
{
  std::cout<<"change quit"<<std::endl;
  quit=1;
  std::cout<<quit<<std::endl;
}
int main()
{
  signal(2,handler);
  while(!quit);

  std::cout<<"quit ----"<<std::endl;
  return 0;

}
