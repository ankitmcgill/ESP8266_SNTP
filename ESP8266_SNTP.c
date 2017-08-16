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

#include "ESP8266_SNTP.h"

//LOCAL LIBRARY VARIABLES////////////////////////////////
//DEBUG RELATED
static uint8_t _esp8266_sntp_debug;

//TIMER RELATED
static uint8_t esp8266_sntp_try_count;
static os_timer_t _esp8266_sntp_done_check_timer;

//CB FUNCTION
static void (*_esp8266_sntp_user_cb)(uint32_t);
//END LOCAL LIBRARY VARIABLES/////////////////////////////

void ICACHE_FLASH_ATTR ESP8266_SNTP_SetDebug(uint8_t debug_on)
{
	//SET DEBUG PRINTF ON(1) OR OFF(0)

	_esp8266_sntp_debug = debug_on;
}

void ICACHE_FLASH_ATTR ESP8266_SNTP_SetServerIp(uint8_t index, struct ip_addr* i)
{
	//SET THE SNTP TIME SERVER BASED ON IP ADDRESS SUPPLIED
	//ESP8266 CAN HAVE 3 TIME SERVER CONFIGURED
	// 0 <= index <= 2

	if(index > 2)
	{
		if(_esp8266_sntp_debug)
		{
			os_printf("ESP8266 : SNTP : Server index greater than 2 not allowed\n");
		}
		return;
	}

	sntp_setserver(index, i);
	if(_esp8266_sntp_debug)
	{
		os_printf("ESP8266 : SNTP : server index %d set to %s\n", index, IP2STR(i));
	}
}

void ICACHE_FLASH_ATTR ESP8266_SNTP_SetServerName(uint8_t index, uint8_t* server_name)
{
	//SET THE SNTP TIME SERVER BASED ON NAMED TIME SERVER SUPPLIED
	//ESP8266 CAN HAVE 3 TIME SERVER CONFIGURED
	// 0 <= index <= 2

	if(index > 2)
	{
		if(_esp8266_sntp_debug)
		{
			os_printf("ESP8266 : SNTP : Server index greater than 2 not allowed\n");
		}
		return;
	}

	sntp_setservername(index, server_name);
	if(_esp8266_sntp_debug)
	{
		os_printf("ESP8266 : SNTP : server index %d set to %s\n", index, server_name);
	}
}

void ICACHE_FLASH_ATTR ESP8266_SNTP_SetTimezone(int8_t utc_offset_hours)
{
	//SET TIMEZONE OFFFSET FROM UTC
	//USED BY ESP8266 TO CALCULATE LOCAL TIME AS NTP RETURNS
	//UTC TIME
	//RIGHT NOW ONLY SUPPORTS HOURS OFFSET

	sntp_set_timezone(utc_offset_hours);
	if(_esp8266_sntp_debug)
	{
		os_printf("ESP8266 : SNTP : timezone offset set to %d\n", utc_offset_hours);
	}
}

void ICACHE_FLASH_ATTR ESP8266_SNTP_SetCbFunction(void (*user_cb_fn)(uint32))
{
	//SET THE USER CB FUNCTION

	_esp8266_sntp_user_cb = user_cb_fn;
	if(_esp8266_sntp_debug)
	{
		os_printf("ESP8266 : SNTP : user cb function set\n");
	}
}

void ICACHE_FLASH_ATTR ESP8266_SNTP_Start(void)
{
	//START SNTP PROCESS OF CONNECTING TO THE TIME SERVER
	//AND GETTING THE TIME

	sntp_init();
	if(_esp8266_sntp_debug)
	{
		os_printf("ESP8266 : SNTP : Started\n");
	}
}

void ICACHE_FLASH_ATTR ESP8266_SNTP_Stop(void)
{
	//STOP SNTP

	sntp_stop();
	if(_esp8266_sntp_debug)
	{
		os_printf("ESP8266 : SNTP : Stopped\n");
	}
}

uint32_t ICACHE_FLASH_ATTR ESP8266_SNTP_GetCurrentTimestamp(void)
{
	//RETURN THE CURRENT LOCAL TIMESTAMP FROM
	//JANUARY 1 1970

	uint32_t ts = sntp_get_current_timestamp();
	if(ts == 0)
	{
		//INVALID TIMESTAMP
		//START TIMER TO GET TIMESTAMP AGAIN
		if(_esp8266_sntp_debug)
		{
			os_printf("ESP8266 : SNTP : First try unsuccessfull. Starting timer to try again\n");
		}
		esp8266_sntp_try_count = 0;
		os_timer_setfn(&_esp8266_sntp_done_check_timer, _esp8266_sntp_timer_cb, NULL);
		os_timer_arm(&_esp8266_sntp_done_check_timer, 500, 0);
	}
	else
	{
		//TIMESTAMP IS VALID
		return ts;
	}

	return 0;
}

char* ICACHE_FLASH_ATTR ESP8266_SNTP_GetTimeString(uint32_t timestamp)
{
	//RETURN THE CURRENT TIME STRING IN SPECIFIED FORMAT
	//FROM THE SUPPLIED TIMESTAMP

	return sntp_get_real_time(timestamp);
}

void ICACHE_FLASH_ATTR _esp8266_sntp_timer_cb(void *pArg)
{
	//INTERNAL TIMER CB FUNCTION

	esp8266_sntp_try_count++;
	uint32_t val = sntp_get_current_timestamp();

	if(val != 0)
	{
		//SNTP TIME IS READY
		os_timer_disarm(&_esp8266_sntp_done_check_timer);
		if(_esp8266_sntp_debug)
		{
			os_printf("ESP8266 : SNTP : Got timestamp @ attempt #%u\n", esp8266_sntp_try_count);
		}
		//CALL THE USER SPECIFIED CALLBACK FUNCTION IF NOT NULL
		if(_esp8266_sntp_user_cb != NULL)
		{
			(*_esp8266_sntp_user_cb)(val);
		}
	}
	else
	{
		if(esp8266_sntp_try_count > ESP8266_SNTP_RETRIES_COUNT)
		{
			//MAX RETRY COUNT EXCEEDED
			os_timer_disarm(&_esp8266_sntp_done_check_timer);
			if(_esp8266_sntp_debug)
			{
				os_printf("ESP8266 : SNTP : All tries finished. Calling user cb with timestamp = 0\n", esp8266_sntp_try_count);
			}
			if(_esp8266_sntp_user_cb != NULL)
			{
				(*_esp8266_sntp_user_cb)(0);
			}
		}
		else
		{
			os_timer_arm(&_esp8266_sntp_done_check_timer, 500, 0);
		}
	}
}
