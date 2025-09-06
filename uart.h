#ifndef __UART_H__
#define __UART_H__
#define UART_MAX_BLOCK_LEN  8

typedef enum
{
    UART_FUNC_TRANSMIT_RAW   = 'T',
    UART_FUNC_TRANSMIT_NODE  = 'N',
    UART_FUNC_GET_AVAIL      = 'A',
    UART_FUNC_READ_RAW       = 'W',
    UART_FUNC_READ_DECODED   = 'D',
    UART_FUNC_READ_OPTO      = 'O',
    UART_FUNC_READ_ALARM     = 'M',
} UART_FUNC_et;


typedef enum
{
    UART_REPLY_AVAILABLE    = 'a',
    UART_REPLY_READ_RAW     = 'r',
    UART_REPLY_READ_OPTO    = 'o' 
} uart_reply_et;

typedef enum
{
    UART_PARSE_FRAME    = 0u,
    UART_PARSE_COMMAND,
    UART_PARSE_REPLY
} uart_parse_select_et;

typedef enum
{ 
    UART_ACTION_SET    = '=',
    UART_ACTION_GET    = '?',
    UART_ACTION_REPLY  = ':',
} uart_action_et;

typedef enum
{
    UART_VALUE_LOW      = 'L',
    UART_VALUE_HIGH     = 'H'
} uart_value_et;

typedef struct
{
    String          str;
    char            radio_msg[MAX_MESSAGE_LEN];
    uint8_t         len;
    bool            avail;
    char            module_tag;
    char            module_addr;
    char            function;
    char            index;  
    uart_action_et  action;   //set/get/reply
    char            value;       
    //UART_FUNC_et     cmd;
    //msg_format_et   format;
    msg_status_et   status;
} uart_msg_st;


typedef struct
{
    uart_msg_st     msg;
    uint32_t        timeout;
    UART_FUNC_et     cmd;
} uart_rx_st;

typedef struct
{
    uart_msg_st     msg;
    uint32_t        timeout;
    msg_format_et   cmd_format;
} uart_tx_st;

typedef struct
{
    String zone;
    String name; 
    String value;
    String remark;
} uart_node_st;

typedef struct
{
    uart_rx_st      rx;
    uart_tx_st      tx;
    uart_node_st    node;
} uart_st;

/// @brief Clear rx available
/// @param  -
/// @return -
void uart_initialize(void);

/// @brief  Get pointer to module data
/// @param
/// @return data pointer
uart_st *uart_get_data_ptr(void);

/// @brief  Read uart
/// @note   Save mesage in uart.rx.str
/// @param  -
/// @return true if data is available
bool uart_read_uart(void);

/// @brief  Parse Rx frame, 
/// @note   check that the frame is valid and addressed to me
/// @param  -
/// @return -
void uart_parse_rx_frame(void);

/// @brief  Print message metadata for debugging
/// @param  pointer to msg struct
/// @return -
void uart_print_msg_metadata(uart_msg_st *msg);

/// @brief  Build radio mesasge
/// @param  -
/// @return -
void uart_rx_build_rfm_array(void);

/// @brief  Execute command
/// @param  UART command
/// @return
void uart_exec_cmnd(UART_FUNC_et ucmd);

void uart_alarm_handling_task(void);


#endif