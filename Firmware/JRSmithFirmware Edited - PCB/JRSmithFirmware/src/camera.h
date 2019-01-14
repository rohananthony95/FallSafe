/*
 * camera.h
 *
 * Created: 2/11/2018 2:49:33 PM
 *  Author: ece-lab3
 */ 


#ifndef CAMERA_H_
#define CAMERA_H_

#include <asf.h>
#include "wifi.h"
#include <string.h>
#include "timer_interface.h"
#include "ov2640.h"
#include "conf_board.h"
#include "conf_clock.h"

volatile bool vsync_flag;
uint8_t *cap_dest_buf;

#define MAX_BUFF		20000
volatile uint8_t image_buffer[MAX_BUFF];
volatile int image_length_JPEG;
volatile int start_image;
volatile bool start_image_flag;
volatile int end_image;
volatile bool end_image_flag;


#define TWI_CLK     (400000UL)

/** TWI0 data pin */
#define PIN_TWI_TWD0                   {PIO_PA3A_TWD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/** TWI0 clock pin */
#define PIN_TWI_TWCK0                  {PIO_PA4A_TWCK0, PIOA, ID_PIOA,	PIO_PERIPH_A, PIO_DEFAULT}

/** TWI0 Data pins definition */
#define TWI0_DATA_GPIO                 PIO_PA3_IDX
#define TWI0_DATA_FLAGS                (PIO_PERIPH_A | PIO_DEFAULT)
#define TWI0_DATA_MASK                 PIO_PA3
#define TWI0_DATA_PIO                  PIOA
#define TWI0_DATA_ID                   ID_PIOA
#define TWI0_DATA_TYPE                 PIO_PERIPH_A
#define TWI0_DATA_ATTR                 PIO_DEFAULT

/** TWI0 clock pins definition */
#define TWI0_CLK_GPIO                  PIO_PA4_IDX
#define TWI0_CLK_FLAGS                 (PIO_PERIPH_A | PIO_DEFAULT)
#define TWI0_CLK_MASK                  PIO_PA4
#define TWI0_CLK_PIO                   PIOA
#define TWI0_CLK_ID                    ID_PIOA
#define TWI0_CLK_TYPE                  PIO_PERIPH_A
#define TWI0_CLK_ATTR                  PIO_DEFAULT

#define CONF_BOARD_TWI0

/** TWI0 pins */
#define PINS_TWI0                      PIN_TWI_TWD0, PIN_TWI_TWCK0

#define ID_BOARD_TWI                   ID_TWI0
#define BOARD_TWI                      TWI0
#define BOARD_TWI_IRQn                 TWI0_IRQn

/** XCLK pin definition. */
#define XCLK_GPIO                  (PIO_PA17_IDX)
#define XCLK_FLAGS                 (PIO_PERIPH_B | PIO_DEFAULT)
#define XCLK_MASK                  PIO_PA17
#define XCLK_PIO                   PIOA
#define XCLK_ID                    ID_PIOA
#define XCLK_TYPE                  PIO_PERIPH_B
#define XCLK_ATTR                  PIO_DEFAULT

#define PIN_PCK1                       XCLK_GPIO
#define PIN_PCK1_FLAGS                 XCLK_FLAGS

/** OV_HSYNC pin definition */
#define OV_HREF_GPIO                  PIO_PA16_IDX
#define OV_HREF_FLAGS                 (PIO_PULLUP | PIO_IT_RISE_EDGE)
#define OV_HREF_MASK                  PIO_PA16
#define OV_HREF_PIO                   PIOA
#define OV_HREF_ID                    ID_PIOA
#define OV_HREF_TYPE                  PIO_PULLUP



/** OV_VSYNC pin definition */
#define VSYNC_PIN_GPIO		PIO_PA15_IDX
#define VSYNC_PIN_MSK       PIO_PA15
#define VSYNC_PIO           PIOA
#define VSYNC_ID            ID_PIOA
#define VSYNC_TYPE          PIO_INPUT
#define VSYNC_ATTR          PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE
#define VSYNC_FLAGS         (PIO_PULLUP | PIO_IT_RISE_EDGE)

/** OV Data Bus pins */
#define OV_DATA_BUS_D0                 PIO_PA24_IDX
#define OV_DATA_BUS_D1                 PIO_PA25_IDX
#define OV_DATA_BUS_D2                 PIO_PA26_IDX
#define OV_DATA_BUS_D3                 PIO_PA27_IDX
#define OV_DATA_BUS_D4                 PIO_PA28_IDX
#define OV_DATA_BUS_D5                 PIO_PA29_IDX
#define OV_DATA_BUS_D6                 PIO_PA30_IDX
#define OV_DATA_BUS_D7                 PIO_PA31_IDX
#define OV_DATA_BUS_FLAGS              (PIO_INPUT | PIO_PULLUP)
#define OV_DATA_BUS_MASK               (0xFF000000UL)
#define OV_DATA_BUS_PIO                PIOA
#define OV_DATA_BUS_ID                 ID_PIOA
#define OV_DATA_BUS_TYPE               PIO_INPUT
#define OV_DATA_BUS_ATTR               PIO_DEFAULT

void vsync_handler(uint32_t ul_id, uint32_t ul_mask);
void init_vsync_interrupts(void);
void configure_twi(void);
void pio_capture_init(Pio *p_pio, uint32_t ul_id);
uint8_t pio_capture_to_buffer(Pio *p_pio, uint8_t *uc_buf, uint32_t ul_size);
void init_camera(void);
void configure_camera(void);
uint8_t start_capture(void);
uint8_t find_image_len(void);



#endif /* CAMERA_H_ */