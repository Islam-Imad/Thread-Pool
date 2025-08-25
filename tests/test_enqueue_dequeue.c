#include "thread_pool.h"
#include <stdio.h>
#include <stdlib.h>

void mark_seen(void *arg) { 
    int *p = (int *)arg; 
    *p = 1; 
}

int main(void)
{
    struct task_queue queue;
    init_task_queue(&queue);

    int seen = 0;

    enqueue_task(&queue, (task_t){.function = mark_seen, .argument = &seen});

    task_t t;
    dequeue_task(&queue, &t);
    if (t.function == NULL) {
        fprintf(stderr, "dequeue returned empty task\n");
        destroy_task_queue(&queue);
        return EXIT_FAILURE;
    }

    t.function(t.argument);
    if (seen != 1) {
        fprintf(stderr, "task did not run\n");
        destroy_task_queue(&queue);
        return EXIT_FAILURE;
    }

    destroy_task_queue(&queue);
    printf("test_enqueue_dequeue passed\n");
    return EXIT_SUCCESS;
}
