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
    log_msg(LOG_INFO, STD_OUT, "CAN Frame is:");
    printf("%s\t%X\t%X\t", CAN_IFNAME, frame.can_dlc, frame.can_id);
    for(int i = 0; i < CAN_MAX_DLEN; i++)
        printf("%X ", frame.data[i]);
    printf("\n");
}

void convert_CAN_to_JSON() {
    // Convert CAN frame to JSON
        char resp_type[20] = {0};
    int port_id = 0;
    int port_state = 0;
    int err_code = 0;
    float voltage = 0.0;
    float current = 0.0;

    signal_data.payload = (char *)calloc(DATA_SIZE, sizeof(char));

    port_id = rx_frame.data[frame_port] & 0x0F;
    port_state = (rx_frame.data[frame_port] >> 4) & 0x0F;

    log_msg(LOG_DEBUG, STD_OUT, "port_id = %d", port_id);
    log_msg(LOG_DEBUG, STD_OUT, "port_state = %d", port_state);

    switch(rx_frame.data[frame_type])
    {
        case CAN_frame_type_request:
            log_msg(LOG_WARN, STD_OUT, "Request Frame received");
            break;

        case CAN_frame_type_resp_pos:
            log_msg(LOG_DEBUG, STD_OUT, "Response frame received");
            strcpy(resp_type, "response");
            
            voltage = ((((float)rx_frame.data[2])*100) + (rx_frame.data[3]%100))/100;
            current = ((((float)rx_frame.data[4])*100) + (rx_frame.data[5]%100))/100;
            
            /*if(port_state)
            {
                voltage = ((((float)rx_frame.data[2])*100) + (rx_frame.data[3]%100))/100;
                current = ((((float)rx_frame.data[4])*100) + (rx_frame.data[5]%100))/100;
            }
            else
            {
                err_code = rx_frame.data[2];
            }*/
            break;

        case CAN_frame_type_resp_neg:
            log_msg(LOG_DEBUG, STD_OUT, "Negative Response frame received");
            strcpy(resp_type, "response");

            err_code = rx_frame.data[2];
            break;

        case CAN_frame_type_status:
            log_msg(LOG_DEBUG, STD_OUT, "Status frame received");
            strcpy(resp_type, "status");

            voltage = ((((float)rx_frame.data[2])*100) + (rx_frame.data[3]%100))/100;
            current = ((((float)rx_frame.data[4])*100) + (rx_frame.data[5]%100))/100;
            
            /*if(port_state)
            {
                voltage = ((((float)rx_frame.data[2])*100) + (rx_frame.data[3]%100))/100;
                current = ((((float)rx_frame.data[4])*100) + (rx_frame.data[5]%100))/100;
            }
            else
            {
                err_code = rx_frame.data[2];
            }*/
            break;

        default:
            log_msg(LOG_INFO, STD_OUT, "Invalid frame is received");
            // Free the signal_data.payload variable memory and return from the function with sending any signal on D-Bus
            signal_data.payload = NULL;
            free(signal_data.payload);
            return;
            //break;
    }

    sprintf(signal_data.payload, "{\"reponse\": {\"port_id\": %d, \"resp_type\": \"%s\", \"is_err\": %s, \"err_code\": %d, \"voltage\": %.2f, \"current\": %.2f}}",
                                                                               port_id, resp_type, (err_code ? "true" : "false"), err_code, voltage, current);

    log_msg(LOG_INFO, STD_OUT, "%s\n", signal_data.payload);
    CAN_dbus_emitter(RESPONSE_SIGNAL);
}

