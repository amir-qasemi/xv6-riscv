#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int _pipe[2];

    if (pipe(_pipe) < 0)
    {
        fprintf(2, "error creating the pipe");
        exit(1);
    }
    int n;
    int createdNext;
    int _pipe2[2];
    if (fork() == 0)
    {
        close(_pipe[1]);

        int p;

    newProcess:
        createdNext = 0;
        if (read(_pipe[0], &p, sizeof(p)) > 0)
        {
            fprintf(1, "prime %d\n", p);

            while (read(_pipe[0], &n, sizeof(n)) > 0)
            {
                if (n % p != 0)
                {
                    if (createdNext == 0)
                    {
                        createdNext = 1;
                        if (pipe(_pipe2) < 0)
                        {
                            fprintf(2, "error creating the pipe");
                            exit(1);
                        }

                        if (fork() == 0)
                        {
                            close(_pipe[0]);

                            _pipe[0] = _pipe2[0];
                            close(_pipe2[1]);
                            goto newProcess;
                        }
                        else
                        {
                            close(_pipe2[0]);
                        }
                    }
                    write(_pipe2[1], &n, sizeof(n));
                }
            }
            if (createdNext == 1)
            {
                close(_pipe2[1]);
            }
        }
        close(_pipe[0]);
    }
    else
    { // the first generating process
        close(_pipe[0]);
        for (int i = 2; i <= 35; i++)
        {
            write(_pipe[1], &i, sizeof(i));
        }
        close(_pipe[1]);
    }

    wait(0);
    exit(0);
}