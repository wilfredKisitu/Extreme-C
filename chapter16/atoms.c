#define _POSIX_C_SOURCE 200112L
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

#define TOT_ATOMS 150
#define O_ATOMS 50

// Declaration of synchronizers
pthread_barrier_t water_barrier;
sem_t *hydrogen_sem;
pthread_mutex_t oxygen_mutex;
unsigned int num_of_molecules;

// Oxgen threads waits for single atom
void *oxygen_thread_body(void *arg)
{
    pthread_mutex_lock(&oxygen_mutex);
    pthread_barrier_wait(&water_barrier);
    num_of_molecules++;
    pthread_mutex_unlock(&oxygen_mutex);

    return NULL;
}

// Hydrogen threads waits for two atoms
void *hydrogen_thread_body(void *arg)
{
    sem_wait(hydrogen_sem);
    pthread_barrier_wait(&water_barrier);
    sem_post(hydrogen_sem);

    return NULL;
}

int main(int argc, char const *argv[])
{

    // Thread synchronization Initialization
    num_of_molecules = 0;
    pthread_mutex_init(&oxygen_mutex, NULL);
    pthread_barrier_init(&water_barrier, NULL, 3);

    sem_t local_sem;
    hydrogen_sem = &local_sem;
    sem_init(hydrogen_sem, 0, 2);

    // Oxygen atoms initialization
    pthread_t thread[TOT_ATOMS];

    for (int i = 0; i < O_ATOMS; i++)
    {
        if (pthread_create(thread + i, NULL, oxygen_thread_body, NULL))
        {
            printf("Could not create oxygen atom\n");
            exit(1);
        }
    }

    // Hydrogen atoms initialization
    for (int i = O_ATOMS; i < TOT_ATOMS; i++)
    {
        if (pthread_create(thread + i, NULL, hydrogen_thread_body, NULL))
        {
            printf("Could not create hydrogen atom\n");
            exit(2);
        }
    }

    // Wait for thread completion
    for (int i = 0; i < TOT_ATOMS; i++)
    {
        if (pthread_join(thread[i], NULL))
        {
            printf("The thread could not be joined\n");
            exit(3);
        }
    }

    printf("Total number of created water molecules: %d\n",
           num_of_molecules);

    // Freeing memory
    pthread_mutex_destroy(&oxygen_mutex);
    pthread_barrier_destroy(&water_barrier);
    sem_destroy(hydrogen_sem);

    return 0;
}
