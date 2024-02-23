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
 * @file configuration.h
 * @author Gaina Stefan
 * @date 08.12.2023
 * @brief This file defines function prototypes for reading and writing of plog.conf file. This is
 * used internally by Plog and not meant to be public API.
 * @todo N/A.
 * @bug No known bugs.
 *****************************************************************************************************/

#ifndef INTERNAL_CONFIGURATION_H_
#define INTERNAL_CONFIGURATION_H_

/******************************************************************************************************
 * HEADER FILE INCLUDES
 *****************************************************************************************************/

#include <glib.h>

/******************************************************************************************************
 * MACROS
 *****************************************************************************************************/

/** ***************************************************************************************************
 * @brief The name of the file that is opened to read the persistent configuration (one will be created
 * if not already).
 *****************************************************************************************************/
#define PLOG_CONFIGURATION_FILE_NAME "plog.conf"

/******************************************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/** ***************************************************************************************************
 * @brief Reads the configuration from plog.conf and if the file does not exist create one with
 * default values.
 * @param void
 * @return TRUE - the configuration has been read successfully.
 * @return FALSE - an error occured.
 *****************************************************************************************************/
extern gboolean configuration_read(void);

/** ***************************************************************************************************
 * @brief Writes the potentially updated configuration to the file.
 * @param void
 * @return void
 *****************************************************************************************************/
extern void configuration_write(void);

#ifdef __cplusplus
}
#endif

#endif /*< INTERNAL_CONFIGURATION_H_ */
