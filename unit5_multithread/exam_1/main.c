#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

#define OUTPUT_FILE_1 "./out1"
#define OUTPUT_FILE_2 "./out2"

#define MAX_WRITE 20

uint32_t commondata = 0;

sem_t mySema;
pthread_mutex_t myMutexCount;
pthread_mutex_t myMutexFile1;
pthread_mutex_t myMutexFile2;

/* Thread */
void *exe_thread_write(void *arg)
{
    int fd1;
    char datafile1[10];
    int fd2;
    char datafile2[10];

    while (commondata < MAX_WRITE)
    {
        printf("Semaphore wait by %s\n", arg);
        sem_wait(&mySema);

        if (pthread_mutex_trylock(&myMutexFile1) == 0)
        {
            memset(datafile1, 0, 10);

            /* mutex for increa count common */
            pthread_mutex_lock(&myMutexCount);
            commondata++;
            sprintf(datafile1, "%d\n", commondata);
            pthread_mutex_unlock(&myMutexCount);

            fd1 = open(OUTPUT_FILE_1, O_CREAT | O_APPEND | O_RDWR, S_IRWXU);
            write(fd1, datafile1, strlen(datafile1));
            close(fd1);
            sleep(1);
            pthread_mutex_unlock(&myMutexFile1);
        }
        else
        {
            pthread_mutex_lock(&myMutexFile2);
            memset(datafile2, 0, 10);

            /* mutex for increa count common */
            pthread_mutex_lock(&myMutexCount);
            commondata++;
            sprintf(datafile2, "%d\n", commondata);
            pthread_mutex_unlock(&myMutexCount);

            fd2 = open(OUTPUT_FILE_2, O_CREAT | O_APPEND | O_RDWR, S_IRWXU);
            write(fd2, datafile2, strlen(datafile2));
            close(fd2);
            sleep(1);
            pthread_mutex_unlock(&myMutexFile2);
        }

        sem_post(&mySema);
    }
}

/* MAIN */
void main()
{
    pthread_t myThread1;
    pthread_t myThread2;
    pthread_t myThread3;

    sem_init(&mySema, 0, 2);
    pthread_mutex_init(&myMutexFile1, NULL);
    pthread_mutex_init(&myMutexFile2, NULL);
    pthread_mutex_init(&myMutexCount, NULL);

    pthread_create(&myThread1, NULL, exe_thread_write, "thread 1");
    pthread_create(&myThread2, NULL, exe_thread_write, "thread 2");
    pthread_create(&myThread3, NULL, exe_thread_write, "thread 3");

    pthread_join(myThread1, NULL);
    pthread_join(myThread2, NULL);
    pthread_join(myThread3, NULL);

    pthread_mutex_destroy(&myMutexFile1);
    pthread_mutex_destroy(&myMutexFile2);
    pthread_mutex_destroy(&myMutexCount);
    sem_destroy(&mySema);
}