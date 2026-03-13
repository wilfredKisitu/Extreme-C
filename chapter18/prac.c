#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>

#define TRUE 1
#define FALSE 0
#define SH_MEM "/shm0"
#define MUTEX_SH_MEM "/mutex0"

typedef int bool_t;


// cancelation shm, flag, owner
bool_t cancel_flag_shm_owner = FALSE;
int cancel_flag_fd = -1;
bool_t* cancel_flag = FALSE;


// mutex shm, mutex, owner
int mutex_shm_fd = -1;
pthread_mutex_t* mutex;
bool_t mutex_owner = FALSE;


void init_shared_resource()
{
    cancel_flag_fd = shm_open(SH_MEM, O_RDWR, 0600);

    if(cancel_flag_fd >= 0)
    {
        cancel_flag_shm_owner = FALSE;
        fprintf(stdout, "Shared resource opened\n");
    }

    // Else create shared resource
    else if(errno == ENOENT )
    {
        fprintf(stderr, "WARN: Shared resource not found: %s\n", strerror(errno));
        fprintf(stdout, "Creating shared resource\n");

        cancel_flag_fd = shm_open(SH_MEM, O_CREAT | O_RDWR | O_EXCL, 0600);

        if(cancel_flag_fd >= 0)
        {
            cancel_flag_shm_owner = TRUE;
            fprintf(stdout, "Shared resource created with fd: %d\n", cancel_flag_fd);
        }
        else {
            fprintf(stderr, "ERROR: Failed to create shared resource: %s\n", strerror(errno));
            exit(1);
        }
    }
    else {
        fprintf(stderr, "ERROR: Failed to read or create shared memory: %s\n", strerror(errno));
        exit(1);
    }

    // Truncate memory if owner process
    if(cancel_flag_shm_owner)
    {
        if(ftruncate(cancel_flag_fd, sizeof(bool_t)) < 0)
        {
            fprintf(stderr, "ERROR: Failed to truncate shared memory: %s\n", strerror(errno));
            exit(1);
        }
    }

    // Map memory to virtual address
    void* map = mmap(0, sizeof(bool_t), PROT_WRITE, MAP_SHARED, cancel_flag_fd, 0);
    if(map == MAP_FAILED)
    {
        fprintf(stderr, "ERROR: Failed to map shared memory: %s\n", strerror(errno));
        exit(1);
    }

    cancel_flag = (bool_t*)map;

    if(cancel_flag_shm_owner)
    {
        *cancel_flag = FALSE;
    }
}

void destroy_shared_resource()
{
    // Unmap memory
    if(munmap(cancel_flag, sizeof(bool_t)) < 0)
    {
        fprintf(stderr, "ERROR: Unmapping shared memory failed: %s\n", strerror(errno));
        exit(1);
    }

    // close fd 
    if(close(cancel_flag_fd) < 0)
    {
        fprintf(stderr, "ERROR: Closing shared resource fd failed: %s\n", strerror(errno));
        exit(1);
    }

    if(cancel_flag_shm_owner)
    {
        sleep(1);
        if(shm_unlink(SH_MEM) < 0)
        {
            fprintf(stderr, "ERROR: Failed to unlink shared memory: %s\n", strerror(errno));
            exit(1);
        }
    }
}

void init_control_mechanism()
{
    // Try opening shared mutex
    mutex_shm_fd = shm_open(MUTEX_SH_MEM, O_RDWR, 0600);
    if(mutex_shm_fd >= 0)
    {
        mutex_owner = FALSE;
        fprintf(stdout, "Opened shared mutex object.\n");
    }

    else if(errno == ENOENT)
    {
        // create shared mutex object
        mutex_shm_fd = shm_open(MUTEX_SH_MEM, O_CREAT | O_EXCL | O_RDWR, 0600);
        if(mutex_shm_fd >= 0)
        {
            mutex_owner = TRUE;
            fprintf(stdout, "Created shared mutex object\n");
        }
        else {
            fprintf(stderr, "ERROR: Failed to create shared mutex object: %s\n", strerror(errno));
            exit(1);
        }
    }
    else 
    {
        fprintf(stderr, "ERROR: Failed to create or read shared mutex object: %s\n", strerror(errno));
        exit(1);
    }

    // Truncate memory if owner
    if(mutex_owner)
    {
        if(ftruncate(mutex_shm_fd, sizeof(pthread_mutex_t)) < 0)
        {
            fprintf(stderr, "ERROR: Failed to truncate mutex object: %s\n", strerror(errno));
            exit(1);
        }
    }

    // Map virtual memory
    void* map = mmap(0, sizeof(pthread_mutex_t), PROT_WRITE, MAP_SHARED, mutex_shm_fd, 0);
    if(map == MAP_FAILED)
    {
        fprintf(stderr, "ERROR: Failed to map mutex object: %s\n", strerror(errno));
        exit(1);
    }

    mutex = (pthread_mutex_t*)map;

    // initialize mutex if owner
    if(mutex_owner)
    {
        int ret = -1;
        pthread_mutexattr_t attr;
        
        if((ret = pthread_mutexattr_init(&attr)))
        {
            fprintf(stderr, "ERROR: Failed to init mutex attr: %s\n", strerror(ret));
            exit(1);
        }

        if((ret = pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED)))
        {
            fprintf(stderr, "ERROR: Failed to init shared attr property: %s\n", strerror(ret));
            exit(1);
        }

        if((ret = pthread_mutex_init(mutex, &attr)))
        {
            fprintf(stderr, "ERROR: Failed to init shared mutex: %s\n", strerror(ret));
            exit(1);
        }

        // Destroy mutex attribute
        if((ret = pthread_mutexattr_destroy(&attr)))
        {
            fprintf(stderr, "WARN: Failed to destroy mutex attribute: %s\n", strerror(ret));
        }
    }
}

void destroy_control_mechanism()
{
    // Destroy mutex
    int ret = -1;
    if((ret = pthread_mutex_destroy(mutex)))
    {
        fprintf(stderr, "ERROR: Failed to destroy mutex: %s\n", strerror(ret));
        exit(1);
    }

    // umap memory
    if(munmap(mutex, sizeof(pthread_mutex_t)) < 0)
    {
        fprintf(stderr, "ERROR: Failed to unmap shared mutex object: %s\n", strerror(errno));
        exit(1);
    }

    // Close fd
    if(close(mutex_shm_fd) < 0)
    {
        fprintf(stderr, "ERROR: Failed to close mutex object fd: %s\n",strerror(errno));
        exit(1);
    }

    // unlink if owner
    if(mutex_owner)
    {
        sleep(1);
        if(shm_unlink(MUTEX_SH_MEM) < 0)
        {
            fprintf(stderr, "ERROR: Failed to unlink shared mutex object: %s\n", strerror(errno));
            exit(1);
        }
    }
}

void cancel()
{
    sleep(1);
    pthread_mutex_lock(mutex);
    *cancel_flag = TRUE;
    pthread_mutex_unlock(mutex);
}

bool_t is_canceled()
{
    return *cancel_flag;
}

void sigint_handler(int signo)
{
    fprintf(stdout, "Hanlding signo: %d\n", signo);
    cancel();
}


int main(int argc, char const *argv[])
{
    // CTRL+C handler
    signal(SIGINT, sigint_handler);
    
    // Initialize shared resource
    init_shared_resource();

    // Initialize control mechanism
    init_control_mechanism();

    while (!is_canceled())
    {
        fprintf(stdout, "Working....\n");
        sleep(1);
    }

    fprintf(stdout,  "Signal receive\n");

    // destroy shared resource
    destroy_shared_resource();

    // destroy control mechansim
    destroy_control_mechanism();

    return 0;
}

// SOCKET PROGRAMMING
// TARGET TO SEE HOW PROGRAMS COMMUNITCATE 
// EXAMPLE OF SHARED VARIABLES
