#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>




int main()
{
	char path[] = "./data";
	char buff[128];
	int fd = -1;
	int len = 0;

	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		printf("can't open file");
		return -1;
	}

	len = read(fd, buff, sizeof(buff) - 1);

	if (len > 0)
		printf("read : %s\n", buff);
	else
		printf("can't open file");

	close(fd);

	return 0;
}
