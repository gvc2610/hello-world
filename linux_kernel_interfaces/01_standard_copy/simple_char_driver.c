#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h> // copy_to_user, copy_from_user
#include <linux/slab.h>    // kmalloc

#define DEVICE_NAME "simple_char"
#define BUF_SIZE 1024

static int major_num;
static char *kernel_buffer;

static ssize_t device_write(struct file *filp, const char __user *buff, size_t len, loff_t *off) {
    if (len > BUF_SIZE - 1) return -EINVAL;
    
    // INTERVIEW NOTE: copy_from_user returns 0 on success.
    // It returns >0 (bytes not copied) if a page fault fails or bad address.
    if (copy_from_user(kernel_buffer, buff, len)) {
        return -EFAULT;
    }
    
    kernel_buffer[len] = '\0'; // Null terminate
    printk(KERN_INFO "SimpleChar: Received %zu bytes: %s\n", len, kernel_buffer);
    return len;
}

static ssize_t device_read(struct file *filp, char __user *buff, size_t len, loff_t *off) {
    char *msg_ptr;
    
    if (*off >= strlen(kernel_buffer)) return 0; // EOF

    msg_ptr = kernel_buffer + *off;
    if (len > strlen(msg_ptr)) {
        len = strlen(msg_ptr);
    }

    // INTERVIEW NOTE: We use copy_to_user, not memcpy!
    if (copy_to_user(buff, msg_ptr, len)) {
        return -EFAULT;
    }

    *off += len;
    return len;
}

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
};

static int __init simple_init(void) {
    kernel_buffer = kmalloc(BUF_SIZE, GFP_KERNEL);
    if (!kernel_buffer) return -ENOMEM;
    
    major_num = register_chrdev(0, DEVICE_NAME, &fops);
    printk(KERN_INFO "SimpleChar: Registered with major %d\n", major_num);
    return 0;
}

static void __exit simple_exit(void) {
    unregister_chrdev(major_num, DEVICE_NAME);
    kfree(kernel_buffer);
    printk(KERN_INFO "SimpleChar: Exiting\n");
}

module_init(simple_init);
module_exit(simple_exit);
MODULE_LICENSE("GPL");
