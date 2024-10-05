#pragma once

#include <cstdio>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

using namespace std;


#define PATHNAME "."
#define PROJ_ID 0x11111111
#define SHM_SIZE 4096

#define CLIENT 1
#define SERVER 0
key_t getKey(){
    key_t getkey = ftok(PATHNAME, PROJ_ID);
    if(getkey==-1){
        cerr << errno << ":"<< strerror(errno) << endl;
    }
    return getkey;
}
int creatShm(key_t key,size_t size){
    umask(0);
    int shmid = shmget(key, size, IPC_CREAT | IPC_EXCL | 0666);
    if (shmid == -1)
    {
        cerr << "创建失败："<<errno << ":"<< strerror(errno) << endl;
    }
    return shmid;
}
int getShm(key_t key,size_t size){
    int shmid = shmget(key, size, IPC_CREAT);
    if(shmid==-1){
        cerr <<"获取失败："<< errno << ":" << strerror(errno) << endl;
    }
    return shmid;
}

//转十六进制
// char* toHex(int x){
//     char buffer[64];
//     char *ch = itoa(x, buffer, 16);
//     snprintf(buffer,sizeof(buffer),"0x%x",x);
//     return ch;
// }

//此处链接失败是因为权限不够
char * attachShm(int shmid){
    char *dest = (char*)shmat(shmid, nullptr, 0);
    // if(dest==(char*)-1){
    //     cerr << "链接失败："<<errno << ":" << strerror(errno) << endl;
    // }
    return dest;
}
void unattachShm(char* start){
    int rt = shmdt(start);
    if(rt==-1){
        cerr << "断联失败："<<errno << ":" << strerror(errno) << endl;
    }
}
void delShm(int shmid){
    int del = shmctl(shmid, IPC_RMID, nullptr);
    if(del==-1){
        cerr <<"删除失败："<< errno << ":" << strerror(errno) << endl;
    }
}

class Init{
public:
    Init(int t):type(t){
        key_t key=getKey();
        if(t==CLIENT){
            shmid=getShm(key, SHM_SIZE);
        }else if(t==SERVER){
            shmid = creatShm(key, SHM_SIZE);
        }
        start = attachShm(shmid);
    }
    char* getStart(){
        return start;
    }
    ~Init(){
        unattachShm(start);
        if(type==SERVER) delShm(shmid);
    }

private:
    char *start;
    int type;
    int shmid;
};