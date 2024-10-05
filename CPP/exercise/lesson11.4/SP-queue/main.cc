#include <iostream>
#include "sp.hpp"
#include <unistd.h>
#include "task.hpp"
#include <string>


#define CAPACITY 100
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

void *consumer(void *args)
{
    blockQueue<Task> *bq = static_cast<blockQueue<Task> *>(args);
    while(true)
    {
        // sleep(1);
        Task t;
        bq->pop(&t);
        t();
        pthread_mutex_lock(&mutex);
        std::cout << t._x << t._op << t._y << "=" << t._result << std::endl;
        pthread_mutex_unlock(&mutex);
    }
}
void* produce(void* args)
{
    std::string op = "+-*/%";
    blockQueue<Task> *bq = static_cast<blockQueue<Task> *>(args);
    while(true)
    {
        // sleep(1);
        int x = rand() % 10 + 1;
        int y = rand() % 20 + 1;
        int tmp = rand() % op.size();
        char opt = op[tmp];
        Task t(x, y, opt);
        bq->push(t);
        pthread_mutex_lock(&mutex);
        std::cout << x<<opt<<y<<"=?" << std::endl;
        pthread_mutex_unlock(&mutex);
    }
}

int main()
{
    // blockQueue<int> *bq = new blockQueue<int>();
    // pthread_t c, p;
    // pthread_create(&c, nullptr, consumer, bq);
    // pthread_create(&p, nullptr, produce, bq);

    // pthread_join(c, nullptr);
    // pthread_join(p, nullptr);

    blockQueue<Task> *bq = new blockQueue<Task>();
    pthread_t cs[2], ps[3];
    pthread_create(&cs[0], nullptr, consumer, bq);
    pthread_create(&cs[1], nullptr, consumer, bq);
    pthread_create(&ps[0], nullptr, produce, bq);
    pthread_create(&ps[1], nullptr, produce, bq);
    pthread_create(&ps[2], nullptr, produce, bq);

    pthread_join(cs[0], nullptr);
    pthread_join(cs[1], nullptr);
    pthread_join(ps[0], nullptr);
    pthread_join(ps[1], nullptr);
    pthread_join(ps[2], nullptr);
    return 0;
}
