/* -----------------------------------------------------------------------------
 * os_task.c                                             (c) 2017 Rocks
 * -----------------------------------------------------------------------------
 * Author: Gertjan Rocks
 * Web:    www.gertjanrocks.com
 * Mail:   gertjanrocks@outlook.com
 * -----------------------------------------------------------------------------
 * Description: 
 * -----------------------------------------------------------------------------
 * Created on: Feb 24, 2017
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

typedef struct {
	os_task_t task;
	os_task_id_t this;
	bool event_subscribe[os_nmbr_of_events];
} os_task_admin_t;

/* ---------------------*
 * File-scope variables *
 * ---------------------*
 */

static os_task_admin_t os_task_admin[OS_MAXIMUM_TASKS];
static os_task_id_t os_latest_added_task;
/* ----------------------*
 * Function declarations *
 * ----------------------*
 */

/* ---------------------*
 * Function definitions *
 * ---------------------*
 */

/**
 * initialse the os task layer
 * @return
 */
os_return_codes_t os_task_init(void) {
	os_return_codes_t returnValue = os_init_failed;

	//Clear all task entries
	for (os_task_id_t task = 0; task < OS_MAXIMUM_TASKS; task++) {
		os_task_admin[task].task.task_cb = NULL;
		os_task_admin[task].this = task;
		//Clear the event subscription
		for (os_event_t event = 0; event < os_nmbr_of_events; event++) {
			os_task_admin[task].event_subscribe[event] = false;
		}
	}
	//Clear the variable
	os_latest_added_task = 0;
	//Init is successful
	returnValue = os_init_succeed;
	return returnValue;
}

/**
 * add a task to the local task table
 * @param new_task
 * @return -1 if error on adding
 */
int os_add_task(os_task_t new_task) {
	if (os_task_admin[os_latest_added_task].task.task_cb == NULL) {
		os_task_admin[os_latest_added_task++].task = new_task;
		if (os_latest_added_task >= OS_MAXIMUM_TASKS) {
			os_log(os_log_level_error, "Error on adding task");
			os_set_error();
			return -1;
		}
	} else {
		os_log(os_log_level_error, "Error on adding task");
		os_set_error();
		return -1;
	}
	return 0;
}

/**
 * register a task id for a os event
 * @param  event   event to register
 * @param  task_id task id which want to register to the event
 * @return         returns 0 if it was not possible to register for the event
 */
int os_register_for_event(os_event_t event, os_task_id_t task_id){
	os_task_admin[task_id].event_subscribe[event] = true;
	return 0;
}
