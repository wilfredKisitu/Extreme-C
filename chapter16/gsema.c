#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

#ifdef __APPLE__
/*  Barrier functionality on OS/X */
pthread_mutex_t barrier_mtx;
pthread_cond_t barrier_cv;
unsigned int barrier_thread_count;
unsigned int barrier_round;
unsigned int barrier_thread_limit;

void barrier_wait()
{
    pthread_mutex_lock(&barrier_mtx);
    barrier_thread_count++;
    if (barrier_thread_count >= barrier_thread_limit)
    {
        barrier_thread_count = 0;
        barrier_round++;
        pthread_cond_broadcast(&barrier_cv);
    }
    else
    {
        unsigned int my_round = my_round;
        do
        {
            pthread_cond_wait(&barrier_cv, &barrier_mtx);
        } while (my_round == barrier_round);
    }
    pthread_mutex_unlock(&barrier_mtx);
}
#else
/*  Barrier to make hydrogen and oxygen threads synchronized */
pthread_barrier_t water_barrier;
#endif

pthread_mutex_t oxygen_mutex;
sem_t *hydrogen_sem; /* Sem to make hydrogen thread synchronized */
unsigned int num_of_water_molecules;

void *hydrogen_thread_body(void *arg)
{
    sem_wait(hydrogen_sem);
// Wait for other hydrogen thred to join
#ifdef __APPLE__
    barrier_wait();
#else
    pthread_barrier_wait(&water_barrier);
#endif
    sem_post(hydrogen_sem);
    return NULL;
}

void *oxygen_thread_body(void *)
{
    pthread_mutex_lock(&oxygen_mutex);
#ifdef __APPLE__
    barrier_wait();
#else
    pthread_barrier_wait(&water_barrier);
#endif
    num_of_water_molecules++;
    pthread_mutex_unlock(&oxygen_mutex);
    return NULL;
}

int main(int argc, char const *argv[])
{
    num_of_water_molecules = 0;

    pthread_mutex_init(&oxygen_mutex, NULL);

#ifdef __APPLE__
    hydrogen_sem = sem_open("hydrogen_sem",
                            O_CREATE | O_EXCL, 0644, 2);
#else
    sem_t local_sem;
    hydrogen_sem = &local_sem;
    sem_init(hydrogen_sem, 0, 2);
#endif

#ifdef __APPLE__
    pthread_mutex_init(&barrier_mutex, NULL);
    pthread_cond_init(&barrier_cv, NULL);
    barrier_thread_count = 0;
    barrier_thread_limit = 0;
    barrier_round = 0;
#else
    pthread_barrier_init(&water_barrier, NULL, 3);
#endif
    pthread_t thread[150];
    // Create oxygen threads
    for (int i = 0; i < 50; i++)
    {
        if (pthread_create(thread + i, NULL, oxygen_thread_body, NULL))
        {
            printf("Couldn't create an oxygen thread. \n");
            exit(1);
        }
    }

    // Create hydrogen atoms
    for (int i = 50; i < 150; i++)
    {
        if (pthread_create(thread + i, NULL, hydrogen_thread_body, NULL))
        {
            printf("Couldn't create a hydrogen atom.\n");
            exit(2);
        }
    }

    printf("Waiting for oxygen and hydrogen atoms to react ...\n");
    for (int i = 0; i < 150; i++)
    {
        if (pthread_join(thread[i], NULL))
        {
            printf("the thread could not be joined. \n");
            exit(3);
        }
    }

    printf("Number of made water molecules: %d\n",
           num_of_water_molecules);

#ifdef __APPLE__
    sem_close(hydrogen_sem);
#else
    sem_destroy(hydrogen_sem);
#endif

    pthread_mutex_destroy(&oxygen_mutex);
    pthread_barrier_destroy(&water_barrier);

    return 0;
}
