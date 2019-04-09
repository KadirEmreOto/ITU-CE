rm -rf Project1
mkdir Project1
mkdir Project1/linux-source-3.13.0
mkdir Project1/linux-source-3.13.0/set_myFlag

mkdir Project1/linux-source-3.13.0/include
mkdir Project1/linux-source-3.13.0/include/linux

mkdir Project1/linux-source-3.13.0/arch
mkdir Project1/linux-source-3.13.0/arch/x86
mkdir Project1/linux-source-3.13.0/arch/x86/syscalls
mkdir Project1/linux-source-3.13.0/arch/x86/boot
mkdir Project1/linux-source-3.13.0/arch/x86/boot/compressed

mkdir Project1/linux-source-3.13.0/kernel

cp linux-source-3.13.0/set_myFlag/set_myFlag.c Project1/linux-source-3.13.0/set_myFlag
cp linux-source-3.13.0/set_myFlag/Makefile Project1/linux-source-3.13.0/set_myFlag

cp linux-source-3.13.0/include/linux/sched.h Project1/linux-source-3.13.0/include/linux/
cp linux-source-3.13.0/include/linux/init_task.h Project1/linux-source-3.13.0/include/linux/
cp linux-source-3.13.0/include/linux/syscalls.h Project1/linux-source-3.13.0/include/linux/

cp linux-source-3.13.0/arch/x86/syscalls/syscall_32.tbl Project1/linux-source-3.13.0/arch/x86/syscalls/

cp linux-source-3.13.0/kernel/exit.c Project1/linux-source-3.13.0/kernel
cp linux-source-3.13.0/kernel/fork.c Project1/linux-source-3.13.0/kernel

cp linux-source-3.13.0/Makefile Project1/linux-source-3.13.0
cp linux-source-3.13.0/arch/x86/boot/compressed/head_32.S Project1/linux-source-3.13.0/arch/x86/boot/compressed/

mkdir Project1/Tests
cp kill_test.c Project1/Tests
cp fork_test.c Project1/Tests

mkdir Project1/Diffs
diff linux-source-3.13.0-main/include/linux/sched.h Project1/linux-source-3.13.0/include/linux/sched.h > Project1/Diffs/sched.h
diff linux-source-3.13.0-main/include/linux/init_task.h Project1/linux-source-3.13.0/include/linux/init_task.h > Project1/Diffs/init_task.h
diff linux-source-3.13.0-main/include/linux/syscalls.h Project1/linux-source-3.13.0/include/linux/syscalls.h > Project1/Diffs/syscalls.h

diff linux-source-3.13.0-main/arch/x86/syscalls/syscall_32.tbl Project1/linux-source-3.13.0/arch/x86/syscalls/syscall_32.tbl  > Project1/Diffs/syscall_32.tbl

diff linux-source-3.13.0-main/kernel/exit.c Project1/linux-source-3.13.0/kernel/exit.c  > Project1/Diffs/exit.c
diff linux-source-3.13.0-main/kernel/fork.c Project1/linux-source-3.13.0/kernel/fork.c  > Project1/Diffs/fork.c

diff linux-source-3.13.0-main/Makefile Project1/linux-source-3.13.0/Makefile  > Project1/Diffs/Makefile
diff linux-source-3.13.0-main/arch/x86/boot/compressed/head_32.S Project1/linux-source-3.13.0/arch/x86/boot/compressed/head_32.S  > Project1/Diffs/head_32.S
