/******************************************************************************************************
 * @file plog.c                                                                                       *
 * @date:      @author:                   Reason for change:                                          *
 * 22.06.2023  Gaina Stefan               Initial version.                                            *
 * 22.06.2023  Gaina Stefan               Add plog_get_version.                                       *
 * 29.06.2023  Gaina Stefan               Moved plog_get_version to plog_version.c.                   *
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

/******************************************************************************************************
 * FUNCTION DEFINITIONS                                                                               *
 *****************************************************************************************************/

void plog_init(const char* file_name)
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

void plog_set_severity_level(uint8_t severity_level_mask)
{
	severity_level = severity_level_mask;
}

uint8_t plog_get_severity_level(void)
{
	return severity_level;
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
    time_t now         = time(NULL);
    char*  time_string = ctime(&now);

    time_string[strlen(time_string) - 1ULL] = '\0'; /*< Remove the '\n'.                 */
	return (const char*)(time_string + 4);          /*< Remove the day of the week part. */
}
