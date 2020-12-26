#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>

#include "queue_ioctl.h"

int main(int argc, char *argv[]){
    // usage: ./pop_test [queue_number]

    if (argc != 2){
        printf("usage: ./pop_test [queue_number]\n");
        return 0;
    }

	int status = -1;
	char name[100] = "/dev/queue";
    strcat(name, argv[1]);
    
    int fd = open(name, O_RDWR);
    status = ioctl(fd, QUEUE_IOCPOP);

    if (status != 0)
        printf("Error: %s \n", strerror(-status));
	
	return 0;
}
