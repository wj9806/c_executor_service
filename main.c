#include<stdio.h>
#include <malloc.h>
#include "thread_pool.h"
#include<pthread.h>
#include <unistd.h>

void my_task(void* arg)
{
    int num = *(int*)arg;
    printf("threadis working, number = %d, tid = %ld\n", num, pthread_self());
    sleep(1);
}

int main()
{
    //创建线程池
    thread_pool *pool = create_thread_pool(3, 10, 10);
    for (int i = 0; i < 100; ++i)
    {
        int * num = (int*)malloc(sizeof(int));
        *num = i + 100;
        submit(pool, my_task, num);
    }

    sleep(30);


    shutdown(pool);
    return 0;
}