/**
 * \file
 *
 * \brief User board configuration template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

#define BOARD_FREQ_SLCK_XTAL      (32768U)
#define BOARD_FREQ_SLCK_BYPASS    (32768U)
#define BOARD_FREQ_MAINCK_XTAL    (12000000U)
#define BOARD_FREQ_MAINCK_BYPASS  (12000000U)
#define BOARD_MCK                 CHIP_FREQ_CPU_MAX
#define BOARD_OSC_STARTUP_US      15625

#define WIFI_CTS_PIN		PIO_PA7_IDX
#define WIFI_RESET_PIN		PIO_PB0_IDX
#define NETWORK_STATUS_PIN	PIO_PB11_IDX

#define LED1_PIN			PIO_PA3_IDX		// LED 6
#define LED2_PIN			PIO_PA2_IDX		// LED 5
#define LED3_PIN			PIO_PA30_IDX	// LED 7
#define LED4_PIN			PIO_PA27_IDX	// LED 9
#define LED5_PIN			PIO_PA29_IDX	// LED 8


#endif // CONF_BOARD_H
