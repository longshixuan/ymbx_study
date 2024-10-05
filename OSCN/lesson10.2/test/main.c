#include"mystdio.h"
#include<string.h>
#include<unistd.h>

#define LOG "log.txt"

int main(){
  MY_FILE* myfile=my_fopen(LOG,"w");
  if(myfile==NULL) return 1;
  int cnt = 50;
  const char* msg="hello bit";
  while(cnt){
    char buffer[1024];
    snprintf(buffer,sizeof(buffer),"%s:%d",msg,cnt--);
   size_t size=my_fwrite(buffer,strlen(buffer),1,myfile);
   sleep(1);
   printf("current bytes:%lu\n",size);
  }

  my_fclose(myfile);
  return 0;
}
