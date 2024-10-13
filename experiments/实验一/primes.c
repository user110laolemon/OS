// primes.c

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void mapping(int n, int pd[])
{
    close(n); // close file descriptor n

    dup(pd[n]); // generate a map fromn to pd[n]

    close(pd[0]); // close descriptor in the pipe
    close(pd[1]);
}

void primes()
{
    int pre, next;
    int fd[2];

    if (read(0, &pre, sizeof(int)))
    {
        printf("prime %d\n", pre); // print the first number directly
        pipe(fd);

        if (fork() == 0)
        {
            mapping(1, fd); // child maps the write port of the pipe to 1
            while (read(0, &next, sizeof(int)))
            {
                if (next % pre != 0)
                { // write who is not a multiple of the first number
                    write(1, &next, sizeof(int));
                }
            }
        }
        else
        {
            wait(0);        // parent waits until child write all numbers
            mapping(0, fd); // parent maps the read port of the pipe to 0

            primes();
        }
    }
}

int main(int argc, char *argv[])
{
    int fd[2];
    pipe(fd);

    if (fork() == 0)
    {
        mapping(1, fd);
        for (int i = 2; i < 36; i++)
        {
            write(1, &i, sizeof(int));
        }
    }
    else
    {
        wait(0);
        mapping(0, fd);
        primes();
    }
    exit(0);
}