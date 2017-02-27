/* -----------------------------------------------------------------------------
 * os_queue.c                                             (c) 2017 Rocks
 * -----------------------------------------------------------------------------
 * Author: Gertjan Rocks
 * Web:    www.gertjanrocks.com
 * Mail:   gertjanrocks@outlook.com
 * -----------------------------------------------------------------------------
 * Description: 
 * -----------------------------------------------------------------------------
 * Created on: Feb 23, 2017
 * -----------------------------------------------------------------------------
 */

/* --------------*
 * Include files *
 * --------------*
 */

#include <string.h>
#include "os_queue.h"

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

/* ---------------------*
 * Function definitions *
 * ---------------------*
 */

/**
 * queue initialize
 * @param queue
 * @param buffer
 * @param bufferSize
 * @param elementSize
 */
void os_queue_init(os_queue_t *queue, void* buffer, uint16_t bufferSize,
		uint16_t elementSize) {
	queue->buffer = buffer;
	queue->capacity = bufferSize;
	queue->elementSize = elementSize;
	queue->currentSize = 0;
	queue->in = 0;
	queue->out = 0;

}

/**
 * add element to queue
 * @param queue
 * @param element
 */
void os_queue_add(os_queue_t *queue, const void* element) {

	(void) memcpy(queue->buffer + queue->in * queue->elementSize, element,
			queue->elementSize);
	++queue->currentSize;
	++queue->in;
	queue->in %= queue->capacity;

}

/**
 * remove element from queue
 * @param queue
 */
void* os_queue_remove(os_queue_t *queue) {
	void *value;

	if (queue->currentSize > 0) {
		value = queue->buffer + queue->out * queue->elementSize;
		--queue->currentSize;
		++queue->out;
		queue->out %= queue->capacity;
	}

	return value;
}

/**
 * returns queue size
 * @param queue
 * @return
 */
uint16_t os_queue_size(os_queue_t *queue) {
	return queue->currentSize;
}

/**
 * return the queue capacity
 * @param queue
 * @return
 */
uint16_t os_queue_capacity(os_queue_t *queue) {
	return queue->capacity;
}

/**
 * return true if queue is full
 * @param queue
 * @return
 */
bool os_queue_isFull(os_queue_t *queue) {
	bool full;
	full = queue->currentSize == queue->capacity;
	return full;
}


/**
 * return true is queue is empty
 * @param queue
 * @return
 */
bool os_queue_isEmpty(os_queue_t *queue) {
	bool empty;
	empty = queue->currentSize == 0;
	return empty;
}
