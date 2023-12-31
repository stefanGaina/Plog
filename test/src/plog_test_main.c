/******************************************************************************************************
 * @file plog_test.c                                                                                  *
 * @date:      @author:                   Reason for change:                                          *
 * 22.06.2023  Gaina Stefan               Initial version.                                            *
 * 22.06.2023  Gaina Stefan               Add check for Plog version.                                 *
 * 29.06.2023  Gaina Stefan               Update check for Plog version with patch.                   *
 * 08.08.2023  Gaina Stefan               Updated apitest.                                            *
 * 10.09.2023  Gaina Stefan               Added terminal mode.                                        *
 * @details This file is implementing a program that loads Plog and tests it using API-Test.          *
 * @todo N/A.                                                                                         *
 * @bug No known bugs.                                                                                *
 *****************************************************************************************************/

/******************************************************************************************************
 * HEADER FILE INCLUDES                                                                               *
 *****************************************************************************************************/

#include <stdlib.h>
#include <apitest.h>
#include <apitest_version.h>

#include "plog.h"

// #define CHECK_PARAMETERS_COUNT(minimum_parameter_count, input_parameter_count)


// /******************************************************************************************************
//  * LOCAL FUNCTIONS                                                                                    *
//  *****************************************************************************************************/

// /**
//  * @brief Handles the commands and calls the functions from Plog's API.
//  * @param input_file: The file where the command will be inputed from.
//  * @return void
// */
// static void handle_commands(FILE* input_file);

// /**
//  * @brief Prints the version of API-Test in use.
//  * @param void
//  * @return void
// */
// static void print_versions(void);

// /**
//  * @brief Prints the usage of the application to the user.
//  * @param void
//  * @return void
// */
// static void print_usage(void);

// /**
//  * @brief Prints the help menu to the user.
//  * @param void
//  * @return void
// */
// static void print_help(void);

typedef struct s_Node_t Node_t;
/**
 * @brief Data that will be stored in the queue.
*/
struct s_Node_t
{
	gchar*  buffer;       /**< Stored log buffer.                       */
	guint8  severity_bit; /**< Stored severity bit.                     */
	Node_t* next;         /**< Reference to the next node in the queue. */
};

/**
 * @brief Explicit data type of the queue for internal usage.
*/
typedef struct s_PrivateQueue_t
{
	Node_t* head;      /**< The oldest node.                            */
	Node_t* tail;      /**< The latest node.                            */
	GMutex  lock;      /**< Lock for thread-safe access.                */
	GCond   condition; /**< Condition signaled when queue is not empty. */
}
PrivateQueue_t;

/******************************************************************************************************
 * FUNCTION DEFINITIONS                                                                               *
 *****************************************************************************************************/

int main(int argc, char* argv[])
{
	//FILE* input_file = NULL;

	//print_versions();

	plog_init(NULL);
	printf("%u\n", plog_get_severity_level());
	plog_info("TRALALALA");

	while (1);

	plog_deinit();
	// printf("%ld", sizeof(PrivateQueue_t));

	// if (1L >= argc)
	// {
	// 	(void)fprintf(stdout, "Running in terminal mode!\n");
	// 	print_usage();
	// }
	// else if (2L <= argc)
	// {
	// 	input_file = fopen(argv[1], "r");
	// 	if (NULL == input_file)
	// 	{
	// 		(void)fprintf(stdout, "Failed to open \"%s\"!\n", argv[1]);
	// 		(void)fprintf(stdout, "Running in terminal mode!\n\n");
	// 	}
	// }

	// if (3L <= argc)
	// {
	// 	(void)fprintf(stdout, "Extra parameters will be ignored!\n");
	// 	print_usage();
	// }

	// handle_commands(input_file);

	// (void)fprintf(stdout, "Process exited successfully!\n");
	return EXIT_SUCCESS;
}

// static void handle_commands(FILE* input_file)
// {
// 	apitest_Command_t command                  = {};
// 	apitest_Error_t   error                    = E_APITEST_ERROR_NONE;
// 	int64_t           terminal_mode_parameter  = 0LL;
// 	bool              terminal_mode_return     = false;
// 	int64_t           severity_level_parameter = 0LL;
// 	uint8_t           severity_level_return    = 0U;

// 	while (true)
// 	{
// 		command = apitest_get_command("plog-test> ", input_file);

// 		if (0L >= command.argc)
// 		{
// 			(void)fprintf(stdout, "Switching to terminal mode! (EOF reached)\n\n");
// 			input_file = NULL;
// 			continue;
// 		}

// 		if (0 == apitest_string_compare("quit", command.argv[0])
// 		 || 0 == apitest_string_compare("q"   , command.argv[0]))
// 		{
// 			apitest_free_command(&command);
// 			break;
// 		}

// 		if (0 == apitest_string_compare("help", command.argv[0])
// 		 || 0 == apitest_string_compare("h"   , command.argv[0]))
// 		{
// 			print_help();
// 			goto FREE_COMMAND;
// 		}

// 		if (0 == apitest_string_compare("plog_init", command.argv[0]))
// 		{
// 			if (2L == command.argc)
// 			{
// 				(void)fprintf(stdout, "Not enough parameters!\n");
// 				goto FREE_COMMAND;
// 			}

// 			if (3L < command.argc)
// 			{
// 				(void)fprintf(stdout, "Extra parameters will be ignored!\n");
// 			}

// 			error = apitest_string_to_integer(command.argv[2], &terminal_mode_parameter);
// 			switch (error)
// 			{
// 				case E_APITEST_ERROR_NONE:
// 				{
// 					break;
// 				}
// 				case E_APITEST_ERROR_INVALID_CHARACTER: /*< Missed break; on purpose. */
// 				case E_APITEST_ERROR_OUT_OF_RANGE:
// 				{
// 					terminal_mode_parameter = 1LL;
// 					break;
// 				}
// 				case E_APITEST_ERROR_INVALID_PARAMETER: /*< Missed break; on purpose. */
// 				default:
// 				{
// 					(void)fprintf(stdout, "Internal error! (error code: %" PRId32 ")\n", error);
// 					break;
// 				}
// 			}

// 			if (0 == apitest_string_compare("NULL", command.argv[1]))
// 			{
// 				plog_init(NULL, 512ULL, (bool)terminal_mode_parameter); // TODO
// 			}
// 			else
// 			{
// 				plog_init(command.argv[1], 0ULL, (bool)terminal_mode_parameter); // TODO
// 			}
// 			(void)fprintf(stdout, "plog_init = void\n");

// 			goto FREE_COMMAND;
// 		}

// 		if (0 == apitest_string_compare("plog_deinit", command.argv[0]))
// 		{
// 			if (1L < command.argc)
// 			{
// 				(void)fprintf(stdout, "Extra parameters will be ignored!\n");
// 			}

// 			plog_deinit();
// 			(void)fprintf(stdout, "plog_deinit = void\n");

// 			goto FREE_COMMAND;
// 		}

// 		if (0 == apitest_string_compare("plog_set_severity_level", command.argv[0]))
// 		{
// 			if (1L == command.argc)
// 			{
// 				(void)fprintf(stdout, "Not enough parameters!\n");
// 				goto FREE_COMMAND;
// 			}

// 			if (2L < command.argc)
// 			{
// 				(void)fprintf(stdout, "Extra parameters will be ignored!\n");
// 			}

// 			error = apitest_string_to_integer(command.argv[1], &severity_level_parameter);
// 			switch (error)
// 			{
// 				case E_APITEST_ERROR_NONE:
// 				{
// 					plog_set_severity_level((uint8_t)severity_level_parameter);
// 					(void)fprintf(stdout, "plog_set_severity_level = void\n");
// 					break;
// 				}
// 				case E_APITEST_ERROR_INVALID_CHARACTER:
// 				{
// 					(void)fprintf(stdout, "The parameter format is wrong!\n");
// 					break;
// 				}
// 				case E_APITEST_ERROR_OUT_OF_RANGE:
// 				{
// 					(void)fprintf(stdout, "The parameter is out of range!\n");
// 					break;
// 				}
// 				case E_APITEST_ERROR_INVALID_PARAMETER: /*< Missed break; on purpose. */
// 				default:
// 				{
// 					(void)fprintf(stdout, "Internal error! (error code: %" PRId32 ")\n", error);
// 					break;
// 				}
// 			}
// 			goto FREE_COMMAND;
// 		}

// 		if (0 == apitest_string_compare("plog_get_severity_level", command.argv[0]))
// 		{
// 			if (1L < command.argc)
// 			{
// 				(void)fprintf(stdout, "Extra parameters will be ignored!\n");
// 			}

// 			severity_level_return = plog_get_severity_level();
// 			(void)fprintf(stdout, "plog_get_severity_level = %" PRIu8 "\n", severity_level_return);

// 			goto FREE_COMMAND;
// 		}

// 		if (0 == apitest_string_compare("plog_set_terminal_mode", command.argv[0]))
// 		{
// 			if (1L == command.argc)
// 			{
// 				(void)fprintf(stdout, "Not enough parameters!\n");
// 				goto FREE_COMMAND;
// 			}

// 			if (2L < command.argc)
// 			{
// 				(void)fprintf(stdout, "Extra parameters will be ignored!\n");
// 			}

// 			error = apitest_string_to_integer(command.argv[1], &terminal_mode_parameter);
// 			switch (error)
// 			{
// 				case E_APITEST_ERROR_NONE:
// 				{
// 					break;
// 				}
// 				case E_APITEST_ERROR_INVALID_CHARACTER: /*< Missed break; on purpose. */
// 				case E_APITEST_ERROR_OUT_OF_RANGE:
// 				{
// 					terminal_mode_parameter = 1LL;
// 					break;
// 				}
// 				case E_APITEST_ERROR_INVALID_PARAMETER: /*< Missed break; on purpose. */
// 				default:
// 				{
// 					(void)fprintf(stdout, "Internal error! (error code: %" PRId32 ")\n", error);
// 					break;
// 				}
// 			}
// 			plog_set_terminal_mode((bool)terminal_mode_parameter);
// 			(void)fprintf(stdout, "plog_set_terminal_mode = void\n");

// 			goto FREE_COMMAND;
// 		}

// 		if (0 == apitest_string_compare("plog_get_terminal_mode", command.argv[0]))
// 		{
// 			if (1L < command.argc)
// 			{
// 				(void)fprintf(stdout, "Extra parameters will be ignored!\n");
// 			}

// 			terminal_mode_return = plog_get_terminal_mode();
// 			(void)fprintf(stdout, "plog_get_terminal_mode = %s\n", false == terminal_mode_return ? "FALSE" : "TRUE");

// 			goto FREE_COMMAND;
// 		}

// 		if (0 == apitest_string_compare("plog_fatal", command.argv[0]))
// 		{
// 			if (1L == command.argc)
// 			{
// 				(void)fprintf(stdout, "Not enough parameters!\n");
// 				goto FREE_COMMAND;
// 			}

// 			if (2L < command.argc)
// 			{
// 				(void)fprintf(stdout, "Extra parameters will be ignored!\n");
// 			}

// 			plog_fatal(command.argv[1]);
// 			(void)fprintf(stdout, "plog_fatal = void\n");

// 			goto FREE_COMMAND;
// 		}

// 		if (0 == apitest_string_compare("plog_error", command.argv[0]))
// 		{
// 			if (1L == command.argc)
// 			{
// 				(void)fprintf(stdout, "Not enough parameters!\n");
// 				goto FREE_COMMAND;
// 			}

// 			if (2L < command.argc)
// 			{
// 				(void)fprintf(stdout, "Extra parameters will be ignored!\n");
// 			}

// 			plog_error(command.argv[1]);
// 			(void)fprintf(stdout, "plog_error = void\n");

// 			goto FREE_COMMAND;
// 		}

// 		if (0 == apitest_string_compare("plog_warn", command.argv[0]))
// 		{
// 			if (1L == command.argc)
// 			{
// 				(void)fprintf(stdout, "Not enough parameters!\n");
// 				goto FREE_COMMAND;
// 			}

// 			if (2L < command.argc)
// 			{
// 				(void)fprintf(stdout, "Extra parameters will be ignored!\n");
// 			}

// 			plog_warn(command.argv[1]);
// 			(void)fprintf(stdout, "plog_warn = void\n");

// 			goto FREE_COMMAND;
// 		}

// 		if (0 == apitest_string_compare("plog_info", command.argv[0]))
// 		{
// 			if (1L == command.argc)
// 			{
// 				(void)fprintf(stdout, "Not enough parameters!\n");
// 				goto FREE_COMMAND;
// 			}

// 			if (2L < command.argc)
// 			{
// 				(void)fprintf(stdout, "Extra parameters will be ignored!\n");
// 			}

// 			plog_info(command.argv[1]);
// 			(void)fprintf(stdout, "plog_info = void\n");

// 			goto FREE_COMMAND;
// 		}

// 		if (0 == apitest_string_compare("plog_debug", command.argv[0]))
// 		{
// 			if (1L == command.argc)
// 			{
// 				(void)fprintf(stdout, "Not enough parameters!\n");
// 				goto FREE_COMMAND;
// 			}

// 			if (2L < command.argc)
// 			{
// 				(void)fprintf(stdout, "Extra parameters will be ignored!\n");
// 			}

// 			plog_debug(command.argv[1]);
// 			(void)fprintf(stdout, "plog_debug = void\n");

// 			goto FREE_COMMAND;
// 		}

// 		if (0 == apitest_string_compare("plog_trace", command.argv[0]))
// 		{
// 			if (1L == command.argc)
// 			{
// 				(void)fprintf(stdout, "Not enough parameters!\n");
// 				goto FREE_COMMAND;
// 			}

// 			if (2L < command.argc)
// 			{
// 				(void)fprintf(stdout, "Extra parameters will be ignored!\n");
// 			}

// 			plog_trace(command.argv[1]);
// 			(void)fprintf(stdout, "plog_trace = void\n");

// 			goto FREE_COMMAND;
// 		}

// 		if (0 == apitest_string_compare("plog_verbose", command.argv[0]))
// 		{
// 			if (1L == command.argc)
// 			{
// 				(void)fprintf(stdout, "Not enough parameters!\n");
// 				goto FREE_COMMAND;
// 			}

// 			if (2L < command.argc)
// 			{
// 				(void)fprintf(stdout, "Extra parameters will be ignored!\n");
// 			}

// 			plog_verbose(command.argv[1]);
// 			(void)fprintf(stdout, "plog_verbose = void\n");

// 			goto FREE_COMMAND;
// 		}

// 		if (0 == apitest_string_compare("performance_test", command.argv[0]))
// 		{
// 			size_t iterations = 0ULL;
// 			for (; iterations < 3000000ULL; ++iterations)
// 			{
// 				plog_fatal("TEST_TEST_TEST %" PRIu64, iterations);
// 			}
// 			printf("DONE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
// 			return;

// 			goto FREE_COMMAND;
// 		}

// 		(void)fprintf(stdout, "Command is not valid! (press h for help)\n");

// FREE_COMMAND:

// 		(void)fprintf(stdout, "\n");
// 		apitest_free_command(&command);
// 	}
// }

// static void print_versions(void)
// {
// 	const apitest_Version_t apitest_version = apitest_get_version();
// 	const plog_Version_t    plog_version    = plog_get_version();

// 	(void)fprintf(stdout, "Using API-Test %" PRIu8 ".%" PRIu8 ".%" PRIu8 "\n", apitest_version.major, apitest_version.minor, apitest_version.patch);
// 	if (APITEST_VERSION_MAJOR != apitest_version.major
// 	 || APITEST_VERSION_MINOR != apitest_version.minor
// 	 || APITEST_VERSION_PATCH != apitest_version.patch)
// 	{
// 		(void)fprintf(stdout, "Version mismatch! (compiled version: %" PRIu8 ".%" PRIu8 ".%" PRIu8 ")\n", APITEST_VERSION_MAJOR, APITEST_VERSION_MINOR, APITEST_VERSION_PATCH);
// 	}

// 	(void)fprintf(stdout, "Using Plog %" PRIu8 ".%" PRIu8 ".%" PRIu8 "\n", plog_version.major, plog_version.minor, plog_version.patch);
// 	if (PLOG_VERSION_MAJOR != plog_version.major
// 	 || PLOG_VERSION_MINOR != plog_version.minor
// 	 || PLOG_VERSION_PATCH != plog_version.patch)
// 	{
// 		(void)fprintf(stdout, "Version mismatch! (compiled version: %" PRIu8 ".%" PRIu8 ".%" PRIu8 ")\n", PLOG_VERSION_MAJOR, PLOG_VERSION_MINOR, PLOG_VERSION_PATCH);
// 	}
// }

// static void print_usage(void)
// {
// 	(void)fprintf(stdout, "Usage: <executable_name> <path_to_input_file> (optional)\n\n");
// }

// static void print_help(void)
// {
// 	(void)fprintf(stdout, "\tplog_init               <file_name> <terminal_mode>\n");
// 	(void)fprintf(stdout, "\tplog_deinit\n");
// 	(void)fprintf(stdout, "\tplog_set_severity_level <severity_level>\n");
// 	(void)fprintf(stdout, "\tplog_get_severity_level\n");
// 	(void)fprintf(stdout, "\tplog_set_terminal_mode  <terminal_mode>\n");
// 	(void)fprintf(stdout, "\tplog_get_terminal_mode\n");
// 	(void)fprintf(stdout, "\tplog_fatal              <text>\n");
// 	(void)fprintf(stdout, "\tplog_error              <text>\n");
// 	(void)fprintf(stdout, "\tplog_warn               <text>\n");
// 	(void)fprintf(stdout, "\tplog_info               <text>\n");
// 	(void)fprintf(stdout, "\tplog_debug              <text>\n");
// 	(void)fprintf(stdout, "\tplog_trace              <text>\n");
// 	(void)fprintf(stdout, "\tplog_verbose            <text>\n");
// 	(void)fprintf(stdout, "\tperformance_test        <print_count>\n");
// }
