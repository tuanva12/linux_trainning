#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

/*
gcc -o simple_thread simple_thread.c -lpthread

*/





void *my_thread(void* arg)
{
    printf("Hello thread %s\n", (char*)arg);
    sleep(5);
}

void main()
{
    pthread_t thread1;
    pthread_t thread2;

    pthread_create(&thread1, NULL, my_thread, "thread 1");
    pthread_create(&thread1, NULL, my_thread, "thread 2");

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
}