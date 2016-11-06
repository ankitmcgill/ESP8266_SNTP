/*************************************************
* ESP8266 SNTP LIBRARY
*
* OCTOBER 27 2016
* ANKIT BHATNAGAR
* ANKIT.BHATNAGARINDIA@GMAIL.COM
* ***********************************************/

#include "ESP8266_SNTP.h"

os_timer_t sntp_done_check_timer;

void ESP8266_SNTP_set_server(uint8_t index, struct ip_addr* i)
{
	//SET THE SNTP TIME SERVER BASED ON IP ADDRESS SUPPLIED
	//ESP8266 CAN HAVE 3 TIME SERVER CONFIGURED
	// 0 <= index <= 2

	sntp_setserver(index, i);
}

void ESP8266_SNTP_set_server_name(uint8_t index, uint8_t* server_name)
{
	//SET THE SNTP TIME SERVER BASED ON NAMED TIME SERVER SUPPLIED
	//ESP8266 CAN HAVE 3 TIME SERVER CONFIGURED
	// 0 <= index <= 2

	sntp_setservername(index, server_name);
}

void ESP8266_SNTP_set_timezone(int8_t utc_offset_hours)
{
	//SET TIMEZONE OFFFSET FROM UTC
	//USED BY ESP8266 TO CALCULATE LOCAL TIME AS NTP RETURNS
	//UTC TIME
	//RIGHT NOW ONLY SUPPORTS HOURS OFFSET

	sntp_set_timezone(utc_offset_hours);
}

void ESP8266_SNTP_start(void)
{
	//START SNTP PROCESS OF CONNECTING TO THE TIME SERVER
	//AND GETTING THE TIME

	sntp_init();
}

void ESP8266_SNTP_get_current_timestamp(void)
{
	//RETURN THE CURRENT LOCAL TIMESTAMP FROM
	//JANUARY 1 1970

	os_timer_setfn(&sntp_done_check_timer, ESP8266_SNTP_timer_cb, NULL);

	//NEED TO GIVE SNTP MORE TIME
	os_timer_arm(&sntp_done_check_timer, 500, 1);
}

void ESP8266_SNTP_get_time_string(void)
{
	//GET THE CURRENT LOCAL TIME AS A STRING
}

void ESP8266_SNTP_set_callback(void (*func)(uint32))
{
	//SET THE CALLBACK FUNCTION THAT THE LIBRARY WILL CALL ONCE A VALID TIME IS
	//IS RECEIVED FROM THE TIME SERVER. CURRENT TIMESTAMP WILL BE PASSED TO
	//CALLBACK FUNCTION AS ARGUMENT

	ESP8266_SNTP_time_operation_done_callback = func;
}

void ESP8266_SNTP_timer_cb(void* pArg)
{
	os_printf("sntp timer tick\n");
	uint32_t val = sntp_get_current_timestamp();
	if(val != 0)
	{
		os_printf("sntp timer done\n");
		//SNTP TIME IS READY
		os_timer_disarm(&sntp_done_check_timer);
		//CALL THE USER SPECIFIED CALLBACK FUNCTION
		ESP8266_SNTP_time_operation_done_callback(val);
	}
}
