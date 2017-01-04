#define   __LIBRARY__
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define   N        500
#define   BUFFERSIZE   10

int main()
{
    int shmid;
    int put_pos = 0, i;
    int *buffer;
    sem_t *empty, *full, *mutex;

    empty = (sem_t *)sem_open("empty", O_CREAT, 0777, 10);
    full  = (sem_t *)sem_open("full", O_CREAT, 0777, 0);
    mutex = (sem_t *)sem_open("mutex",O_CREAT, 0777, 1);

    shmid = shmget( 555555, BUFFERSIZE*sizeof(int), IPC_CREAT|0666);  


    buffer = (int*)shmat(shmid, NULL, 0);
    

    for( i = 0 ; i < N; i++)
        {
            sem_wait(empty);
            sem_wait(mutex);

            buffer[put_pos] = i;
            put_pos = ( put_pos + 1 ) % BUFFERSIZE;

            sem_post(mutex);
            sem_post(full);
        }


    return 0;
}
