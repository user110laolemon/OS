// pingpong.c

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{

	int p2c_fd[2], c2p_fd[2]; // parent & child
	pipe(p2c_fd), pipe(c2p_fd);
	char buf[5]; // save massage from pipe

	int size;
	int pid = fork();

	if (pid == 0)
	{
		// child,read massage from parent
		close(p2c_fd[1]);
		size = read(p2c_fd[0], buf, sizeof(buf));
		printf("%d: received ", getpid());
		write(1, buf, size);

		// write massage to parent
		close(c2p_fd[0]);
		write(c2p_fd[1], "pong\n", 5);
		exit(0);
	}
	else if (pid > 0)
	{
		// parent,write massage to child
		close(p2c_fd[0]);
		write(p2c_fd[1], "ping\n", 5);

		wait(0);
		// read massage from child
		close(c2p_fd[1]);
		size = read(c2p_fd[0], buf, sizeof(buf));
		printf("%d: received ", getpid());
		write(1, buf, size);
	}
	else
	{
		printf("fork error!\n");
	}

	exit(0);
}
