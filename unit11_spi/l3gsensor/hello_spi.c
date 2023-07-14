#include <linux/module.h>
#include <linux/printk.h>
#include <linux/spi/spi.h>

static int spi_gyroscope_example_probe(struct spi_device *spi) {
    pr_err("Jay: %s, %d\n", __func__, __LINE__);
    return 0;
}

static int spi_gyroscope_example_exit(struct spi_device *spi) {
    pr_err("Jay: %s, %d\n", __func__, __LINE__);

    return 0;
}


struct of_device_id spi_gyroscope_example_of_match[] = {
	{ .compatible = "st,l3g4200d" },
	{ }
};


static struct spi_driver spi_gyroscope_example_diver = {
    .probe = spi_gyroscope_example_probe,
    .remove = spi_gyroscope_example_exit,
    .driver = {
        .name = "gyroscope_example",
        .owner = THIS_MODULE,
        .of_match_table = spi_gyroscope_example_of_match,
    },
};

module_spi_driver(spi_gyroscope_example_diver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JayNguyen <huuthinh1603@gmail.com>");
MODULE_DESCRIPTION("A simple device driver for gyroscope sensor");
MODULE_VERSION("1.44");