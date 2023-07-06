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

//Dang ky thong tin device tree node ma driver muon dieu khien
static const struct of_device_id blink_led_of_match[] = {
    {.compatible = "led-example0"},
    {.compatible = "led-example1"},
    {.compatible = "led-example2"},
    {.compatible = "led-example3"},
    {},
};

// Trong ham probe nhap nhay led luon
int blink_led_driver_probe(struct platform_device *pdev)
{
    pr_info("led probe\n");

    return 0;
}

int blink_led_driver_remove(struct platform_device *pdev)
{
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
