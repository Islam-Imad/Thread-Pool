#include "thread_pool.h"
#include <stdio.h>
#include <unistd.h> // for sleep()

void task1(void *arg)
{
    int *arr = (int *)arg;
    printf("Task 1 is processing the following data:\n");
    for (int i = 0; i < 1; ++i)
    {
        printf("%d\n", arr[i]);
    }
}

void task2(void *arg)
{
    int *arr = (int *)arg;
    printf("Task 2 is processing the following data:\n");
    for (int i = 0; i < 2; ++i)
    {
        printf("%d\n", arr[i]);
    }
}

void task3(void *arg)
{
    int *arr = (int *)arg;
    printf("Task 3 is processing the following data:\n");
    for (int i = 0; i < 3; ++i)
    {
        printf("%d\n", arr[i]);
    }
}

void task4(void *arg)
{
    int *arr = (int *)arg;
    printf("Task 4 is processing the following data:\n");
    for (int i = 0; i < 4; ++i)
    {
        printf("%d\n", arr[i]);
    }
}

void task5(void *arg)
{
    int *arr = (int *)arg;
    printf("Task 5 is processing the following data:\n");
    for (int i = 0; i < 5; ++i)
    {
        printf("%d\n", arr[i]);
    }
}

int main(int argc, char *argv[])
{
    int data1[] = {1};
    int data2[] = {2, 3};
    int data3[] = {4, 5, 6};
    int data4[] = {7, 8, 9, 10};
    int data5[] = {11, 12, 13, 14, 15};

    printf("Hello thread pool\n");

    struct threads_pool pool;
    init_threads_pool(&pool, 4);

    add_task(&pool, (task_t){.function = task1, .argument = data1});
    add_task(&pool, (task_t){.function = task2, .argument = data2});
    add_task(&pool, (task_t){.function = task3, .argument = data3});
    add_task(&pool, (task_t){.function = task4, .argument = data4});
    add_task(&pool, (task_t){.function = task5, .argument = data5});

    wait_shutdown_threads_pool(&pool);

    return 0;
}
