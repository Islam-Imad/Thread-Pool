
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>
typedef struct
{
    void (*function)(void *);
    void *argument;
} task_t;

struct task_node
{
    task_t task;
    struct task_node *next;
};

struct task_queue
{
    pthread_mutex_t lock;
    struct task_node *front;
    struct task_node *rear;
    int size;
};

void init_task_queue(struct task_queue *queue);

void destroy_task_queue(struct task_queue *queue);

void enqueue_task(struct task_queue *queue, task_t task);

void dequeue_task(struct task_queue *queue, task_t *task);

/* Execute all tasks currently in the queue by calling their function pointers. */
void execute_tasks(struct task_queue *queue);

struct threads_pool
{
    int num_threads;
    int shutdown;
    pthread_t *threads;
    struct task_queue task_queue;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
};

void *worker_thread(void *arg);

void init_threads_pool(struct threads_pool *pool, int num_threads);

void add_task(struct threads_pool *pool, task_t task);

void wait_shutdown_threads_pool(struct threads_pool *pool);

#endif // THREAD_POOL_H