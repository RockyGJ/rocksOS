/* -----------------------------------------------------------------------------
 * gpio.h                                           (c) 2017 Rocks
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


#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

/* --------------*
 * Include files *
 * --------------*
 */

#include "stdbool.h"

/* -------------------------------*
 * Constant and macro definitions *
 * -------------------------------*
 */



/* -----------------*
 * Type definitions *
 * -----------------*
 */

/**
 * !!! See the device header file which channel is coupled to which output/input
 */
typedef uint8_t gpio_pin_t;

typedef enum{
	GPIO_MODE_INPUT_NOPULL,
	GPIO_MODE_INPUT_PULL_UP,
	GPIO_MODE_INPUT_PULL_DOWN,
	GPIO_MODE_OUTPUT_PUSH_PULL,
	GPIO_MODE_OUTPUT_OPEN_DRAIN,
}gpio_mode_t;

/* ---------------------*
 * File-scope variables *
 * ---------------------*
 */


/* ----------------------*
 * Function declarations *
 * ----------------------*
 */

/**
 * init the gpio hal layer
 */
extern void gpio_init(void);

/**
 * open a pin selected in the device config table and the mode
 * @param gpio_pin
 * @param mode
 * @return
 */
extern int gpio_open(gpio_pin_t gpio_pin, gpio_mode_t mode);

/**
 * set a gpio pin with a gpio pin from the pin table in config
 * @param gpio_pin
 * @param pinState
 * @return
 */
extern int gpio_set_pin(gpio_pin_t gpio_pin, bool pinState);

/**
 * Get a gpio pin setting which will be returned in the pinsState pointer
 * @param gpio_pin
 * @param pinState
 * @return
 */
extern int gpio_get_pin(gpio_pin_t gpio_pin, bool *pinState);

#ifdef __cplusplus
}
#endif

#endif /* INC_GPIO_H_ */
