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
******************************************************************************************************/

/******************************************************************************************************
 * @file plog_test.cpp                                                                                *
 * @date:      @author:                   Reason for change:                                          *
 * 22.06.2023  Gaina Stefan               Initial version.                                            *
 * 20.12.2023  Gaina Stefan               Updated copyright.                                          *
 * 13.10.2024  Gaina Stefan               Updated the tests.                                          *
 * Current coverage report:                                                                           *
 * Line coverage: 64.0% (146/228)                                                                     *
 * Functions:     68.4% (13/19)                                                                       *
 * Branches:      45.0% (37/81)                                                                       *
 * @details This file unit-tests plog.c.                                                              *
 * @todo Finish the remaining tests.                                                                  *
 * @bug No known bugs.                                                                                *
 *****************************************************************************************************/

/******************************************************************************************************
 * HEADER FILE INCLUDES                                                                               *
 *****************************************************************************************************/

#include <gtest/gtest.h>

#include "queue_mock.hpp"
#include "configuration_mock.hpp"
#include "glib_mock.hpp"
#include "plog.h"

/******************************************************************************************************
 * CONSTANTS                                                                                          *
 *****************************************************************************************************/

#define NOT_NULL (void*)1

static constexpr const guint8 SEVERITY_LEVEL_ALL =
	E_PLOG_SEVERITY_LEVEL_FATAL |
	E_PLOG_SEVERITY_LEVEL_ERROR |
	E_PLOG_SEVERITY_LEVEL_WARN  |
	E_PLOG_SEVERITY_LEVEL_INFO  |
	E_PLOG_SEVERITY_LEVEL_DEBUG |
	E_PLOG_SEVERITY_LEVEL_TRACE |
	E_PLOG_SEVERITY_LEVEL_VERBOSE;

/******************************************************************************************************
 * TEST CLASS                                                                                         *
 *****************************************************************************************************/

class PlogTest : public testing::Test
{
public:
	PlogTest(void)
		: configurationMock{}
		, queueMock        {}
		, glibMock         {}
	{
	}

	~PlogTest(void) = default;

protected:
	void SetUp(void) override
	{
	}

	void TearDown(void) override
	{
		plog_deinit();
	}

public:
	ConfigurationMock configurationMock;
	QueueMock         queueMock;
	GlibMock          glibMock;
};

/******************************************************************************************************
 * plog_init                                                                                          *
 *****************************************************************************************************/

TEST_F(PlogTest, plog_init_fileOpen_fail)
{
	ASSERT_EQ(FALSE, plog_init("read_only.txt")) << "Successfully initialized Plog using read-only file!";
}

TEST_F(PlogTest, plog_init_configurationRead_fail)
{
	EXPECT_CALL(configurationMock, configuration_read())
		.WillOnce(testing::Return(FALSE));
	ASSERT_EQ(FALSE, plog_init(NULL)) << "Successfully initialized Plog without reading configuration!";
}

TEST_F(PlogTest, plog_init_tryMalloc_fail)
{
	EXPECT_CALL(configurationMock, configuration_read())
		.WillOnce(testing::Return(TRUE));
	EXPECT_CALL(glibMock, g_try_malloc(strlen(PLOG_DEFAULT_FILE_NAME) + 5UL))
		.WillOnce(testing::Return((gpointer)NULL));
	ASSERT_EQ(FALSE, plog_init(NULL)) << "Successfully initialized Plog without copying file name!";
}

TEST_F(PlogTest, plog_init_alreadyInit_success)
{
	gchar buffer[128] = "";

	EXPECT_CALL(configurationMock, configuration_read())
		.WillOnce(testing::Return(TRUE));
	EXPECT_CALL(glibMock, g_try_malloc(strlen(PLOG_DEFAULT_FILE_NAME) + 5UL))
		.WillOnce(testing::Return((gpointer)buffer));
	ASSERT_EQ(TRUE, plog_init("")) << "Failed to initialize Plog with default file name!";
	ASSERT_EQ(FALSE, plog_init(NULL)) << "Multiple initialization succeeded!";

	EXPECT_CALL(configurationMock, configuration_write());
	EXPECT_CALL(glibMock, g_free(testing::_));
}

/******************************************************************************************************
 * plog_set_buffer_size                                                                               *
 *****************************************************************************************************/

TEST_F(PlogTest, plog_set_buffer_size_notInitialized_fail)
{
	ASSERT_EQ(FALSE, plog_set_buffer_size(128UL)) << "Succeeded to set buffer size to 128 without initialization!";
	ASSERT_EQ(0UL, plog_get_buffer_size());
}

TEST_F(PlogTest, plog_set_buffer_size_tryRealloc_fail)
{
	gchar buffer[128] = "";

	EXPECT_CALL(configurationMock, configuration_read())
		.WillOnce(testing::Return(TRUE));
	EXPECT_CALL(glibMock, g_try_malloc(strlen(PLOG_DEFAULT_FILE_NAME) + 5UL))
		.WillOnce(testing::Return((gpointer)buffer));
	ASSERT_EQ(TRUE, plog_init(NULL)) << "Failed to initialize Plog with default file name!";

	EXPECT_CALL(glibMock, g_try_realloc(testing::_, testing::_))
		.WillOnce(testing::Return((gpointer)NULL));
	ASSERT_EQ(FALSE, plog_set_buffer_size(128UL)) << "Succeeded to set buffer size to 128 without reallocation!";

	ASSERT_EQ(0UL, plog_get_buffer_size());
	EXPECT_CALL(configurationMock, configuration_write());
	EXPECT_CALL(glibMock, g_free(testing::_));
}

TEST_F(PlogTest, plog_set_buffer_size_tryNewThread_fail)
{
	gchar buffer[128] = "";

	EXPECT_CALL(configurationMock, configuration_read())
		.WillOnce(testing::Return(TRUE));
	EXPECT_CALL(glibMock, g_try_malloc(strlen(PLOG_DEFAULT_FILE_NAME) + 5UL))
		.WillOnce(testing::Return((gpointer)buffer));
	ASSERT_EQ(TRUE, plog_init(NULL)) << "Failed to initialize Plog with default file name!";

	EXPECT_CALL(glibMock, g_try_realloc(testing::_, testing::_))
		.WillOnce(testing::Return((gpointer)buffer));
	EXPECT_CALL(queueMock, queue_init(testing::_));
	EXPECT_CALL(glibMock, g_thread_try_new(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::Return((GThread*)NULL));
	EXPECT_CALL(glibMock, g_free(testing::_));
	EXPECT_CALL(queueMock, queue_deinit(testing::_));
	ASSERT_EQ(FALSE, plog_set_buffer_size(128UL)) << "Succeeded to set buffer size to 128 without creating worker thread!";

	ASSERT_EQ(0UL, plog_get_buffer_size()) << "Buffer size has been set even though it failed!";
	EXPECT_CALL(configurationMock, configuration_write());
	EXPECT_CALL(glibMock, g_free(testing::_));
}

// TEST_FF(PlogTest, plog_set_buffer_size_success)
// {
// 	gchar           buffer[128]           = "";
// 	pthread_t       thread                = 0U;
// 	gchar           test_log[]            = "Test log!";
// 	gboolean        queue_is_empty_return = TRUE;
// 	gboolean        queue_pop_return      = TRUE;
// 	pthread_cond_t  cond                  = {};
// 	pthread_mutex_t mutex                 = {};
// 	gint32          error_code            = 0;

// 	error_code = pthread_cond_init(&cond, NULL);
// 	if (0 != error_code)
// 	{
// 		ADD_FAILURE() << "Failed to initialize cond! (error message: " << strerror(error_code) << ")";
// 	}

// 	error_code = pthread_mutex_init(&mutex, NULL);
// 	if (0 != error_code)
// 	{
// 		ADD_FAILURE() << "Failed to initialize mutex! (error message: " << strerror(error_code) << ")";
// 	}

// 	EXPECT_CALL(configurationMock, configuration_read())
// 		.WillOnce(testing::Return(TRUE));
// 	EXPECT_CALL(glibMock, g_try_malloc(testing::_))
// 		.WillRepeatedly(testing::Return((gpointer)buffer));
// 	ASSERT_EQ(TRUE, plog_init(NULL)) << "Failed to initialize Plog with default file name!";

// 	EXPECT_CALL(glibMock, g_try_realloc(testing::_, testing::_))
// 		.WillOnce(testing::Return((gpointer)buffer));
// 	EXPECT_CALL(queueMock, queue_init(testing::_));
// 	EXPECT_CALL(glibMock, g_thread_try_new(testing::_, testing::_, testing::_, testing::_))
// 		.WillOnce(testing::Invoke([&thread] (const gchar* const name, GThreadFunc const func, gpointer const data, GError** const error) -> GThread*
// 		{
// 			const gint32 error_code = pthread_create(&thread, NULL, func, data);
// 			if (0 != error_code)
// 			{
// 				ADD_FAILURE() << "Failed to create work thread! (error message: " << strerror(error_code) << ")";
// 			}
// 			return NOT_NULL;
// 		}));

// 	ASSERT_EQ(TRUE, plog_set_buffer_size(128UL)) << "Failed to set buffer size to 128!";
// 	ASSERT_EQ(128UL, plog_get_buffer_size()) << "Buffer size has not been set even though it succeeded!";

// 	ON_CALL(queueMock, queue_is_empty(testing::_))
// 		.WillByDefault(testing::Invoke([&queue_is_empty_return] (const Queue_t* const public_queue) -> gboolean
// 		{
// 			return queue_is_empty_return;
// 		}));
// 	EXPECT_CALL(queueMock, queue_pop(testing::_, testing::_, testing::_))
// 		.WillRepeatedly(testing::Invoke([&queue_pop_return, &test_log, &cond, &mutex]
// 			(Queue_t* const queue, gchar** const buffer, guint8* const severity_bit) -> gboolean
// 		{
// 			gint32 error_code = 0;

// 			(void)pthread_mutex_lock(&mutex);

// 			error_code = pthread_cond_wait(&cond, &mutex);
// 			if (0 != error_code)
// 			{
// 				ADD_FAILURE() << "Failed to wait cond! (error message: " << strerror(error_code) << ")";
// 			}

// 			*buffer       = test_log;
// 			*severity_bit = 127U;

// 			(void)pthread_mutex_unlock(&mutex);
// 			return queue_pop_return;
// 		}));

// 	plog_set_severity_level(E_PLOG_SEVERITY_LEVEL_INFO);
// 	ASSERT_EQ(E_PLOG_SEVERITY_LEVEL_INFO, plog_get_severity_level()) << "Failed to set severity level!";

// 	EXPECT_CALL(glibMock, g_free(testing::_));
// 	EXPECT_CALL(queueMock, queue_put(testing::_, testing::_, testing::_));
// 	plog_info("File only log!");
// 	(void)pthread_mutex_lock(&mutex);
// 	(void)pthread_cond_signal(&cond);
// 	(void)pthread_mutex_unlock(&mutex);

// 	plog_set_terminal_mode(TRUE);
// 	ASSERT_EQ(TRUE, plog_get_terminal_mode()) << "Terminal mode has not been set!";

// 	EXPECT_CALL(glibMock, g_free(testing::_));
// 	EXPECT_CALL(queueMock, queue_put(testing::_, testing::_, testing::_));
// 	plog_info("Terminal log!");
// 	(void)pthread_mutex_lock(&mutex);
// 	(void)pthread_cond_signal(&cond);
// 	(void)pthread_mutex_unlock(&mutex);

// 	EXPECT_CALL(glibMock, g_try_realloc(testing::_, testing::_))
// 		.WillOnce(testing::Return((gpointer)NULL));
// 	EXPECT_CALL(queueMock, queue_deinit(testing::_));
// 	EXPECT_CALL(glibMock, g_thread_exit(testing::_));

// 	EXPECT_CALL(queueMock, queue_interrupt_wait(testing::_))
// 		.WillOnce(testing::Invoke([&cond, &mutex, &queue_pop_return] (Queue_t* const queue) -> void
// 		{
// 			// TODO
// 			// (void)pthread_mutex_lock(&mutex);
// 			// queue_pop_return = FALSE;
// 			// (void)pthread_cond_signal(&cond);
// 			// (void)pthread_mutex_unlock(&mutex);
// 		}));

// 	EXPECT_CALL(glibMock, g_thread_join(testing::_))
// 		.WillOnce(testing::Invoke([&thread] (GThread* const gthread) -> gpointer
// 		{
// 			const gint32 error_code = pthread_join(thread, NULL);

// 			if (0 != error_code)
// 			{
// 				ADD_FAILURE() << "Failed to join thread! (error message: " << strerror(error_code) << ")";
// 			}

// 			return NULL;
// 		}));
// 	ASSERT_EQ(TRUE, plog_set_buffer_size(0UL)) << "Failed to set buffer size to 0!";
// 	ASSERT_EQ(0UL, plog_get_buffer_size()) << "Buffer size has not been set even though it succeeded!";

// 	EXPECT_CALL(configurationMock, configuration_write());
// 	EXPECT_CALL(glibMock, g_free(testing::_));

// 	error_code = pthread_cond_destroy(&cond);
// 	if (0 != error_code)
// 	{
// 		ADD_FAILURE() << "Failed to destroy cond! (error message: " << strerror(error_code) << ")";
// 	}

// 	error_code = pthread_mutex_destroy(&mutex);
// 	if (0 != error_code)
// 	{
// 		ADD_FAILURE() << "Failed to destroy mutex! (error message: " << strerror(error_code) << ")";
// 	}
// }

/******************************************************************************************************
 * plog_internal                                                                                      *
 *****************************************************************************************************/

TEST_F(PlogTest, plog_internal_success)
{
	gchar buffer[128] = "";

	EXPECT_CALL(configurationMock, configuration_read())
		.WillOnce(testing::Return(TRUE));
	EXPECT_CALL(glibMock, g_try_malloc(testing::_))
		.WillRepeatedly(testing::Return((gpointer)buffer));
	ASSERT_EQ(TRUE, plog_init(NULL)) << "Failed to initialize Plog with default file name!";

	plog_set_severity_level(SEVERITY_LEVEL_ALL);
	ASSERT_EQ(SEVERITY_LEVEL_ALL, plog_get_severity_level()) << "Failed to set severity level!";

	// TODO
	// plog_set_file_size(128);
	// plog_set_file_count(2);

	plog_fatal("File only log!");
	plog_error("File only log!");
	plog_warn("File only log!");
	plog_info("File only log!");
	plog_debug("File only log!");
	plog_trace("File only log!");
	plog_verbose("File only log!");

	plog_set_terminal_mode(TRUE);
	ASSERT_EQ(TRUE, plog_get_terminal_mode()) << "Terminal mode has not been set!";

	plog_fatal("Terminal log!");
	plog_error("Terminal log!");
	plog_warn("Terminal log!");
	plog_info("Terminal log!");
	plog_debug("Terminal log!");
	plog_trace("Terminal log!");
	plog_verbose("Terminal log!");

	EXPECT_CALL(glibMock, g_free(testing::_));
}

// TEST_FF(PlogTest, plog_internal_terminal_success)
// {
//	plog_info("Terminal log!");
// }


