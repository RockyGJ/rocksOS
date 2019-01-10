/* -----------------------------------------------------------------------------
 * systick.c                                           (c) 2017 Rocks
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

#include "stdio.h"
#include "systick.h"

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

static void (*systick_cb_function)(void);

/* ----------------------*
 * Function declarations *
 * ----------------------*
 */

/* ----------------------*
 * Function definitions  *
 * ----------------------*
 */

/**
 * init the systick hal driver
 */
void systick_init(void) {
	systick_cb_function = NULL;
}

/**
 * open the systick with the function used ass callback
 * @param time_in_ms
 * @param cbFunction
 * @return
 */
int systick_open(uint16_t time_in_ms, void *cbFunction) {
	int returnValue = OK;

	//Save callback
	if (systick_cb_function == NULL) {
		systick_cb_function = cbFunction;
		//Set systick
		if (SysTick_Config(SystemCoreClock / (1000 / time_in_ms))) {
			returnValue = FAULT;
		}
	} else {
		returnValue = FAULT;
	}

	return returnValue;
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void) {
	if (systick_cb_function != NULL) {
		systick_cb_function();
	}
}

#endif /* LPC1769 */
