/* -----------------------------------------------------------------------------
 * pwm.h                                           (c) 2017 Rocks
 * -----------------------------------------------------------------------------
 * Author: Gertjan Rocks
 * Web:    www.gertjanrocks.com
 * Mail:   gertjanrocks@outlook.com
 * -----------------------------------------------------------------------------
 * Description: Global PWM header file used for all control types
 * -----------------------------------------------------------------------------
 * Created on: 19 mrt. 2017
 * -----------------------------------------------------------------------------
 */


#ifndef PWM_H_
#define PWM_H_

#ifdef __cplusplus
extern "C" {
#endif

/* --------------*
 * Include files *
 * --------------*
 */


/* -------------------------------*
 * Constant and macro definitions *
 * -------------------------------*
 */

#define PWM_MAX_DUTYCYLE	(1000)


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

int pwm_change_frequency(uint8_t channel, uint16_t pwm_frequency);


#ifdef __cplusplus
}
#endif

#endif /* PWM_H_ */
