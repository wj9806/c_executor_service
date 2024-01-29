#include<stdio.h>
#include "thread_pool.h"

int main()
{
    thread_pool *pool = create_thread_pool(1, 10, 10);
    return 0;
}