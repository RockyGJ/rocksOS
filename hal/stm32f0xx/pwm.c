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

#define NMBR_OF_TIMERS_USED	(1)

enum {
	FAULT, OK
};

/* -----------------*
 * Type definitions *
 * -----------------*
 */

typedef struct{
	uint32_t timer_period;
	uint16_t pwm_frequency;
}timer_settings_t;


/* ---------------------*
 * File-scope variables *
 * ---------------------*
 */

static timer_settings_t timer_settings[NMBR_OF_TIMERS_USED];

/* ----------------------*
 * Function declarations *
 * ----------------------*
 */

/* ----------------------*
 * Function definitions  *
 * ----------------------*
 */

void pwm_init(void) {
	//Set all settings to zero
	for(uint8_t index = 0; index > NMBR_OF_TIMERS_USED; index++){
		timer_settings[index].pwm_frequency = 0;
		timer_settings[index].timer_period = 0;
	}
}

int pwm_open(uint8_t channel, uint16_t pwm_freq) {
	int returnValue = FAULT;
	GPIO_InitTypeDef GPIO_InitStructure;


	//First init the output channel
	if (device_config.pwm_channel[channel].Port == GPIOA) {
		//Enable the GPIO clock output
		RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
		returnValue = OK;
	} else {
		returnValue = FAULT;
	}

	//Only continue if clock is set
	if (returnValue == OK) {
		//Set the GPIO init struct
		GPIO_InitStructure.GPIO_Pin = device_config.pwm_channel[channel].Pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(device_config.pwm_channel[channel].Port, &GPIO_InitStructure);
		//Set to alternative function
		GPIO_PinAFConfig(device_config.pwm_channel[channel].Port,
				device_config.pwm_channel[channel].Pin_Source,
				device_config.pwm_channel[channel].Alternative_Function);

		//Select the timer for the settings.
		//It is possible to have multiple channels for each timer however they will share the
		//pwm frequency. When the pwm frequency is changed this will be changed for each channel
		if (channel < PWM_CHANNELS_ON_FIRST_TIMER) {
			//Init the clock for the timer
			if (PWM_FIRST_TIMER == TIM1) {
				// TIM1 clock enable
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
				returnValue = OK;
			} else {
				returnValue = FAULT;
			}

			//If the clock settings are valid continue with init for the timer
			if (returnValue == OK) {
				//init the pwm freq
				pwm_change_frequency(channel,pwm_freq);


				/* Channel 1, 2, 3 and 4 Configuration in PWM mode */
				TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
				TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
				TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
				TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
				TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
				TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
				TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
			}

		}

		/* TIM1 Configuration ---------------------------------------------------
		 Generate PWM signals with 4 different duty cycles:
		 TIM1 input clock (TIM1CLK) is set to APB2 clock (PCLK2)
		 => TIM1CLK = PCLK2 = SystemCoreClock
		 TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
		 SystemCoreClock is set to 48 MHz for STM32F0xx devices

		 The objective is to generate 4 PWM signal at 17.57 KHz:
		 - TIM1_Period = (SystemCoreClock / 17570) - 1
		 The channel 1 and channel 1N duty cycle is set to 50%
		 The channel 2 and channel 2N duty cycle is set to 37.5%
		 The channel 3 and channel 3N duty cycle is set to 25%
		 The channel 4 duty cycle is set to 12.5%
		 The Timer pulse is calculated as follows:
		 - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100

		 Note:
		 SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f0xx.c file.
		 Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
		 function to update SystemCoreClock variable value. Otherwise, any configuration
		 based on this variable will be incorrect.
		 ----------------------------------------------------------------------- */

		/* Compute CCR1 value to generate a duty cycle at 50% for channel 1 */
		Channel1Pulse = (uint16_t) (((uint32_t) 5 * (TimerPeriod - 1)) / 10);
		/* Compute CCR2 value to generate a duty cycle at 37.5%  for channel 2 */
		Channel2Pulse =
				(uint16_t) (((uint32_t) 375 * (TimerPeriod - 1)) / 1000);
		/* Compute CCR3 value to generate a duty cycle at 25%  for channel 3 */
		Channel3Pulse = (uint16_t) (((uint32_t) 25 * (TimerPeriod - 1)) / 100);
		/* Compute CCR4 value to generate a duty cycle at 12.5%  for channel 4 */
		Channel4Pulse =
				(uint16_t) (((uint32_t) 125 * (TimerPeriod - 1)) / 1000);

		TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
		TIM_OC1Init(TIM1, &TIM_OCInitStructure);

		TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
		TIM_OC2Init(TIM1, &TIM_OCInitStructure);

		TIM_OCInitStructure.TIM_Pulse = Channel3Pulse;
		TIM_OC3Init(TIM1, &TIM_OCInitStructure);

		TIM_OCInitStructure.TIM_Pulse = Channel4Pulse;
		TIM_OC4Init(TIM1, &TIM_OCInitStructure);

		/* TIM1 counter enable */
		TIM_Cmd(TIM1, ENABLE);

		/* TIM1 Main Output Enable */
		TIM_CtrlPWMOutputs(TIM1, ENABLE);

	}

	return returnValue;
}

/**
 * Change the frequency of a pwm channel of multiple channels
 * @param channel
 * @param pwm_frequency
 * @return
 */
int pwm_change_frequency(uint8_t channel, uint16_t pwm_frequency) {
	int returnValue = FAULT;
	TIM_TypeDef Timer;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	uint8_t timer_setting_index;

	if (channel < PWM_CHANNELS_ON_FIRST_TIMER) {
		Timer = PWM_FIRST_TIMER;
		//Index for first timer is 0
		timer_setting_index = 0;
		//Save PWM freq
		timer_settings[timer_setting_index].pwm_frequency = pwm_frequency;
		returnValue = OK;
	} else {
		returnValue = FAULT;
	}

	if (returnValue == OK) {
		// TIM counter enable
		TIM_Cmd(Timer, DISABLE);

		// Compute the value to be set in ARR regiter to generate the pwn frequency
		timer_settings[timer_setting_index].timer_period = (SystemCoreClock / pwm_frequency) - 1;
		// Time Base configuration
		TIM_TimeBaseStructure.TIM_Prescaler = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_Period = timer_settings[timer_setting_index].timer_period;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		//Set the config
		TIM_TimeBaseInit(Timer, &TIM_TimeBaseStructure);

		// TIM counter enable
		TIM_Cmd(Timer, ENABLE);
	}
	return returnValue;
}

/**
 * Change the duty cycle in pre mile
 * @param channel
 * @param dutyCycle 0 - 1000
 * @return
 */
int pwm_change_dutyCyle(uint8_t channel, uint16_t dutyCycle) {
	int returnValue = FAULT;
	TIM_TypeDef Timer;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	uint32_t ChannelPulse;
	uint8_t timer_setting_index;

	if (channel < PWM_CHANNELS_ON_FIRST_TIMER) {
		Timer = PWM_FIRST_TIMER;
		//Index for first timer is 0
		timer_setting_index = 0;
		//Save PWM freq
		timer_settings[timer_setting_index].pwm_frequency = pwm_frequency;
		returnValue = OK;
	} else {
		returnValue = FAULT;
	}

	if (returnValue == OK) {
		// TIM counter enable
		TIM_Cmd(Timer, DISABLE);

		// TIM Main Output Enable
		TIM_CtrlPWMOutputs(Timer, DISABLE);

		//Configure the channel for pmm mode
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

		//Calculate the pwm channel
		ChannelPulse =
				(uint16_t) (((uint32_t) dutyCycle * (timer_settings[timer_setting_index].timer_period - 1)) / PWM_MAX_DUTYCYLE);


#error BEGIN MELDING
		TImer nummer toevoegen en timer channel toevegen aan de config dan hoef je nergens de timer number op te zoeen dan
		kan de groote timer worden gebruikt!

#error EINDE MELDING
	}


/* TIM1 Configuration ---------------------------------------------------
 Generate PWM signals with 4 different duty cycles:
 TIM1 input clock (TIM1CLK) is set to APB2 clock (PCLK2)
 => TIM1CLK = PCLK2 = SystemCoreClock
 TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
 SystemCoreClock is set to 48 MHz for STM32F0xx devices

 The objective is to generate 4 PWM signal at 17.57 KHz:
 - TIM1_Period = (SystemCoreClock / 17570) - 1
 The channel 1 and channel 1N duty cycle is set to 50%
 The channel 2 and channel 2N duty cycle is set to 37.5%
 The channel 3 and channel 3N duty cycle is set to 25%
 The channel 4 duty cycle is set to 12.5%
 The Timer pulse is calculated as follows:
 - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100

 Note:
 SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f0xx.c file.
 Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
 function to update SystemCoreClock variable value. Otherwise, any configuration
 based on this variable will be incorrect.
 ----------------------------------------------------------------------- */

/* Compute CCR1 value to generate a duty cycle at 50% for channel 1 */
Channel1Pulse = (uint16_t) (((uint32_t) 5 * (TimerPeriod - 1)) / 10);
/* Compute CCR2 value to generate a duty cycle at 37.5%  for channel 2 */

/* Compute CCR3 value to generate a duty cycle at 25%  for channel 3 */
Channel3Pulse = (uint16_t) (((uint32_t) 25 * (TimerPeriod - 1)) / 100);
/* Compute CCR4 value to generate a duty cycle at 12.5%  for channel 4 */
Channel4Pulse =
		(uint16_t) (((uint32_t) 125 * (TimerPeriod - 1)) / 1000);

TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
TIM_OC1Init(TIM1, &TIM_OCInitStructure);

TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
TIM_OC2Init(TIM1, &TIM_OCInitStructure);

TIM_OCInitStructure.TIM_Pulse = Channel3Pulse;
TIM_OC3Init(TIM1, &TIM_OCInitStructure);

TIM_OCInitStructure.TIM_Pulse = Channel4Pulse;
TIM_OC4Init(TIM1, &TIM_OCInitStructure);

/* TIM1 counter enable */
TIM_Cmd(TIM1, ENABLE);

/* TIM1 Main Output Enable */
TIM_CtrlPWMOutputs(TIM1, ENABLE);




		// Compute the value to be set in ARR regiter to generate the pwn frequency
		timer_settings[timer_setting_index].timer_period = (SystemCoreClock / pwm_frequency) - 1;
		// Time Base configuration
		TIM_TimeBaseStructure.TIM_Prescaler = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_Period = timer_settings[timer_setting_index].timer_period;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		//Set the config
		TIM_TimeBaseInit(Timer, &TIM_TimeBaseStructure);

		// TIM counter enable
		TIM_Cmd(Timer, ENABLE);

		// TIM Main Output Enable
		TIM_CtrlPWMOutputs(Timer, ENABLE);
	}
}

#endif /* STM32F0XX */
