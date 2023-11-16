#include "main.h"

#include <stdint.h>
#include "stm32g4xx.h"
#include "stm32g473xx.h"
#include "stm32g4xx_hal.h"

#define LED_PIN 5

TIM_HandleTypeDef motor_timer;
TIM_HandleTypeDef encoder_timer;
TIM_HandleTypeDef aux_encoder_timer;
SPI_HandleTypeDef aux_encoder_spi;

void main(void)
{
	HAL_Init();

	system_clock_config();
}

void system_clock_config(void)
{
	// do two dummy reads after enabling the peripheral clock, as per the errata
	volatile uint32_t dummy;
	dummy = RCC->AHB1ENR;
	dummy = RCC->AHB1ENR;
}

void motor_timer_setup(void)
{
	motor_timer.Instance = TIM20;
}

void encoder_timer_setup(void)
{
	encoder_timer.Instance = TIM5;
}

void aux_encoder_timer_setup(void)
{
	// aux_encoder.Instance =
}

void aux_encoder_spi_setup(void)
{
	aux_encoder_spi.Instance = SPI4;
}