#ifndef __IISC_DBUS_H__
#define __IISC_DBUS_H__

enum erros
{
/*    SUCCESS=0, */
    DBUS_GET_CONN_ERROR=1,
    DBUS_CREATE_SIG_NULL,
    DBUS_APPEND_ERROR,
    DBUS_WRITE_ERROR,
    DBUS_SIGNAL_MATCH_ERROR,
};

struct dbus_var_t
{
  DBusError err;
  DBusConnection* conn;
  dbus_uint32_t serial;
  DBusMessage* msg;
  DBusMessageIter args;

  char *obj;
  char *interface;
  char *sig_name;
  char *rcvr_sig;
};

struct json_obj_t
{
  struct json_object *obj;
  struct json_object *sig;
  struct json_object *dlc;
  struct json_object *cmd;
  struct json_object *can_id;
  struct json_object *payload;
};

struct signal_data_t
{
  uint32_t  signal_id;
  uint32_t  can_id;
  uint8_t   dlc;
  uint32_t  cmd;
  char     *payload;
};

enum SYSTEM_ERROR_CODES
{
    //CAN APP
    CAN_INVALID_JSON = 0x001,
    CAN_INVALID_PORT_ID = 0x002,
    CAN_INVALID_PORT_ACTION = 0x003,

    //SMART APP
    SMART_APP_INVALID_JSON = 0x004,

};


void hex_to_string(uint32_t value, char *str);
int string_to_hex(char *str);
int write_init(struct dbus_var_t *dbus);
struct json_obj_t read_data(struct dbus_var_t *dbus);
int read_init(struct dbus_var_t *dbus);
int write_data(char *data, struct dbus_var_t dbus);
struct json_obj_t read_data(struct dbus_var_t *dbus);
int add_filter(struct dbus_var_t *dbus, char *signal);
int compose_data(struct json_obj_t *j_data, char *db_payload, struct signal_data_t signal_data);
int check_data(struct dbus_var_t *dbus);

#endif 
