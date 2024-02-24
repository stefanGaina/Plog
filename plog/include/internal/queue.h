/******************************************************************************************************
 * Plog Copyright (C) 2024
 *
 * This software is provided 'as-is', without any express or implied warranty. In no event will the
 * authors be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose, including commercial
 * applications, and to alter it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not claim that you wrote the
 *    original software. If you use this software in a product, an acknowledgment in the product
 *    documentation would be appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being
 *    the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *****************************************************************************************************/

/** ***************************************************************************************************
 * @file queue.h
 * @author Gaina Stefan
 * @date 08.12.2023
 * @brief This file defines queue data structure that is used internally by Plog and not meant to be
 * public API.
 * @todo N/A.
 * @bug No known bugs.
 *****************************************************************************************************/

#ifndef INTERNAL_QUEUE_H_
#define INTERNAL_QUEUE_H_

/******************************************************************************************************
 * HEADER FILE INCLUDES
 *****************************************************************************************************/

#include <glib.h>

/******************************************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************************************/

/** ***************************************************************************************************
 * @brief Opaque data structure for storing log buffers and severity bits and getting them in a FIFO
 * way.
 *****************************************************************************************************/
typedef struct s_Queue_t
{
	gchar dummy[40]; /**< The size of the queue is 40 bytes. */
}
Queue_t;

/******************************************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/** ***************************************************************************************************
 * @brief Initializes the queue. Do not call any other function before this.
 * @param queue: Queue object.
 * @return void
 *****************************************************************************************************/
extern void queue_init(Queue_t* queue);

/** ***************************************************************************************************
 * @brief Deinitializes the queue. Do not call any other function after this.
 * @param queue: Queue object.
 * @return void
 *****************************************************************************************************/
extern void queue_deinit(Queue_t* queue);

/** ***************************************************************************************************
 * @brief Pushes a node in the queue.
 * @param queue: Queue object.
 * @param[in] buffer: Log buffer to be stored (the queue does not take ownership).
 * @param severity_bit: Severity bit to be stored.
 * @return TRUE - the node has been successfully put.
 * @return FALSE - failed to allocate memory for the new node.
 *****************************************************************************************************/
extern gboolean queue_push(Queue_t* queue, gchar* buffer, guint8 severity_bit);

/** ***************************************************************************************************
 * @brief Pops a node from the queue (if the queue is empty this function blocks until it is no longer
 * empty, has been deinitialized or queue_interrupt_wait() has been called).
 * @param queue: Queue object.
 * @param[out] buffer: Stored log buffer.
 * @param[out] severity_bit: Stored severity bit.
 * @return TRUE - buffer and severity bit are valid.
 * @return FALSE - buffer and severity bit are invalid.
 *****************************************************************************************************/
extern gboolean queue_pop(Queue_t* queue, gchar** buffer, guint8* severity_bit);

/** ***************************************************************************************************
 * @brief Queries if the queue currently has any node.
 * @param queue: Queue object.
 * @return TRUE - the queue does not store any node.
 * @return FALSE - the queue does store at least one node.
 *****************************************************************************************************/
extern gboolean queue_is_empty(Queue_t* queue);

/** ***************************************************************************************************
 * @brief Interupts the wait inside queue_pop in case the queue is empty.
 * @param queue: Queue object.
 * @return void
 *****************************************************************************************************/
extern void queue_interrupt_wait(Queue_t* queue);

#ifdef __cplusplus
}
#endif

#endif /*< INTERNAL_QUEUE_H_ */
