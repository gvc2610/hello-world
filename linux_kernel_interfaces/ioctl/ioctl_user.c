#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>

// Define the struct to match the kernel
struct my_data {
    int id;
    int value;
    char description[64];
};

// Define ioctl commands (must match kernel definition)
#define IOCTL_SET_DATA _IOW('x', 1, struct my_data)
#define IOCTL_GET_DATA _IOR('x', 2, struct my_data)

int main() {
    int fd = open("/dev/mydevice", O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return -1;
    }

    struct my_data data_to_send = {42, 100, "Test device data"};
    struct my_data data_received;

    // Send data to the kernel
    if (ioctl(fd, IOCTL_SET_DATA, &data_to_send) == -1) {
        perror("ioctl set data failed");
        close(fd);
        return -1;
    }
    printf("Sent to kernel: id=%d, value=%d, description=%s\n",
           data_to_send.id, data_to_send.value, data_to_send.description);

    // Retrieve data from the kernel
    if (ioctl(fd, IOCTL_GET_DATA, &data_received) == -1) {
        perror("ioctl get data failed");
        close(fd);
        return -1;
    }
    printf("Received from kernel: id=%d, value=%d, description=%s\n",
           data_received.id, data_received.value, data_received.description);

    close(fd);
    return 0;
}
