#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>


int main( int argc, char **argv ){

    if( argc != 2 ){
        printf("Usage: './a.out <flagVal>'");
        return 0;
    }

    int rootPID = getpid();
    int flagVal = atoi(argv[1]);

    printf("Root: %d\n", rootPID);

    int firstFork = fork();

    if( firstFork < 0 ){
        printf("An error occured in first fork stage: %s\n", strerror(-firstFork));
        return 0;
    }
    
    else if( firstFork > 0 ){  // parent process 
		
		printf("First Child: %d\n", firstFork);
        int secondFork = fork();

        if( secondFork < 0 ){
            printf("An error occured in second fork stage: %s\n", strerror(-secondFork));
            return secondFork;
        }
        else if( secondFork > 0 ){  // parent process
			printf("Second Child: %d\n", secondFork);
            system("sleep 1");

            int sysRev = syscall(355, firstFork, flagVal);  // set flag value

            if (sysRev < 0){
                printf("An error occured in system call stage: %s\n", strerror(-sysRev));
                return 0;
            }

            int killRev = kill(firstFork, SIGKILL);  // kill one of siblings 

            if( killRev < 0 ){
                printf("An error occured in kill stage: %s\n", strerror(-killRev));
                return 0;
            }

            system("sleep 10");
            return 0;
        }
        else{  // child process

            int cnt = 0;

            while( cnt < 40 ){
                printf("%d I'm alive :)  (Second child)\n", cnt);
                system("sleep 0.2");
                cnt++;
            }
        }
    }
    else{

        nice(11);
        
        int cnt = 0;
              
        while( cnt < 40 ){
			printf("%d I'm alive :)  (First child)\n", cnt);
			system("sleep 0.2");
			cnt++;
        }
    }

    return 0;
}
