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
 * @file queue.c
 * @author Gaina Stefan
 * @date 15.12.2023
 * @brief This file implements the interface defined in queue.h.
 * @todo N/A.
 * @bug No known bugs.
 *****************************************************************************************************/

/******************************************************************************************************
 * HEADER FILE INCLUDES
 *****************************************************************************************************/

#include "internal/queue.h"

/******************************************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************************************/

typedef struct s_Node_t Node_t;
/** ***************************************************************************************************
 * @brief Data that will be stored in the queue.
 *****************************************************************************************************/
struct s_Node_t
{
	gchar*	buffer;		  /**< Stored log buffer.                       */
	guint8	severity_bit; /**< Stored severity bit.                     */
	Node_t* next;		  /**< Reference to the next node in the queue. */
};

/** ***************************************************************************************************
 * @brief Explicit data type of the queue for internal usage.
 *****************************************************************************************************/
typedef struct s_PrivateQueue_t
{
	Node_t* head;	   /**< The oldest node.                            */
	Node_t* tail;	   /**< The latest node.                            */
	GMutex	lock;	   /**< Lock for thread-safe access.                */
	GCond	condition; /**< Condition signaled when queue is not empty. */
} PrivateQueue_t;

/******************************************************************************************************
 * FUNCTION DEFINITIONS
 *****************************************************************************************************/

void queue_init(Queue_t* const public_queue)
{
	PrivateQueue_t* const queue = (PrivateQueue_t*)public_queue;

	g_mutex_init(&queue->lock);
	g_cond_init(&queue->condition);

	queue->head = NULL;
	queue->tail = NULL;
}

void queue_deinit(Queue_t* const public_queue)
{
	PrivateQueue_t* const queue = (PrivateQueue_t*)public_queue;
	Node_t*				  node	= NULL;

	g_mutex_lock(&queue->lock);

	while (NULL != queue->head)
	{
		node		= queue->head;
		queue->head = queue->head->next;

		g_free(node);
		node = NULL;
	}
	queue->tail = NULL;

	g_cond_signal(&queue->condition);
	g_cond_clear(&queue->condition);

	g_mutex_unlock(&queue->lock);
	g_mutex_clear(&queue->lock);
}

gboolean queue_push(Queue_t* const public_queue, gchar* const buffer, const guint8 severity_bit)
{
	PrivateQueue_t* const queue = (PrivateQueue_t*)public_queue;
	Node_t*				  node	= NULL;

	g_mutex_lock(&queue->lock);

	node = (Node_t*)g_try_malloc(sizeof(Node_t));
	if (NULL == node)
	{
		g_mutex_unlock(&queue->lock);
		return FALSE;
	}

	node->buffer	   = buffer;
	node->severity_bit = severity_bit;
	node->next		   = NULL;

	if (NULL == queue->tail)
	{
		queue->tail = node;
		queue->head = node;
	}
	else
	{
		queue->tail->next = node;
		queue->tail		  = node;
	}

	g_cond_signal(&queue->condition);
	g_mutex_unlock(&queue->lock);

	return TRUE;
}

gboolean queue_pop(Queue_t* const public_queue, gchar** const buffer, guint8* const severity_bit)
{
	PrivateQueue_t* const queue = (PrivateQueue_t*)public_queue;
	Node_t*				  node	= NULL;

	g_mutex_lock(&queue->lock);

	if (NULL == queue->tail)
	{
		/* This is commented because spurious wake-ups will return right back here and     */
		/* we want to be able to exit in case of queue_deinit() or queue_interrupt_wait(). */
		// while (NULL == queue->tail)
		// {
		g_cond_wait(&queue->condition, &queue->lock);
		// }
	}

	node = queue->head;
	if (NULL == node)
	{
		g_mutex_unlock(&queue->lock);
		return FALSE;
	}

	if (queue->head == queue->tail)
	{
		queue->tail = NULL;
	}
	queue->head = queue->head->next;

	*buffer		  = node->buffer;
	*severity_bit = node->severity_bit;

	g_free(node);
	node = NULL;

	g_mutex_unlock(&queue->lock);
	return TRUE;
}

gboolean queue_is_empty(Queue_t* const public_queue)
{
	PrivateQueue_t* const queue	 = (PrivateQueue_t*)public_queue;
	gboolean			  result = FALSE;

	g_mutex_lock(&queue->lock);
	result = NULL == queue->tail;
	g_mutex_unlock(&queue->lock);

	return result;
}

void queue_interrupt_wait(Queue_t* const public_queue)
{
	PrivateQueue_t* const queue = (PrivateQueue_t*)public_queue;

	g_mutex_lock(&queue->lock);
	g_cond_signal(&queue->condition);
	g_mutex_unlock(&queue->lock);
}
