/* -----------------------------------------------------------------------------
 * pwm.c                                           (c) 2017 Rocks
 * -----------------------------------------------------------------------------
 * Author: Gertjan Rocks
 * Web:    www.gertjanrocks.com
 * Mail:   gertjanrocks@outlook.com
 * -----------------------------------------------------------------------------
 * Description: 
 * -----------------------------------------------------------------------------
 * Created on: 19 mrt. 2017
 * -----------------------------------------------------------------------------
 */

#include "device.h"

#ifdef STM32F0XX

#include "pwm.h"
/* --------------*
 * Include files *
 * --------------*
 */

/* -------------------------------*
 * Constant and macro definitions *
 * -------------------------------*
 */

enum {
	FAULT, OK
};

enum {
	TIM1_Settings, TIM_Settings_Last
};

/* -----------------*
 * Type definitions *
 * -----------------*
 */

typedef struct {
	uint32_t timer_period;
	uint16_t pwm_frequency;
} timer_settings_t;

/* ---------------------*
 * File-scope variables *
 * ---------------------*
 */

static timer_settings_t timer_settings[TIM_Settings_Last];

/* ----------------------*
 * Function declarations *
 * ----------------------*
 */

/* ----------------------*
 * Function definitions  *
 * ----------------------*
 */

/**
 * Init by setting timer settings to zero
 */
void pwm_init(void) {
	//Set all settings to zero
	for (uint8_t index = 0; index > TIM_Settings_Last; index++) {
		timer_settings[index].pwm_frequency = 0;
		timer_settings[index].timer_period = 0;
	}
}

/**
 * open a PWM channel with a PWM frequency
 * @param channel
 * @param pwm_freq
 * @return FAULT or OK
 */
int pwm_open(pwm_channel_t channel, uint16_t pwm_freq) {
	int returnValue = FAULT;
	GPIO_InitTypeDef GPIO_InitStructure;

	//First init the output channel
	if (device_config.pwm_channelConfig[channel].Port == GPIOA) {
		//Enable the GPIO clock output
		RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
		returnValue = OK;
	} else {
		returnValue = FAULT;
	}

	//Only continue if clock is set
	if (returnValue == OK) {
		//Set the GPIO init struct
		GPIO_InitStructure.GPIO_Pin = device_config.pwm_channelConfig[channel].Pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(device_config.pwm_channelConfig[channel].Port, &GPIO_InitStructure);
		//Set to alternative function
		GPIO_PinAFConfig(device_config.pwm_channelConfig[channel].Port,
				device_config.pwm_channelConfig[channel].Pin_Source,
				device_config.pwm_channelConfig[channel].Alternative_Function);
	}

	//Set clock settings
	if (returnValue == OK) {
		if (device_config.pwm_channelConfig[channel].Timer == TIM1) {
			// TIM1 clock enable
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
			returnValue = OK;
		} else {
			returnValue = FAULT;
		}
	}

	//Init PWM frequency
	if (returnValue == OK) {
		//It is possible to have multiple channels for each timer however they will share the
		//pwm frequency. When the pwm frequency is changed this will be changed for each channel
		pwm_change_frequency(channel, pwm_freq);
	}

	return returnValue;
}

/**
 * Change the frequency of a PWM channel or multiple channels
 * @param channel
 * @param pwm_frequency
 * @return
 */
int pwm_change_frequency(pwm_channel_t channel, uint16_t pwm_frequency) {
	int returnValue = FAULT;
	TIM_TypeDef *pTimer;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	uint8_t timer_setting_index;

	//Get the timer pointer
	pTimer = device_config.pwm_channelConfig[channel].Timer;

	//Select timer setting index
	if (pTimer == TIM1) {
		timer_setting_index = TIM1_Settings;
		//Save PWM freq
		timer_settings[timer_setting_index].pwm_frequency = pwm_frequency;
		returnValue = OK;
	} else {
		returnValue = FAULT;
	}

	//Only continue on valid settings
	if (returnValue == OK) {
		// TIM counter enable
		TIM_Cmd(pTimer, DISABLE);

		// Compute the value to be set in ARR regiter to generate the pwn frequency
		timer_settings[timer_setting_index].timer_period = (SystemCoreClock
				/ pwm_frequency) - 1;
		// Time Base configuration
		TIM_TimeBaseStructure.TIM_Prescaler = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_Period =
				timer_settings[timer_setting_index].timer_period;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		//Set the config
		TIM_TimeBaseInit(pTimer, &TIM_TimeBaseStructure);

		// TIM counter enable
		TIM_Cmd(pTimer, ENABLE);
	}
	return returnValue;
}

/**
 * Change the duty cycle in pre mile
 * @param channel
 * @param dutyCycle 0 - 1000
 * @return
 */
int pwm_change_dutyCyle(pwm_channel_t channel, uint16_t dutyCycle) {
	int returnValue = FAULT;
	TIM_TypeDef *pTimer;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	uint32_t ChannelPulse;
	uint8_t timer_setting_index;

	//Get the timer pointer
	pTimer = device_config.pwm_channelConfig[channel].Timer;

	//Select timer setting index
	if (pTimer == TIM1) {
		timer_setting_index = TIM1_Settings;
		//Save PWM freq
		returnValue = OK;
	} else {
		returnValue = FAULT;
	}

	if (returnValue == OK) {
		// TIM counter enable
		TIM_Cmd(pTimer, DISABLE);

		// TIM Main Output Disable
		TIM_CtrlPWMOutputs(pTimer, DISABLE);

		//Configure the channel for pmm mode
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

		//Calculate the pwm channel
		ChannelPulse = (uint16_t) (((uint32_t) dutyCycle
				* (timer_settings[timer_setting_index].timer_period - 1))
				/ PWM_MAX_DUTYCYLE);

		TIM_OCInitStructure.TIM_Pulse = ChannelPulse;
		switch (device_config.pwm_channelConfig[channel].Timer_Channel) {
		case 1:
			TIM_OC1Init(pTimer, &TIM_OCInitStructure);
			break;
		case 2:
			TIM_OC2Init(pTimer, &TIM_OCInitStructure);
			break;
		case 3:
			TIM_OC3Init(pTimer, &TIM_OCInitStructure);
			break;
		case 4:
			TIM_OC4Init(pTimer, &TIM_OCInitStructure);
			break;
		}

		/* TIM1 counter enable */
		TIM_Cmd(pTimer, ENABLE);

		/* TIM1 Main Output Enable */
		TIM_CtrlPWMOutputs(pTimer, ENABLE);

	}
	return returnValue;
}

#endif /* STM32F0XX */
