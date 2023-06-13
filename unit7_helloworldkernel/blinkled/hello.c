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

///////////////////////////////////////////////////////////////////////
MODULE_LICENSE("GPL");
MODULE_AUTHOR("AnhTuanVo");
MODULE_DESCRIPTION("A simple Hello world LKM!");
MODULE_VERSION("0.1");

///////////////////////////////////////////////////////////////////////

#define GPIO_ADDR_BASE 0x44E07000
#define ADDR_SIZE (0x1000)
#define GPIO_SETDATAOUT_OFFSET 0x194
#define GPIO_CLEARDATAOUT_OFFSET 0x190
#define GPIO_OE_OFFSET 0x134
#define LED ~(1 << 31)
#define DATA_OUT (1 << 31)

#define GPIO_20 (20)

void __iomem *base_addr;

int init_module(void)
{
    int i = 0;
    uint32_t reg_data = 0;

    // Setting chan GPIO0_31 lam mode output
    base_addr = ioremap(GPIO_ADDR_BASE, ADDR_SIZE);
    reg_data = readl_relaxed(base_addr + GPIO_OE_OFFSET);
    reg_data &= LED;
    writel_relaxed(reg_data, base_addr + GPIO_OE_OFFSET);

    for (i = 10; i > 0; i--) {
        writel_relaxed(DATA_OUT, base_addr + GPIO_SETDATAOUT_OFFSET);
        msleep(1000);
        writel_relaxed(DATA_OUT, base_addr + GPIO_CLEARDATAOUT_OFFSET);
        msleep(1000);
    }

    // if (gpio_is_valid(GPIO_20) == false)
    // {
    //     pr_err("GPIO %d is not valid\n", GPIO_20);
    //     return -1;
    // }

    // if(gpio_request(GPIO_20,"GPIO_20") < 0) {
    //     pr_err("ERROR: GPIO %d request\n", GPIO_20);
    //     return -1;
    // }

    // gpio_direction_output(GPIO_20, 0);
    // gpio_export(GPIO_20, false);

    // for (i = 10; i > 0; i--) {
    //     gpio_set_value(GPIO_20, 1);
    //     msleep(1000);
    //     gpio_set_value(GPIO_20, 0);
    //     msleep(1000);
    // }

    return 0;
}

void cleanup_module(void)
{
    pr_info("hello world driver is unloaded\n");
}

