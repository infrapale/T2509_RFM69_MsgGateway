#ifndef __MAIN_H__
#define __MAIN_H__
#include "WString.h"

#undef  DEBUG_PRINT 
//#define SEND_TEST_MSG 
//#define ADA_M0_RFM69 
#define PRO_MINI_RFM69
#include <Arduino.h>
#include "rfm69.h"

#ifdef  ADA_M0_RFM69
#define SerialX  Serial1
#else
#define SerialX Serial
#endif

#define APP_NAME    ("T2509_RFM69_MsgGateway")
//#define TEST_MODE
#define TASK_NBR_OF  3

#define MY_MODULE_TAG       ('A')
#define MY_MODULE_ADDR      ('1')

//#define LED_INDICATION

typedef struct
{
    char            module;
    char            addr;         
} module_data_st;


typedef enum
{
    MSG_FORMAT_RAW = 0,
    MSG_FORMAT_SENSOR_JSON,
    MSG_FORMAT_RELAY_JSON
}  msg_format_et;

typedef enum
{
    STATUS_UNDEFINED = 0,
    STATUS_OK_FOR_ME,
    STATUS_NOT_FOR_ME,
    STATUS_UNKNOWN_COMMAND,
    STATUS_CORRECT_FRAME,
    STATUS_INCORRECT_FRAME,
} msg_status_et;


extern module_data_st     me;


#endif