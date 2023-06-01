#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
    int fd = 0;

    /* Open file */
    fd = open(argv[0], O_RDWR | O_CREAT, S_IRWXU);
    if (fd < 0)
    {
        printf("can't open file\n");
        return 1;
    }

    if (write(fd, "Hello world.", 12) == -1)
    {
        printf("unable to write file\n");
        return 2;
    }

    close(fd);

    return 0;
}