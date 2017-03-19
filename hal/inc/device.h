/* -----------------------------------------------------------------------------
 * device.h                                           (c) 2017 Rocks
 * -----------------------------------------------------------------------------
 * Author: Gertjan Rocks
 * Web:    www.gertjanrocks.com
 * Mail:   gertjanrocks@outlook.com
 * -----------------------------------------------------------------------------
 * Description: 
 * -----------------------------------------------------------------------------
 * Created on: 19 mrt. 2017
 * -----------------------------------------------------------------------------
 */


#ifndef DEVICE_H_
#define DEVICE_H_


/*!
 *  SELECT THE DEVICE TO USE THE HAL FUNCIONS!
 */

//Only select one!

#define STM32F0XX
//#define LPC82X



/*!
 * Select subdevice for the configuration
 */

#ifdef STM32F0XX
#include "STM32F051R8_LQFP64.h"
#endif

/**
 * Check the selection pre-compile
 */

//Check on multiple checked
#if defined(STM32F0XX) && defined(LPC82X)
#error "Multiple devices defined in the hal"
#endif


//Check on non selected
#if !defined(STM32F0XX) && !defined(LPC82X)
#error "None device is defined in hal"
#endif

#endif /* DEVICE_H_ */
