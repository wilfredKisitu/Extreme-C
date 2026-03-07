#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define TRUE 1
#define FALSE 0

typedef int bool_t;

typedef struct
{
    int *shared_int;
    pthread_mutex_t mutex;
    pthread_cond_t cv_shared;
    pthread_cond_t cv_print;
    bool_t DONE_INIT;
    bool_t DONE_PRINT;
} shared_t;

void shared_init(shared_t *shared)
{
    shared->shared_int = NULL;

    pthread_mutex_init(&shared->mutex, NULL);
    pthread_cond_init(&shared->cv_shared, NULL);
    pthread_cond_init(&shared->cv_print, NULL);

    shared->DONE_INIT = FALSE;
    shared->DONE_PRINT = FALSE;
}

void shared_destroy(shared_t *shared)
{
    shared->shared_int = NULL;
    pthread_mutex_destroy(&shared->mutex);
    pthread_cond_destroy(&shared->cv_shared);
    pthread_cond_destroy(&shared->cv_print);

    shared->DONE_INIT = FALSE;
    shared->DONE_PRINT = FALSE;
}

void *t1_body(void *arg)
{
    shared_t *st = (shared_t *)arg;
    int local_var = 100;

    pthread_mutex_lock(&st->mutex);

    st->shared_int = &local_var;
    st->DONE_INIT = TRUE;

    pthread_cond_signal(&st->cv_shared);

    while (!st->DONE_PRINT)
    {
        pthread_cond_wait(&st->cv_print, &st->mutex);
    }

    pthread_mutex_unlock(&st->mutex);

    return NULL;
}

void *t2_body(void *arg)
{
    shared_t *st = (shared_t *)arg;

    pthread_mutex_lock(&st->mutex);
    while (!st->DONE_INIT)
    {
        pthread_cond_wait(&st->cv_shared, &st->mutex);
    }

    printf("%d\n", *st->shared_int);
    st->DONE_PRINT = TRUE;

    pthread_cond_signal(&st->cv_print);
    pthread_mutex_unlock(&st->mutex);

    return NULL;
}

int main(int argc, char const *argv[])
{
    shared_t shared_mem;
    shared_init(&shared_mem);
    pthread_t t1, t2;

    int res1 = pthread_create(&t1, NULL, t1_body, &shared_mem);
    int res2 = pthread_create(&t2, NULL, t2_body, &shared_mem);

    if (res1 || res2)
    {
        printf("Failed to crreate threads\n");
        exit(1);
    }

    res1 = pthread_join(t1, NULL);
    res2 = pthread_join(t2, NULL);

    if (res1 || res2)
    {
        printf("Failed to join thread\n");
        exit(2);
    }

    shared_destroy(&shared_mem);

    return 0;
}
