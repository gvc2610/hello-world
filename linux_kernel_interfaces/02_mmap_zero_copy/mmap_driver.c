#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/mm.h>       // remap_pfn_range
#include <linux/slab.h>     // kmalloc

#define DEVICE_NAME "mmap_example"
#define BUF_SIZE (4096 * 4) // 16KB

static int major;
static void *kernel_buffer; 

static int device_mmap(struct file *filp, struct vm_area_struct *vma) {
    unsigned long pfn;
    unsigned long len = vma->vm_end - vma->vm_start;

    if (len > BUF_SIZE) return -EINVAL;

    // Get Physical Frame Number from Kernel Virtual Address
    // INTERVIEW NOTE: virt_to_phys only works on kmalloc/lowmem memory
    pfn = virt_to_phys(kernel_buffer) >> PAGE_SHIFT;

    // INTERVIEW NOTE: remap_pfn_range builds the page tables.
    // It maps User Virtual Addr (vma->vm_start) -> Kernel Physical PFN.
    if (remap_pfn_range(vma, 
                        vma->vm_start, 
                        pfn, 
                        len, 
                        vma->vm_page_prot)) {
        return -EAGAIN;
    }
    return 0;
}

static struct file_operations fops = {
    .mmap = device_mmap,
};

static int __init mmap_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);
    
    // Allocate contiguous physical memory
    kernel_buffer = kmalloc(BUF_SIZE, GFP_KERNEL);
    if (!kernel_buffer) return -ENOMEM;
    
    // Write initial data to prove it's shared
    strcpy(kernel_buffer, "Shared Memory Initialized by Kernel!");
    
    printk(KERN_INFO "MmapDriver: Loaded. Create node with: mknod /dev/%s c %d 0\n", DEVICE_NAME, major);
    return 0;
}

static void __exit mmap_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    kfree(kernel_buffer);
}

module_init(mmap_init);
module_exit(mmap_exit);
MODULE_LICENSE("GPL");
