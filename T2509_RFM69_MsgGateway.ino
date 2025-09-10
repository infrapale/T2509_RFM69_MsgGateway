/**
T2509_RFM69_Alarm_Gateway 
HW: Arduino Mini + RFM69 Module

Send and receive data via UART

*******************************************************************************
https://github.com/infrapale/T2310_RFM69_TxRx
https://learn.adafruit.com/adafruit-feather-m0-radio-with-rfm69-packet-radio
https://learn.sparkfun.com/tutorials/rfm69hcw-hookup-guide/all
*******************************************************************************

*******************************************************************************

UART Functions
  UART_FUNC_TRANSMIT_RAW   = 'T',
  UART_FUNC_TRANSMIT_NODE  = 'N',
  UART_FUNC_GET_AVAIL      = 'A',
  UART_FUNC_READ_RAW       = 'R',
  UART_FUNC_READ_NODE      = 'O' 

UART Replies
  UART_REPLY_AVAILABLE    = 'a',
  UART_REPLY_READ_RAW     = 'r',
  UART_REPLY_READ_NODE    = 'o' 

*******************************************************************************
Sensor Radio Message:   {"Z":"OD_1","S":"Temp","V":23.1,"R":"-"}
                        {"Z":"Dock","S":"T_dht22","V":"8.7","R":"-"}
Relay Radio Message     {"Z":"MH1","S":"RKOK1","V":"T","R":"-"}
Sensor Node Rx Mesage:  <#X1N:OD1;Temp;25.0;->
Relay Node Rx Mesage:   <#X1N:RMH1;RKOK1;T;->

Relay Alarm Message:    <A1M1:OD1;PIR2;H;->

******** UART ******************* Get @home  ********* Radio ********************
                                  -----------
                                  |         |
                                  |         |
                                  | Home=F  |<----{"Z":"TK1","S":"Home","V":"F","R":"-"}-
                                  |         |
                                  |         |<-------------------------------------
                                  -----------




    rfm_send_msg_st *rx_msg = &send_msg; 
    Opto:  <R1O2:L>
            \\\\\\______ value, message (optional)
             \\\\\______ action set: '=' get: '?' reply: ':'
              \\\\______ index
               \\\______ function
                \\______ module_addr
                 \______ module_tag





*******************************************************************************
Relay Mesage:      
      <#Rur=x>   u = relay unit, r= relay index, x=  0=off, 1=on, T=toggle
      <#R12=x>   x:  0=off, 1=on, T=toggle

Read Opto Input Message:
      <#Oui>   u = opto unit, i= opto index
      <=Oui:s>  s=state = H|L   High/Low
      ----> <#O12>  
      <---- <#O12:L>  

Read Alarm Input Message:
      <#Oui>   u = opto unit, i= opto index
      <=Oui:s>  s=state = H|L   High/Low
      ----> <#O12>  
      <---- <#O12:L>  

    <A1A1?->
    <A1W1?->
    <A1D1?->

*******************************************************************************
**/
#include <Arduino.h>
#include "main.h"
#ifdef ADAFRUIT_FEATHER_M0
#include <wdt_samd21.h>
#endif
#ifdef PRO_MINI_RFM69
#include "avr_watchdog.h"
#endif
#include "secrets.h"
#include <RH_RF69.h>
#include <VillaAstridCommon.h>
#include "atask.h"
#include "json.h"
#include "rfm69.h"
#include "uart.h"
#include "rfm_receive.h"
#include "rfm_send.h"
#include "io.h"
#include "pir.h"

#define ZONE  "OD_1"
//*********************************************************************************************
#define SERIAL_BAUD   9600
#define ENCRYPTKEY    RFM69_KEY   // defined in secret.h


RH_RF69         rf69(RFM69_CS, RFM69_INT);
RH_RF69         *rf69p;

// Function Prototypes
void debug_print_task(void);
void run_100ms(void);
void send_test_data_task(void);
void rfm_receive_task(void); 

// Task definitions (mor may be dfined in modules)
atask_st debug_print_handle        = {"Debug Print    ", 5000,0, 0, 255, 0, 1, debug_print_task};
atask_st clock_handle              = {"Tick Task      ", 100,0, 0, 255, 0, 1, run_100ms};
atask_st rfm_receive_handle        = {"Receive <- RFM ", 100,0, 0, 255, 0, 1, rfm_receive_task};
#ifdef SEND_TEST_MSG
atask_st send_test_data_handle     = {"Send Test Task ", 10000,0, 0, 255, 0, 1, send_test_data_task};
#endif

#ifdef PRO_MINI_RFM69
//AVR_Watchdog watchdog(4);
#endif

rfm_receive_msg_st  *receive_p;
rfm_send_msg_st     *send_p;
uart_st         *uart_p;



void initialize_tasks(void)
{
  atask_initialize();
  atask_add_new(&debug_print_handle);
  atask_add_new(&clock_handle);
  atask_add_new(&rfm_receive_handle);
  #ifdef DEBUG_PRINT
  Serial.print(F("Tasks initialized ")); Serial.println(TASK_NBR_OF);
  #endif
}


void setup() 
{
    Serial.setTimeout(5000);
    Serial.begin(9600);
    while (!Serial){delay(1);} ; // wait until serial console is open, remove if not tethered to computer
    delay(2000);
    Serial.print(APP_NAME); Serial.print(" Compiled: ");
    Serial.flush();
    Serial.print(__DATE__); Serial.print(" "); Serial.print(__TIME__); 
    Serial.flush();
    Serial.print("/ Module Tag: "); Serial.print(MY_MODULE_TAG); 
    // Serial.print(F(" Addr: ")); Serial.print(MY_MODULE_ADDR); 
    Serial.println();
    Serial.flush();

    #ifdef  ADA_M0_RFM69
    SerialX.begin(9600);
    #endif
    
    uart_p = uart_get_data_ptr();
    send_p = rfm_send_get_data_ptr();

    rf69p = &rf69;
    rfm69_initialize(&rf69);
    rfm_receive_initialize();
    io_initialize();
    // Hard Reset the RFM module
    rfm_send_initialize();
    initialize_tasks();
    uart_initialize();
    pir_initialize();

    rfm_send_radiate_msg(APP_NAME);


    #ifdef ADAFRUIT_FEATHER_M0
    // Initialze WDT with a 2 sec. timeout
    //wdt_init ( WDT_CONFIG_PER_16K );
    #endif
    #ifdef PRO_MINI_RFM69
    //watchdog.set_timeout(4);
    #endif


}



void loop() 
{
    atask_run();  
}


void rfm_receive_task(void) 
{
    rfm_receive_message();
    #ifdef ADAFRUIT_FEATHER_M0
    wdt_reset();
    #endif
    #ifdef PRO_MINI_RFM69
    // watchdog.clear();
    #endif
}


void run_100ms(void)
{
    // static uint8_t ms100 = 0;
    // if (++ms100 >= 10 )
    // {
    //     ms100 = 0;
    //     if (++MyTime.second > 59 )
    //     {
    //       MyTime.second = 0;
    //       if (++MyTime.minute > 59 )
    //       {    
    //         MyTime.minute = 0;
    //         if (++MyTime.hour > 23)
    //         {
    //             MyTime.hour = 0;
    //         }
    //       }   
    //   }
    // }
    io_run_100ms();
}

void debug_print_task(void)
{
  #ifdef DEBUG_PRINT
  atask_print_status(true);
  #endif
  //rfm_send_radiate_msg("Debug");
}

