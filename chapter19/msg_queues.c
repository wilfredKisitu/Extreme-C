#include <mqueue.h>
#include <stdlib.h>
#include <mqueue.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char const *argv[])
{

    // Message queue handler
    mqd_t mq;

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 32;
    attr.mq_curmsgs = 0;

    int childpid = fork();

    if (childpid == -1)
    {
        fprintf(stderr, "fork error\n");
        exit(1);
    }
    if (childpid == 0)
    {
        // child process
        sleep(1);
        mqd_t mq = mq_open("/mq0", O_WRONLY);
        char str[] = "Hello Daddy...\n";

        fprintf(stdout, "CHILD: Waiting for 2 seconds..\n");
        fprintf(stdout, "CHILD: Writing to Daddy...\n");
        mq_send(mq, str, strlen(str) + 1, 0);
        mq_close(mq);
    }

    else
    {
        // Parent process
        mqd_t mq = mq_open("/mq0", O_RDONLY | O_CREAT, 0664, &attr);
        char buff[32];
        fprintf(stdout, "PARENT: Reading from child...\n");
        int num_of_bytes = mq_receive(mq, buff, 32, NULL);
        fprintf(stdout, "PARENT: Received from child: %s\n", buff);
        mq_close(mq);
        mq_unlink("/mq0");
    }

    return 0;
}
