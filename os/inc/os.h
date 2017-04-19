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

#ifdef __cplusplus
extern "C" {
#endif

/* --------------*
 * Include files *
 * --------------*
 */

#include "stdint.h"
#include "stdbool.h"
#include "os_settings.h"

/* -------------------------------*
 * Constant and macro definitions *
 * -------------------------------*
 */

/* -----------------*
 * Type definitions *
 * -----------------*
 */

/**
 * os return codes which are used for some OS functions
 */
typedef enum {
	os_failed = -2,				//! Os has failed OS should stop working
	os_init_failed,				//! Init has failed OS should not start
	os_init_succeed,			//! Init is successful OS will start
	os_succesful				//! Return never
} os_return_codes_t;

/**
 * return function for a called task
 */
typedef enum {
	os_task_succeed, //! The called task is succeed and only needs be to called when necessary
	os_task_failed,          //! The called task has failed stops running the OS
	os_task_not_registerd		//!The task is not registered for the event
} os_task_return_codes_t;

/**
 * Event where a task can register for
 */
typedef enum {
	os_event_msg_pending, 		//! Message is pending
	os_event_init,		//! Os is started and calls each task one time on init
	os_event_idle,	//! No messages and if activated timer events are pending

#ifdef OS_USE_TIMERS
	os_event_timer,
#endif /* OS_USE_TIMERS */

	os_nmbr_of_events
} os_event_t;

/**
 * Message event priorities
 */
typedef enum {
	os_msg_priority_high,		//! Message has a high priority
	os_msg_priority_normal,		//! Message has a normal priority
	os_msg_priority_low,		//! Message has a low priority

	os_msg_nmbr_of_priorities	//! Value of the number of the priorities
} os_msg_priority_t;

/**
 * An os message object containing a message id and data union field
 */
typedef struct {
	uint32_t os_msg_id;	//! Message ID of the message
	uint32_t data;			//! Data along with the message
} os_msg_t;

/**
 * All possible log levels
 */
typedef enum {
	os_log_level_all,	//! Will be outputed when in all log levels
	os_log_level_error, //! Will be outputed when error log is active
	os_log_level_os, //! Will be outputed when os log is active
} os_log_level_t;

/**
 * A task entry, used to register a task
 */
typedef struct {
	const char* task_name;	//! Task name
	os_task_return_codes_t (*task_cb)(os_event_t); //! Callback function for the task
	uint32_t identifier; //! User identifier, is not used by the os. Can be changed and read during a running os
} os_task_t;

typedef uint16_t os_task_id_t;  //! The task id assigned to a tasks

#ifdef OS_USE_TIMERS

/**
 * Differnt timer types
 */
typedef enum {
	os_timer_repeat,	//! Timer will auto restart after timer was ended
	os_timer_one_shot //! Timer needs a start command after finishing
} os_timer_type_t;

/**
 * Timer ID used for timer functions
 */
typedef uint16_t os_timer_id_t;

#endif /* OS_USE_TIMERS */

/**
 * The functions pointers below must assigned before running main
 */
typedef struct {
	void (*enable_irq)(void);
	void (*disable_irq)(void);
	void (*stdio)(void);
} os_functions_pointers_t;

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

/**
 * Add an user identifier to a task. This may be used to identify a task between the
 * initializing and running task. This is not necessary for running a task and
 * is not monitored or controlled by the os!
 * @param task_id
 * @param identifier
 * @return 0 if successful and -1 if unsuccessful
 */
extern int os_set_task_identifier(os_task_id_t task_id, uint32_t identifier);

/**
 * Get the identifier set by the user
 * @param task_id
 * @param identifier
 * @return 0 if successful and -1 if unsuccessful
 */
extern int os_get_task_identifier(os_task_id_t task_id, uint32_t* identifier);

/*********************************
 *      Message functions        *
 *********************************/

/**
 * post a message to another task. The task id of the destination is necessary
 * to successful post the msg
 * @param  msg
 * @param  dest_task_id
 * @param  prio
 * @return              return 0 if it was not possible to post msg
 */
extern int os_post_msg(os_msg_t msg, os_task_id_t dest_task_id,
		os_msg_priority_t prio);

/**
 * post a message to another task. The task id of the destination is necessary
 * to successful post the msg
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
 *      Timer functions        *
 *********************************/

#ifdef OS_USE_TIMERS
/**
 * Must be called every millisecond to count
 */
extern void os_timer_count(void);
/**
 * Add a timer with default settings. The function will return a unique timer ID which can be used to
 * start or stop the timer
 * @param timer_value_ms
 * @param timer_type
 * @return
 */
extern os_timer_id_t os_timer_add(uint32_t timer_value_ms,
		os_timer_type_t timer_type);
/**
 * Stop a timer. A timer can be stoped even when not running
 * @param timer_id
 */
extern void os_timer_stop(os_timer_id_t timer_id);
/**
 * Start or restart the timer
 * @param timer_id
 */
extern void os_timer_start(os_timer_id_t timer_id);

#endif /* OS_USE_TIMERS */

/*********************************
 *           Log functions        *
 *********************************/

/**
 * logging function from the os
 * @param level
 * @param fmt
 */
extern void os_log(os_log_level_t level, const char *fmt, ...);

/*********************************
 *           OS functions        *
 *********************************/

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
extern os_return_codes_t os_add_function_pointers(
		os_functions_pointers_t* os_functions);

#ifdef __cplusplus
}
#endif

#endif /* OS_H_ */
