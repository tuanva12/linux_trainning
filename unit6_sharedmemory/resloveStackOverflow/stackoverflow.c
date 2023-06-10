/*
    Làm lại yêu cầu tại : https://stackoverflow.com/questions/72823495/trouble-with-shared-memory-program-in-c


Vấn đề:
   Biến i không được động bộ giữa các process
   Hàm getc không trả về được giá trị EOF
   Khi fork process, tiến trình tác ra và chạy đồng thời vên không thể vào vòng for do biến i chưa được tăng lên.
   Vấn đề khởi tạo sem : semget(claveSem, 1, IPC_CREAT | 0100);






*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    FILE *archivo, *salida1, *salida2;
    int i = 0;
    int j, k;
    char actual;

    key_t claveMC = ftok("UCAB.txt", 5);
    int shmid = shmget(claveMC, 512, 0777 | IPC_CREAT);
    char *buffer = (char *)shmat(shmid, NULL, 0);

    memset(buffer, 0, 512);

    key_t claveSem = ftok("UCAB.txt", 7);
    // int idsem = semget(claveSem, 1, IPC_CREAT | 0100);
    int idsem = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);

    struct sembuf valores;
    valores.sem_num = 0;
    valores.sem_flg = 0;
    semctl(idsem, 0, SETVAL, 1);

    // archivo = fopen("UCAB.txt", "r");

    int id = fork();
    int id2;

    if (id > 0)
    {
        printf("P: %d", id);
        id2 = fork();
        if (id2 > 0)
        {
            /* open file */
            archivo = fopen("UCAB.txt", "r");

            // while (((actual = getc(archivo)) != EOF) && (i < 512))
            // {
            //     valores.sem_op = -1;
            //     semop(idsem, &valores, 1);
            //     buffer[i] = actual;
            //     valores.sem_op = 1;
            //     semop(idsem, &valores, 1);
            //     i++;
            // }

            /* read file store on to memory shared */
            fgets(buffer, 512, archivo);
            printf("%s", buffer);

            /* operation semaphore */
            valores.sem_op = -1;
            semop(idsem, &valores, 1);
            // printf("%s", buffer);
            // valores.sem_op = 1;
            // semop(idsem, &valores, 1);

            fclose(archivo);
            printf("\nPadre con id: %d", id2);

            waitpid(id, NULL, 0);
            waitpid(id2, NULL, 0);
        }
        else
        {
            printf("H2: %d", id);
            salida2 = fopen("minusculas.txt", "w");

             /* wait for semaphore reach to 0 */
            valores.sem_op = 0;
            semop(idsem, &valores, 1);

            for (j = 0; j < strlen(buffer); j++)
            {
                // valores.sem_op = -1;
                // semop(idsem, &valores, 1);
                putc(tolower(buffer[j]), salida2);
                // valores.sem_op = 1;
                // semop(idsem, &valores, 1);
            }

            printf("\nHijo 2 con id: %d", id2);
            fclose(salida2);

            return 0;
        }
    }
    else
    {
        // hijo1
        printf("H1: %d", id);
        salida1 = fopen("MAYUSCULAS.txt", "w");

        /* wait for semaphore reach to 0 */
        valores.sem_op = 0;
        semop(idsem, &valores, 1);

        for (k = 0; k < strlen(buffer); k++)
        {
            // valores.sem_op = -1;
            // semop(idsem, &valores, 1);
            putc(toupper(buffer[k]), salida1);
            // valores.sem_op = 1;
            // semop(idsem, &valores, 1);
        }

        printf("\nHijo 1 con id: %d", id);
        fclose(salida1);

        return 0;
    }

    shmctl(shmid, IPC_RMID, NULL);
    semctl(idsem, 0, IPC_RMID, 0);

    return 0;
}