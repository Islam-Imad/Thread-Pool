#include "thread_pool.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    struct task_queue queue;
    init_task_queue(&queue);

    /* Should do nothing and not crash */
    execute_tasks(&queue);

    destroy_task_queue(&queue);

    printf("test_empty_queue passed\n");
    return EXIT_SUCCESS;
}
