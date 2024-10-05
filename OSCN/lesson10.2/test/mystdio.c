#include "mystdio.h"
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<malloc.h>
#include<assert.h>


MY_FILE* my_fopen(const char *path, const char *mode){
  int flag=0;
  if(strcmp(mode,"r")==0) flag|= O_RDONLY;
  else if(strcmp(mode,"w")==0) flag |= (O_CREAT |O_WRONLY | O_TRUNC);
  else if(strcmp(mode,"a")==0) flag |= (O_CREAT |O_WRONLY | O_APPEND);
  else{
    //other option
  }
  int fd;
  if(flag & O_CREAT){
    fd = open(path,flag,0666);
  }else{
    fd=open(path,flag);
  }
  if(fd<0) return NULL;
  MY_FILE *myfile=(MY_FILE*)malloc(sizeof(MY_FILE));
  if(myfile==NULL){
    close(fd);
    return NULL;
  }
  myfile->fd=fd;
  myfile->flag=BUFF_LINE;
  memset(myfile->outputbuffer,'\0',sizeof(myfile->outputbuffer));
  myfile->current=0;
  return myfile;
}
int my_fflush(MY_FILE *fp){
  assert(fp);
  write(fp->fd,fp->outputbuffer,fp->current);
  fp->current=0;
  fsync(fp->fd);//刷新进入磁盘
  return 0;
}
size_t my_fwrite(const void *ptr, size_t size, size_t nmemb,MY_FILE *stream){
  assert(stream);
  if(stream->current==NUM) my_fflush(stream);
  size_t user_size=nmemb*size;
  size_t rest_size=NUM-stream->current;
  size_t writen_size;
  if(user_size<rest_size){
  memcpy((stream->outputbuffer)+stream->current,ptr,user_size);
    stream->current+=user_size;
    writen_size=user_size;
  }else{
    memcpy((stream->outputbuffer)+stream->current,ptr,rest_size);
    stream->current+=rest_size;
    writen_size=rest_size;
  }
  if(stream->flag & BUFF_ALL){
    if(stream->current==NUM){
      my_fflush(stream);
    }
  }else if(stream->flag & BUFF_LINE){
    if(stream->outputbuffer[stream->current-1]=='\n'){
      my_fflush(stream);
  }
  }else{
    
  }
  return writen_size;
}
//size_t fread(void *ptr, size_t size, size_t nmemb, MY_FILE *stream){
//
//}
int my_fclose(MY_FILE *fp){
  assert(fp);
  if(fp->current > 0){
    my_fflush(fp);
  }
 close(fp->fd);
 free(fp);
 fp=NULL;
 return 0;
}
