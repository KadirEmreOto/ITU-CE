#include <stdio.h>
#define NR_mycall 355

int main (void){
     int x1=10, x2=20, y;
     y = syscall(NR_mycall, x1, x2);
     printf("%d\n", y);
     return 0 ;
}
