/*
 ============================================================================
 Name        : simple_tasks.c
 Author      : Gertjan Rocks
 Version     :
 Copyright   : Your copyright notice
 Description : Create three simple tasks with most of the os features
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "os.h"

os_task_t test_task;
os_task_t test_task2;
os_task_return_codes_t test_task_callback(os_event_t event);
os_task_return_codes_t test2_task_callback(os_event_t event);
os_task_id_t task1, task2;
os_functions_pointers_t function_pointers;

void disable_interrupt(void);
void enable_interrupt(void);
void stdio_function(void);

int main(void) {

		test_task.task_cb 		= &test_task_callback;
		test_task.task_name 	= "test";

		test_task2.task_cb 		= &test2_task_callback;
		test_task2.task_name 	= "test2";

		function_pointers.disable_irq = &disable_interrupt;
		function_pointers.enable_irq = &enable_interrupt;
		function_pointers.stdio = &stdio_function;

		os_add_function_pointers(&function_pointers);
		os_init();
		os_add_task(test_task);
		os_add_task(test_task2);



		os_main();
	return EXIT_SUCCESS;
}

void disable_interrupt(void){
	os_log(os_log_level_all,"disable_interrupt\n\r");
}

void enable_interrupt(void){
	os_log(os_log_level_all,"enable_interrupt\n\r");
}

void stdio_function(void){
	os_log(os_log_level_all,"stdio_function\n\r");
}

os_task_return_codes_t test_task_callback(os_event_t event)
{
	os_msg_t msg;
	switch(event){
	case os_event_init:
		os_log(os_log_level_all,"Success from init\n\r");
		os_subscribe_for_event(os_event_msg_pending, os_current_task_id());
		msg.os_msg_id = 10;
		msg.data = 11;
		os_post_msg(msg, os_current_task_id(), os_msg_priority_normal);
		task1 = os_current_task_id();
		break;
	case os_event_msg_pending:
		os_retrieve_msg(&msg);
		os_log(os_log_level_all,"Success from pending message = %d, %d\n\r",msg.os_msg_id, msg.data);
		msg.data++;
		os_post_msg(msg, task2, os_msg_priority_normal);
		break;
	default:
	break;
	}
	os_log(os_log_level_all,"Success from task\n\r");
	return os_task_succeed; //Succeed
}

os_task_return_codes_t test2_task_callback(os_event_t event)
{
	os_msg_t msg;
	switch(event){
	case os_event_init:
		os_log(os_log_level_all,"Success from init\n\r");
		os_subscribe_for_event(os_event_msg_pending, os_current_task_id());
		os_subscribe_for_event(os_event_idle, os_current_task_id());
		task2 = os_current_task_id();
		break;
	case os_event_msg_pending:
		os_retrieve_msg(&msg);
		os_log(os_log_level_all,"Success from pending message 2 = %d, %d\n\r",msg.os_msg_id, msg.data);
		msg.data++;
		os_post_msg(msg, task1, os_msg_priority_normal);
		break;
	case os_event_idle:
		os_log(os_log_level_all, "idle from %d\n\r",task2);
		break;
	default:
	break;
	}
	os_log(os_log_level_all,"Success from task\n\r");
	return os_task_succeed; //Succeed
}

