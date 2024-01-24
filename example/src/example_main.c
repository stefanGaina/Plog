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
 * @file example_main.c                                                                               *
 * @date:      @author:                   Reason for change:                                          *
 * 15.12.2023  Gaina Stefan               Initial version.                                            *
 * 20.12.2023  Gaina Stefan               Updated copyright.                                          *
 * 24.01.2024  Gaina Stefan               Added an assert.                                            *
 * @details This file implements a program that showcases how the API can be called and a short       *
 * performance test to enfesize the benefits and drawbacks of the features.                           *
 * @todo N/A.                                                                                         *
 * @bug No known bugs.                                                                                *
 *****************************************************************************************************/

/******************************************************************************************************
 * HEADER FILE INCLUDES                                                                               *
 *****************************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <plog.h>

/******************************************************************************************************
 * MACROS                                                                                             *
 *****************************************************************************************************/

/**
 * @brief How many times the logging macros will be invoked.
*/
#define PERFORMANE_TEST_COUNT 100000UL

/******************************************************************************************************
 * ENTRY POINT                                                                                        *
 *****************************************************************************************************/

int main(int argc, char* argv[])
{
#ifdef PLOG_STRIP_ALL
	(void)fprintf(stdout, "Plog has been stripped from compilation!\n");
	plog_fatal("Function no longer has log function calls!");
#else
	const plog_Version_t version        = plog_get_version();
	const gchar*         log_file_name  = NULL;
	guint8               severity_level = 0U;
	gsize                file_size      = 0UL;
	guint8               file_count     = 0U;
	gboolean             terminal_mode  = FALSE;
	gsize                buffer_size    = 0UL;
	gsize                index          = 0UL;
	struct timespec      start_time     = {};
	struct timespec      end_time       = {};

	if (PLOG_VERSION_MAJOR != version.major
	 || PLOG_VERSION_MINOR != version.minor
	 || PLOG_VERSION_PATCH != version.patch)
	{
		(void)fprintf(stdout, "Plog version mismatch!\n");
		return EXIT_FAILURE;
	}

	if (1 == argc)
	{
		(void)fprintf(stdout, "Logs will be stored in \"%s\"!\n", PLOG_DEFAULT_FILE_NAME);
	}
	else if (2 <= argc)
	{
		log_file_name = argv[1];
		(void)fprintf(stdout, "Logs will be stored in \"%s\"!\n", log_file_name);

		if (3 <= argc)
		{
			(void)fprintf(stdout, "Extra parameters will be ignored!");
		}
	}

	plog_warn("Logs before initialization will have no effect!");
	plog_set_severity_level(E_PLOG_SEVERITY_LEVEL_DEBUG | E_PLOG_SEVERITY_LEVEL_TRACE);
	plog_set_file_size(512UL * 1024UL);
	plog_set_file_count(10U);
	plog_set_terminal_mode(TRUE);
	plog_set_buffer_size(1024UL);
	plog_warn("As well as any other function!");

	if (FALSE == plog_init(log_file_name))
	{
		(void)fprintf(stdout, "Failed to initialize plog!\n");
		return EXIT_FAILURE;
	}
	plog_info("Logs/functions after initialization will have an effect!");

	plog_assert(1 == 1);

	severity_level = plog_get_severity_level();
	plog_set_severity_level(E_PLOG_SEVERITY_LEVEL_FATAL | E_PLOG_SEVERITY_LEVEL_ERROR);
	plog_error("Logs after severity level was set only to fatal and error!");
	plog_verbose("Logs after severity level was set only to fatal and error!");
	plog_set_severity_level(severity_level);

	file_size = plog_get_file_size();
	plog_set_file_size(512UL * 1024UL);
	plog_set_file_size(file_size);

	file_count = plog_get_file_count();
	plog_set_file_count(1U);
	plog_set_file_count(file_count);

	terminal_mode = plog_get_terminal_mode();
	plog_set_terminal_mode(!terminal_mode);
	plog_info("Logs after the terminal mode has been switched!");
	plog_set_terminal_mode(terminal_mode);

	buffer_size = plog_get_buffer_size();
	if (FALSE == plog_set_buffer_size(0UL == buffer_size ? 512UL : 0UL))
	{
		plog_error("Failed to set buffer size!");
	}
	(void)plog_set_buffer_size(buffer_size);

	clock_gettime(CLOCK_MONOTONIC, &start_time);
	for (index = 1UL; index <= PERFORMANE_TEST_COUNT; ++index)
	{
		plog_fatal("Performance test! (%"   G_GSIZE_FORMAT " / %" G_GSIZE_FORMAT ")", index, PERFORMANE_TEST_COUNT);
		plog_error("Performance test! (%"   G_GSIZE_FORMAT " / %" G_GSIZE_FORMAT ")", index, PERFORMANE_TEST_COUNT);
		plog_info("Performance test! (%"    G_GSIZE_FORMAT " / %" G_GSIZE_FORMAT ")", index, PERFORMANE_TEST_COUNT);
		plog_warn("Performance test! (%"    G_GSIZE_FORMAT " / %" G_GSIZE_FORMAT ")", index, PERFORMANE_TEST_COUNT);
		plog_debug("Performance test! (%"   G_GSIZE_FORMAT " / %" G_GSIZE_FORMAT ")", index, PERFORMANE_TEST_COUNT);
		plog_trace("Performance test! (%"   G_GSIZE_FORMAT " / %" G_GSIZE_FORMAT ")", index, PERFORMANE_TEST_COUNT);
		plog_verbose("Performance test! (%" G_GSIZE_FORMAT " / %" G_GSIZE_FORMAT ")", index, PERFORMANE_TEST_COUNT);
	}
	clock_gettime(CLOCK_MONOTONIC, &end_time);

	plog_warn("Main thread has been unblocked after %" G_GUINT64_FORMAT " milliseconds!",
		(((guint64)end_time.tv_sec * 1000UL) + ((guint64)end_time.tv_nsec / 1000000UL))
		- (((guint64)start_time.tv_sec * 1000UL) + ((guint64)start_time.tv_nsec / 1000000UL)));

	plog_deinit();
	clock_gettime(CLOCK_MONOTONIC, &end_time);

	(void)fprintf(stdout, "Printing the logs finished after: %" G_GUINT64_FORMAT " milliseconds!\n",
		(((guint64)end_time.tv_sec * 1000UL) + ((guint64)end_time.tv_nsec / 1000000UL))
		- (((guint64)start_time.tv_sec * 1000UL) + ((guint64)start_time.tv_nsec / 1000000UL)));

	plog_debug("Logs/functions after deinitialization will have no effect!");
	plog_trace("The terminal mode and buffer size have been changed!");
	plog_verbose("These changes will take place in the next lifecycle! (check plog.conf)");
#endif

	return EXIT_SUCCESS;
}
