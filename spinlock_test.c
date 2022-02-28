#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kthread.h>             
#include <linux/sched.h>             
#include <linux/delay.h>

DEFINE_SPINLOCK(spinlock);
static struct task_struct *thread1;
static struct task_struct *thread2; 

int mail = 0;

int routine(void)
{
    int i;
	for(i = 0; i< 1000000; i++) {
        spin_lock(&spinlock);
        mail++;
        spin_unlock(&spinlock);
    }
    return 0;
}

int thread_function1(void *pv)
{
    while(!kthread_should_stop()) {
        routine();
    }
    return 0;
}
int thread_function2(void *pv)
{
    while(!kthread_should_stop()) {
        routine();
    }
    return 0;
}
static int spinlock_init(void) {
        thread1 = kthread_run(thread_function1,NULL,"Thread1");
        if(thread1) {
            pr_err("Kthread1 Created Successfully...\n");
        } else {
            pr_err("Cannot create kthread1\n");
        }
        thread2 = kthread_run(thread_function2,NULL,"Thread2");
        if(thread2) {
            pr_err("Kthread2 Created Successfully...\n");
        } else {
            pr_err("Cannot create kthread2\n");
        }
        pr_info("Number : %d\n", mail);
        return 0;
}
void spinlock_cleanup(void)
{
    kthread_stop(thread1);
    kthread_stop(thread2);
	pr_info("Device Driver Remove...Done!!\n");
}

MODULE_LICENSE("GPL");
module_init(spinlock_init);
module_exit(spinlock_cleanup);

