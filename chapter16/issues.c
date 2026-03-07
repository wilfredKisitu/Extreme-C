#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define TRUE 1;
#define FALSE 0;
typedef int boot_t;

int *shared_int;
pthread_cond_t cv;
pthread_cond_t print_cv;
pthread_mutex_t mutex;
boot_t DONE;
boot_t PRINT_DONE;

void *t1_body(void *arg)
{
    // Synchronize assignment of local variable
    int local_var = 100;
    pthread_mutex_lock(&mutex);
    shared_int = &local_var;
    DONE = TRUE;
    pthread_cond_signal(&cv);
    pthread_mutex_unlock(&mutex);

    // Wait for printing of value
    pthread_mutex_lock(&mutex);
    while (!PRINT_DONE)
    {
        pthread_cond_wait(&print_cv, &mutex);
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *t2_body(void *arg)
{
    pthread_mutex_lock(&mutex);
    while (!DONE)
    {
        pthread_cond_wait(&cv, &mutex);
    }
    printf("%d\n", *shared_int);
    PRINT_DONE = TRUE;
    pthread_cond_signal(&print_cv);
    pthread_mutex_unlock(&mutex);

    return NULL;
}



int main(int argc, char const *argv[])
{
    // initalization of pointer
    shared_int = NULL;

    pthread_t t1;
    pthread_t t2;

    pthread_create(&t1, NULL, t1_body, NULL);
    pthread_create(&t2, NULL, t2_body, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
