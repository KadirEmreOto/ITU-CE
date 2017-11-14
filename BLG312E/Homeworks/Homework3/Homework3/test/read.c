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
    int shmid = shmget(shmkey, shmsize * sizeof(applicant), 0700);

    applicant* shmpointer = shmat(shmid, NULL, 0);
    if (shmpointer == (applicant*)(-1)){
        perror("shmat");
        exit(1);
    }

    printf("Number: %d\n", shmpointer->number);
    printf("Status: %d\n", shmpointer->status);
    printf("Status: %d\n", shmpointer->shmid);

    return 0;
}
