#include "thread_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <atomic>

std::atomic<int> counter(0);

void increment_task(void *arg)
{
    int *value = (int *)arg;
    counter += *value;
    usleep(100000); // 100ms delay to simulate work
}

int main(void)
{
    struct threads_pool pool;
    init_threads_pool(&pool, 3); // 3 worker threads
    
    int values[] = {1, 2, 3, 4, 5};
    
    // Add 5 tasks
    for (int i = 0; i < 5; i++) {
        add_task(&pool, (task_t){.function = increment_task, .argument = &values[i]});
    }
    
    // Wait for tasks to complete
    // sleep(1);

    wait_shutdown_threads_pool(&pool);

    int expected = 1 + 2 + 3 + 4 + 5; // 15
    if (counter.load() != expected) {
        fprintf(stderr, "FAIL: Expected %d, got %d\n", expected, counter.load());
        return EXIT_FAILURE;
    }
    
    printf("SUCCESS: Multi-threaded execution completed correctly (counter = %d)\n", counter.load());
    return EXIT_SUCCESS;
}
