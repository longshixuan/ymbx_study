//系统方案文件
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<string.h>

#define LOG "log.txt"


int main()
{
  printf("%d\n",stdin->_fileno);
  printf("%d\n",stdout->_fileno);
  printf("%d\n",stderr->_fileno);

  //int ret = open(LOG,O_WRONLY | O_CREAT | O_TRUNC,0666);
 // int ret = open(LOG,O_RDONLY);
 // if(ret==-1){
 //   printf("ret:%d,error:%d,strerror:%s\n",ret,errno,strerror(errno));
 // }else{
 //   printf("ret:%d,error:%d,strerror:%s\n",ret,errno,strerror(errno));
 // }
 // char buffer[1024];
 // read(ret,buffer,sizeof(buffer)-1);
 // printf("%s",buffer);
  //const char* msg="bbbbbbb";
  //int cnt=5;
  //while(cnt){
  //  char buffer[1024];
  //  snprintf(buffer,sizeof(buffer),"%s:%d\n",msg,cnt);
  //  write(ret,buffer,strlen(buffer));
  //  cnt--;
  //}
  return 0;
}




























//C语言方案实现文件访问

//#include<stdio.h>

//
//#define LOG "log.txt"
//
//
//int main()
//{
//  FILE* fd=fopen(LOG,"a");
//  if(fd==NULL){
//    return -1;
//  }
//  const char* msg="aaaaa\n";
//  int cnt=5;
//  while(cnt){
//    fputs(msg,fd);
//    cnt--;
//  }
//  return 0;
//}
