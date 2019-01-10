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

#ifdef LPC1769

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

static bool global_gpio_init = false;

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

	//Init points which has to be init only once
	if(!global_gpio_init){
		//Enable overal GPIO clock
		Chip_GPIO_Init(LPC_GPIO);
		global_gpio_init = true;
	}

	//If the clock settings are valid enable the pin
	if (returnValue == OK) {
		//Use the mode to select In/Out PushPull and pulls up
		switch (mode) {
		case GPIO_MODE_INPUT_NOPULL:
			Chip_IOCON_PinMux(LPC_IOCON,device_config.gpio_pinConfig[gpio_pin].Port,
										device_config.gpio_pinConfig[gpio_pin].Pin, IOCON_MODE_INACT,IOCON_FUNC0);
			Chip_GPIO_SetPinDIRInput(LPC_GPIO, device_config.gpio_pinConfig[gpio_pin].Port,
										device_config.gpio_pinConfig[gpio_pin].Pin);
			break;
		case GPIO_MODE_INPUT_PULL_UP:
			Chip_IOCON_PinMux(LPC_IOCON,device_config.gpio_pinConfig[gpio_pin].Port,
										device_config.gpio_pinConfig[gpio_pin].Pin, IOCON_MODE_PULLUP,IOCON_FUNC0);
			Chip_GPIO_SetPinDIRInput(LPC_GPIO, device_config.gpio_pinConfig[gpio_pin].Port,
										device_config.gpio_pinConfig[gpio_pin].Pin);
			break;
		case GPIO_MODE_INPUT_PULL_DOWN:
			Chip_IOCON_PinMux(LPC_IOCON,device_config.gpio_pinConfig[gpio_pin].Port,
										device_config.gpio_pinConfig[gpio_pin].Pin, IOCON_MODE_PULLDOWN,IOCON_FUNC0);
			Chip_GPIO_SetPinDIRInput(LPC_GPIO, device_config.gpio_pinConfig[gpio_pin].Port,
										device_config.gpio_pinConfig[gpio_pin].Pin);
			break;
		case GPIO_MODE_OUTPUT_PUSH_PULL:
			Chip_IOCON_PinMux(LPC_IOCON,device_config.gpio_pinConfig[gpio_pin].Port,
										device_config.gpio_pinConfig[gpio_pin].Pin, IOCON_MODE_INACT,IOCON_FUNC0);
			Chip_GPIO_SetPinDIROutput(LPC_GPIO, device_config.gpio_pinConfig[gpio_pin].Port,
										device_config.gpio_pinConfig[gpio_pin].Pin);
			break;
		case GPIO_MODE_OUTPUT_OPEN_DRAIN:
			Chip_IOCON_PinMux(LPC_IOCON,device_config.gpio_pinConfig[gpio_pin].Port,
										device_config.gpio_pinConfig[gpio_pin].Pin, IOCON_MODE_INACT,IOCON_FUNC0);
			Chip_IOCON_EnableOD(LPC_IOCON,device_config.gpio_pinConfig[gpio_pin].Port,
										device_config.gpio_pinConfig[gpio_pin].Pin);
			Chip_GPIO_SetPinDIROutput(LPC_GPIO, device_config.gpio_pinConfig[gpio_pin].Port,
										device_config.gpio_pinConfig[gpio_pin].Pin);
			break;
		default:
			//Unknown value
			returnValue = FAULT;
			break;
		}
		//Configure the pin
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
		Chip_GPIO_WritePortBit(LPC_GPIO, device_config.gpio_pinConfig[gpio_pin].Port,
				device_config.gpio_pinConfig[gpio_pin].Pin, true);
	} else {
		Chip_GPIO_WritePortBit(LPC_GPIO, device_config.gpio_pinConfig[gpio_pin].Port,
				device_config.gpio_pinConfig[gpio_pin].Pin, false);
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
	//Read out the bitAction
	*pinState = Chip_GPIO_ReadPortBit(LPC_GPIO,
			device_config.gpio_pinConfig[gpio_pin].Port,
			device_config.gpio_pinConfig[gpio_pin].Pin);
	//If bit is set set pinstate to true otherwist to false
	return returnValue;
}

#endif /* LPC1769 */
