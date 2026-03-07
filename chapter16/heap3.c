#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define CHECK_RESULT(result)                     \
    {                                            \
        if (result)                              \
        {                                        \
            printf("A thread error happened\n"); \
            exit(1);                             \
        }                                        \
    }

int *shared_int;
pthread_barrier_t init_barrier;
pthread_barrier_t done_barrier;

void *alloc_thread_body(void *arg)
{
    int local_var = 400;
    shared_int = &local_var;

    pthread_barrier_wait(&init_barrier);
    pthread_barrier_wait(&done_barrier);

    return NULL;
}

void *print_thread_body(void *arg)
{
    pthread_barrier_wait(&init_barrier);
    printf("%d\n", *shared_int);
    pthread_barrier_wait(&done_barrier);

    return NULL;
}

int main(int argc, char const *argv[])
{
    shared_int = NULL;

    pthread_barrier_init(&init_barrier, NULL, 2);
    pthread_barrier_init(&done_barrier, NULL, 2);

    pthread_t init_thread;
    pthread_t done_thread;

    int res = pthread_create(&init_thread, NULL, alloc_thread_body, NULL);
    CHECK_RESULT(res);

    res = pthread_create(&done_thread, NULL, print_thread_body, NULL);
    CHECK_RESULT(res);

    res = pthread_join(init_thread, NULL);
    CHECK_RESULT(res);

    res = pthread_join(done_thread, NULL);
    CHECK_RESULT(res);

    // Destroy
    pthread_barrier_destroy(&init_barrier);
    pthread_barrier_destroy(&done_barrier);

    return 0;
}
