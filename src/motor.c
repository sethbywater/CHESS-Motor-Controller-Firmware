#include "stm32g4xx_hal.h"

#include "main.h"

TIM_HandleTypeDef motor_timer;
volatile uint32_t *phaseAp, *phaseAn, *phaseBp, *phaseBn, *phaseU, *phaseV, *phaseW;

/// Set the PWM frequency in Hz
void set_pwm_freq(uint32_t freq)
{
	__HAL_TIM_CALC_PSC(170000000, 256 * freq);
}

void flip_motor_direction()
{
}

void motor_timer_setup(void)
{
	TIM_OC_InitTypeDef config = {0};

	motor_timer.Instance = TIM20;
	motor_timer.Init.= 256;
	config.OCMode = ;

	phaseAp = &motor_timer.Instance->CCR1;
	phaseAn = &motor_timer.Instance->CCR2;
	phaseBp = &motor_timer.Instance->CCR3;
	phaseBn = &motor_timer.Instance->CCR4;

	phaseU = &motor_timer.Instance->CCR1;
	phaseV = &motor_timer.Instance->CCR2;
	phaseW = &motor_timer.Instance->CCR3;
}

void stepper_pwm();

void bldc_pwm(float a, float b)
{
	float u, v, w;

	inv_clarke(a, b, &u, &v, &w);

	*phaseU = (uint32_t)u;
	*phaseV = (uint32_t)v;
	*phaseW = (uint32_t)w;
}

struct Motor
{
	void (*set_phase_voltages)(uint32_t, uint32_t);
};

const struct Motor STEPPER = {
	.set_phase_voltages = &stepper_pwm,
};
