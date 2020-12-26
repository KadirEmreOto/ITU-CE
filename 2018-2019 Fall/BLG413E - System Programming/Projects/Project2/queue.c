#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>       // kprintf
#include <linux/slab.h>         // kmalloc
#include <linux/fs.h>           
#include <linux/errno.h>        // error codes
#include <linux/types.h>        // size_t
#include <linux/proc_fs.h>
#include <linux/fcntl.h>        // O_ACCMODE
#include <linux/seq_file.h>
#include <linux/cdev.h>

#include <asm/switch_to.h>      // cli, *_flags
#include <asm/uaccess.h>        // copy_*_user

#include "queue_ioctl.h"

#define QUEUE_MAJOR 0
#define QUEUE_MINOR 0
#define QUEUE_NR_DEVS 5

int queue_major = QUEUE_MAJOR;
int queue_minor = QUEUE_MINOR;
int queue_nr_devs = QUEUE_NR_DEVS;

module_param(queue_major, int, S_IRUGO);
module_param(queue_minor, int, S_IRUGO);
module_param(queue_nr_devs, int, S_IRUGO);

MODULE_AUTHOR("Kadir Emre Oto, Muhammed Burak Bugrul, Ezgi Yilmaz");
MODULE_LICENSE("Dual BSD/GPL");

struct queue_node {
    struct queue_node * next;
    char* data;
    size_t size; // size of data
};

struct queue_dev {
    struct queue_node *head;
    struct queue_node *tail;

    int id;  // device id 
    unsigned long size;  // size of queue (number of queue nodes)
    unsigned long data_size;  // size of queue data (number of characters in queue)
    
    struct semaphore sem;
    struct cdev cdev;
};

void enqueue(struct queue_dev *dev, char* data, size_t count){
    struct queue_node *node = kmalloc(sizeof(struct queue_node), GFP_KERNEL);

    node->size = count;
    node->next = NULL;
    node->data = kmalloc(sizeof(char) * (count), GFP_KERNEL);
    strncpy(node->data, data, count);

    if (dev->head == NULL)
        dev->head = dev->tail = node;

    else{
        dev->tail->next = node;
        dev->tail = node;
    }

    dev->size++;
    dev->data_size += count;
}

char* dequeue(struct queue_dev *dev){
    char* retval = dev->head->data;

    struct queue_node *tmp = dev->head;
    dev->head = dev->head->next;

    if (dev->head == NULL)
        dev->tail = NULL;
    
    dev->size--;
    dev->data_size -= tmp->size;

    kfree(tmp);
    return retval;
}


struct queue_dev *queue_devices;

int queue_trim(struct queue_dev *dev){
    while (dev->size)  // pop all nodes in queue and deallocate string data 
        kfree(dequeue(dev));

    return 0;
}

int queue_open(struct inode *inode, struct file *filp){
    struct queue_dev *dev;
    
    dev = container_of(inode->i_cdev, struct queue_dev, cdev);
    filp->private_data = dev;

    return 0;
}

int queue_release(struct inode *inode, struct file *filp){ 

    return 0;
}

ssize_t queue_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos){
    struct queue_dev *dev = filp->private_data;
    int cursor = 0;
    int total = 0;
    struct queue_node *it;

    if (dev->id == 0)  // read op is not permitted for queue0
        return -EPERM;

    if (down_interruptible(&dev->sem))
        return -ERESTARTSYS;

    it = dev->head;

    while (it && cursor + it->size < *f_pos){
        cursor += it->size;
        it = it->next;
    }

    if (it && cursor < *f_pos){
        int length = it->size - (*f_pos - cursor);

        if (length > count)
            length = count;

        total = length;
        if (copy_to_user(buf + total, it->data, length)){  // copy_to_user returns number of bytes that could not be copied
            up(&dev->sem);
            return -EFAULT;
        }
        it = it->next;
    }

    while (it && total < count){
        int length = it->size;

        if (length > count - total)
            length = count - total;

        if (copy_to_user(buf + total, it->data, length)){
            up(&dev->sem);
            return -EFAULT;
        }
        it = it->next;
        total += length;
    }
    
    *f_pos += total;
    up(&dev->sem);

    return total;
}

ssize_t queue_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos){
    struct queue_dev *dev = filp->private_data;
    char *data;

    if (dev->id == 0)
        return -EPERM;

    if (down_interruptible(&dev->sem))
        return -ERESTARTSYS;

    data = kmalloc(sizeof(char) * (count), GFP_KERNEL);
    
    if (!data)
        return -ENOMEM;

    if (copy_from_user(data, buf, count)){ 
        up(&dev->sem);
        return -EFAULT;
    }

    enqueue(dev, data, count);
    kfree(data);

    up(&dev->sem);
    return count;
}


long queue_ioctl(struct file *filp, unsigned int cmd, unsigned long arg){
    struct queue_dev *dev;

	if (_IOC_TYPE(cmd) != QUEUE_IOC_MAGIC) 
        return -ENOTTY;
	
    if (_IOC_NR(cmd) > QUEUE_IOC_MAXNR) 
        return -ENOTTY;

	if (_IOC_DIR(cmd) & _IOC_READ){
		if (!access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd)))
            return -EFAULT;
    }

	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		if (!access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd)))
            return -EFAULT;
	
    dev = filp->private_data;

    if (dev->id == 0){
        int i;
        for (i = 1; i < queue_nr_devs; i++)
            if (queue_devices[i].size){
                kfree(dequeue(queue_devices+i));
                return 0;
            }
        return -EPERM;
    }

    else
        if (dev->size)
            kfree(dequeue(dev));
        else
            return -EPERM;
	
    return 0;
}


loff_t queue_llseek(struct file *filp, loff_t off, int whence){
    struct queue_dev *dev = filp->private_data;
    loff_t newpos;

    switch(whence) {
        case 0: /* SEEK_SET */
            newpos = off;
            break;

        case 1: /* SEEK_CUR */
            newpos = filp->f_pos + off;
            break;

        case 2: /* SEEK_END */
            newpos = dev->data_size + off;
            break;

        default: /* can't happen */
            return -EINVAL;
    }
    if (newpos < 0)
        return -EINVAL;
    filp->f_pos = newpos;
    return newpos;
}

struct file_operations queue_fops = {
    .owner =        THIS_MODULE,
    .llseek =       queue_llseek,
    .read =         queue_read,
    .write =        queue_write,
    .unlocked_ioctl = queue_ioctl,
    .open =         queue_open,
    .release =      queue_release,
};

void queue_cleanup_module(void){
    dev_t devno = MKDEV(queue_major, queue_minor);

    if (queue_devices){
        int i;
        for (i=0; i < queue_nr_devs; i++){
            queue_trim(queue_devices + i);
            cdev_del(&queue_devices[i].cdev);
        }
        kfree(queue_devices);
    }
    
    unregister_chrdev_region(devno, queue_nr_devs);
}

int queue_init_module(void){
    dev_t devno;
    int result, i;
    
    if (queue_major){
        devno = MKDEV(queue_major, queue_minor);
        result = register_chrdev_region(devno, queue_nr_devs, "queue");
    }
    else{
        result = alloc_chrdev_region(&devno, queue_minor, queue_nr_devs, "queue");
        queue_major = MAJOR(devno);
    }

    if (result < 0){
        printk(KERN_WARNING "queue: can't get major %d\n", queue_major);
        return result;
    }   

    queue_devices = kmalloc(queue_nr_devs * sizeof(struct queue_dev), GFP_KERNEL);

    if (!queue_devices){
        queue_cleanup_module();
        return -ENOMEM;   // out of memory
    }

    memset(queue_devices, 0, queue_nr_devs * sizeof(struct queue_dev));
    
    for (i=0; i < queue_nr_devs; i++){
        int err;

        queue_devices[i].id = i;
        queue_devices[i].size = 0;
        queue_devices[i].data_size = 0;
        queue_devices[i].head = NULL;
        queue_devices[i].tail = NULL;

        sema_init(&queue_devices[i].sem, 1);
        
        devno = MKDEV(queue_major, queue_minor + i);
        cdev_init(&queue_devices[i].cdev, &queue_fops);
        
        queue_devices[i].cdev.owner = THIS_MODULE;
        queue_devices[i].cdev.ops = &queue_fops;
        
        err = cdev_add(&queue_devices[i].cdev, devno, 1);
        
        if (err)
            printk(KERN_NOTICE "Error %d adding queue%d\n", err, i);
    }

    return 0;
}



module_init(queue_init_module);
module_exit(queue_cleanup_module);
