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


#ifndef DEVICE_CONFIG_LPC1769_LPCXPRESSO_H_
#define DEVICE_CONFIG_LPC1769_LPCXPRESSO_H_

#ifdef __cplusplus
extern "C" {
#endif

/* --------------*
 * Include files *
 * --------------*
 */

#include "stdint.h"

//LPC includes
#include "chip.h"


/**
 * PWM Settings
 */
#define PWM_CHANNELS				(4)

/**
 * one pwm channel
 */
//typedef struct{
//	uint16_t 			Pin;
//	GPIO_TypeDef 		*Port;
//	uint8_t 			Pin_Source;
//	uint8_t				Alternative_Function;
//	TIM_TypeDef 		*Timer;
//	uint8_t				Timer_Channel;
//}pwm_channel_config_t;


/**
 * GPIO Settings
 */
#define GPIO_PINS					(11)

/**
 * one gpio pin
 */
//typedef struct{
//	uint16_t 			Pin;
//	GPIO_TypeDef		*Port;
//	GPIOSpeed_TypeDef	Speed_Level;
//}gpio_pin_config_t	;


/**
 * Device config structure
 */
//typedef struct{
//	pwm_channel_config_t 	pwm_channelConfig[PWM_CHANNELS];
//	gpio_pin_config_t		gpio_pinConfig[GPIO_PINS];
//}device_config_t;

/**
 * Default config table
 */

//static const device_config_t device_config = {
///** PWM Channel configuration **/
//	{//		Pin,			Port,	Pin_Source, 		Alternative_Function, T	imer,	Timer_Channel
// /*0*/	{	GPIO_Pin_8,		GPIOA,	GPIO_PinSource8,	GPIO_AF_2,				TIM1,	1	},
// /*1*/	{	GPIO_Pin_9,		GPIOA,	GPIO_PinSource9,	GPIO_AF_2,				TIM1,	2	},
// /*2*/	{	GPIO_Pin_10,	GPIOA,	GPIO_PinSource10,	GPIO_AF_2,				TIM1,	3	},
// /*3*/	{	GPIO_Pin_11,	GPIOA,	GPIO_PinSource11,	GPIO_AF_2,				TIM1,	4	}
//	},
///** GPIO pinnen configuration **/
//	{//		Pin,			Port,	Speed_Level
// /*0*/	{	GPIO_Pin_1,		GPIOA,	GPIO_Speed_Level_3},
// /*1*/	{	GPIO_Pin_2,		GPIOA,	GPIO_Speed_Level_3},
// /*2*/	{	GPIO_Pin_3,		GPIOA,	GPIO_Speed_Level_3},
// /*3*/	{	GPIO_Pin_4,		GPIOA,	GPIO_Speed_Level_3},
// /*4*/	{	GPIO_Pin_5,		GPIOA,	GPIO_Speed_Level_3},
// /*5*/ 	{ 	GPIO_Pin_6,		GPIOC,	GPIO_Speed_Level_3},
// /*6*/ 	{ 	GPIO_Pin_7,		GPIOC,	GPIO_Speed_Level_3},
// /*7*/ 	{ 	GPIO_Pin_8,		GPIOC,	GPIO_Speed_Level_3},
// /*8*/ 	{ 	GPIO_Pin_9,		GPIOC,	GPIO_Speed_Level_3},
// /*9*/ 	{ 	GPIO_Pin_6,		GPIOF,	GPIO_Speed_Level_3},
// /*10*/	{ 	GPIO_Pin_7,		GPIOF,	GPIO_Speed_Level_3},
//	}
//};

#ifdef __cplusplus
}
#endif

#endif /* DEVICE_CONFIG_LPC1769_LPCXPRESSO_H_ */
