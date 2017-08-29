/*************************************************
* ESP8266 SNTP LIBRARY
*
*	REFERENCE
* ----------
*	(1) ONLINE NTP TOOL
*			http://www.timestampconvert.com/
*
*	(2) NTP TOOL
*			https://w3dt.net/tools/ntpq
*
* OCTOBER 27 2016
*
* ANKIT BHATNAGAR
* ANKIT.BHATNAGARINDIA@GMAIL.COM
* ***********************************************/

#ifndef _ESP8266_SNTP_H_
#define _ESP8266_SNTP_H_

#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "mem.h"
#include "string.h"
#include "stdio.h"
#include "user_interface.h"

#define ESP8266_SNTP_RETRIES_COUNT 5

//CUSTOM VARIABLE STRUCTURES/////////////////////////////
typedef struct
{
  uint16_t year;
  char month[4];
  uint8_t date;
  char day[4];
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
} ESP8266_SNTP_TIME_COMPONENTS;
//END CUSTOM VARIABLE STRUCTURES/////////////////////////

//FUNCTION PROTOTYPES/////////////////////////////////////////////
//CONFIGURATION FUNCTIONS
void ICACHE_FLASH_ATTR ESP8266_SNTP_SetDebug(uint8_t debug_on);
void ICACHE_FLASH_ATTR ESP8266_SNTP_SetServerIp(uint8_t index, struct ip_addr* i);
void ICACHE_FLASH_ATTR ESP8266_SNTP_SetServerName(uint8_t index, uint8_t* server_name);
void ICACHE_FLASH_ATTR ESP8266_SNTP_SetTimezone(int16_t utc_offset_min);
void ICACHE_FLASH_ATTR ESP8266_SNTP_SetCbFunction(void (*user_cb_fn)(uint32));

//OPERATION FUNCTIONS
void ICACHE_FLASH_ATTR ESP8266_SNTP_Start(void);
void ICACHE_FLASH_ATTR ESP8266_SNTP_Stop(void);

//GET FUNCTIONS
uint32_t ICACHE_FLASH_ATTR ESP8266_SNTP_GetCurrentTimestamp(void);
char* ICACHE_FLASH_ATTR ESP8266_SNTP_GetTimeString(uint32_t timestamp);
ESP8266_SNTP_TIME_COMPONENTS* ICACHE_FLASH_ATTR ESP8266_SNTP_GetTimeComponents(char* str);

//INTERNAL FUNCTIONS
void ICACHE_FLASH_ATTR _esp8266_sntp_timer_cb(void *pArg);
#endif
