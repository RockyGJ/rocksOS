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

extern os_return_codes_t os_msg_init(void);
extern os_return_codes_t os_task_init(void);
extern void os_set_error(void);

#endif /* OS_PRIVATE_H_ */
