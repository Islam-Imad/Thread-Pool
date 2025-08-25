#include "thread_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void init_task_queue(struct task_queue *queue)
{
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    pthread_mutex_init(&queue->lock, NULL);
}

void destroy_task_queue(struct task_queue *queue)
{
    while (queue->size > 0)
    {
        task_t task;
        dequeue_task(queue, &task);
    }
    pthread_mutex_destroy(&queue->lock);
}

void enqueue_task(struct task_queue *queue, task_t task)
{
    pthread_mutex_lock(&queue->lock);
    if (queue->size == 0)
    {
        struct task_node *new_node = (struct task_node *)malloc(sizeof(struct task_node));
        new_node->task = task;
        new_node->next = NULL;
        queue->front = new_node;
        queue->rear = new_node;
        queue->size++;
    }
    else
    {
        struct task_node *new_node = (struct task_node *)malloc(sizeof(struct task_node));
        new_node->task = task;
        new_node->next = NULL;
        queue->rear->next = new_node;
        queue->rear = new_node;
        queue->size++;
    }
    pthread_mutex_unlock(&queue->lock);
}

void dequeue_task(struct task_queue *queue, task_t *task)
{
    pthread_mutex_lock(&queue->lock);
    if (queue->size > 0)
    {
        struct task_node *temp = queue->front;
        *task = temp->task;
        queue->front = temp->next;
        if (queue->front == NULL)
        {
            queue->rear = NULL;
        }
        free(temp);
        queue->size--;
    }
    else
    {
        task->function = NULL;
        task->argument = NULL;
    }
    pthread_mutex_unlock(&queue->lock);
}

void execute_tasks(struct task_queue *queue)
{
    while (queue->size > 0)
    {
        task_t task;
        dequeue_task(queue, &task);
        if (task.function != NULL)
        {
            task.function(task.argument);
        }
    }
}

void *worker_thread(void *arg)
{
    struct threads_pool *pool = (struct threads_pool *)(arg);
    while (1)
    {
        pthread_mutex_lock(&pool->lock);
        while (pool->task_queue.size == 0 && !pool->shutdown)
        {
            pthread_cond_wait(&pool->not_empty, &pool->lock);
        }
        if (pool->task_queue.size == 0 && pool->shutdown)
        {
            pthread_mutex_unlock(&pool->lock);
            break;
        }
        task_t task;
        dequeue_task(&pool->task_queue, &task);
        pthread_mutex_unlock(&pool->lock);
        if (task.function != NULL)
        {
            task.function(task.argument);
        }
    }
    return NULL;
}

void init_threads_pool(struct threads_pool *pool, int num_threads)
{
    pool->shutdown = 0;
    pool->num_threads = num_threads;
    pool->threads = malloc(num_threads * sizeof(pthread_t));
    init_task_queue(&pool->task_queue);
    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->not_empty, NULL);
    for (int i = 0; i < num_threads; i++)
    {
        pthread_create(&pool->threads[i], NULL, worker_thread, pool);
    }
}

void add_task(struct threads_pool *pool, task_t task)
{
    pthread_mutex_lock(&pool->lock);
    enqueue_task(&pool->task_queue, task);
    pthread_cond_signal(&pool->not_empty);
    pthread_mutex_unlock(&pool->lock);
}

void wait_shutdown_threads_pool(struct threads_pool *pool)
{
    pthread_mutex_lock(&pool->lock);
    pool->shutdown = 1;
    pthread_cond_broadcast(&pool->not_empty); // Wake up all waiting threads
    pthread_mutex_unlock(&pool->lock);

    for (int i = 0; i < pool->num_threads; i++)
    {
        pthread_join(pool->threads[i], NULL);
    }

    free(pool->threads);
    destroy_task_queue(&pool->task_queue);
    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->not_empty);
}
