#include "thread_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

volatile int shared_counter = 0;
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

void cpu_intensive_task(void *arg)
{
    int *iterations = (int *)arg;
    
    // Simulate CPU-intensive work
    int sum = 0;
    for (int i = 0; i < *iterations; i++) {
        sum += i * i;
    }
    
    // Thread-safe counter increment
    pthread_mutex_lock(&counter_mutex);
    shared_counter++;
    pthread_mutex_unlock(&counter_mutex);
}

double get_time_diff(struct timeval start, struct timeval end)
{
    return (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
}

int main(void)
{
    const int NUM_TASKS = 100;
    const int NUM_THREADS = 4;
    const int WORK_SIZE = 100000;
    
    struct timeval start, end;
    
    printf("=== Thread Pool Performance Benchmark ===\n");
    printf("Tasks: %d, Threads: %d, Work per task: %d iterations\n\n", 
           NUM_TASKS, NUM_THREADS, WORK_SIZE);
    
    // Test 1: Thread pool
    gettimeofday(&start, NULL);
    
    struct threads_pool pool;
    init_threads_pool(&pool, NUM_THREADS);
    
    int work_data[NUM_TASKS];
    for (int i = 0; i < NUM_TASKS; i++) {
        work_data[i] = WORK_SIZE;
        add_task(&pool, (task_t){.function = cpu_intensive_task, .argument = &work_data[i]});
    }
    
    wait_shutdown_threads_pool(&pool);
    
    gettimeofday(&end, NULL);
    double thread_pool_time = get_time_diff(start, end);
    
    printf("Thread Pool Results:\n");
    printf("  Time: %.3f seconds\n", thread_pool_time);
    printf("  Tasks completed: %d\n", shared_counter);
    printf("  Throughput: %.1f tasks/second\n\n", NUM_TASKS / thread_pool_time);
    
    // Test 2: Sequential execution for comparison
    shared_counter = 0;
    gettimeofday(&start, NULL);
    
    for (int i = 0; i < NUM_TASKS; i++) {
        cpu_intensive_task(&work_data[i]);
    }
    
    gettimeofday(&end, NULL);
    double sequential_time = get_time_diff(start, end);
    
    printf("Sequential Results:\n");
    printf("  Time: %.3f seconds\n", sequential_time);
    printf("  Tasks completed: %d\n", shared_counter);
    printf("  Throughput: %.1f tasks/second\n\n", NUM_TASKS / sequential_time);
    
    printf("Performance Analysis:\n");
    printf("  Speedup: %.2fx\n", sequential_time / thread_pool_time);
    printf("  Efficiency: %.1f%%\n", (sequential_time / thread_pool_time) / NUM_THREADS * 100);
    
    pthread_mutex_destroy(&counter_mutex);
    
    return 0;
}
