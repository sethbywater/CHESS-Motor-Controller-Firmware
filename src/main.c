#include "main.h"

#include <stdint.h>
#include "stm32g4xx.h"
#include "stm32g473xx.h"
#include "stm32g4xx_hal.h"

#define LED_PIN 5

TIM_HandleTypeDef motor;
TIM_HandleTypeDef encoder;
TIM_HandleTypeDef aux_encoder;

void main(void)
{
	HAL_Init();

	system_clock_config();
	// do two dummy reads after enabling the peripheral clock, as per the errata
	volatile uint32_t dummy;
	dummy = RCC->AHB1ENR;
	dummy = RCC->AHB1ENR;

	// motor->CR1 |=
	// CENTER;

	// GPIOA->MODER |= (1 << GPIO_MODER_MODER5_Pos);

	// while (1)
	// {
	// 	GPIOA->ODR ^= (1 << LED_PIN);
	// 	for (uint32_t i = 0; i < 1000000; i++)
	// 		;
	// }
}

void system_clock_config(void)
{
}

void motor_timer_setup(void)
{
	motor.Instance = TIM20;
}

void encoder_timer_setup(void)
{
	encoder.Instance = TIM5;
}

void aux_encoder_timer_setup(void)
{
	// aux_encoder.Instance =
}
