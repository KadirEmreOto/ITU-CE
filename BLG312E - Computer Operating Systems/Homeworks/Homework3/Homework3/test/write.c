/*
 *  Lecture: Computer Operation Systems
 *  Project: 3
 *  Author: Kadir Emre Oto
 *  Student ID: 150140032
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

key_t shmkey = 785644;
size_t shmsize = 20;

typedef struct applicant applicant;

struct applicant{
    int number;
    int status;
    int shmid;
};

int main(int argc, const char * argv[]) {
    int shmid = shmget(shmkey, shmsize * sizeof(applicant), IPC_CREAT | 0700);  // allocate from shared memory

    if (shmid == -1){
        perror("Shared Memory Allocation Error!\n");
        exit(1);
    }

    printf("(%d)\n", shmid);
    applicant* shmpointer = shmat(shmid, NULL, 0);   // create shared memory pointer

    if (shmpointer == (applicant*)(-1)){
        perror("Shared Memory Pointer Error!\n");
        exit(1);
    }

    applicant t;
    t.number = 2;
    t.status = 2;
    t.shmid = 12453;

    shmpointer[0] = t;

    shmdt(&shmpointer);                         // delete shared memory pointer
    // shmctl(shmid, IPC_RMID, NULL);              // release shared memory

    return 0;
}
