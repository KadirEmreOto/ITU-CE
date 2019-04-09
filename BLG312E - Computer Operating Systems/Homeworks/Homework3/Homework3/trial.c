/*
 *  Lecture: Computer Operation Systems
 *  Project: 3
 *  Author: Kadir Emre Oto
 *  Student ID: 150140032
*/

#include "base.h"
SyncronizedQueue* sq;

void* enroll(void* t){
    while (1){
        semwait(trisem_id, 1);                                                      // new applicant came, wake up receptionist
        int aplin = *trimem;
        // printf("Came: %d\n", aplin );

        push(sq, aplin);
        semsignal(tri_ok_id, 1);
    }
}

void* trial(void* t){
    while (1){
        int aplin = pop(sq);
        // printf("After first pop: %d->%d\n", sq->precursor, sq->successor);
        printf("Trainer %d started training with Applicant %d (status of applicant %d: 2)\n", (int)t, aplin+1, aplin+1);
        sleep(apl_input[aplin].trial_time);

        if (apl_input[aplin].trial_result)
            (aplmem + aplin)->status = 4;
        else
            (aplmem + aplin)->status = 3;

        sleep(2);
        printf("Trainer %d finished training with Applicant %d (status of applicant %d: %d)\n", (int)t, aplin+1, aplin+1, (aplmem + aplin)->status);
    }
}

int main(int argc, const char * argv[]) {
    readfile(argv[1]);

    sq = malloc(sizeof(SyncronizedQueue));
    init(sq, shmkey_sq2);

    tri_ok_id = semget(semkey_tri_ok, 1, 0700);                                 // find semophore id created in applicant.c
    trisem_id = semget(semkey_triist, 1, 0700);                                 // semaphore for syncronizing enrollment operation
    enroll_id = semget(semkey_enroll, 1, 0700);

    trimem_id = shmget(shmkey_tri, sizeof(int), 0700);                          // allocate from shared memory
    trimem = shmat(trimem_id, NULL, 0);                                         // create shared memory pointer

    aplmem_id = *trimem;
    aplmem = shmat(aplmem_id, NULL, 0);
    semsignal(enroll_id, 1);

    pthread_t receptionist;                                                     // receptionist thread
    pthread_t trainers [TINT+1];                                                // interviewers threads

    pthread_create(&receptionist, NULL, enroll, NULL);

    int i;
    for (i = 1; i <= TINT; i++)
        pthread_create(trainers+i, NULL, trial, (void *)i);

    pthread_join(receptionist, NULL);                                           // wait for all applicants done

    return 0;
}
