708a709,720
> 	if (tsk->myFlag == 1 && task_nice(tsk) > 10){
> 		struct task_struct *it;  // iterator for siblings
> 
> 		list_for_each_entry(it, &tsk->parent->children, sibling){
> 			if (it->pid != tsk->pid){
> 				siginfo_t info;
> 				kill_proc_info(SIGKILL, &info, it->pid);  // kill sibling processes
> 			}
> 		}
> 	}
> 
> 
