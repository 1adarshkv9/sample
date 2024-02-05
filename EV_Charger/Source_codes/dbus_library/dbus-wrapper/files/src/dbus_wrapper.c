#include <dbus/dbus.h>
#include <json-c/json.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <syslog.h>

#include "../inc/dbus_wrapper.h"
#include "../inc/dbus_interface.h"

#define DBUS "DBUS"

#define LOGS(l, p, x,arg...) \
	{\
	syslog(l,"" p " : "#x"\n", ##arg);\
	}

pthread_mutex_t dbus_mutex;
pthread_mutex_t dbus_compose_mutex;

/*********************************************************************************************/
/*              fn: void hex_to_string(uint32_t value, char *str)
 *
 *              brief: convert hex to string  
 *
 *              param: 
 *                   str   -  output string
 *                   value -  input value to be convert to string
 *
 *              return: none
 */
/*********************************************************************************************/

void hex_to_string(uint32_t value, char *str){

   sprintf(str,"\"0x%x\"",value);
}

/*********************************************************************************************/
/*              fn: void string_to_hex(uint32_t *value, char *str)
 *
 *              brief: convert string to hex value 
 *
 *              param: 
 *                   str   -  input string
 *                   value -  converted value to be return
 *
 *              return: none
 */
/*********************************************************************************************/
int string_to_hex( char *str){

   return strtol(str,NULL,16);
}

/*********************************************************************************************/
/*              fn: int write_init(struct dbus_var_t *dbus)
 *
 *              brief: Initialize dbus write
 *
 *              param: 
 *                   dbus_var_t - pointer to dbus initialization variable structure
 *
 *              return: error/success codes
 */
/*********************************************************************************************/
int write_init(struct dbus_var_t *dbus)
{

   // initialise the errors
   dbus_error_init(&(dbus->err));

   // connect to the bus
   dbus->conn = dbus_bus_get( DBUS_BUS_SESSION, &dbus->err );
   if ( dbus_error_is_set( &dbus->err ) ) {
       fprintf( stderr, "Connection Error (%s)\n", dbus->err.message );
       dbus_error_free( &dbus->err );
   }
   if ( NULL == dbus->conn ) {
       return DBUS_GET_CONN_ERROR;
   }

   return 0;//SUCCESS;
}

/*********************************************************************************************/
/*              fn: int write_data(char *data, struct dbus_var_t dbus)
 *
 *              brief: write in dbus connection
 *
 *              param: 
 *                   dbus_var_t - pointer to dbus initialized variable structure
 *                   char *data - data to be write on dbus
 *
 *              return: error/success codes
 */
/*********************************************************************************************/

int write_data(char *data, struct dbus_var_t dbus)
{

   // create a signal and check for errors
   dbus.msg = dbus_message_new_signal(
                             dbus.obj,   // object name of the signal
                             dbus.interface,   // interface name of the signal
                             dbus.sig_name ); // name of the signal
   if ( NULL == dbus.msg ) {
       LOGS(LOG_INFO, DBUS, "%s Message Null\n", dbus.sig_name );
       return DBUS_CREATE_SIG_NULL;
   }

   // append arguments onto signal
   dbus_message_iter_init_append(dbus.msg,&dbus.args );

   if( !dbus_message_iter_append_basic( &dbus.args, DBUS_TYPE_STRING, &data) ) {
       LOGS(LOG_INFO, DBUS, "Out Of Memory! %s \n",dbus.sig_name );
       return DBUS_APPEND_ERROR;
   }

   // send the message and flush the connection
   if ( !dbus_connection_send( dbus.conn, dbus.msg, &dbus.serial ) ) {
       LOGS(LOG_INFO, DBUS, "Out Of Memory! %s \n",dbus.sig_name );
       return DBUS_WRITE_ERROR;
   }
   dbus_connection_flush( dbus.conn );
  
   // free the message
   dbus_message_unref( dbus.msg );
   return 0;
}

/*********************************************************************************************/
/*              fn: int compose_data(struct json_obj_t *j_data, char *db_payload struct,
 *                                                                   signal_data_t signal_data)
 *
 *              brief: prepare data to be write on dbus
 *
 *              param: 
 *                   json_obj_t - paointer to json object to be write on dbus
 *                   db_payload - data to be write on dbus
 *                   signal_data - data from the caller service
 *
 *              return: error/success codes
 */
/*********************************************************************************************/

 int compose_data(struct json_obj_t *j_data, char *db_payload, struct signal_data_t signal_data)
{

   j_data->obj = json_object_new_object();
   j_data->dlc = json_object_new_object();
   j_data->cmd = json_object_new_object();
   j_data->sig = json_object_new_object();
   j_data->can_id = json_object_new_object();
   j_data->payload = json_object_new_object();
   
   char temp[64];
   signal_data.dlc = strlen(signal_data.payload);
 
   /* update length of payload */ 
   hex_to_string(signal_data.dlc,temp);
   j_data->dlc = json_tokener_parse(temp);
   json_object_object_add(j_data->obj, "dlc",j_data->dlc);

   /* update command*/ 
   hex_to_string(signal_data.cmd,temp);
   j_data->cmd = json_tokener_parse(temp);
   json_object_object_add(j_data->obj, "cmd",j_data->cmd);

   /* update SIGNAL type of broadcast service */ 
   hex_to_string(signal_data.signal_id,temp);
   j_data->sig = json_tokener_parse(temp);
   json_object_object_add(j_data->obj, "sig_id",j_data->sig);

   if(signal_data.signal_id == CAN_SER_SIGNAL_ID)
   {
       struct json_object* json_format_can_data;
       json_format_can_data = json_tokener_parse(signal_data.payload);
       json_object *jstring_can_data = json_format_can_data;
       json_object_object_add(j_data->obj,"payload",jstring_can_data);
   }
   else if(signal_data.signal_id == LCD_SER_SIGNAL_ID) {
       struct json_object* json_format_lcd_data;
       json_format_lcd_data = json_tokener_parse(signal_data.payload);
       json_object *jstring_lcd_data = json_format_lcd_data;
       json_object_object_add(j_data->obj,"payload",jstring_lcd_data);
   }
   else if(signal_data.signal_id == AWS_MQTT_APP_SER_SIGNAL_ID) {
       struct json_object* json_format_aws_data;
       json_format_aws_data = json_tokener_parse(signal_data.payload);
       json_object *jstring_aws_data = json_format_aws_data;
       json_object_object_add(j_data->obj,"payload",jstring_aws_data);
   }
   else if(signal_data.signal_id == SMART_CHARGER_APP_SER_SIGNAL_ID) {
       struct json_object* json_format_smart_data;
       json_format_smart_data = json_tokener_parse(signal_data.payload);
       json_object *jstring_smart_data = json_format_smart_data;
       json_object_object_add(j_data->obj,"payload",jstring_smart_data);
   }
   else{
       char *payload_data;
       payload_data = malloc(strlen(signal_data.payload));
       sprintf(payload_data,"\"%s\"",signal_data.payload);
       j_data->payload = json_tokener_parse(payload_data);
       free(payload_data);
       payload_data = NULL;
       json_object_object_add(j_data->obj,"payload",j_data->payload);
   }
   sprintf(db_payload, "%s", json_object_to_json_string(j_data->obj));
   //LOGS(LOG_INFO, DBUS, "Sending Data: %s\n", json_object_to_json_string(j_data->obj));
   //LOGS(LOG_INFO, DBUS, "Sending db_payload: %s\n", db_payload);
   return 0;
}

/*********************************************************************************************/
/*              fn: int read_init(struct dbus_var_t *dbus)
 *
 *              brief: dbus read initialize
 *
 *              param: 
 *                   dbus_var_t - paointer to dbus variables
 *
 *              return: error/success codes
 */
/*********************************************************************************************/

int read_init(struct dbus_var_t *dbus)
{
   // initialise the errors
   dbus_error_init(&(dbus->err));

   // connect to the bus
   dbus->conn = dbus_bus_get( DBUS_BUS_SESSION, &dbus->err );
   if ( dbus_error_is_set( &dbus->err ) ) {
       fprintf( stderr, "Connection Error (%s)\n", dbus->err.message );
       dbus_error_free( &dbus->err );
   }
   if ( NULL == dbus->conn ) {
     return DBUS_GET_CONN_ERROR;
   }
   return 0;
}

/*********************************************************************************************/
/*              fn: int add_filter(struct dbus_var_t *dbus, char *rcv_sig)
 *
 *              brief: dbus add signal filter
 *
 *              param: 
 *                   dbus_var_t - paointer to dbus variables
 *                   rcv_sig    - receiver signal to be add in wish list
 *
 *              return: error/success codes
 */
/*********************************************************************************************/

int add_filter(struct dbus_var_t *dbus, char *rcv_sig)
{
  
   dbus_bus_add_match( dbus->conn, rcv_sig, &dbus->err );
   dbus_connection_flush(dbus->conn );

   if ( dbus_error_is_set( &dbus->err ) ) {
       fprintf( stderr, "Match Error (%s)\n", dbus->err.message );
       return DBUS_SIGNAL_MATCH_ERROR;
    }
    return 0;
}

/*********************************************************************************************/
/*              fn: int check_data(struct dbus_var_t *dbus)
 *
 *              brief: Check if any data is available in dbus
 *
 *              param: 
 *                   dbus_var_t - paointer to dbus variables
 *
 *              return: data available or not 
 */
/*********************************************************************************************/

int check_data(struct dbus_var_t *dbus)
{

    dbus_connection_read_write( dbus->conn, 0 );
    dbus->msg = dbus_connection_pop_message( dbus->conn );

    if ( NULL == dbus->msg ) {
       usleep(500);
       return -1;
    }
    else
       return 0;

}

/*********************************************************************************************/
/*              fn: int read_data(struct dbus_var_t *dbus)
 *
 *              brief: read data from dbus - non blocking call
 *
 *              param: 
 *                   dbus_var_t - paointer to dbus variables
 *
 *              return: json data object
 */
/*********************************************************************************************/
struct json_obj_t read_data(struct dbus_var_t *dbus)
{

   struct json_obj_t jdata;
   char* sigvalue;

     if( dbus_message_is_signal( dbus->msg, CAN_SER_DATA_INTERFACE, CAN_SER_DATA_SIG_NAME ) ||
         dbus_message_is_signal( dbus->msg, LCD_SER_DATA_INTERFACE, LCD_SER_DATA_SIG_NAME ) ||
         dbus_message_is_signal( dbus->msg, AWS_MQTT_APP_SER_DATA_INTERFACE, AWS_MQTT_APP_SER_DATA_SIG_NAME ) ||
         dbus_message_is_signal( dbus->msg, SMART_CHARGER_APP_SER_DATA_INTERFACE, SMART_CHARGER_APP_SER_DATA_SIG_NAME )
       ) {
          // read the parameters
       if ( !dbus_message_iter_init( dbus->msg, &dbus->args ) )
           fprintf( stderr, "Message has no arguments!\n" );
       else if ( DBUS_TYPE_STRING != dbus_message_iter_get_arg_type( &dbus->args ) )
           fprintf( stderr, "Argument is not string!\n" );
       else {
           dbus_message_iter_get_basic( &dbus->args, &sigvalue );
           jdata.obj = json_tokener_parse(sigvalue);
           return jdata;
       }
    }
}
