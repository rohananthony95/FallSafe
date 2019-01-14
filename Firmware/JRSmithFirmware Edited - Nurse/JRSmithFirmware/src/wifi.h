/*
 * wifi.h
 *
 * Created: 2/11/2018 2:49:07 PM
 *  Author: ece-lab3
 */ 

#ifndef WIFI_H_
#define WIFI_H_

#include <asf.h>
#include <string.h>
#include "timer_interface.h"
#include "conf_board.h"
#include "conf_clock.h"

#define BOARD_USART					USART0
#define BOARD_USART_ID				ID_USART0
#define BOARD_USART_BAUDRATE		115200
#define WIFI_USART_HANDLER			USART0_Handler
#define BOARD_USART_IRQn			USART0_IRQn
#define BOARD_USART_CHAR_LENGTH		US_MR_CHRL_8_BIT
#define BOARD_USART_PARITY			US_MR_PAR_NO
#define BOARD_USART_STOP_BITS		US_MR_NBSTOP_1_BIT
#define BOARD_USART_MODE			US_MR_CHMODE_NORMAL
 
volatile bool start_transfer_flag ;
volatile bool none_flag;
volatile bool closed_flag;
volatile bool fall_flag;
volatile bool okay_flag;
 
volatile uint32_t received_byte_wifi;
volatile bool new_rx_wifi;
#define MAX_INPUT_WIFI		1000
volatile char input_line_wifi[MAX_INPUT_WIFI];
volatile char image_length_JPEG_string[20];
volatile int image_string_length;
volatile unsigned int input_pos_wifi;
volatile bool web_setup_flag;
volatile bool led_rst_flag;
volatile bool command_received_flag;

#define WIFI_COMM_PIN_MSK       PIO_PB10
#define WIFI_COMM_PIO           PIOB
#define WIFI_COMM_ID            ID_PIOB
#define WIFI_COMM_TYPE          PIO_INPUT
#define WIFI_COMM_ATTR          PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE

#define WEB_SETUP_PIN_MSK       PIO_PA22
#define WEB_SETUP_PIO           PIOA
#define WEB_SETUP_ID            ID_PIOA
#define WEB_SETUP_TYPE          PIO_INPUT
#define WEB_SETUP_ATTR          PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE

#define LED_RST_PIN_MSK       PIO_PA0
#define LED_RST_PIO           PIOA
#define LED_RST_ID            ID_PIOA
#define LED_RST_TYPE          PIO_INPUT
#define LED_RST_ATTR          PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE

/** USART0 pin RX */
#define PIN_USART0_RXD	  {PIO_PA5A_RXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_RXD_IDX        (PIO_PA5_IDX)
#define PIN_USART0_RXD_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)
/** USART0 pin TX */
#define PIN_USART0_TXD    {PIO_PA6A_TXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_TXD_IDX        (PIO_PA6_IDX)
#define PIN_USART0_TXD_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)
/** USART0 pin CTS */
#define PIN_USART0_CTS    {PIO_PA8A_CTS0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_CTS_IDX        (PIO_PA8_IDX)
#define PIN_USART0_CTS_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)
/** USART0 pin RTS */
#define PIN_USART0_RTS    {PIO_PA7A_RTS0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_RTS_IDX        (PIO_PA7_IDX)
#define PIN_USART0_RTS_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)

#define ALL_INTERRUPT_MASK  0xffffffff

void WIFI_USART_HANDLER(void);

void process_incoming_byte_wifi(uint8_t in_byte);

void wifi_command_response_handler(uint32_t ul_id, uint32_t ul_mask);

void process_data_wifi (void);

void wifi_web_setup_handler(uint32_t ul_id, uint32_t ul_mask);

void led_rst_handler(uint32_t ul_id, uint32_t ul_mask);

void configure_usart_wifi(void);

void configure_wifi_comm_pin(void);

void configure_wifi_web_setup_pin(void);

void configure_led_rst_pin(void);

void write_wifi_command(char* comm, uint8_t cnt);

void query_server(void);


//void write_image_to_file(void);


#endif /* WIFI_H_ */