#include "thread_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

volatile int counter = 0;
pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER;

void increment_task(void *arg)
{
    int *value = (int *)arg;
    pthread_mutex_lock(&counter_lock);
    counter += *value;
    pthread_mutex_unlock(&counter_lock);
    usleep(100000); // 100ms delay to simulate work
}

int main(void)
{
    struct threads_pool pool;
    init_threads_pool(&pool, 3); // 3 worker threads

    int values[] = {1, 2, 3, 4, 5};

    // Add 5 tasks
    for (int i = 0; i < 5; i++)
    {
        add_task(&pool, (task_t){.function = increment_task, .argument = &values[i]});
    }

    // Wait for tasks to complete
    // sleep(1);

    wait_shutdown_threads_pool(&pool);
    pthread_mutex_destroy(&counter_lock);

    int expected = 1 + 2 + 3 + 4 + 5; // 15
    if (counter != expected)
    {
        fprintf(stderr, "FAIL: Expected %d, got %d\n", expected, counter);
        return EXIT_FAILURE;
    }

    printf("SUCCESS: Multi-threaded execution completed correctly (counter = %d)\n", counter);
    return EXIT_SUCCESS;
}
