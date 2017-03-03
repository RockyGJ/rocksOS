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
#include "stdio.h"

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
//Public for OS
os_functions_pointers_t* os_functions_pointer;

//Private
static bool os_error;
static bool os_functions_done = false;

/* ----------------------*
 * Function declarations *
 * ----------------------*
 */

/* ---------------------*
 * Function definitions *
 * ---------------------*
 */

/**
 * Function will initialize every necessary things for the os
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

/**
 * main function for the os should only return on a error
 * @return
 */
int os_main(void) {
	os_task_id_t task_id;
	os_task_return_codes_t returnValue;

	//Check functions pointers
	if(!os_functions_done){
		os_set_error();
		os_log(os_log_level_os, "Error functions pointers not done\n\r");
	}

	//init all the task
	if(!os_error){
		for (task_id = 0; task_id < os_nmbr_of_tasks(); task_id++){
			os_log(os_log_level_os, "Send init to %d\n\r",task_id);
			returnValue = os_run_task(task_id,os_event_init);

		}
	}
	//Run the OS while main loop until an error occurs
	while(!os_error){
		//Check if message is pending
		task_id = os_msg_pending();
		//Check if message is valid
		if(task_id < OS_MAXIMUM_TASKS){
			os_log(os_log_level_os, "Message pending for %d\n\r",task_id);
			returnValue = os_run_task(task_id, os_event_msg_pending);
			if(returnValue == os_task_failed){
				os_log(os_log_level_os, "Error on message pending for %d\n\r",task_id);
			}
		}
		//Check if task are registered for the message event

		//Check if task is registered for idle event
	}
	return os_failed;
}

/**
 * Set the OS error on true
 */
void os_set_error(void) {
	os_error = true;
	os_log(os_log_level_all, "Error reported for OS!!\n\rt");
}


/**
 * add the necessary functions pointers to the os
 * @param os_functions
 * @return
 */
os_return_codes_t os_add_function_pointers(os_functions_pointers_t* os_functions){
	os_return_codes_t os_returnValue = os_init_succeed;
	//Check the necessary functions

	os_functions_pointer = os_functions;
	//Check disable irq function
	if (os_returnValue == os_init_succeed) {
		if (os_functions_pointer->disable_irq == NULL) {
			os_returnValue = os_init_failed;
		}
	}
	//Check enable irq function
	if (os_returnValue == os_init_succeed) {
		if (os_functions_pointer->enable_irq == NULL) {
			os_returnValue = os_init_failed;
		}
	}
	//Check disable irq function
	if (os_returnValue == os_init_succeed) {
		if (os_functions_pointer->stdio == NULL) {
			os_returnValue = os_init_failed;
		}
	}

	if(os_returnValue == os_init_succeed){
		os_functions_done = true;
	}
	return os_returnValue;
}
