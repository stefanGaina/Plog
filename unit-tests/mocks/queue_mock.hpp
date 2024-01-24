/******************************************************************************************************
 * Plog Copyright (C) 2024                                                                            *
 *                                                                                                    *
 * This software is provided 'as-is', without any express or implied warranty. In no event will the   *
 * authors be held liable for any damages arising from the use of this software.                      *
 *                                                                                                    *
 * Permission is granted to anyone to use this software for any purpose, including commercial         *
 * applications, and to alter it and redistribute it freely, subject to the following restrictions:   *
 *                                                                                                    *
 * 1. The origin of this software must not be misrepresented; you must not claim that you wrote the   *
 *    original software. If you use this software in a product, an acknowledgment in the product      *
 *    documentation would be appreciated but is not required.                                         *
 * 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being *
 *    the original software.                                                                          *
 * 3. This notice may not be removed or altered from any source distribution.                         *
 *****************************************************************************************************/

#ifndef QUEUE_MOCK_HPP_
#define QUEUE_MOCK_HPP_

/******************************************************************************************************
 * HEADER FILE INCLUDES                                                                               *
 *****************************************************************************************************/

#include <gmock/gmock.h>

#include "internal/queue.h"

/******************************************************************************************************
 * TYPE DEFINITIONS                                                                                   *
 *****************************************************************************************************/

class Queue
{
public:
	virtual ~Queue(void) = default;

	virtual void queue_init(Queue_t* queue) = 0;
	virtual void queue_deinit(Queue_t* queue) = 0;
	virtual gboolean queue_put(Queue_t* queue, gchar* buffer, guint8 severity_bit) = 0;
	virtual gboolean queue_pop(Queue_t* queue, gchar** buffer, guint8* severity_bit) = 0;
	virtual gboolean queue_is_empty(Queue_t* queue) = 0;
	virtual void queue_interrupt_wait(Queue_t* queue) = 0;
};

class QueueMock : public Queue
{
public:
	QueueMock(void)
	{
		queueMock = this;
	}

	virtual ~QueueMock(void)
	{
		queueMock = nullptr;
	}

	MOCK_METHOD1(queue_init, void(Queue_t*));
	MOCK_METHOD1(queue_deinit, void(Queue_t*));
	MOCK_METHOD3(queue_put, gboolean(Queue_t*, gchar*, guint8));
	MOCK_METHOD3(queue_pop, gboolean(Queue_t*, gchar**, guint8*));
	MOCK_METHOD1(queue_is_empty, gboolean(Queue_t*));
	MOCK_METHOD1(queue_interrupt_wait, void(Queue_t*));

public:
	static QueueMock* queueMock;
};

/******************************************************************************************************
 * LOCAL VARIABLES                                                                                    *
 *****************************************************************************************************/

QueueMock* QueueMock::queueMock = nullptr;

/******************************************************************************************************
 * FUNCTION DEFINITIONS                                                                               *
 *****************************************************************************************************/

extern "C" {

void queue_init(Queue_t* const queue)
{
	ASSERT_NE(nullptr, QueueMock::queueMock) << "queue_init(): nullptr == QueueMock::queueMock";
	QueueMock::queueMock->queue_init(queue);
}

void queue_deinit(Queue_t* const queue)
{
	ASSERT_NE(nullptr, QueueMock::queueMock) << "queue_deinit(): nullptr == QueueMock::queueMock";
	QueueMock::queueMock->queue_deinit(queue);
}

gboolean queue_put(Queue_t* const queue, gchar* const buffer, const guint8 severity_bit)
{
	if (nullptr == QueueMock::queueMock)
	{
		ADD_FAILURE() << "queue_put(): nullptr == QueueMock::queueMock";
		return FALSE;
	}
	return QueueMock::queueMock->queue_put(queue, buffer, severity_bit);
}

gboolean queue_pop(Queue_t* const queue, gchar** const buffer, guint8* const severity_bit)
{
	if (nullptr == QueueMock::queueMock)
	{
		ADD_FAILURE() << "queue_pop(): nullptr == QueueMock::queueMock";
		return FALSE;
	}
	return QueueMock::queueMock->queue_pop(queue, buffer, severity_bit);
}

gboolean queue_is_empty(Queue_t* const queue)
{
	if (nullptr == QueueMock::queueMock)
	{
		ADD_FAILURE() << "queue_is_empty(): nullptr == QueueMock::queueMock";
		return FALSE;
	}
	return QueueMock::queueMock->queue_is_empty(queue);
}

void queue_interrupt_wait(Queue_t* const queue)
{
	ASSERT_NE(nullptr, QueueMock::queueMock) << "queue_interrupt_wait(): nullptr == QueueMock::queueMock";
	QueueMock::queueMock->queue_interrupt_wait(queue);
}

}

#endif /*< QUEUE_MOCK_HPP_ */
