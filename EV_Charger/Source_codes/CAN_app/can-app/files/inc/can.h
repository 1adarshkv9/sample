#ifndef CAN_H
#define CAN_H

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "log.h"

enum can_error_states{
	CAN_SUCCESS = 0,
	CAN_TIMEOUT = -1,
	CAN_CLOSE_TIMEOUT = -2,
	CAN_OPEN_SOC_ERROR = -3,
	CAN_OPEN_SOC_BIND_ERROR = -4,
	CAN_SOC_SET_TIMEOUT_ERROR = -5,
	CAN_CLOSE_ERROR = -6,
	CAN_CLOSE_FD_ALREADY_CLOSED = -7,
	CAN_WRITE_ERROR = -8,
	CAN_READ_ERROR = -9,
	CAN_UDS_WRITE_ERROR = -10,
};

int open_canbus_socket(const char *ifname);
int write_canbus_frame(char fd, struct can_frame frame);
int read_canbus_frame(char fd, struct can_frame *frame);
int close_canbus_socket(char fd);

#endif
