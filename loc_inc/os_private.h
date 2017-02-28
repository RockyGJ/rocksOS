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
#endif /* OS_PRIVATE_H_ */
