#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/gpio.h>


struct config
{
    int gpio_num;
    int tan_so;
};
#define MAGIC_NO 100
#define SEND_DATA_CMD _IOW(MAGIC_NO, sizeof(struct config), struct config *)





#define STORAGE_SIZE 100
char storage_data[STORAGE_SIZE];

static bool first;

/// @brief Open device file
/// @param node 
/// @param filep 
/// @return 
int misc_open(struct inode *node, struct file *filep)
{
    first = true;
    pr_info("%s, %d\n", __func__, __LINE__);

    pr_info("misc_open first is %d\n", first);

    return 0;
}

/// @brief Close device file
/// @param node 
/// @param filep 
/// @return 
int misc_release(struct inode *node, struct file *filep)
{
    pr_info("%s, %d\n", __func__, __LINE__);
    first = false;

    return 0;
}

/// @brief Handeler when have read acction on device file
/// @param filp 
/// @param buf 
/// @param count 
/// @param f_pos 
/// @return 
static ssize_t misc_read(struct file *filp, char __user *buf, size_t count,
                         loff_t *f_pos)
{
    int n = 0;
    int max = 0;

    pr_info("misc_read first is %d\n", first);

    if (first == false)
    {
        pr_info("No more data to read\n");
        return 0;
    }

    if (count > strlen(storage_data))
        max = strlen(storage_data);
    else
        max = count;

    n = copy_to_user(buf, storage_data, max);
    first = false;

    return max;
}

// Ham write: ghi du lieu vao vung storage
static ssize_t misc_write(struct file *filp, const char __user *buf,
                          size_t count, loff_t *f_pos)
{
    int n = 0;

    if (first == false)
    {
        pr_info("No more storage to write\n");
        return 0;
    }

    if (count > STORAGE_SIZE)
    {
        pr_info("Write out of max size\n");
        return 0;
    }

    n = copy_from_user(storage_data, buf, count);
    first = false;

    return count;
}

/// @brief Handler when have write acction on devide file.
/// @param filep 
/// @param cmd 
/// @param arg 
/// @return 
static long misc_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
    struct config config_data;

    switch (cmd)
    {
    case SEND_DATA_CMD:
        memset(&config_data, 0, (sizeof(struct config)));
        copy_from_user(&config_data, (struct config *)arg, (sizeof(struct config)));
        pr_info("config_data.gpio_num: %d config_data.tanso: %d\n", config_data.gpio_num, config_data.tan_so);
        break;

    default:
        return -ENOTTY;
    }

    return 0;
}


struct file_operations misc_fops = {
    .owner = THIS_MODULE,
    .open = misc_open,
    .release = misc_release,
    .read = misc_read,
    .write = misc_write,
    .unlocked_ioctl = misc_ioctl,
};

static struct miscdevice misc_example = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "misc_example",
    .fops = &misc_fops,
};

static int misc_init(void)
{
    pr_info("misc module init\n");

    if (misc_register(&misc_example) == 0)
    {
        pr_info("registered device file success\n");
    }

    return 0;
}

static void misc_exit(void)
{
    pr_info("misc module exit\n");
    misc_deregister(&misc_example);
}

module_init(misc_init);
module_exit(misc_exit);

MODULE_AUTHOR("Anh Tuan Vo");
MODULE_DESCRIPTION("Example misc driver.");
MODULE_LICENSE("GPL");
