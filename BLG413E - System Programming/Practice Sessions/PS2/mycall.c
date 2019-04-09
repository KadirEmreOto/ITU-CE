#include <linux/syscalls.h>
#include <linux/kernel.h>

asmlinkage int sys_mycall(int i, int j){
     return i + j;
}
