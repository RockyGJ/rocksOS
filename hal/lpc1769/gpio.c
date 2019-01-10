/* -----------------------------------------------------------------------------
 * gpio.c                                           (c) 2017 Rocks
 * -----------------------------------------------------------------------------
 * Author: Gertjan Rocks
 * Web:    www.gertjanrocks.com
 * Mail:   gertjanrocks@outlook.com
 * -----------------------------------------------------------------------------
 * Description: 
 * -----------------------------------------------------------------------------
 * Created on: 30 mrt. 2017
 * -----------------------------------------------------------------------------
 */

#include "device.h"

#ifdef LPC17f69

/* --------------*
 * Include files *
 * --------------*
 */

#include "gpio.h"
#include "stdint.h"
#include "stdbool.h"

/* -------------------------------*
 * Constant and macro definitions *
 * -------------------------------*
 */

enum {
	FAULT, OK
};

/* -----------------*
 * Type definitions *
 * -----------------*
 */

/* ---------------------*
 * File-scope variables *
 * ---------------------*
 */

/* ----------------------*
 * Function declarations *
 * ----------------------*
 */

/* ----------------------*
 * Function definitions  *
 * ----------------------*
 */

/**
 * init the gpio hal layer
 */
void gpio_init(void) {
	//On purpose still empty
}

/**
 * open a pin selected in the device config table and the mode
 * @param gpio_pin
 * @param mode
 * @return
 */
int gpio_open(gpio_pin_t gpio_pin, gpio_mode_t mode) {
	int returnValue = OK;
	GPIO_InitTypeDef GPIO_InitStructure;

	//First enable the GPIO clock
	if (device_config.gpio_pinConfig[gpio_pin].Port == GPIOA) {
		//Enable the GPIO clock
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
		returnValue = OK;
	} else if (device_config.gpio_pinConfig[gpio_pin].Port == GPIOC) {
		//Enable the GPIO clock
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
		returnValue = OK;
	} else if (device_config.gpio_pinConfig[gpio_pin].Port == GPIOF) {
		//Enable the GPIO clock
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
		returnValue = OK;
	} else {
		returnValue = FAULT;
	}

	//If the clock settings are valid enable the pin
	if (returnValue == OK) {

		//First set the Pin en Speed level
		GPIO_InitStructure.GPIO_Pin =
				device_config.gpio_pinConfig[gpio_pin].Pin;
		GPIO_InitStructure.GPIO_Speed =
				device_config.gpio_pinConfig[gpio_pin].Speed_Level;
		//Use the mode to select In/Out PushPull and pulls up
		switch (mode) {
		case GPIO_MODE_INPUT_NOPULL:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
			//Set output type which will not be used but not leave empty
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
			break;
		case GPIO_MODE_INPUT_PULL_UP:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
			//Set output type which will not be used but not leave empty
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			break;
		case GPIO_MODE_INPUT_PULL_DOWN:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
			//Set output type which will not be used but not leave empty
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
			break;
		case GPIO_MODE_OUTPUT_PUSH_PULL:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			//Set puPds which will not be used but not leave empty
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
			break;
		case GPIO_MODE_OUTPUT_OPEN_DRAIN:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
			//Set puPds which will not be used but not leave empty
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
			break;
		default:
			//Unknown value
			returnValue = FAULT;
			break;
		}
		//Configure the pin
		GPIO_Init(device_config.gpio_pinConfig[gpio_pin].Port,
				&GPIO_InitStructure);
	}
	return returnValue;
}

/**
 * set a gpio pin with a gpio pin from the pin table in config
 * @param gpio_pin
 * @param pinState
 * @return
 */
int gpio_set_pin(gpio_pin_t gpio_pin, bool pinState) {
	int returnValue = OK;
	//check if bitstate must be set or reset
	if (pinState) {
		GPIO_WriteBit(device_config.gpio_pinConfig[gpio_pin].Port,
				device_config.gpio_pinConfig[gpio_pin].Pin, Bit_SET);
	} else {
		GPIO_WriteBit(device_config.gpio_pinConfig[gpio_pin].Port,
				device_config.gpio_pinConfig[gpio_pin].Pin, Bit_RESET);
	}
	return returnValue;
}

/**
 * Get a gpio pin setting which will be returned in the pinsState pointer
 * @param gpio_pin
 * @param pinState
 * @return
 */
int gpio_get_pin(gpio_pin_t gpio_pin, bool *pinState) {
	int returnValue = OK;
	BitAction readBit;

	//Read out the bitAction
	readBit = (BitAction) GPIO_ReadInputDataBit(
			device_config.gpio_pinConfig[gpio_pin].Port,
			device_config.gpio_pinConfig[gpio_pin].Pin);
	//If bit is set set pinstate to true otherwist to false
	if (readBit == Bit_SET) {
		*pinState = true;
	} else if (readBit == Bit_RESET) {
		*pinState = false;
	} else {
		returnValue = FAULT;
	}

	return returnValue;
}

#endif /* STM32F0XX */
