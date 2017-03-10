/* -----------------------------------------------------------------------------
 * os_queue.h                                             (c) 2017 Rocks
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

#ifndef OS_QUEUE_H_
#define OS_QUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* --------------*
 * Include files *
 * --------------*
 */

#include "stdint.h"
#include "stdbool.h"

/* ----------------------*
 * Function declarations *
 * ----------------------*
 */


/* -------------------------------*
 * Constant and macro definitions *
 * -------------------------------*
 */

/* -----------------*
 * Type definitions *
 * -----------------*
 */

/**
 * This structure declares the queue
 */
typedef struct
{
   void* buffer;             //!< \private
   uint16_t capacity;        //!< \private
   uint16_t elementSize;     //!< \private
   uint16_t currentSize;     //!< \private
   int in;                   //!< \private
   int out;                  //!< \private
}os_queue_t;

/* ----------------------*
 * Function declarations *
 * ----------------------*
 */
/**
 * Initialize the queue buffer
 * @param queue			pointer to queue
 * @param buffer		pointer to buffer
 * @param bufferSize	size of buffer
 * @param elementSize	size of one element
 */
void os_queue_init(os_queue_t *queue, void* buffer, uint16_t bufferSize, uint16_t elementSize);

/**
 * add a item to queue
 * @param queue
 * @param element to add
 */
void os_queue_add(os_queue_t *queue, const void* element);

/**
 * removes the first item from the queue
 * @param queue
 * @return the first item available from the queue
 */
void os_queue_remove(os_queue_t *queue, void* element);

/**
 * return the current size of the queue
 * @param queue
 * @return the current size
 */
uint16_t os_queue_size(os_queue_t *queue);

/**
 * returns the capacity of the queue
 * @param queue
 * @return the size of the queue
 */
uint16_t os_queue_capacity(os_queue_t *queue);

/**
 * returns true if the queue is full and false when not full
 * @param queue
 * @return
 */
bool os_queue_isFull(os_queue_t *queue);

/**
 * return true if the queue is empty and false when not empty
 * @param queue
 * @return
 */
bool os_queue_isEmpty(os_queue_t *queue);

#ifdef __cplusplus
}
#endif


#endif /* OS_QUEUE_H_ */
