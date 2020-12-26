/*
 *  Lecture: Computer Operation Systems
 *  Project: 3
 *  Author: Kadir Emre Oto
 *  Student ID: 150140032
*/

#include "base.h"

SyncronizedQueue* sq;                                                           

void* add(void* a){
    push(sq, ((int)a));
    return NULL;
}

void traverse(){
    while (1){
        int a = pop(sq);
        printf("Number: %d \n", a);
    }
}


int main(int argc, const char * argv[]) {
    pthread_t receptionist;                                                     // receptionist thread
    pthread_t interviewers [5+1];                                               // interviewers threads

    sq = malloc(sizeof(SyncronizedQueue));
    init(sq, 412);
    int i;
    for (i = 1; i <= 5; i++){
        pthread_create(interviewers+i, NULL, add, (void *)i);
    }

    pthread_join(interviewers[i - 1], NULL);
    traverse();
    return 0;
}
