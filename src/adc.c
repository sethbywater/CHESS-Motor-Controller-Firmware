/// Functions to set up and use the current-sensing ADCs

#include "stm32g4xx_hal.h"

#define ADC_RES_10_BITS ADC_CFGR_RES_1

ADC_HandleTypeDef ph1_current_adc, ph2_current_adc, ph3_current_adc, ph4_current_adc, temp_adc;
OPAMP_HandleTypeDef ph1_current_opamp, ph2_current_opamp, ph3_current_opamp, ph4_current_opamp;

void adc_setup(void)
{
	ph1_current_adc.Instance = ADC2;
	ph2_current_adc.Instance = ADC4;
	ph3_current_adc.Instance = ADC5;
	ph4_current_adc.Instance = ADC3;
	temp_adc.Instance = ADC5;

	ph1_current_adc.Init.Resolution = ADC_RES_10_BITS;

	// From the STM32 manual regarding the temperature sensor:
	// To use the sensor :
	// 1. Select the ADC input channels that is connected to VTS.
	// 2. Program with the appropriate sampling time(refer to electrical characteristics section of the device datasheet)
	// 3. Set the VSENSESEL bit in the ADCx_CCR register to wake up the temperature sensor from power - down mode.
	// 4. Start the ADC conversion.
	// 5. Read the resulting VTS data in the ADC data register.
	// 6. Calculate the actual temperature using the following formula:
}

void opamp_setup(void)
{
	ph1_current_opamp.Instance = OPAMP2;
	ph2_current_opamp.Instance = OPAMP6;
	ph3_current_opamp.Instance = OPAMP4;
	ph4_current_opamp.Instance = OPAMP3;

	// ph1_current_opamp->CSR |= OPAMP_FOLLOWER_MODE | OPAMP_NONINVERTINGINPUT_IO3 | OPAMP_ENABLE;
	// ph2_current_opamp->CSR |= OPAMP_FOLLOWER_MODE | OPAMP_NONINVERTINGINPUT_IO1 | OPAMP_ENABLE;
	// ph3_current_opamp->CSR |= OPAMP_FOLLOWER_MODE | OPAMP_NONINVERTINGINPUT_IO2 | OPAMP_ENABLE;
	// ph4_current_opamp->CSR |= OPAMP_FOLLOWER_MODE | OPAMP_NONINVERTINGINPUT_IO1 | OPAMP_ENABLE;

	ph1_current_opamp.Init.PowerMode = OPAMP_POWERMODE_NORMALSPEED;
	ph1_current_opamp.Init.Mode = OPAMP_FOLLOWER_MODE;
	ph1_current_opamp.Init.NonInvertingInput = OPAMP_NONINVERTINGINPUT_IO3;
	ph1_current_opamp.Init.InternalOutput = ENABLE;
	ph1_current_opamp.Init.TimerControlledMuxmode = ;
	ph1_current_opamp.Init.PgaConnect = OPAMP_PGA_CONNECT_INVERTINGINPUT_NO;
	ph1_current_opamp.Init.PgaGain = OPAMP_PGA_GAIN_2_OR_MINUS_1;
	ph1_current_opamp.Init.UserTrimming = OPAMP_TRIMMING_FACTORY;

	if (HAL_OPAMP_Init(&ph1_current_opamp) + HAL_OPAMP_Init(&ph2_current_opamp) + HAL_OPAMP_Init(&ph3_current_opamp) + HAL_OPAMP_Init(&ph4_current_opamp) != HAL_OK)
		error_handler();
}

void get_phase_currents(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d)
{
	*a = ph1_current_adc.Instance->DR;
	*b = ph2_current_adc.Instance->DR;
	*c = ph3_current_adc.Instance->DR;
	*d = ph3_current_adc.Instance->DR;
}

float temperature()
{
	static uint16_t temp_calibration_factor = (TEMPSENSOR_CAL2_TEMP - TEMPSENSOR_CAL1_TEMP) / (TEMPSENSOR_CAL2_ADDR - TEMPSENSOR_CAL1_ADDR);
	static uint16_t temp_vref_factor = TEMPSENSOR_CAL_VREFANALOG / 3300; // 3300 being our system's VREF (3.3V)

	return (temp_calibration_factor * ((temp_adc->DR * temp_vref_factor) - *TEMPSENSOR_CAL1_ADDR)) + 30;
}

void opamp_set_gains(float gain)
{
	// Taken from the STM32G4 refrence manual:

	// 	The procedure to use the OPAMP in follower mode is presented hereafter.
	// • configure VM_SEL bits as “opamp_out connected to OPAMPx_VINM input”, 11
	// • configure VP_SEL bits as “GPIO connected to OPAMPx_VINP”, 00
	// • As soon as the OPAEN bit is set, the voltage on pin OPAMPx_VINP is buffered to pin OPAMPx_VOUT.

	// 	The procedure to use the OPAMP as programmable gain amplifier is presented hereafter.
	// • configure VM_SEL bits as “Feedback resistor is connected to OPAMPx_VINM input”, 10
	// • configure PGA_GAIN bits as “internal Gain 2, 4, 8, 16, 32, or 64”, 00000 to 00101
	// • configure VP_SEL bits as “GPIO connected to OPAMPx_VINP”, 00 As soon as the OPAEN bit is set, the voltage on pin OPAMPx_VINP is amplified by the selected gain and visible on pin OPAMPx_VOUT.

	uint32_t gain_setting;

	if (gain < 2)
	{
		// Configure in follower mode, gain = 1
		ph1_current_opamp->CSR |= OPAMP_CSR_VMSEL;
		ph2_current_opamp->CSR |= OPAMP_CSR_VMSEL;
		ph3_current_opamp->CSR |= OPAMP_CSR_VMSEL;
		ph4_current_opamp->CSR |= OPAMP_CSR_VMSEL;
	}
	else
	{
		// Configure in PGA Mode
		ph1_current_opamp->CSR &= OPAMP_CSR_VMSEL_0;
		ph2_current_opamp->CSR &= OPAMP_CSR_VMSEL_0;
		ph3_current_opamp->CSR &= OPAMP_CSR_VMSEL_0;
		ph4_current_opamp->CSR &= OPAMP_CSR_VMSEL_0;
		// Clear PGA bits
		ph1_current_opamp->CSR &= !(OPAMP_CSR_PGGAIN);
		ph2_current_opamp->CSR &= !(OPAMP_CSR_PGGAIN);
		ph3_current_opamp->CSR &= !(OPAMP_CSR_PGGAIN);
		ph4_current_opamp->CSR &= !(OPAMP_CSR_PGGAIN);
		if (gain > 64)
			gain_setting = OPAMP_PGA_GAIN_64_OR_MINUS_63;
		else if (gain > 32)
			gain_setting = OPAMP_PGA_GAIN_32_OR_MINUS_31;
		else if (gain > 16)
			gain_setting = OPAMP_PGA_GAIN_16_OR_MINUS_15;
		else if (gain > 8)
			gain_setting = OPAMP_PGA_GAIN_8_OR_MINUS_7;
		else if (gain > 4)
			gain_setting = OPAMP_PGA_GAIN_4_OR_MINUS_3;
		else
			gain_setting = OPAMP_PGA_GAIN_2_OR_MINUS_1;

		ph1_current_opamp->CSR |= gain_setting;
		ph2_current_opamp->CSR |= gain_setting;
		ph3_current_opamp->CSR |= gain_setting;
		ph4_current_opamp->CSR |= gain_setting;
	}
}