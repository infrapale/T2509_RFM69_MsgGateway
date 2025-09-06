2025-09-05 -------------------------------------------------------------------------------
void uart_rx_send_rfm_from_node(void)
{
    uart.rx.msg.str = uart.rx.msg.str.substring(6,uart.rx.msg.len - 1);
    uart_build_node_from_rx_str();
    rfm_send_msg_st *send_p = rfm_send_get_data_ptr();
    json_convert_uart_node_to_json(send_p->radio_msg, &uart);
    rfm_send_radiate_msg(send_p->radio_msg);
}

void uart_exec_cmnd(UART_FUNC_et ucmd)
{
    // String str = "<Xuxi:0>";
    // switch(ucmd)
    // {
    //     case UART_FUNC_TRANSMIT_RAW:
    //         io_led_flash(LED_INDX_RED, 10);
    //         uart_rx_send_rfm_from_raw();
    //         break;
    //     case UART_FUNC_TRANSMIT_NODE:
    //         io_led_flash(LED_INDX_RED, 20);
    //         uart_rx_send_rfm_from_node();
    //         break;
    //     case UART_FUNC_GET_AVAIL:
    //         str[4] = UART_REPLY_AVAILABLE;
    //         if(rfm_receive_message_is_avail()) str[6] = '1';
    //         SerialX.println(str);
    //         break;
    //     case UART_FUNC_READ_RAW:
    //         rfm_receive_clr_message_flag();
    //         uart_build_raw_tx_str();
    //         SerialX.println(uart.tx.msg.str);          
    //         break;
    //     case UART_FUNC_READ_OPTO:
    //         rfm_receive_clr_message_flag();
    //         uart_build_node_tx_str();
    //         SerialX.println(uart.tx.msg.str);          
    //         break;
    //     case UART_FUNC_READ_ALARM:
    //         rfm_receive_clr_message_flag();
    //         uart_build_node_tx_str();
    //         SerialX.println(uart.tx.msg.str);          
    //         break;

    // }
}

