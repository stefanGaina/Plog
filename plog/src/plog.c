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
 * @file plog.c                                                                                       *
 * @date:      @author:                   Reason for change:                                          *
 * 22.06.2023  Gaina Stefan               Initial version.                                            *
 * 22.06.2023  Gaina Stefan               Added plog_get_version.                                     *
 * 29.06.2023  Gaina Stefan               Moved plog_get_version to plog_version.c.                   *
 * 10.09.2023  Gaina Stefan               Added terminal mode.                                        *
 * 13.09.2023  Gaina Stefan               Added color for Windows.                                    *
 * 13.09.2023  Gaina Stefan               Added color for Linux.                                      *
 * 16.09.2023  Gaina Stefan               Added mutex for Windows.                                    *
 * 15.12.2023  Gaina Stefan               Made Glib refactor.                                         *
 * 20.12.2023  Gaina Stefan               Updated copyright.                                          *
 * @details This file implements the interface defined in plog.h.                                     *
 * @todo N/A.                                                                                         *
 * @bug No known bugs.                                                                                *
 *****************************************************************************************************/

/******************************************************************************************************
 * HEADER FILE INCLUDES                                                                               *
 *****************************************************************************************************/

#include <stdlib.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <string.h>
#include <time.h>
#include <glib/gprintf.h>

#include "plog.h"
#include "internal/configuration.h"
#include "internal/queue.h"
#include "internal/common.h"

/******************************************************************************************************
 * LOCAL VARIABLES                                                                                    *
 *****************************************************************************************************/

/**
 * @brief The file where the logs will be written.
*/
static FILE* file = NULL;

/**
 * @brief A copy to the file name that has extra space for suffix (e.g. ".254").
*/
static gchar* file_name_buffer = NULL;

/**
 * @brief The current severity level bitmask.
*/
static atomic_uchar severity_level = 127U;

/**
 * @brief Flag indicating if the logs have to be printed in the terminal as well.
*/
static atomic_bool is_terminal_enabled = FALSE;

/**
 * @brief The thread on which the logging will be done in case te buffering option is selected.
*/
static GThread* thread = NULL;

/**
 * @brief Flag indicating if the thread is currently running.
*/
static atomic_bool is_working = FALSE;

/**
 * @brief Lock protecting the data from multiple thread access.
*/
static GMutex lock = {};

/**
 * @brief The maximum size of the log file before rotating to another file.
*/
static atomic_ullong file_size = 0UL;

/**
 * @brief The maximum additional created log files.
*/
static atomic_uchar file_count = 0U;

/**
 * @brief The size of the currently opened file.
*/
static atomic_ullong current_file_size = 0UL;

/**
 * @brief The count of the currently opened file.
*/
static atomic_uchar current_file_count = 0U;

/**
 * @brief Buffer in which the logs are composed.
*/
static gchar* buffer = NULL;

/**
 * @brief The size of the buffer.
*/
static gsize buffer_size = 0UL;

/**
 * @brief Queue in which the logs are being stored and consumed asynchronically.
*/
static Queue_t queue = {};

/******************************************************************************************************
 * LOCAL FUNCTIONS                                                                                    *
 *****************************************************************************************************/

/**
 * @brief Checks if file size has been achieved and opens another file if it is the case.
 * @param void
 * @return void
*/
static void check_file_size(void);

/**
 * @brief Function consuming the logs from the queue. This is being run asynchronically.
 * @param data: User data (NULL).
 * @return NULL
*/
static gpointer work_function(gpointer data);

/**
 * @brief Prints the last log from the queue or waits until one is available.
 * @param void
 * @return void
*/
static void print_from_queue(void);

/**
 * @brief This function is not meant to be called outside plog macros.
 * @param void
 * @return A string representing the current time in a "mmm dd hh:mm:ss yyyy" format.
*/
static const gchar* get_time(void);

/**
 * @brief Sets the color of the text printed in the terminal.
 * @param severity_bit: Bit indicating the severity of the log message.
 * @return void
*/
static void set_color(guint8 severity_bit);

/**
 * @brief Restores the color of the text printed in the terminal to default (white).
 * @param void
 * @return void
*/
static void restore_color(void);

/******************************************************************************************************
 * FUNCTION DEFINITIONS                                                                               *
 *****************************************************************************************************/

gboolean plog_init(const gchar* file_name)
{
	gsize file_name_size = 0UL;

	if (NULL != file)
	{
		plog_error(LOG_PREFIX "Plog is already initialized!");
		return FALSE;
	}

	if (NULL == file_name || 0 == g_strcmp0("", file_name))
	{
		file_name = PLOG_DEFAULT_FILE_NAME;
	}

	file = fopen(file_name, "w");
	if (NULL == file)
	{
		(void)g_fprintf(stdout, LOG_PREFIX "Failed to open \"%s\" in write mode!\n", file_name);
		return FALSE;
	}

	g_mutex_init(&lock);

	if (FALSE == configuration_read())
	{
		g_mutex_clear(&lock);
		(void)fclose(file);
		file = NULL;

		return FALSE;
	}

	file_name_size   = strlen(file_name) + 1UL;
	file_name_buffer = (gchar*)g_try_malloc(file_name_size + 4UL * sizeof(gchar));
	if (NULL == file_name_buffer)
	{
		plog_error(LOG_PREFIX "Failed to copy the file name!");
		g_mutex_clear(&lock);
		(void)fclose(file);
		file = NULL;

		return FALSE;
	}
	(void)g_strlcpy(file_name_buffer, file_name, file_name_size);

	plog_info(LOG_PREFIX "Plog has initialized successfully!");
	return TRUE;
}

void plog_deinit(void)
{
	if (NULL == file)
	{
		plog_error(LOG_PREFIX "Plog is already deinitialized!");
		return;
	}

	configuration_write();

	g_mutex_lock(&lock);
	(void)fclose(file);
	file = NULL;

	g_free(file_name_buffer);
	file_name_buffer = NULL;

	current_file_size = 0UL;
	g_mutex_unlock(&lock);

	g_mutex_clear(&lock);
}

void plog_set_severity_level(const guint8 severity_level_mask)
{
	severity_level = (atomic_uchar)severity_level_mask;
}

guint8 plog_get_severity_level(void)
{
	return (guint8)severity_level;
}

void plog_set_file_size(const gsize new_file_size)
{
	file_size = (atomic_ullong)new_file_size;
}

gsize plog_get_file_size(void)
{
	return (gsize)file_size;
}

void plog_set_file_count(const guint8 new_file_count)
{
	file_count = (atomic_uchar)new_file_count;
}

guint8 plog_get_file_count(void)
{
	return (guint8)file_count;
}

void plog_set_terminal_mode(const gboolean terminal_mode)
{
	is_terminal_enabled = (atomic_bool)terminal_mode;
}

gboolean plog_get_terminal_mode(void)
{
	return (gboolean)is_terminal_enabled;
}

gboolean plog_set_buffer_size(const gsize new_buffer_size)
{
	gchar* auxiliary_buffer = NULL;

	if (NULL == file)
	{
		plog_error(LOG_PREFIX "Plog is not initialized!");
		return FALSE;
	}

	g_mutex_lock(&lock);

	auxiliary_buffer = (gchar*)g_try_realloc((gpointer)buffer, new_buffer_size);
	if (NULL == auxiliary_buffer && 0UL != new_buffer_size)
	{
		g_mutex_unlock(&lock);
		return FALSE;
	}

	buffer      = auxiliary_buffer;
	buffer_size = new_buffer_size;

	if (NULL == auxiliary_buffer && TRUE == is_working)
	{
		is_working = FALSE;
		queue_interrupt_wait(&queue);

		(void)g_thread_join(thread);
		thread = NULL;

		while (FALSE == queue_is_empty(&queue))
		{
			print_from_queue();
		}
		queue_deinit(&queue);
	}
	else if (NULL != auxiliary_buffer && FALSE == is_working)
	{
		queue_init(&queue);
		is_working = TRUE;

		thread = g_thread_try_new("worker_thread", work_function, NULL, NULL);
		if (NULL == thread)
		{
			g_free(buffer);
			buffer = NULL;

			queue_deinit(&queue);
			buffer_size = 0UL;
			is_working  = FALSE;

			g_mutex_unlock(&lock);
			return FALSE;
		}
	}

	g_mutex_unlock(&lock);
	return TRUE;
}

gsize plog_get_buffer_size(void)
{
	gsize result = 0UL;

	g_mutex_lock(&lock);
	result = buffer_size;
	g_mutex_unlock(&lock);

	return result;
}

void plog_internal(const guint8 severity_bit, const gchar* const severity_tag, const gchar* const function_name, const gchar* const format, ...)
{
	va_list argument_list      = {};
	va_list argument_list_copy = {};
	gint32  bytes_copied       = 0;
	gint32  bytes_copied2      = 0;
	gchar*  buffer_copy        = NULL;

	if (severity_bit != (severity_bit & severity_level) || NULL == file) /*< Left unsafe on purpose. */
	{
		return;
	}

	va_start(argument_list, format);
	g_mutex_lock(&lock);

	if (TRUE == is_working)
	{
		bytes_copied = g_snprintf(buffer, buffer_size, "[%s] [%s] [%s] ", get_time(), function_name, severity_tag);
		if (0 <= bytes_copied)
		{
			if (buffer_size > bytes_copied + 1)
			{
				bytes_copied2 = g_vsnprintf((gchar*)(buffer + bytes_copied), buffer_size - bytes_copied, format, argument_list);
			}

			buffer_copy = (gchar*)g_try_malloc((gsize)bytes_copied + (gsize)bytes_copied2 + 1UL);
			if (NULL != buffer_copy)
			{
				(void)g_strlcpy(buffer_copy, buffer, (gsize)bytes_copied + (gsize)bytes_copied2 + 1UL);
				if (FALSE == queue_put(&queue, buffer_copy, severity_bit))
				{
					g_free(buffer_copy);
					buffer_copy = NULL;
				}
			}
		}

		g_mutex_unlock(&lock);
		va_end(argument_list);
		return;
	}

	if (TRUE == is_terminal_enabled)
	{
		set_color(severity_bit);
		(void)g_fprintf(stdout, "[%s] [%s] [%s] ", get_time(), function_name, severity_tag);

		va_copy(argument_list_copy, argument_list);
		(void)g_vfprintf(stdout, format, argument_list_copy);
		va_end(argument_list_copy);

		restore_color();
		(void)g_fprintf(stdout, "\n");
	}

	current_file_size += g_fprintf(file, "[%s] [%s] [%s] ", get_time(), function_name, severity_tag);
	current_file_size += g_vfprintf(file, format, argument_list);
	current_file_size += g_fprintf(file, "\n");
	check_file_size();

	g_mutex_unlock(&lock);
	va_end(argument_list);
}

static void check_file_size(void)
{
	gsize  file_name_size  = 0UL;
	FILE*  auxiliary_file  = NULL;
	guint8 file_count_copy = 0U;

	if (0UL == file_size || current_file_size < file_size)
	{
		return;
	}

	file_name_size = strlen(file_name_buffer);
	if (file_count > current_file_count)
	{
		file_name_buffer[file_name_size] = '.';

		if (10U > current_file_count)
		{
			file_name_buffer[file_name_size + 1UL] = '0' + current_file_count % 10U;
			file_name_buffer[file_name_size + 2UL] = '\0';
		}
		else if (10U <= current_file_count && 100U > current_file_count)
		{
			file_name_buffer[file_name_size + 1UL] = '0' + current_file_count / 10U;
			file_name_buffer[file_name_size + 2UL] = '0' + current_file_count % 10U;
			file_name_buffer[file_name_size + 3UL] = '\0';
		}
		else
		{
			file_name_buffer[file_name_size + 1UL] = '0' + current_file_count / 100U;
			file_name_buffer[file_name_size + 2UL] = '0' + (current_file_count / 10U) % 10U;
			file_name_buffer[file_name_size + 3UL] = '0' + current_file_count % 10U;
			file_name_buffer[file_name_size + 4UL] = '\0';
		}
	}

	file_count_copy = file_count;
	if (0U == file_count_copy)
	{
		(void)fclose(file);
	}

	auxiliary_file = fopen(file_name_buffer, "w");
	if (NULL != auxiliary_file)
	{
		if (0U != file_count_copy)
		{
			(void)fclose(file);
		}
		file = auxiliary_file;

		if (file_count < ++current_file_count)
		{
			current_file_count = 0U;
		}
	}
	else
	{
		plog_error(LOG_PREFIX "Failed to open a new log file in write mode!");
	}

	current_file_size                       = 0UL;
	file_name_buffer[file_name_size]        = '\0';
	file_name_buffer[file_name_size + 1ULL] = '\0';
	file_name_buffer[file_name_size + 2ULL] = '\0';
	file_name_buffer[file_name_size + 3ULL] = '\0';
}

static gpointer work_function(gpointer const data)
{
	while (TRUE == is_working)
	{
		print_from_queue();
	}

	g_thread_exit(NULL);
	return NULL; /*< To avoid warning. */
}

static void print_from_queue(void)
{
	gchar* buffer_copy  = NULL;
	guint8 severity_bit = 0U;

	if (FALSE == queue_pop(&queue, &buffer_copy, &severity_bit))
	{
		return;
	}

	if (TRUE == is_terminal_enabled)
	{
		set_color(severity_bit);
		(void)g_fprintf(stdout, "%s", buffer_copy);

		restore_color();
		(void)g_fprintf(stdout, "\n");
	}

	/* Left unsafe on purpose. */
	current_file_size += g_fprintf(file, "%s\n", buffer_copy);
	check_file_size();

	g_free(buffer_copy);
	buffer_copy = NULL;
}

static const gchar* get_time(void)
{
	const time_t now         = time(NULL);
	gchar* const time_string = ctime(&now);

	time_string[strlen(time_string) - 1UL] = '\0'; /*< Remove the '\n'.                 */
	return (const gchar*)(time_string + 4);        /*< Remove the day of the week part. */
}

static void set_color(const guint8 severity_bit)
{
	switch (severity_bit)
	{
		case E_PLOG_SEVERITY_LEVEL_FATAL:
		{
			(void)g_fprintf(stdout, "\033[1;31m");
			break;
		}
		case E_PLOG_SEVERITY_LEVEL_ERROR:
		{
			(void)g_fprintf(stdout, "\033[0;91m");
			break;
		}
		case E_PLOG_SEVERITY_LEVEL_WARN:
		{
			(void)g_fprintf(stdout, "\033[0;93m");
			break;
		}
		case E_PLOG_SEVERITY_LEVEL_INFO:
		{
			(void)g_fprintf(stdout, "\033[1;32m");
			break;
		}
		case E_PLOG_SEVERITY_LEVEL_DEBUG:
		{
			(void)g_fprintf(stdout, "\033[1;36m");
			break;
		}
		case E_PLOG_SEVERITY_LEVEL_TRACE:
		{
			restore_color();
			break;
		}
		case E_PLOG_SEVERITY_LEVEL_VERBOSE:
		{
			(void)g_fprintf(stdout, "\033[0;90m");
			break;
		}
		default:
		{
			break;
		}
	}
}

static void restore_color(void)
{
	(void)g_fprintf(stdout, "\033[1;0m");
}
