#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/kernel.h>

#define DEVICE_NAME "mydevice"
#define IOCTL_SET_DATA _IOW('x', 1, struct my_data)
#define IOCTL_GET_DATA _IOR('x', 2, struct my_data)

// Define a struct to be transferred between user and kernel space
struct my_data {
    int id;
    int value;
    char description[64];
};

static struct my_data device_data; // Kernel-side data storage
static struct cdev my_cdev;
static dev_t dev_num;

// ioctl handler
static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    struct my_data user_data;

    switch (cmd) {
        case IOCTL_SET_DATA:
            // Copy struct from user space to kernel space
            if (copy_from_user(&user_data, (struct my_data __user *)arg, sizeof(user_data))) {
                pr_err("Failed to copy data from user\n");
                return -EFAULT;
            }

            // Save the data in the kernel
            device_data = user_data;
            pr_info("Received from user: id=%d, value=%d, description=%s\n",
                    device_data.id, device_data.value, device_data.description);
            break;

        case IOCTL_GET_DATA:
            // Copy kernel data back to user space
            if (copy_to_user((struct my_data __user *)arg, &device_data, sizeof(device_data))) {
                pr_err("Failed to copy data to user\n");
                return -EFAULT;
            }

            pr_info("Sent to user: id=%d, value=%d, description=%s\n",
                    device_data.id, device_data.value, device_data.description);
            break;

        default:
            pr_err("Invalid ioctl command\n");
            return -EINVAL;
    }

    return 0;
}

// File operations structure
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = my_ioctl,
};

// Initialize the module and register the device
static int __init my_driver_init(void) {
    // Allocate a device number
    if (alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME) < 0) {
        pr_err("Failed to allocate device number\n");
        return -1;
    }

    // Initialize character device
    cdev_init(&my_cdev, &fops);
    if (cdev_add(&my_cdev, dev_num, 1) < 0) {
        pr_err("Failed to add cdev\n");
        unregister_chrdev_region(dev_num, 1);
        return -1;
    }

    pr_info("Driver initialized: major=%d, minor=%d\n",
            MAJOR(dev_num), MINOR(dev_num));
    return 0;
}

// Cleanup the module
static void __exit my_driver_exit(void) {
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);
    pr_info("Driver removed\n");
}

module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Example ioctl driver with struct support");
