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
 * @file configuration_test.c
 * @author Gaina Stefan
 * @date 15.12.2023
 * @brief This file unit-tests configuration.c.
 * @details Current coverage report:
 * Line coverage: 99.3%  (142/143)
 * Functions:     100.0% (4/4)
 * Branches:      98.5%  (65/66)
 * @todo Find a way to make fclose() fail to achieve 100.0% coverage (not a priority).
 * @bug configuration_read_fileNotFound_success needs to be executed first for plog.conf file to be
 * created. This breaks the principle that tests should not depend on other tests but in this case is
 * accepted.
 *****************************************************************************************************/

/******************************************************************************************************
 * HEADER FILE INCLUDES
 *****************************************************************************************************/

#include <fcntl.h>
#include <gtest/gtest.h>

#include "plog_mock.hpp"
#include "vector_mock.hpp"
#include "glib_mock.hpp"
#include "internal/configuration.h"

/******************************************************************************************************
 * TEST CLASS
 *****************************************************************************************************/

class ConfigurationTest : public testing::Test
{
public:
	ConfigurationTest(void)
		: plogMock{}
		, vectorMock{}
		, glibMock{}
	{
	}

	~ConfigurationTest(void) = default;

protected:
	void SetUp(void) override
	{
	}

	void TearDown(void) override
	{
	}

public:
	PlogMock   plogMock;
	VectorMock vectorMock;
	GlibMock   glibMock;
};

/******************************************************************************************************
 * configuration_read
 *****************************************************************************************************/

TEST_F(ConfigurationTest, configuration_read_fileNotFound_success)
{
	EXPECT_CALL(plogMock, plog_set_severity_level(127U));
	EXPECT_CALL(plogMock, plog_set_file_size(0UL));
	EXPECT_CALL(plogMock, plog_set_file_count(0U));
	EXPECT_CALL(plogMock, plog_set_terminal_mode(FALSE));
	EXPECT_CALL(plogMock, plog_set_buffer_mode(FALSE));
	EXPECT_EQ(TRUE, configuration_read());
}

TEST_F(ConfigurationTest, configuration_read_fail)
{
	struct stat previous_stat	= {};
	int32_t		file_descriptor = 0;

	file_descriptor = open(PLOG_CONFIGURATION_FILE_NAME, O_RDONLY);
	if (-1 == file_descriptor)
	{
		ADD_FAILURE() << "Failed to open " << PLOG_CONFIGURATION_FILE_NAME << " in read-only mode! (error message: " << strerror(errno) << ")";
	}

	if (0 != stat(PLOG_CONFIGURATION_FILE_NAME, &previous_stat))
	{
		ADD_FAILURE() << "Failed to get information about " << PLOG_CONFIGURATION_FILE_NAME << "! (error message: " << strerror(errno) << ")";
	}

	if (0 != fchmod(file_descriptor, S_IXUSR))
	{
		ADD_FAILURE() << "Failed to change permissions of " << PLOG_CONFIGURATION_FILE_NAME << "! (error message: " << strerror(errno) << ")";
	}

	EXPECT_EQ(FALSE, configuration_read());

	if (0 != fchmod(file_descriptor, previous_stat.st_mode))
	{
		ADD_FAILURE() << "Failed to restore permissions of " << PLOG_CONFIGURATION_FILE_NAME << "! (error message: " << strerror(errno) << ")";
	}

	if (0 != close(file_descriptor))
	{
		ADD_FAILURE() << "Failed to close " << PLOG_CONFIGURATION_FILE_NAME << "! (error message: " << strerror(errno) << ")";
	}
}

TEST_F(ConfigurationTest, configuration_read_success)
{
	FILE* const file = fopen(PLOG_CONFIGURATION_FILE_NAME, "w");

	if (NULL == file)
	{
		ADD_FAILURE() << "Failed to open " << PLOG_CONFIGURATION_FILE_NAME << " in write mode! (error message: " << strerror(errno) << ")";
	}

	(void)fprintf(
		file,
		"INVALID_LINE\n"
		"# Configuration file for Plog that is being read at initialization and is being written at deinitialization with runtime updates.\n\n"

		"# Bitmask enabling/disabling logs.\n"
		"# 2^0 - fatal | 2^1 - error | 2^2 - warn | 2^3 - info | 2^4 - debug | 2^5 - trace | 2^6 - verbose.\n"
		"LOG_LEVEL = 18446744073709551616\n"
		"LOG_LEVEL = 127\n\n"

		"# Maximum size of a log file (in bytes) before creating another (or overwriting in case file count is 0).\n"
		"LOG_FILE_SIZE = 18446744073709551616\n"
		"LOG_FILE_SIZE = 0\n\n"

		"# The count of the additional log files created (does not have any effect if file size is 0).\n"
		"LOG_FILE_COUNT = 18446744073709551616\n"
		"LOG_FILE_COUNT = 0\n\n"

		"# 1 - logs will also be printed in terminal | 0 - logs will only be printed in the file.\n"
		"TERMINAL_MODE = 18446744073709551616\n"
		"TERMINAL_MODE = 1\n"
		"TERMINAL_MODE = 0\n\n"

		"# Size of the buffer of each log, 0 - asynchronically logging is disabled.\n"
		"BUFFER_MODE = 18446744073709551616\n"
		"BUFFER_MODE = 0\n"
		"BUFFER_MODE = 1\n"
		"BUFFER_MODE = 0\n");

	if (0 != fclose(file))
	{
		ADD_FAILURE() << "Failed to close file! (error message: " << strerror(errno) << ")";
	}

	EXPECT_CALL(plogMock, plog_set_severity_level(testing::_));
	EXPECT_CALL(plogMock, plog_set_file_size(testing::_));
	EXPECT_CALL(plogMock, plog_set_file_count(testing::_));
	EXPECT_CALL(plogMock, plog_set_terminal_mode(testing::_)) /**/
		.Times(2);
	EXPECT_CALL(plogMock, plog_set_buffer_mode(testing::_)) /**/
		.WillOnce(testing::Return(FALSE))
		.WillOnce(testing::Return(FALSE))
		.WillOnce(testing::Return(TRUE));
	EXPECT_EQ(TRUE, configuration_read());
}

/******************************************************************************************************
 * configuration_write
 *****************************************************************************************************/

TEST_F(ConfigurationTest, configuration_write_fileNotReadable_fail)
{
	struct stat previous_stat	= {};
	int32_t		file_descriptor = 0;

	file_descriptor = open(PLOG_CONFIGURATION_FILE_NAME, O_RDONLY);
	if (-1 == file_descriptor)
	{
		ADD_FAILURE() << "Failed to open " << PLOG_CONFIGURATION_FILE_NAME << " in read-only mode! (error message: " << strerror(errno) << ")";
	}

	if (0 != stat(PLOG_CONFIGURATION_FILE_NAME, &previous_stat))
	{
		ADD_FAILURE() << "Failed to get information about " << PLOG_CONFIGURATION_FILE_NAME << "! (error message: " << strerror(errno) << ")";
	}

	if (0 != fchmod(file_descriptor, S_IXUSR))
	{
		ADD_FAILURE() << "Failed to change permissions of " << PLOG_CONFIGURATION_FILE_NAME << "! (error message: " << strerror(errno) << ")";
	}

	EXPECT_CALL(vectorMock, vector_init(testing::_));
	EXPECT_CALL(plogMock, plog_set_buffer_mode(FALSE)) /**/
		.WillOnce(testing::Return(FALSE));
	EXPECT_CALL(plogMock, plog_set_severity_level(0U));
	EXPECT_CALL(plogMock, plog_set_file_size(0UL));
	EXPECT_CALL(plogMock, plog_set_file_count(0U));
	EXPECT_CALL(plogMock, plog_set_terminal_mode(FALSE));
	configuration_write();

	if (0 != fchmod(file_descriptor, previous_stat.st_mode))
	{
		ADD_FAILURE() << "Failed to restore permissions of " << PLOG_CONFIGURATION_FILE_NAME << "! (error message: " << strerror(errno) << ")";
	}

	if (0 != close(file_descriptor))
	{
		ADD_FAILURE() << "Failed to close " << PLOG_CONFIGURATION_FILE_NAME << "! (error message: " << strerror(errno) << ")";
	}
}

TEST_F(ConfigurationTest, configuration_write_vectorPush_fail)
{
	EXPECT_CALL(vectorMock, vector_init(testing::_));
	EXPECT_CALL(vectorMock, vector_push(testing::_, testing::_)) /**/
		.WillOnce(testing::Return(FALSE));
	EXPECT_CALL(vectorMock, vector_clean(testing::_));
	EXPECT_CALL(plogMock, plog_set_buffer_mode(FALSE)) /**/
		.WillOnce(testing::Return(TRUE));
	EXPECT_CALL(plogMock, plog_set_severity_level(0U));
	EXPECT_CALL(plogMock, plog_set_file_size(0UL));
	EXPECT_CALL(plogMock, plog_set_file_count(0U));
	EXPECT_CALL(plogMock, plog_set_terminal_mode(FALSE));
	configuration_write();
}

TEST_F(ConfigurationTest, configuration_write_fileNotWriteable_fail)
{
	struct stat previous_stat	= {};
	int32_t		file_descriptor = 0;

	file_descriptor = open(PLOG_CONFIGURATION_FILE_NAME, O_RDONLY);
	if (-1 == file_descriptor)
	{
		ADD_FAILURE() << "Failed to open " << PLOG_CONFIGURATION_FILE_NAME << " in read-only mode! (error message: " << strerror(errno) << ")";
	}

	if (0 != stat(PLOG_CONFIGURATION_FILE_NAME, &previous_stat))
	{
		ADD_FAILURE() << "Failed to get information about " << PLOG_CONFIGURATION_FILE_NAME << "! (error message: " << strerror(errno) << ")";
	}

	if (0 != fchmod(file_descriptor, S_IRUSR))
	{
		ADD_FAILURE() << "Failed to change permissions of " << PLOG_CONFIGURATION_FILE_NAME << "! (error message: " << strerror(errno) << ")";
	}

	EXPECT_CALL(vectorMock, vector_init(testing::_));
	EXPECT_CALL(vectorMock, vector_push(testing::_, testing::_)) /**/
		.WillRepeatedly(testing::Return(TRUE));
	EXPECT_CALL(vectorMock, vector_clean(testing::_));
	EXPECT_CALL(plogMock, plog_set_buffer_mode(FALSE)) /**/
		.WillOnce(testing::Return(TRUE));
	EXPECT_CALL(plogMock, plog_set_severity_level(0U));
	EXPECT_CALL(plogMock, plog_set_file_size(0UL));
	EXPECT_CALL(plogMock, plog_set_file_count(0U));
	EXPECT_CALL(plogMock, plog_set_terminal_mode(FALSE));
	configuration_write();

	if (0 != fchmod(file_descriptor, previous_stat.st_mode))
	{
		ADD_FAILURE() << "Failed to restore permissions of " << PLOG_CONFIGURATION_FILE_NAME << "! (error message: " << strerror(errno) << ")";
	}

	if (0 != close(file_descriptor))
	{
		ADD_FAILURE() << "Failed to close " << PLOG_CONFIGURATION_FILE_NAME << "! (error message: " << strerror(errno) << ")";
	}
}

TEST_F(ConfigurationTest, configuration_write_vectorPush_success)
{
	std::vector<std::string> vector = {};

	vector.push_back("BUFFER_MODE = 1\n");
	vector.push_back("TERMINAL_MODE = 1\n\n");
	vector.push_back("LOG_FILE_COUNT = 2\n\n");
	vector.push_back("LOG_FILE_SIZE = 20480\n\n");
	vector.push_back("LOG_LEVEL = 127\n\n");
	vector.push_back("# Configuration file for Plog that is being read at initialization and is being written at deinitialization with runtime updates.\n\n");

	EXPECT_CALL(vectorMock, vector_init(testing::_));
	EXPECT_CALL(vectorMock, vector_push(testing::_, testing::_)) /**/
		.WillRepeatedly(testing::Return(TRUE));
	ON_CALL(vectorMock, vector_is_empty(testing::_))
		.WillByDefault(testing::Invoke([&vector](const Vector_t* const public_vector) -> gboolean { return true == vector.empty() ? TRUE : FALSE; }));
	EXPECT_CALL(vectorMock, vector_is_empty(testing::_)) /**/
		.Times(7);
	EXPECT_CALL(vectorMock, vector_pop(testing::_, testing::_, testing::_))
		.WillRepeatedly(testing::Invoke(
			[&vector](Vector_t* const public_vector, gchar* const buffer, const gsize buffer_size) -> void
			{
				(void)strncpy(buffer, vector.back().c_str(), buffer_size);
				vector.pop_back();
			}));
	EXPECT_CALL(plogMock, plog_get_severity_level()) /**/
		.WillOnce(testing::Return((guint8)127U));
	EXPECT_CALL(plogMock, plog_get_file_size()) /**/
		.WillOnce(testing::Return((gsize)20480UL));
	EXPECT_CALL(plogMock, plog_get_file_count()) /**/
		.WillOnce(testing::Return((guint8)2U));
	EXPECT_CALL(plogMock, plog_get_terminal_mode()) /**/
		.WillOnce(testing::Return(TRUE));
	EXPECT_CALL(plogMock, plog_get_buffer_mode()) /**/
		.WillOnce(testing::Return(TRUE));
	EXPECT_CALL(vectorMock, vector_clean(testing::_));
	EXPECT_CALL(plogMock, plog_set_buffer_mode(FALSE)) /**/
		.WillOnce(testing::Return(TRUE));
	EXPECT_CALL(plogMock, plog_set_severity_level(0U));
	EXPECT_CALL(plogMock, plog_set_file_size(0UL));
	EXPECT_CALL(plogMock, plog_set_file_count(0U));
	EXPECT_CALL(plogMock, plog_set_terminal_mode(FALSE));
	configuration_write();
}
