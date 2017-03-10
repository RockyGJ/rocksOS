/* -----------------------------------------------------------------------------
 * os_private.h                                             (c) 2017 Rocks
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

#ifndef OS_PRIVATE_H_
#define OS_PRIVATE_H_

/* --------------*
 * Include files *
 * --------------*
 */
#include "os.h"

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

extern os_functions_pointers_t* os_functions_pointer;

/* ----------------------*
 * Function declarations *
 * ----------------------*
 */

//Functions for private in the os
//TODO need to set comments

extern os_return_codes_t os_task_init(void);
extern os_task_return_codes_t os_run_task(os_task_id_t task_id, os_event_t event);
extern os_task_id_t os_nmbr_of_tasks(void);

extern void os_set_error(void);

extern os_return_codes_t os_msg_init(void);
extern os_task_id_t os_msg_pending(void);

#ifdef OS_USE_TIMERS
extern os_return_codes_t os_timer_init(void) ;
/**
 * Call this function from the OS main loop to check the timers
 */
extern void os_timer_check_timers(void) ;
/**
 * Theis functions can be called to check is a timer is pending
 * @return  task id of pending timer
 */
extern os_task_id_t os_timer_pending(void);
#endif /* OS_USE_TIMERS */

#endif /* OS_PRIVATE_H_ */
