/*
 * wifi.c
 *
 * Created: 2/11/2018 2:48:45 PM
 *  Author: ece-lab3
 */ 

#include "wifi.h"
#include <string.h>

volatile uint32_t received_byte_wifi = 0;
volatile bool new_rx_wifi = false;
volatile unsigned int input_pos_wifi = 0;
volatile char input_line_wifi[MAX_INPUT_WIFI];
volatile char image_length_JPEG_string[20];
volatile int image_string_length = 0;
volatile bool web_setup_flag = false;
volatile bool command_received_flag = false;

volatile bool start_transfer_flag = false;
volatile bool none_flag = false;
volatile bool closed_flag = false;


void WIFI_USART_HANDLER(void)
{
	uint32_t ul_status;

	/* Read USART status. */
	ul_status = usart_get_status(BOARD_USART);

	/* Receive buffer is full. */
	if (ul_status & US_CSR_RXBUFF) {
		usart_read(BOARD_USART, &received_byte_wifi);
		new_rx_wifi = true;
		process_incoming_byte_wifi((uint8_t)received_byte_wifi);
	}
}

void wifi_command_response_handler(uint32_t ul_id, uint32_t ul_mask)
{
	unused(ul_id);
	unused(ul_mask);
	
	command_received_flag = true;

	process_data_wifi();
	for (int jj=0;jj<MAX_INPUT_WIFI;jj++) input_line_wifi[jj] = 0;
	input_pos_wifi = 0;
}

void wifi_web_setup_handler(uint32_t ul_id, uint32_t ul_mask)
{
	unused(ul_id);
	unused(ul_mask);

	web_setup_flag = true;
}

void configure_usart_wifi(void)
{
	gpio_configure_pin(PIN_USART0_RXD_IDX, PIN_USART0_RXD_FLAGS);
	gpio_configure_pin(PIN_USART0_TXD_IDX, PIN_USART0_TXD_FLAGS);
	gpio_configure_pin(PIN_USART0_CTS_IDX, PIN_USART0_CTS_FLAGS);
// 	gpio_configure_pin(PIN_USART0_RTS_IDX, PIN_USART0_RTS_FLAGS);
	
	static uint32_t ul_sysclk;
	const sam_usart_opt_t usart_console_settings = {
		BOARD_USART_BAUDRATE,
		US_MR_CHRL_8_BIT,
		US_MR_PAR_NO,
		US_MR_NBSTOP_1_BIT,
		US_MR_CHMODE_NORMAL,
		/* This field is only used in IrDA mode. */
		0
	};

	/* Get peripheral clock. */
	ul_sysclk = sysclk_get_peripheral_hz();

	/* Enable peripheral clock. */
	sysclk_enable_peripheral_clock(BOARD_USART_ID);

	/* Configure USART. */
	usart_init_hw_handshaking(BOARD_USART, &usart_console_settings, ul_sysclk);

	/* Disable all the interrupts. */
	usart_disable_interrupt(BOARD_USART, ALL_INTERRUPT_MASK);
	
	/* Enable TX & RX function. */
	usart_enable_tx(BOARD_USART);
	usart_enable_rx(BOARD_USART);
	
	usart_enable_interrupt(BOARD_USART, US_IER_RXRDY);

	/* Configure and enable interrupt of USART. */
	NVIC_EnableIRQ(BOARD_USART_IRQn);
}

void configure_wifi_comm_pin(void)
{
	/* Configure PIO clock. */
	pmc_enable_periph_clk(WIFI_COMM_ID);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(WIFI_COMM_PIO, WIFI_COMM_ID, WIFI_COMM_PIN_MSK,
			WIFI_COMM_ATTR, wifi_command_response_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)WIFI_COMM_ID);

	/* Enable PIO interrupt lines. */
	pio_enable_interrupt(WIFI_COMM_PIO, WIFI_COMM_PIN_MSK);
}

void configure_wifi_web_setup_pin(void)
{
	/* Configure PIO clock. */
	pmc_enable_periph_clk(WEB_SETUP_ID);

	/* Adjust PIO debounce filter using a 10 Hz filter. */
	pio_set_debounce_filter(WEB_SETUP_PIO, WEB_SETUP_PIN_MSK, 10);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(WEB_SETUP_PIO, WEB_SETUP_ID, WEB_SETUP_PIN_MSK,
			WEB_SETUP_ATTR, wifi_web_setup_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)WEB_SETUP_ID);

	/* Enable PIO interrupt lines. */
	pio_enable_interrupt(WEB_SETUP_PIO, WEB_SETUP_PIN_MSK);
}


void process_incoming_byte_wifi(uint8_t in_byte)
{ 
	input_line_wifi[input_pos_wifi++] = in_byte;
}

void process_data_wifi (void)
{
	
	if (strstr(input_line_wifi, "Start transfer"))
	{
		start_transfer_flag = true;
	}
	
	 else if (strstr(input_line_wifi, "None"))
	 {
		 none_flag = true;
	 }
	 
	 else if (strstr(input_line_wifi, "[Closed: 0]"))
	 {
		 closed_flag = true;
	 }
}

void write_wifi_command(char* comm, uint8_t cnt)
{
	counts = 0;
	command_received_flag = false;

	usart_write_line(BOARD_USART, comm);
		
	while (counts < cnt)
	{
		if (command_received_flag == true)
		{
			return;
		}
	}
}

void write_image_to_file(void)
{
 	if (image_length_JPEG == 0)
 	{
 		return;
 	}

	else
	{	
		itoa(image_length_JPEG, image_length_JPEG_string, 10);
		image_string_length = strlen(image_length_JPEG_string);
		
		uint8_t current_byte = "";
		
		char data_length[128] = "";
		char write_length[128] = "";
		
		if (closed_flag == true)
		{
			write_wifi_command("tcpc 18.191.102.247 8000 \r\n", 10);
		}
		
		char post_string[100];
		sprintf(post_string, "POST /image HTTP/1.1\r\nHost: 10.0.0.0\r\nContent-Type: imagebin\r\nContent-Length: \r\n\r\n");
		uint8_t len_post_string = strlen(post_string);
		
		sprintf(write_length, "write 0 %d\r\n", len_post_string+image_string_length+image_length_JPEG+1);
		usart_write_line(BOARD_USART, write_length);
		//write_wifi_command(write_length,10);
		delay_ms(10);
		
		sprintf(data_length, "POST /image HTTP/1.1\r\nHost: 10.0.0.0\r\nContent-Type: imagebin\r\nContent-Length: %d\r\n\r\n", image_length_JPEG+1); 
		usart_write_line(BOARD_USART, data_length);
		//write_wifi_command(data_length, 10);
		
		for (int i = 0; i < image_length_JPEG+1; i++)
		{
			current_byte = image_buffer[start_image + i];
			usart_putchar(BOARD_USART, current_byte);					
		}
            
		usart_write_line(BOARD_USART, "read 0 1000 \r\n");
		//write_wifi_command("read 0 1000 \r\n", 10);
		
		closed_flag = false;
		
		delay_ms(50);
	}

}


