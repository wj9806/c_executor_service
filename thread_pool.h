#ifndef C_EXECUTOR_SERVICE_THREAD_POOL_H
#define C_EXECUTOR_SERVICE_THREAD_POOL_H

typedef struct thread_pool thread_pool;

//创建线程池并初始化
thread_pool* create_thread_pool(int min, int max, int queue_size);

//销毁线程池

//给线程池添加任务

//获取线程池中工作的线程的个数

//获取线程池中活着的线程的个数


////////////////////////////////
void* worker(void* arg);


#endif //C_EXECUTOR_SERVICE_THREAD_POOL_H
