#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int fd = open("/dev/locking_demo", O_RDONLY);
    if (fd < 0) {
        perror("Failed to open device");
        printf("Did you run mknod? Check dmesg for the Major number.\n");
        return -1;
    }

    printf("--- User Space Locking Tester ---\n");
    printf("I will now try to read from the device.\n");
    printf("Since the Semaphore starts at 0, I should BLOCK (sleep) here...\n");
    printf("...waiting for the Kernel Timer (ISR) to wake me up (every 2s).\n\n");

    char buf[100];
    
    // Loop 3 times to prove it works repeatedly
    for(int i=0; i<3; i++) {
        printf("[App] Calling read()... (Simulating waiting for HW Interrupt)\n");
        
        int ret = read(fd, buf, sizeof(buf)-1);
        
        if (ret > 0) {
            buf[ret] = '\0';
            printf("[App] WOKE UP! Kernel said: %s", buf);
        } else {
            perror("Read failed");
        }
        printf("----------------------------------------\n");
    }

    close(fd);
    return 0;
}
