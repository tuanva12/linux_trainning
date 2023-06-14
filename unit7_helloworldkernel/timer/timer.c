#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/interrupt.h>


/** variable for timer */
static struct timer_list my_timer;

void timer_callback(struct timer_list *data)
{
    printk(KERN_EMERG "Timer handler.\n");
    my_timer.expires = jiffies + 3 * HZ;
    add_timer(&my_timer);
}

int init_module(void)
{
    printk(KERN_EMERG "Init module.\n");

    my_timer.expires = jiffies + 3 * HZ;
    my_timer.function = timer_callback;
    // timer_setup(&my_timer, timer_callback, 0);
    // mod_timer(&my_timer, jiffies + msecs_to_jiffies(5000));
    add_timer(&my_timer);

    return 0;
}

void cleanup_module(void)
{
    printk(KERN_EMERG "Release module.\n");
    del_timer(&my_timer);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("VOANHTUAN");
MODULE_DESCRIPTION("GPIO led kernel module");