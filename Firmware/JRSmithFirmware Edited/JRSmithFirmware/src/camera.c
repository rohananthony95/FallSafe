/*
 * camera.c
 *
 * Created: 2/11/2018 2:49:22 PM
 *  Author: ece-lab3
 */ 

#include "camera.h"

volatile bool vsync_flag = false;
volatile int image_length_JPEG = 0;
volatile int start_image = 0;
volatile bool start_image_flag = false;
volatile int end_image = 0;
volatile bool end_image_flag = false;

uint8_t *g_p_uc_cap_dest_buf;


void vsync_handler(uint32_t ul_id, uint32_t ul_mask)
{
	unused(ul_id);
	unused(ul_mask);

	vsync_flag = true;
}

void init_vsync_interrupts(void)
{
	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(VSYNC_PIO, VSYNC_ID, VSYNC_PIN_MSK, VSYNC_TYPE, vsync_handler);

	/* Enable PIO controller IRQs */
	NVIC_EnableIRQ((IRQn_Type)VSYNC_ID);
}

void init_camera(void)
{
	gpio_configure_pin(OV_DATA_BUS_D0, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D1, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D2, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D3, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D4, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D5, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D6, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D7, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(XCLK_GPIO, XCLK_FLAGS);
	gpio_configure_pin(OV_HREF_GPIO, OV_HREF_FLAGS);
	gpio_configure_pin(VSYNC_PIN_GPIO, VSYNC_FLAGS);
	
	pio_set_peripheral(PIOA, PIO_PERIPH_B, PIO_PA17);
	
	/* Init Vsync handler*/
	init_vsync_interrupts();

	/* Init PIO capture*/
	pio_capture_init(OV_DATA_BUS_PIO, OV_DATA_BUS_ID);

	/* Turn on ov7740 image sensor using power pin */
	// "Set the CAM reset pin high" - Ilya
	gpio_set_pin_high(RST_CAM_PIN);
	//	ov_power(true, OV_POWER_PIO, OV_POWER_MASK);

	pmc_enable_pllbck(3, 0x1, 1);
	
	/* Init PCK0 to work at 24 Mhz */
	/* 96/4=24 Mhz */
	PMC->PMC_PCK[1] = (PMC_PCK_PRES_CLK_1 | PMC_PCK_CSS_PLLB_CLK);
	PMC->PMC_SCER = PMC_SCER_PCK1;
	while (!(PMC->PMC_SCSR & PMC_SCSR_PCK1)) {
	}
	
	configure_twi();
}


void configure_twi(void)
{
	gpio_configure_pin(TWI0_DATA_GPIO, TWI0_DATA_FLAGS);
	gpio_configure_pin(TWI0_CLK_GPIO, TWI0_CLK_FLAGS);
	
	twi_options_t opt;
	
	/* Enable TWI peripheral */
	pmc_enable_periph_clk(ID_BOARD_TWI);

	/* Init TWI peripheral */
	opt.master_clk = sysclk_get_cpu_hz();
	opt.speed      = TWI_CLK;
	twi_master_init(BOARD_TWI, &opt);

	/* Configure TWI interrupts */
	NVIC_DisableIRQ(BOARD_TWI_IRQn);
	NVIC_ClearPendingIRQ(BOARD_TWI_IRQn);
	NVIC_SetPriority(BOARD_TWI_IRQn, 0);
	NVIC_EnableIRQ(BOARD_TWI_IRQn);
}

void configure_camera(void)
{
	/* ov7740 Initialization */
	while (ov_init(BOARD_TWI) == 1) {
	}
	
	/* ov7740 configuration */
	ov_configure(BOARD_TWI, JPEG_INIT);
	ov_configure(BOARD_TWI, YUV422);
	ov_configure(BOARD_TWI, JPEG);
	ov_configure(BOARD_TWI, JPEG_320x240);
	//ov_configure(BOARD_TWI, JPEG_1600x1200);

	/* Wait 3 seconds to let the image sensor to adapt to environment */
	delay_ms(3000);
}


void pio_capture_init(Pio *p_pio, uint32_t ul_id)
{
	/* Enable periphral clock */
	pmc_enable_periph_clk(ul_id);

	/* Disable pio capture */
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_PCEN);

	/* Disable rxbuff interrupt */
	p_pio->PIO_PCIDR |= PIO_PCIDR_RXBUFF;

	/* 32bit width*/
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_DSIZE_Msk);
	p_pio->PIO_PCMR |= PIO_PCMR_DSIZE_WORD;

	/* Only HSYNC and VSYNC enabled */
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_ALWYS);
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_HALFS);
	
//#if !defined(DEFAULT_MODE_COLORED)
	/* Samples only data with even index */
	//p_pio->PIO_PCMR |= PIO_PCMR_HALFS;
	//p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_FRSTS);
//#endif
}

uint8_t pio_capture_to_buffer(Pio *p_pio, uint8_t *uc_buf, uint32_t ul_size)
{
	/* Check if the first PDC bank is free */
	if ((p_pio->PIO_RCR == 0) && (p_pio->PIO_RNCR == 0)) 
	{
		p_pio->PIO_RPR = (uint32_t)uc_buf;
		p_pio->PIO_RCR = ul_size;
		p_pio->PIO_PTCR = PIO_PTCR_RXTEN;
		return 1;
	} 
	
	else if (p_pio->PIO_RNCR == 0) 
	{
		p_pio->PIO_RNPR = (uint32_t)uc_buf;
		p_pio->PIO_RNCR = ul_size;
		return 1;
	} 
	
	else 
	{
		return 0;
	}
}

uint8_t start_capture(void)
{
    for (int jj=0;jj<MAX_BUFF;jj++) image_buffer[jj] = 0;

	g_p_uc_cap_dest_buf = (uint8_t *)image_buffer;


	/* Enable vsync interrupt*/
	pio_enable_interrupt(VSYNC_PIO, VSYNC_PIN_MSK);

	/* Capture acquisition will start on rising edge of Vsync signal.
	 * So wait vsync_flag = 1 before start process
	 */
	while (!vsync_flag) {
	}

	/* Disable vsync interrupt*/
	pio_disable_interrupt(VSYNC_PIO, VSYNC_PIN_MSK);

	/* Enable pio capture*/
	pio_capture_enable(OV_DATA_BUS_PIO);

	/* Capture data and send it to external SRAM memory thanks to PDC
	 * feature */
	pio_capture_to_buffer(OV_DATA_BUS_PIO, g_p_uc_cap_dest_buf, (MAX_BUFF) >> 2);

	/* Wait end of capture*/
	while (!((OV_DATA_BUS_PIO->PIO_PCISR & PIO_PCIMR_RXBUFF) == PIO_PCIMR_RXBUFF)) {
	}

	/* Disable pio capture*/
	pio_capture_disable(OV_DATA_BUS_PIO);

	/* Reset vsync flag*/
	vsync_flag = false;
	
	return find_image_len();

}

uint8_t find_image_len(void)
{
	char c1 = 0x0;
	char c2 = 0x0;
	
	start_image_flag = false;
	end_image_flag = false;
	start_image = 0;
	end_image = 0;
	
	for (uint32_t ii = 1; ii < MAX_BUFF; ii++)
	{
		c1 = image_buffer[ii-1];
		c2 = image_buffer[ii];
		
		if (c1 == 0xff)
		{
			if (c2 == 0xd8)
			{
				// start of image
				start_image = ii-1;
				start_image_flag = true;
			}
			if (c2 == 0xd9 && start_image_flag && ii > start_image)
			{
				// end of image
				end_image = ii;
				end_image_flag = true;
				break;
			}
		}
	}
	
	image_length_JPEG = end_image - start_image;
	
	if ((image_length_JPEG == 0) || ((start_image_flag == false) || (end_image_flag == false)))
	{
		return 0;
	}
	
	else
	{
		return 1;
	}
}
