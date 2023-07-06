/****************************************************************************************
 * Include Library
 ****************************************************************************************/
#include <linux/module.h> /* Needed by all modules */
#include <linux/kernel.h> /* Needed for KERN_INFO */
#include <linux/time.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/mod_devicetable.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/miscdevice.h>

/****************************************************************************************
 * Definition
 ****************************************************************************************/
#define GPIO_SETDATAOUT_OFFSET 0x194
#define GPIO_CLEARDATAOUT_OFFSET 0x190
#define GPIO_OE_OFFSET 0x134


typedef struct led_config_t
{
    uint32_t led;
    uint32_t data_out;
    uint32_t gpio;
    uint32_t delta_time;
} led_config_t;

/* Lưu dữ liệu tổng của led */
struct led_driver_data
{
    void __iomem *base_addr;
    struct led_config_t led_config;
    char name[128];
};

struct container
{
    struct platform_device old_pdev;
    struct led_driver_data *led_data;
    struct timer_list my_timer;
    uint32_t count;
};

/****************************************************************************************
 * Variable
 ****************************************************************************************/

//Dang ky thong tin device tree node ma driver muon dieu khien
static const struct of_device_id blink_led_of_match[] = {
    {.compatible = "led-example0"},
    {},
};

struct container *my_container = NULL;

/****************************************************************************************
 * Function implementation
 ****************************************************************************************/

// Ham xu ly timer
static void timer_function(struct timer_list *t)
{
    struct container *container_ptr = container_of(t, struct container, my_timer);

    struct led_driver_data *this_led = container_ptr->led_data;

    pr_info("delay time is %d\n", this_led->led_config.delta_time);

    // Nhap nhay led
    if ((container_ptr->count % this_led->led_config.delta_time) == 0) {
        pr_info("led on\n");
        writel_relaxed(this_led->led_config.data_out, my_container->led_data->base_addr + GPIO_SETDATAOUT_OFFSET);
    }
    else {
        pr_info("led off\n");
        writel_relaxed(this_led->led_config.data_out, my_container->led_data->base_addr + GPIO_CLEARDATAOUT_OFFSET);
    }

    container_ptr->count++;
    mod_timer(&(container_ptr->my_timer), jiffies + HZ);
}

// Trong ham probe nhap nhay led luon
int blink_led_driver_probe(struct platform_device *pdev)
{
    const struct device *dev = &pdev->dev;
    const struct device_node *np = dev->of_node;
    struct resource *res = NULL;
    uint32_t led_config[4];
    uint32_t reg_data = 0;

    if(dev == NULL)
    {
        pr_info("dev null\n");
	      return -1;
    }

    if(np == NULL)
    {
        pr_info("np null\n");
	      return -1;
    }


    pr_info("probe is called\n");

    /* Allocate memory */
    my_container = kmalloc(sizeof(struct container), GFP_KERNEL);
    my_container->led_data = kmalloc(sizeof(struct led_driver_data), GFP_KERNEL);

    if(my_container == NULL)
    {
        pr_info("my container null\n");
        return -1;
    }

    if(my_container->led_data == NULL)
    {
        pr_info("my_container->led_data null\n");
        return -1;
    }

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    pr_info("res->start: %08x res->end: %08x\n", res->start, res->end);

    my_container->led_data->base_addr = (uint32_t *)ioremap(res->start, (res->end - res->start));

    if (my_container->led_data->base_addr == NULL)
    {
        pr_info("could not request base address\n");
        return -1;
    }

    if (0 != of_property_read_u32_array(np, "led_config", led_config, 4))
    {
        pr_info("read data node failed\n");
        return -1;
    }

    pr_info("led_config: %08x %08x %08x %08x\n", led_config[0], led_config[1], led_config[2], led_config[3]);

    my_container->led_data->led_config.led = led_config[0];
    my_container->led_data->led_config.data_out = led_config[1];
    my_container->led_data->led_config.gpio = led_config[2];
    my_container->led_data->led_config.delta_time = led_config[3];

    //Setting output mode for gpio pin
    reg_data = readl_relaxed(my_container->led_data->base_addr + GPIO_OE_OFFSET);
    reg_data &= ~my_container->led_data->led_config.led;

    writel_relaxed(reg_data, my_container->led_data->base_addr + GPIO_OE_OFFSET);

    //Setup timer de nhap nhay led
    my_container->my_timer.expires = jiffies + HZ;
    my_container->my_timer.function = timer_function;
    my_container->old_pdev = *pdev;
    timer_setup(&(my_container->my_timer), timer_function, 0);
    add_timer(&(my_container->my_timer));

    return 0;
}

int blink_led_driver_remove(struct platform_device *pdev)
{
  /*  struct container *my_container = container_of(pdev, struct container, old_pdev);*/

    if (NULL != my_container)
    {
        del_timer_sync(&(my_container->my_timer));

        /* Off led */
        writel_relaxed(my_container->led_data->led_config.data_out, my_container->led_data->base_addr + GPIO_CLEARDATAOUT_OFFSET);

        kfree(my_container->led_data);
        kfree(my_container);
    }

    pr_info("driver remove\n");

    return 0;
}

static struct platform_driver blink_led_driver = {
    .probe = blink_led_driver_probe,
    .remove = blink_led_driver_remove,
    .driver = {
        .name = "blink_led",
        .of_match_table = blink_led_of_match,
    },
};

module_platform_driver(blink_led_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Blink Led kernel module");
