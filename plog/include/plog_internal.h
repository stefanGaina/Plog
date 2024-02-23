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
 * @file plog_internal.h
 * @author Gaina Stefan
 * @date 22.06.2023
 * @brief This file defines macros and interfaces of Plog that are meant to be internal.
 * @todo N/A.
 * @bug No known bugs.
 *****************************************************************************************************/

#ifndef PLOG_INTERNAL_H_
#define PLOG_INTERNAL_H_

/******************************************************************************************************
 * HEADER FILE INCLUDES
 *****************************************************************************************************/

#ifndef PLOG_STRIP_ALL

#include <glib.h>

/******************************************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/** ***************************************************************************************************
 * @brief This function is not meant to be called outside plog macros.
 * @param severity_bit: The message will not be logged if the severity bit is not set in severity
 * level mask.
 * @param severity_tag: The tag that will be attached between time and the actual message (indicating
 * the severity of the message).
 * @param function_name: String that contains the name of the caller function.
 * @param format: String that contains the text to be written.
 * @param VA_ARGS: The parameters passed in a printf style (optional).
 * @return void
 *****************************************************************************************************/
extern void plog_internal(guint8 severity_bit, const gchar* severity_tag, const gchar* function_name, const gchar* format, ...);

/** ***************************************************************************************************
 * @brief Performs sanity check and prints an error message if the condition did not pass.
 * @param condition: The condition that needs to be true for the assertion to pass. Otherwise the
 * program will be aborted.
 * @param message: The message that will be printed if the condition did not pass.
 * @param file_name: String that contains the name of the caller file.
 * @param function_name: String that contains the name of the caller function.
 * @param line: The code line where this function is called.
 * @return void
 *****************************************************************************************************/
extern void plog_internal_assert(gboolean condition, const gchar* message, const gchar* file_name, const gchar* function_name, gint32 line);

#ifdef __cplusplus
}
#endif

#endif /*< PLOG_STRIP_ALL */

#endif /*< PLOG_INTERNAL_H_ */
