#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/watchdog.h>
#include <asm/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(void)
{
    int fd = 0;
    int c = '\0';
    int timeout = 0;

    fd = open("/dev/watchdog1", O_RDWR); // ham start duoi driver se duoc goi
    if (fd < 0)
    {
        perror("can not open this file, permission...\n");
        return -1;
    }

    while (1)
    {
        printf("\nChoose: 1. Thuc hien ping watchdog\n");
        printf("Choose: 2. set timeout driver\n");
        printf("Choose: 3. stop watchdog\n");

        scanf("%d", &c);
        fflush(stdin);

        switch (c)
        {
        case 1:
            ioctl(fd, WDIOC_KEEPALIVE, 0);
            break;
        case 2:
            puts("nhap timeout");
            scanf("%d", &timeout);
            fflush(stdin);

            ioctl(fd, WDIOC_SETTIMEOUT, &timeout);
            break;
        case 3:
            close(fd); // ham stop duoi driver se duoc goi
            exit(0);
            break;
        default:
            printf("nhap lai");
            break;
        }
    }

    return 0;
}
