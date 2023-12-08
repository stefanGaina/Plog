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
 * @file plog.c                                                                                       *
 * @date:      @author:                   Reason for change:                                          *
 * 22.06.2023  Gaina Stefan               Initial version.                                            *
 * 22.06.2023  Gaina Stefan               Added plog_get_version.                                     *
 * 29.06.2023  Gaina Stefan               Moved plog_get_version to plog_version.c.                   *
 * 10.09.2023  Gaina Stefan               Added terminal mode.                                        *
 * 13.09.2023  Gaina Stefan               Added color for Windows.                                    *
 * 13.09.2023  Gaina Stefan               Added color for Linux.                                      *
 * 16.09.2023  Gaina Stefan               Added mutex for Windows.                                    *
 * 08.12.2023  Gaina Stefan               Made Glib refactor.                                         *
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
 * @brief Buffer in which the logs are composed.
*/
static gchar* buffer = NULL;

/**
 * @brief The size of the buffer.
*/
static gsize buffer_size = 0ULL;

/**
 * @brief Queue in which the logs are being stored and consumed asynchronically.
*/
static Queue_t queue = {};

/******************************************************************************************************
 * LOCAL FUNCTIONS                                                                                    *
 *****************************************************************************************************/

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
	if (NULL != file)
	{
		return FALSE;
	}

	if (NULL == file_name || 0L == g_strcmp0("", file_name))
	{
		file_name = PLOG_DEFAULT_FILE_NAME;
	}

	file = fopen(file_name, "w");
	if (NULL == file)
	{
		(void)g_fprintf(stdout, "Failed to open \"%s\" in write mode!\n", file_name);
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

	plog_info(LOG_PREFIX "Plog has initialized successfully!");
	return TRUE;
}

void plog_deinit(void)
{
	if (NULL == file)
	{
		return;
	}

	configuration_write();
	(void)fclose(file);
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
		return FALSE;
	}

	g_mutex_lock(&lock);

	auxiliary_buffer = (gchar*)g_try_realloc((gpointer)buffer, new_buffer_size);
	if (NULL == auxiliary_buffer && 0ULL != new_buffer_size)
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
			buffer_size = 0ULL;
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
	gsize result = 0ULL;

	g_mutex_lock(&lock);
	result = buffer_size;
	g_mutex_unlock(&lock);

	return result;
}

void plog_internal(const guint8 severity_bit, const gchar* const severity_tag, const gchar* const function_name, const gchar* const format, ...)
{
	va_list argument_list      = {};
	va_list argument_list_copy = {};
	gint32  bytes_copied       = 0L;
	gint32  bytes_copied2      = 0L;
	gchar*  buffer_copy        = NULL;

	if (severity_bit != (severity_bit & plog_get_severity_level()) || NULL == file)
	{
		return;
	}

	va_start(argument_list, format);
	g_mutex_lock(&lock);

	if (TRUE == is_working)
	{
		bytes_copied = g_sprintf(buffer, "[%s] [%s] [%s] ", get_time(), function_name, severity_tag);
		if (0L <= bytes_copied)
		{
			bytes_copied2 = g_vsprintf((gchar*)(buffer + bytes_copied), format, argument_list);
			if (0L <= bytes_copied2)
			{
				buffer_copy = (gchar*)g_try_malloc((gsize)bytes_copied + (gsize)bytes_copied2 + 1ULL);
				if (NULL != buffer_copy)
				{
					(void)g_strlcpy(buffer_copy, buffer, (gsize)bytes_copied + (gsize)bytes_copied2 + 1ULL);
					if (TRUE == queue_put(&queue, buffer_copy, severity_bit))
					{
						g_mutex_unlock(&lock);
						va_end(argument_list);

						return;
					}

					g_free(buffer_copy);
					buffer_copy = NULL;
				}
			}
		}
	}

	if (TRUE == plog_get_terminal_mode())
	{
		set_color(severity_bit);
		(void)g_fprintf(stdout, "[%s] [%s] [%s] ", get_time(), function_name, severity_tag);

		va_copy(argument_list_copy, argument_list);
		(void)g_vfprintf(stdout, format, argument_list_copy);
		va_end(argument_list_copy);

		restore_color();
		(void)g_fprintf(stdout, "\n");
	}

	(void)g_fprintf(file, "[%s] [%s] [%s] ", get_time(), function_name, severity_tag);
	(void)g_vfprintf(file, format, argument_list);
	(void)g_fprintf(file, "\n");

	g_mutex_unlock(&lock);
	va_end(argument_list);
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

	(void)g_fprintf(file, "%s\n", buffer_copy);

	if (TRUE == plog_get_terminal_mode())
	{
		set_color(severity_bit);
		(void)g_fprintf(stdout, "%s", buffer_copy);

		restore_color();
		(void)g_fprintf(stdout, "\n");
	}

	g_free(buffer_copy);
	buffer_copy = NULL;
}

static const char* get_time(void)
{
	const time_t now         = time(NULL);
	char* const  time_string = ctime(&now);

	time_string[strlen(time_string) - 1ULL] = '\0'; /*< Remove the '\n'.                 */
	return (const char*)(time_string + 4);          /*< Remove the day of the week part. */
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
