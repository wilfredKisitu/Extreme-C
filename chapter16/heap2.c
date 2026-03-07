#define _POSIX_C_SOURCE 200112L
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define CHECK_RESULT(result)                 \
    if (result)                              \
    {                                        \
        printf("A pthread error happend\n"); \
        exit(1);                             \
    }

// Unique even barrier codes
int TRUE = 1;
int FASLE = 0;

// Barrier Definitions
pthread_barrier_t alloc_barrier;
pthread_barrier_t filler_barrier;
pthread_barrier_t dealloc_barrier;

// Global Array
char *shared_array;
unsigned int shared_array_len;

// Initalizer thread body
void *alloc_thread_body(void *arg)
{
    shared_array_len = 26;
    shared_array = (char *)malloc(shared_array_len * sizeof(char));
    pthread_barrier_wait(&alloc_barrier);

    return NULL;
}

// Filler thread body
void *filler_thread_body(void *arg)
{
    pthread_barrier_wait(&alloc_barrier);
    int even = *((int *)arg);
    size_t start_index = 1;
    char c = 'a';

    if (even)
    {
        start_index = 0;
        c = 'z';
    }

    for (size_t i = start_index; i < shared_array_len; i += 2)
    {
        shared_array[i] = even ? c-- : c++;
    }
    shared_array[shared_array_len - 1] = '\0';
    pthread_barrier_wait(&filler_barrier);

    return NULL;
}

// Printer thread body
void *printer_thread_body(void *arg)
{
    pthread_barrier_wait(&filler_barrier);
    printf(">> %s\n", shared_array);
    pthread_barrier_wait(&dealloc_barrier);

    return NULL;
}

// Deallocator thread body
void *dealloc_thread_body(void *arg)
{
    pthread_barrier_wait(&dealloc_barrier);
    free(shared_array);
    pthread_barrier_destroy(&alloc_barrier);
    pthread_barrier_destroy(&filler_barrier);
    pthread_barrier_destroy(&dealloc_barrier);

    return NULL;
}

int main(int argc, char const *argv[])
{
    // Intialization shared array
    shared_array = NULL;

    // Thread intialization
    pthread_t alloc_thread;
    pthread_t even_filler_thread;
    pthread_t odd_filler_thread;
    pthread_t printer_thread;
    pthread_t dealloc_thread;

    // Initalize threads
    pthread_barrier_init(&alloc_barrier, NULL, 3);
    pthread_barrier_init(&filler_barrier, NULL, 3);
    pthread_barrier_init(&dealloc_barrier, NULL, 2);

    // Detach Configurations
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int res = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    CHECK_RESULT(res);

    res = pthread_create(&alloc_thread, &attr, alloc_thread_body, NULL);
    CHECK_RESULT(res);

    res = pthread_create(&even_filler_thread, &attr, filler_thread_body, &TRUE);
    CHECK_RESULT(res);

    res = pthread_create(&odd_filler_thread, &attr, filler_thread_body, &FASLE);
    CHECK_RESULT(res);

    res = pthread_create(&printer_thread, &attr, printer_thread_body, NULL);
    CHECK_RESULT(res);

    res = pthread_create(&dealloc_thread, &attr, dealloc_thread_body, NULL);
    CHECK_RESULT(res);

    pthread_exit(NULL);

    return 0;
}
