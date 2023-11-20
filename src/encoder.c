/// Functions to set up and use the main quadrature encoder.
/// Configures TIM5, COMP5, COMP6, COMP7 and connects them accordingly

#include "stm32g4xx_hal.h"

TIM_HandleTypeDef encoder_timer;
COMP_HandleTypeDef enc_a, enc_b, enc_i;

void encoder_setup(void)
{
	encoder_timer_setup();
}

void encoder_timer_setup(void)
{
	TIM_Encoder_InitTypeDef config = {0};

	encoder_timer.Instance = TIM5;
	config.EncoderMode = TIM_ENCODERMODE_TI12;
	config.IC1Filter;

	if (HAL_TIM_Encoder_Init(&encoder_timer, &config) != HAL_OK)
		error_handler();
}

void encoder_comp_setup(void)
{
	COMP_InitTypeDef config = {0};

	enc_a.Instance = COMP5;
	config.InputPlus = COMP_INPUT_PLUS_IO2;
	config.InputMinus = COMP_INPUT_MINUS_IO2;
	config.OutputPol = COMP_OUTPUTPOL_NONINVERTED;
	config.TriggerMode = COMP_TRIGGERMODE_NONE;
	config.BlankingSrce = COMP_BLANKINGSRC_NONE;
	config.Hysteresis = COMP_HYSTERESIS_40MV;

	if (HAL_COMP_Init(&enc_a, &config) != HAL_OK)
		error_handler();

	enc_i.Instance = COMP7;
	config.TriggerMode = COMP_TRIGGERMODE_EVENT_FALLING;
}