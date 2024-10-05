#include "comm.hpp"

int main()
{
    Init init(CLIENT);
    char *start = init.getStart();
    for (char i = 'a'; i < 'z'; i++)
    {
        start[i - 'a'] = i;
        sleep(1);
    }
    // key_t keyid = getKey();
    // int shmid = getShm(keyid, SHM_SIZE);
    // cout << "key:" << keyid << endl;
    // cout << "shmid:" << shmid << endl;
    // sleep(5);
    // char* start=attachShm(shmid);
    // sleep(8);
    // unattachShm(start);
    // sleep(5);
    return 0;
}