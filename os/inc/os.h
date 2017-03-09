/* -----------------------------------------------------------------------------
 * os.h                                             (c) 2017 Rocks
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

#ifndef OS_H_
#define OS_H_

/* --------------*
 * Include files *
 * --------------*
 */
#include "stdint.h"
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
 * os return codes
 */
typedef enum {
	os_failed = -2,
	os_init_failed,
	os_init_succeed,
	os_succesful
} os_return_codes_t;

/**
 * return function for a called task
 */
typedef enum {
	os_task_succeed, //! The called task is succeed and only needs be to called when necessary
	os_task_failed,          	//! The called task has failed stop running
	os_task_rerun,            	//! The called task needs to run again
	os_task_not_registerd		//!The task is not registered for the event
} os_task_return_codes_t;

/**
 * Event priorities
 */
typedef enum {
	os_msg_priority_high,		//! Message has a high priority
	os_msg_priority_normal,		//! Message has a normal priority
	os_msg_priority_low,		//! Message has a low priority

	os_msg_nmbr_of_priorities	//! Value of the number of the priorities
} os_msg_priority_t;

/**
 * Event where a task can register for
 */
typedef enum {
	os_event_msg_pending, //! Message is pending
	os_event_init,
	os_event_idle,

	os_nmbr_of_events
} os_event_t;

typedef enum {
	os_log_level_all, os_log_level_error, os_log_level_os,
} os_log_level_t;

/**
 * a os message object containing a message id and data union field
 */
typedef struct {
	uint32_t os_msg_id;
	uint32_t data;
} os_msg_t;

typedef struct {
	const char* task_name;
	os_task_return_codes_t (*task_cb)(os_event_t);
} os_task_t;

typedef uint16_t os_task_id_t;  //! The task id assigned to a tasks

/**--------------------------------------------------------------
 * The functions pointers below must assigned before running main
 *---------------------------------------------------------------
 */
typedef struct {
	void (*enable_irq)(void);
	void (*disable_irq)(void);
	void (*stdio)(void);
} os_functions_pointers_t;

/* ---------------------*
 * File-scope variables *
 * ---------------------*
 */


/* ----------------------*
 * Function declarations *
 * ----------------------*
 */

/*********************************
 *        Tasks functions        *
 *********************************/

/**
 * add a task to the os task table
 * @param new_task
 * @return return a 0 if successful and -1 if a error occur
 */
extern int os_add_task(os_task_t new_task);

/**
 * return the task id of the current running task
 * @return  task id of running task
 */
extern os_task_id_t os_current_task_id(void);

/**
 * register a task id for a os event
 * @param  event   event to register
 * @param  task_id task id which want to register to the event
 * @return         returns 0 if it was not possible to register for the event
 */
extern int os_subscribe_for_event(os_event_t event, os_task_id_t task_id);


/*********************************
 *      Message functions        *
 *********************************/

/**
 * post a message to another task. The task id of the destination is nevessery
 * to succsesful post the msg
 * @param  msg
 * @param  dest_task_id
 * @param  prio
 * @return              return 0 if it was not possible to post msg
 */
extern int os_post_msg(os_msg_t msg, os_task_id_t dest_task_id,
		os_msg_priority_t prio);

/**
 * post a message to another task. The task id of the destination is nevessery
 * to succsesful post the msg
 *
 * This function is same as post message but cann be called from of the interrupt
 * @param  msg
 * @param  dest_task_id
 * @param  prio
 * @return              return 0 if it was not possible to post msg
 */
extern int os_post_msg_from_irq(os_msg_t msg, os_task_id_t dest_task_id,
		os_msg_priority_t prio);

/**
 * retrieve the message from the buffer. The message buffer has a fifo system removing
 * the first entry.
 * @param  msg
 * @return      return 0 if call was made without a msg pending or event msg pending
 */
extern int os_retrieve_msg(os_msg_t *msg);

/*********************************
 *           OS functions        *
 *********************************/

/**
 * logging function from the os
 * @param level
 * @param fmt
 */
extern void os_log(os_log_level_t level, const char *fmt, ...);

/**
 * The os_init function needs to be called before the os main is called
 */
extern os_return_codes_t os_init(void);

/**
 * The os_main function will never return unless a critical error has occurred
 * @return
 */
extern os_return_codes_t os_main(void);

/**
 * add the necessery os functions pointers
 * @param os_functions
 * @return
 */
extern os_return_codes_t os_add_function_pointers(os_functions_pointers_t* os_functions);

#ifdef OS_COOPERATIVE_MULTITASKING
/**
 * Let the active task sleep for a minimum of sleep_ms the os main function
 * will schedule other tasks when this function is called
 * @param  sleep_ms sleep time in  milliseconds
 * @return          return 0 if sleep is succeed or the number of milliseconds
 *                  to go when interrupt
 */
extern int os_sleep(uint32_t sleep_ms);

#endif /* OS_COOPERATIVE_MULTITASKING */

#endif /* OS_H_ */
