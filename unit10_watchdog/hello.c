#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/watchdog.h>
#include <linux/pm_runtime.h>

#define DRIVER_NAME "imx2-wdt"

#define PTV 2
#define GET_WLDR_VAL(secs) (0xffffffff - ((secs) * (32768 / (1 << PTV))) + 1)
#define BBB_MAX_TIMEOUT (24 * 60 * 60) /* 1 day */
#define BBB_MIN_TIMEOUT 1              /* 1 sec */
#define BBB_TIMEOUT_DEFAULT 60         /* 60 secs */

#define WDT_WIDR (0x00)
#define WDT_WDSC (0x10)
#define WDT_WDST (0x14)
#define WDT_WISR (0x18)
#define WDT_WIER (0x1C)
#define WDT_WCLR (0x24)
#define WDT_WCRR (0x28)
#define WDT_WLDR (0x2C)
#define WDT_WTGR (0x30)
#define WDT_WWPS (0x34)
#define WDT_WDLY (0x44)
#define WDT_WSPR (0x48)

struct bbb_wdt_dev
{
    void __iomem *base; /* virtual */
    bool is_active_wdt;
    int wdt_trigg_val;
    struct device *dev;
};

struct bbb_wdt_dev *wdev_data = NULL;

static void bbb_wdt_enable(struct bbb_wdt_dev *wdev)
{
    void __iomem *base = wdev->base;

    iowrite32(0xBBBB, (base + WDT_WSPR));
    while (readl_relaxed(base + WDT_WWPS) & (1 << 4))
        cpu_relax();

    iowrite32(0x4444, (base + WDT_WSPR));
    while (readl_relaxed(base + WDT_WWPS) & (1 << 4))
        cpu_relax();
}

static void bbb_wdt_disable(struct bbb_wdt_dev *wdev)
{
    void __iomem *base = wdev->base;

    iowrite32(0xAAAA, (base + WDT_WSPR));
    while (readl_relaxed(base + WDT_WWPS) & (1 << 4))
        cpu_relax();

    iowrite32(0x5555, (base + WDT_WSPR));
    while (readl_relaxed(base + WDT_WWPS) & (1 << 4))
        cpu_relax();
}

static void bbb_wdt_reload(struct bbb_wdt_dev *wdev)
{
    void __iomem *base = wdev->base;

    wdev->wdt_trigg_val = ~wdev->wdt_trigg_val;

    /* wait for posted write to complete */
    while ((readl_relaxed(base + WDT_WWPS)) & (1 << 3))
        cpu_relax();

    iowrite32(wdev->wdt_trigg_val, (base + WDT_WTGR));

    /* wait for posted write to complete */
    while ((readl_relaxed(base + WDT_WWPS)) & (1 << 3))
        cpu_relax();
}

static void bbb_wdt_set_timer(struct bbb_wdt_dev *wdev, unsigned int timeout)
{
    void __iomem *base = wdev->base;
    int load_val = GET_WLDR_VAL(timeout);

    while (readl_relaxed(base + WDT_WWPS) & (1 << 2))
        cpu_relax();
    iowrite32(load_val, (base + WDT_WLDR));

    while (readl_relaxed(base + WDT_WWPS) & (1 << 2))
        cpu_relax();
}

static void bbb_wdt_setup(struct bbb_wdt_dev *wdev, unsigned int timeout)
{
    void __iomem *base = wdev->base;

    bbb_wdt_disable(wdev);

    /* set prescale */
    while (readl_relaxed(base + WDT_WWPS) & 0x01)
        cpu_relax();

    /* PTV = 2 */
    iowrite32((1 << 5) | (1 << 2), base + WDT_WCLR);

    bbb_wdt_set_timer(wdev, timeout);
    bbb_wdt_reload(wdev);
    bbb_wdt_enable(wdev);

    wdev->is_active_wdt = true;
}

static int imx2_wdt_start(struct watchdog_device *wdog)
{
    pr_info("imx2_wdt_start\n");

    pm_runtime_get_sync(wdev_data->dev);
    bbb_wdt_setup(wdev_data, 3600);

    return 0;
}

static int imx2_wdt_ping(struct watchdog_device *wdog)
{
    pr_info("imx2_wdt_ping\n");

    bbb_wdt_reload(wdev_data);

    return 0;
}

static int imx2_wdt_set_timeout(struct watchdog_device *wdog, unsigned int timeout)
{
    pr_info("imx2_wdt_set_timeout\n");

    bbb_wdt_setup(wdev_data, timeout);

    return 0;
}

static int imx2_wdt_stop(struct watchdog_device *wdog)
{
    pr_info("imx2_wdt_stop\n");

    bbb_wdt_disable(wdev_data);

    return 0;
}

static const struct watchdog_ops imx2_wdt_ops = {
    .owner = THIS_MODULE,
    .start = imx2_wdt_start,
    .ping = imx2_wdt_ping,
    .set_timeout = imx2_wdt_set_timeout,
    .stop = imx2_wdt_stop,
};

static const struct watchdog_info omap_wdt_info = {
    .options = WDIOF_SETTIMEOUT | WDIOF_KEEPALIVEPING,
    .identity = "OMAP Watchdog",
};

static struct watchdog_device wdog = {
    .ops = &imx2_wdt_ops,
    .min_timeout = 1,
    .max_timeout = 0x7fff,
    .timeout = 60,
    .pretimeout = 0,
    .info = &omap_wdt_info,
};

static int imx2_wdt_probe(struct platform_device *pdev)
{
    int ret = 0;
    struct resource *res;

    wdev_data = devm_kzalloc(&pdev->dev, sizeof(struct bbb_wdt_dev), GFP_KERNEL);
    wdev_data->is_active_wdt = false;
    wdev_data->wdt_trigg_val = 0x1998;

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    wdev_data->base = devm_ioremap_resource(&pdev->dev, res);

    wdev_data->dev = &pdev->dev;
    pm_runtime_enable(wdev_data->dev);
    pm_runtime_get_sync(wdev_data->dev);
    pm_runtime_put(wdev_data->dev);

    ret = watchdog_register_device(&wdog);

    return 0;
}

static int imx2_wdt_remove(struct platform_device *pdev)
{
    watchdog_unregister_device(&wdog);

    return 0;
}

static const struct of_device_id imx2_wdt_dt_ids[] = {
    {
        .compatible = "omap,my_watchdog",
    },
    {/* sentinel */}};
MODULE_DEVICE_TABLE(of, imx2_wdt_dt_ids);

static struct platform_driver imx2_wdt_driver = {
    .probe = imx2_wdt_probe,
    .remove = imx2_wdt_remove,
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = imx2_wdt_dt_ids,
    },
};

module_platform_driver_probe(imx2_wdt_driver, imx2_wdt_probe);

MODULE_AUTHOR("Wolfram Sang");
MODULE_DESCRIPTION("Watchdog driver for IMX2 and later");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:" DRIVER_NAME);