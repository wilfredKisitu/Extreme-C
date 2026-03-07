#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0

typedef unsigned int bool_t;

typedef struct
{
    bool_t done;
    pthread_cond_t cv;
    pthread_mutex_t mtx;

} shared_state_t;

void shared_state_init(shared_state_t *shared_state)
{
    shared_state->done = FALSE;
    pthread_cond_init(&shared_state->cv, NULL);
    pthread_mutex_init(&shared_state->mtx, NULL);
}

void shared_state_destroy(shared_state_t *shared_state)
{
    shared_state->done = FALSE;
    pthread_cond_destroy(&shared_state->cv);
    pthread_mutex_destroy(&shared_state->mtx);
}

void *thread_body_1(void *arg)
{
    shared_state_t *ss = (shared_state_t *)arg;
    pthread_mutex_lock(&ss->mtx);
    ss->done = TRUE;
    printf("A\n");
    pthread_cond_signal(&ss->cv);
    pthread_mutex_unlock(&ss->mtx);

    return NULL;
}

void *thread_body_2(void *arg)
{
    shared_state_t *ss = (shared_state_t *)arg;
    pthread_mutex_lock(&ss->mtx);
    while (!ss->done)
    {
        pthread_cond_wait(&ss->cv, &ss->mtx);
    }
    printf("B\n");
    pthread_mutex_unlock(&ss->mtx);

    return NULL;
}

int main(int argc, char const *argv[])
{

    shared_state_t shared_state;
    shared_state_init(&shared_state);

    pthread_t thread1;
    pthread_t thread2;

    int result1 = pthread_create(&thread1, NULL, thread_body_1, &shared_state);
    int result2 = pthread_create(&thread2, NULL, thread_body_2, &shared_state);

    if (result1 || result2)
    {
        printf("Threads counld not be created\n");
        exit(1);
    }

    // Wait for threads to finish
    result1 = pthread_join(thread1, NULL);
    result2 = pthread_join(thread2, NULL);

    if (result1 || result2)
    {
        printf("Threads couldnot be joined\n");
        exit(2);
    }

    shared_state_destroy(&shared_state);

    return 0;
}
