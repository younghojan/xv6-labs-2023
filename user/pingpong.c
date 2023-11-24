#include "kernel/types.h"
#include "user.h"

char buf[1];

int main(int argc, char *argv[])
{
    int p[2][2];
    pipe(p[0]);
    pipe(p[1]);

    if (fork() == 0) // child process
    {
        int n = -1;
        while (n <= 0)
        {
            n = read(p[0][0], buf, sizeof buf);
        }
        printf("%d: received ping\n", getpid());

        write(p[1][1], "c", 1);
        
        exit(0);
    }
    else // parent process
    {
        write(p[0][1], "p", 1);

        int n = -1;
        while (n <= 0)
        {
            n = read(p[1][0], buf, sizeof buf);
        }
        fprintf(1, "%d: received pong\n", getpid());

        exit(0);
    }
}