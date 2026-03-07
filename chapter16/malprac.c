#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>

void *thread_body_1(void *arg)
{
    int local_var = 0;
    printf("Thread1 Address: %p\n", (void *)&local_var);
    return NULL;
}

void *thread_body_2(void *arg)
{
    int local_var = 0;
    printf("Thread2 Addrees: %p\n", (void *)&local_var);
    return NULL;
}

int main(int argc, char const *argv[])
{
    size_t buffer_len = PTHREAD_STACK_MIN + 100;
    char *buffer = (char *)malloc(buffer_len * sizeof(char));

    // create pthread
    pthread_t thread1;
    pthread_t thread2;

    // create thread1
    int result1 = pthread_create(&thread1, NULL, thread_body_1, NULL);

    // create thread2
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    if (pthread_attr_setstack(&attr, buffer, buffer_len))
    {
        printf("Failed to create attribute\n");
        exit(1);
    }

    int result2 = pthread_create(&thread2, &attr, thread_body_2, NULL);
    if (result1 || result2)
    {
        printf("Could not create threads\n");
        exit(2);
    }

    // wait threads to finish
    result1 = pthread_join(thread1, NULL);
    result2 = pthread_join(thread2, NULL);

    if (result1 || result2)
    {
        printf("Failed to join threads\n");
        exit(3);
    }

    printf("Main Thread > Stack Address: %p\n", (void *)&buffer_len);
    printf("Main Thread > Heap Address: %p\n", (void *)buffer);

    free(buffer);

    return 0;
}
