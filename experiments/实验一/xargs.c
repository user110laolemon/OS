// xargs.c

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int readline(char *new_argv[32], int cur)
{
	char buf[1024];				// store input strings
	int n = 0;					// record the number of char had read
	while (read(0, buf + n, 1)) // read a char from stdin, store in buf[n]
	{
		if (n == 1023) // is too long
		{
			fprintf(2, "argument is too long\n");
			exit(1);
		}
		if (buf[n] == '\n') // finish reading a line, exit cycle
			break;
		n++;
	}

	buf[n] = 0; // put 0 in end, to be a string
	if (n == 0)
	{
		return 0;
	} // no valid input

	int offset = 0;
	while (offset < n)
	{
		new_argv[cur++] = buf + offset;
		while (buf[offset] != ' ' && offset < n)
		{
			offset++;
		}
		while (buf[offset] == ' ' && offset < n)
		{
			buf[offset++] = 0;  //replace ' ' by'0'
		}
	}
	return cur; // the number of parameters
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(2, "Usage: xargs command (arg ...)\n");
		exit(1);
	}

	char *command = malloc(strlen(argv[1]) + 1); // save the first as command
	strcpy(command, argv[1]);

	char *new_argv[MAXARG]; // save others in new_argv
	for (int i = 1; i < argc; i++)
	{
		new_argv[i - 1] = malloc(strlen(argv[i]) + 1);
		strcpy(new_argv[i - 1], argv[i]);
	}

	int cur;
	while ((cur = readline(new_argv, argc - 1)) != 0)
	{ // read standerd input
		new_argv[cur] = 0;
		if (fork() == 0)
		{
			exec(command, new_argv); // child execute command
			fprintf(2, "exec failed\n");
			exit(1);
		}
		wait(0);
	}
	exit(0);
}