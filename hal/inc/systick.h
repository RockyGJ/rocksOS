/* -----------------------------------------------------------------------------
 * systick.h                                           (c) 2017 Rocks
 * -----------------------------------------------------------------------------
 * Author: Gertjan Rocks
 * Web:    www.gertjanrocks.com
 * Mail:   gertjanrocks@outlook.com
 * -----------------------------------------------------------------------------
 * Description: 
 * -----------------------------------------------------------------------------
 * Created on: 30 mrt. 2017
 * -----------------------------------------------------------------------------
 */


#ifndef INC_SYSTICK_H_
#define INC_SYSTICK_H_

#ifdef __cplusplus
extern "C" {
#endif

/* --------------*
 * Include files *
 * --------------*
 */

#include "stdint.h"


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

/**
 * init the systick hal driver
 */
extern void systick_init(void);

/**
 * open the systick with the function used ass callback
 * @param time_in_ms
 * @param cbFunction
 * @return
 */
extern int systick_open(uint16_t time_in_ms, void *cbFunction);


#ifdef __cplusplus
}
#endif

#endif /* INC_SYSTICK_H_ */
