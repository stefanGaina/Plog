/******************************************************************************************************
 * @file plog.c                                                                                       *
 * @date:      @author:                   Reason for change:                                          *
 * 22.06.2023  Gaina Stefan               Initial version.                                            *
 * 22.06.2023  Gaina Stefan               Added plog_get_version.                                     *
 * 29.06.2023  Gaina Stefan               Moved plog_get_version to plog_version.c.                   *
 * 10.09.2023  Gaina Stefan               Added terminal mode.                                        *
 * 13.09.2023  Gaina Stefan               Added color for Windows.                                    *
 * 13.09.2023  Gaina Stefan               Added color for Linux.                                      *
 * 16.09.2023  Gaina Stefan               Added mutex for Windows.                                    *
 * @details This file implements the interface defined in plog.h.                                     *
 * @todo N/A.                                                                                         *
 * @bug No known bugs.                                                                                *
 *****************************************************************************************************/

/******************************************************************************************************
 * HEADER FILE INCLUDES                                                                               *
 *****************************************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#else
#error "Platform not supported!"
#endif

#include "plog.h"

/******************************************************************************************************
 * MACROS                                                                                             *
 *****************************************************************************************************/

/**
 * @brief The tag attached at the beginning of the messages logged by Plog library.
*/
#define LOG_PREFIX "[PLOG] "

/******************************************************************************************************
 * LOCAL VARIABLES                                                                                    *
 *****************************************************************************************************/

/**
 * @brief The file where the logs will be written.
*/
static FILE* file = NULL;

/**
 * @brief The current severity level bitmask.
*/
static uint8_t severity_level = 127U;

/**
 * @brief Flag indicating if the logs have to be printed in the terminal as well.
*/
static bool is_terminal_enabled = false;

#ifdef _WIN32
/**
 * @brief Mutex object for Windows.
*/
HANDLE mutex = NULL;
#endif /*< _WIN32 */

/******************************************************************************************************
 * FUNCTION DEFINITIONS                                                                               *
 *****************************************************************************************************/

void plog_init(const char* file_name, const bool terminal_mode)
{
	FILE*   level_file = NULL;
	char    buffer[]   = "LOG_LEVEL = 127";
	int32_t error_code = 0L;

	if (NULL == file_name || 0L == strcmp("", file_name))
	{
		file_name = PLOG_DEFAULT_FILE_NAME;
	}

	file = fopen(file_name, "w");
	if (NULL == file)
	{
		(void)fprintf(stdout, "Failed to open \"%s\" in write mode!", file_name);
		return;
	}

	level_file = fopen(PLOG_LEVEL_FILE_NAME, "r");
	if (NULL == level_file)
	{
		plog_warn(LOG_PREFIX "Failed to open \"" PLOG_LEVEL_FILE_NAME "\" in read mode!");

		level_file = fopen(PLOG_LEVEL_FILE_NAME, "w");
		if (0L != error_code)
		{
			plog_warn(LOG_PREFIX "Failed to open \"" PLOG_LEVEL_FILE_NAME "\" in write mode!");
			return;
		}
		(void)fprintf(level_file, "%s", buffer);
	}
	else
	{
		(void)fgets(buffer, sizeof(buffer), level_file);
		buffer[15] = '\0';

		severity_level = (uint8_t)strtol(buffer + strlen("LOG_LEVEL = "), NULL, 0L);
		plog_info(LOG_PREFIX "Severity level from \"" PLOG_LEVEL_FILE_NAME "\" is: %" PRIu8 "", severity_level);
	}
	plog_set_terminal_mode(terminal_mode);

#ifdef _WIN32
	mutex = CreateMutex(NULL, FALSE, NULL);
#endif /*< _WIN32 */

	error_code = fclose(level_file);
	if (0L != error_code)
	{
		plog_warn(LOG_PREFIX "Failed to close \"%s\"! (error code: %" PRId32 ")", file_name, error_code);
		return;
	}
	plog_info(LOG_PREFIX "Plog has initialized successfully!");
}

void plog_deinit(void)
{
	int32_t error_code = 0L;

	plog_set_severity_level(0U);
	plog_set_terminal_mode(false);

#ifdef _WIN32
	(void)CloseHandle(mutex);
#endif /*< _WIN32 */

	if (NULL != file)
	{
		error_code = fclose(file);
		if (0L != error_code)
		{
			plog_warn(LOG_PREFIX "Failed to close the logging file! (error code: %" PRId32 ")", error_code);
			return;
		}
		file = NULL;
	}
}

void plog_set_severity_level(const uint8_t severity_level_mask)
{
	severity_level = severity_level_mask;
}

uint8_t plog_get_severity_level(void)
{
	return severity_level;
}

void plog_set_terminal_mode(const bool terminal_mode)
{
	is_terminal_enabled = terminal_mode;
}

bool plog_get_terminal_mode(void)
{
	return is_terminal_enabled;
}

FILE* plog_internal_get_file(void)
{
	return file;
}

uint8_t plog_internal_get_severity_level(void)
{
	return severity_level;
}

const char* plog_internal_get_time(void)
{
	const time_t now         = time(NULL);
	char* const  time_string = ctime(&now);

	time_string[strlen(time_string) - 1ULL] = '\0'; /*< Remove the '\n'.                 */
	return (const char*)(time_string + 4);          /*< Remove the day of the week part. */
}

void plog_internal_set_color(const uint8_t severity_bit)
{
	switch (severity_bit)
	{
		case E_PLOG_SEVERITY_LEVEL_FATAL:
		{
#ifdef _WIN32
			(void)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
#elif __linux__
			(void)fprintf(stdout, "\033[1;31m");
#endif /*< _WIN32 */
			break;
		}
		case E_PLOG_SEVERITY_LEVEL_ERROR:
		{
#ifdef _WIN32
			(void)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
#elif __linux__
			(void)fprintf(stdout, "\033[0;91m");
#endif /*< _WIN32 */
			break;
		}
		case E_PLOG_SEVERITY_LEVEL_WARN:
		{
#ifdef _WIN32
			(void)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#elif __linux__
			(void)fprintf(stdout, "\033[0;93m");
#endif /*< _WIN32 */
			break;
		}
		case E_PLOG_SEVERITY_LEVEL_INFO:
		{
#ifdef _WIN32
			(void)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
#elif __linux__
			(void)fprintf(stdout, "\033[1;32m");
#endif /*< _WIN32 */
			break;
		}
		case E_PLOG_SEVERITY_LEVEL_DEBUG:
		{
#ifdef _WIN32
			(void)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#elif __linux__
			(void)fprintf(stdout, "\033[1;36m");
#endif /*< _WIN32 */
			break;
		}
		case E_PLOG_SEVERITY_LEVEL_TRACE:
		{
			plog_internal_restore_color();
			break;
		}
		case E_PLOG_SEVERITY_LEVEL_VERBOSE:
		{
#ifdef _WIN32
			(void)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
#elif __linux__
			(void)fprintf(stdout, "\033[0;90m");
#endif /*< _WIN32 */
			break;
		}
		default:
		{
			break;
		}
	}
}

void plog_internal_restore_color(void)
{
#ifdef _WIN32
	(void)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#elif __linux__
	(void)fprintf(stdout, "\033[1;0m");
#endif /*< _WIN32 */
}

void plog_internal_mutex_lock(void)
{
#ifdef _WIN32
	(void)WaitForSingleObject(mutex, INFINITE);
#elif __linux__
	// TODO
#endif /*< _WIN32 */
}

void plog_internal_mutex_unlock(void)
{
#ifdef _WIN32
	(void)ReleaseMutex(mutex);
#elif __linux__
	// TODO
#endif /*< _WIN32 */
}
