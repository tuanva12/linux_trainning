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

#define GPIO_31 (31)

int init_module(void)
{
    int i = 10;
    if (gpio_is_valid(GPIO_31) == false)
    {
        pr_err("GPIO %d is not valid\n", GPIO_31);
        return -1;
    }

    if(gpio_request(GPIO_31,"GPIO_31") < 0) {
        pr_err("ERROR: GPIO %d request\n", GPIO_31);
        return -1;
    }

    gpio_direction_output(GPIO_31, 0);
    gpio_export(GPIO_31, false);

    while(i--) {
        gpio_set_value(GPIO_31, 1);
        msleep(1000);
        gpio_set_value(GPIO_31, 0);
        msleep(1000);
    }

    return 0;
}

void cleanup_module(void)
{
    gpio_unexport(GPIO_31);
    gpio_free(GPIO_31);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("VOANHTUAN");
MODULE_DESCRIPTION("GPIO led kernel module");