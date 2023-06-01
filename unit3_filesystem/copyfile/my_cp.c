#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char* argv[])
{
  int fd_s = -1;
  int fd_d = -1;
  char src_p[64];
  char des_p[64];
  char file_name[64];
  char buf[128];
  int len = 0;

  /* clean buffer data */
  memset(src_p, 0, 64);
  memset(des_p, 0, 64);
  memset(file_name, 0, 64);
  memset(buf, 0, 128);

  /* get file name*/
  for (int i = strlen(argv[1]); i >0 ; i--)
  {
    if (argv[1][i] == '/')
    {
      memcpy(file_name, &argv[1][i], strlen(&argv[1][i]));
      break;
    }
  }
  printf("%s", file_name);

  sprintf(src_p, "%s", argv[1]);
  sprintf(des_p, "%s%s", argv[2], file_name);

  fd_s = open(src_p, O_RDONLY);
  if (fd_s < 0)
  {
    printf("can't open source file\n");
    return -1;
  }

  fd_d = open(des_p, O_RDWR | O_CREAT, S_IRWXU);
  if (fd_d < 0)
  {
    printf("can't open destinate file\n");
    return -1;
  }

  printf("start file\n");

  do
  {
    len = read(fd_s, buf, 128);
    printf("%s", buf);
    if (len > 0)
    {
      write(fd_d, buf, len);
    }
  } while (len == 128);

  close(fd_s);
  close(fd_d);

  return 0;
}
