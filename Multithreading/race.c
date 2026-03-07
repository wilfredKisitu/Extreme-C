#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_body(void *arg)
{
    char *str = (char *)arg;
    printf("%s\n", str);
    return NULL;
}

// Voilation of invariant contraint leading to race conditions

int main(int argc, char const *argv[])
{

    // The thrad handlers
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;

    //  Create new thread
    int result1 = pthread_create(&thread1, NULL, thread_body, "Apple");
    int result2 = pthread_create(&thread2, NULL, thread_body, "Orange");
    int result3 = pthread_create(&thread3, NULL, thread_body, "Lemon");

    if (result1 || result2 || result3)
    {
        printf("The threads could no tbe created.\n");
        exit(1);
    }

    // Wait for the threads to finish
    result1 = pthread_join(thread1, NULL);
    result2 = pthread_join(thread2, NULL);
    result3 = pthread_join(thread3, NULL);

    if (result1 || result2 || result3)
    {
        printf("The threads could not be joined \n");
        exit(2);
    }

    return 0;
}
