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
 * @file queue_test.c
 * @author Gaina Stefan
 * @date 15.12.2023
 * @brief This file unit-tests queue.c.
 * @details Current coverage report:
 * Line coverage: 100.0% (73/73)
 * Functions:     100.0% (6/6)
 * Branches:      100.0% (12/12)
 * @todo N/A.
 * @bug No known bugs.
 *****************************************************************************************************/

/******************************************************************************************************
 * HEADER FILE INCLUDES
 *****************************************************************************************************/

#include <gtest/gtest.h>

#include "glib_mock.hpp"
#include "internal/queue.h"

/******************************************************************************************************
 * MACROS
 *****************************************************************************************************/

/** ***************************************************************************************************
 * @brief Dummy address to pass the != NULL check.
 *****************************************************************************************************/
#define NOT_NULL (void*)1

/******************************************************************************************************
 * TEST CLASS
 *****************************************************************************************************/

class QueueTest : public testing::Test
{
public:
	QueueTest(void)
		: glibMock{}
	{
	}

	~QueueTest(void) = default;

protected:
	void SetUp(void) override
	{
	}

	void TearDown(void) override
	{
	}

public:
	GlibMock glibMock;
};

/******************************************************************************************************
 * queue_init
 *****************************************************************************************************/

TEST_F(QueueTest, queue_init_success)
{
	Queue_t queue = {};

	queue_init(&queue);
	ASSERT_EQ(TRUE, queue_is_empty(&queue)) << "The queue is not empty after initialization!";

	queue_deinit(&queue);
	ASSERT_EQ(TRUE, queue_is_empty(&queue)) << "The queue is not empty after deinitialization!";
}

/******************************************************************************************************
 * queue_put
 *****************************************************************************************************/

TEST_F(QueueTest, queue_put_tryMalloc_fail)
{
	Queue_t queue = {};

	queue_init(&queue);

	EXPECT_CALL(glibMock, g_try_malloc(testing::_)) /**/
		.WillOnce(testing::Return((gpointer)NULL));
	ASSERT_EQ(FALSE, queue_put(&queue, NULL, 0U)) << "Successfully put node in queue even though memory allocation failed!";

	queue_deinit(&queue);
}

TEST_F(QueueTest, queue_put_success)
{
	Queue_t	 queue = {};
	gpointer node  = NULL;

	queue_init(&queue);

	EXPECT_CALL(glibMock, g_try_malloc(testing::_))
		.WillOnce(testing::Invoke(
			[&node](const gsize n_bytes) -> gpointer
			{
				void* const auxiliary = malloc(n_bytes);

				(void)memcpy((gpointer*)&node, (const void*)&auxiliary, sizeof(node));
				if (NULL == node)
				{
					ADD_FAILURE() << "Failed to allocate memory for node!";
					return NULL;
				}
				return node;
			}));
	ASSERT_EQ(TRUE, queue_put(&queue, NULL, 0U)) << "Failed to put node in queue!";

	EXPECT_CALL(glibMock, g_free(testing::_));
	queue_deinit(&queue);
	free(node);
}

/******************************************************************************************************
 * queue_pop
 *****************************************************************************************************/

TEST_F(QueueTest, queue_pop_success)
{
	Queue_t	 queue			= {};
	gpointer node1			= NULL;
	gpointer node2			= NULL;
	gchar	 buffer1[]		= "BUFFER1";
	gchar	 buffer2[]		= "BUFFER2";
	gchar*	 buffer			= NULL;
	guint8	 severity_level = 0U;

	queue_init(&queue);

	EXPECT_CALL(glibMock, g_try_malloc(testing::_))
		.WillOnce(testing::Invoke(
			[&node1](const gsize n_bytes) -> gpointer
			{
				void* const auxiliary = malloc(n_bytes);

				(void)memcpy((gpointer*)&node1, (const void*)&auxiliary, sizeof(node1));
				if (NULL == node1)
				{
					ADD_FAILURE() << "Failed to allocate memory for node1!";
					return NULL;
				}
				return node1;
			}));
	ASSERT_EQ(TRUE, queue_put(&queue, buffer1, 127U)) << "Failed to put node in queue!";

	EXPECT_CALL(glibMock, g_try_malloc(testing::_))
		.WillOnce(testing::Invoke(
			[&node2](const gsize n_bytes) -> gpointer
			{
				void* const auxiliary = malloc(n_bytes);

				(void)memcpy((gpointer*)&node2, (const void*)&auxiliary, sizeof(node2));
				if (NULL == node2)
				{
					ADD_FAILURE() << "Failed to allocate memory for node2!";
					return NULL;
				}
				return node2;
			}));
	ASSERT_EQ(TRUE, queue_put(&queue, buffer2, 63U)) << "Failed to put node in queue!";

	EXPECT_CALL(glibMock, g_free(testing::_));
	ASSERT_EQ(TRUE, queue_pop(&queue, &buffer, &severity_level)) << "Failed to pop node from queue";
	ASSERT_EQ(buffer1, buffer) << "Incorrect buffer popped!";
	ASSERT_EQ(127U, severity_level) << "Invalid severity level popped!";

	EXPECT_CALL(glibMock, g_free(testing::_));
	ASSERT_EQ(TRUE, queue_pop(&queue, &buffer, &severity_level)) << "Failed to pop node from queue";
	ASSERT_EQ(buffer2, buffer) << "Incorrect buffer popped!";
	ASSERT_EQ(63U, severity_level) << "Invalid severity level popped!";

	EXPECT_CALL(glibMock, g_cond_wait(testing::_, testing::_));
	ASSERT_EQ(TRUE, queue_is_empty(&queue)) << "The queue is not empty after popping 2 nodes!";
	ASSERT_EQ(FALSE, queue_pop(&queue, &buffer, &severity_level));
	ASSERT_EQ(buffer2, buffer) << "Buffer changed after failed pop!";
	ASSERT_EQ(63U, severity_level) << "Severity level changed after failed pop!";

	queue_deinit(&queue);
	free(node1);
	free(node2);
}

/******************************************************************************************************
 * queue_interrupt_wait
 *****************************************************************************************************/

TEST_F(QueueTest, queue_interrupt_wait_success)
{
	Queue_t queue = {};

	queue_init(&queue);
	queue_interrupt_wait(&queue);
	queue_deinit(&queue);
}
