#include "thread_pool.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *counter;
    int inc;
} counter_arg_t;

void inc_task(void *arg)
{
    counter_arg_t *a = (counter_arg_t *)arg;
    *(a->counter) += a->inc;
}

int main(void)
{
    struct task_queue queue;
    init_task_queue(&queue);

    int c1 = 0, c2 = 0, c3 = 0;
    counter_arg_t a1 = {&c1, 1};
    counter_arg_t a2 = {&c2, 2};
    counter_arg_t a3 = {&c3, 3};

    enqueue_task(&queue, (task_t){.function = inc_task, .argument = &a1});
    enqueue_task(&queue, (task_t){.function = inc_task, .argument = &a2});
    enqueue_task(&queue, (task_t){.function = inc_task, .argument = &a3});

    execute_tasks(&queue);

    int ok = 1;
    if (c1 != 1) ok = 0;
    if (c2 != 2) ok = 0;
    if (c3 != 3) ok = 0;

    destroy_task_queue(&queue);

    if (!ok) {
        fprintf(stderr, "test failed: counters = %d, %d, %d\n", c1, c2, c3);
        return EXIT_FAILURE;
    }

    printf("test passed: counters = %d, %d, %d\n", c1, c2, c3);
    return EXIT_SUCCESS;
}
