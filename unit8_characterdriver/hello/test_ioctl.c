#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

struct config
{
    int gpio_num;
    int tan_so;
};

#define MAGIC_NO	100
#define SEND_DATA_CMD	_IOW(MAGIC_NO, sizeof(struct config), struct config *)

int main()
{
    int fd = -1;
    struct config data;
    int len = -1;

    fd = open("/dev/misc_example", O_RDWR);

    if (fd > 0) {
        data.gpio_num = 6;
        data.tan_so = 5;
        ioctl(fd, SEND_DATA_CMD, &data);
        close(fd);
    } else {
        printf("Can't open device file\n");
    }

    return 0;
}