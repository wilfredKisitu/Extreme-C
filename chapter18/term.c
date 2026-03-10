#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>
#include <signal.h>

#define TRUE 1
#define FALSE 0
typedef int bool_t;

// shared memory identifiers
#define MUTEX_SH_NAME "/mutex0"
#define SHM_NAME "/shm0"

// Cancel flag fd and owner flag
int cancel_flag_shm_fd = -1;
bool_t cancel_flag_shm_owner = FALSE;

// Mutex to synch access to cancel flag
int mutex_shm_fd = -1;
pthread_mutex_t *mutex = NULL;

// Mutex owner and cancel flag init and declaration
bool_t mutex_owner = FALSE;
bool_t *cancel_flag = NULL;

void init_shared_resource()
{
    // Open cancel flag shared memory
    cancel_flag_shm_fd = shm_open(SHM_NAME, O_RDWR, 0600);
    if (cancel_flag_shm_fd >= 0)
    {
        cancel_flag_shm_owner = FALSE;
        fprintf(stdout, "The shared memory object is opened\n");
    }
    else if (errno == ENOENT)
    {

        fprintf(stderr, "WARN: The sared memory object doesn't exit. \n");
        fprintf(stdout, "Creating the shared memory object ...\n");

        // Create cancel flag shared memory
        cancel_flag_shm_fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
        if (cancel_flag_shm_fd >= 0)
        {
            cancel_flag_shm_owner = TRUE;
            fprintf(stdout, "The shared memory object is created.\n");
        }
        else
        {
            fprintf(stderr, "ERROR: Failed to create shared memory: %s\n", strerror(errno));
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "ERROR: Failed to open shared memory: %s\n", strerror(errno));
        exit(1);
    }

    if (cancel_flag_shm_owner)
    {
        // Allocate and truncate memory
        if (ftruncate(cancel_flag_shm_fd, sizeof(bool_t)) < 0)
        {
            fprintf(stderr, "ERROR: Truncation failed: %s\n", strerror(errno));
            exit(1);
        }
        fprintf(stdout, "The memory region is truncated.\n");
    }
    // Map shared_mem and initialize cancel flag
    void *map = mmap(0, sizeof(bool_t), PROT_WRITE, MAP_SHARED, cancel_flag_shm_fd, 0);
    if (map == MAP_FAILED)
    {
        fprintf(stderr, "ERROR: Mapping failed: %s\n", strerror(errno));
        exit(1);
    }

    cancel_flag = (bool_t *)map;

    // update status of owner process
    if (cancel_flag_shm_owner)
    {
        *cancel_flag = FALSE;
    }
}

void shutdown_shared_resource()
{
    // Unmappng memory
    if (munmap(cancel_flag, sizeof(bool_t)) < 0)
    {
        fprintf(stderr, "ERROR: Unmapping failed: %s\n", strerror(errno));
        exit(1);
    }

    // Closing file descriptor
    if (close(cancel_flag_shm_fd) < 0)
    {
        fprintf(stderr, "ERROR: Closing shared memory fd failed: %s\n", strerror(errno));
        exit(1);
    }

    // Unlinking shared memory object
    if (cancel_flag_shm_owner)
    {
        sleep(1);
        if (shm_unlink(SHM_NAME) < 0)
        {
            fprintf(stderr, "ERROR: Unlinking the shared memory failed: %s\n", strerror(errno));
            exit(1);
        }
    }
}

void init_control_mechanism()
{
    // open mutex
    mutex_shm_fd = shm_open(MUTEX_SH_NAME, O_RDWR, 0600);
    if (mutex_shm_fd >= 0)
    {
        mutex_owner = FALSE;
        fprintf(stdout, "The mutex's shared object is opened\n");
    }

    // Create mutex if it doesnot exist
    else if (errno == ENOENT)
    {
        fprintf(stderr, "WARN: Mutex's shared memory does not exit\n");
        fprintf(stdout, "Creating mutex's shared object ...\n");

        // create mutex and set mutex owner
        mutex_shm_fd = shm_open(MUTEX_SH_NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
        if (mutex_shm_fd >= 0)
        {
            mutex_owner = TRUE;
            fprintf(stdout, "The mutex's shared object is created\n");
        }
        else
        {
            fprintf(stderr, "ERROR: Failed to create mutex's shared memory: %s\n", strerror(errno));
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "ERROR: Failed to open mutex's shared memory: %s\n", strerror(errno));
        exit(1);
    }

    if (mutex_owner)
    {

        // Allocate and truncate the mutex's shared memory
        if (ftruncate(mutex_shm_fd, sizeof(pthread_mutex_t)) < 0)
        {
            fprintf(stderr, "ERROR: Truncation of mutex failed\n");
            exit(1);
        }
    }

    // Map the mutex's shared  memory
    void *map = mmap(0, sizeof(pthread_mutex_t), PROT_WRITE, MAP_SHARED, mutex_shm_fd, 0);
    if (map == MAP_FAILED)
    {
        fprintf(stderr, "ERROR: Mapping failed: %s\n", strerror(errno));
        exit(1);
    }
    mutex = (pthread_mutex_t *)map;

    // Initialize mutex if mutex owner
    if (mutex_owner)
    {
        int ret = -1;

        // Intialize mutex attribute
        pthread_mutexattr_t attr;
        if ((ret = pthread_mutexattr_init(&attr)))
        {
            fprintf(stderr, "ERROR: Initializing mutex attributes failed: %s\n", strerror(ret));
            exit(1);
        }

        // set properties for mutex to be shareable among processes
        if ((ret = pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED)))
        {
            fprintf(stderr, "ERROR: Setting mutex attribute failed: %s\n", strerror(ret));
            exit(1);
        }

        // Initialize mutex
        if ((ret = pthread_mutex_init(mutex, &attr)))
        {
            fprintf(stderr, "ERROR: Initializing the mutex failed: %s\n", strerror(errno));
            exit(1);
        }

        // Destroy attribute after mutex initialization
        if ((ret = pthread_mutexattr_destroy(&attr)))
        {
            fprintf(stderr, "ERROR: Destrunction of mutex attributes failed: %s\n", strerror(ret));
            exit(1);
        }
    }
}

void shutdown_control_mechanism()
{
    sleep(1);
    if (mutex_owner)
    {
        int ret = -1;
        if ((ret = pthread_mutex_destroy(mutex)))
        {
            fprintf(stdout, "WARD: Destruction of mutex failed: %s\n", strerror(errno));
        }
    }

    if (munmap(mutex, sizeof(pthread_mutex_t)) < 0)
    {
        fprintf(stderr, "ERROR: Unmapping the mutex failed: %s\n", strerror(errno));
        exit(1);
    }

    if (close(mutex_shm_fd) < 0)
    {
        fprintf(stderr, "ERROR: Closing mutex failed: %s\n", strerror(errno));
        exit(1);
    }

    if (mutex_owner)
    {
        if (shm_unlink(MUTEX_SH_NAME) < 0)
        {
            fprintf(stderr, "ERROR: Unlinking the mutex failed: %s\n", strerror(errno));
            exit(1);
        }
    }
}

// check cancel flag state
bool_t is_cancelled()
{
    pthread_mutex_lock(mutex);
    bool_t temp = *cancel_flag;
    pthread_mutex_unlock(mutex);
    return temp;
}

// set the cancel flag
void cancel()
{
    pthread_mutex_lock(mutex);
    *cancel_flag = TRUE;
    pthread_mutex_unlock(mutex);
}

// Signal handler
void sigint_handler(int signo)
{
    fprintf(stdout, "\nHandling INT signal: %d\n", signo);
    cancel();
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, sigint_handler);

    // Initialize shared resource
    init_shared_resource();

    // Initialize control mechanism
    init_control_mechanism();

    while (!is_cancelled())
    {
        fprintf(stdout, "Working...\n");
        sleep(1);
    }
    fprintf(stdout, "Cancel signal is received. \n");

    shutdown_shared_resource();
    shutdown_control_mechanism();

    return 0;
}
