#include <stdio.h>
#include <string.h>

#define PATH "/dev/misc_example"

int main()
{
    FILE *fp = fopen(PATH, "r");
    char buf[100];
    int n = 0;

    if (NULL == fp) {
        printf("Mo khong thanh cong\n");
        return -1;
    }

    memset(buf, 0, sizeof(buf));
    n = fread(buf, 1, sizeof(buf), fp);
    if (n > 0)
        printf("read: %s\n", buf);

    fclose(fp);

    return 0;
}