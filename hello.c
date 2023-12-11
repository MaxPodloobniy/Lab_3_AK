#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/moduleparam.h>

MODULE_AUTHOR("Max Podloobniy <max.podloobniy@gmail.com>");
MODULE_DESCRIPTION("Lab 3 AK");
MODULE_LICENSE("Dual BSD/GPL");

static uint myCounter = 1;

module_param(myCounter, uint, 0444);
MODULE_PARM_DESC(myCounter, "my counter");

struct my_data {
    struct list_head entry;
    ktime_t timestamp;
};

static LIST_HEAD(my_list);

static int __init my_init(void)
{
    if (myCounter > 10) {
        printk(KERN_WARNING "Error, parameter is bigger than 10)\n");
        return -EINVAL;
    }

    int i;
    for (i = 0; i < myCounter; i++) {
        struct my_data *data = NULL;

        data = kmalloc(sizeof(struct my_data), GFP_KERNEL);
        data->timestamp = ktime_get();
        list_add_tail(&data->entry, &my_list);

        printk(KERN_INFO "\nHello, world!");
    }

    return 0;
}

static void __exit my_exit(void)
{
    int count = 0;
    struct my_data *entry, *tmp;

    list_for_each_entry_safe(entry, tmp, &my_list, entry) {
        pr_info("i: %d; ktime = %lld\n", count++, ktime_to_ns(entry->timestamp));
        list_del(&entry->entry);
        kfree(entry);
    }
    printk(KERN_INFO "Module unloaded\n");
}

module_init(my_init);
module_exit(my_exit);
