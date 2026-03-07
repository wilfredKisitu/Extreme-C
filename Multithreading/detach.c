#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Thread logic running as detached thread
void *thread_body(void *arg)
{
    printf("Hello from detachable thread\n");
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t thread;

    // Create a new thread
    int result = pthread_create(&thread, NULL, thread_body, NULL);
    if (result)
    {
        printf("Thread could not be created, Error number: %d\n", result);
        exit(1);
    }

    // Detach the thread
    result = pthread_detach(thread);

    if (result)
    {
        printf("Thread could not be detached. Error number:%d\n", result);
        exit(2);
    }
    // Exit the main thread
    pthread_exit(NULL);

    return 0;
}
