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

#include "device.h"

#ifdef LPC1769
#include "hal.h"

//Modules
#include "system.h"
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
	//Init LPC1769 clocking and power
	system_init();
	//Init systick hal layer
	systick_init();
	//Init pwm hal layer
//	pwm_init();
	//Init gpio hal layer
//	gpio_init();
}


#endif /* LPC1769 */
