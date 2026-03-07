#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define TOT_ATOMS 150
#define O_ATOMS 50
#define H_ATOMS 100

pthread_barrier_t water_barrier;
pthread_mutex_t oxygen_mutex;
sem_t *hydrogen_sem;
unsigned int num_of_molecules;

void *hydrogen_thread_body(void *arg)
{
    sem_wait(hydrogen_sem);
    pthread_barrier_wait(&water_barrier);
    sem_post(hydrogen_sem);

    return NULL;
}

void *oxygen_thread_body(void *arg)
{
    pthread_mutex_lock(&oxygen_mutex);
    pthread_barrier_wait(&water_barrier);
    num_of_molecules++;
    pthread_mutex_unlock(&oxygen_mutex);

    return NULL;
}

int main(int argc, char const *argv[])
{
    num_of_molecules = 0;
    pthread_barrier_init(&water_barrier, NULL, 3);
    pthread_mutex_init(&oxygen_mutex, NULL);

    sem_t local_sem;
    hydrogen_sem = &local_sem;
    sem_init(hydrogen_sem, 0, 2);

    pthread_t thread[TOT_ATOMS];

    /* Create Oxygen Atoms */
    for (int i = 0; i < O_ATOMS; i++)
    {
        if (pthread_create(thread + i, NULL, oxygen_thread_body, NULL))
        {
            printf("Couldnot create oxygen thread\n");
            exit(1);
        }
    }

    /* Create Hydrogen atoms */
    for (int i = O_ATOMS; i < TOT_ATOMS; i++)
    {
        if (pthread_create(thread + i, NULL, hydrogen_thread_body, NULL))
        {
            printf("Could not create hydrogen thread\n");
            exit(2);
        }
    }

    printf("Waiting for oxygen and hydrogen atoms to react...\n");

    for (int i = 0; i < TOT_ATOMS; i++)
    {
        if (pthread_join(thread[i], NULL))
        {
            printf("Could not join thread\n");
            exit(3);
        }
    }

    printf("Number of water molecules: %d\n", num_of_molecules);

    sem_destroy(hydrogen_sem);
    pthread_barrier_destroy(&water_barrier);
    pthread_mutex_destroy(&oxygen_mutex);

    return 0;
}
