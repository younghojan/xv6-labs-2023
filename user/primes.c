#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes_filter(int left_pipe_r_end)
{
    int right_pipe[2], primes[35], count = 0, prime, i = 0;
    char num, buf[1];

    // Read numbers from left_pipe_r_end
    for (; read(left_pipe_r_end, buf, sizeof(buf)) != 0; ++count)
    {
        primes[count] = buf[0];
    }
    close(left_pipe_r_end);
    if (count == 0)
    {
        return;
    }

    // Output
    prime = primes[0];
    printf("prime %d\n", prime);

    // Filter
    pipe(right_pipe);
    for (; i < count; ++i)
    {
        if (primes[i] % prime != 0)
        {
            num = primes[i];
            write(right_pipe[1], &num, 1);
        }
    }
    close(right_pipe[1]);

    if (fork() == 0)
    {
        primes_filter(right_pipe[0]);
    }
}

int main(int argc, char *argv[])
{
    int first_pipe[2], i = 2;
    char num;

    pipe(first_pipe);

    for (; i <= 35; i++)
    {
        num = i;
        write(first_pipe[1], &num, 1);
    }
    close(first_pipe[1]);

    primes_filter(first_pipe[0]);

    wait((int *)0);

    return 0;
}