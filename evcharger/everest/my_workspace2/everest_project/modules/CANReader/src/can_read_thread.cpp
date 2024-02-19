#include "can_read_thread.hpp"
#include <iostream>

// Define your struct can_json_req and other necessary variables if not defined elsewhere

void *CAN_read_thread(void *arg) {
    while (read_can_bus) {
        struct can_frame rx_frame;
        if (read_canbus_frame(fd_can, &rx_frame) > 0) {
            // TODO: If the frame ID is 7E1, then only process it. Otherwise, ignore
            show_can_frame(rx_frame);
            convert_CAN_to_JSON();
        }
        //sleep(1);
        usleep(250);
    }
    return NULL;
}

int read_canbus_frame(int fd, struct can_frame *frame) {
    int ret = 0;
    int nbytes = 0;

    nbytes = read(fd, frame, sizeof(struct can_frame));
    if (nbytes < 2) {
        // Handle the error appropriately
        ret = CAN_READ_ERROR;
    } else {
        // Handle successful read
        ret = nbytes;
    }

    return ret;
}

void show_can_frame(struct can_frame frame) {
    // Display CAN frame information
}

void convert_CAN_to_JSON() {
    // Convert CAN frame to JSON
    // Implementation is missing and needs to be added based on your requirements
}

