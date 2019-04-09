#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/types.h>

typedef struct applicant applicant;
typedef struct applicant_input applicant_input;
typedef struct SyncronizedQueue SyncronizedQueue;

struct applicant{
    int number;
    int status;
    int shmid;
} *aplmem;

struct applicant_input{
    int number;
    int interview_time;
    int trial_time;
    int interview_result;
    int trial_result;
};

key_t semkey_enroll = 7;
key_t semkey_regist = 8;
key_t semkey_reg_ok = 9;
key_t semkey_enroll_trial = 17;
key_t semkey_triist = 18;
key_t semkey_tri_ok = 19;

key_t shmkey_apl = 300;
key_t shmkey_reg = 301;
key_t shmkey_tri = 401;

key_t shmkey_sq1 = 600;
key_t shmkey_sq2 = 800;

size_t shmsize = 1024;
size_t NINT = 3;                                                                // number of interviews
size_t TINT = 5;                                                                // number of trialers

int* regmem;
int* trimem;

int enroll_id;
int reg_ok_id;
int regsem_id;
int tri_ok_id;
int trisem_id;
int enroll_trial_id;

int regmem_id;
int trimem_id;
int aplmem_id;

// increment operation
void semsignal(int semid, int val){
    struct sembuf semaphore;
    semaphore.sem_num = 0;
    semaphore.sem_op = val;
    semaphore.sem_flg = 1;
    semop(semid, &semaphore, 1) ;
}

// decrement operation
void semwait(int semid, int val){
    struct sembuf semaphore;
    semaphore.sem_num = 0;
    semaphore.sem_op = (-1*val);
    semaphore.sem_flg = 1;
    semop (semid, &semaphore, 1) ;
}


//-------------- SyncronizedQueue --------------
struct SyncronizedQueue{
    int semid;
    int semid_count;

    key_t semkey;
    key_t semkey_count;

    int precursor;
    int successor;
    int* array;
};

void init(SyncronizedQueue* q, int sk){                                         // initilize given SyncronizedQueue using given semaphore key
    q->precursor = 0;                                                           // cursors point the beggining of array
    q->successor = 0;

    q->semkey = sk;                                                             // assign keys to queue
    q->semkey_count = sk + 734;

    q->semid = semget(q->semkey, 1, IPC_CREAT | 0700);                          // create semaphores
    q->semid_count = semget(q->semkey_count, 1, IPC_CREAT | 0700);

    q->array = malloc(sizeof(int) * 1024);                                      // allocate memory for array
    semctl(q->semid, 0 , SETVAL, 1);                                            // first value is 1
    semctl(q->semid_count, 0 , SETVAL, 0);                                      // represents number of elements of queue
}

int empty(SyncronizedQueue* q){                                                 // return 1 if queue is empty else 0
    return q->precursor == q->successor;
}

void push(SyncronizedQueue* q, int a){
    semwait(q->semid, 1);                                                       // only one item can be added at a time

    *(q->array + q->successor) = a;
    q->successor++;

    semsignal(q->semid, 1);                                                     // adding process is done
    semsignal(q->semid_count, 1);                                               // increase number of items
}

int pop(SyncronizedQueue* q){
    semwait(q->semid_count, 1);                                                 // if queue has element
    semwait(q->semid, 1);                                                       // only one element can be popped

    int a = *(q->array + q->precursor);
    q->precursor++;

    semsignal(q->semid, 1);                                                     // pop process is done
    return a;
}

//-------------- readfile --------------
int regtime;
int applicant_count;
applicant_input* apl_input;

void readfile(const char* filename){
    FILE *input = fopen(filename, "r");
    apl_input = malloc(sizeof(applicant_input) * 1024);

    fscanf(input, "%d", &regtime);

    int index = 0;

    while (!feof(input)) {
        fscanf(input, "%d", &(apl_input[index].interview_time));
        fscanf(input, "%d", &(apl_input[index].trial_time));
        fscanf(input, "%d", &(apl_input[index].interview_result));
        fscanf(input, "%d", &(apl_input[index].trial_result));

        if (feof(input)) break;
        index++;
    }
    applicant_count = index;
}
