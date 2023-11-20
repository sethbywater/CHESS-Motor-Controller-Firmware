#include "stm32g4xx_hal.h"

TIM_HandleTypeDef aux_encoder_timer;
SPI_HandleTypeDef aux_encoder_spi;

void aux_encoder_setup(void)
{
}

void aux_encoder_timer_setup(void)
{
	// aux_encoder.Instance =
}

void aux_encoder_spi_setup(void)
{
	aux_encoder_spi.Instance = SPI4;
}