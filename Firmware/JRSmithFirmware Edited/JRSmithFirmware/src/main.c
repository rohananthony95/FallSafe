/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include "wifi.h"
#include "camera.h"
#include "timer_interface.h"
#include "ov2640.h"
#include "conf_board.h"
#include "conf_clock.h"


int main (void)
{	
	int state_position = 1;
	
	sysclk_init();
	wdt_disable(WDT);
	board_init();
	
	configure_tc();
	
	configure_usart_wifi();
	configure_wifi_comm_pin();
	configure_wifi_web_setup_pin();
		
	write_wifi_command("set uart.flow 0 on \r\n", 5);
	write_wifi_command("set bu c r 5000 \r\n", 5);
	write_wifi_command("set sy i g wlan 20 \r\n", 5);
	write_wifi_command("set sy i g network 18 \r\n", 5);
	write_wifi_command("set sy i g softap 21 \r\n", 5);
	write_wifi_command("set sy c g 13 \r\n", 5);
	write_wifi_command("set wl n o 14 \r\n", 5);
	write_wifi_command("set http.server.root_filename webcam/index.html \r\n", 5);
	write_wifi_command("save \r\n", 5);
	
	command_received_flag = false;
	
	ioport_set_pin_level(WIFI_RESET_PIN, false);
	delay_ms(100);
	ioport_set_pin_level(WIFI_RESET_PIN, true);
	
	delay_ms(10000);
	
	web_setup_flag = false; 
	
	while (ioport_get_pin_level(NETWORK_STATUS_PIN) == false)
	{
		if (web_setup_flag == true)
		{
			write_wifi_command("setup web \r\n", 100);
			
			web_setup_flag = false;
		}
	}
		
	init_camera();
	
	configure_camera();
	
 	write_wifi_command("set sy c p off \r\n", 5);
 	write_wifi_command("set sy c e off \r\n", 5);
	
	write_wifi_command("tcpc 18.191.102.247 8000 \r\n", 10);
	
	while(1)
	{
		switch(state_position)
		{
			case 1:
				if (web_setup_flag == true)
				{
					write_wifi_command("setup web \r\n", 100);
					web_setup_flag = false;
					
					while (ioport_get_pin_level(NETWORK_STATUS_PIN) == false)
					{
						// Waiting for connection
					}
				}
				
				else
				{
					state_position = 2;
				}
				break;
			
			case 2:
				if (ioport_get_pin_level(NETWORK_STATUS_PIN) == true)
				{
					state_position = 3;
				}
				
				else
				{
					ioport_set_pin_level(WIFI_RESET_PIN, false);
					delay_ms(100);
					ioport_set_pin_level(WIFI_RESET_PIN, true);
					
					state_position = 1;
				}
				break;
			
			case 3:
				write_wifi_command("poll all \r\n", 5);
				
				if (none_flag == true)
				{
					delay_ms(1000);
					none_flag = false;
					state_position = 1;
				}
				else
				{
					state_position = 4;
				}
				break;
			
			case 4:
				if (start_capture() == 1)
				{
					write_image_to_file();
					delay_ms(1500);
				}
				
				state_position = 1;
				break;
			
			default:
				state_position = 1;
		}
	}
}
