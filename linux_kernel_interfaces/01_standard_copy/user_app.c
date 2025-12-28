#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd = open("/dev/simple_char", O_RDWR);
    if (fd < 0) {
        perror("Failed to open");
        return -1;
    }

    char tx_buf[] = "Hello from Userspace via copy_from_user!";
    printf("Writing to kernel: %s\n", tx_buf);
    write(fd, tx_buf, strlen(tx_buf));

    char rx_buf[1024] = {0};
    printf("Reading from kernel...\n");
    read(fd, rx_buf, sizeof(rx_buf));
    printf("Read back: %s\n", rx_buf);

    close(fd);
    return 0;
}
