#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#include <limits.h>

#define OXYGEN_COUNT 50
#define HYDROGEN_COUNT 100

pthread_barrier_t water_barrier;
pthread_mutex_t oxygen_mtx;
sem_t *hydrogen_sem;
unsigned int num_of_water_molecules;

void *hydrogen_thread_body(void *arg)
{
    sem_wait(hydrogen_sem);
    pthread_barrier_wait(&water_barrier);
    sem_post(hydrogen_sem);
    return NULL;
}

void *oxygen_thread_body(void *arg)
{
    pthread_mutex_lock(&oxygen_mtx);
    pthread_barrier_wait(&water_barrier);
    num_of_water_molecules++;
    pthread_mutex_unlock(&oxygen_mtx);
    return NULL;
}

int main(int argc, char const *argv[])
{

    num_of_water_molecules = 0;
    pthread_mutex_init(&oxygen_mtx, NULL);

    sem_t local_sem;
    hydrogen_sem = &local_sem;

    sem_init(hydrogen_sem, 0, 2);
    pthread_barrier_init(&water_barrier, NULL, 3);

    pthread_t thread[150];

    // Create oxygen threads
    for (int i = 0; i < OXYGEN_COUNT; i++)
    {
        if (pthread_create(thread + i, NULL, oxygen_thread_body, NULL))
        {
            printf("Couldnot create an oxygen thread.\n");
            exit(1);
        }
    }

    // create hydrogen threads
    for (int i = OXYGEN_COUNT; i < (HYDROGEN_COUNT + OXYGEN_COUNT); i++)
    {
        if (pthread_create(thread + i, NULL, hydrogen_thread_body, NULL))
        {
            printf("Couldn't create a hydrogen thread.\n");
            exit(2);
        }
    }

    // Wait threads to finish
    printf("Waiting for oxygen and hydrogen atoms to react ...\n");
    for (int i = 0; i < HYDROGEN_COUNT + OXYGEN_COUNT; i++)
    {
        if (pthread_join(thread[i], NULL))
        {
            printf("Couldn't join thread.\n");
            exit(3);
        }
    }

    printf("Number of made water molecuels: %d\n",
           num_of_water_molecules);

    // Detroy sem, mutex, & barriers
    sem_destroy(hydrogen_sem);
    pthread_mutex_destroy(&oxygen_mtx);
    pthread_barrier_destroy(&water_barrier);

    return 0;
}
