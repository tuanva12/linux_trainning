// C program for Consumer process illustrating
// POSIX shared-memory API.
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <semaphore.h>

/* the size (in bytes) of shared memory object */
#define SIZE 4096
#define OFFSET_ID 0
#define OFFSET_DATA 10
/* Data struct */
struct data
{
    char name[25];
    int tuoi;
};
int pidServer;

/* pointer to shared memory obect */
void* ptr;

sem_t* psema;

int main()
{
    /* name of the shared memory object */
    const char* name = "OS1";

    /* shared memory file descriptor */
    int shm_fd;

    /* pointer to shared memory object */
    void* ptr;

    struct data usrdata;

    /* open the shared memory object */
    shm_fd = shm_open(name, O_RDWR, 0666);
    /* configure the size of the shared memory object */

    /* memory map the shared memory object */
    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    /* read pid server */
    pidServer = *(int*)ptr;

    printf("Server id: %d\n", pidServer);

    /* Open named semaphore */
    psema = sem_open("mysema", O_RDWR);

    while(1)
    {
        printf("Enter name: ");
        scanf("%s", usrdata.name);
        printf("Enter eag: ");
        scanf("%d", &usrdata.tuoi);

        /* write data to shared memory */
        sem_wait(psema);
        *(struct data*)(ptr + OFFSET_DATA) = usrdata;
        sem_post(psema);

        /* send signal */
        kill(pidServer, SIGUSR1);

        printf("End of transmit data through memory\n\n");
        sleep(2);
    }

    /* remove the shared memory object */
    shm_unlink(name);


    return 0;
}