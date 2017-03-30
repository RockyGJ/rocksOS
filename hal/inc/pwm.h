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
#include "stdint.h"

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

/**
 * Init by setting timer settings to zero
 */
extern void pwm_init(void);

/**
 * open a PWM channel with a PWM frequency
 * @param channel
 * @param pwm_freq
 * @return FAULT or OK
 */
extern int pwm_open(uint8_t channel, uint16_t pwm_freq);

/**
 * Change the frequency of a PWM channel or multiple channels
 * @param channel
 * @param pwm_frequency
 * @return
 */
extern int pwm_change_frequency(uint8_t channel, uint16_t pwm_frequency);

/**
 * Change the duty cycle in pre mile
 * @param channel
 * @param dutyCycle 0 - 1000
 * @return
 */
extern int pwm_change_dutyCyle(uint8_t channel, uint16_t dutyCycle);


#ifdef __cplusplus
}
#endif

#endif /* PWM_H_ */
