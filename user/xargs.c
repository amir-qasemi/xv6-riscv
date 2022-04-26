#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

void executeLine(char *line, int argc, char *argv[])
{
    char *inputarg[MAXARG];

    for (int j = 1; j < argc; j++)
    {
        inputarg[j - 1] = argv[j];
    }
    inputarg[argc - 1] = line;
    if (!fork())
    {
        exec(argv[1], inputarg);
    }
    else
    {
        wait(0);
    }
}
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(2, "fewer arguments than expected\n");
        exit(1);
    }

    
    char buf[512];
    char readChar;
    int i = 0;
    while (read(0, &readChar, sizeof(readChar)) > 0)
    {
        if (readChar == '\n')
        {
            buf[i] = '\0';
            executeLine(buf, argc, argv);
            i = 0;
        }

        else
        {
            buf[i++] = readChar;
        }
    }

    exit(0);
}