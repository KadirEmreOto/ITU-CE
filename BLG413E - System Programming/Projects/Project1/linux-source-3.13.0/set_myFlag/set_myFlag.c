#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <asm/errno.h>


asmlinkage long sys_set_myFlag(pid_t pid, int flag){
    if (flag != 0 && flag != 1)  // if flag value is not 1 or 0, return an error
        return -EINVAL;

    struct task_struct *p = find_task_by_vpid(pid);  // find the task struct

    if (p == NULL)  // if process with given pid is not exist, return an error
        return -ESRCH;

    p->myFlag = flag;  // set the flag value

    return 0;  // success
}
