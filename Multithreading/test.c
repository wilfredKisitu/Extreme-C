#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// thread logic
void *thread_body(void *arg)
{
    printf("Hello from first thread\n");
}

// Second thread
void *thread_body_2(void *arg)
{
    printf("Hello from second thread\n");
}

int main(int argc, char const *argv[])
{
    pthread_t thread;
    int result = pthread_create(&thread, NULL, thread_body, NULL);
    if (result)
    {
        printf("Could not create thread. Eror no: %d\n", result);
        exit(1);
    }

    result = pthread_join(thread, NULL);
    if (result)
    {
        printf("Thread could not be joined. Error no: %d\n", result);
        exit(2);
    }

    pthread_t thread_2;
    int res_2 = pthread_create(&thread_2, NULL, thread_body_2, NULL);
    if (res_2)
    {
        printf("Failed to create thread two\n");
        exit(3);
    }

    res_2 = pthread_join(thread_2, NULL);
    if (res_2)
    {
        printf("Joining the second thread failed\n");
        exit(4);
    }

    return 0;
}
