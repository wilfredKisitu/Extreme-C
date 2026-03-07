#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>

int main(int argc, char const *argv[])
{
	char message[20] = "Hello world\n";
	syscall(__NR_write, 1, message, 13);
	return 0;
}
