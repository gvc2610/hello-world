#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/spinlock.h>
#include <linux/mutex.h>
#include <linux/semaphore.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "locking_demo"

// --- Global Data ---
static int major;

// 1. DATA TO PROTECT
static int fast_irq_counter = 0; // Accessed by ISR (Timer)
static int heavy_processed_data = 0; // Accessed by Workqueue

// 2. LOCKS
static spinlock_t irq_lock;       // For fast_irq_counter (Top Half)
static struct mutex process_lock; // For heavy_processed_data (Bottom Half)
static struct semaphore data_sem; // For signaling "Data Ready" to user

// 3. ASYNC MECHANISMS
static struct timer_list my_timer; // Simulates Hardware Interrupt
static struct work_struct my_work; // The Bottom Half

// --- Bottom Half: Workqueue Handler ---
// Runs in Process Context. CAN SLEEP.
void bottom_half_worker(struct work_struct *work) {
    // THEORY: We use a Mutex here because we might want to sleep 
    // or do heavy operations that take time.
    mutex_lock(&process_lock);
    
    heavy_processed_data++;
    pr_info("LockingDemo: [Bottom Half] Workqueue ran. Processed Data: %d\n", heavy_processed_data);
    
    mutex_unlock(&process_lock);
}

// --- Top Half: Timer Handler (Simulated ISR) ---
// Runs in Atomic Context. CANNOT SLEEP.
void top_half_isr(struct timer_list *t) {
    unsigned long flags;

    // THEORY: We MUST use Spinlock here. Mutex is forbidden in ISR.
    // irqsave disables local interrupts to prevent deadlocks.
    spin_lock_irqsave(&irq_lock, flags);
    fast_irq_counter++;
    spin_unlock_irqrestore(&irq_lock, flags);

    // THEORY: Signal the semaphore. This wakes up any process waiting on down().
    // Unlike Mutex, we don't own it, we just "up" it (increment count).
    up(&data_sem);

    // THEORY: Schedule the Bottom Half to run later
    schedule_work(&my_work);

    // Re-arm timer to fire again in 2 seconds
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(2000));
}

// --- File Operation: Read ---
static ssize_t dev_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset) {
    int ret;
    int data_snapshot;
    char msg[64];
    
    pr_info("LockingDemo: [User] Read called. Waiting for semaphore...\n");

    // THEORY: This puts the user process to SLEEP until the ISR calls up().
    // interruptible means Ctrl+C can kill it.
    if (down_interruptible(&data_sem)) {
        return -ERESTARTSYS;
    }

    // If we are here, the ISR fired and gave us the "key" (semaphore).
    
    // Let's grab the data safely.
    // We can use the spinlock even here to read the fast counter safely.
    spin_lock_irq(&irq_lock);
    data_snapshot = fast_irq_counter;
    spin_unlock_irq(&irq_lock);

    snprintf(msg, sizeof(msg), "ISR Fired! Counter: %d\n", data_snapshot);
    ret = copy_to_user(buffer, msg, strlen(msg));

    return ret ? -EFAULT : strlen(msg);
}

static struct file_operations fops = {
    .read = dev_read,
};

// --- Init ---
static int __init locking_demo_init(void) {
    // 1. Initialize Locks
    spin_lock_init(&irq_lock);
    mutex_init(&process_lock);
    sema_init(&data_sem, 0); // Init to 0 (Locked). Consumer must wait for Producer.

    // 2. Initialize Workqueue
    INIT_WORK(&my_work, bottom_half_worker);

    // 3. Register Device
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) return major;

    // 4. Setup Timer (Simulated Interrupt)
    timer_setup(&my_timer, top_half_isr, 0);
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(2000)); // Fire in 2s

    pr_info("LockingDemo: Module loaded. Major: %d. Timer started.\n", major);
    return 0;
}

// --- Exit ---
static void __exit locking_demo_exit(void) {
    del_timer(&my_timer);       // Stop ISR
    cancel_work_sync(&my_work); // Stop Bottom Half
    unregister_chrdev(major, DEVICE_NAME);
    pr_info("LockingDemo: Module unloaded.\n");
}

module_init(locking_demo_init);
module_exit(locking_demo_exit);
MODULE_LICENSE("GPL");
