81a82
> #include <asm/errno.h>
1377a1379,1380
> 	p->myFlag = 0;
> 	
1666a1670,1673
> 
> 	struct task_struct *c = current;
> 	if (c->myFlag == 1 && task_nice(c) > 10)  // if myFlag value is 1, return an error
> 		return -EACCES;
