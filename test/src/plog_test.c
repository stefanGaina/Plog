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
 * @file plog_test.c
 * @author Gaina Stefan
 * @date 22.06.2023
 * @brief This file is implementing a program that loads Plog and tests it using API-Test.
 * @todo N/A.
 * @bug No known bugs.
 *****************************************************************************************************/

/******************************************************************************************************
 * HEADER FILE INCLUDES
 *****************************************************************************************************/

#include <apitest.h>

#include "plog.h"

/******************************************************************************************************
 * MACROS
 *****************************************************************************************************/

/** ***************************************************************************************************
 * @brief Checks if a bit is enabled.
 * @param bit: The bit to be checked.
 * @param bit_mask: The bitmask where the bit is checked.
 * @return "enabled" or "disabled" string depending of the values of the bits.
 *****************************************************************************************************/
#define IS_BIT_ENABLED_STRING(bit, bit_mask) bit == (bit & bit_mask) ? "enabled" : "disabled"

/******************************************************************************************************
 * LOCAL FUNCTIONS
 *****************************************************************************************************/

/** ***************************************************************************************************
 * @brief Handles the commands when they are available.
 * @param void
 * @return void
 *****************************************************************************************************/
static void test_main(void);

/** ***************************************************************************************************
 * @brief Prints the help menu to the user.
 * @param void
 * @return void
 *****************************************************************************************************/
static void print_help(void);

/** ***************************************************************************************************
 * @brief Function that will be called when plog_init() is requested by the user.
 * @param void
 * @return void
 *****************************************************************************************************/
static void plog_init_test(void);

/** ***************************************************************************************************
 * @brief Function that will be called when plog_deinit() is requested by the user.
 * @param void
 * @return void
 *****************************************************************************************************/
static void plog_deinit_test(void);

/** ***************************************************************************************************
 * @brief Function that will be called when plog_set_severity_level() is requested by the user.
 * @param void
 * @return void
 *****************************************************************************************************/
static void plog_set_severity_level_test(void);

/** ***************************************************************************************************
 * @brief Function that will be called when plog_get_severity_level() is requested by the user.
 * @param void
 * @return void
 *****************************************************************************************************/
static void plog_get_severity_level_test(void);

/** ***************************************************************************************************
 * @brief Function that will be called when plog_set_file_size() is requested by the user.
 * @param void
 * @return void
 *****************************************************************************************************/
static void plog_set_file_size_test(void);

/** ***************************************************************************************************
 * @brief Function that will be called when plog_get_file_size() is requested by the user.
 * @param void
 * @return void
 *****************************************************************************************************/
static void plog_get_file_size_test(void);

/** ***************************************************************************************************
 * @brief Function that will be called when plog_set_file_count() is requested by the user.
 * @param void
 * @return void
 *****************************************************************************************************/
static void plog_set_file_count_test(void);

/** ***************************************************************************************************
 * @brief Function that will be called when plog_get_file_count() is requested by the user.
 * @param void
 * @return void
 *****************************************************************************************************/
static void plog_get_file_count_test(void);

/** ***************************************************************************************************
 * @brief Function that will be called when plog_set_terminal_mode() is requested by the user.
 * @param void
 * @return void
 *****************************************************************************************************/
static void plog_set_terminal_mode_test(void);

/** ***************************************************************************************************
 * @brief Function that will be called when plog_get_terminal_mode() is requested by the user.
 * @param void
 * @return void
 *****************************************************************************************************/
static void plog_get_terminal_mode_test(void);

/** ***************************************************************************************************
 * @brief Function that will be called when plog_set_buffer_size() is requested by the user.
 * @param void
 * @return void
 *****************************************************************************************************/
static void plog_set_buffer_size_test(void);

/** ***************************************************************************************************
 * @brief Function that will be called when plog_get_buffer_size() is requested by the user.
 * @param void
 * @return void
 *****************************************************************************************************/
static void plog_get_buffer_size_test(void);

/** ***************************************************************************************************
 * @brief Function that will be called when plog_fatal() is requested by the user.
 * @param void
 * @return void
 *****************************************************************************************************/
static void plog_fatal_test(void);

/** ***************************************************************************************************
 * @brief Function that will be called when plog_error() is requested by the user.
 * @param void
 * @return void
 *****************************************************************************************************/
static void plog_error_test(void);

/** ***************************************************************************************************
 * @brief Function that will be called when plog_warn() is requested by the user.
 * @param void
 * @return void
 *****************************************************************************************************/
static void plog_warn_test(void);

/** ***************************************************************************************************
 * @brief Function that will be called when plog_info() is requested by the user.
 * @param void
 * @return void
 *****************************************************************************************************/
static void plog_info_test(void);

/** ***************************************************************************************************
 * @brief Function that will be called when plog_debug() is requested by the user.
 * @param void
 * @return void
 *****************************************************************************************************/
static void plog_debug_test(void);

/** ***************************************************************************************************
 * @brief Function that will be called when plog_trace() is requested by the user.
 * @param void
 * @return void
 *****************************************************************************************************/
static void plog_trace_test(void);

/** ***************************************************************************************************
 * @brief Function that will be called when plog_verbose() is requested by the user.
 * @param void
 * @return void
 *****************************************************************************************************/
static void plog_verbose_test(void);

/** ***************************************************************************************************
 * @brief Function that will be called when plog_assert() is requested by the user.
 * @param void
 * @return void
 *****************************************************************************************************/
static void plog_assert_test(void);

/******************************************************************************************************
 * FUNCTION DEFINITIONS
 *****************************************************************************************************/

APITEST_MAIN("plog_apitest> ", 1024UL, test_main, print_help)

static void test_main(void)
{
	APITEST_HANDLE_COMMAND(plog_init, 1U);
	APITEST_HANDLE_COMMAND(plog_deinit, 0U);
	APITEST_HANDLE_COMMAND(plog_set_severity_level, 1U);
	APITEST_HANDLE_COMMAND(plog_get_severity_level, 0U);
	APITEST_HANDLE_COMMAND(plog_set_file_size, 1U);
	APITEST_HANDLE_COMMAND(plog_get_file_size, 0U);
	APITEST_HANDLE_COMMAND(plog_set_file_count, 1U);
	APITEST_HANDLE_COMMAND(plog_get_file_count, 0U);
	APITEST_HANDLE_COMMAND(plog_set_terminal_mode, 1U);
	APITEST_HANDLE_COMMAND(plog_get_terminal_mode, 0U);
	APITEST_HANDLE_COMMAND(plog_set_buffer_size, 1U);
	APITEST_HANDLE_COMMAND(plog_get_buffer_size, 0U);
	APITEST_HANDLE_COMMAND(plog_fatal, 1U);
	APITEST_HANDLE_COMMAND(plog_error, 1U);
	APITEST_HANDLE_COMMAND(plog_warn, 1U);
	APITEST_HANDLE_COMMAND(plog_info, 1U);
	APITEST_HANDLE_COMMAND(plog_debug, 1U);
	APITEST_HANDLE_COMMAND(plog_trace, 1U);
	APITEST_HANDLE_COMMAND(plog_verbose, 1U);
	APITEST_HANDLE_COMMAND(plog_assert, 1U);
	(void)g_fprintf(stdout, "Invalid function name! Type \"h\" or \"help\" for a list of supported commands!\n");
}

static void print_help(void)
{
	(void)g_fprintf(stdout, "plog_init               <input_file>\n");
	(void)g_fprintf(stdout, "plog_deinit\n");
	(void)g_fprintf(stdout, "plog_set_severity_level <mask>\n");
	(void)g_fprintf(stdout, "plog_get_severity_level\n");
	(void)g_fprintf(stdout, "plog_set_file_size      <size>\n");
	(void)g_fprintf(stdout, "plog_get_file_size\n");
	(void)g_fprintf(stdout, "plog_set_file_count     <count>\n");
	(void)g_fprintf(stdout, "plog_get_file_count\n");
	(void)g_fprintf(stdout, "plog_set_terminal_mode  <mode>\n");
	(void)g_fprintf(stdout, "plog_get_terminal_mode\n");
	(void)g_fprintf(stdout, "plog_set_buffer_size    <size>\n");
	(void)g_fprintf(stdout, "plog_get_buffer_size\n");
	(void)g_fprintf(stdout, "plog_fatal              <text>\n");
	(void)g_fprintf(stdout, "plog_error              <text>\n");
	(void)g_fprintf(stdout, "plog_warn               <text>\n");
	(void)g_fprintf(stdout, "plog_info               <text>\n");
	(void)g_fprintf(stdout, "plog_debug              <text>\n");
	(void)g_fprintf(stdout, "plog_trace              <text>\n");
	(void)g_fprintf(stdout, "plog_verbose            <text>\n");
	(void)g_fprintf(stdout, "plog_assert             <condition>\n");
}

static void plog_init_test(void)
{
	const gchar* const input = 0 == strcmp("NULL", command.argv[1]) ? NULL : command.argv[1];

	if (TRUE == plog_init(input))
	{
		(void)g_fprintf(stdout, "Plog has been initialized successfully!\n");
		return;
	}
	(void)g_fprintf(stdout, "Failed to initialize Plog!\n");
}

static void plog_deinit_test(void)
{
	plog_deinit();
	(void)g_fprintf(stdout, "Plog has been deinitialized successfully!\n");
}

static void plog_set_severity_level_test(void)
{
	guint8 severity_level = 0U;

	APITEST_STRING_TO_UINT8(1, severity_level);

	plog_set_severity_level(severity_level);
	(void)g_fprintf(stdout, "Severity level has been set successfully!\n");
}

static void plog_get_severity_level_test(void)
{
	const guint8 severity_level = plog_get_severity_level();

	(void)g_fprintf(
		stdout,
		"Severity level bit mask: %" PRIu8 "!\n"
		"Fatal   logs are %s\n"
		"Error   logs are %s\n"
		"Warn    logs are %s\n"
		"Info    logs are %s\n"
		"Debug   logs are %s\n"
		"Trace   logs are %s\n"
		"Verbose logs are %s\n",
		severity_level, IS_BIT_ENABLED_STRING(E_PLOG_SEVERITY_LEVEL_FATAL, severity_level), IS_BIT_ENABLED_STRING(E_PLOG_SEVERITY_LEVEL_ERROR, severity_level),
		IS_BIT_ENABLED_STRING(E_PLOG_SEVERITY_LEVEL_WARN, severity_level), IS_BIT_ENABLED_STRING(E_PLOG_SEVERITY_LEVEL_INFO, severity_level),
		IS_BIT_ENABLED_STRING(E_PLOG_SEVERITY_LEVEL_DEBUG, severity_level), IS_BIT_ENABLED_STRING(E_PLOG_SEVERITY_LEVEL_TRACE, severity_level),
		IS_BIT_ENABLED_STRING(E_PLOG_SEVERITY_LEVEL_VERBOSE, severity_level));
}

static void plog_set_file_size_test(void)
{
	gsize file_size = 0UL;

	APITEST_STRING_TO_UINT64(1, file_size);

	plog_set_file_size(file_size);
	(void)g_fprintf(stdout, "File size has been set successfully!\n");
}

static void plog_get_file_size_test(void)
{
	const gsize file_size = plog_get_file_size();

	(void)g_fprintf(stdout,
					"File size has been got successfully!\n"
					"File size: %" G_GSIZE_FORMAT "\n",
					file_size);
}

static void plog_set_file_count_test(void)
{
	guint8 file_count = 0U;

	APITEST_STRING_TO_UINT64(1, file_count);

	plog_set_file_count(file_count);
	(void)g_fprintf(stdout, "File count has been set successfully!\n");
}

static void plog_get_file_count_test(void)
{
	const guint8 file_count = plog_get_file_count();

	(void)g_fprintf(stdout,
					"File count has been got successfully!\n"
					"File count: %" PRIu8 "\n",
					file_count);
}

static void plog_set_terminal_mode_test(void)
{
	gboolean terminal_mode = 0U;

	APITEST_STRING_TO_INT32(1, terminal_mode);

	plog_set_terminal_mode(terminal_mode);
	(void)g_fprintf(stdout, "Terminal mode has been set successfully!\n");
}

static void plog_get_terminal_mode_test(void)
{
	const gboolean terminal_mode = plog_get_terminal_mode();

	(void)g_fprintf(stdout,
					"Terminal mode has been got successfully!\n"
					"Terminal mode is %s\n",
					FALSE == terminal_mode ? "disabled" : "enabled");
}

static void plog_set_buffer_size_test(void)
{
	gsize buffer_size = 0UL;

	APITEST_STRING_TO_UINT64(1, buffer_size);

	if (TRUE == plog_set_buffer_size(buffer_size))
	{
		(void)g_fprintf(stdout, "Buffer size has been set successfully!\n");
		return;
	}
	(void)g_fprintf(stdout, "Failed to set buffer size!\n");
}

static void plog_get_buffer_size_test(void)
{
	const gsize buffer_size = plog_get_buffer_size();

	(void)g_fprintf(stdout,
					"Buffer size has been got successfully!\n"
					"Buffer size: %" G_GSIZE_FORMAT "\n",
					buffer_size);
}

static void plog_fatal_test(void)
{
	plog_fatal("%s", command.argv[1]);
	(void)g_fprintf(stdout, "Fatal message has been successfully logged!\n");
}

static void plog_error_test(void)
{
	plog_error("%s", command.argv[1]);
	(void)g_fprintf(stdout, "Error message has been successfully logged!\n");
}

static void plog_warn_test(void)
{
	plog_warn("%s", command.argv[1]);
	(void)g_fprintf(stdout, "Warn message has been successfully logged!\n");
}

static void plog_info_test(void)
{
	plog_info("%s", command.argv[1]);
	(void)g_fprintf(stdout, "Info message has been successfully logged!\n");
}

static void plog_debug_test(void)
{
	plog_debug("%s", command.argv[1]);
	(void)g_fprintf(stdout, "Debug message has been successfully logged!\n");
}

static void plog_trace_test(void)
{
	plog_trace("%s", command.argv[1]);
	(void)g_fprintf(stdout, "Trace message has been successfully logged!\n");
}

static void plog_verbose_test(void)
{
	plog_verbose("%s", command.argv[1]);
	(void)g_fprintf(stdout, "Verbose message has been successfully logged!\n");
}

static void plog_assert_test(void)
{
	gboolean condition = FALSE;

	APITEST_STRING_TO_INT32(1, condition);

	plog_assert(TRUE == condition);
	(void)g_fprintf(stdout, "Assertion has passed successfully!\n");
}
