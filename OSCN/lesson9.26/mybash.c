#include<stdio.h>
#include<unistd.h>
#include<assert.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <stdlib.h>

#define _GNU_SOURCE 1
#define _SVID_SOURCE 1

#define ARR_SIZE 64
#define MAX_SIZE 10000
#define MAX_ROW 60


int Split(char* com,char* arr[]){
  assert(com);
  assert(arr);
  arr[0]=strtok(com," ");
  if(arr[0]==NULL)return -1;
  int i=1;
  while(1){
    arr[i]=strtok(NULL," ");
    if(arr[i]==NULL){
      break;
    }
    i++;
  }
  return 0;
}
void Print(char* arr[]){
  for(int i=0;arr[i];i++){
    printf("%d:%s\n",i,arr[i]);
  }
}

int main()
{
  extern char** environ;
  int exit_code=0;
  char* env_global[MAX_ROW];
  int index=0;
  while(1){
    char command[MAX_SIZE]={0};
    char *com_arr[ARR_SIZE]={NULL};
    printf("[yumu@mybash demo]#");
    fflush(stdout);
    char* id=fgets(command,sizeof(command),stdin);
    assert(id);
    (void)id;
    // printf("%s",command); 
    command[strlen(command)-1]='\0';
    int n = Split(command,com_arr);
    if(n!=0)continue;
    (void)n;
    if(strcmp(com_arr[0],"ls")==0){
      int pos=0;
      while(com_arr[++pos]);
      com_arr[pos++]=(char*)"--color=auto";
      com_arr[pos]=NULL;
      continue;
    }else if(strcmp(com_arr[0],"export")==0){
      if(com_arr[1]){
        strcpy(env_global[index],com_arr[1]);
        putenv((char*)env_global[index++]);
        //putenv((char*)str);
      }
      continue;
    }else if(strcmp(com_arr[0],"echo")==0){
      if(com_arr[1][0]=='$'){
        if(com_arr[1][1]=='?'){
          printf("%d\n",exit_code);
        }
        else{
          if(getenv(com_arr[1]+1)){
            printf("%s=%s\n",com_arr[1]+1,getenv(com_arr[1]+1));
          }
        }
      }
      continue;
    }
    if(strcmp(com_arr[0],"cd")==0){
      if(com_arr[1]) chdir(com_arr[1]);
    } 
    // Print(com_arr);
    pid_t ret=fork();
    assert(ret>=0);
    (void)ret;
    if(ret==0){
      execvpe(com_arr[0],com_arr,environ);
      exit(1);
    }
    int status=0;
    waitpid(ret,&status,0);
    if(ret>0){
      exit_code=WEXITSTATUS(status);
    }
  } 
  return 0;
}
