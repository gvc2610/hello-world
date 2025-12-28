#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE (4096 * 4)

int main() {
    int fd = open("/dev/mmap_example", O_RDWR);
    if (fd < 0) {
        perror("Open failed (Did you mknod?)");
        return -1;
    }

    // Map the kernel buffer into our process address space
    char *ptr = mmap(NULL, BUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    if (ptr == MAP_FAILED) {
        perror("mmap failed");
        return -1;
    }

    printf("1. Reading directly from memory: %s\n", ptr);

    printf("2. Writing directly to memory...\n");
    sprintf(ptr, "This message was written by Userspace via MMAP!");

    // No read/write syscalls needed!
    printf("3. Done. Check dmesg or read again to confirm kernel sees it.\n");
    
    munmap(ptr, BUF_SIZE);
    close(fd);
    return 0;
}
