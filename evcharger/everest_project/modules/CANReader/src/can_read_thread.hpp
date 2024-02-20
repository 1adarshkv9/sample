#ifndef CAN_READ_THREAD_HPP
#define CAN_READ_THREAD_HPP

#include <pthread.h>
#include <linux/can.h>

// Define your struct can_json_req and other necessary variables if not defined elsewhere
struct can_json_req {
    // Define the structure members
};

// Function prototypes
void *CAN_read_thread(void *arg);
int read_canbus_frame(int fd, struct can_frame *frame);
void show_can_frame(struct can_frame frame);
void convert_CAN_to_JSON();

#endif /* CAN_READ_THREAD_HPP */

