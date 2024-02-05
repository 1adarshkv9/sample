#include "../inc/smart_app.h"

void smart_app_dbus_emitter(int signal_cmd)
{
    struct dbus_var_t dbus_info;
    char db_payload[DATA_SIZE] = {0};

    dbus_info.obj = SMART_CHARGER_APP_SER_DATA_OBJECT;
    dbus_info.interface = SMART_CHARGER_APP_SER_DATA_INTERFACE;
    dbus_info.sig_name = SMART_CHARGER_APP_SER_DATA_SIG_NAME;

    signal_data.cmd = signal_cmd;

    write_init(&dbus_info);

    pthread_mutex_lock(&data_lock);

    compose_data(&json_data, db_payload, signal_data);
    log_msg(LOG_INFO, STD_OUT, "db_payload Check: %s", db_payload);
    write_data(db_payload, dbus_info);
    
    pthread_mutex_unlock(&data_lock);

    signal_data.payload = NULL;
    free(signal_data.payload);
}

int main(int argc, char* argv[])
{
    int ret = 0;
    pthread_t db_listener;

    signal_data.signal_id = SMART_CHARGER_APP_SER_SIGNAL_ID;
    signal_data.can_id = CAN_ID;
    signal_data.cmd = 0;
    
    station_property.cause = 0;
    station_property.id = STATION_ID;
    strcpy(station_property.status, ACTIVE);

    // Thread for D-Bus listener
    ret = pthread_create(&db_listener, NULL, &dbus_listener, NULL);
    if(ret != 0)
    {
        log_msg(LOG_ERR, STD_ERR, "Error: dbus_listener thread creation Failed!, Error_code:[%d]\n", ret);
        exit(EXIT_FAILURE);
    }

    // Waiting for the created thread to terminate
    pthread_join(db_listener, NULL);
    return 0;
}


void *dbus_listener()
{
    struct dbus_var_t dbus_info;

    uint32_t signal_id = 0;
    char signal_id_str[10] = {0};

    //init dbus and add filters
    read_init(&dbus_info);
    add_filter(&dbus_info, CAN_SER_DATA_RCVR);
    add_filter(&dbus_info, LCD_SER_DATA_RCVR);
    add_filter(&dbus_info, AWS_MQTT_APP_SER_DATA_RCVR);

    while(dbus_listen)
    {
        if(check_data(&dbus_info) == 0)
        {
            jdata = read_data(&dbus_info);

            json_object_object_get_ex(jdata.obj, "sig_id", &jdata.sig);
            json_object_object_get_ex(jdata.obj, "dlc", &jdata.dlc);
            json_object_object_get_ex(jdata.obj, "can_id", &jdata.can_id);
            json_object_object_get_ex(jdata.obj, "cmd", &jdata.cmd);
            json_object_object_get_ex(jdata.obj, "payload", &jdata.payload);

            // get signal_id
            if(jdata.sig != NULL)
            {
                sprintf(signal_id_str, "%s", json_object_get_string(jdata.sig));
                signal_id = (int)strtol(signal_id_str, NULL, 16);
                log_msg(LOG_DEBUG, STD_OUT, "signal id : %04x", signal_id);
            }
            switch(signal_id)
            {
                case CAN_SER_SIGNAL_ID:
                    log_msg(LOG_DEBUG, STD_OUT, "Msg from CAN Module");
                    handle_msg_from_CAN_module();
                    break;
                case LCD_SER_SIGNAL_ID:
                    log_msg(LOG_DEBUG, STD_OUT, "Msg from LCD Module");
                    handle_msg_from_LCD_module();
                    break;
                case AWS_MQTT_APP_SER_SIGNAL_ID:
                    log_msg(LOG_DEBUG, STD_OUT, "Msg from AWS_MQTT Module");
                    handle_msg_from_AWS_MQTT_module();
                    break;
                default:
                    log_msg(LOG_DEBUG, STD_OUT, "IN the default switch case");
                    break;
            }
        }
    }
    //pthread_exit(NULL);
    return NULL;
}

void handle_msg_from_AWS_MQTT_module()
{
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
        log_msg(LOG_DEBUG, STD_OUT, "Invalid signal command from AWS_MQTT module");
    }

    sprintf(get_json_dbus_payload, "%s", json_object_get_string(jdata.payload));
    log_msg(LOG_INFO, STD_OUT, "D-Bus signal payload : %s", get_json_dbus_payload);
    log_msg(LOG_INFO, STD_OUT, "D-Bus signal payload len : %ld", strlen(get_json_dbus_payload));

    json_object *jobj = json_tokener_parse(get_json_dbus_payload);

    if(jobj == NULL)
    {
        log_msg(LOG_ERR, STD_ERR, "Invalid JSON string");

        // Sending Invalid AWS_MQTT request JSON format error
        signal_data.payload = (char *)calloc(DATA_SIZE, sizeof(char));
        sprintf(signal_data.payload, "{\"ev_station\":{\"status\":\"%s\",\"cause\":%d,\"id\":%d,\"event\":{\"type\":\"error\",\"trigger_from\":\"ev_station\",\"is_request\":false,\"is_err\":true,\"err_code\":%d,\"request\":null,\"reponse\":null}}}", station_property.status, station_property.cause, station_property.id, SMART_APP_INVALID_JSON);
        log_msg(LOG_INFO, STD_OUT, "%s\n", signal_data.payload);
        smart_app_dbus_emitter(ERROR_SIGNAL);

        is_req_valid = false;
        return;
    }

    log_msg(LOG_DEBUG, STD_OUT, "A valid JSON string is passed");
    is_req_valid = true;
    //json_parse(jobj);
}

void handle_msg_from_LCD_module()
{

}

void handle_msg_from_CAN_module()
{

}

