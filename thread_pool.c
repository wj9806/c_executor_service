#include <pthread.h>
#include "thread_pool.h"
#include<stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct task
{
    /**
     * 这行代码定义了一个指针变量 function，它指向一个函数，
     * 该函数接受一个 void* 类型的参数，并且不返回任何值（void）。
     */
    void (*function)(void* arg);
    void* arg;
}task;

struct thread_pool
{
    task* _task_q;//任务队列
    int _queue_capacity; //容量
    int _queue_size; //当前任务个数
    int _queue_front; //队头
    int _queue_rear; //队尾

    pthread_t _manager_id;//管理者线程id

    pthread_t *_thread_ids;//工作线程id

    pthread_cond_t _not_full; //是不是满了
    pthread_cond_t _not_empty;//是不是空了

    int _min_num;//最小线程数
    int _max_num;//最大线程数
    int _busy_num;//忙碌线程数
    int _live_num;//存活线程数
    int _exit_num;//要销毁的线程数

    pthread_mutex_t _mutex_pool; //锁整个线程池
    pthread_mutex_t _mutex_busy; //锁busy_num变量

    int _shutdown; //是不是要销毁整个线程池 销毁1 不销毁0
};

thread_pool *create_thread_pool(int min, int max, int queue_size)
{
    //在C中，malloc 函数返回的指针类型是 void*，这是一种通用指针类型，可以指向任何类型的内存。
    thread_pool* pool = (thread_pool*) malloc(sizeof(thread_pool));

    do {
        if(pool == NULL)
        {
            printf("malloc thread_poll failed\n");
            break;
        }
        pool->_thread_ids = (pthread_t *)malloc(sizeof(pthread_t*) * max);
        if(pool->_thread_ids == NULL)
        {
            printf("malloc _thread_ids failed\n");
            break;
        }
        //将 pool->_thread_ids 指向的内存块的前 sizeof(pthread_t) * max 个字节设置为0
        memset(pool->_thread_ids, 0, sizeof(pthread_t) * max);

        pool->_min_num = min;
        pool->_max_num = max;
        pool->_busy_num = 0;
        pool->_live_num = min;
        pool->_exit_num = 0;

        if (pthread_mutex_init(&pool->_mutex_pool, NULL) != 0 ||
            pthread_mutex_init(&pool->_mutex_busy, NULL) != 0 ||
            pthread_cond_init(&pool->_not_full, NULL) != 0 ||
            pthread_cond_init(&pool->_not_empty, NULL) != 0)
        {
            printf("mutex or cond init failed\n");
        }

        pool->_task_q = (task*) malloc(sizeof(task*) * queue_size);
        pool->_queue_capacity = queue_size;
        pool->_queue_size = 0;
        pool->_queue_front = 0;
        pool->_queue_rear = 0;
        pool->_shutdown = 0;

        /**
         * 创建线程四个参数
         * 1.传入指向pthread_t的指针
         * 2.__attr 设置新线程的属性
         * 3.__start_routine 指定新线程将要执行的线程函数的地址
         * 4.__arg 传递给线程函数 __start_routine 的参数
         */
        //pthread_create(&pool->_manager_id, NULL, manager, NULL);
        for (int i = 0; i < pool->_live_num; ++i)
        {
            pthread_create(&pool->_thread_ids[i], NULL, worker, pool);
        }
        return pool;
    } while (0);

    if (pool && pool->_thread_ids) free(pool->_thread_ids);
    if (pool && pool->_task_q) free(pool->_task_q);
    if (pool) free(pool);
    return NULL;
}

void *worker(void *arg) {
    thread_pool* pool = (thread_pool*)arg;

    while (1)
    {
        pthread_mutex_lock(&pool->_mutex_pool);
        //当前队列是否为空
        while (pool->_queue_size == 0 && !pool->_shutdown)
        {
            //阻塞工作线程
            pthread_cond_wait(&pool->_not_empty, &pool->_mutex_pool);
        }
        //线程池是否关闭了
        if (pool->_shutdown)
        {
            pthread_mutex_unlock(&pool->_mutex_pool);
            pthread_exit(NULL);
        }
        pthread_mutex_unlock(&pool->_mutex_pool);
    }
    return NULL;
}
