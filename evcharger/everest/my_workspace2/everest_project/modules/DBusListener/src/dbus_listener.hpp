#ifndef DBUS_LISTENER_HPP
#define DBUS_LISTENER_HPP

#include <pthread.h>
#include <json-c/json.h>

// Define your struct dbus_var_t here if it's not defined elsewhere
struct dbus_var_t {
    // Define the structure members
};

// Function prototypes
void *dbus_listener(void *arg);
void handle_msg_from_SMART_CHARGER_APP_module();
void CAN_dbus_emitter(int signal_cmd);

#endif /* DBUS_LISTENER_HPP */

