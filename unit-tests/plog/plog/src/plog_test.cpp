/******************************************************************************************************
 * Plog Copyright (C) 2023                                                                            *
 *                                                                                                    *
 * This software is provided 'as-is', without any express or implied warranty. In no event will the   *
 * authors be held liable for any damages arising from the use of this software.                      *
 *                                                                                                    *
 * Permission is granted to anyone to use this software for any purpose, including commercial         *
 * applications, and to alter it and redistribute it freely, subject to the following restrictions:   *
 *                                                                                                    *
 * 1. The origin of this software must not be misrepresented; you must not claim that you wrote the   *
 * original software. If you use this software in a product, an acknowledgment in the product         *
 * documentation would be appreciated but is not required.                                            *
 * 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being *
 * the original software.                                                                             *
 * 3. This notice may not be removed or altered from any source distribution.                         *
******************************************************************************************************/

/******************************************************************************************************
 * @file plog_test.cpp                                                                                *
 * @date:      @author:                   Reason for change:                                          *
 * 22.06.2023  Gaina Stefan               Initial version.                                            *
 * @details This file unit-tests plog.c.                                                              *
 * @todo Refactor because there were many changes done.                                               *
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
 * MACROS                                                                                             *
 *****************************************************************************************************/

#define NOT_NULL (void*)1

/******************************************************************************************************
 * TEST CLASS                                                                                         *
 *****************************************************************************************************/

class PlogTest : public testing::Test
{
public:
	PlogTest(void)
		: m_configurationMock{}
		, m_queueMock        {}
		, m_glibMock         {}
	{
	}

	~PlogTest(void) = default;

	void SetUp(void) override
	{
	}

	void TearDown(void) override
	{
		plog_deinit();
	}

public:
	ConfigurationMock m_configurationMock;
	QueueMock         m_queueMock;
	GlibMock          m_glibMock;
};

// /******************************************************************************************************
//  * plog_init                                                                                          *
//  *****************************************************************************************************/

// TEST_FF(PlogTest, plog_init_alreadyInit_success)
// {
// 	EXPECT_CALL(m_configurationMock, configuration_read())
// 		.WillOnce(testing::Return(TRUE));
// 	ASSERT_EQ(TRUE, plog_init("")) << "Failed to initialize Plog with default file name!";

// 	ASSERT_EQ(FALSE, plog_init(NULL)) << "Multiple initialization succeeded!";

// 	EXPECT_CALL(m_configurationMock, configuration_write);
// }

// TEST_FF(PlogTest, plog_init_fileOpen_fail)
// {
// 	ASSERT_EQ(FALSE, plog_init("read_only.txt")) << "Initialized Plog using read-only file!";
// }

// TEST_FF(PlogTest, plog_init_configurationRead_fail)
// {
// 	EXPECT_CALL(m_configurationMock, configuration_read())
// 		.WillOnce(testing::Return(FALSE));
// 	ASSERT_EQ(FALSE, plog_init(NULL)) << "Initialized Plog without reading configuration!";
// }

// /******************************************************************************************************
//  * plog_deinit                                                                                        *
//  *****************************************************************************************************/

// TEST_FF(PlogTest, plog_deinit_alreadyDeinit)
// {
// 	EXPECT_CALL(m_configurationMock, configuration_read())
// 		.WillOnce(testing::Return(TRUE));
// 	ASSERT_EQ(TRUE, plog_init(NULL)) << "Failed to initialize Plog with default file name!";

// 	EXPECT_CALL(m_configurationMock, configuration_write());
// 	plog_deinit();
// 	plog_info("Log after deinitialization!");
// }

// /******************************************************************************************************
//  * plog_set_buffer_size                                                                               *
//  *****************************************************************************************************/

// TEST_FF(PlogTest, plog_set_buffer_size_notInitialized_fail)
// {
// 	ASSERT_EQ(FALSE, plog_set_buffer_size(128ULL)) << "Succeeded to set buffer size to 128 without initialization!";
// 	ASSERT_EQ(0ULL, plog_get_buffer_size());
// }

// TEST_FF(PlogTest, plog_set_buffer_size_tryRealloc_fail)
// {
// 	EXPECT_CALL(m_configurationMock, configuration_read())
// 		.WillOnce(testing::Return(TRUE));
// 	ASSERT_EQ(TRUE, plog_init(NULL)) << "Failed to initialize Plog with default file name!";

// 	EXPECT_CALL(m_glibMock, g_try_realloc(testing::_, testing::_))
// 		.WillOnce(testing::Return((gpointer)NULL));
// 	ASSERT_EQ(FALSE, plog_set_buffer_size(128ULL)) << "Succeeded to set buffer size to 128 without reallocation!";

// 	ASSERT_EQ(0ULL, plog_get_buffer_size());
// 	EXPECT_CALL(m_configurationMock, configuration_write());
// }

// TEST_FF(PlogTest, plog_set_buffer_size_tryNewThread_fail)
// {
// 	gchar buffer[128] = "";

// 	EXPECT_CALL(m_configurationMock, configuration_read())
// 		.WillOnce(testing::Return(TRUE));
// 	ASSERT_EQ(TRUE, plog_init(NULL)) << "Failed to initialize Plog with default file name!";

// 	EXPECT_CALL(m_glibMock, g_try_realloc(testing::_, testing::_))
// 		.WillOnce(testing::Return((gpointer)buffer));
// 	EXPECT_CALL(m_queueMock, queue_init(testing::_));
// 	EXPECT_CALL(m_glibMock, g_thread_try_new(testing::_, testing::_, testing::_, testing::_))
// 		.WillOnce(testing::Return((GThread*)NULL));
// 	EXPECT_CALL(m_glibMock, g_free(testing::_));
// 	EXPECT_CALL(m_queueMock, queue_deinit(testing::_));
// 	ASSERT_EQ(FALSE, plog_set_buffer_size(128ULL)) << "Succeeded to set buffer size to 128 without creating worker thread!";

// 	ASSERT_EQ(0ULL, plog_get_buffer_size()) << "Buffer size has been set even though it failed!";
// 	EXPECT_CALL(m_configurationMock, configuration_write());
// }

// // TEST_FF(PlogTest, plog_set_buffer_size_success)
// // {
// // 	gchar     buffer[128] = "";
// // 	pthread_t thread      = 0UL;
// // 	gchar     test_log[]  = "Test log!";

// // 	EXPECT_CALL(m_configurationMock, configuration_read())
// // 		.WillOnce(testing::Return(TRUE));
// // 	ASSERT_EQ(TRUE, plog_init(NULL)) << "Failed to initialize Plog with default file name!";

// // 	EXPECT_CALL(m_glibMock, g_try_realloc(testing::_, testing::_))
// // 		.WillOnce(testing::Return((gpointer)buffer));
// // 	EXPECT_CALL(m_queueMock, queue_init(testing::_));
// // 	EXPECT_CALL(m_glibMock, g_thread_try_new(testing::_, testing::_, testing::_, testing::_))
// // 		.WillOnce(testing::Invoke([thread] (const gchar* name, GThreadFunc const func, gpointer const data, GError** const error)
// // 		{
// // 			const int32_t error_code = pthread_create((pthread_t*)&thread, NULL, func, data);
// // 			if (0L != error_code)
// // 			{
// // 				ADD_FAILURE() << "Failed to create work thread! (error message: " << strerror(error_code) << ")";
// // 			}
// // 			return (GThread*)NOT_NULL;
// // 		}));

// // 	ASSERT_EQ(TRUE, plog_set_buffer_size(128ULL)) << "Failed to set buffer size to 128!";
// // 	ASSERT_EQ(128ULL, plog_get_buffer_size()) << "Buffer size has not been set even though it succeeded!";

// // 	EXPECT_CALL(m_queueMock, queue_put(testing::_, testing::_, testing::_));
// // 	EXPECT_CALL(m_queueMock, queue_pop(testing::_, testing::_, testing::_))
// // 		.WillRepeatedly(testing::DoAll(testing::Invoke([test_log] (Queue_t* const queue, gchar** const buffer, guint8* const severity_bit)
// // 		{
// // 			*buffer       = (gchar*)test_log;
// // 			*severity_bit = 127U;

// // 			return TRUE;
// // 		})));
// // 	plog_info("File only log!");

// // 	plog_set_terminal_mode(TRUE);
// // 	ASSERT_EQ(TRUE, plog_get_terminal_mode()) << "Terminal mode has not been set!";

// // 	EXPECT_CALL(m_queueMock, queue_put(testing::_, testing::_, testing::_));
// // 	plog_info("Terminal log!");

// // 	EXPECT_CALL(m_glibMock, g_try_realloc(testing::_, testing::_))
// // 		.WillOnce(testing::Return((gpointer)NULL));
// // 	EXPECT_CALL(m_queueMock, queue_deinit(testing::_));
// // 	EXPECT_CALL(m_glibMock, g_thread_join(testing::_));
// // 	EXPECT_CALL(m_glibMock, g_thread_exit(testing::_));
// // 	ASSERT_EQ(TRUE, plog_set_buffer_size(0ULL)) << "Failed to set buffer size to 0!";
// // 	ASSERT_EQ(0ULL, plog_get_buffer_size()) << "Buffer size has not been set even though it succeeded!";

// // 	EXPECT_CALL(m_configurationMock, configuration_write());
// // }

// // TODO: severity level
// // TODO: terminal mode and colors
// // TODO: valgrind