#include <linux/module.h>    
#include <linux/kernel.h>    
#include <linux/init.h>      
#include <linux/delay.h> 

MODULE_LICENSE("GPL");

MODULE_AUTHOR("AnhTuanVo");

MODULE_DESCRIPTION("A simple Hello world LKM!");

MODULE_VERSION("0.1");

int init_module(void)
{
    int i = 0;
    for (i = 0; i < 10; i++)
    {
        printk(KERN_INFO "Hello world. \n");
        mdelay(1000);
    }
    
    return 0;
}

void cleanup_module(void)
{
    printk(KERN_INFO "Goodbye Hello.\n");
}

