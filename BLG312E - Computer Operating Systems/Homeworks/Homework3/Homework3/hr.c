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
        semwait(regsem_id, 1);                                                  // new applicant came, wake up
        int aplin = *regmem;                                                    // ask who are you

        sleep(regtime);                                                         // wait registration
        (aplmem + aplin)->status = 0;                                           // set the status

        printf("Applicant %d’s registration is done (status of applicant %d: 0)\n", aplin+1, aplin+1);

        push(sq, aplin);                                                        // add the applicant to queue for interviewing
        semsignal(reg_ok_id, 1);                                                // the applicant enrolled, call one more
    }
}

void* interview(void* s){
    while (1){
        int aplin = pop(sq);
        printf("Interviewer %d started interview with Applicant %d (status of applicant %d: 0)\n", (int)s, aplin+1, aplin+1);
        // sleep(apl_input[aplin].interview_time);

        if (apl_input[aplin].interview_result){
            (aplmem + aplin)->status = 2;

            sleep(apl_input[aplin].interview_time);
            printf("Interviewer %d finished interview with Applicant %d (status of applicant %d: %d)\n", (int)s, aplin+1, aplin+1, (aplmem + aplin)->status);

            semwait(enroll_trial_id, 1);                                        // only one applicant can enroll at a time
            *trimem = aplin;
            semsignal(trisem_id, 1);                                            // new applicant came, wake up receptionist

            semwait(tri_ok_id, 1);
            semsignal(enroll_trial_id, 1);
        }
        else{
            (aplmem + aplin)->status = 1;
            printf("Interviewer %d finished interview with Applicant %d (status of applicant %d: %d)\n", (int)s, aplin+1, aplin+1, (aplmem + aplin)->status);
        }
    }
}

int main(int argc, const char * argv[]) {
    readfile(argv[1]);
    printf("Registeration time is: %d seconds\n", regtime);

    sq = malloc(sizeof(SyncronizedQueue));
    init(sq, shmkey_sq1);

    reg_ok_id = semget(semkey_reg_ok, 1, 0700);                                 // find semophore id created in applicant.c
    regsem_id = semget(semkey_regist, 1, 0700);                                 // semaphore for syncronizing enrollment operation
    enroll_id = semget(semkey_enroll, 1, 0700);
    enroll_trial_id = semget(semkey_enroll_trial, 1, 0700);

    regmem_id = shmget(shmkey_reg, sizeof(int), 0700);                          // allocate from shared memory
    regmem = shmat(regmem_id, NULL, 0);                                         // create shared memory pointer

    tri_ok_id = semget(semkey_tri_ok, 1, 0700);                                 // find semophore id created in applicant.c
    trisem_id = semget(semkey_triist, 1, 0700);                                 // semaphore for syncronizing enrollment operation

    trimem_id = shmget(shmkey_tri, sizeof(int), 0700);                          // allocate from shared memory
    trimem = shmat(trimem_id, NULL, 0);

    aplmem_id = *regmem;
    aplmem = shmat(aplmem_id, NULL, 0);
    semsignal(enroll_id, 1);

    pthread_t receptionist;                                                     // receptionist thread
    pthread_t interviewers [NINT+1];                                            // interviewers threads

    pthread_create(&receptionist, NULL, enroll, NULL);

    int i;
    for (i = 1; i <= NINT; i++)
        pthread_create(interviewers+i, NULL, interview, (void *)i);

    pthread_join(receptionist, NULL);                                           // wait for all applicants done

    return 0;
}
