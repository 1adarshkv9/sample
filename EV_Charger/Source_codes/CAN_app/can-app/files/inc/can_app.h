#ifndef CAN_APP_H
#define CAN_APP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <dbus/dbus.h>
#include <dbus_interface.h>
#include <dbus_wrapper.h>
#include <json-c/json.h>
#include <json-c/json_object.h>
#include <ctype.h>
#include <errno.h>

#include "log.h"
#include "can.h"

#define DATA_SIZE    500

//#define SIG_DATA    "{\"request\": {\"port_id\": 123, \"action\": \"set\", \"voltage\": 48.45}}"
//#define CAN_JSON_Req_string     "{\"request\": {\"port_id\": 1, \"action\": \"set\", \"voltage\": 48.45}}"
//#define CAN_JSON_resp_string    "{\"reponse\": {\"port_id\": 1, \"resp_type\": \"response\", \"is_err\": fasle, \"err_code\": 0, \"voltage\": 13.19, \"current\": 8.18}}"

#define CAN_ID      0x7E0
#define CAN_DLC     8
#define CAN_IFNAME  "can0"

#define PORT_ID     "port_id"
#define PORT_ON     0x01
#define PORT_OFF    0x00

#define ACTION      "action"
#define SET         "set"
#define RESET       "reset"

#define VOLTAGE     "voltage"

/*
Minimal valid JSON objects are as below:
The empty object '{}'
The empty array '[]'
The string that is empty '""'
A number e.g. '123.4'
The boolean value true 'true'
The boolean value false 'false'
The null value 'null'
*/

enum CAN_frame_type
{
    CAN_frame_type_request = 0x10,
    CAN_frame_type_resp_pos = 0x60,
    CAN_frame_type_resp_neg = 0x6F,
    CAN_frame_type_status = 0x11
};

enum CAN_frame_indices
{
    frame_type = 0,
    frame_port = 1,
    volt_i = 2,
    volt_d = 3,
    curr_i = 4,
    curr_d = 5
};

struct can_json_req
{
    int port_id;
    char *action;
    float voltage;
}__attribute__((packed));

struct can_json_req can_req_st;

int fd_can;
struct can_frame tx_frame;
struct can_frame rx_frame;

static volatile unsigned char read_can_bus = 1;
static volatile unsigned char dbus_listen = 1;
static bool is_req_sent = false;
static bool is_req_valid = false;

struct json_obj_t jdata;
struct json_obj_t json_data;
struct signal_data_t signal_data;

pthread_mutex_t data_lock;

void prepare_can_frame(int can_id, uint8_t can_dlc, uint8_t data[CAN_MAX_DLEN], struct can_frame *frame);
void show_can_frame(struct can_frame frame);
void print_st(struct can_json_req *st);
int valid_por(int p_id);
void send_CAN_frame(struct can_json_req *st);
void add_key_value_pair_to_st(char *k, void *v);
void json_parse(json_object *jobj);
void handle_msg_from_SMART_CHARGER_APP_module();
void handle_JSON_CAN_request(char *payload, int len);
void *CAN_read_thread();
void *dbus_listener();

#ifdef __cplusplus
}
#endif

#endif
