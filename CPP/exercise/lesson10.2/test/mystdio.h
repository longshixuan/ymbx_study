#pragma once

#include<stdio.h>

#define NUM 1024
#define BUFF_NO 0x1
#define BUFF_LINE 0x2
#define BUFF_ALL 0x4

typedef struct MY_FILE
{
  int fd;
  char outputbuffer[NUM];
  int flag;
  int current;
} MY_FILE;


MY_FILE* my_fopen(const char *path, const char *mode);
int my_fllush(MY_FILE *fp);
size_t my_fwrite(const void *ptr, size_t size, size_t nmemb,MY_FILE *stream);
size_t my_fread(void *ptr, size_t size, size_t nmemb, MY_FILE *stream);
int my_fclose(MY_FILE* fp);

