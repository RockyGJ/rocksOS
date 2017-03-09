/* -----------------------------------------------------------------------------
 * os_settings.h                                             (c) 2017 Rocks
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

#ifndef LOC_INC_OS_SETTINGS_H_
#define LOC_INC_OS_SETTINGS_H_

/* --------------*
 * Include files *
 * --------------*
 */


/* -------------------------------*
 * Constant and macro definitions *
 * -------------------------------*
 */

/** OS Settings **/
#define OS_USE_TIMERS		//! Disable timers to save memory


/** SIZE settings **/
#define OS_MAXIMUM_TASKS	(32)		//! Maximum number of tasks

#define OS_MSG_QUEUE_SIZE	(32)		//! Size of the message queue for each prio

#ifdef OS_USE_TIMERS
	#define OS_MAXIMUM_TIMERS	(32)	//! Number of availble timers
#endif /* OS_USE_TIMERS */
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



#endif /* LOC_INC_OS_SETTINGS_H_ */
