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
#include "os_private.h"

/* -------------------------------*
 * Constant and macro definitions *
 * -------------------------------*
 */

/* -----------------*
 * Type definitions *
 * -----------------*
 */

typedef struct {
	os_msg_t msg;
	os_task_id_t destination;
} os_msg_queue_entry_t;

typedef struct {
	os_msg_queue_entry_t queue_buffer[OS_MSG_QUEUE_SIZE];
	os_queue_t queue;
} os_msg_queue_t;

/* ---------------------*
 * File-scope variables *
 * ---------------------*
 */

static os_msg_queue_t os_msg_buffer[os_msg_nmbr_of_priorities];
static os_msg_queue_entry_t msg_pending;
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
os_return_codes_t os_msg_init(void) {

	os_return_codes_t returnValue;

	//Initialize the queues for each message priority's
	for (uint8_t queue_index = 0; queue_index < os_msg_nmbr_of_priorities;
			queue_index++) {
		os_queue_init(&os_msg_buffer[queue_index].queue,
				&os_msg_buffer[queue_index].queue_buffer,
				(sizeof(os_msg_buffer[queue_index].queue_buffer)
						/ sizeof(os_msg_buffer[queue_index].queue_buffer[0])),
				sizeof(os_msg_buffer[queue_index].queue_buffer[0]));
	}

	returnValue = os_init_succeed;

	return returnValue;
}

/**
 * Check if a message is pending
 * @return task id of message pending
 */
os_task_id_t os_msg_pending(void) {
	os_msg_priority_t check_prio;
	os_task_id_t msg_task;

	//Set the message task to the maximum if not a message is found
	msg_task = OS_MAXIMUM_TASKS;
	//Always check if the highest prio is not empty
	for (check_prio = os_msg_priority_high;
			check_prio < os_msg_nmbr_of_priorities; check_prio++) {
		//Check if queue is not empty
		if (!os_queue_isEmpty(&os_msg_buffer[check_prio].queue)) {
			//Get the message from the queue
			os_queue_remove(&os_msg_buffer[check_prio].queue, &msg_pending);
			//Get the task id from the message
			msg_task = msg_pending.destination;
			//Break priority
			break;
		}
	}

	return msg_task;
}

/**
 * retreive the message pending
 * @param msg
 * @return
 */
int os_retrieve_msg(os_msg_t *msg)
{
	if(msg_pending.destination == os_current_task_id()){
		*msg = msg_pending.msg;
		return 1;
	} else {
		os_set_error();
	}
	return 0;
}

/**
 * post msg to queue
 * @param msg
 * @param dest_task_id
 * @param prio
 * @return
 */
int os_post_msg(os_msg_t msg, os_task_id_t dest_task_id,
		os_msg_priority_t prio)
{
	os_msg_queue_entry_t new_entry;
	new_entry.msg = msg;
	new_entry.destination = dest_task_id;
	//Disable irq
	os_functions_pointer->disable_irq();
	os_queue_add(&os_msg_buffer[prio].queue, &new_entry);
	//Enable irq
	os_functions_pointer->enable_irq();
	return 0;
}
