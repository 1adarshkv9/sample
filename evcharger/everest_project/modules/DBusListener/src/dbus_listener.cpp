#include "dbus_listener.hpp"
#include <iostream>

// Assuming definition of struct dbus_var_t and other necessary variables

void *dbus_listener(void *arg) {
    struct dbus_var_t dbus_info;

    uint32_t signal_id = 0;
    char signal_id_str[10] = {0};

    // Initialize dbus and add filters
    read_init(&dbus_info);
    add_filter(&dbus_info, SMART_CHARGER_APP_SER_DATA_RCVR);

    while (dbus_listen) {
        if (check_data(&dbus_info) == 0) {
            // Read and process incoming D-Bus data
            char* data = read_data(&dbus_info); // Read D-Bus data
            process_data(data); // Process the received data
            free(data); // Free the allocated memory for data
        }
    }
    return NULL;
}

void handle_msg_from_SMART_CHARGER_APP_module() {
    // Implement handling of messages from SMART CHARGER APP module
    // This function should be implemented based on the requirements of your application
    char get_json_dbus_payload[DATA_SIZE] = {0};
    uint32_t sig_cmd = 0;

    // get sig_cmd
    if(jdata.cmd == NULL)
    {
        log_msg(LOG_WARN, STD_OUT, "No signal command");
        return;
    }

    if(jdata.payload == NULL)
    {
        log_msg(LOG_WARN, STD_OUT, "No signal payload");
        return;
    }

    sprintf(get_json_dbus_payload, "%s", json_object_get_string(jdata.cmd));
    sig_cmd = (int)strtol(get_json_dbus_payload, NULL, 16);
    log_msg(LOG_DEBUG, STD_OUT, "signal cmd : %04x", sig_cmd);

    if(sig_cmd != REQUEST_SIGNAL)
    {
        log_msg(LOG_DEBUG, STD_OUT, "Invalid signal command from SMART CHARGER APP module");
    }

    sprintf(get_json_dbus_payload, "%s", json_object_get_string(jdata.payload));
    log_msg(LOG_INFO, STD_OUT, "D-Bus signal payload : %s", get_json_dbus_payload);
    log_msg(LOG_INFO, STD_OUT, "D-Bus signal payload len : %ld", strlen(get_json_dbus_payload));

    handle_JSON_CAN_request(get_json_dbus_payload, strlen(get_json_dbus_payload));
}

void CAN_dbus_emitter(int signal_cmd) {
    struct dbus_var_t dbus_info;
    char db_payload[DATA_SIZE] = {0};

    dbus_info.obj = CAN_SER_DATA_OBJECT;
    dbus_info.interface = CAN_SER_DATA_INTERFACE;
    dbus_info.sig_name = CAN_SER_DATA_SIG_NAME;

    // Construct and emit D-Bus signals
    write_init(&dbus_info);

    pthread_mutex_lock(&data_lock);

    compose_data(&json_data, db_payload, signal_data);
    write_data(db_payload, dbus_info);

    pthread_mutex_unlock(&data_lock);

    signal_data.payload = NULL;
    free(signal_data.payload);
}
