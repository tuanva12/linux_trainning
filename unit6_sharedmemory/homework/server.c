#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
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

FILE *pResult;

/* signal receiver */
void sig_handler(int signo)
{
    struct data datareceiv;

    if (signo == SIGUSR1)
    {
        sem_wait(psema);
        // sleep(15);
        datareceiv = *(struct data *)(ptr + OFFSET_DATA);
        sem_post(psema);
        /* open file for writing data result */
        pResult = fopen("logfile.txt", "a");
        fprintf(pResult, "Name: %s\n", datareceiv.name);
        fprintf(pResult, "Tuoi: %d\n", datareceiv.tuoi);
        fclose(pResult);
        printf("Write to log file.\n");
    }
}

/* main function */
int main()
{
    pidServer = getpid();
    printf("Server start on id: %d\n", pidServer);

    /* name of the shared memory object */
    const char* name = "OS1";

    /* shared memory file descriptor */
    int shm_fd;

    /* create the shared memory object */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    /* configure the size of the shared memory object */
    ftruncate(shm_fd, SIZE);

    /* memory map the shared memory object */
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    /* write process id */
    *(int*)ptr = pidServer;

    /* Open named semaphore */
    psema = sem_open("mysema", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 1);

    /* init signal */
    if (signal(SIGUSR1, sig_handler) == SIG_ERR)
    {
        printf("\ncan't catch SIGUSR1\n");
    }

    while (1)
    {
        printf("sleep\n");
        sleep(1);
    }

    return 0;
}