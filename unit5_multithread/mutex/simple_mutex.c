#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

/*
gcc -o simple_thread simple_thread.c -lpthread

*/


int count = 0;
pthread_mutex_t my_mutex;

void *my_thread(void* arg)
{
    pthread_mutex_lock(&my_mutex);
    for (size_t i = 0; i < 50000; i++)
    {
        count ++;
    }
    pthread_mutex_unlock(&my_mutex);

}

void main()
{
    pthread_t thread1;
    pthread_t thread2;

    pthread_mutex_init(&my_mutex, NULL);

    pthread_create(&thread1, NULL, my_thread, "thread 1");
    pthread_create(&thread1, NULL, my_thread, "thread 2");

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_mutex_destroy(&my_mutex);

    printf("%d\n", count);
}