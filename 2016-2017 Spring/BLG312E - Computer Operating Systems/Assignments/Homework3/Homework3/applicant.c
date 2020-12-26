/*
 *  Lecture: Computer Operation Systems
 *  Project: 3
 *  Author: Kadir Emre Oto
 *  Student ID: 150140032
*/

#include "base.h"                                                               // import base header file

void enroll(int applicant_id){
    semwait(enroll_id, 2);                                                      // only one applicant can enroll at a time
    // printf("Enroll: %d\n", applicant_id);
    *regmem = applicant_id;
    semsignal(regsem_id, 1);                                                    // new applicant came, wake up receptionist

    semwait(reg_ok_id, 1);
    semsignal(enroll_id, 2);                                                    // this applicant enrolled
}

int main(int argc, const char * argv[]) {
    int applicant_count = atoi(argv[1]);

    enroll_id = semget(semkey_enroll, 1, IPC_CREAT | 0700);                     // semaphore for syncronizing enrollment operation
    reg_ok_id = semget(semkey_reg_ok, 1, IPC_CREAT | 0700);
    regsem_id = semget(semkey_regist, 1, IPC_CREAT | 0700);                     // semaphore for syncronizing enrollment operation
    tri_ok_id = semget(semkey_tri_ok, 1, IPC_CREAT | 0700);
    trisem_id = semget(semkey_triist, 1, IPC_CREAT | 0700);                     // semaphore for syncronizing enrollment operation
    enroll_trial_id = semget(semkey_enroll_trial, 1, IPC_CREAT | 0700);         // semaphore for syncronizing enrollment operation

    semctl(enroll_id, 0 , SETVAL, 0);                                           // set first value of semaphores
    semctl(regsem_id, 0 , SETVAL, 0);
    semctl(reg_ok_id, 0 , SETVAL, 0);
    semctl(trisem_id, 0 , SETVAL, 0);
    semctl(tri_ok_id, 0 , SETVAL, 0);
    semctl(enroll_trial_id, 0 , SETVAL, 1);

    aplmem_id = shmget(shmkey_apl, sizeof(applicant) * applicant_count, IPC_CREAT | 0700);  // allocate shared memory for applicants
    aplmem = shmat(aplmem_id, NULL, 0);                                         // attach the pointer to our process

    regmem_id = shmget(shmkey_reg, sizeof(int), IPC_CREAT | 0700);              // allocate shared memory for registration
    regmem = shmat(regmem_id, NULL, 0);                                         // attach the pointer

    trimem_id = shmget(shmkey_tri, sizeof(int), IPC_CREAT | 0700);              // allocate shared memory for trainers
    trimem = shmat(trimem_id, NULL, 0);

    *regmem = aplmem_id;
    *trimem = aplmem_id;

    int i; pid_t pid;
    for (i = 0; i < applicant_count; i++){
        pid = fork();

        if (pid == 0){                                                          // new applicant
            printf("Applicant %d applied to the receptionist (status of applicant %d: 0)\n", i+1, i+1);
            aplmem[i].number = i;
            enroll(i);

            break;
        }
    }

    if (pid != 0 && 0){
        printf("Remove SHM_REG\n");
        shmctl(shmkey_reg, IPC_RMID, NULL);                                          // release shared memory
    }

    return 0;
}
