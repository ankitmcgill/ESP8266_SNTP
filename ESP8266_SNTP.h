/*************************************************
* ESP8266 SNTP LIBRARY
*
* OCTOBER 27 2016
* ANKIT BHATNAGAR
* ANKIT.BHATNAGARINDIA@GMAIL.COM
* ***********************************************/

#ifndef _ESP8266_SNTP_H_
#define _ESP8266_SNTP_H_

#include <osapi.h>
#include <sntp.h>
#include "user_interface.h"
#include <ets_sys.h>

os_timer_t sntp_done_check_timer;
struct ip_addr ip;

//POINTER TO THE CALLBACK FUNCTION THAT THE LIBRARY WILL CALL
//AFTER SNTP OPERATION DONE AND PASS THE CALCULATED TIMESTAMP AS ARGUMENT
void (*ESP8266_SNTP_time_operation_done_callback)(uint32);

void ESP8266_SNTP_set_server(uint8_t index, struct ip_addr* i);
void ESP8266_SNTP_set_server_name(uint8_t index, uint8_t* server_name);
void ESP8266_SNTP_set_timezone(int8_t utc_offset_hours);
void ESP8266_SNTP_start(void);
void ESP8266_SNTP_get_current_timestamp(void);
void ESP8266_SNTP_get_time_string(void);
void ESP8266_SNTP_set_callback(void (*func)(uint32));
void ESP8266_SNTP_timer_cb(void *pArg);

#endif
