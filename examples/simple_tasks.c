/*
 ============================================================================
 Name        : simple_tasks.c
 Author      : Gertjan Rocks
 Version     :
 Copyright   : Your copyright notice
 Description : Create three simple tasks with most of the os features
 ============================================================================
 */

/* The simple task will create three tasks. Each task will react on messages
 *
 * Task 1 - Will be inited with a simple auto repeating timer for 10 seconds.
 * 			Each ten seconds the timer will send a message to task 2 and does nothing
 *
 * Task 2 - Will be inited with a single shot timer of 1 second. The timer will
 * 			start after receiving the message from task 1. When the timer events happen
 * 			it will send a event to task 3
 *
 * Task 3 - Will receive the message a set a variable on idle the variable is checked and
 * 			a status is logged
 *
 */
#define COMPILE_FOR_LINUX
#ifdef COMPILE_FOR_LINUX

#include <stdio.h>
#include <stdlib.h>
#include "os.h"

//system include
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

os_task_t task1;
os_task_t task2;
os_task_t task3;
os_task_return_codes_t task1_callback(os_event_t event);
os_task_return_codes_t task2_callback(os_event_t event);
os_task_return_codes_t task3_callback(os_event_t event);
os_task_id_t taskID_1, taskID_2, taskID_3;

os_functions_pointers_t function_pointers;


pthread_t tid;

void disable_interrupt(void);
void enable_interrupt(void);
void stdio_function(void);

/**
 * Thread to create counter tick for the os
 * @param arg
 */
void* Thread(void *arg){
	while (1) {
		os_timer_count();
		usleep(10000);
	}
}

/**
 * Example main function
 * @return
 */
int main(void) {

	//Add functions pointers necessery for interrupt
	function_pointers.disable_irq = &disable_interrupt;
	function_pointers.enable_irq = &enable_interrupt;
	function_pointers.stdio = &stdio_function;
	//Add structure to os
	os_add_function_pointers(&function_pointers);

	//Call init
	os_init();

	//Add task 1
	task1.task_cb = &task1_callback;
	task1.task_name = "task1";
	os_add_task(task1);
	//Add task 2
	task2.task_cb = &task2_callback;
	task2.task_name = "task2";
	os_add_task(task2);
	//Add task 3
	task3.task_cb = &task3_callback;
	task3.task_name = "task3";
	os_add_task(task3);

	//Begin thread for 1ms timing
	pthread_create(&tid, NULL, &Thread, NULL);

	//Start os should never return
	os_main();

	return EXIT_SUCCESS;
}

/**
 * Function to disable interupt
 */
void disable_interrupt(void) {
	//os_log(os_log_level_all, "disable_interrupt\n\r");
}

/**
 * Function to enable interrupt
 */
void enable_interrupt(void) {
	//os_log(os_log_level_all, "enable_interrupt\n\r");
}

/**
 * Function used to output stdio
 */
void stdio_function(void) {
	os_log(os_log_level_all, "stdio_function\n\r");
}

/**
 * Task 1
 * @param event
 * @return
 */
os_task_return_codes_t task1_callback(os_event_t event) {
	os_msg_t msg;
	static os_timer_id_t timer;

	switch (event) {
	case os_event_init:
		//Save task id
		taskID_1 = os_current_task_id();
		//Log the task id
		os_log(os_log_level_all, "Task %d is initialized\n\r", os_current_task_id());
		//Subscribe for timer event
		os_subscribe_for_event(os_event_timer, os_current_task_id());
		//Create timer for 10 seconds = 10000 milliseconds
		timer = os_timer_add(10000, os_timer_repeat);
		// Start the timer
		os_timer_start(timer);
		break;

	case os_event_timer:
		os_log(os_log_level_all, "Timer send message from %d to %d\n\r",os_current_task_id(),taskID_2);
		msg.os_msg_id = taskID_1;
		msg.data = 0x55;
		//Post message to task2
		os_post_msg(msg, taskID_2, os_msg_priority_normal);
		break;

	default:
		break;
	}
	return os_task_succeed; //Succeed
}


/**
 * Task 2
 * @param event
 * @return
 */
os_task_return_codes_t task2_callback(os_event_t event) {
	os_msg_t msg;
	static os_timer_id_t timer = OS_MAXIMUM_TIMERS;

	switch (event) {
	case os_event_init:
		//Save task id
		taskID_2 = os_current_task_id();
		//Log the task id
		os_log(os_log_level_all, "Task %d is initialized\n\r", os_current_task_id());
		//Subscribe for message pending
		os_subscribe_for_event(os_event_msg_pending, os_current_task_id());
		//Subscribe for timer event
		os_subscribe_for_event(os_event_timer, os_current_task_id());
		//Create timer for 1 seconds = 1000 milliseconds
		timer = os_timer_add(1000, os_timer_one_shot);
		break;

	case os_event_msg_pending:
		//Retrieve message
		os_retrieve_msg(&msg);
		os_log(os_log_level_all, "Pending message = %d, %d\n\r", msg.os_msg_id, msg.data);
		if((msg.os_msg_id == taskID_1) && (msg.data == 0x55)){
			// Start the timer
			os_timer_start(timer);
		}
		break;

	case os_event_timer:
		os_log(os_log_level_all, "Timer send message from %d to %d\n\r",os_current_task_id(),taskID_3);
		msg.os_msg_id = taskID_2;
		msg.data = 0xAA;
		//Post message to task3
		os_post_msg(msg, taskID_3, os_msg_priority_normal);
		break;
	default:
		break;
	}
	return os_task_succeed; //Succeed
}

/**
 * Task 3
 * @param event
 * @return
 */
os_task_return_codes_t task3_callback(os_event_t event) {
	os_msg_t msg;
	static bool msgReceived = false;

	switch (event) {
	case os_event_init:
		//Save task id
		taskID_3 = os_current_task_id();
		//Log the task id
		os_log(os_log_level_all, "Task %d is initialized\n\r", os_current_task_id());
		//Subscribe for message pending
		os_subscribe_for_event(os_event_msg_pending, os_current_task_id());
		//Subscribe for idle
		os_subscribe_for_event(os_event_idle, os_current_task_id());
		break;

	case os_event_msg_pending:
		//Retrieve message
		os_retrieve_msg(&msg);
		os_log(os_log_level_all, "Pending message = %d, %d\n\r", msg.os_msg_id, msg.data);
		if((msg.os_msg_id == taskID_2) && (msg.data == 0xAA)){
			msgReceived = true;
		}
		break;
	case os_event_idle:
		if(msgReceived){
			os_log(os_log_level_all, "Task %d in idle\n\r", os_current_task_id());
			msgReceived = false;
		}
		break;

	default:
		break;
	}
	return os_task_succeed; //Succeed
}

#endif /* COMPILE_FOR_LINUX */
