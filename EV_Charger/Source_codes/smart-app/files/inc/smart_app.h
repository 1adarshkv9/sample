#ifndef DBUS_EMITTER_H
#define DBUS_EMITTER_H

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

#define CAN_ID      0x7E0
#define DATA_SIZE   500
#define SIG_DATA    "{\"request\": {\"port_id\": 2, \"action\": \"set\", \"voltage\": 48.45}}"
#define ACTIVE      "active"
#define INACTIVE    "inactive"
#define STATION_ID  12345

struct json_obj_t json_data;
struct json_obj_t jdata;
struct signal_data_t signal_data;

struct ev_station_properties
{
    char status[20];
    int cause;
    int id;
}station_property;

pthread_mutex_t data_lock;
static volatile unsigned char dbus_listen = 1;
static bool is_req_valid = false;

void smart_app_dbus_emitter(int signal_cmd);
void *dbus_listener();
void handle_msg_from_AWS_MQTT_module();
void handle_msg_from_LCD_module();
void handle_msg_from_CAN_module();
#ifdef __cplusplus
}
#endif

#endif
