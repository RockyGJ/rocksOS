/* -----------------------------------------------------------------------------
 * STM32F051R8_LQFP64.h                                           (c) 2017 Rocks
 * -----------------------------------------------------------------------------
 * Author: Gertjan Rocks
 * Web:    www.gertjanrocks.com
 * Mail:   gertjanrocks@outlook.com
 * -----------------------------------------------------------------------------
 * Description: This file containts the configuration for a STM32F051R8 in the
 * LPQFP64 package. This is the only place where pin dedicated are defined
 * -----------------------------------------------------------------------------
 * Created on: 19 mrt. 2017
 * -----------------------------------------------------------------------------
 */


#ifndef DEVICE_CONFIG_STM32F051R8_LQFP64_H_
#define DEVICE_CONFIG_STM32F051R8_LQFP64_H_

#ifdef __cplusplus
extern "C" {
#endif

/* --------------*
 * Include files *
 * --------------*
 */

#include "stdint.h"

//STM includes
#include "stm32f0xx.h"

//Config functions for the STM32F051R8

/**
 * PWM Settings
 */
#define PWM_CHANNELS				(4)
//#define	PWM_FIRST_TIMER				(TIM1)
//#define PWM_CHANNELS_ON_FIRST_TIMER	(4)
/**
 * one pwm channel
 */
typedef struct{
	uint16_t 			Pin;
	GPIO_TypeDef 		*Port;
	uint8_t 			Pin_Source;
	uint8_t				Alternative_Function;
	TIM_TypeDef 		*Timer;
	uint8_t				Timer_Channel;
}pwm_channel_t;

/**
 * Device config structure
 */
typedef struct{
	pwm_channel_t pwm_channel[PWM_CHANNELS];
}device_config_t;

/**
 * Default config table
 */

const device_config_t device_config = {
/** PWM Channel configuration **/
	{//		Pin,			Port,	Pin_Source, 		Alternative_Function, T	imer,	Timer_Channel
		{	GPIO_Pin_8,		GPIOA,	GPIO_PinSource8,	GPIO_AF_2,				TIM1,	1	},
		{	GPIO_Pin_9,		GPIOA,	GPIO_PinSource9,	GPIO_AF_2,				TIM1,	2	},
		{	GPIO_Pin_10,	GPIOA,	GPIO_PinSource10,	GPIO_AF_2,				TIM1,	3	},
		{	GPIO_Pin_11,	GPIOA,	GPIO_PinSource11,	GPIO_AF_2,				TIM1,	4	}
	}
};

#ifdef __cplusplus
}
#endif

#endif /* DEVICE_CONFIG_STM32F051R8_LQFP64_H_ */
