





int open_canbus_socket(const char *ifname)
{

	uint8_t fd;
	int ret;
	struct timeval tv;
	struct sockaddr_can addr;
	struct ifreq ifr;
	
	if((fd = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
	{
		log_msg(LOG_ERR, STD_ERR, "Fail to create CAN Socket");
		ret = CAN_OPEN_SOC_ERROR;
		return ret;
	}
	else
	{
		strcpy(ifr.ifr_name, ifname);
		ioctl(fd, SIOCGIFINDEX, &ifr);
		addr.can_family  = AF_CAN;
		addr.can_ifindex = ifr.ifr_ifindex;
		
		log_msg(LOG_DEBUG, STD_OUT, "%s at index %d", ifname, ifr.ifr_ifindex);
		
		if(bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		{ 
			log_msg(LOG_ERR, STD_ERR, "CAN socket Bind error");
			ret = CAN_OPEN_SOC_BIND_ERROR;
			return ret;
		}
		else
		{
			tv.tv_sec = 0;
			//can-bus read time-out issue, decrease the can-bus set time-out
			tv.tv_usec = 2000; 
			//CAN_TIMEOUT_SET_FAIL = -22;
			if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
			{
			     log_msg(LOG_ERR, STD_ERR, "set socket options error");
			     ret = CAN_SOC_SET_TIMEOUT_ERROR;
			     return ret;
			}
		}
	}
	ret = fd;
	log_msg(LOG_INFO, STD_OUT, "CAN socket initialization is success!");
	return fd;
}



int close_canbus_socket(char fd)
{
	int ret;

	if (fd > 0)
	{
	  if (close(fd) < 0)
	  {
	      log_msg(LOG_ERR, STD_ERR, "Fail to close socket fd = %d", fd);
	      ret = CAN_CLOSE_ERROR;
	  }
	  else 
	  {
	      log_msg(LOG_INFO, STD_OUT, "Successfully closed CAN socket fd = %d", fd);
	      ret = CAN_SUCCESS;
	  }
	}
	else
	{
		log_msg(LOG_ERR, STD_OUT, "Invalid CAN socket fd %d", fd);
		ret = CAN_CLOSE_FD_ALREADY_CLOSED;
	}

	return ret;
}



int write_canbus_frame(char fd, struct can_frame frame)
{
	int ret;
	int nbytes = 0;
	
	//write the buffer
	nbytes = write(fd, &frame, sizeof(struct can_frame));
	if (nbytes < 1)
	{
		ret = CAN_WRITE_ERROR;
		log_msg(LOG_ERR, STD_ERR, "Error in writing CAN-Frame, nbytes: %d", nbytes);
	}
	else 
	{
		log_msg(LOG_DEBUG, STD_OUT, "Written %d bytes on CAN-BUS", nbytes);
		ret = CAN_SUCCESS;
	}
	
	return ret;
}




int read_canbus_frame(char fd, struct can_frame *frame)
{
	int ret = 0;
	int nbytes = 0;
	
	nbytes = read(fd, frame, sizeof(struct can_frame));
	if(nbytes < 2)
	{
		log_msg(LOG_DEBUG, STD_OUT, "%s failed to read CAN frame", __func__);
		ret = CAN_READ_ERROR;
	}
	else
	{
		log_msg(LOG_INFO, STD_OUT, "%d bytes read from the CAN-BUS", nbytes);
		ret =  nbytes;
	}
	
	return ret;
}



void prepare_can_frame(int can_id, uint8_t can_dlc, uint8_t data[CAN_MAX_DLEN], struct can_frame *frame)
{
    frame->can_id = can_id;
    frame->can_dlc = can_dlc;
    memset((void *)frame->data, 0, CAN_MAX_DLEN);
    memcpy((void *)frame->data, (const void *)data, CAN_MAX_DLEN);
}

void show_can_frame(struct can_frame frame)
{
    log_msg(LOG_INFO, STD_OUT, "CAN Frame is:");
    printf("%s\t%X\t%X\t", CAN_IFNAME, frame.can_dlc, frame.can_id);
    for(int i = 0; i < CAN_MAX_DLEN; i++)
        printf("%X ", frame.data[i]);
    printf("\n");
}

void send_CAN_frame(struct can_json_req *st)
{
    uint8_t can_frame[CAN_MAX_DLEN] = {0};
    uint8_t can_frame_len = 0;

    if(valid_port(st->port_id))
    {
        can_frame[0] = CAN_frame_type_request;
        can_frame_len += 1;
        
        can_frame[1] = st->port_id;
        can_frame_len += 1;

        //log_msg(LOG_DEBUG, STD_OUT, "cmp = %d", strcmp(st->action, SET));
        //log_msg(LOG_DEBUG, STD_OUT, "set_len = %ld", strlen(SET));
        //log_msg(LOG_DEBUG, STD_OUT, "action_len = %ld", strlen(st->action));
        
        if(strcmp(st->action, SET) == 0)
        {
            can_frame[1] = (PORT_ON << 4) | (can_frame[1]);
            
            can_frame[2] = (int )st->voltage;
            can_frame_len += 1;
            
            can_frame[3] = (int )((st->voltage - ((int )st->voltage))*100);
            can_frame_len += 1;
        }
        else if(strcmp(st->action, RESET) == 0)
        {
            can_frame[1] = (PORT_OFF << 4) | (can_frame[1]);
        }
        else
        {
            log_msg(LOG_ERR, STD_ERR, "Invalid operation on the port!");
            // send Negative reply to Smart App
            signal_data.payload = (char *)calloc(DATA_SIZE, sizeof(char));
            sprintf(signal_data.payload, "{\"reponse\": {\"port_id\": %d, \"resp_type\": \"%s\", \"is_err\": %s, \"err_code\": %d, \"voltage\": %.2f, \"current\": %.2f}}",
                                                                               0, "response", "true", CAN_INVALID_PORT_ACTION, 0, 0);
            log_msg(LOG_INFO, STD_OUT, "%s\n", signal_data.payload);
            CAN_dbus_emitter(RESPONSE_SIGNAL);
            is_req_valid = false;
        }
    }
    else
    {
        log_msg(LOG_ERR, STD_ERR, "Invalid Port Id of the charger!");
        // TODO: Send Negative reply to Smart App
        is_req_valid = false;
    }
    if(is_req_valid)
    {
        prepare_can_frame(CAN_ID, can_frame_len, can_frame, &tx_frame);
        show_can_frame(tx_frame);
        write_canbus_frame(fd_can, tx_frame);
        is_req_sent = true;
        is_req_valid = false;   // Reset back to false to avoid sending duplicate req frames
        can_req_st.action = NULL;
        free(can_req_st.action);
    }
}



void handle_JSON_CAN_request(char *payload, int len)
{
    json_object *jobj = json_tokener_parse(payload);

    if(jobj != NULL)
    {
        log_msg(LOG_DEBUG, STD_OUT, "A valid JSON string is passed");
        is_req_valid = true;
        json_parse(jobj);
        print_st(&can_req_st);
        send_CAN_frame(&can_req_st);
    }
    else
    {
        log_msg(LOG_ERR, STD_ERR, "Invalid JSON string");

        // Sending Invalid CAN request JSON format error
        signal_data.payload = (char *)calloc(DATA_SIZE, sizeof(char));
        sprintf(signal_data.payload, "{\"reponse\": {\"port_id\": %d, \"resp_type\": \"%s\", \"is_err\": %s, \"err_code\": %d, \"voltage\": %.2f, \"current\": %.2f}}",
                                                                               0, "response", "true", CAN_INVALID_JSON, 0, 0);
        log_msg(LOG_INFO, STD_OUT, "%s\n", signal_data.payload);
        CAN_dbus_emitter(RESPONSE_SIGNAL);

        is_req_valid = false;
    }
}

/* ------------------------------------------------------------- JSON Functions --------------------------------------------------------------------------------- */

void json_parse(json_object *jobj)
{
    enum json_type type;
    
    json_object_object_foreach(jobj, key, val)
    {
        type = json_object_get_type(val);
        log_msg(LOG_DEBUG, STD_OUT, "key = %s", key);

        switch(type)
        {
            case json_type_null:
                log_msg(LOG_DEBUG, STD_OUT, "json_type_null");
                int null_value = -1;
                add_key_value_pair_to_st(key, (void *)&null_value);
                break;
            case json_type_boolean:
                log_msg(LOG_DEBUG, STD_OUT, "json_type_booleann");
                int b = json_object_get_boolean(val);
                log_msg(LOG_DEBUG, STD_OUT, "value: %d", b);
                add_key_value_pair_to_st(key, (void *)&b);
                break;
            case json_type_double:
                log_msg(LOG_DEBUG, STD_OUT, "json_type_doublen");
                float f = json_object_get_double(val);
                log_msg(LOG_DEBUG, STD_OUT, "value: %lf", f);
                add_key_value_pair_to_st(key, (void *)&f);
                break;
            case json_type_int:
                log_msg(LOG_DEBUG, STD_OUT, "json_type_int");
                int i = json_object_get_int(val);
                log_msg(LOG_DEBUG, STD_OUT, "value: %d", i);
                add_key_value_pair_to_st(key, (void *)&i);
                break;
            case json_type_string:
                log_msg(LOG_DEBUG, STD_OUT, "json_type_string");
                char str[DATA_SIZE] = {0};
                memcpy((void *)str, (const void *)json_object_get_string(val), json_object_get_string_len(val));
                str[json_object_get_string_len(val)] = '\0';
                log_msg(LOG_DEBUG, STD_OUT, "value: %s", str);
                add_key_value_pair_to_st(key, (void *)str);
                break;
            case json_type_object:
                log_msg(LOG_DEBUG, STD_OUT, "json_type_object");
                json_object *value_obj = val;
                json_object_object_get_ex(jobj, key, &value_obj);
                json_parse(value_obj);
                break;
            case json_type_array:
                log_msg(LOG_DEBUG, STD_OUT, "type: json_type_array");
                //json_parse_array(jobj, key);
                break;
            default:
                break;
        }
    }
}

/* ------------------------------------------------------------- D-Bus Emitter --------------------------------------------------------------------------------- */

void CAN_dbus_emitter(int signal_cmd)
{
    struct dbus_var_t dbus_info;
    char db_payload[DATA_SIZE] = {0};

    dbus_info.obj = CAN_SER_DATA_OBJECT;
    dbus_info.interface = CAN_SER_DATA_INTERFACE;
    dbus_info.sig_name = CAN_SER_DATA_SIG_NAME;

    signal_data.signal_id = CAN_SER_SIGNAL_ID;
    signal_data.can_id = CAN_ID;
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

/* ------------------------------------------------------------- D-Bus Listener --------------------------------------------------------------------------------- */

void *dbus_listener()
{
    struct dbus_var_t dbus_info;

    uint32_t signal_id = 0;
    char signal_id_str[10] = {0};

    //init dbus and add filters
    read_init(&dbus_info);
    add_filter(&dbus_info, SMART_CHARGER_APP_SER_DATA_RCVR);

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
                case SMART_CHARGER_APP_SER_SIGNAL_ID:
                    log_msg(LOG_DEBUG, STD_OUT, "Msg from SMART CHARGER APP Module");
                    handle_msg_from_SMART_CHARGER_APP_module();
                    break;
                default:
                    log_msg(LOG_DEBUG, STD_OUT, "In the default switch case");
                    break;
            }
        }
    }
    //pthread_exit(NULL);
    return NULL;
}

void handle_msg_from_SMART_CHARGER_APP_module()
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
        log_msg(LOG_DEBUG, STD_OUT, "Invalid signal command from SMART CHARGER APP module");
    }

    sprintf(get_json_dbus_payload, "%s", json_object_get_string(jdata.payload));
    log_msg(LOG_INFO, STD_OUT, "D-Bus signal payload : %s", get_json_dbus_payload);
    log_msg(LOG_INFO, STD_OUT, "D-Bus signal payload len : %ld", strlen(get_json_dbus_payload));

    handle_JSON_CAN_request(get_json_dbus_payload, strlen(get_json_dbus_payload));
}

/* ------------------------------------------------------------- CAN port Listener --------------------------------------------------------------------------------- */

void *CAN_read_thread()
{
    while(read_can_bus)
    {
        if(read_canbus_frame(fd_can, &rx_frame) > 0)
        {
            // TODO: If the frame ID is 7E1 then only process it. otherwise ignore
            show_can_frame(rx_frame);
            convert_CAN_to_JSON();
            //break;
        }
        //sleep(1);
        usleep(250);
    }
    //pthread_exit(NULL);
    return NULL;
}

/* ------------------------------------------------------------- Helper functions --------------------------------------------------------------------------------- */

void convert_CAN_to_JSON()
{
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
            strcpy(resp_type, "response");/* ------------------------------------------------------------- MAIN function --------------------------------------------------------------------------------- */

int main(int argc, char** argv)
{
    int ret = 0;

    pthread_t db_listener;
    pthread_t CAN_listener;
    
    // CAN bus Init
    if((fd_can = open_canbus_socket(CAN_IFNAME)) < 0)
    {
        log_msg(LOG_ERR, STD_ERR, "CAN bus init failed!");
        exit(EXIT_FAILURE);
    }
str
    // Thread for D-Bus listener
    ret = pthread_create(&db_listener, NULL, &dbus_listener, NULL);
    if(ret != 0)
    {
        log_msg(LOG_ERR, STD_ERR, "Error: dbus_listener thread creation Failed!, Error_code:[%d]\n", ret);
        exit(EXIT_FAILURE);
    }

    // Thread for CAN read
    ret = pthread_create(&CAN_listener, NULL, &CAN_read_thread, NULL);
    if(ret != 0)
    {
        log_msg(LOG_ERR, STD_ERR, "Error: CAN read thread creation Failed!, Error_code:[%d]\n", ret);
        exit(EXIT_FAILURE);
    }

    pthread_join(db_listener, NULL);
    pthread_join(CAN_listener, NULL);

    return 0;
}


            
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

void print_st(struct can_json_req *st)
{
    log_msg(LOG_INFO, STD_OUT, "st->port_id = %d", st->port_id);
    log_msg(LOG_INFO, STD_OUT, "st->action = %s", st->action);
    log_msg(LOG_INFO, STD_OUT, "st->voltage = %f", st->voltage);
}


int valid_port(int p_id)
{
    return 1;
}

void add_key_value_pair_to_st(char *k, void *v)
{
    if(strcmp(k, PORT_ID) == 0)
    {
        can_req_st.port_id = *((int *) v);
    }
    else if(strcmp(k, ACTION) == 0)
    {
        int action_len = strlen((char *)v);

        can_req_st.action = (char *)malloc(action_len);

        memcpy((void *)can_req_st.action, (const void *)v, action_len);
        can_req_st.action[action_len] = '\0';
    }
    else if(strcmp(k, VOLTAGE) == 0)
    {
        can_req_st.voltage = *((float *) v);
    }
    else
    {
        log_msg(LOG_ERR, STD_ERR, "Invalid key!");
        // TODO: Send Invalid CAN Request JSON key pairs error
        signal_data.payload = (char *)calloc(DATA_SIZE, sizeof(char));
        sprintf(signal_data.payload, "{\"reponse\": {\"port_id\": %d, \"resp_type\": \"%s\", \"is_err\": %s, \"err_code\": %d, \"voltage\": %.2f, \"current\": %.2f}}",
                                                                               0, "response", "true", CAN_INVALID_JSON, 0, 0);
        log_msg(LOG_INFO, STD_OUT, "%s\n", signal_data.payload);
        CAN_dbus_emitter(RESPONSE_SIGNAL);

        is_req_valid = false;

    }
}



/* ------------------------------------------------------------- MAIN function --------------------------------------------------------------------------------- */

int main(int argc, char** argv)
{
    int ret = 0;

    pthread_t db_listener;
    pthread_t CAN_listener;
    
    // CAN bus Init
    if((fd_can = open_canbus_socket(CAN_IFNAME)) < 0)
    {
        log_msg(LOG_ERR, STD_ERR, "CAN bus init failed!");
        exit(EXIT_FAILURE);
    }
str
    // Thread for D-Bus listener
    ret = pthread_create(&db_listener, NULL, &dbus_listener, NULL);
    if(ret != 0)
    {
        log_msg(LOG_ERR, STD_ERR, "Error: dbus_listener thread creation Failed!, Error_code:[%d]\n", ret);
        exit(EXIT_FAILURE);
    }

    // Thread for CAN read
    ret = pthread_create(&CAN_listener, NULL, &CAN_read_thread, NULL);
    if(ret != 0)
    {
        log_msg(LOG_ERR, STD_ERR, "Error: CAN read thread creation Failed!, Error_code:[%d]\n", ret);
        exit(EXIT_FAILURE);
    }

    pthread_join(db_listener, NULL);
    pthread_join(CAN_listener, NULL);

    return 0;
}
