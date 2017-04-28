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
static os_task_id_t task_running;
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
		os_task_admin[task].task.identifier = 0;
	}
	//Clear the variable
	os_latest_added_task = 0;
	task_running = OS_MAXIMUM_TASKS;
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
		//Every event is auto subscribed to the os_event_init
		os_task_admin[os_latest_added_task].event_subscribe[os_event_init] =
				true;
		//Add task to the table
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
 * subscribe a task id for a os event
 * @param  event   event to subscribed
 * @param  task_id task id which want to subscribe to the event
 * @return         returns 0 if it was not possible to subscribe for the event
 */
int os_subscribe_for_event(os_event_t event, os_task_id_t task_id) {
	os_task_admin[task_id].event_subscribe[event] = true;
	return 1;
}

/**
 *	return the running task id
 * @return
 */
extern os_task_id_t os_current_task_id(void){
	return task_running;
}


/**
 * run a task with a given event
 * @param task_id
 * @param event
 * @return the task return code
 */
os_task_return_codes_t os_run_task(os_task_id_t task_id, os_event_t event) {
	//Check if task id registered for the event
	if (os_task_admin[task_id].event_subscribe[event]) {
		if (os_task_admin[task_id].task.task_cb != NULL) {
			task_running = os_task_admin[task_id].this;
			return os_task_admin[task_id].task.task_cb(event);
		} else {
			return os_task_failed;
		}
	} else {
		return os_task_not_registerd;
	}
}

/**
 * return the number of task
 */
os_task_id_t os_nmbr_of_tasks(void){
	return os_latest_added_task;
}

/**
 * Add an user identifier to a task. This may be used to identify a task between the
 * initializing and running task. This is not necessary for running a task and
 * is not monitored or controlled by the os!
 * @param task_id
 * @param identifier
 * @return 0 if successful and -1 if unsuccessful
 */
int os_set_task_identifier(os_task_id_t task_id, uint32_t identifier)
{
	if(task_id == os_task_admin[task_id].this){
		//Set the identifier
		os_task_admin[task_id].task.identifier = identifier;
		//Everything went fine so return 0
		return 0;
	} else {
		//Error on finding the right task
		return -1;
	}
}

/**
 * Get the identifier set by the user
 * @param task_id
 * @param identifier
 * @return 0 if successful and -1 if unsuccessful
 */
int os_get_task_identifier(os_task_id_t task_id, uint32_t* identifier)
{
	if(task_id == os_task_admin[task_id].this){
		//Get the identifier
		*identifier = os_task_admin[task_id].task.identifier;
		//Everything went fine so return 0
		return 0;
	} else {
		//Error on finding the right task
		return -1;
	}
}
