#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_body_1(void *arg)
{
    int *shared_var_ptr = (int *)arg;
    (*shared_var_ptr)++;
    printf("%d\n", *shared_var_ptr);
    return NULL;
}

void *thread_body_2(void *arg)
{
    int *shared_var_ptr = (int *)arg;
    *shared_var_ptr += 2;
    printf("%d\n", *shared_var_ptr);
    return NULL;
}

int main(int argc, char const *argv[])
{
    int shared_var = 0;

    pthread_t thread1;
    pthread_t thread2;

    int result1 = pthread_create(&thread1, NULL, thread_body_1, &shared_var);
    int result2 = pthread_create(&thread2, NULL, thread_body_2, &shared_var);

    if (result1 || result2)
    {
        printf("Coundnot create threads\n");
        exit(1);
    }

    // Wait threads to finish
    result1 = pthread_join(thread1, NULL);
    result2 = pthread_join(thread2, NULL);

    return 0;
}
