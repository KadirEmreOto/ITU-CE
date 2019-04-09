#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>


int main(int argc, char **argv){
	if( argc != 2 ){
        printf("Usage: './a.out <flagVal>'");
        return 0;
    }

	int rootPID = getpid();
	int flagVal = atoi(argv[1]);

	nice(11);  // increment the nice value of root process by 11
	int sysRev = syscall(355, rootPID, flagVal);  // set the flag value
	
	if (sysRev < 0){
		printf("An error occured in system call stage: %s\n", strerror(-sysRev));
		return 0;
	}
	
	int f = fork();  // do fork
	
	if (f < 0)
		printf("An error occured in fork stage: %s\n", strerror(-f));
	
	else if (f == 0)
		printf("Child created, pid: %d\n", getpid());

	return 0;
}
