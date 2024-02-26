#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#define CAN_INTERFACE "can0"
#define CAN_BITRATE "500" // 500 bits per second as a string
#define DATA_SIZE 8 // Specify the size of the data to be transmitted

int main() {
    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;

    // Create a CAN socket
    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (s < 0) {
        perror("socket");
        return 1;
    }

    // Specify the CAN interface
    strcpy(ifr.ifr_name, CAN_INTERFACE);
    ioctl(s, SIOCGIFINDEX, &ifr);

    // Set the CAN interface options
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    // Bind the socket to the CAN interface
    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(s);
        return 1;
    }

    // Set the CAN bitrate using system call to ip tool
    char command[100];
    sprintf(command, "ip link set %s type can bitrate %s", CAN_INTERFACE, CAN_BITRATE);
    if (system(command) < 0) {
        perror("system");
        close(s);
        return 1;
    }

    // Prepare CAN frame
    frame.can_id = 0x123; // Specify the CAN identifier
    frame.can_dlc = DATA_SIZE; // Specify the data length code
    memset(frame.data, 0xAA, DATA_SIZE); // Fill the data field with a pattern (0xAA)

    // Send CAN frame
    int nbytes = write(s, &frame, sizeof(struct can_frame));
    if (nbytes != sizeof(struct can_frame)) {
        perror("write");
        close(s);
        return 1;
    }

    // Close the socket
    close(s);

    printf("CAN frame sent successfully\n");

    return 0;
}

