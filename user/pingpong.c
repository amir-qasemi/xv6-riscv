#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {
    int p[2];

    if(pipe(p) < 0) {
        fprintf(2, "error creating the pipe");
        exit(1);
    }

    char buf[1] = "1";
    if(fork() == 0) {
        if(read(p[0], buf, 1) < 0) {
            fprintf(2, "error in reading from pipe in child\n");
            exit(1);
        }
        fprintf(1, "%d: received ping\n", getpid());
        close(p[0]);

        write(p[1], buf, 1);
        close(p[1]);
    } else {
        write(p[1], buf, 1);
        close(p[1]);
        wait(0);
        if(read(p[0], buf, 1) < 0) {
            fprintf(2, "error in reading from pipe in parent\n");
            exit(1);
        }
        fprintf(1, "%d: received pong\n", getpid());
        close(p[0]);
    }

    exit(0);
}