// sleep.c

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{

	if (argc != 2)  //output wrong message
	{
		write(2, "Usage: sleep time\n", strlen("Usage: sleep time\n"));
		exit(1);
	}

	int time = atoi(argv[1]);  //get int time
	sleep(time);
	exit(0);
}
