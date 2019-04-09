#include <linux/init.h>     /* for module_init and module_exit */
#include <linux/module.h>   /* needed by all modules */
MODULE_LICENSE("Dual BSD/GPL");   /* a macro to declare that this module is open source */

static int hello_init(void)  /* static: unvisible outside the module to avoid namespace pollution */
{
	printk(KERN_ALERT "Hello, world\n");   /* printk: kernel print function (macros for priority) */
	return 0;                              /* KERN_ALERT: a situation requiring immediate action */
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
