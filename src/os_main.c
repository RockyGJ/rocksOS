/* -----------------------------------------------------------------------------
 * os_main.c                                             (c) 2017 Rocks
 * -----------------------------------------------------------------------------
 * Author: Gertjan Rocks
 * Web:    www.gertjanrocks.com
 * Mail:   gertjanrocks@outlook.com
 * -----------------------------------------------------------------------------
 * Description:
 * -----------------------------------------------------------------------------
 * Created on: Feb 22, 2017
 * -----------------------------------------------------------------------------
 */

/* --------------*
 * Include files *
 * --------------*
 */
#include "os.h"
#include "os_settings.h"
#include "os_private.h"

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
bool os_error;
/* ----------------------*
 * Function declarations *
 * ----------------------*
 */

/* ---------------------*
 * Function definitions *
 * ---------------------*
 */

/**
 * Function will initialize every necessery things for the os
 * @return
 */
os_return_codes_t os_init(void) {
	os_return_codes_t os_returnValue = os_init_succeed;
	//Clear os error;
	os_error = false;
	//Init the os message layer
	if (os_returnValue == os_init_succeed) {
		if (os_msg_init() != os_init_succeed) {
			os_returnValue = os_init_failed;
			os_log(os_log_level_error, "Error on message initialize");
		}
	}
	//Init the task layer
	if (os_returnValue == os_init_succeed) {
		if (os_task_init() != os_init_succeed) {
			os_returnValue = os_init_failed;
			os_log(os_log_level_error, "Error on task initialize");
		}
	}
	return os_returnValue;
}

int os_main(void) {
	if(!os_error){
		//Check if message is pending
		os_log(os_log_level_os, "Test loggin %d %d\n\r",200,200);

		//Check if task are registered for the message event

		//Check if task is registered for idle event
	}
}

void os_set_error(void) {
	os_error = true;
}
