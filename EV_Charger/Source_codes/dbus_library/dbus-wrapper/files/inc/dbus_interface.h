#ifndef __DBUS_INFRA_H__
#define __DBUS_INFRA_H__

/*---------------------------------------------------------------------------------------------------------*/
/* General Commands for D-Bus signal */
#define REQUEST_SIGNAL                      0x2001
#define RESPONSE_SIGNAL                     0x2002
#define STATUS_SIGNAL                       0x2003
#define ERROR_SIGNAL                        0x2004
/*---------------------------------------------------------------------------------------------------------*/
/* CAN SERVICE D-BUS signal command */
#define CAN_OPEN_SOC_ERR_CMD                0x2600
#define CAN_OPEN_SOC_BIND_ERR_CMD           0x2601
#define CAN_SOC_SET_TIMEOUT_ERR_CMD         0x2602
#define CAN_RX_ERR_CMD                      0x2603
#define CAN_SOC_CAN_RAW_ERR_FILTER_ERR_CMD  0x2604

/* CAN service event broadcast object and interface */
#define CAN_SER_DATA_OBJECT        "/iisc/evstation/can"
#define CAN_SER_DATA_INTERFACE     "iisc.evstation.can"
#define CAN_SER_DATA_SIG_NAME      "can"
#define CAN_SER_SIGNAL_ID          0x1001

/* CAN Service event subscriber interface names */
#define CAN_SER_DATA_RCVR          "type='signal',interface='iisc.evstation.can'"

/*---------------------------------------------------------------------------------------------------------*/
/* LCD service event broadcast object and interface */
#define LCD_SER_DATA_OBJECT        "/iisc/evstation/lcd"
#define LCD_SER_DATA_INTERFACE     "iisc.evstation.lcd"
#define LCD_SER_DATA_SIG_NAME      "lcd"
#define LCD_SER_SIGNAL_ID          0x1002

/* LCD Service event subscriber interface names */
#define LCD_SER_DATA_RCVR          "type='signal',interface='iisc.evstation.lcd'"

/*---------------------------------------------------------------------------------------------------------*/
/* AWS_MQTT_APP service event broadcast object and interface */
#define AWS_MQTT_APP_SER_DATA_OBJECT        "/iisc/evstation/aws_mqtt_app"
#define AWS_MQTT_APP_SER_DATA_INTERFACE     "iisc.evstation.aws_mqtt_app"
#define AWS_MQTT_APP_SER_DATA_SIG_NAME      "aws_mqtt_app"
#define AWS_MQTT_APP_SER_SIGNAL_ID          0x1003

/* AWS_MQTT_APP Service event subscriber interface names */
#define AWS_MQTT_APP_SER_DATA_RCVR          "type='signal',interface='iisc.evstation.aws_mqtt_app'"

/*---------------------------------------------------------------------------------------------------------*/
/* SMART CHARGER APP Service event broadcast object and interface */
#define SMART_CHARGER_APP_SER_DATA_OBJECT        "/iisc/evstation/smart_charger_app"
#define SMART_CHARGER_APP_SER_DATA_INTERFACE     "iisc.evstation.smart_charger_app"
#define SMART_CHARGER_APP_SER_DATA_SIG_NAME      "smart_charger_app"
#define SMART_CHARGER_APP_SER_SIGNAL_ID          0x1004

/* SMART CHARGER APP Service event subscriber interface names */
#define SMART_CHARGER_APP_SER_DATA_RCVR          "type='signal',interface='iisc.evstation.smart_charger_app'"

/*---------------------------------------------------------------------------------------------------------*/
#endif

