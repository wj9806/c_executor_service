#ifndef C_EXECUTOR_SERVICE_THREAD_POOL_H
#define C_EXECUTOR_SERVICE_THREAD_POOL_H

typedef struct thread_pool thread_pool;

typedef struct task task;

//创建线程池并初始化
thread_pool* create_thread_pool(int min, int max, int queue_size);

//销毁线程池
int shutdown(thread_pool* pool);

//给线程池添加任务
void submit(thread_pool* pool, void(*func)(void*), void* arg);

//获取线程池中工作的线程的个数
int busy_num(thread_pool* pool);

//获取线程池中活着的线程的个数
int live_num(thread_pool* pool);


////////////////////////////////
void* worker(void* arg);

void *manager(void *arg);

void thread_exit(thread_pool* pool);

#endif //C_EXECUTOR_SERVICE_THREAD_POOL_H
