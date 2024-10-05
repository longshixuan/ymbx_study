#include "comm.hpp"

int main()
{
    Init init(SERVER);
    char *start = init.getStart();
    int cnt = 30;
    while (cnt--)
    {
        printf("%s", start);
        fflush(stdout);
        sleep(1);
    }



    //扩展版
        //client写完之后通知server来读，server读完之后通知client来写
        //通过namepipe来实现




        // //1.获取key
        // key_t keyid = getKey();
        // // 2.创建共享内存
        // int shmid = creatShm(keyid, SHM_SIZE);
        // cout << "key:" << keyid << endl;
        // cout << "shmid:" << shmid << endl;
        // sleep(5);
        // // struct shmid_ds sh;
        // // int n=shmctl(shmid, IPC_STAT, &sh);
        // // cout << "atime:" << sh.shm_atime << endl;
        // // cout << "pid:" << sh.shm_cpid << ":" << getpid() << endl;

        // // 3.建立与共享内存的联系
        // char* start=attachShm(shmid);
        // sleep(15);

        // //进行通信

        // //4.取消与共享内存之间的联系
        // unattachShm(start);
        // sleep(5);
        // // 5.关闭共享内存
        // delShm(shmid);

        return 0;
}