#ifndef __QUEUE_IOCTL
#define __QUEUE_IOCTL

#include <linux/ioctl.h>

#define QUEUE_IOC_MAGIC  'k'
#define QUEUE_IOCPOP    _IO(QUEUE_IOC_MAGIC, 0)
#define QUEUE_IOC_MAXNR 0

#endif