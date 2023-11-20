#include "main.h"

#include <stdint.h>

// SDK
#include "stm32g4xx.h"
#include "stm32g473xx.h"
#include "stm32g4xx_hal.h"

#include "adc.c"
#include "aux_encoder.c"
#include "encoder.c"
#include "error_handler.h"
#include "motor.c"

#define LED_PIN 5

float motor_inductance, motor_resistance;
/// Constants used in back EMF calculations
/// Need to be updated whenever motor_inductance or motor_resistance are changed
float kf, kg;

uint32_t isr_period;

void main(void)
{
	HAL_Init();

	system_clock_config();

	encoder_setup();
	aux_encoder_setup();
	motor_timer_setup();
}

void foc_isr(void)
{
	// Position estimate

	// Torque PI loop
}

float bemf_angle_estimate(float i_alpha, float i_beta, float voltage_alpha, float voltage_beta)
{
	static float last_i_alpha, last_i_beta;
	static float last_voltage_alpha, last_voltage_beta;
	static float last_back_emf_alpha, last_back_emf_beta;

	float est_i_alpha = (kf * last_i_alpha) + (kg * (last_voltage_alpha - last_back_emf_alpha));
	float est_i_beta = (kf * last_i_beta) + (kg * (last_voltage_beta - last_back_emf_beta));

	// This part is wrong, note sure what units are here
	float back_emf_alpha = est_i_alpha - i_alpha;
	float back_emf_beta = est_i_beta - i_beta;

	float angle = atanf(-back_emf_alpha / back_emf_beta);

	last_i_alpha = i_alpha;
	last_i_beta = i_beta;
	last_voltage_alpha = voltage_alpha;
	last_voltage_beta = voltage_beta;
	last_back_emf_alpha = back_emf_alpha;
	last_back_emf_beta = back_emf_beta;

	return angle;
}

void clarke(float u, float v, float w, float *a, float *b)
{
	*a = (1 / 3) * ((2 * u) - v - w);
	*b = (1 / sqrt(3)) * (v - w);
}

void inv_clarke(float a, float b, float *u, float *v, float *w)
{
	*u = a;
	*v = (-a + (sqrt(3) * b)) / 2;
	*w = (-a - (sqrt(3) * b)) / 2;
}

void park(float a, float b, float angle, float *d, float *q)
{
	*d = (a * cosf(angle)) + (b * sinf(angle));
	*q = (b * cosf(angle)) - (a * sinf(angle));
}

void inv_park(float d, float q, float angle, float *a, float *b)
{
	*a = (d * cosf(angle)) - (q * sinf(angle));
	*b = (q * cosf(angle)) + (d * sinf(angle));
}

void system_clock_config(void)
{
	// do two dummy reads after enabling the peripheral clock, as per the errata
	volatile uint32_t dummy;
	dummy = RCC->AHB1ENR;
	dummy = RCC->AHB1ENR;
}
