/* -----------------------------------------------------------------------------
 * os_timer.c                                             (c) 2017 Rocks
 * -----------------------------------------------------------------------------
 * Author: Gertjan Rocks
 * Web:    www.gertjanrocks.com
 * Mail:   gertjanrocks@outlook.com
 * -----------------------------------------------------------------------------
 * Description: 
 * -----------------------------------------------------------------------------
 * Created on: Mar 3, 2017
 * -----------------------------------------------------------------------------
 */


/* --------------*
 * Include files *
 * --------------*
 */

#include "os.h"
#include "os_settings.h"
#include "os_private.h"

/* -------------------------------*
 * Constant and macro definitions *
 * -------------------------------*
 */

/* -----------------*
 * Type definitions *
 * -----------------*
 */
 
typedef enum{
	os_timer_repeat,
	os_timer_one_shot
}os_timer_type_t;

typedef uint16_t os_timer_id_t;

typedef struct{
	uint64_t timer_start_value;
	uint32_t
}os_timer_entry_t;
/* ---------------------*
 * File-scope variables *
 * ---------------------*
 */

//unsigned 64 bit timer @ 1 milliseconds can overflow in 584942417 years! Take care of this!
uint64_t os_timer_counter;

/* ----------------------*
 * Function declarations *
 * ----------------------*
 */


/* ---------------------*
 * Function definitions *
 * ---------------------*
 */

os_return_codes_t os_timer_init(void){
	os_timer_counter = 0;
}

os_timer_id_t os_timer_add(uint32_t timer_value_ms, os_timer_type_t timer_type)
{

}

os_return_codes_t os_timer_stop(os_timer_id_t timer_id)
{

}

os_return_codes_t os_timer_start(os_timer_id_t timer_id)
{

}

os_return_codes_t os_timer_new_value(os_timer_id_t timer_id, uint32_t timer_value_ms)
{

}

void os_timer_count(void){
	os_timer_counter++;
}

os_task_id_t os_timer_pending(void){

}
