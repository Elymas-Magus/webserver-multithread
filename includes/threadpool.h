#ifndef THREADPOOL_H_INCLUDED
#define THREADPOOL_H_INCLUDED

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>

typedef struct task{
    void (*func)(void *);
    void * arg;
    struct tk_task * next;    // 任务链表（下一节点指针）
} task_t;

typedef struct threadpool {
    pthread_mutex_t lock;
    pthread_cond_t cond;
    pthread_t * threads;
    task_t * head;
    int length;
    int queue_size;
} threadpool_t;

#endif // THREADPOOL_H_INCLUDED
