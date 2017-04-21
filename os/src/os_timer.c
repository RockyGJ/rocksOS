/* -----------------------------------------------------------------------------
 * os_timer.c                                             (c) 2017 Rocks
 * -----------------------------------------------------------------------------
 * Author: Gertjan Rocks
 * Web:    www.gertjanrocks.com
 * Mail:   gertjanrocks@outlook.com
 * -----------------------------------------------------------------------------
 * Description:
 * -----------------------------------------------------------------------------
 * Created on: Mar 3, 2017
 * -----------------------------------------------------------------------------
 */

/* --------------*
 * Include files *
 * --------------*
 */

#include "os.h"
#include "os_settings.h"
#include "os_private.h"
#include "os_queue.h"

#ifdef OS_USE_TIMERS
/* -------------------------------*
 * Constant and macro definitions *
 * -------------------------------*
 */

/* -----------------*
 * Type definitions *
 * -----------------*
 */

typedef struct {
	uint64_t start_value;
	uint32_t value;
	os_timer_type_t type;
	os_task_id_t task_id;
	bool used;
} os_timer_entry_t;

/* ---------------------*
 * File-scope variables *
 * ---------------------*
 */
//Timer entries
static os_timer_entry_t os_timers[OS_MAXIMUM_TIMERS];
//Queue to store the pending task_ids
static os_task_id_t	os_timer_pending_queue_buffer[OS_MAXIMUM_TIMERS];
static os_queue_t os_timer_pending_queue;

//unsigned 64 bit timer @ 1 milliseconds can overflow in 584942417 years! Take care of this!
static uint64_t os_timer_counter;

/* ----------------------*
 * Function declarations *
 * ----------------------*
 */

/* ---------------------*
 * Function definitions *
 * ---------------------*
 */
/**
 * initialize the timer functions
 * @return  os_init_succeed if succeed
 */
os_return_codes_t os_timer_init(void) {
	os_return_codes_t returnValue = os_init_succeed;
	//Clear os timer counter
	//Start at one so zero can be used to disable timer
	os_timer_counter = 1;
	//Clear and disable all entries
	for (os_timer_id_t id = 0; id < OS_MAXIMUM_TIMERS; id++) {
		os_timers[id].start_value = 0;
		os_timers[id].task_id = OS_MAXIMUM_TASKS;
		os_timers[id].value = 0;
		os_timers[id].used = false;
	}
	//Init the timer pending queue
	os_queue_init(&os_timer_pending_queue, &os_timer_pending_queue_buffer,
		 (sizeof(os_timer_pending_queue_buffer)	/ sizeof(os_timer_pending_queue_buffer[0])),
		 sizeof(os_timer_pending_queue_buffer[0]));
	return returnValue;
}

/**
 * Must be called every OS_COUNTER_TIME to count
 */
void os_timer_count(void) {
	os_timer_counter += OS_COUNTER_TIME;
}

/**
 * Add a timer with default settings. The function will return a unique timer ID which can be used to
 * start or stop the timer
 * @param timer_value_ms
 * @param timer_type
 * @return
 */
os_timer_id_t os_timer_add(uint32_t timer_value_ms, os_timer_type_t timer_type) {
	os_timer_id_t free_slot;
	//Set free slot to max to find empty slot
	free_slot = OS_MAXIMUM_TIMERS;
	//Search for empty timer slot
	for (os_timer_id_t id = 0; id < OS_MAXIMUM_TIMERS; id++) {
		if (!os_timers[id].used) {
			free_slot = id;
			break;
		}
	}

	if (free_slot >= OS_MAXIMUM_TIMERS) {
		os_log(os_log_level_error, "Not enough room to add timer!\n\r");
		os_set_error();
	} else {
		//Add the data to the new timer
		os_timers[free_slot].task_id = os_current_task_id();
		os_timers[free_slot].type = timer_type;
		os_timers[free_slot].value = timer_value_ms;
		os_timers[free_slot].used = true;
	}

	return free_slot;
}

/**
 * Stop a timer. A timer can be stoped even when not running
 * @param timer_id
 */
void os_timer_stop(os_timer_id_t timer_id) {
	//Check if timer is in use
	if (os_timers[timer_id].used) {
		//Set start value to zero to disable timer
		os_timers[timer_id].start_value = 0;
	}
}

/**
 * Start or restart the timer
 * @param timer_id
 */
void os_timer_start(os_timer_id_t timer_id) {
	//Check if timer is in use
	if (os_timers[timer_id].used) {
		os_timers[timer_id].start_value = os_timer_counter;
	}
}

/**
 * Set a new timer value. This is only possible when the timer is stopped
 * @param timer_id
 * @param timer_value_ms
 */
void os_timer_new_value(os_timer_id_t timer_id, uint32_t timer_value_ms) {
	//Check is timer is in use and if timer is stopped
	if((os_timers[timer_id].used) && (os_timers[timer_id].start_value == 0)){
		os_timers[timer_id].value = timer_value_ms;
	}
}

/**
 * Call this function from the OS main loop to check the timers
 */
void os_timer_check_timers(void) {
	//Check all timers if in use
	for(os_timer_id_t id = 0; id < OS_MAXIMUM_TIMERS; id++){
		//Check for in use and for started
		if((os_timers[id].used) && (os_timers[id].start_value > 0)){
			//Check if timer has occurred
			if(os_timer_counter >= (os_timers[id].start_value + os_timers[id].value)){
				//Add the timer to the timer queue
				os_queue_add(&os_timer_pending_queue, &os_timers[id].task_id);
				if(os_timers[id].type == os_timer_repeat){
					os_timer_start(id);
				} else {
					os_timer_stop(id);
				}
			}
		}
	}
}

/**
 * This functions can be called to check is a timer is pending
 * @return  task id of pending timer
 */
os_task_id_t os_timer_pending(void) {
	os_task_id_t pending_task;
	//Set task is to max
	pending_task = OS_MAXIMUM_TASKS;
	//Check if the queue is not empty
	if (!os_queue_isEmpty(&os_timer_pending_queue)) {
		//Get the task id from the queue
		os_queue_remove(&os_timer_pending_queue, &pending_task);
	}
	return pending_task;
}

/**
 * This function will return the counter setting in ms which needs to be used
 * to initialize the calling counter for os_timer_count();
 * @return
 */
uint16_t os_timer_counter_step_setting(void){
	return OS_COUNTER_TIME;
}

#endif /* OS_USE_TIMERS */
