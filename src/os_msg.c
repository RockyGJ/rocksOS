/* -----------------------------------------------------------------------------
 * os_msg.c                                             (c) 2017 Rocks
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


/* --------------*
 * Include files *
 * --------------*
 */

#include "os.h"
#include "os_settings.h"
#include "os_queue.h"

/* -------------------------------*
 * Constant and macro definitions *
 * -------------------------------*
 */

/* -----------------*
 * Type definitions *
 * -----------------*
 */
 
typedef struct {
	os_msg_t		msg;
	os_task_id_t	destination;
}os_msg_queue_entry_t;

typedef struct {
	os_msg_queue_entry_t queue_buffer[OS_MSG_QUEUE_SIZE];
	os_queue_t			 queue;
}os_msg_queue_t;

/* ---------------------*
 * File-scope variables *
 * ---------------------*
 */

static os_msg_queue_t os_msg_buffer[os_msg_nmbr_of_priorities];

/* ----------------------*
 * Function declarations *
 * ----------------------*
 */


/* ---------------------*
 * Function definitions *
 * ---------------------*
 */

/**
 * initialise the message layer
 * @return
 */
os_return_codes_t os_msg_init(void){

	os_return_codes_t returnValue;

	//Initialize the queues for each message priority's
	for(uint8_t queue_index = 0; queue_index < os_msg_nmbr_of_priorities; queue_index++){
		os_queue_init(&os_msg_buffer[queue_index].queue, &os_msg_buffer[queue_index].queue_buffer,
				(sizeof(os_msg_buffer[queue_index].queue_buffer) / sizeof(os_msg_buffer[queue_index].queue_buffer[0])),
				sizeof(os_msg_buffer[queue_index].queue_buffer[0]));
	}

	returnValue = os_init_succeed;

	return returnValue;
}

os_task_id_t os_msg_pending(void){
	//Always check if the highest prio is not empty
}
