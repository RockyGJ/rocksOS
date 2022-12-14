/* -----------------------------------------------------------------------------
 * hal.c                                           (c) 2017 Rocks
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


/* --------------*
 * Include files *
 * --------------*
 */

#include "hal.h"

//Modules
#include "systick.h"
#include "pwm.h"
#include "gpio.h"

/* -------------------------------*
 * Constant and macro definitions *
 * -------------------------------*
 */


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
 * init all hal functions
 */
void hal_init(void){
	//Init systick hal layer
	systick_init();
	//Init pwm hal layer
	pwm_init();
	//Init gpio hal layer
	gpio_init();
}
