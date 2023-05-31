#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    int pid = 0;
    int status = 10;

    char* child_arg[] = {"./data", NULL};

    printf("Start ID: %d\n", getpid());

    pid = fork();

    if (pid == 0)
    {
        printf("I am child. PID = %d\n", getpid());
        execv("./child_process", child_arg);
    }
    else
    {
        wait(&status);
        status >>= 8;
        printf("status = %d\n", status);

        switch (status)
        {
        case 0:
            printf("the child process written succeed\n");
            break;
        case 1:
            printf("can't open file\n");
            break;
        case 2:
            printf("unable to write file\n");
            break;
        }
    }

    return 0;
}