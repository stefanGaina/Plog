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
 * @file configuration.c
 * @author Gaina Stefan
 * @date 15.12.2023
 * @brief This file implements the interface defined in configuration.h.
 * @todo N/A.
 * @bug No known bugs.
 *****************************************************************************************************/

/******************************************************************************************************
 * HEADER FILE INCLUDES
 *****************************************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <glib/gprintf.h>

#include "plog.h"
#include "internal/configuration.h"
#include "internal/common.h"
#include "internal/vector.h"

/******************************************************************************************************
 * MACROS
 *****************************************************************************************************/

/** ***************************************************************************************************
 * @brief The string indicating the log level value is following.
 *****************************************************************************************************/
#define LOG_LEVEL_STRING "LOG_LEVEL = "

/** ***************************************************************************************************
 * @brief The length of the log level string.
 *****************************************************************************************************/
#define LOG_LEVEL_STRING_SIZE 12UL

/** ***************************************************************************************************
 * @brief The string indicating the log file size value is following.
 *****************************************************************************************************/
#define LOG_FILE_SIZE_STRING "LOG_FILE_SIZE = "

/** ***************************************************************************************************
 * @brief The length of the log file size string.
 *****************************************************************************************************/
#define LOG_FILE_SIZE_STRING_SIZE 16UL

/** ***************************************************************************************************
 * @brief The string indicating the log file count value is following.
 *****************************************************************************************************/
#define LOG_FILE_COUNT_STRING "LOG_FILE_COUNT = "

/** ***************************************************************************************************
 * @brief The length of the log file count string.
 *****************************************************************************************************/
#define LOG_FILE_COUNT_STRING_SIZE 17UL

/** ***************************************************************************************************
 * @brief The string indicating the terminal mode value is following.
 *****************************************************************************************************/
#define TERMINAL_MODE_STRING "TERMINAL_MODE = "

/** ***************************************************************************************************
 * @brief The length of the terminal mode string.
 *****************************************************************************************************/
#define TERMINAL_MODE_STRING_SIZE 16UL

/** ***************************************************************************************************
 * @brief The string indicating the buffer size value is following.
 *****************************************************************************************************/
#define BUFFER_MODE_STRING "BUFFER_MODE = "

/** ***************************************************************************************************
 * @brief The length of the buffer size string.
 *****************************************************************************************************/
#define BUFFER_MODE_STRING_SIZE 14UL

/******************************************************************************************************
 * LOCAL FUNCTIONS
 *****************************************************************************************************/

/** ***************************************************************************************************
 * @brief Closes the configuration file and prints a warning message in case of failure.
 * @param file: The opened file.
 * @return void
 *****************************************************************************************************/
static void close_configuration_file(FILE* file);

/** ***************************************************************************************************
 * @brief Transforms an integer into string format.
 * @param buffer: Where the string will be stored.
 * @param integer: The integer to be transformed.
 * @return The length of the number.
 *****************************************************************************************************/
static gsize integer_to_string(gchar* buffer, guint64 integer);

/******************************************************************************************************
 * FUNCTION DEFINITIONS
 *****************************************************************************************************/

gboolean configuration_read(void)
{
	static const char* const DEFAULT_CONFIGURATION =
		"# Configuration file for Plog that is being read at initialization and is being written at deinitialization with runtime updates.\n\n"

		"# Bitmask enabling/disabling logs.\n"
		"# 2^0 - fatal | 2^1 - error | 2^2 - warn | 2^3 - info | 2^4 - debug | 2^5 - trace | 2^6 - verbose.\n"
		"" LOG_LEVEL_STRING "127\n\n"

		"# Maximum size of a log file (in bytes) before creating another (or overwriting in case file count is 0).\n"
		"" LOG_FILE_SIZE_STRING "0\n\n"

		"# The count of the additional log files created (does not have any effect if file size is 0).\n"
		"" LOG_FILE_COUNT_STRING "0\n\n"

		"# 1 - logs will also be printed in terminal | 0 - logs will only be printed in the file.\n"
		"" TERMINAL_MODE_STRING "0\n\n"

		"# 1 - logs will be printed asynchronically | 0 - caller thread will be blocked until logs are printed.\n"
		"" BUFFER_MODE_STRING "0\n";

	FILE*	file		= NULL;
	gchar	buffer[256] = "";
	guint64 auxiliary	= 0UL;

	file = fopen(PLOG_CONFIGURATION_FILE_NAME, "r");
	if (NULL == file)
	{
		plog_warn(LOG_PREFIX "Failed to open \"" PLOG_CONFIGURATION_FILE_NAME "\" in read mode! (error message: %s)", strerror(errno));

		file = fopen(PLOG_CONFIGURATION_FILE_NAME, "w");
		if (NULL == file)
		{
			plog_error(LOG_PREFIX "Failed to open \"" PLOG_CONFIGURATION_FILE_NAME "\" in write mode! (error message: %s)", strerror(errno));
			return FALSE;
		}

		(void)g_fprintf(file, "%s", DEFAULT_CONFIGURATION);
		plog_set_severity_level(127U);
		plog_set_file_size(0UL);
		plog_set_file_count(0U);
		plog_set_terminal_mode(FALSE);
		(void)plog_set_buffer_mode(FALSE);

		goto CLOSE_FILE;
	}

	while (NULL != fgets(buffer, sizeof(buffer), file))
	{
		if ('#' == buffer[0] || '\n' == buffer[0])
		{
			continue;
		}

		if (0 == g_ascii_strncasecmp(buffer, LOG_LEVEL_STRING, LOG_LEVEL_STRING_SIZE))
		{
			errno	  = 0;
			auxiliary = g_ascii_strtoull(buffer + LOG_LEVEL_STRING_SIZE, NULL, 0U);
			if (0 != errno)
			{
				plog_error(LOG_PREFIX "Invalid severity level! (text: %s)", buffer + LOG_LEVEL_STRING_SIZE);
				continue;
			}

			plog_set_severity_level((guint8)auxiliary);
			plog_info(LOG_PREFIX "Severity level has been set successfully! (value: %" G_GUINT16_FORMAT ")", (guint8)auxiliary);
			continue;
		}

		if (0 == g_ascii_strncasecmp(buffer, LOG_FILE_SIZE_STRING, LOG_FILE_SIZE_STRING_SIZE))
		{
			errno	  = 0;
			auxiliary = g_ascii_strtoull(buffer + LOG_FILE_SIZE_STRING_SIZE, NULL, 0U);
			if (0 != errno)
			{
				plog_error(LOG_PREFIX "Invalid log file size! (text: %s)", buffer + LOG_FILE_SIZE_STRING_SIZE);
				continue;
			}

			plog_set_file_size((gsize)auxiliary);
			plog_info(LOG_PREFIX "Log file size has been set successfully! (value: %" G_GSIZE_FORMAT ")", (gsize)auxiliary);
			continue;
		}

		if (0 == g_ascii_strncasecmp(buffer, LOG_FILE_COUNT_STRING, LOG_FILE_COUNT_STRING_SIZE))
		{
			errno	  = 0;
			auxiliary = g_ascii_strtoull(buffer + LOG_FILE_COUNT_STRING_SIZE, NULL, 0U);
			if (0 != errno)
			{
				plog_error(LOG_PREFIX "Invalid log file count! (text: %s)", buffer + LOG_FILE_COUNT_STRING_SIZE);
				continue;
			}

			plog_set_file_count((guint8)auxiliary);
			plog_info(LOG_PREFIX "Log file count has been set successfully! (value: %" G_GSIZE_FORMAT ")", (gsize)auxiliary);
			continue;
		}

		if (0 == g_ascii_strncasecmp(buffer, TERMINAL_MODE_STRING, TERMINAL_MODE_STRING_SIZE))
		{
			errno	  = 0;
			auxiliary = g_ascii_strtoull(buffer + TERMINAL_MODE_STRING_SIZE, NULL, 0U);
			if (0 != errno)
			{
				plog_error(LOG_PREFIX "Invalid terminal mode! (text: %s)", buffer + TERMINAL_MODE_STRING_SIZE);
				continue;
			}

			plog_set_terminal_mode((gboolean)auxiliary);
			plog_info(LOG_PREFIX "Terminal mode has been set successfully! (value: %s)", TRUE == (gboolean)auxiliary ? "TRUE" : "FALSE");
			continue;
		}

		if (0 == g_ascii_strncasecmp(buffer, BUFFER_MODE_STRING, BUFFER_MODE_STRING_SIZE))
		{
			errno	  = 0;
			auxiliary = g_ascii_strtoull(buffer + BUFFER_MODE_STRING_SIZE, NULL, 0U);
			if (0 != errno)
			{
				plog_error(LOG_PREFIX "Invalid buffer size! (text: %s)", buffer + BUFFER_MODE_STRING_SIZE);
				continue;
			}

			if (FALSE == plog_set_buffer_mode(((gboolean)auxiliary)))
			{
				plog_error(LOG_PREFIX "Failed to set buffer size! (value: %s)", TRUE == (gboolean)auxiliary ? "TRUE" : "FALSE");
				continue;
			}

			plog_info(LOG_PREFIX "Buffer size has been set successfully! (value: %" G_GSIZE_FORMAT ")", (gsize)auxiliary);
			continue;
		}

		plog_warn(LOG_PREFIX "Invalid configuration line: %s (error message: %s)", buffer, strerror(errno));
	}

CLOSE_FILE:
	close_configuration_file(file);
	return TRUE;
}

void configuration_write(void)
{
	FILE*	 file				= NULL;
	gchar	 buffer[256]		= "";
	Vector_t vector				= {};
	gboolean is_read_successful = TRUE;
	gsize	 offset				= 0UL;

	vector_init(&vector);

	file = fopen(PLOG_CONFIGURATION_FILE_NAME, "r");
	if (NULL == file)
	{
		plog_warn(LOG_PREFIX "Failed to open \"" PLOG_CONFIGURATION_FILE_NAME "\" in read mode! (error message: %s)", strerror(errno));
		goto RESET_CONFIGURATION;
	}

	while (NULL != fgets(buffer, sizeof(buffer), file))
	{
		if (FALSE == vector_push(&vector, buffer))
		{
			plog_warn(LOG_PREFIX "Failed to read \"" PLOG_CONFIGURATION_FILE_NAME "\"!");
			is_read_successful = FALSE;

			break;
		}
	}
	close_configuration_file(file);

	if (FALSE == is_read_successful)
	{
		goto RESET_CONFIGURATION;
	}

	file = fopen(PLOG_CONFIGURATION_FILE_NAME, "w");
	if (NULL == file)
	{
		plog_warn(LOG_PREFIX "Failed to open \"" PLOG_CONFIGURATION_FILE_NAME "\" in write mode! (error message: %s)", strerror(errno));
		goto RESET_CONFIGURATION;
	}

	while (FALSE == vector_is_empty(&vector))
	{
		vector_pop(&vector, buffer, sizeof(buffer));

		if (0 == g_ascii_strncasecmp(buffer, LOG_LEVEL_STRING, LOG_LEVEL_STRING_SIZE))
		{
			offset = integer_to_string(buffer + LOG_LEVEL_STRING_SIZE, (guint64)plog_get_severity_level());

			buffer[offset + LOG_LEVEL_STRING_SIZE]		 = '\n';
			buffer[offset + LOG_LEVEL_STRING_SIZE + 1UL] = '\0';
		}
		else if (0 == g_ascii_strncasecmp(buffer, LOG_FILE_SIZE_STRING, LOG_FILE_SIZE_STRING_SIZE))
		{
			offset = integer_to_string(buffer + LOG_FILE_SIZE_STRING_SIZE, (guint64)plog_get_file_size());

			buffer[offset + LOG_FILE_SIZE_STRING_SIZE]		 = '\n';
			buffer[offset + LOG_FILE_SIZE_STRING_SIZE + 1UL] = '\0';
		}
		else if (0 == g_ascii_strncasecmp(buffer, LOG_FILE_COUNT_STRING, LOG_FILE_COUNT_STRING_SIZE))
		{
			offset = integer_to_string(buffer + LOG_FILE_COUNT_STRING_SIZE, (guint8)plog_get_file_count());

			buffer[offset + LOG_FILE_COUNT_STRING_SIZE]		  = '\n';
			buffer[offset + LOG_FILE_COUNT_STRING_SIZE + 1UL] = '\0';
		}
		else if (0 == g_ascii_strncasecmp(buffer, TERMINAL_MODE_STRING, TERMINAL_MODE_STRING_SIZE))
		{
			offset = integer_to_string(buffer + TERMINAL_MODE_STRING_SIZE, (guint64)plog_get_terminal_mode());

			buffer[offset + TERMINAL_MODE_STRING_SIZE]		 = '\n';
			buffer[offset + TERMINAL_MODE_STRING_SIZE + 1UL] = '\0';
		}
		else if (0 == g_ascii_strncasecmp(buffer, BUFFER_MODE_STRING, BUFFER_MODE_STRING_SIZE))
		{
			offset = integer_to_string(buffer + BUFFER_MODE_STRING_SIZE, (guint64)plog_get_buffer_mode());

			buffer[offset + BUFFER_MODE_STRING_SIZE]	   = '\n';
			buffer[offset + BUFFER_MODE_STRING_SIZE + 1UL] = '\0';
		}

		(void)g_fprintf(file, "%s", buffer);
	}

	close_configuration_file(file);

RESET_CONFIGURATION:
	vector_clean(&vector);

	if (FALSE == plog_set_buffer_mode(FALSE))
	{
		plog_error(LOG_PREFIX "Failed to disable buffer mode!");
	}

	plog_set_severity_level(0U);
	plog_set_file_size(0UL);
	plog_set_file_count(0U);
	plog_set_terminal_mode(FALSE);
}

static void close_configuration_file(FILE* const file)
{
	if (0 != fclose(file))
	{
		plog_warn(LOG_PREFIX "Failed to close \"" PLOG_CONFIGURATION_FILE_NAME "\"! (error message: %s)", strerror(errno));
	}
}

static gsize integer_to_string(gchar* const buffer, guint64 integer)
{
	gsize offset = 0UL;
	gsize index	 = 0UL;
	gchar digit	 = '\0';

	do
	{
		digit = (gchar)(integer % 10UL) + '0';
		integer /= 10UL;

		for (index = offset++; index > 0UL; --index)
		{
			buffer[index] = buffer[index - 1UL];
		}
		buffer[0] = digit;
	}
	while (0UL != integer);

	return offset;
}
