#include "cirque.hpp"
#include <pthread.h>
#include <unistd.h>
#include "task.hpp"
#include <time.h>

void* consumerRoutine(void* args)
{
    cirQue<Task> *cq = static_cast<cirQue<Task> *>(args);
    while(true)
    {
        //拿数据
        // sleep(1);
        Task t;
        cq->pop(&t);
        t();
        std::cout << "success get:" << t.formatRet() << std::endl;
    }
}
void* productorRoutine(void* args)
{
    cirQue<Task> *cq = static_cast<cirQue<Task> *>(args);
    while(true)
    {
        //放数据
        std::string ops = "+-*/%";
        int x = rand() % 10 + 1;
        int y = rand() % 30 + 1;
        char op = ops[rand() % ops.size()];
        Task t(x, y, op);
        cq->push(t);
        sleep(1);
        std::cout << "success add:" << t.formatSend() << std::endl;
    }
}
int main()
{
    srand(time(nullptr));
    pthread_t c[2], p[3];
    cirQue<Task> *cq=new cirQue<Task>();
    pthread_create(&c[0], nullptr, consumerRoutine, cq);
    pthread_create(&c[1], nullptr, consumerRoutine, cq);
    pthread_create(&p[0], nullptr, productorRoutine, cq);
    pthread_create(&p[1], nullptr, productorRoutine, cq);
    pthread_create(&p[2], nullptr, productorRoutine, cq);

    pthread_join(c[0], nullptr);
    pthread_join(c[1], nullptr);
    pthread_join(p[0], nullptr);
    pthread_join(p[1], nullptr);
    pthread_join(p[2], nullptr);
    delete cq;
    return 0;
}